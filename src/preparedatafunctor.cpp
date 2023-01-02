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
#include "doc.h"
#include "dot.h"
#include "elementpart.h"
#include "f.h"
#include "harm.h"
#include "layer.h"
#include "plistinterface.h"
#include "rest.h"
#include "runningelement.h"
#include "score.h"
#include "staff.h"
#include "stem.h"
#include "syl.h"
#include "symboltable.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "timestamp.h"
#include "tuplet.h"
#include "verse.h"
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

//----------------------------------------------------------------------------
// PrepareAltSymFunctor
//----------------------------------------------------------------------------

PrepareAltSymFunctor::PrepareAltSymFunctor()
{
    m_symbolTable = NULL;
}

FunctorCode PrepareAltSymFunctor::VisitObject(Object *object)
{
    if (object->Is(SCORE)) {
        Score *score = vrv_cast<Score *>(object);
        assert(score);
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

PrepareFacsimileFunctor::PrepareFacsimileFunctor(Facsimile *facsimile)
{
    m_facsimile = facsimile;
}

FunctorCode PrepareFacsimileFunctor::VisitObject(Object *object)
{
    if (object->HasInterface(INTERFACE_FACSIMILE)) {
        FacsimileInterface *interface = object->GetFacsimileInterface();
        assert(interface);
        if (interface->HasFacs()) {
            std::string facsID = ((interface->GetFacs().compare(0, 1, "#") == 0) ? interface->GetFacs().substr(1)
                                                                                 : interface->GetFacs());
            Zone *zone = m_facsimile->FindZoneByID(facsID);
            if (zone != NULL) {
                interface->AttachZone(zone);
            }
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

PrepareLinkingFunctor::PrepareLinkingFunctor()
{
    m_fillList = true;
}

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
    if (m_fillList && object->HasInterface(INTERFACE_LINKING)) {
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
    if (m_fillList) {
        if (note->HasStemSameas()) {
            std::string idTarget = ExtractIDFragment(note->GetStemSameas());
            m_stemSameasIDPairs[idTarget] = note;
        }
    }
    // Second pass we resolve links
    else {
        const std::string id = note->GetID();
        if (m_stemSameasIDPairs.count(id)) {
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

PreparePlistFunctor::PreparePlistFunctor()
{
    m_fillList = true;
}

void PreparePlistFunctor::InsertInterfaceIDTuple(const std::string &elementID, PlistInterface *interface)
{
    m_interfaceIDTuples.push_back(std::make_tuple(interface, elementID, (Object *)NULL));
}

FunctorCode PreparePlistFunctor::VisitObject(Object *object)
{
    if (m_fillList) {
        if (object->HasInterface(INTERFACE_PLIST)) {
            PlistInterface *interface = object->GetPlistInterface();
            assert(interface);
            return interface->InterfacePreparePlist(*this, object);
        }
    }
    else {
        if (!object->IsLayerElement()) return FUNCTOR_CONTINUE;

        std::string id = object->GetID();
        auto i = std::find_if(m_interfaceIDTuples.begin(), m_interfaceIDTuples.end(),
            [&id](std::tuple<PlistInterface *, std::string, Object *> tuple) { return (std::get<1>(tuple) == id); });
        if (i != m_interfaceIDTuples.end()) {
            std::get<2>(*i) = object;
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareDurationFunctor
//----------------------------------------------------------------------------

PrepareDurationFunctor::PrepareDurationFunctor()
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
            if (m_durDefaultForStaffN.count(staff->GetN()) > 0) {
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

PrepareTimePointingFunctor::PrepareTimePointingFunctor() {}

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

PrepareTimeSpanningFunctor::PrepareTimeSpanningFunctor()
{
    m_fillList = true;
}

void PrepareTimeSpanningFunctor::InsertInterfaceOwnerPair(Object *owner, TimeSpanningInterface *interface)
{
    m_timeSpanningInterfaces.push_back({ interface, owner });
}

FunctorCode PrepareTimeSpanningFunctor::VisitF(F *f)
{
    // Pass it to the pseudo functor of the interface
    TimeSpanningInterface *interface = f->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfacePrepareTimeSpanning(*this, f);
}

FunctorCode PrepareTimeSpanningFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    // Pass it to the pseudo functor of the interface
    if (floatingObject->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = floatingObject->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfacePrepareTimeSpanning(*this, floatingObject);
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

FunctorCode PrepareTimeSpanningFunctor::VisitMeasureEnd(Measure *measure)
{
    if (!m_fillList) {
        return FUNCTOR_CONTINUE;
    }

    ListOfSpanningInterOwnerPairs::iterator iter = m_timeSpanningInterfaces.begin();
    while (iter != m_timeSpanningInterfaces.end()) {
        // At the end of the measure (going backward) we remove elements for which we do not need to match the end (for
        // now). Eventually, we could consider them, for example if we want to display their spanning or for improved
        // midi output
        if (iter->second->GetClassId() == HARM) {
            iter = m_timeSpanningInterfaces.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// PrepareTimestampsFunctor
//----------------------------------------------------------------------------

PrepareTimestampsFunctor::PrepareTimestampsFunctor() {}

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

    Measure *lastMeasure = dynamic_cast<Measure *>(doc->FindDescendantByType(MEASURE, UNLIMITED_DEPTH, BACKWARD));
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
// PreparePointersByLayerFunctor
//----------------------------------------------------------------------------

PreparePointersByLayerFunctor::PreparePointersByLayerFunctor()
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

//----------------------------------------------------------------------------
// PrepareLyricsFunctor
//----------------------------------------------------------------------------

PrepareLyricsFunctor::PrepareLyricsFunctor()
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
    Verse *verse = dynamic_cast<Verse *>(syl->GetFirstAncestor(VERSE, MAX_NOTE_DEPTH));
    if (verse) {
        syl->m_drawingVerse = std::max(verse->GetN(), 1);
    }

    syl->SetStart(dynamic_cast<LayerElement *>(syl->GetFirstAncestor(NOTE, MAX_NOTE_DEPTH)));
    // If there isn't an ancestor note, it should be a chord
    if (!syl->GetStart()) {
        syl->SetStart(dynamic_cast<LayerElement *>(syl->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH)));
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
            if (m_currentSyl->GetStart() == m_penultimateNoteOrChord)
                LogWarning("Syllable with underline extender under one single note '%s'",
                    m_currentSyl->GetStart()->GetID().c_str());
            else
                m_currentSyl->SetEnd(m_penultimateNoteOrChord);
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

PrepareLayerElementPartsFunctor::PrepareLayerElementPartsFunctor() {}

FunctorCode PrepareLayerElementPartsFunctor::VisitChord(Chord *chord)
{
    Stem *currentStem = dynamic_cast<Stem *>(chord->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    if (currentStem) currentFlag = dynamic_cast<Flag *>(currentStem->GetFirst(FLAG));

    if (!currentStem) {
        currentStem = new Stem();
        currentStem->IsAttribute(true);
        chord->AddChild(currentStem);
    }
    currentStem->AttGraced::operator=(*chord);
    currentStem->FillAttributes(*chord);

    int duration = chord->GetNoteOrChordDur(chord);
    if ((duration < DUR_2) || (chord->GetStemVisible() == BOOLEAN_false)) {
        currentStem->IsVirtual(true);
    }

    if ((duration > DUR_4) && !chord->IsInBeam() && !chord->GetAncestorFTrem()) {
        // We should have a stem at this stage
        assert(currentStem);
        if (!currentFlag) {
            currentFlag = new Flag();
            currentStem->AddChild(currentFlag);
        }
    }
    // This will happen only if the duration has changed (no flag required anymore)
    else if (currentFlag) {
        assert(currentStem);
        if (currentStem->DeleteChild(currentFlag)) currentFlag = NULL;
    }

    chord->SetDrawingStem(currentStem);

    // Calculate chord clusters
    chord->CalculateClusters();

    // Also set the drawing stem object (or NULL) to all child notes
    const ListOfObjects &childList = chord->GetList(chord);
    for (ListOfObjects::const_iterator it = childList.begin(); it != childList.end(); ++it) {
        assert((*it)->Is(NOTE));
        Note *note = vrv_cast<Note *>(*it);
        assert(note);
        note->SetDrawingStem(currentStem);
    }

    /************ dots ***********/

    Dots *currentDots = dynamic_cast<Dots *>(chord->FindDescendantByType(DOTS, 1));

    if (chord->GetDots() > 0) {
        if (!currentDots) {
            currentDots = new Dots();
            chord->AddChild(currentDots);
        }
        currentDots->AttAugmentDots::operator=(*chord);
    }
    // This will happen only if the duration has changed
    else if (currentDots) {
        if (chord->DeleteChild(currentDots)) {
            currentDots = NULL;
        }
    }

    /************ Prepare the drawing cue size ************/

    PrepareCueSizeFunctor prepareCueSize;
    chord->Process(prepareCueSize);

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareLayerElementPartsFunctor::VisitNote(Note *note)
{
    Stem *currentStem = dynamic_cast<Stem *>(note->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    Chord *chord = note->IsChordTone();
    if (currentStem) currentFlag = dynamic_cast<Flag *>(currentStem->GetFirst(FLAG));

    if (!note->IsChordTone() && !note->IsTabGrpNote()) {
        if (!currentStem) {
            currentStem = new Stem();
            currentStem->IsAttribute(true);
            note->AddChild(currentStem);
        }
        currentStem->AttGraced::operator=(*note);
        currentStem->FillAttributes(*note);

        if (note->GetActualDur() < DUR_2 || (note->GetStemVisible() == BOOLEAN_false)) {
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

    // We don't care about flags or dots in mensural notes
    if (note->IsMensuralDur()) return FUNCTOR_CONTINUE;

    if ((note->GetActualDur() > DUR_4) && !note->IsInBeam() && !note->GetAncestorFTrem() && !note->IsChordTone()
        && !note->IsTabGrpNote()) {
        // We should have a stem at this stage
        assert(currentStem);
        if (!currentFlag) {
            currentFlag = new Flag();
            currentStem->AddChild(currentFlag);
        }
    }
    // This will happen only if the duration has changed (no flag required anymore)
    else if (currentFlag) {
        assert(currentStem);
        if (currentStem->DeleteChild(currentFlag)) currentFlag = NULL;
    }

    if (!chord) note->SetDrawingStem(currentStem);

    /************ dots ***********/

    Dots *currentDots = dynamic_cast<Dots *>(note->FindDescendantByType(DOTS, 1));

    if (note->GetDots() > 0) {
        if (chord && (chord->GetDots() == note->GetDots())) {
            LogWarning(
                "Note '%s' with a @dots attribute with the same value as its chord parent", note->GetID().c_str());
        }
        if (!currentDots) {
            currentDots = new Dots();
            note->AddChild(currentDots);
        }
        currentDots->AttAugmentDots::operator=(*note);
    }
    // This will happen only if the duration has changed
    else if (currentDots) {
        if (note->DeleteChild(currentDots)) {
            currentDots = NULL;
        }
    }

    /************ Prepare the drawing cue size ************/

    PrepareCueSizeFunctor prepareCueSize;
    note->Process(prepareCueSize);

    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareLayerElementPartsFunctor::VisitRest(Rest *rest)
{
    Dots *currentDots = dynamic_cast<Dots *>(rest->FindDescendantByType(DOTS, 1));

    if ((rest->GetDur() > DUR_BR) && (rest->GetDots() > 0)) {
        if (!currentDots) {
            currentDots = new Dots();
            rest->AddChild(currentDots);
        }
        currentDots->AttAugmentDots::operator=(*rest);
    }
    // This will happen only if the duration has changed
    else if (currentDots) {
        if (rest->DeleteChild(currentDots)) {
            currentDots = NULL;
        }
    }

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

    if (!currentStem) {
        currentStem = new Stem();
        currentStem->IsAttribute(true);
        tabDurSym->AddChild(currentStem);
    }
    tabDurSym->SetDrawingStem(currentStem);

    /************ flags ***********/

    TabGrp *tabGrp = vrv_cast<TabGrp *>(tabDurSym->GetFirstAncestor(TABGRP));
    assert(tabGrp);

    // No flag within beam for durations longer than 8th notes
    if (!tabDurSym->IsInBeam() && tabGrp->GetActualDur() > DUR_4) {
        // We must have a stem at this stage
        assert(currentStem);
        if (!currentFlag) {
            currentFlag = new Flag();
            currentStem->AddChild(currentFlag);
        }
    }
    // This will happen only if the duration has changed (no flag required anymore)
    else if (currentFlag) {
        assert(currentStem);
        if (currentStem->DeleteChild(currentFlag)) currentFlag = NULL;
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode PrepareLayerElementPartsFunctor::VisitTuplet(Tuplet *tuplet)
{
    TupletBracket *currentBracket = dynamic_cast<TupletBracket *>(tuplet->FindDescendantByType(TUPLET_BRACKET, 1));
    TupletNum *currentNum = dynamic_cast<TupletNum *>(tuplet->FindDescendantByType(TUPLET_NUM, 1));

    bool beamed = false;
    // Are we contained in a beam?
    if (tuplet->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH)) {
        // is only the tuplet beamed? (will not work with nested tuplets)
        Beam *currentBeam = dynamic_cast<Beam *>(tuplet->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
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
    tuplet->SetDrawingLeft(dynamic_cast<LayerElement *>(tuplet->FindDescendantByComparison(&comparison)));
    tuplet->SetDrawingRight(
        dynamic_cast<LayerElement *>(tuplet->FindDescendantByComparison(&comparison, UNLIMITED_DEPTH, BACKWARD)));

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
