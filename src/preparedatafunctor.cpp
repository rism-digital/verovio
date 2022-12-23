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
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareCrossStaffFunctor::VisitLayerElementEnd(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareCrossStaffFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
