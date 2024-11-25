/////////////////////////////////////////////////////////////////////////////
// Name:        adjustaccidxfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustaccidxfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "score.h"

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
    m_adjustedAccids.clear();

    std::vector<Accid *> accids = this->GetAccidentalsForAdjustment(alignmentReference);
    if (accids.empty()) return FUNCTOR_SIBLINGS;

    assert(m_doc);
    ScoreDef *scoreDef = m_doc->GetCorrespondingScore(alignmentReference)->GetScoreDef();
    StaffDef *staffDef = scoreDef->GetStaffDef(alignmentReference->GetN());
    int staffSize = (staffDef && staffDef->HasScale()) ? staffDef->GetScale() : 100;

    std::sort(accids.begin(), accids.end(), AccidSpaceSort());
    // process accid layer alignment
    for (Accid *accid : accids) {
        this->SetAccidLayerAlignment(accid, alignmentReference);
    }

    // Detect accids which are an octave apart => they will be grouped together in the multiset
    std::multiset<Accid *, AccidOctaveSort> octaveEquivalence;
    std::copy(accids.begin(), accids.end(), std::inserter(octaveEquivalence, octaveEquivalence.begin()));

    // Align the octaves
    for (Accid *accid : accids) {
        // Skip any accid that was already adjusted
        if (m_adjustedAccids.contains(accid)) continue;
        // Skip accid not descendant of a note (e.g., mensural)
        if (!accid->GetFirstAncestor(NOTE)) continue;

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
            this->AdjustAccidWithSpace(*octaveIter, alignmentReference, staffSize);
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
    for (Accid *accid : accids) {
        if (accid->GetDrawingUnisonAccid() == NULL) continue;
        accid->SetDrawingXRel(accid->GetDrawingUnisonAccid()->GetDrawingXRel());
    }

    const int count = (int)accids.size();
    const int middle = (count / 2) + (count % 2);
    // Zig-zag processing
    for (int i = 0, j = count - 1; i < middle; ++i, --j) {
        // top one - but skip if already adjusted (i.e. octaves)
        if (!m_adjustedAccids.contains(accids.at(i))) {
            this->AdjustAccidWithSpace(accids.at(i), alignmentReference, staffSize);
        }

        // Break with odd number of elements once the middle is reached
        if (i == j) break;

        // bottom one - but skip if already adjusted
        if (!m_adjustedAccids.contains(accids.at(j))) {
            this->AdjustAccidWithSpace(accids.at(j), alignmentReference, staffSize);
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

std::vector<Accid *> AdjustAccidXFunctor::GetAccidentalsForAdjustment(AlignmentReference *alignmentReference) const
{
    std::vector<Accid *> accidentals;
    for (Object *child : alignmentReference->GetChildren()) {
        if (child->Is(ACCID)) {
            Accid *accid = vrv_cast<Accid *>(child);
            if (accid->HasAccid() && accid->GetFirstAncestor(NOTE)) accidentals.push_back(accid);
        }
    }
    return accidentals;
}

void AdjustAccidXFunctor::SetAccidLayerAlignment(Accid *accid, const AlignmentReference *alignmentReference) const
{
    if (accid->IsAlignedWithSameLayer()) return;

    const ArrayOfConstObjects &children = alignmentReference->GetChildren();
    Note *parentNote = vrv_cast<Note *>(accid->GetFirstAncestor(NOTE));
    const bool hasUnisonOverlap = std::any_of(children.begin(), children.end(), [parentNote](const Object *object) {
        if (!object->Is(NOTE)) return false;
        const Note *otherNote = vrv_cast<const Note *>(object);
        // in case notes are in unison but have different accidentals
        return parentNote && parentNote->IsUnisonWith(otherNote, true) && !parentNote->IsUnisonWith(otherNote, false);
    });

    if (!hasUnisonOverlap) return;

    Chord *chord = parentNote->IsChordTone();
    // no chord, so align only parent note
    if (!chord) {
        accid->IsAlignedWithSameLayer(true);
        return;
    }
    // we have chord ancestor, so need to align all of its accidentals
    ListOfObjects accidentals = chord->FindAllDescendantsByType(ACCID);
    std::for_each(accidentals.begin(), accidentals.end(), [](Object *object) {
        Accid *accid = vrv_cast<Accid *>(object);
        accid->IsAlignedWithSameLayer(true);
    });
}

void AdjustAccidXFunctor::AdjustAccidWithSpace(Accid *accid, AlignmentReference *alignmentReference, int staffSize)
{
    std::vector<Accid *> leftAccids;
    const ArrayOfObjects &children = alignmentReference->GetChildren();

    // bottom one
    for (Object *child : children) {
        // if accidental has unison overlap, ignore elements on other layers for overlap
        if (accid->IsAlignedWithSameLayer() && (accid->GetFirstAncestor(LAYER) != child->GetFirstAncestor(LAYER)))
            continue;
        accid->AdjustX(dynamic_cast<LayerElement *>(child), m_doc, staffSize, leftAccids, m_adjustedAccids);
    }

    // Mark as adjusted (even if position was not altered)
    m_adjustedAccids.insert(accid);
}

} // namespace vrv
