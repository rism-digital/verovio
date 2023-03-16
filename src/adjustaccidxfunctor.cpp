/////////////////////////////////////////////////////////////////////////////
// Name:        adjustaccidxfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustaccidxfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustAccidXFunctor
//----------------------------------------------------------------------------

AdjustAccidXFunctor::AdjustAccidXFunctor(Doc *doc) : DocFunctor(doc)
{
    m_currentMeasure = NULL;
}

FunctorCode AdjustAccidXFunctor::VisitAlignment(Alignment *alignment)
{
    for (const auto &graceAlignerPair : alignment->GetGraceAligners()) {
        graceAlignerPair.second->Process(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustAccidXFunctor::VisitAlignmentReference(AlignmentReference *alignmentReference)
{
    std::vector<Accid *> &accidSpace = alignmentReference->m_accidSpace;
    if (accidSpace.empty()) return FUNCTOR_SIBLINGS;

    assert(m_doc);
    StaffDef *staffDef = m_doc->GetCurrentScoreDef()->GetStaffDef(alignmentReference->GetN());
    int staffSize = (staffDef && staffDef->HasScale()) ? staffDef->GetScale() : 100;

    std::sort(accidSpace.begin(), accidSpace.end(), AccidSpaceSort());
    // process accid layer alignment
    alignmentReference->SetAccidLayerAlignment();

    // Detect accids which are an octave apart => they will be grouped together in the multiset
    std::multiset<Accid *, AccidOctaveSort> octaveEquivalence;
    std::copy(accidSpace.begin(), accidSpace.end(), std::inserter(octaveEquivalence, octaveEquivalence.begin()));

    std::set<Accid *> adjustedAccids;
    // Align the octaves
    for (Accid *accid : accidSpace) {
        // Skip any accid that was already adjusted
        if (adjustedAccids.count(accid) > 0) continue;
        auto range = octaveEquivalence.equal_range(accid);
        // Handle at least two octave accids without unisons
        int octaveAccidCount = 0;
        std::set<data_OCTAVE> octaves;
        for (auto octaveIter = range.first; octaveIter != range.second; ++octaveIter) {
            Note *note = vrv_cast<Note *>((*octaveIter)->GetFirstAncestor(NOTE));
            octaves.insert(note->GetOct());
            ++octaveAccidCount;
        }
        if ((octaveAccidCount < 2) || ((int)octaves.size() < octaveAccidCount)) continue;
        // Now adjust the octave accids and store the left most position
        int minDrawingX = -VRV_UNSET;
        for (auto octaveIter = range.first; octaveIter != range.second; ++octaveIter) {
            alignmentReference->AdjustAccidWithAccidSpace(*octaveIter, m_doc, staffSize, adjustedAccids);
            minDrawingX = std::min(minDrawingX, (*octaveIter)->GetDrawingX());
        }
        // Finally, align the accidentals whenever the adjustment is not too large
        for (auto octaveIter = range.first; octaveIter != range.second; ++octaveIter) {
            const int dist = (*octaveIter)->GetDrawingX() - minDrawingX;
            if ((dist > 0) && (*octaveIter)->HasContentHorizontalBB()) {
                const int accidWidth = (*octaveIter)->GetContentRight() - (*octaveIter)->GetContentLeft();
                if (dist < accidWidth / 2) {
                    (*octaveIter)->SetDrawingXRel((*octaveIter)->GetDrawingXRel() - dist);
                }
            }
        }
    }

    // Align accidentals for unison notes if any of them are present
    for (Accid *accid : accidSpace) {
        if (accid->GetDrawingUnisonAccid() == NULL) continue;
        accid->SetDrawingXRel(accid->GetDrawingUnisonAccid()->GetDrawingXRel());
    }

    const int count = (int)accidSpace.size();
    const int middle = (count / 2) + (count % 2);
    // Zig-zag processing
    for (int i = 0, j = count - 1; i < middle; ++i, --j) {
        // top one - but skip if already adjusted (i.e. octaves)
        if (adjustedAccids.count(accidSpace.at(i)) == 0) {
            alignmentReference->AdjustAccidWithAccidSpace(accidSpace.at(i), m_doc, staffSize, adjustedAccids);
        }

        // Break with odd number of elements once the middle is reached
        if (i == j) break;

        // bottom one - but skip if already adjusted
        if (adjustedAccids.count(accidSpace.at(j)) == 0) {
            alignmentReference->AdjustAccidWithAccidSpace(accidSpace.at(j), m_doc, staffSize, adjustedAccids);
        }
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustAccidXFunctor::VisitMeasure(Measure *measure)
{
    m_currentMeasure = measure;

    measure->m_measureAligner.Process(*this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
