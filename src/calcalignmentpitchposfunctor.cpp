/////////////////////////////////////////////////////////////////////////////
// Name:        calcalignmentpitchposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcalignmentpitchposfunctor.h"

//----------------------------------------------------------------------------

#include "custos.h"
#include "doc.h"
#include "layer.h"
#include "mrest.h"
#include "rest.h"
#include "staff.h"
#include "tuning.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcAlignmentPitchPosFunctor
//----------------------------------------------------------------------------

CalcAlignmentPitchPosFunctor::CalcAlignmentPitchPosFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcAlignmentPitchPosFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    LayerElement *layerElementY = layerElement;
    Staff *staffY = layerElement->GetAncestorStaff();
    Layer *layerY = vrv_cast<Layer *>(layerElement->GetFirstAncestor(LAYER));
    assert(layerY);

    if (layerElement->m_crossStaff && layerElement->m_crossLayer) {
        layerElementY = layerElement->m_crossLayer->GetAtPos(layerElement->GetDrawingX());
        staffY = layerElement->m_crossStaff;
        layerY = layerElement->m_crossLayer;
    }

    // Adjust m_drawingYRel for notes and rests, etc.
    if (layerElement->Is(ACCID)) {
        Accid *accid = vrv_cast<Accid *>(layerElement);
        assert(accid);
        Note *note = vrv_cast<Note *>(accid->GetFirstAncestor(NOTE));
        // We should probably also avoid to add editorial accidentals to the accid space
        // However, since they are placed above by View::DrawNote it works without avoiding it
        if (note) {
            if (note->HasGraceAlignment()) {
                note->GetGraceAlignment()->AddToAccidSpace(accid);
            }
            else {
                accid->GetAlignment()->AddToAccidSpace(accid);
            }
        }
        else if (accid->GetFirstAncestor(CUSTOS)) {
            accid->GetAlignment()->AddToAccidSpace(
                accid); // If this is not added, the accidental is drawn an octave below the custos
        }
        else {
            // do something for accid that are not children of a note - e.g., mensural?
            accid->SetDrawingYRel(staffY->CalcPitchPosYRel(m_doc, accid->CalcDrawingLoc(layerY, layerElementY)));
        }
        // override if staff position is set explicitly
        if (accid->HasPloc() && accid->HasOloc()) {
            accid->SetDrawingLoc(
                PitchInterface::CalcLoc(accid->GetPloc(), accid->GetOloc(), layerY->GetClefLocOffset(layerElementY)));
            accid->SetDrawingYRel(staffY->CalcPitchPosYRel(m_doc, accid->GetDrawingLoc()));
        }
        else if (accid->HasLoc()) {
            accid->SetDrawingLoc(accid->GetLoc());
            accid->SetDrawingYRel(staffY->CalcPitchPosYRel(m_doc, accid->GetLoc()));
        }
    }
    else if (layerElement->Is(CHORD)) {
        // The y position is set to the top note one
        int loc = PitchInterface::CalcLoc(layerElement, layerY, layerElementY, true);
        layerElement->SetDrawingYRel(staffY->CalcPitchPosYRel(m_doc, loc));
    }
    else if (layerElement->Is(DOT)) {
        PositionInterface *interface = layerElement->GetPositionInterface();
        assert(interface);
        layerElement->SetDrawingYRel(staffY->CalcPitchPosYRel(m_doc, interface->CalcDrawingLoc(layerY, layerElementY)));
    }
    else if (layerElement->Is(CUSTOS)) {
        Custos *custos = vrv_cast<Custos *>(layerElement);
        assert(custos);
        int loc = 0;
        if (custos->HasPname()) {
            loc = PitchInterface::CalcLoc(custos, layerY, layerElementY);
        }
        int yRel = staffY->CalcPitchPosYRel(m_doc, loc);
        custos->SetDrawingLoc(loc);
        custos->SetDrawingYRel(yRel);
    }
    else if (layerElement->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(layerElement);
        assert(note);
        Chord *chord = note->IsChordTone();
        int loc = 0;
        TabGrp *tabGrp = note->IsTabGrpNote();
        if (tabGrp) {
            assert(staffY->m_drawingTuning);
            loc = staffY->m_drawingTuning->CalcPitchPos(
                note->GetTabCourse(), staffY->m_drawingNotationType, staffY->m_drawingLines);
        }
        else if ((note->HasPname() && note->HasOct()) || note->HasLoc()) {
            loc = PitchInterface::CalcLoc(note, layerY, layerElementY);
        }
        int yRel = staffY->CalcPitchPosYRel(m_doc, loc);
        // Make it relative to the top note one (see above) but not for cross-staff notes in chords
        if (chord && !note->m_crossStaff) {
            yRel -= chord->GetDrawingYRel();
        }
        note->SetDrawingLoc(loc);
        note->SetDrawingYRel(yRel);
    }
    else if (layerElement->Is(MREST)) {
        MRest *mRest = vrv_cast<MRest *>(layerElement);
        assert(mRest);
        int loc = 0;
        if (mRest->HasPloc() && mRest->HasOloc()) {
            loc = PitchInterface::CalcLoc(mRest->GetPloc(), mRest->GetOloc(), layerY->GetClefLocOffset(layerElementY));
        }
        else if (mRest->HasLoc()) {
            loc = mRest->GetLoc();
        }
        // Automatically calculate rest position
        else {
            // set default location to the middle of the staff
            Staff *staff = mRest->GetAncestorStaff();
            loc = staff->m_drawingLines - 1;
            if (loc % 2 != 0) --loc;
            if (staff->m_drawingLines > 1) loc += 2;
            // Limitation: GetLayerCount does not take into account editorial markup
            // should be refined later
            bool hasMultipleLayer = (staffY->GetChildCount(LAYER) > 1);
            if (hasMultipleLayer) {
                loc = mRest->GetOptimalLayerLocation(layerY, loc);
            }
        }

        mRest->SetDrawingLoc(loc);
        mRest->SetDrawingYRel(staffY->CalcPitchPosYRel(m_doc, loc));
    }
    else if (layerElement->Is(REST)) {
        Rest *rest = vrv_cast<Rest *>(layerElement);
        assert(rest);
        int loc = 0;
        if (rest->HasPloc() && rest->HasOloc()) {
            loc = PitchInterface::CalcLoc(rest->GetPloc(), rest->GetOloc(), layerY->GetClefLocOffset(layerElementY));
        }
        else if (rest->HasLoc()) {
            loc = rest->GetLoc();
        }
        // Automatically calculate rest position
        else {
            // set default location to the middle of the staff
            Staff *staff = rest->GetAncestorStaff();
            loc = staff->m_drawingLines - 1;
            if ((rest->GetDur() < DUR_4) && (loc % 2 != 0)) --loc;
            // Adjust special cases
            if ((rest->GetDur() == DUR_1) && (staff->m_drawingLines > 1)) loc += 2;
            if ((rest->GetDur() == DUR_BR) && (staff->m_drawingLines < 2)) loc -= 2;

            // If within a beam, calculate the rest's height based on it's relationship to the notes that surround it
            Beam *beam = vrv_cast<Beam *>(rest->GetFirstAncestor(BEAM, 1));
            if (beam) {
                beam->ResetList(beam);

                const ListOfObjects &beamList = beam->GetList(beam);
                const int restIndex = beam->GetListIndex(rest);
                assert(restIndex >= 0);

                int leftLoc = loc;
                ListOfObjects::const_iterator it = beamList.begin();
                std::advance(it, restIndex);
                ListOfObjects::const_reverse_iterator rit(it);
                // iterate through the elements from the rest to the beginning of the beam
                // until we hit a note or chord, which we will use to determine where the rest should be placed
                for (; rit != beamList.rend(); ++rit) {
                    LayerElement *beamElement = vrv_cast<LayerElement *>(*rit);
                    assert(beamElement);
                    if (beamElement->Is(NOTE)) {
                        leftLoc = PitchInterface::CalcLoc(beamElement, layerY, layerElementY);
                        break;
                    }
                    else if (beamElement->Is(CHORD)) {
                        int topChordLoc = PitchInterface::CalcLoc(beamElement, layerY, layerElementY, true);
                        int bottomChordLoc = PitchInterface::CalcLoc(beamElement, layerY, layerElementY, false);
                        // if it's a rest, use the middle of the chord as the rest's location
                        leftLoc = (topChordLoc + bottomChordLoc) / 2;
                        break;
                    }
                }

                int rightLoc = loc;
                it = beamList.begin();
                std::advance(it, restIndex);
                // iterate through the elements from the rest to the end of the beam
                // until we hit a note or chord, which we will use to determine where the rest should be placed
                for (; it != beamList.end(); ++it) {
                    LayerElement *beamElement = vrv_cast<LayerElement *>(*it);
                    assert(beamElement);
                    if (beamElement->Is(NOTE)) {
                        rightLoc = PitchInterface::CalcLoc(beamElement, layerY, layerElementY);
                        break;
                    }
                    else if (beamElement->Is(CHORD)) {
                        int topChordLoc = PitchInterface::CalcLoc(beamElement, layerY, layerElementY, true);
                        int bottomChordLoc = PitchInterface::CalcLoc(beamElement, layerY, layerElementY, false);
                        // if it's a rest, use the middle of the chord as the rest's location
                        rightLoc = (topChordLoc + bottomChordLoc) / 2;
                        break;
                    }
                }

                // average the left note and right note's locations together to get our rest location
                const int locAvg = (rightLoc + leftLoc) / 2;
                if (abs(locAvg - loc) > 3) {
                    loc = locAvg;
                }

                // note: bottomAlignedLoc and topAlignedLoc are only accounting for discrepencies
                // between 8th, 16th and 32nd notes, not 64th's and on
                // I've described how to implement 64ths and beyond below

                // we need to check for bottom and top alignment because a 32nd rest that's top is in the space
                // under the staff (d4 on treble) can not be moved any closer to center by an increment of 1
                // because the dots will collide with the staff
                // whereas a 16th rest that is in the same "loc" as the 32nd is actually below the 32nd
                // (the top of the 16th will be in note b3 on treble clef) so can be moved closer to the staff
                // than the 32nd without fear of the dots colliding with the staff lines

                // bottomAlignedLoc is the location where all of the rest's stems align to form a straight line
                int bottomAlignedLoc = loc;
                // 8th note rests are aligned with the top of a 16th note rest, so to bottom align we have to push it
                // down 2
                if (rest->GetActualDur() == DURATION_8) bottomAlignedLoc -= 2;
                // for durations smaller than 32nd, bottomAlignedLoc will need to decrease by 2 every iteration greater
                // than from 32 so 32 will be -0, 64 is -2, 128 is -4 (currently not implemented)

                // topAlignedLoc is the location where all of the top of the rests align to form a straight line
                int topAlignedLoc = loc;
                if (rest->GetActualDur() == DURATION_32) topAlignedLoc += 2;
                // for smaller durations, topAlignedLoc offset will increase by 2 every iteration greater than from 32
                // so 32 will need to be +2, 64 is +4, 128 is +6, etc.
                // (currently only implemented for 32nds)

                const int topOfStaffLoc = 10;
                const int bottomOfStaffLoc = -4;

                // move the extrema towards center a little for aesthetic reasons
                const bool restAboveStaff = bottomAlignedLoc >= topOfStaffLoc;
                const bool restBelowStaff = topAlignedLoc <= bottomOfStaffLoc;
                if (restAboveStaff) {
                    loc--;
                }
                else if (restBelowStaff) {
                    loc++;
                }

                // if loc is odd, we need to offset it to be even
                // so that the dots do not collide with the staff lines
                // or on ledger lines
                if (loc % 2 != 0) {
                    // if it's above the staff, offset downwards
                    // if below the staff, offset upwards
                    if (loc > 4) {
                        loc--;
                    }
                    else {
                        loc++;
                    }
                }
            }

            Layer *layer = vrv_cast<Layer *>(layerElement->GetFirstAncestor(LAYER));
            loc = rest->GetOptimalLayerLocation(staff, layer, loc);
        }
        rest->SetDrawingLoc(loc);
        rest->SetDrawingYRel(staffY->CalcPitchPosYRel(m_doc, loc));
    }
    else if (layerElement->Is(TABDURSYM)) {
        int yRel = 0;
        if (staffY->IsTabWithStemsOutside()) {
            double spacingRatio = (staffY->IsTabLuteFrench()) ? 2.0 : 1.0;
            yRel += m_doc->GetDrawingUnit(staffY->m_drawingStaffSize) * spacingRatio;
        }
        layerElement->SetDrawingYRel(yRel);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
