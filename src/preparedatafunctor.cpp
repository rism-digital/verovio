/////////////////////////////////////////////////////////////////////////////
// Name:        preparedatafunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "preparedatafunctor.h"

//----------------------------------------------------------------------------

#include "altsyminterface.h"
#include "areaposinterface.h"
#include "beamspan.h"
#include "dir.h"
#include "div.h"
#include "doc.h"
#include "dot.h"
#include "dynam.h"
#include "editorial.h"
#include "elementpart.h"
#include "ending.h"
#include "f.h"
#include "hairpin.h"
#include "harm.h"
#include "layer.h"
#include "mrpt.h"
#include "pedal.h"
#include "plistinterface.h"
#include "reh.h"
#include "repeatmark.h"
#include "rest.h"
#include "runningelement.h"
#include "score.h"
#include "section.h"
#include "staff.h"
#include "stem.h"
#include "syl.h"
#include "symboltable.h"
#include "system.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "text.h"
#include "timestamp.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// PrepareDataInitializationFunctor
//----------------------------------------------------------------------------

PrepareDataInitializationFunctor::PrepareDataInitializationFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode PrepareDataInitializationFunctor::VisitDiv(Div *div)
{
    this->VisitTextLayoutElement(div);

    if (m_doc->GetOptions()->m_breaks.GetValue() == BREAKS_none) {
        div->SetDrawingInline(true);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitChord(Chord *chord)
{
    if (chord->HasEmptyList()) {
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

FunctorCode PrepareDataInitializationFunctor::VisitRepeatMark(RepeatMark *repeatMark)
{
    // Call parent one too
    this->VisitControlElement(repeatMark);

    if (repeatMark->GetChildCount() == 0 && repeatMark->HasFunc() && repeatMark->GetFunc() == repeatMarkLog_FUNC_fine) {
        Text *fine = new Text();
        fine->IsGenerated(true);
        fine->SetText(U"Fine");
        repeatMark->AddChild(fine);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitScore(Score *score)
{
    assert(score->GetScoreDef());

    // Evaluate functor on scoreDef
    score->GetScoreDef()->Process(*this);

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitTextLayoutElement(TextLayoutElement *textLayoutElement)
{
    textLayoutElement->ResetCells();
    textLayoutElement->ResetDrawingScaling();

    const ListOfObjects &childList = textLayoutElement->GetList();
    for (Object *child : childList) {
        int pos = 0;
        AreaPosInterface *interface = dynamic_cast<AreaPosInterface *>(child);
        assert(interface);
        pos = textLayoutElement->GetAlignmentPos(interface->GetHalign(), interface->GetValign());
        TextElement *text = vrv_cast<TextElement *>(child);
        assert(text);
        textLayoutElement->AppendTextToCell(pos, text);
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareCueSizeFunctor
//----------------------------------------------------------------------------

PrepareCueSizeFunctor::PrepareCueSizeFunctor() : Functor() {}

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
            Note *note = vrv_cast<Note *>(accid->GetFirstAncestor(NOTE, MAX_ACCID_DEPTH));
            if (note) accid->SetDrawingCueSize(note->GetDrawingCueSize());
        }
    }
    else if (layerElement->Is({ ARTIC, DOTS, FLAG, STEM })) {
        Note *note = vrv_cast<Note *>(layerElement->GetFirstAncestor(NOTE, MAX_NOTE_DEPTH));
        if (note)
            layerElement->SetDrawingCueSize(note->GetDrawingCueSize());
        else {
            Chord *chord = vrv_cast<Chord *>(layerElement->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
            if (chord) layerElement->SetDrawingCueSize(chord->GetDrawingCueSize());
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareCrossStaffFunctor
//----------------------------------------------------------------------------

PrepareCrossStaffFunctor::PrepareCrossStaffFunctor() : Functor()
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
    layerElement->m_crossStaff = vrv_cast<Staff *>(m_currentMeasure->FindDescendantByComparison(&comparisonFirst, 1));
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
        = vrv_cast<Layer *>(layerElement->m_crossStaff->FindDescendantByComparison(&comparisonFirstLayer, 1));
    if (!layerElement->m_crossLayer) {
        // Just try to pick the first one... (i.e., last one when crossing above)
        layerElement->m_crossLayer
            = vrv_cast<Layer *>(layerElement->m_crossStaff->FindDescendantByType(LAYER, UNSPECIFIED, direction));
    }
    if (!layerElement->m_crossLayer) {
        // Nothing we can do
        LogWarning("Could not get the layer with cross-staff reference '%d' for element '%s'",
            crossElement->GetStaff().at(0), layerElement->GetID().c_str());
        layerElement->m_crossStaff = NULL;
    }
    else {
        if (direction == FORWARD) {
            layerElement->m_crossLayer->SetCrossStaffFromAbove(true);
        }
        else {
            layerElement->m_crossLayer->SetCrossStaffFromBelow(true);
        }
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
        for (Object *object : durations) {
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

//----------------------------------------------------------------------------
// PrepareAltSymFunctor
//----------------------------------------------------------------------------

PrepareAltSymFunctor::PrepareAltSymFunctor() : Functor()
{
    m_symbolTable = NULL;
}

FunctorCode PrepareAltSymFunctor::VisitObject(Object *object)
{
    if (object->Is(SCORE)) {
        Score *score = vrv_cast<Score *>(object);
        assert(score);
        assert(score->GetScoreDef());
        m_symbolTable = vrv_cast<SymbolTable *>(score->GetScoreDef()->FindDescendantByType(SYMBOLTABLE));
    }

    if (object->HasInterface(INTERFACE_ALT_SYM)) {
        AltSymInterface *interface = object->GetAltSymInterface();
        assert(interface);
        interface->InterfacePrepareAltSym(*this, object);
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareFacsimileFunctor
//----------------------------------------------------------------------------

PrepareFacsimileFunctor::PrepareFacsimileFunctor(Facsimile *facsimile) : Functor()
{
    m_facsimile = facsimile;
}

FunctorCode PrepareFacsimileFunctor::VisitObject(Object *object)
{
    if (object->HasInterface(INTERFACE_FACSIMILE)) {
        FacsimileInterface *interface = object->GetFacsimileInterface();
        assert(interface);
        if (interface->HasFacs()) {
            interface->InterfacePrepareFacsimile(*this, object);
        }
        // Zoneless syl
        else if (object->Is(SYL)) {
            m_zonelessSyls.push_back(object);
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareLinkingFunctor
//----------------------------------------------------------------------------

PrepareLinkingFunctor::PrepareLinkingFunctor() : Functor(), CollectAndProcess() {}

void PrepareLinkingFunctor::InsertNextIDPair(const std::string &nextID, LinkingInterface *interface)
{
    m_nextIDPairs.insert({ nextID, interface });
}

void PrepareLinkingFunctor::InsertSameasIDPair(const std::string &sameasID, LinkingInterface *interface)
{
    m_sameasIDPairs.insert({ sameasID, interface });
}

FunctorCode PrepareLinkingFunctor::VisitObject(Object *object)
{
    if (this->IsCollectingData() && object->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = object->GetLinkingInterface();
        assert(interface);
        interface->InterfacePrepareLinking(*this, object);
    }

    if (object->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(object);
        assert(note);
        this->ResolveStemSameas(note);
    }

    // @next
    std::string id = object->GetID();
    auto r1 = m_nextIDPairs.equal_range(id);
    if (r1.first != m_nextIDPairs.end()) {
        for (auto i = r1.first; i != r1.second; ++i) {
            i->second->SetNextLink(object);
        }
        m_nextIDPairs.erase(r1.first, r1.second);
    }

    // @sameas
    auto r2 = m_sameasIDPairs.equal_range(id);
    if (r2.first != m_sameasIDPairs.end()) {
        for (auto j = r2.first; j != r2.second; ++j) {
            j->second->SetSameasLink(object);
            // Issue a warning if classes of object and sameas do not match
            Object *owner = dynamic_cast<Object *>(j->second);
            if (owner && (owner->GetClassId() != object->GetClassId())) {
                LogWarning("%s with @xml:id %s has @sameas to an element of class %s.", owner->GetClassName().c_str(),
                    owner->GetID().c_str(), object->GetClassName().c_str());
            }
        }
        m_sameasIDPairs.erase(r2.first, r2.second);
    }
    return FUNCTOR_CONTINUE;
}

void PrepareLinkingFunctor::ResolveStemSameas(Note *note)
{
    // First pass we fill m_stemSameasIDPairs
    if (this->IsCollectingData()) {
        if (note->HasStemSameas()) {
            std::string idTarget = ExtractIDFragment(note->GetStemSameas());
            m_stemSameasIDPairs[idTarget] = note;
        }
    }
    // Second pass we resolve links
    else {
        const std::string id = note->GetID();
        if (m_stemSameasIDPairs.contains(id)) {
            Note *noteStemSameas = m_stemSameasIDPairs.at(id);
            // Instanciate the bi-directional references and mark the roles as unset
            note->SetStemSameasNote(noteStemSameas);
            note->SetStemSameasRole(SAMEAS_UNSET);
            noteStemSameas->SetStemSameasNote(note);
            noteStemSameas->SetStemSameasRole(SAMEAS_UNSET);
            // Also resolve beams and instanciate the bi-directional references
            Beam *beamStemSameas = noteStemSameas->GetAncestorBeam();
            if (beamStemSameas) {
                Beam *beam = note->GetAncestorBeam();
                if (!beam) {
                    // This is one thing that can go wrong. We can have many others here...
                    // E.g., not the same number of notes, conflicting durations, not all notes sharing stems, ...
                    // Not sure everything could be checked here.
                    LogError("Notes with @stem.sameas in a beam should refer only to a note also in beam.");
                }
                else {
                    beam->SetStemSameasBeam(beamStemSameas);
                    beamStemSameas->SetStemSameasBeam(beam);
                }
            }
            m_stemSameasIDPairs.erase(id);
        }
    }
}

//----------------------------------------------------------------------------
// PreparePlistFunctor
//----------------------------------------------------------------------------

PreparePlistFunctor::PreparePlistFunctor() : Functor(), CollectAndProcess() {}

void PreparePlistFunctor::InsertInterfaceObjectIDPair(Object *objectWithPlist, const std::string &elementID)
{
    m_plistObjectIDPairs.push_back(std::make_pair(objectWithPlist, elementID));
}

FunctorCode PreparePlistFunctor::VisitObject(Object *object)
{
    if (this->IsCollectingData()) {
        // Skip expansion elements because these are handled in Doc::ExpandExpansions
        if (object->HasInterface(INTERFACE_PLIST) && !object->Is(EXPANSION)) {
            PlistInterface *interface = object->GetPlistInterface();
            assert(interface);
            return interface->InterfacePreparePlist(*this, object);
        }
    }
    else {
        if (!object->IsLayerElement()) return FUNCTOR_CONTINUE;

        const std::string &id = object->GetID();
        auto iter = std::find_if(m_plistObjectIDPairs.begin(), m_plistObjectIDPairs.end(),
            [&id](const std::pair<Object *, std::string> &pair) { return (pair.second == id); });
        if (iter != m_plistObjectIDPairs.end()) {
            // Set reference for matched pair and erase it from the list
            PlistInterface *interface = iter->first->GetPlistInterface();
            assert(interface);
            interface->SetRef(object);
            // Add back link to the object referred in the plist - for now only for Annot
            if (iter->first->Is(ANNOTSCORE)) {
                object->AddPlistReference(iter->first);
            }
            m_plistObjectIDPairs.erase(iter);
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareDurationFunctor
//----------------------------------------------------------------------------

PrepareDurationFunctor::PrepareDurationFunctor() : Functor()
{
    m_durDefault = DURATION_NONE;
}

FunctorCode PrepareDurationFunctor::VisitLayerElement(LayerElement *layerElement)
{
    DurationInterface *durInterface = layerElement->GetDurationInterface();
    if (durInterface) {
        durInterface->SetDurDefault(m_durDefault);
        // Check if there is a duration default for the staff
        if (!m_durDefaultForStaffN.empty()) {
            Staff *staff = layerElement->GetAncestorStaff(RESOLVE_CROSS_STAFF);
            if (m_durDefaultForStaffN.contains(staff->GetN())) {
                durInterface->SetDurDefault(m_durDefaultForStaffN.at(staff->GetN()));
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDurationFunctor::VisitScore(Score *score)
{
    ScoreDef *scoreDef = score->GetScoreDef();
    if (scoreDef) {
        scoreDef->Process(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDurationFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    m_durDefaultForStaffN.clear();
    m_durDefault = scoreDef->GetDurDefault();

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDurationFunctor::VisitStaffDef(StaffDef *staffDef)
{
    if (staffDef->HasDurDefault() && staffDef->HasN()) {
        m_durDefaultForStaffN[staffDef->GetN()] = staffDef->GetDurDefault();
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareTimePointingFunctor
//----------------------------------------------------------------------------

PrepareTimePointingFunctor::PrepareTimePointingFunctor() : Functor() {}

void PrepareTimePointingFunctor::InsertInterfaceIDTuple(ClassId classID, TimePointInterface *interface)
{
    m_timePointingInterfaces.push_back({ interface, classID });
}

FunctorCode PrepareTimePointingFunctor::VisitF(F *f)
{
    // At this stage we require <f> to have a @startid - eventually we can
    // modify this method and set as start the parent <harm> so @startid would not be
    // required anymore

    // Pass it to the pseudo functor of the interface
    TimePointInterface *interface = f->GetTimePointInterface();
    assert(interface);
    return interface->InterfacePrepareTimePointing(*this, f);
}

FunctorCode PrepareTimePointingFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    // Pass it to the pseudo functor of the interface
    if (floatingObject->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = floatingObject->GetTimePointInterface();
        assert(interface);
        return interface->InterfacePrepareTimePointing(*this, floatingObject);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimePointingFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Do not look for tstamp pointing to these
    if (layerElement->Is({ ARTIC, BEAM, FLAG, TUPLET, STEM, VERSE })) return FUNCTOR_CONTINUE;

    ListOfPointingInterClassIdPairs::iterator iter = m_timePointingInterfaces.begin();
    while (iter != m_timePointingInterfaces.end()) {
        if (iter->first->SetStartOnly(layerElement)) {
            // We have both the start and the end that are matched
            iter = m_timePointingInterfaces.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimePointingFunctor::VisitMeasureEnd(Measure *measure)
{
    if (!m_timePointingInterfaces.empty()) {
        LogWarning("%d time pointing element(s) could not be matched in measure %s", m_timePointingInterfaces.size(),
            measure->GetID().c_str());
    }

    ListOfPointingInterClassIdPairs::iterator iter = m_timePointingInterfaces.begin();
    while (iter != m_timePointingInterfaces.end()) {
        iter = m_timePointingInterfaces.erase(iter);
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareTimeSpanningFunctor
//----------------------------------------------------------------------------

PrepareTimeSpanningFunctor::PrepareTimeSpanningFunctor() : Functor(), CollectAndProcess()
{
    m_insideMeasure = false;
}

void PrepareTimeSpanningFunctor::InsertInterfaceOwnerPair(Object *owner, TimeSpanningInterface *interface)
{
    m_timeSpanningInterfaces.push_back({ interface, owner });
}

FunctorCode PrepareTimeSpanningFunctor::VisitF(F *f)
{
    if (!m_insideMeasure) {
        return this->CallPseudoFunctor(f);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimeSpanningFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    if (!m_insideMeasure && floatingObject->HasInterface(INTERFACE_TIME_SPANNING)) {
        return this->CallPseudoFunctor(floatingObject);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimeSpanningFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Do not look for tstamp pointing to these
    if (layerElement->Is({ ARTIC, BEAM, FLAG, TUPLET, STEM, VERSE })) return FUNCTOR_CONTINUE;

    ListOfSpanningInterOwnerPairs::iterator iter = m_timeSpanningInterfaces.begin();
    while (iter != m_timeSpanningInterfaces.end()) {
        if (iter->first->SetStartAndEnd(layerElement)) {
            // Verify that the interface owner is encoded in the measure of its start
            iter->first->VerifyMeasure(iter->second);
            // We have both the start and the end that are matched
            iter = m_timeSpanningInterfaces.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimeSpanningFunctor::VisitMeasure(Measure *measure)
{
    if (this->IsCollectingData()) {
        ListOfObjects timeSpanningObjects;
        InterfaceComparison ic(INTERFACE_TIME_SPANNING);
        measure->FindAllDescendantsByComparison(&timeSpanningObjects, &ic);
        for (Object *object : timeSpanningObjects) {
            this->CallPseudoFunctor(object);
        }
    }
    m_insideMeasure = true;

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimeSpanningFunctor::VisitMeasureEnd(Measure *measure)
{
    if (this->IsCollectingData()) {
        ListOfSpanningInterOwnerPairs::iterator iter = m_timeSpanningInterfaces.begin();
        while (iter != m_timeSpanningInterfaces.end()) {
            // At the end of the measure we remove elements for which we do not need to match the end (for now).
            // Eventually, we could consider them, for example if we want to display their spanning or for
            // improved MIDI output
            if (iter->second->GetClassId() == HARM) {
                iter = m_timeSpanningInterfaces.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }
    m_insideMeasure = false;

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimeSpanningFunctor::CallPseudoFunctor(Object *timeSpanningObject)
{
    TimeSpanningInterface *interface = timeSpanningObject->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfacePrepareTimeSpanning(*this, timeSpanningObject);
}

//----------------------------------------------------------------------------
// PrepareTimestampsFunctor
//----------------------------------------------------------------------------

PrepareTimestampsFunctor::PrepareTimestampsFunctor() : Functor() {}

void PrepareTimestampsFunctor::InsertInterfaceIDPair(ClassId classID, TimeSpanningInterface *interface)
{
    m_timeSpanningInterfaces.push_back({ interface, classID });
}

void PrepareTimestampsFunctor::InsertObjectBeatPair(Object *object, const data_MEASUREBEAT &beat)
{
    m_tstamps.push_back({ object, beat });
}

FunctorCode PrepareTimestampsFunctor::VisitDocEnd(Doc *doc)
{
    if (!doc->GetOptions()->m_openControlEvents.GetValue() || m_timeSpanningInterfaces.empty()) {
        return FUNCTOR_CONTINUE;
    }

    Measure *lastMeasure = vrv_cast<Measure *>(doc->FindDescendantByType(MEASURE, UNLIMITED_DEPTH, BACKWARD));
    if (!lastMeasure) {
        return FUNCTOR_CONTINUE;
    }

    for (auto &pair : m_timeSpanningInterfaces) {
        TimeSpanningInterface *interface = pair.first;
        assert(interface);
        if (!interface->GetEnd()) {
            interface->SetEnd(lastMeasure->GetRightBarLine());
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimestampsFunctor::VisitF(F *f)
{
    // Using @tstamp on <f> will work only if @staff is also given on <f>

    // Pass it to the pseudo functor of the interface
    TimeSpanningInterface *interface = f->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfacePrepareTimestamps(*this, f);
}

FunctorCode PrepareTimestampsFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    // Pass it to the pseudo functor of the interface
    if (floatingObject->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = floatingObject->GetTimePointInterface();
        assert(interface);
        return interface->InterfacePrepareTimestamps(*this, floatingObject);
    }
    else if (floatingObject->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = floatingObject->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfacePrepareTimestamps(*this, floatingObject);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareTimestampsFunctor::VisitMeasureEnd(Measure *measure)
{
    ListOfObjectBeatPairs::iterator iter = m_tstamps.begin();
    // Loop through the object/beat pairs and create the TimestampAttr when necessary
    while (iter != m_tstamps.end()) {
        // -1 means that we have a @tstamp (start) to add to the current measure
        if ((*iter).second.first == -1) {
            TimePointInterface *interface = ((*iter).first)->GetTimePointInterface();
            assert(interface);
            TimestampAttr *timestampAttr = measure->m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetStart(timestampAttr);
            // purge the list of unmatched elements if this is a TimeSpanningInterface element
            if ((*iter).first->HasInterface(INTERFACE_TIME_SPANNING)) {
                TimeSpanningInterface *tsInterface = ((*iter).first)->GetTimeSpanningInterface();
                assert(tsInterface);
                if (tsInterface->HasStartAndEnd()) {
                    auto item = std::find_if(m_timeSpanningInterfaces.begin(), m_timeSpanningInterfaces.end(),
                        [tsInterface](
                            std::pair<TimeSpanningInterface *, ClassId> pair) { return (pair.first == tsInterface); });
                    if (item != m_timeSpanningInterfaces.end()) {
                        // LogDebug("Found it!");
                        m_timeSpanningInterfaces.erase(item);
                    }
                }
            }
            // remove it
            iter = m_tstamps.erase(iter);
        }
        // 0 means that we have a @tstamp2 (end) to add to the current measure
        else if ((*iter).second.first == 0) {
            TimeSpanningInterface *interface = ((*iter).first)->GetTimeSpanningInterface();
            assert(interface);
            TimestampAttr *timestampAttr = measure->m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetEnd(timestampAttr);
            // We can check if the interface is now fully mapped (start / end) and purge the list of unmatched
            // elements
            if (interface->HasStartAndEnd()) {
                auto item = std::find_if(m_timeSpanningInterfaces.begin(), m_timeSpanningInterfaces.end(),
                    [interface](
                        std::pair<TimeSpanningInterface *, ClassId> pair) { return (pair.first == interface); });
                if (item != m_timeSpanningInterfaces.end()) {
                    // LogDebug("Found it!");
                    m_timeSpanningInterfaces.erase(item);
                }
            }
            iter = m_tstamps.erase(iter);
        }
        // we have not reached the correct end measure yet
        else {
            (*iter).second.first--;
            ++iter;
        }
    }

    // Here we can also set the start for F within Harm that have no @startid or @tstamp but might have an extender
    // In the future, we can do something similar to handle Dir within other types of control events
    // Basically, a child control event should use the start (and end) of its parent.
    // In the case of F, we still expect the @tstamp2 to be given in F, but this could be changed
    // Eventually, this could be done in another functor if it becomes a more common way to set start / end because it
    // is a bit weird to iterate over F objects here.
    ListOfObjects fs = measure->FindAllDescendantsByType(FIGURE);
    for (auto &object : fs) {
        F *f = vrv_cast<F *>(object);
        assert(f);
        // Nothing to do if the f has a start or has no end
        if (f->GetStart() || !f->GetEnd()) continue;

        Harm *harm = vrv_cast<Harm *>(f->GetFirstAncestor(HARM));
        if (harm) {
            f->SetStart(harm->GetStart());
            // We should also remove the f from the list because we can consider it as being mapped now
            auto item = std::find_if(m_timeSpanningInterfaces.begin(), m_timeSpanningInterfaces.end(),
                [f](std::pair<TimeSpanningInterface *, ClassId> pair) { return (pair.first == f); });
            if (item != m_timeSpanningInterfaces.end()) {
                // LogDebug("Found it!");
                m_timeSpanningInterfaces.erase(item);
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PreparePedalsFunctor
//----------------------------------------------------------------------------

PreparePedalsFunctor::PreparePedalsFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode PreparePedalsFunctor::VisitMeasureEnd(Measure *measure)
{
    // Match down and up pedal lines
    using PedalIter = std::list<Pedal *>::iterator;
    PedalIter iter = m_pedalLines.begin();
    while (iter != m_pedalLines.end()) {
        if ((*iter)->GetDir() != pedalLog_DIR_down) {
            ++iter;
            continue;
        }
        PedalIter up = std::find_if(m_pedalLines.begin(), m_pedalLines.end(), [&iter](Pedal *pedal) {
            return (((*iter)->GetStaff() == pedal->GetStaff()) && (pedal->GetDir() != pedalLog_DIR_down));
        });
        if (up != m_pedalLines.end()) {
            (*iter)->SetEnd((*up)->GetStart());
            if ((*up)->GetDir() == pedalLog_DIR_bounce) {
                (*iter)->EndsWithBounce(true);
            }
            m_pedalLines.erase(up);
            iter = m_pedalLines.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PreparePedalsFunctor::VisitPedal(Pedal *pedal)
{
    if (!pedal->HasDir()) return FUNCTOR_CONTINUE;

    System *system = vrv_cast<System *>(pedal->GetFirstAncestor(SYSTEM));
    assert(system);
    data_PEDALSTYLE form = pedal->GetPedalForm(m_doc, system);
    if ((form == PEDALSTYLE_line) || (form == PEDALSTYLE_pedline)) {
        m_pedalLines.push_back(pedal);
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PreparePointersByLayerFunctor
//----------------------------------------------------------------------------

PreparePointersByLayerFunctor::PreparePointersByLayerFunctor() : Functor()
{
    m_currentElement = NULL;
    m_lastDot = NULL;
}

FunctorCode PreparePointersByLayerFunctor::VisitDot(Dot *dot)
{
    dot->m_drawingPreviousElement = m_currentElement;
    m_lastDot = dot;

    return FUNCTOR_CONTINUE;
}

FunctorCode PreparePointersByLayerFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Skip ligatures because we want it attached to the first note in it
    if (m_lastDot && !layerElement->Is(LIGATURE)) {
        m_lastDot->m_drawingNextElement = layerElement;
        m_lastDot = NULL;
    }
    if (layerElement->Is(BARLINE)) {
        // Do not attach a note when a barline is passed
        m_currentElement = NULL;
    }
    else if (layerElement->Is({ NOTE, REST })) {
        m_currentElement = layerElement;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PreparePointersByLayerFunctor::VisitMeasureEnd(Measure *measure)
{
    if (m_lastDot) {
        m_lastDot->m_drawingNextElement = measure->GetRightBarLine();
        m_lastDot = NULL;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareLyricsFunctor
//----------------------------------------------------------------------------

PrepareLyricsFunctor::PrepareLyricsFunctor() : Functor()
{
    m_currentSyl = NULL;
    m_lastNoteOrChord = NULL;
    m_penultimateNoteOrChord = NULL;
}

FunctorCode PrepareLyricsFunctor::VisitChord(Chord *chord)
{
    m_penultimateNoteOrChord = m_lastNoteOrChord;
    m_lastNoteOrChord = chord;

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareLyricsFunctor::VisitDocEnd(Doc *doc)
{
    if (!m_currentSyl) {
        return FUNCTOR_STOP; // early return
    }
    if (m_lastNoteOrChord && (m_currentSyl->GetStart() != m_lastNoteOrChord)) {
        m_currentSyl->SetEnd(m_lastNoteOrChord);
    }
    else if (doc->GetOptions()->m_openControlEvents.GetValue()) {
        sylLog_WORDPOS wordpos = m_currentSyl->GetWordpos();
        if ((wordpos == sylLog_WORDPOS_i) || (wordpos == sylLog_WORDPOS_m)) {
            Measure *lastMeasure = vrv_cast<Measure *>(doc->FindDescendantByType(MEASURE, UNLIMITED_DEPTH, BACKWARD));
            assert(lastMeasure);
            m_currentSyl->SetEnd(lastMeasure->GetRightBarLine());
        }
    }

    return FUNCTOR_STOP;
}

FunctorCode PrepareLyricsFunctor::VisitNote(Note *note)
{
    if (!note->IsChordTone()) {
        m_penultimateNoteOrChord = m_lastNoteOrChord;
        m_lastNoteOrChord = note;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareLyricsFunctor::VisitSyl(Syl *syl)
{
    Verse *verse = vrv_cast<Verse *>(syl->GetFirstAncestor(VERSE, MAX_NOTE_DEPTH));
    if (verse) {
        syl->m_drawingVerseN = std::max(verse->GetN(), 1);
        syl->m_drawingVersePlace = verse->GetPlace();
    }

    syl->SetStart(vrv_cast<LayerElement *>(syl->GetFirstAncestor(NOTE, MAX_NOTE_DEPTH)));
    // If there isn't an ancestor note, it should be a chord
    if (!syl->GetStart()) {
        syl->SetStart(vrv_cast<LayerElement *>(syl->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH)));
    }

    // At this stage currentSyl is actually the previous one that is ending here
    if (m_currentSyl) {
        // The previous syl was an initial or median -> The note we just parsed is the end
        if ((m_currentSyl->GetWordpos() == sylLog_WORDPOS_i) || (m_currentSyl->GetWordpos() == sylLog_WORDPOS_m)) {
            m_currentSyl->SetEnd(m_lastNoteOrChord);
            m_currentSyl->m_nextWordSyl = syl;
        }
        // The previous syl was a underscore -> the previous but one was the end
        else if (m_currentSyl->GetCon() == sylLog_CON_u) {
            if (m_currentSyl->GetStart() == m_penultimateNoteOrChord) {
                LogWarning("Syllable with underline extender under one single note '%s'",
                    m_currentSyl->GetStart()->GetID().c_str());
            }
            else {
                m_currentSyl->SetEnd(m_penultimateNoteOrChord);
            }
        }
    }

    // Now decide what to do with the starting syl and check if it has a forward connector
    if ((syl->GetWordpos() == sylLog_WORDPOS_i) || (syl->GetWordpos() == sylLog_WORDPOS_m)) {
        m_currentSyl = syl;
        return FUNCTOR_CONTINUE;
    }
    else if (syl->GetCon() == sylLog_CON_u) {
        m_currentSyl = syl;
        return FUNCTOR_CONTINUE;
    }
    else {
        m_currentSyl = NULL;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareLayerElementPartsFunctor
//----------------------------------------------------------------------------

PrepareLayerElementPartsFunctor::PrepareLayerElementPartsFunctor() : Functor() {}

FunctorCode PrepareLayerElementPartsFunctor::VisitChord(Chord *chord)
{
    Stem *currentStem = vrv_cast<Stem *>(chord->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    if (currentStem) currentFlag = vrv_cast<Flag *>(currentStem->GetFirst(FLAG));

    currentStem = this->EnsureStemExists(currentStem, chord);
    currentStem->AttGraced::operator=(*chord);
    currentStem->FillAttributes(*chord);

    data_DURATION duration = chord->GetNoteOrChordDur(chord);
    if ((duration < DURATION_2) || (chord->GetStemVisible() == BOOLEAN_false)) {
        currentStem->IsVirtual(true);
    }

    const bool shouldHaveFlag = ((duration > DURATION_4) && !chord->IsInBeam() && !chord->GetAncestorFTrem());
    currentFlag = this->ProcessFlag(currentFlag, currentStem, shouldHaveFlag);

    chord->SetDrawingStem(currentStem);

    // Calculate chord note groups (except for chord clusters)
    if (!chord->HasCluster()) chord->CalculateNoteGroups();

    // Also set the drawing stem object (or NULL) to all child notes
    const ListOfObjects &childList = chord->GetList();
    for (Object *child : childList) {
        assert(child->Is(NOTE));
        Note *note = vrv_cast<Note *>(child);
        assert(note);
        note->SetDrawingStem(currentStem);
    }

    /************ dots ***********/

    Dots *currentDots = vrv_cast<Dots *>(chord->FindDescendantByType(DOTS, 1));

    const bool shouldHaveDots = (chord->GetDots() > 0);
    currentDots = this->ProcessDots(currentDots, chord, shouldHaveDots);

    /************ Prepare the drawing cue size ************/

    PrepareCueSizeFunctor prepareCueSize;
    chord->Process(prepareCueSize);

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareLayerElementPartsFunctor::VisitNote(Note *note)
{
    Stem *currentStem = vrv_cast<Stem *>(note->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    Chord *chord = note->IsChordTone();
    if (currentStem) currentFlag = vrv_cast<Flag *>(currentStem->GetFirst(FLAG));

    if (!note->IsChordTone() && !note->IsTabGrpNote()) {
        currentStem = this->EnsureStemExists(currentStem, note);
        currentStem->AttGraced::operator=(*note);
        currentStem->FillAttributes(*note);

        if (note->GetActualDur() < DURATION_2 || (note->GetStemVisible() == BOOLEAN_false)) {
            currentStem->IsVirtual(true);
        }
    }
    // This will happen only if the duration has changed
    else if (currentStem) {
        if (note->DeleteChild(currentStem)) {
            currentStem = NULL;
            // The currentFlag (if any) will have been deleted above
            currentFlag = NULL;
        }
    }

    /************ dots ***********/

    Dots *currentDots = vrv_cast<Dots *>(note->FindDescendantByType(DOTS, 1));

    const bool shouldHaveDots = (note->GetDots() > 0);
    if (shouldHaveDots && chord && (chord->GetDots() == note->GetDots())) {
        LogWarning("Note '%s' with a @dots attribute with the same value as its chord parent", note->GetID().c_str());
    }
    currentDots = this->ProcessDots(currentDots, note, shouldHaveDots);

    // We don't care about flags in mensural notes
    if (note->IsMensuralDur()) return FUNCTOR_CONTINUE;

    if (currentStem) {
        const bool shouldHaveFlag = ((note->GetActualDur() > DURATION_4) && !note->IsInBeam()
            && !note->GetAncestorFTrem() && !note->IsChordTone() && !note->IsTabGrpNote());
        currentFlag = this->ProcessFlag(currentFlag, currentStem, shouldHaveFlag);

        if (!chord) note->SetDrawingStem(currentStem);
    }

    /************ Prepare the drawing cue size ************/

    PrepareCueSizeFunctor prepareCueSize;
    note->Process(prepareCueSize);

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareLayerElementPartsFunctor::VisitRest(Rest *rest)
{
    Dots *currentDots = vrv_cast<Dots *>(rest->FindDescendantByType(DOTS, 1));

    const bool shouldHaveDots = (rest->GetDur() > DURATION_breve) && (rest->GetDots() > 0);
    currentDots = this->ProcessDots(currentDots, rest, shouldHaveDots);

    /************ Prepare the drawing cue size ************/

    PrepareCueSizeFunctor prepareCueSize;
    rest->Process(prepareCueSize);

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareLayerElementPartsFunctor::VisitTabDurSym(TabDurSym *tabDurSym)
{
    Stem *currentStem = vrv_cast<Stem *>(tabDurSym->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    if (currentStem) currentFlag = vrv_cast<Flag *>(currentStem->GetFirst(FLAG));

    currentStem = this->EnsureStemExists(currentStem, tabDurSym);
    tabDurSym->SetDrawingStem(currentStem);

    /************ flags ***********/

    TabGrp *tabGrp = vrv_cast<TabGrp *>(tabDurSym->GetFirstAncestor(TABGRP));
    assert(tabGrp);

    // No flag within beam for durations longer than 8th notes
    const bool shouldHaveFlag = (!tabDurSym->IsInBeam() && (tabGrp->GetActualDur() > DURATION_4));
    currentFlag = this->ProcessFlag(currentFlag, currentStem, shouldHaveFlag);

    return FUNCTOR_SIBLINGS;
}

FunctorCode PrepareLayerElementPartsFunctor::VisitTuplet(Tuplet *tuplet)
{
    TupletBracket *currentBracket = vrv_cast<TupletBracket *>(tuplet->GetFirst(TUPLET_BRACKET));
    TupletNum *currentNum = vrv_cast<TupletNum *>(tuplet->GetFirst(TUPLET_NUM));

    bool beamed = false;
    // Are we contained in a beam?
    if (tuplet->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH)) {
        // is only the tuplet beamed? (will not work with nested tuplets)
        Beam *currentBeam = vrv_cast<Beam *>(tuplet->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
        if (currentBeam->GetChildCount() == 1) {
            beamed = true;
        }
    }
    // Is a beam or bTrem the only child? (will not work with editorial elements)
    if (tuplet->GetChildCount() == 1) {
        if ((tuplet->GetChildCount(BEAM) == 1) || (tuplet->GetChildCount(BTREM) == 1)) beamed = true;
    }

    if ((!tuplet->HasBracketVisible() && !beamed) || (tuplet->GetBracketVisible() == BOOLEAN_true)) {
        if (!currentBracket) {
            currentBracket = new TupletBracket();
            tuplet->AddChild(currentBracket);
        }
        currentBracket->AttTupletVis::operator=(*tuplet);
    }
    // This will happen only if the @bracket.visible value has changed
    else if (currentBracket) {
        if (tuplet->DeleteChild(currentBracket)) {
            currentBracket = NULL;
        }
    }

    if (tuplet->HasNum() && (!tuplet->HasNumVisible() || (tuplet->GetNumVisible() == BOOLEAN_true))) {
        if (!currentNum) {
            currentNum = new TupletNum();
            tuplet->AddChild(currentNum);
        }
        currentNum->AttNumberPlacement::operator=(*tuplet);
        currentNum->AttTupletVis::operator=(*tuplet);
    }
    // This will happen only if the @num.visible value has changed
    else if (currentNum) {
        if (tuplet->DeleteChild(currentNum)) {
            currentNum = NULL;
        }
    }

    /************ Prepare the drawing cue size ************/

    PrepareCueSizeFunctor prepareCueSize;
    tuplet->Process(prepareCueSize);

    /*********** Set the left and right element ***********/

    ClassIdsComparison comparison({ CHORD, NOTE, REST });
    tuplet->SetDrawingLeft(vrv_cast<LayerElement *>(tuplet->FindDescendantByComparison(&comparison)));
    tuplet->SetDrawingRight(
        vrv_cast<LayerElement *>(tuplet->FindDescendantByComparison(&comparison, UNLIMITED_DEPTH, BACKWARD)));

    return FUNCTOR_CONTINUE;
}

Stem *PrepareLayerElementPartsFunctor::EnsureStemExists(Stem *stem, Object *parent) const
{
    assert(parent);

    if (!stem) {
        stem = new Stem();
        stem->IsAttribute(true);
        parent->AddChild(stem);
    }
    return stem;
}

Dots *PrepareLayerElementPartsFunctor::ProcessDots(Dots *dots, Object *parent, bool shouldExist) const
{
    assert(parent);
    assert(parent->GetDurationInterface());

    if (shouldExist) {
        if (!dots) {
            dots = new Dots();
            parent->AddChild(dots);
        }
        dots->AttAugmentDots::operator=(*parent->GetDurationInterface());
    }
    else if (dots) {
        if (parent->DeleteChild(dots)) {
            dots = NULL;
        }
    }
    return dots;
}

Flag *PrepareLayerElementPartsFunctor::ProcessFlag(Flag *flag, Object *parent, bool shouldExist) const
{
    assert(parent);

    if (shouldExist) {
        if (!flag) {
            flag = new Flag();
            parent->AddChild(flag);
        }
    }
    else if (flag) {
        if (parent->DeleteChild(flag)) {
            flag = NULL;
        }
    }
    return flag;
}

//----------------------------------------------------------------------------
// PrepareRptFunctor
//----------------------------------------------------------------------------

PrepareRptFunctor::PrepareRptFunctor(Doc *doc) : DocFunctor(doc)
{
    m_currentMRpt = NULL;
    m_multiNumber = BOOLEAN_NONE;
}

FunctorCode PrepareRptFunctor::VisitLayer(Layer *layer)
{
    // If we have encountered a mRpt before and there is none in this layer, reset it to NULL
    if (m_currentMRpt && !layer->FindDescendantByType(MRPT)) {
        m_currentMRpt = NULL;
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareRptFunctor::VisitMRpt(MRpt *mRpt)
{
    // If multiNumber is not true, nothing needs to be done
    if (m_multiNumber != BOOLEAN_true) {
        return FUNCTOR_CONTINUE;
    }

    // If this is the first one, number has to be 2
    if (m_currentMRpt == NULL) {
        mRpt->m_drawingMeasureCount = 2;
    }
    // Otherwise increment it
    else {
        mRpt->m_drawingMeasureCount = m_currentMRpt->m_drawingMeasureCount + 1;
    }
    m_currentMRpt = mRpt;
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareRptFunctor::VisitStaff(Staff *staff)
{
    // If multiNumber is set, we already know that nothing needs to be done
    // Furthermore, if @multi.number is false, the functor should have stopped (see below)
    if (m_multiNumber != BOOLEAN_NONE) {
        return FUNCTOR_CONTINUE;
    }

    // This is happening only for the first staff element of the staff @n
    ScoreDef *scoreDef = m_doc->GetCorrespondingScore(staff)->GetScoreDef();
    assert(scoreDef);
    if (StaffDef *staffDef = scoreDef->GetStaffDef(staff->GetN())) {
        const bool hideNumber = (staffDef->GetMultiNumber() == BOOLEAN_false)
            || ((staffDef->GetMultiNumber() != BOOLEAN_true) && (scoreDef->GetMultiNumber() == BOOLEAN_false));
        if (hideNumber) {
            // Set it just in case, but stopping the functor should do it for this staff @n
            m_multiNumber = BOOLEAN_false;
            return FUNCTOR_STOP;
        }
    }
    m_multiNumber = BOOLEAN_true;
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareDelayedTurnsFunctor
//----------------------------------------------------------------------------

PrepareDelayedTurnsFunctor::PrepareDelayedTurnsFunctor() : Functor(), CollectAndProcess()
{
    this->ResetCurrent();
}

void PrepareDelayedTurnsFunctor::ResetCurrent()
{
    m_previousElement = NULL;
    m_currentChord = NULL;
    m_currentTurn = NULL;
}

FunctorCode PrepareDelayedTurnsFunctor::VisitLayerElement(LayerElement *layerElement)
{
    // We are initializing the m_delayedTurns map
    if (this->IsCollectingData()) return FUNCTOR_CONTINUE;

    if (!layerElement->HasInterface(INTERFACE_DURATION)) return FUNCTOR_CONTINUE;

    if (m_previousElement) {
        assert(m_currentTurn);
        if (layerElement->Is(NOTE) && m_currentChord) {
            Note *note = vrv_cast<Note *>(layerElement);
            if (note->IsChordTone() == m_currentChord) return FUNCTOR_CONTINUE;
        }
        m_currentTurn->m_drawingEndElement = layerElement;
        this->ResetCurrent();
    }

    if (m_delayedTurns.contains(layerElement)) {
        m_previousElement = layerElement;
        m_currentTurn = m_delayedTurns.at(layerElement);
        if (layerElement->Is(CHORD)) {
            return FUNCTOR_SIBLINGS;
        }
        else if (layerElement->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(layerElement);
            Chord *chord = note->IsChordTone();
            if (chord) m_currentChord = chord;
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDelayedTurnsFunctor::VisitTurn(Turn *turn)
{
    // We already initialized the m_delayedTurns map
    if (this->IsProcessingData()) return FUNCTOR_CONTINUE;

    // Map only delayed turns
    if (turn->GetDelayed() != BOOLEAN_true) return FUNCTOR_CONTINUE;

    // Map only delayed turn pointing to a LayerElement (i.e., not using @tstamp)
    if (turn->GetStart() && !turn->GetStart()->Is(TIMESTAMP_ATTR)) {
        m_delayedTurns[turn->GetStart()] = turn;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareMilestonesFunctor
//----------------------------------------------------------------------------

PrepareMilestonesFunctor::PrepareMilestonesFunctor() : Functor()
{
    m_lastMeasure = NULL;
    m_currentEnding = NULL;
}

void PrepareMilestonesFunctor::InsertStartMilestone(SystemMilestoneInterface *interface)
{
    m_startMilestones.push_back(interface);
}

FunctorCode PrepareMilestonesFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    if (editorialElement->IsSystemMilestone()) {
        editorialElement->InterfacePrepareMilestones(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareMilestonesFunctor::VisitEnding(Ending *ending)
{
    // Endings should always have an SystemMilestoneEnd
    assert(ending->IsSystemMilestone());

    ending->InterfacePrepareMilestones(*this);

    m_currentEnding = ending;

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareMilestonesFunctor::VisitMeasure(Measure *measure)
{
    std::vector<SystemMilestoneInterface *>::iterator iter;
    for (iter = m_startMilestones.begin(); iter != m_startMilestones.end(); ++iter) {
        (*iter)->SetMeasure(measure);
    }
    m_startMilestones.clear();

    if (m_currentEnding) {
        // Set the ending to each measure in between
        measure->SetDrawingEnding(m_currentEnding);
    }

    // Keep a pointer to the measure for when we are reaching the end (see VisitSystemMilestone)
    m_lastMeasure = measure;

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareMilestonesFunctor::VisitSection(Section *section)
{
    if (section->IsSystemMilestone()) {
        section->InterfacePrepareMilestones(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareMilestonesFunctor::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    // We set its pointer to the last measure we have encountered - this can be NULL in case no measure exists before
    // the end milestone
    // This can happen with a editorial container around a scoreDef at the beginning
    systemMilestoneEnd->SetMeasure(m_lastMeasure);

    // Endings are also set as Measure::m_drawingEnding for all measures in between - when we reach the end milestone of
    // an ending, we need to set the m_currentEnding to NULL
    if (m_currentEnding && systemMilestoneEnd->GetStart()->Is(ENDING)) {
        m_currentEnding = NULL;
        // With ending we need the drawing measure - this will crash with en empty ending at the beginning of a score...
        assert(systemMilestoneEnd->GetMeasure());
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareFloatingGrpsFunctor
//----------------------------------------------------------------------------

PrepareFloatingGrpsFunctor::PrepareFloatingGrpsFunctor()
{
    m_previousEnding = NULL;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitDir(Dir *dir)
{
    if (dir->HasVgrp()) {
        dir->SetDrawingGrpId(-dir->GetVgrp());
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitDynam(Dynam *dynam)
{
    if (dynam->HasVgrp()) {
        dynam->SetDrawingGrpId(-dynam->GetVgrp());
    }

    // Keep it for linking only if start is resolved
    if (!dynam->GetStart()) return FUNCTOR_CONTINUE;

    m_dynams.push_back(dynam);

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitEnding(Ending *ending)
{
    if (m_previousEnding) {
        // We need to group the previous and this ending - the previous one should have a grpId
        if (m_previousEnding->GetDrawingGrpId() == 0) {
            LogDebug("Something went wrong with the grouping of the endings");
        }
        ending->SetDrawingGrpId(m_previousEnding->GetDrawingGrpId());
        // Also set the previous ending to NULL
        // We need this because three or more endings might have to be grouped together
        m_previousEnding = NULL;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitHairpin(Hairpin *hairpin)
{
    if (hairpin->HasVgrp()) {
        hairpin->SetDrawingGrpId(-hairpin->GetVgrp());
    }

    // Only try to link them if start and end are resolved
    if (!hairpin->GetStart() || !hairpin->GetEnd()) return FUNCTOR_CONTINUE;

    m_hairpins.push_back(hairpin);

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitHarm(Harm *harm)
{
    std::string n = harm->GetN();
    // If there is no @n on harm we use the first @staff value as negative
    // This will not work if @staff has more than one staff id, but this is probably not going to be used
    if (n == "" && harm->HasStaff()) {
        n = StringFormat("%d", harm->GetStaff().at(0) * -1);
    }

    for (auto &kv : m_harms) {
        if (kv.first == n) {
            harm->SetDrawingGrpId(kv.second->GetDrawingGrpId());
            return FUNCTOR_CONTINUE;
        }
    }

    // first harm@n, create a new group
    // If @n is a digit string, use it as group id - otherwise order them as they appear
    if (IsDigits(n)) {
        harm->SetDrawingGrpId((int)std::strtol(n.c_str(), NULL, 10));
    }
    else {
        harm->SetDrawingGrpObject(harm);
    }
    m_harms.insert({ n, harm });

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitMeasure(Measure *measure)
{
    if (m_previousEnding) {
        // We have a measure in between endings and the previous one was group, just reset pointer to NULL
        m_previousEnding = NULL;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitMeasureEnd(Measure *measure)
{
    // Link dynamics and hairpins at the end of the measure to make sure that the order of elements in MEI does not
    // dictate their linkage. With this, linking dynamics to hairpin is prioritized and hairpins are linked only after
    // all dynamics were processed.

    for (auto &dynam : m_dynams) {
        for (auto &hairpin : m_hairpins) {
            if ((hairpin->GetEnd() == dynam->GetStart()) && (hairpin->GetStaff() == dynam->GetStaff())) {
                if (!hairpin->GetRightLink()) hairpin->SetRightLink(dynam);
            }
        }
    }

    for (auto &hairpin : m_hairpins) {
        for (auto &dynam : m_dynams) {
            if ((dynam->GetStart() == hairpin->GetStart()) && (dynam->GetStaff() == hairpin->GetStaff())) {
                if (!hairpin->GetLeftLink()) hairpin->SetLeftLink(dynam);
            }
            else if ((dynam->GetStart() == hairpin->GetEnd()) && (dynam->GetStaff() == hairpin->GetStaff())) {
                if (!hairpin->GetRightLink()) hairpin->SetRightLink(dynam);
            }
        }

        for (auto &hairpin2 : m_hairpins) {
            if (hairpin == hairpin2) continue;
            if ((hairpin2->GetEnd() == hairpin->GetStart()) && (hairpin2->GetStaff() == hairpin->GetStaff())) {
                if (!hairpin->GetLeftLink() && !hairpin2->GetRightLink()) {
                    hairpin->SetLeftLink(hairpin2);
                    hairpin2->SetRightLink(hairpin);
                }
            }
            if ((hairpin2->GetStart() == hairpin->GetEnd()) && (hairpin2->GetStaff() == hairpin->GetStaff())) {
                if (!hairpin2->GetLeftLink() && !hairpin->GetRightLink()) {
                    hairpin2->SetLeftLink(hairpin);
                    hairpin->SetRightLink(hairpin2);
                }
            }
        }
    }

    m_dynams.clear();

    std::vector<Hairpin *>::iterator iter = m_hairpins.begin();
    while (iter != m_hairpins.end()) {
        assert((*iter)->GetEnd());
        Measure *measureEnd = vrv_cast<Measure *>((*iter)->GetEnd()->GetFirstAncestor(MEASURE));
        if (measureEnd == measure) {
            iter = m_hairpins.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitPedal(Pedal *pedal)
{
    if (pedal->HasVgrp()) {
        pedal->SetDrawingGrpId(-pedal->GetVgrp());
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareFloatingGrpsFunctor::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    assert(systemMilestoneEnd->GetStart());

    // We are reaching the end of an ending - store it and it will be grouped with the next one if there is
    // no measure in between
    if (systemMilestoneEnd->GetStart()->Is(ENDING)) {
        m_previousEnding = vrv_cast<Ending *>(systemMilestoneEnd->GetStart());
        assert(m_previousEnding);
        // This is the end of the first ending - generate a grpId
        if (m_previousEnding->GetDrawingGrpId() == 0) {
            m_previousEnding->SetDrawingGrpObject(m_previousEnding);
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareStaffCurrentTimeSpanningFunctor
//----------------------------------------------------------------------------

PrepareStaffCurrentTimeSpanningFunctor::PrepareStaffCurrentTimeSpanningFunctor() : Functor() {}

void PrepareStaffCurrentTimeSpanningFunctor::InsertTimeSpanningElement(Object *element)
{
    m_timeSpanningElements.push_back(element);
}

FunctorCode PrepareStaffCurrentTimeSpanningFunctor::VisitF(F *f)
{
    TimeSpanningInterface *interface = f->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfacePrepareStaffCurrentTimeSpanning(*this, f);
}

FunctorCode PrepareStaffCurrentTimeSpanningFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    // Pass it to the pseudo functor of the interface
    if (floatingObject->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = floatingObject->GetTimeSpanningInterface();
        assert(interface);
        interface->InterfacePrepareStaffCurrentTimeSpanning(*this, floatingObject);
    }
    if (floatingObject->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = floatingObject->GetLinkingInterface();
        assert(interface);
        interface->InterfacePrepareStaffCurrentTimeSpanning(*this, floatingObject);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareStaffCurrentTimeSpanningFunctor::VisitMeasureEnd(Measure *measure)
{
    ArrayOfObjects::iterator iter = m_timeSpanningElements.begin();
    while (iter != m_timeSpanningElements.end()) {
        Measure *endParent = NULL;
        if ((*iter)->HasInterface(INTERFACE_TIME_SPANNING)) {
            TimeSpanningInterface *interface = (*iter)->GetTimeSpanningInterface();
            assert(interface);
            if (interface->GetEnd()) {
                endParent = vrv_cast<Measure *>(interface->GetEnd()->GetFirstAncestor(MEASURE));
            }
        }
        if (!endParent && (*iter)->HasInterface(INTERFACE_LINKING)) {
            LinkingInterface *interface = (*iter)->GetLinkingInterface();
            assert(interface);
            if (interface->GetNextLink()) {
                // We should have one because we allow only control events (dir and dynam) to be linked as target
                TimePointInterface *nextInterface = interface->GetNextLink()->GetTimePointInterface();
                assert(nextInterface);
                endParent = vrv_cast<Measure *>(nextInterface->GetStart()->GetFirstAncestor(MEASURE));
            }
        }
        assert(endParent);
        // We have reached the end of the spanning - remove it from the list of running elements
        if (endParent == measure) {
            iter = m_timeSpanningElements.erase(iter);
        }
        else {
            ++iter;
        }
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareStaffCurrentTimeSpanningFunctor::VisitStaff(Staff *staff)
{
    for (Object *element : m_timeSpanningElements) {
        TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
        assert(interface);
        Measure *currentMeasure = vrv_cast<Measure *>(staff->GetFirstAncestor(MEASURE));
        assert(currentMeasure);
        // Special case for harm/fb/f where we are likely not to have a \@staff on /f
        // Use the parent harm to get the staff (necessary when calling IsOnStaff with timestamps)
        if (element->Is(FIGURE) && !interface->HasStaff()) {
            Object *harm = element->GetFirstAncestor(HARM);
            if (harm) interface = harm->GetTimeSpanningInterface();
            assert(interface);
        }
        // We need to make sure we are in the next measure (and not just a staff below because of some cross staff
        // notation
        if ((interface->GetStartMeasure() != currentMeasure) && (interface->IsOnStaff(staff->GetN()))) {
            staff->m_timeSpanningElements.push_back(element);
        }
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareStaffCurrentTimeSpanningFunctor::VisitSyl(Syl *syl)
{
    // Pass it to the pseudo functor of the interface
    TimeSpanningInterface *interface = syl->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfacePrepareStaffCurrentTimeSpanning(*this, syl);
}

//----------------------------------------------------------------------------
// PrepareRehPositionFunctor
//----------------------------------------------------------------------------

PrepareRehPositionFunctor::PrepareRehPositionFunctor() : Functor() {}

FunctorCode PrepareRehPositionFunctor::VisitReh(Reh *reh)
{
    if (!reh->HasStartid() && !reh->HasTstamp()) {
        Measure *measure = vrv_cast<Measure *>(reh->GetFirstAncestor(MEASURE));
        if (measure->GetLeftBarLine()) reh->SetStart(measure->GetLeftBarLine());
    }

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// PrepareBeamSpanElementsFunctor
//----------------------------------------------------------------------------

PrepareBeamSpanElementsFunctor::PrepareBeamSpanElementsFunctor() : Functor() {}

FunctorCode PrepareBeamSpanElementsFunctor::VisitBeamSpan(BeamSpan *beamSpan)
{
    if (!beamSpan->GetBeamedElements().empty() || !beamSpan->GetStart() || !beamSpan->GetEnd()) return FUNCTOR_CONTINUE;

    Layer *layer = vrv_cast<Layer *>(beamSpan->GetStart()->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>(beamSpan->GetStart()->GetFirstAncestor(STAFF));
    if (!layer || !staff) return FUNCTOR_SIBLINGS;

    ArrayOfObjects beamedElements
        = beamSpan->HasPlist() ? beamSpan->GetRefs() : this->GetBeamSpanElementList(beamSpan, layer, staff);

    beamSpan->SetBeamedElements(beamedElements);

    if (beamedElements.empty()) return FUNCTOR_SIBLINGS;

    // mark referenced elements as contained in beam span
    for (const auto element : beamedElements) {
        LayerElement *layerElem = vrv_cast<LayerElement *>(element);
        if (!layerElem) continue;

        Measure *measure = vrv_cast<Measure *>(layerElem->GetFirstAncestor(MEASURE));
        if (!measure) continue;
        layerElem->SetIsInBeamSpan(true);

        Staff *elementStaff = vrv_cast<Staff *>(layerElem->GetFirstAncestor(STAFF));
        if (!elementStaff) continue;
        if (elementStaff->GetN() != staff->GetN()) {
            Layer *elementLayer = vrv_cast<Layer *>(layerElem->GetFirstAncestor(LAYER));
            if (!elementLayer) continue;
            layerElem->m_crossStaff = elementStaff;
            layerElem->m_crossLayer = elementLayer;
        }
    }

    return FUNCTOR_CONTINUE;
}

ArrayOfObjects PrepareBeamSpanElementsFunctor::GetBeamSpanElementList(
    BeamSpan *beamSpan, Layer *layer, const Staff *staff) const
{
    // find all elements between startId and endId of the beamSpan
    ClassIdsComparison classIds({ NOTE, CHORD });
    ListOfObjects objects;
    layer->FindAllDescendantsBetween(&objects, &classIds, beamSpan->GetStart(), beamSpan->GetEnd(), true, 3);
    // To make sure that notes from tuplets and btrems are included, lookup for decendants is done up to depth of 3.
    // However this might result in notes from chords being added as standalone elements. To avoid this we remove any
    // note that is in the span and is a chord tone. Same happens when nextLayerObjects are being processed.
    objects.erase(std::remove_if(objects.begin(), objects.end(),
                      [](Object *object) { return object->Is(NOTE) && vrv_cast<Note *>(object)->IsChordTone(); }),
        objects.end());

    if (objects.empty()) return {};

    ArrayOfObjects beamSpanElements(objects.begin(), objects.end());
    // If last element is not equal to the end, there is high chance that this beamSpan is cross-measure.
    // Look for the same N-staff N-layer in next measure and try finding end there
    Measure *startMeasure = vrv_cast<Measure *>(beamSpan->GetStart()->GetFirstAncestor(MEASURE));
    Measure *endMeasure = vrv_cast<Measure *>(beamSpan->GetEnd()->GetFirstAncestor(MEASURE));
    Measure *nextMeasure = NULL;
    while ((beamSpanElements.back() != beamSpan->GetEnd()) && (startMeasure != endMeasure)) {
        Object *parent = startMeasure->GetParent();

        nextMeasure = vrv_cast<Measure *>(parent->GetNext(startMeasure, MEASURE));
        if (!nextMeasure) break;

        AttNIntegerComparison snc(STAFF, staff->GetN());
        Staff *nextStaff = vrv_cast<Staff *>(nextMeasure->FindDescendantByComparison(&snc));
        if (!nextStaff) break;

        AttNIntegerComparison lnc(LAYER, layer->GetN());
        Layer *nextStaffLayer = vrv_cast<Layer *>(nextStaff->FindDescendantByComparison(&lnc));
        if (!nextStaffLayer) break;

        // find all elements between startId and endId of the beamSpan
        ClassIdsComparison classIds({ NOTE, CHORD });
        ListOfObjects nextLayerObjects;
        // pass NULL as starting element to add all elements until end is reached
        if (endMeasure == nextMeasure) {
            nextStaffLayer->FindAllDescendantsBetween(&nextLayerObjects, &classIds, NULL, beamSpan->GetEnd(), true, 3);
            nextLayerObjects.erase(
                std::remove_if(nextLayerObjects.begin(), nextLayerObjects.end(),
                    [](Object *object) { return object->Is(NOTE) && vrv_cast<Note *>(object)->IsChordTone(); }),
                nextLayerObjects.end());
            // Handle only next measure for the time being
            if (nextLayerObjects.back() == beamSpan->GetEnd()) {
                beamSpanElements.insert(beamSpanElements.end(), nextLayerObjects.begin(), nextLayerObjects.end());
            }
        }
        else {
            nextStaffLayer->FindAllDescendantsByComparison(&nextLayerObjects, &classIds);
            beamSpanElements.insert(beamSpanElements.end(), nextLayerObjects.begin(), nextLayerObjects.end());
        }

        startMeasure = nextMeasure;
    }

    return beamSpanElements;
}

} // namespace vrv
