/////////////////////////////////////////////////////////////////////////////
// Name:        calcarticfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcarticfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "layer.h"
#include "staff.h"
#include "stem.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcArticFunctor
//----------------------------------------------------------------------------

CalcArticFunctor::CalcArticFunctor(Doc *doc) : DocFunctor(doc)
{
    m_parent = NULL;
    m_stemDir = STEMDIRECTION_NONE;
    m_staffAbove = NULL;
    m_staffBelow = NULL;
    m_layerAbove = NULL;
    m_layerBelow = NULL;
    m_crossStaffAbove = false;
    m_crossStaffBelow = false;
}

FunctorCode CalcArticFunctor::VisitArtic(Artic *artic)
{
    if (!m_parent) return FUNCTOR_CONTINUE;

    /************** placement **************/

    Layer *layer = vrv_cast<Layer *>(artic->GetFirstAncestor(LAYER));
    assert(layer);

    if (m_parent->m_crossLayer) {
        layer = m_parent->m_crossLayer;
    }

    bool allowAbove = true;
    data_STEMDIRECTION layerStemDir;

    // for now we ignore within @place
    if (artic->GetPlace() != STAFFREL_NONE) {
        artic->SetDrawingPlace(artic->GetPlace());
        // if we have a place indication do not allow to be changed to above
        allowAbove = false;
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(m_parent)) != STEMDIRECTION_NONE) {
        artic->SetDrawingPlace((layerStemDir == STEMDIRECTION_up) ? STAFFREL_above : STAFFREL_below);
        // If we have more than one layer do not allow to be changed to above
        allowAbove = false;
    }
    else if (m_stemDir == STEMDIRECTION_up) {
        artic->SetDrawingPlace(STAFFREL_below);
    }
    else {
        artic->SetDrawingPlace(STAFFREL_above);
    }

    // Not sure what this is anymore...
    if (artic->IsOutsideArtic()) {
        // If allowAbove is true it will place the artic above if the content requires so (even if place below is given)
        if ((artic->GetDrawingPlace() == STAFFREL_below) && allowAbove && (artic->AlwaysAbove())) {
            artic->SetDrawingPlace(STAFFREL_above);
        }
    }

    /************** adjust the xRel position **************/

    Stem *stem = vrv_cast<Stem *>(m_parent->FindDescendantByType(STEM));
    artic->SetDrawingXRel(this->CalculateHorizontalShift(artic, stem->IsVirtual()));

    /************** set cross-staff / layer **************/

    // Exception for artic because they are relative to the staff - we set m_crossStaff and m_crossLayer
    if ((artic->GetDrawingPlace() == STAFFREL_above) && m_crossStaffAbove) {
        artic->m_crossStaff = m_staffAbove;
        artic->m_crossLayer = m_layerAbove;
    }
    else if ((artic->GetDrawingPlace() == STAFFREL_below) && m_crossStaffBelow) {
        artic->m_crossStaff = m_staffBelow;
        artic->m_crossLayer = m_layerBelow;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcArticFunctor::VisitChord(Chord *chord)
{
    m_parent = chord;
    m_stemDir = chord->GetDrawingStemDir();

    Staff *staff = chord->GetAncestorStaff();
    Layer *layer = vrv_cast<Layer *>(chord->GetFirstAncestor(LAYER));
    assert(layer);

    m_staffAbove = staff;
    m_staffBelow = staff;
    m_layerAbove = layer;
    m_layerBelow = layer;
    m_crossStaffAbove = false;
    m_crossStaffBelow = false;

    if (chord->m_crossStaff) {
        m_staffAbove = chord->m_crossStaff;
        m_staffBelow = chord->m_crossStaff;
        m_layerAbove = chord->m_crossLayer;
        m_layerBelow = chord->m_crossLayer;
        m_crossStaffAbove = true;
        m_crossStaffBelow = true;
    }
    else {
        chord->GetCrossStaffExtremes(m_staffAbove, m_staffBelow, &m_layerAbove, &m_layerBelow);
        if (m_staffAbove) {
            m_crossStaffAbove = true;
            m_staffBelow = staff;
            m_layerBelow = layer;
        }
        else if (m_staffBelow) {
            m_crossStaffBelow = true;
            m_staffAbove = staff;
            m_layerAbove = layer;
        }
    }

    this->IncludeBeamStaff(chord);

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcArticFunctor::VisitNote(Note *note)
{
    if (note->IsChordTone()) return FUNCTOR_CONTINUE;

    m_parent = note;
    m_stemDir = note->GetDrawingStemDir();

    Staff *staff = note->GetAncestorStaff();
    Layer *layer = vrv_cast<Layer *>(note->GetFirstAncestor(LAYER));
    assert(layer);

    m_staffAbove = staff;
    m_staffBelow = staff;
    m_layerAbove = layer;
    m_layerBelow = layer;
    m_crossStaffAbove = false;
    m_crossStaffBelow = false;

    if (note->m_crossStaff) {
        m_staffAbove = note->m_crossStaff;
        m_staffBelow = note->m_crossStaff;
        m_layerAbove = note->m_crossLayer;
        m_layerBelow = note->m_crossLayer;
        m_crossStaffAbove = true;
        m_crossStaffBelow = true;
    }

    this->IncludeBeamStaff(note);

    return FUNCTOR_CONTINUE;
}

int CalcArticFunctor::CalculateHorizontalShift(const Artic *artic, bool virtualStem) const
{
    int shift = m_parent->GetDrawingRadius(m_doc);
    if (virtualStem || (m_parent->GetChildCount(ARTIC) > 1) || (m_doc->GetOptions()->m_staccatoCenter.GetValue())) {
        return shift;
    }
    switch (artic->GetArticFirst()) {
        case ARTICULATION_stacc:
        case ARTICULATION_stacciss: {
            const Staff *staff = artic->GetAncestorStaff();
            const int stemWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            if ((m_stemDir == STEMDIRECTION_up) && (artic->GetDrawingPlace() == STAFFREL_above)) {
                shift += shift - stemWidth / 2;
            }
            else if ((m_stemDir == STEMDIRECTION_down) && (artic->GetDrawingPlace() == STAFFREL_below)) {
                shift = stemWidth / 2;
            }
            break;
        }
        default: {
            break;
        }
    }

    return shift;
}

void CalcArticFunctor::IncludeBeamStaff(LayerElement *layerElement)
{
    if (Beam *beam = layerElement->GetAncestorBeam(); beam) {
        if (m_crossStaffAbove && (beam->m_drawingPlace == BEAMPLACE_above)) {
            m_staffAbove = beam->GetAncestorStaff(RESOLVE_CROSS_STAFF);
        }
        else if (m_crossStaffBelow && (beam->m_drawingPlace == BEAMPLACE_below)) {
            m_staffBelow = beam->GetAncestorStaff(RESOLVE_CROSS_STAFF);
        }
    }
}

} // namespace vrv
