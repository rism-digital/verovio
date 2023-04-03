/////////////////////////////////////////////////////////////////////////////
// Name:        calcbboxoverflowsfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcbboxoverflowsfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "layer.h"
#include "staff.h"
#include "verticalaligner.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcBBoxOverflowsFunctor
//----------------------------------------------------------------------------

CalcBBoxOverflowsFunctor::CalcBBoxOverflowsFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcBBoxOverflowsFunctor::VisitLayerEnd(Layer *layer)
{
    // set scoreDef attr
    if (layer->GetCautionStaffDefClef()) {
        this->VisitClef(layer->GetCautionStaffDefClef());
    }
    if (layer->GetCautionStaffDefKeySig()) {
        this->VisitKeySig(layer->GetCautionStaffDefKeySig());
    }
    if (layer->GetCautionStaffDefMensur()) {
        this->VisitMensur(layer->GetCautionStaffDefMensur());
    }
    if (layer->GetCautionStaffDefMeterSig()) {
        this->VisitMeterSig(layer->GetCautionStaffDefMeterSig());
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcBBoxOverflowsFunctor::VisitObject(Object *object)
{
    // starting a new staff
    if (object->Is(STAFF)) {
        Staff *currentStaff = vrv_cast<Staff *>(object);
        assert(currentStaff);

        if (!currentStaff->DrawingIsVisible()) {
            return FUNCTOR_SIBLINGS;
        }
        return FUNCTOR_CONTINUE;
    }

    // starting new layer
    if (object->Is(LAYER)) {
        Layer *currentLayer = vrv_cast<Layer *>(object);
        assert(currentLayer);
        // set scoreDef attr
        if (currentLayer->GetStaffDefClef()) {
            // System scoreDef clefs are taken into account but treated separately (see below)
            this->VisitClef(currentLayer->GetStaffDefClef());
        }
        if (currentLayer->GetStaffDefKeySig()) {
            this->VisitKeySig(currentLayer->GetStaffDefKeySig());
        }
        if (currentLayer->GetStaffDefMensur()) {
            this->VisitMensur(currentLayer->GetStaffDefMensur());
        }
        if (currentLayer->GetStaffDefMeterSig()) {
            this->VisitMeterSig(currentLayer->GetStaffDefMeterSig());
        }
        return FUNCTOR_CONTINUE;
    }

    if (object->IsSystemElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (object->IsControlElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (!object->IsLayerElement()) {
        return FUNCTOR_CONTINUE;
    }

    // Take into account beam in cross-staff situation
    if (object->Is(BEAM)) {
        Beam *beam = vrv_cast<Beam *>(object);
        assert(beam);
        // Ignore it if it has cross-staff content but is not entirely cross-staff itself
        if (beam->m_crossStaffContent && !beam->m_crossStaff) return FUNCTOR_CONTINUE;
    }

    // Take into account stem for notes in cross-staff situation and in beams
    if (object->Is(STEM)) {
        LayerElement *noteOrChord = dynamic_cast<LayerElement *>(object->GetParent());
        if (noteOrChord && noteOrChord->m_crossStaff) {
            if (noteOrChord->GetAncestorBeam()) {
                Beam *beam = vrv_cast<Beam *>(noteOrChord->GetFirstAncestor(BEAM));
                assert(beam);
                // Ignore it but only if the beam is not entirely cross-staff itself
                if (!beam->m_crossStaff) return FUNCTOR_CONTINUE;
            }
            else if (noteOrChord->GetIsInBeamSpan()) {
                return FUNCTOR_CONTINUE;
            }
        }
    }

    if (object->Is(FB) || object->Is(FIGURE)) {
        return FUNCTOR_CONTINUE;
    }

    if (object->Is(SYL)) {
        // We don't want to add the syl to the overflow since lyrics require a full line anyway
        return FUNCTOR_CONTINUE;
    }

    if (!object->HasSelfBB()) {
        // if nothing was drawn, do not take it into account
        return FUNCTOR_CONTINUE;
    }

    LayerElement *current = vrv_cast<LayerElement *>(object);
    assert(current);

    StaffAlignment *above = NULL;
    StaffAlignment *below = NULL;
    current->GetOverflowStaffAlignments(above, below);

    bool isScoreDefClef = false;
    // Exception for the scoreDef clef where we do not want to take into account the general overflow
    // We have instead distinct members in StaffAlignment to store them
    if (current->Is(CLEF) && (current->GetScoreDefRole() == SCOREDEF_SYSTEM)) {
        isScoreDefClef = true;
    }

    if (above) {
        int overflowAbove = above->CalcOverflowAbove(current);
        int staffSize = above->GetStaffSize();
        if (overflowAbove > m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
            if (isScoreDefClef) {
                above->SetScoreDefClefOverflowAbove(overflowAbove);
            }
            else {
                above->SetOverflowAbove(overflowAbove);
            }
            above->AddBBoxAbove(current);
        }
    }

    if (below) {
        int overflowBelow = below->CalcOverflowBelow(current);
        int staffSize = below->GetStaffSize();
        if (overflowBelow > m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
            if (isScoreDefClef) {
                below->SetScoreDefClefOverflowBelow(overflowBelow);
            }
            else {
                below->SetOverflowBelow(overflowBelow);
            }
            below->AddBBoxBelow(current);
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
