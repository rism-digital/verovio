/////////////////////////////////////////////////////////////////////////////
// Name:        preparedatafunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "preparedatafunctor.h"

//----------------------------------------------------------------------------

#include "areaposinterface.h"
#include "doc.h"
#include "layer.h"
#include "runningelement.h"
#include "score.h"
#include "staff.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// PrepareDataInitializationFunctor
//----------------------------------------------------------------------------

PrepareDataInitializationFunctor::PrepareDataInitializationFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode PrepareDataInitializationFunctor::VisitChord(Chord *chord)
{
    if (chord->HasEmptyList(chord)) {
        LogWarning("Chord '%s' has no child note - a default note is added", chord->GetID().c_str());
        Note *rescueNote = new Note();
        chord->AddChild(rescueNote);
    }
    chord->Modify();

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    floatingObject->ResetDrawingObjectIDs();

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitKeySig(KeySig *keySig)
{
    // Clear and regenerate attribute children
    keySig->GenerateKeyAccidAttribChildren();

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitRunningElement(RunningElement *runningElement)
{
    runningElement->ResetCells();
    runningElement->ResetDrawingScaling();

    const ListOfObjects &childList = runningElement->GetList(runningElement);
    for (ListOfObjects::const_iterator iter = childList.begin(); iter != childList.end(); ++iter) {
        int pos = 0;
        AreaPosInterface *interface = dynamic_cast<AreaPosInterface *>(*iter);
        assert(interface);
        pos = runningElement->GetAlignmentPos(interface->GetHalign(), interface->GetValign());
        TextElement *text = vrv_cast<TextElement *>(*iter);
        assert(text);
        runningElement->AppendTextToCell(pos, text);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitScore(Score *score)
{
    // Evaluate functor on scoreDef
    score->GetScoreDef()->Process(*this);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareCueSizeFunctor
//----------------------------------------------------------------------------

PrepareCueSizeFunctor::PrepareCueSizeFunctor() {}

FunctorCode PrepareCueSizeFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    Layer *currentLayer = vrv_cast<Layer *>(layerElement->GetFirstAncestor(LAYER));
    assert(currentLayer);
    if (currentLayer->GetCue() == BOOLEAN_true) {
        layerElement->SetDrawingCueSize(true);
        return FUNCTOR_CONTINUE;
    }

    if (layerElement->IsGraceNote()) {
        layerElement->SetDrawingCueSize(true);
    }
    // This covers the case when the @size is given on the element
    else if (layerElement->HasAttClass(ATT_CUE)) {
        AttCue *att = dynamic_cast<AttCue *>(layerElement);
        assert(att);
        if (att->HasCue()) layerElement->SetDrawingCueSize(att->GetCue() == BOOLEAN_true);
    }
    // For note, we also need to look at the parent chord
    else if (layerElement->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(layerElement);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord) note->SetDrawingCueSize(chord->GetDrawingCueSize());
    }
    // For tuplet, we also need to look at the first note or chord
    else if (layerElement->Is(TUPLET)) {
        ClassIdsComparison matchType({ NOTE, CHORD });
        ArrayOfObjects children;
        LayerElement *child = dynamic_cast<LayerElement *>(layerElement->FindDescendantByComparison(&matchType));
        if (child) layerElement->SetDrawingCueSize(child->GetDrawingCueSize());
    }
    // For accid, look at the parent if @func="edit" or otherwise to the parent note
    else if (layerElement->Is(ACCID)) {
        Accid *accid = vrv_cast<Accid *>(layerElement);
        assert(accid);
        if (accid->GetFunc() == accidLog_FUNC_edit)
            accid->SetDrawingCueSize(true);
        else {
            Note *note = dynamic_cast<Note *>(accid->GetFirstAncestor(NOTE, MAX_ACCID_DEPTH));
            if (note) accid->SetDrawingCueSize(note->GetDrawingCueSize());
        }
    }
    else if (layerElement->Is({ ARTIC, DOTS, FLAG, STEM })) {
        Note *note = dynamic_cast<Note *>(layerElement->GetFirstAncestor(NOTE, MAX_NOTE_DEPTH));
        if (note)
            layerElement->SetDrawingCueSize(note->GetDrawingCueSize());
        else {
            Chord *chord = dynamic_cast<Chord *>(layerElement->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
            if (chord) layerElement->SetDrawingCueSize(chord->GetDrawingCueSize());
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareCrossStaffFunctor
//----------------------------------------------------------------------------

PrepareCrossStaffFunctor::PrepareCrossStaffFunctor()
{
    m_currentMeasure = NULL;
    m_currentCrossStaff = NULL;
    m_currentCrossLayer = NULL;
}

FunctorCode PrepareCrossStaffFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    layerElement->m_crossStaff = NULL;
    layerElement->m_crossLayer = NULL;

    // Look for cross-staff situations
    // If we have one, make it available in m_crossStaff
    AttStaffIdent *crossElement = dynamic_cast<AttStaffIdent *>(layerElement);
    if (!crossElement) return FUNCTOR_CONTINUE;

    // If we have not @staff, set to what we had before (quite likely NULL for all non cross staff cases)
    if (!crossElement->HasStaff()) {
        layerElement->m_crossStaff = m_currentCrossStaff;
        layerElement->m_crossLayer = m_currentCrossLayer;
        return FUNCTOR_CONTINUE;
    }

    // We have a @staff, set the current pointers to NULL before assigning them
    m_currentCrossStaff = NULL;
    m_currentCrossLayer = NULL;

    AttNIntegerComparison comparisonFirst(STAFF, crossElement->GetStaff().at(0));
    layerElement->m_crossStaff
        = dynamic_cast<Staff *>(m_currentMeasure->FindDescendantByComparison(&comparisonFirst, 1));
    if (!layerElement->m_crossStaff) {
        LogWarning("Could not get the cross staff reference '%d' for element '%s'", crossElement->GetStaff().at(0),
            layerElement->GetID().c_str());
        return FUNCTOR_CONTINUE;
    }

    Staff *parentStaff = layerElement->GetAncestorStaff();
    // Check if we have a cross-staff to itself...
    if (layerElement->m_crossStaff == parentStaff) {
        LogWarning("The cross staff reference '%d' for element '%s' seems to be identical to the parent staff",
            crossElement->GetStaff().at(0), layerElement->GetID().c_str());
        layerElement->m_crossStaff = NULL;
        return FUNCTOR_CONTINUE;
    }

    Layer *parentLayer = vrv_cast<Layer *>(layerElement->GetFirstAncestor(LAYER));
    assert(parentLayer);
    // Now try to get the corresponding layer - for now look for the same layer @n
    int layerN = parentLayer->GetN();
    // When we will have allowed @layer in <note>, we will have to do:
    // int layerN = durElement->HasLayer() ? durElement->GetLayer() : (*currentLayer)->GetN();
    AttNIntegerComparison comparisonFirstLayer(LAYER, layerN);
    bool direction = (parentStaff->GetN() < layerElement->m_crossStaff->GetN()) ? FORWARD : BACKWARD;
    layerElement->m_crossLayer
        = dynamic_cast<Layer *>(layerElement->m_crossStaff->FindDescendantByComparison(&comparisonFirstLayer, 1));
    if (!layerElement->m_crossLayer) {
        // Just try to pick the first one... (i.e., last one when crossing above)
        layerElement->m_crossLayer
            = dynamic_cast<Layer *>(layerElement->m_crossStaff->FindDescendantByType(LAYER, UNSPECIFIED, direction));
    }
    if (!layerElement->m_crossLayer) {
        // Nothing we can do
        LogWarning("Could not get the layer with cross-staff reference '%d' for element '%s'",
            crossElement->GetStaff().at(0), layerElement->GetID().c_str());
        layerElement->m_crossStaff = NULL;
        return FUNCTOR_CONTINUE;
    }

    if (direction == FORWARD) {
        layerElement->m_crossLayer->SetCrossStaffFromAbove(true);
    }
    else {
        layerElement->m_crossLayer->SetCrossStaffFromBelow(true);
    }

    m_currentCrossStaff = layerElement->m_crossStaff;
    m_currentCrossLayer = layerElement->m_crossLayer;

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareCrossStaffFunctor::VisitLayerElementEnd(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    DurationInterface *durInterface = layerElement->GetDurationInterface();
    if (durInterface) {
        // If we have @staff, reset it to NULL - this can be problematic if we have different @staff attributes
        // in the children of one element. We do not consider this now because it seems over the top
        // We would need to look at the @n attribute and to have a stack to handle this properly
        if (durInterface->HasStaff()) {
            m_currentCrossStaff = NULL;
            m_currentCrossLayer = NULL;
        }
    }
    else if (layerElement->Is({ BEAM, BTREM, FTREM, TUPLET })) {
        // For other elements (e.g., beams, tuplets) check if all their child duration elements are cross-staff
        // If yes, make them cross-staff themselves.
        ListOfObjects durations;
        InterfaceComparison hasInterface(INTERFACE_DURATION);
        layerElement->FindAllDescendantsByComparison(&durations, &hasInterface);
        Staff *crossStaff = NULL;
        Layer *crossLayer = NULL;
        for (auto object : durations) {
            LayerElement *durElement = vrv_cast<LayerElement *>(object);
            assert(durElement);
            // The duration element is not cross-staff or the cross-staff is not the same staff (very rare)
            if (!durElement->m_crossStaff || (crossStaff && (durElement->m_crossStaff != crossStaff))) {
                crossStaff = NULL;
                // We can stop here
                break;
            }
            else {
                crossStaff = durElement->m_crossStaff;
                crossLayer = durElement->m_crossLayer;
            }
        }
        if (crossStaff) {
            layerElement->m_crossStaff = crossStaff;
            layerElement->m_crossLayer = crossLayer;
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareCrossStaffFunctor::VisitMeasure(Measure *measure)
{
    m_currentMeasure = measure;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
