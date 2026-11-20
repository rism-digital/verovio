/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxrelfortranscriptionfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustxrelfortranscriptionfunctor.h"

//----------------------------------------------------------------------------

#include "clef.h"
#include "doc.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "metersiggrp.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXRelForTranscriptionFunctor
//----------------------------------------------------------------------------

AdjustXRelForTranscriptionFunctor::AdjustXRelForTranscriptionFunctor() : Functor() {}

FunctorCode AdjustXRelForTranscriptionFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->m_drawingFacsX == VRV_UNSET) return FUNCTOR_CONTINUE;

    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (!layerElement->HasSelfBB()) return FUNCTOR_CONTINUE;

    layerElement->SetDrawingXRel(-layerElement->GetSelfX1());

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXRelForTranscriptionFunctor::VisitMeasure(Measure *measure)
{
    // Neume lines have no barlines
    if (measure->IsNeumeLine()) return FUNCTOR_CONTINUE;
    if ((measure->m_drawingFacsX1 == VRV_UNSET) || (measure->m_drawingFacsX2 == VRV_UNSET)) return FUNCTOR_CONTINUE;

    // The measure barlines are not children of the measure and have no zone: place them at the edges of the measure
    // zone. The right barline is drawn from its left, so its width is subtracted for it to end at the right edge.
    const Doc *doc = vrv_cast<const Doc *>(measure->GetFirstAncestor(DOC));
    assert(doc);
    measure->GetLeftBarLine()->m_drawingFacsX = measure->m_drawingFacsX1;
    measure->GetRightBarLine()->m_drawingFacsX
        = measure->m_drawingFacsX2 - measure->CalculateRightBarLineWidth(doc, 100);

    if (doc->IsCmnFacTranscription()) this->AdjustStaffDefElements(measure, doc);

    return FUNCTOR_CONTINUE;
}

void AdjustXRelForTranscriptionFunctor::AdjustStaffDefElements(Measure *measure, const Doc *doc)
{
    // The clefs, key and meter signatures drawn at the start of a system (or at a change) have no zone. They are
    // placed one after the other from the start of the measure, aligned across the staves, as in the regular layout.
    std::vector<std::vector<LayerElement *>> columns(5);
    for (Object *object : measure->FindAllDescendantsByType(LAYER, false)) {
        Layer *layer = vrv_cast<Layer *>(object);
        assert(layer);
        if (layer->GetStaffDefClef()) columns.at(0).push_back(layer->GetStaffDefClef());
        if (layer->GetStaffDefKeySig()) columns.at(1).push_back(layer->GetStaffDefKeySig());
        if (layer->GetStaffDefMensur()) columns.at(2).push_back(layer->GetStaffDefMensur());
        if (layer->GetStaffDefMeterSig()) columns.at(3).push_back(layer->GetStaffDefMeterSig());
        if (layer->GetStaffDefMeterSigGrp()) columns.at(4).push_back(layer->GetStaffDefMeterSigGrp());
    }

    const int unit = doc->GetDrawingUnit(100);
    int x = measure->GetDrawingX();
    for (const std::vector<LayerElement *> &column : columns) {
        int width = 0;
        bool hasElement = false;
        for (LayerElement *element : column) {
            // Skip elements drawing nothing, such as a key signature without accidentals
            if (!element->HasSelfBB() || (element->GetSelfX2() <= element->GetSelfX1())) continue;
            if (!hasElement) x += doc->GetLeftMargin(element) * unit;
            hasElement = true;
            // The position without the relative x, to which the relative x is set for the left to be at x
            const int drawingX = element->GetDrawingX() - element->GetDrawingXRel();
            element->SetDrawingXRel(x - drawingX - element->GetSelfX1());
            width = std::max(width, element->GetSelfX2() - element->GetSelfX1());
        }
        if (!hasElement) continue;
        x += width + doc->GetRightMargin(column.front()) * unit;
    }
}

} // namespace vrv
