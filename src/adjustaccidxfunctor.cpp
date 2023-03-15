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

    // Detect the octave and mark them
    std::vector<Accid *>::iterator iter, octaveIter;
    for (iter = accidSpace.begin(); iter != accidSpace.end() - 1; ++iter) {
        Note *note = vrv_cast<Note *>((*iter)->GetFirstAncestor(NOTE));
        assert(note);
        if (!note) continue;
        for (octaveIter = iter + 1; octaveIter != accidSpace.end(); ++octaveIter) {
            Note *octave = vrv_cast<Note *>((*octaveIter)->GetFirstAncestor(NOTE));
            assert(octave);
            if (!octave) continue;
            bool sameChordOctave = true;
            if (Chord *chord = vrv_cast<Chord *>((*iter)->GetFirstAncestor(CHORD)); chord != NULL) {
                if ((*octaveIter)->GetFirstAncestor(CHORD) != chord) sameChordOctave = false;
            }
            // Same pitch, different octave, same accid - for now?
            if ((note->GetPname() == octave->GetPname()) && (note->GetOct() != octave->GetOct())
                && ((*iter)->GetAccid() == (*octaveIter)->GetAccid()) && sameChordOctave) {
                (*iter)->SetDrawingOctaveAccid(*octaveIter);
                (*octaveIter)->SetDrawingOctave(true);
            }
        }
    }

    int count = (int)accidSpace.size();

    std::vector<Accid *> adjustedAccids;
    // Align the octaves
    for (int i = 0; i < count - 1; ++i) {
        if (accidSpace.at(i)->GetDrawingOctaveAccid() != NULL) {
            alignmentReference->AdjustAccidWithAccidSpace(accidSpace.at(i), m_doc, staffSize, adjustedAccids);
            alignmentReference->AdjustAccidWithAccidSpace(
                accidSpace.at(i)->GetDrawingOctaveAccid(), m_doc, staffSize, adjustedAccids);
            int dist = accidSpace.at(i)->GetDrawingX() - accidSpace.at(i)->GetDrawingOctaveAccid()->GetDrawingX();
            if (dist > 0)
                accidSpace.at(i)->SetDrawingXRel(accidSpace.at(i)->GetDrawingXRel() - dist);
            else if (dist < 0)
                accidSpace.at(i)->GetDrawingOctaveAccid()->SetDrawingXRel(
                    accidSpace.at(i)->GetDrawingOctaveAccid()->GetDrawingXRel() + dist);
        }
    }

    // Align accidentals for unison notes if any of them are present
    for (Accid *accid : accidSpace) {
        if (accid->GetDrawingUnisonAccid() == NULL) continue;
        accid->SetDrawingXRel(accid->GetDrawingUnisonAccid()->GetDrawingXRel());
    }

    int middle = (count % 2) ? (count / 2) + 1 : (count / 2);
    // Zig-zag processing
    for (int i = 0, j = count - 1; i < middle; ++i, --j) {
        // top one - but skip octaves
        if (!accidSpace.at(j)->GetDrawingOctaveAccid() && !accidSpace.at(j)->GetDrawingOctave())
            alignmentReference->AdjustAccidWithAccidSpace(accidSpace.at(j), m_doc, staffSize, adjustedAccids);

        // Break with odd number of elements once the middle is reached
        if (i == j) break;

        // bottom one - but skip octaves
        if (!accidSpace.at(i)->GetDrawingOctaveAccid() && !accidSpace.at(i)->GetDrawingOctave())
            alignmentReference->AdjustAccidWithAccidSpace(accidSpace.at(i), m_doc, staffSize, adjustedAccids);
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
