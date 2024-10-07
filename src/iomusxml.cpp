/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.cpp
// Author:      Laurent Pugin and Klaus Rettinghaus
// Created:     22/09/2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iomusxml.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <numeric>
#include <regex>
#include <sstream>

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "beam.h"
#include "beamspan.h"
#include "beatrpt.h"
#include "bracketspan.h"
#include "breath.h"
#include "btrem.h"
#include "caesura.h"
#include "chord.h"
#include "clef.h"
#include "comparison.h"
#include "course.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "ending.h"
#include "f.h"
#include "fb.h"
#include "fermata.h"
#include "fing.h"
#include "ftrem.h"
#include "gliss.h"
#include "grpsym.h"
#include "hairpin.h"
#include "harm.h"
#include "instrdef.h"
#include "keyaccid.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "lb.h"
#include "lv.h"
#include "mdiv.h"
#include "measure.h"
#include "metersiggrp.h"
#include "mnum.h"
#include "mordent.h"
#include "mrest.h"
#include "mrpt.h"
#include "mspace.h"
#include "multirest.h"
#include "note.h"
#include "octave.h"
#include "pb.h"
#include "pedal.h"
#include "pgfoot.h"
#include "pghead.h"
#include "reh.h"
#include "rend.h"
#include "rest.h"
#include "sb.h"
#include "score.h"
#include "section.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "syl.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "trill.h"
#include "tuning.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

// Using flags mordent can be easily visualized with the value. E.g.
// 0x212 - approach and depart are both below and form is normal
// APPR_Below | FORM_Normal | DEP_Below
enum MordentExtSymbolFlags {
    APPR_Above = 0x100,
    APPR_Below = 0x200,
    FORM_Normal = 0x10,
    FORM_Inverted = 0x20,
    DEP_Above = 0x1,
    DEP_Below = 0x2
};

enum class MetronomeElements { BEAT_UNIT, BEAT_UNIT_DOT, PER_MINUTE, SEPARATOR };

//----------------------------------------------------------------------------
// MusicXmlInput
//----------------------------------------------------------------------------

MusicXmlInput::MusicXmlInput(Doc *doc) : Input(doc) {}

MusicXmlInput::~MusicXmlInput()
{
    // Clean the collected garbage
    for (Object *object : m_garbage) {
        delete object;
    }
}

#ifndef NO_MUSICXML_SUPPORT

bool MusicXmlInput::Import(const std::string &musicxml)
{
    try {
        m_doc->Reset();
        m_doc->SetType(Raw);
        pugi::xml_document xmlDoc;
        xmlDoc.load_string(musicxml.c_str());
        pugi::xml_node root = xmlDoc.first_child();
        return ReadMusicXml(root);
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////
// XML helpers

bool MusicXmlInput::HasAttributeWithValue(
    const pugi::xml_node node, const std::string &attribute, const std::string &value) const
{
    assert(node);

    if (node.attribute(attribute.c_str()).value() == value) {
        return true;
    }
    return false;
}

bool MusicXmlInput::IsElement(const pugi::xml_node node, const std::string &name) const
{
    assert(node);

    if (std::string(node.name()) == name) {
        return true;
    }
    return false;
}

bool MusicXmlInput::HasContentWithValue(const pugi::xml_node node, const std::string &value) const
{
    assert(node);

    if (GetContent(node) == value) {
        return true;
    }
    return false;
}

std::string MusicXmlInput::GetContent(const pugi::xml_node node) const
{
    assert(node);

    if (node.text()) {
        return std::string(node.text().as_string());
    }
    return "";
}

std::string MusicXmlInput::GetContentOfChild(const pugi::xml_node node, const std::string &child) const
{
    pugi::xpath_node childNode = node.select_node(child.c_str());
    if (childNode.node()) {
        return GetContent(childNode.node());
    }
    return "";
}

void MusicXmlInput::ProcessClefChangeQueue(Section *section)
{
    while (!m_clefChangeQueue.empty()) {
        musicxml::ClefChange clefChange = m_clefChangeQueue.front();
        m_clefChangeQueue.pop();
        AttNNumberLikeComparison comparisonMeasure(MEASURE, clefChange.m_measureNum);
        Measure *currentMeasure = vrv_cast<Measure *>(section->FindDescendantByComparison(&comparisonMeasure));
        if (!currentMeasure) {
            LogWarning("MusicXML import: Clef change at measure %s, staff %d, time %d not inserted",
                clefChange.m_measureNum.c_str(), clefChange.m_staff->GetN(), clefChange.m_scoreOnset);
            delete clefChange.m_clef;
            continue;
        }
        if (!clefChange.m_scoreOnset && !clefChange.m_afterBarline) {
            Measure *previousMeasure = NULL;
            // First try to check whether current measure already exists in the section. Since *measure might
            // not match with the one in the section, comparison search should be done
            previousMeasure = vrv_cast<Measure *>(section->GetPrevious(currentMeasure, MEASURE));
            if (!previousMeasure) {
                AddClefs(currentMeasure, clefChange);
                continue;
            }
            AttNIntegerComparison comparisonStaff(STAFF, clefChange.m_staff->GetN());
            Staff *previousStaff = vrv_cast<Staff *>(previousMeasure->FindDescendantByComparison(&comparisonStaff));
            if (previousStaff == NULL) {
                AddClefs(currentMeasure, clefChange);
                continue;
            }
            Layer *previousLayer
                = vrv_cast<Layer *>(previousStaff->FindDescendantByType(LAYER, UNLIMITED_DEPTH, BACKWARD));
            if (previousLayer == NULL) {
                AddClefs(currentMeasure, clefChange);
            }
            else {
                // For previous measure we need to make sure that clef is set at the end, so pass high duration value
                // (since it won't matter there) and set measureNum to empty, since it doesn't matter as well
                int endDuration = m_ppq;
                for (int &num : m_meterCount) endDuration *= num;
                musicxml::ClefChange previousClefChange(
                    std::string(""), previousStaff, previousLayer, clefChange.m_clef, endDuration, false);
                AddClefs(previousMeasure, previousClefChange);
            }
        }
        else {
            AddClefs(currentMeasure, clefChange);
        }
    }
}

void MusicXmlInput::AddClefs(Measure *measure, const musicxml::ClefChange &clefChange)
{
    // For both measure and staff make sure that corresponding staff/layer is actually a child of it
    int idx = measure->GetChildIndex(clefChange.m_staff);
    if (idx != -1) {
        idx = clefChange.m_staff->GetChildIndex(clefChange.m_layer);
        if (!clefChange.m_layer) {
            Layer *firstLayer = vrv_cast<Layer *>(clefChange.m_staff->GetChild(0, LAYER));
            if (firstLayer) {
                InsertClefToLayer(clefChange.m_staff, firstLayer, clefChange.m_clef, clefChange.m_scoreOnset);
            }
        }
        else if (idx != -1) {
            InsertClefToLayer(clefChange.m_staff, clefChange.m_layer, clefChange.m_clef, clefChange.m_scoreOnset);
        }
        else {
            // If staff doesn't have fitting layer for the clef but has layer with mSpace - this should be a case with
            // cross-staff clef. Remove mSpace in this case and add clef to it instead, filling space if required
            Object *mSpace = clefChange.m_staff->FindDescendantByType(MSPACE);
            if (mSpace) {
                Layer *parentLayer = dynamic_cast<Layer *>(mSpace->GetParent());
                if (mSpace && parentLayer) {
                    parentLayer->DeleteChild(mSpace);
                    m_elementStackMap[parentLayer] = {};
                    FillSpace(parentLayer, clefChange.m_scoreOnset);
                    parentLayer->AddChild(clefChange.m_clef);
                }
            }
            else {
                Layer *firstLayer = vrv_cast<Layer *>(clefChange.m_staff->GetChild(0, LAYER));
                if (firstLayer) {
                    InsertClefToLayer(clefChange.m_staff, firstLayer, clefChange.m_clef, clefChange.m_scoreOnset);
                }
            }
        }
    }
}

void MusicXmlInput::InsertClefToLayer(Staff *staff, Layer *layer, Clef *clef, int scoreOnset)
{
    // Since AddClef handles #sameas clef only for the future layers, we need to check any previous existing layers for
    // the same staff to see if we need to insert #sameas clef to them.
    ListOfObjects staffLayers = staff->FindAllDescendantsByType(LAYER, false);
    for (const auto listLayer : staffLayers) {
        Layer *otherLayer = vrv_cast<Layer *>(listLayer);
        if (m_layerTimes.find(otherLayer) == m_layerTimes.end()) continue;
        // Get first element for the same (or higher if same is not present) duration
        const auto start = m_layerTimes.at(otherLayer).lower_bound(scoreOnset);
        // Add either clef or #sameas, depending on the layer we're adding to
        Clef *clefToAdd = NULL;
        if (listLayer == layer) {
            clefToAdd = clef;
        }
        else {
            clefToAdd = new Clef();
            clefToAdd->SetSameas("#" + clef->GetID());
        }

        // In case scoreOnset is 0 - add clef before the first element
        if (!scoreOnset) {
            InsertClefIntoObject(start->second, clefToAdd, otherLayer, scoreOnset, false);
        }
        else {
            // If corresponding time couldn't be found (i.e. it's higher than any other duration in the layer) - add
            // clef to the end of the layer
            if (start == m_layerTimes.at(otherLayer).end()) {
                otherLayer->AddChild(clefToAdd);
                m_layerTimes.at(otherLayer).emplace(std::prev(m_layerTimes.at(otherLayer).end())->first, clefToAdd);
            }
            else {
                // Always try to add clefs at the end of current duration, to honor their order in the musicxml
                const int actualScoreOnSet = start->first;
                auto end = m_layerTimes.at(otherLayer).upper_bound(actualScoreOnSet);
                LayerElement *layerElement = (--end)->second;
                InsertClefIntoObject(layerElement, clefToAdd, otherLayer, scoreOnset, true);
            }
        }
    }
}

void MusicXmlInput::InsertClefIntoObject(
    Object *layerElement, Clef *clef, Layer *layer, int scoreOnset, bool insertAfter)
{
    if (layerElement->GetParent()->Is(LAYER)) {
        InsertClefIntoObject(layer, clef, layerElement, insertAfter);
        m_layerTimes.at(layer).emplace(scoreOnset, clef);
    }
    else {
        Object *parent = layerElement->GetParent();
        if (parent->Is({ CHORD, FTREM, TABGRP })) {
            InsertClefIntoObject(parent->GetParent(), clef, parent, insertAfter);
        }
        else {
            InsertClefIntoObject(parent, clef, layerElement, insertAfter);
        }
    }
}

void MusicXmlInput::InsertClefIntoObject(Object *parent, Clef *clef, Object *layerElement, bool insertAfter)
{
    if (parent->GetChildIndex(layerElement) == -1) return;
    if (insertAfter) {
        parent->InsertAfter(layerElement, clef);
    }
    else {
        parent->InsertBefore(layerElement, clef);
    }
}

void MusicXmlInput::AddMeasure(Section *section, Measure *measure, int i)
{
    assert(section);
    assert(i >= 0);

    Measure *contentMeasure = NULL;

    // we just need to add a measure
    if (section->GetChildCount(MEASURE) <= i - GetMrestMeasuresCountBeforeIndex(i)) {
        section->AddChild(measure);
        contentMeasure = measure;
    }
    // otherwise copy the content to the corresponding existing measure
    else {
        Measure *existingMeasure = NULL;
        // Search by measure number first
        ListOfObjects matchingMeasures;
        AttNNumberLikeComparison comparisonMeasure(MEASURE, measure->GetN());
        section->FindAllDescendantsByComparison(&matchingMeasures, &comparisonMeasure, 1);
        // For now take the first match
        if (!matchingMeasures.empty()) {
            existingMeasure = vrv_cast<Measure *>(matchingMeasures.front());
        }
        // Prefer any measure with matching index (measure numbers might be non-unique)
        for (Object *object : matchingMeasures) {
            Measure *matchingMeasure = vrv_cast<Measure *>(object);
            if (m_measureCounts.at(matchingMeasure) == i) {
                existingMeasure = matchingMeasure;
                break;
            }
        }
        if (existingMeasure) {
            for (auto current : measure->GetChildren()) {
                if (!current->Is(STAFF)) {
                    continue;
                }
                current->MoveItselfTo(existingMeasure);
            }
            measure->ClearRelinquishedChildren();
        }
        else {
            // The measure was not transferred and not added to the tree. Its entire content will be deleted at the end.
            LogError("MusicXML import: Mismatching measure number %s", measure->GetN().c_str());
        }
        contentMeasure = existingMeasure;

        m_measureCounts.erase(measure);
        m_garbage.push_back(measure);
    }

    // Handle endings
    if (contentMeasure && NotInEndingStack(contentMeasure)) {
        if (m_currentEndingStart) {
            // Create a new ending
            std::vector<Measure *> measures;
            m_endingStack.push_back({ measures, *m_currentEndingStart });
        }
        if (!m_endingStack.empty() && (m_endingStack.back().second.m_endingType == "start")) {
            // Append the current measure
            m_endingStack.back().first.push_back(contentMeasure);
        }
        if (m_currentEndingStop && !m_endingStack.empty()) {
            // Stop the last ending
            m_endingStack.back().second.m_endingType = m_currentEndingStop->m_endingType;
        }
    }
    m_currentEndingStart.reset();
    m_currentEndingStop.reset();
}

void MusicXmlInput::AddLayerElement(Layer *layer, LayerElement *element, int duration)
{
    assert(layer);
    assert(element);

    int currTime = 0;
    if (m_layerEndTimes.count(layer) > 0) currTime = m_layerEndTimes.at(layer);
    if ((layer->GetChildren().size() == 0 && m_durTotal > 0) || currTime < m_durTotal) {
        FillSpace(layer, m_durTotal - currTime);
    }

    if (m_elementStackMap.at(layer).empty()) {
        layer->AddChild(element);
    }
    else {
        m_elementStackMap.at(layer).back()->AddChild(element);
    }
    // Recheck if AddChild was successful
    if (!element->GetParent()) return;

    m_layerEndTimes[layer] = m_durTotal + duration;
    if (!element->Is({ BEAM, TUPLET })) {
        m_layerTimes[layer].emplace(m_durTotal + duration, element);
    }
}

Layer *MusicXmlInput::SelectLayer(pugi::xml_node node, Measure *measure)
{
    // If value is initialized - get current layer
    if (m_isLayerInitialized) return m_currentLayer;

    // Find voice number of node
    short int layerNum = (node.child("voice")) ? node.child("voice").text().as_int() : 1;
    if (layerNum < 1) {
        LogWarning("MusicXML import: Layer %d cannot be found", layerNum);
        layerNum = 1;
    }

    // If not initialized and layer is not set - get first layer in the first staff
    if (!m_currentLayer) {
        Staff *staff = vrv_cast<Staff *>(measure->GetChild(0, STAFF));
        assert(staff);
        m_currentLayer = SelectLayer(layerNum, staff);
        m_isLayerInitialized = true;
        return m_currentLayer;
    }

    // if not, take staff info of node element
    short int staffNum = (node.child("staff")) ? node.child("staff").text().as_int() : 1;
    if ((staffNum < 1) || (staffNum > measure->GetChildCount(STAFF))) {
        LogWarning("MusicXML import: Staff %d cannot be found", staffNum);
        staffNum = 1;
    }
    staffNum--;
    Staff *staff = vrv_cast<Staff *>(measure->GetChild(staffNum, STAFF));
    assert(staff);
    m_currentLayer = SelectLayer(layerNum, staff);
    m_isLayerInitialized = true;
    return m_currentLayer;
}

Layer *MusicXmlInput::SelectLayer(short int staffNum, Measure *measure)
{
    staffNum--;
    Staff *staff = vrv_cast<Staff *>(measure->GetChild(staffNum, STAFF));
    assert(staff);
    // layer -1 means the first one
    return SelectLayer(-1, staff);
}

Layer *MusicXmlInput::SelectLayer(short int layerNum, Staff *staff)
{
    Layer *layer = NULL;
    // no layer specified, return the first one (if any)
    if (layerNum == -1) {
        if (staff->GetChildCount() > 0) {
            layer = dynamic_cast<Layer *>(staff->GetChild(0));
        }
        // otherwise set @n to 1
        layerNum = 1;
    }
    else {
        AttNIntegerComparison comparisonLayer(LAYER, layerNum);
        layer = vrv_cast<Layer *>(staff->FindDescendantByComparison(&comparisonLayer, 1));
    }
    if (layer) return layer;
    // else add it
    // add at least one layer
    layer = new Layer();
    layer->SetN(layerNum);
    staff->AddChild(layer);
    std::vector<LayerElement *> stack;
    m_elementStackMap[layer] = stack;
    return layer;
}

void MusicXmlInput::RemoveLastFromStack(ClassId classId, Layer *layer)
{
    std::vector<LayerElement *>::reverse_iterator riter;
    for (riter = m_elementStackMap.at(layer).rbegin(); riter != m_elementStackMap.at(layer).rend(); ++riter) {
        if ((*riter)->Is(classId)) {
            m_elementStackMap.at(layer).erase((riter + 1).base());
            return;
        }
    }
}

bool MusicXmlInput::IsInStack(ClassId classId, Layer *layer)
{
    return (m_elementStackMap.at(layer).end()
        != std::find_if(m_elementStackMap.at(layer).begin(), m_elementStackMap.at(layer).end(),
            [classId](LayerElement *element) { return element->Is(classId); }));
}

void MusicXmlInput::FillSpace(Layer *layer, int dur)
{
    assert(layer);

    std::string durStr;
    while (dur > 0) {
        double quarters = (double)dur / (double)m_ppq;
        quarters = pow(2, floor(log(quarters) / log(2)));
        // limit space for now
        if (quarters > 2) quarters = 2;
        durStr = std::to_string(int(4 / quarters));

        Space *space = new Space();
        space->SetDur(space->AttDurationLog::StrToDuration(durStr));
        space->SetDurPpq(m_ppq * quarters);
        if (m_elementStackMap.at(layer).empty()) {
            layer->AddChild(space);
        }
        else {
            m_elementStackMap.at(layer).back()->AddChild(space);
        }
        m_layerTimes[layer].emplace(dur, space);
        dur -= m_ppq * quarters;
    }
}

void MusicXmlInput::GenerateID(pugi::xml_node node)
{
    std::string id = std::string(node.name()).at(0) + Object::GenerateHashID();
    node.append_attribute("xml:id").set_value(id.c_str());
}

//////////////////////////////////////////////////////////////////////////////
// Tie and slurs stack management

void MusicXmlInput::OpenTie(Note *note, Tie *tie, int layerNum)
{
    tie->SetStartid("#" + note->GetID());
    m_tieStack.push_back(musicxml::OpenTie(tie, note, layerNum));
}

void MusicXmlInput::CloseTie(Note *note, int layerNum)
{
    // add all notes with identical pitch/oct to m_tieStopStack
    for (auto iter = m_tieStack.begin(); iter != m_tieStack.end(); ++iter) {
        if (note->IsEnharmonicWith(iter->m_note)) {
            m_tieStopStack.push_back(musicxml::CloseTie(note, layerNum));
        }
    }
}

void MusicXmlInput::OpenSlur(Measure *measure, short int number, Slur *slur, curvature_CURVEDIR dir)
{
    // try to match open slur with slur stops within that measure
    for (auto iter = m_slurStopStack.begin(); iter != m_slurStopStack.end(); ++iter) {
        if ((iter->second.m_number == number) && ((iter->second.m_measureNum).compare(measure->GetN()) == 0)) {
            slur->SetEndid("#" + iter->first->GetID());
            slur->SetCurvedir(CombineCurvedir(dir, iter->second.m_curvedir));
            m_slurStopStack.erase(iter);
            return;
        }
    }
    // create new slur otherwise
    musicxml::OpenSlur openSlur(measure->GetN(), number, dir);
    m_slurStack.push_back({ slur, openSlur });
}

void MusicXmlInput::CloseSlur(Measure *measure, short int number, LayerElement *element, curvature_CURVEDIR dir)
{
    // try to match slur stop to open slurs by slur number
    std::vector<std::pair<Slur *, musicxml::OpenSlur>>::reverse_iterator riter;
    for (riter = m_slurStack.rbegin(); riter != m_slurStack.rend(); ++riter) {
        if (riter->second.m_number == number) {
            riter->first->SetEndid("#" + element->GetID());
            riter->first->SetCurvedir(CombineCurvedir(riter->second.m_curvedir, dir));
            m_slurStack.erase(std::next(riter).base());
            return;
        }
    }
    // add to m_slurStopStack, if not able to be closed
    musicxml::CloseSlur closeSlur(measure->GetN(), number, dir);
    m_slurStopStack.push_back({ element, closeSlur });
}

void MusicXmlInput::CloseBeamSpan(Staff *staff, Layer *layer, LayerElement *element)
{
    for (auto riter = m_beamspanStack.rbegin(); riter != m_beamspanStack.rend(); ++riter) {
        if ((riter->second.first == staff->GetN()) || (riter->second.second == layer->GetN())) {
            riter->first->SetEndid("#" + element->GetID());
            m_beamspanStack.erase(std::next(riter).base());
            return;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// Text rendering

std::string MusicXmlInput::GetWordsOrDynamicsText(const pugi::xml_node node) const
{
    if (IsElement(node, "words")) {
        return GetContent(node);
    }
    if (IsElement(node, "dynamics")) {
        std::string dynamStr;
        for (pugi::xml_node xmlDynamPart : node.children()) {
            if (std::string(xmlDynamPart.name()) == "other-dynamics") {
                if (xmlDynamPart != node.first_child()) dynamStr += " ";
                dynamStr += xmlDynamPart.text().as_string();
                if (xmlDynamPart != node.last_child()) dynamStr += " ";
            }
            else {
                dynamStr += xmlDynamPart.name();
            }
        }
        return dynamStr;
    }
    if (IsElement(node, "coda")) {
        return "\xF0\x9D\x84\x8C";
    }
    if (IsElement(node, "segno")) {
        return "\xF0\x9D\x84\x8B";
    }
    return std::string();
}

void MusicXmlInput::TextRendition(const pugi::xpath_node_set words, ControlElement *element) const
{
    for (pugi::xpath_node_set::const_iterator it = words.begin(); it != words.end(); ++it) {
        pugi::xml_node textNode = it->node();
        pugi::xml_node soundNode = textNode.parent().next_sibling("sound");
        std::string textStr = GetWordsOrDynamicsText(textNode);
        std::string textColor = textNode.attribute("color").as_string();
        Object *textParent = element;
        if (textNode.attribute("xml:lang") || textNode.attribute("xml:space") || textNode.attribute("color")
            || textNode.attribute("halign") || textNode.attribute("font-family") || textNode.attribute("font-style")
            || textNode.attribute("font-weight") || textNode.attribute("enclosure")) {
            Rend *rend = new Rend();
            rend->SetLang(textNode.attribute("xml:lang").as_string());
            rend->SetColor(textNode.attribute("color").as_string());
            rend->SetHalign(
                rend->AttHorizontalAlign::StrToHorizontalalignment(textNode.attribute("halign").as_string()));
            rend->SetSpace(textNode.attribute("xml:space").as_string());
            rend->SetFontfam(textNode.attribute("font-family").as_string());
            rend->SetFontstyle(rend->AttTypography::StrToFontstyle(textNode.attribute("font-style").as_string()));
            rend->SetFontweight(rend->AttTypography::StrToFontweight(textNode.attribute("font-weight").as_string()));
            rend->SetRend(ConvertEnclosure(textNode.attribute("enclosure").as_string()));
            element->AddChild(rend);
            textParent = rend;
        }
        else if (soundNode && !(soundNode.attribute("dynamics") || soundNode.attribute("tempo"))) {
            Rend *rend = new Rend();
            rend->SetHalign(HORIZONTALALIGNMENT_right);
            element->AddChild(rend);
            textParent = rend;
        }
        // Whitespace line breaks are significant in MusicXML => split into lines
        std::stringstream sstream(textStr);
        std::string line;
        bool firstLine = true;
        while (std::getline(sstream, line)) {
            if (!firstLine) {
                textParent->AddChild(new Lb());
            }
            Text *text = new Text();
            text->SetText(UTF8to32(line));
            textParent->AddChild(text);
            firstLine = false;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// Style part and group names

std::string MusicXmlInput::StyleLabel(pugi::xml_node display)
{
    std::string displayText;
    for (pugi::xml_node child : display.children()) {
        if (!std::strncmp(child.name(), "display", 7)) displayText += child.text().as_string();
        if (!std::strncmp(child.name(), "accidental", 10)) displayText += ConvertFigureGlyph(child.text().as_string());
    }
    return displayText;
}

//////////////////////////////////////////////////////////////////////////////
// Print Metronome

void MusicXmlInput::PrintMetronome(pugi::xml_node metronome, Tempo *tempo)
{
    std::string rawText;
    bool paren = false;
    if (metronome.attribute("parentheses").as_bool()) {
        Text *text = new Text();
        text->SetText(UTF8to32("("));
        tempo->AddChild(text);
        paren = true;
    }

    // build a sequence based on the elements present in the metronome
    std::list<std::pair<MetronomeElements, std::string>> metronomeElements;
    for (pugi::xml_node child : metronome.children()) {
        if ("beat-unit-dot" == std::string(child.name())) {
            metronomeElements.emplace_back(std::make_pair(MetronomeElements::BEAT_UNIT_DOT, ""));
        }
        else if ("beat-unit" == std::string(child.name())) {
            if (!metronomeElements.empty()) {
                metronomeElements.emplace_back(std::make_pair(MetronomeElements::SEPARATOR, " = "));
            }
            metronomeElements.emplace_back(std::make_pair(MetronomeElements::BEAT_UNIT, child.text().as_string()));
        }
        else if ("per-minute" == std::string(child.name())) {
            if (!metronomeElements.empty()) {
                metronomeElements.emplace_back(std::make_pair(MetronomeElements::SEPARATOR, " = "));
            }
            metronomeElements.emplace_back(std::make_pair(MetronomeElements::PER_MINUTE, child.text().as_string()));
        }
    }

    bool start = true;
    // process metronome element sequence
    for (auto iter = metronomeElements.begin(); iter != metronomeElements.end(); ++iter) {
        switch (iter->first) {
            case MetronomeElements::BEAT_UNIT: {
                std::u32string verovioText = ConvertTypeToVerovioText(iter->second);
                // find separator or use end() if there is no separator
                const auto separator = std::find_if(iter, metronomeElements.end(),
                    [](const auto pair) { return pair.first == MetronomeElements::SEPARATOR; });
                const short int dotCount = (short int)std::count_if(
                    iter, separator, [](const auto pair) { return pair.first == MetronomeElements::BEAT_UNIT_DOT; });
                for (short int i = 0; i < dotCount; ++i) {
                    verovioText += U"\xE1E7"; // SMUFL augmentation dot
                }
                // set @mmUnit and @mmDots attributes only based on the first beat-unit in the sequence
                if (start) {
                    tempo->SetMmUnit(ConvertTypeToDur(iter->second));
                    if (dotCount) tempo->SetMmDots(dotCount);
                    start = false;
                }
                if (!verovioText.empty()) {
                    Rend *rend = new Rend;
                    rend->SetGlyphAuth("smufl");
                    Text *text = new Text();
                    text->SetText(verovioText);
                    rend->AddChild(text);
                    tempo->AddChild(rend);
                }
                break;
            }
            case MetronomeElements::BEAT_UNIT_DOT: {
                // don't do anything here, dots are counted in the BEAT_UNIT section
                break;
            }
            case MetronomeElements::PER_MINUTE: {
                // Use the first floating-point number on the line to set @mm:
                std::string matches("0123456789");
                std::size_t offset = iter->second.find_first_of(matches);
                if (offset < iter->second.length()) {
                    const double mmval = std::stod(iter->second.substr(offset));
                    tempo->SetMm(mmval);
                }
                if (!iter->second.empty()) {
                    Text *text = new Text();
                    text->SetText(UTF8to32(iter->second));
                    tempo->AddChild(text);
                }
                break;
            }
            case MetronomeElements::SEPARATOR: {
                Text *text = new Text();
                text->SetText(UTF8to32(iter->second));
                tempo->AddChild(text);
                break;
            }
        }
    }

    if (paren) {
        Text *text = new Text();
        text->SetText(UTF8to32(")"));
        tempo->AddChild(text);
    }
}

//////////////////////////////////////////////////////////////////////////////
// Parsing methods

bool MusicXmlInput::ReadMusicXml(pugi::xml_node root)
{
    assert(root);

    // check for multimetric music
    bool multiMetric = root.select_node("/score-partwise/part/measure[@non-controlling='yes']");
    if (multiMetric) {
        LogError("MusicXML import: Multimetric music detected. Import cancelled.");
        exit(1);
    }

    ReadMusicXmlTitle(root);

    // the mdiv
    Mdiv *mdiv = new Mdiv();
    mdiv->m_visibility = Visible;
    m_doc->AddChild(mdiv);
    // the score
    Score *score = new Score();
    mdiv->AddChild(score);
    // the section
    Section *section = new Section();
    score->AddChild(section);
    // initialize layout
    if (root.select_node("/score-partwise/part/measure/print[@new-system or @new-page]")) {
        m_layoutInformation = LAYOUT_ENCODED;
        if (!root.select_node("/score-partwise/part[1]/measure[1]/print[@new-system or @new-page]")) {
            // always start with a new page
            Pb *pb = new Pb();
            section->AddChild(pb);
        }
    }

    pugi::xpath_node layout = root.select_node("/score-partwise/defaults/page-layout");
    const float bottom = layout.node().select_node("page-margins/bottom-margin").node().text().as_float();

    // generate page head
    pugi::xpath_node_set credits = root.select_nodes("/score-partwise/credit[@page='1']/credit-words");
    if (!credits.empty()) {
        PgHead *head = NULL;
        PgFoot *foot = NULL;
        for (pugi::xpath_node_set::const_iterator it = credits.begin(); it != credits.end(); ++it) {
            pugi::xpath_node words = *it;
            Rend *rend = new Rend();
            Text *text = new Text();
            text->SetText(UTF8to32(words.node().text().as_string()));
            std::string lang = words.node().attribute("xml:lang").as_string();
            rend->SetColor(words.node().attribute("color").as_string());
            rend->SetHalign(
                rend->AttHorizontalAlign::StrToHorizontalalignment(words.node().attribute("justify").as_string()));
            rend->SetValign(
                rend->AttVerticalAlign::StrToVerticalalignment(words.node().attribute("valign").as_string()));
            rend->SetFontstyle(rend->AttTypography::StrToFontstyle(words.node().attribute("font-style").as_string()));
            // rend->SetFontsize(rend->AttTypography::StrToFontsize(words.node().attribute("font-size").as_string()+std::string("pt")));
            rend->SetFontweight(
                rend->AttTypography::StrToFontweight(words.node().attribute("font-weight").as_string()));
            rend->AddChild(text);
            if (words.node().attribute("default-y").as_float() < 2 * bottom) {
                if (!foot) {
                    foot = new PgFoot();
                }
                foot->AddChild(rend);
            }
            else {
                if (!head) {
                    head = new PgHead();
                }
                head->AddChild(rend);
            }
        }
        if (head) {
            score->GetScoreDef()->AddChild(head);
        }
        if (foot) {
            score->GetScoreDef()->AddChild(foot);
        }
    }

    std::vector<StaffGrp *> m_staffGrpStack;
    StaffGrp *staffGrp = new StaffGrp();
    score->GetScoreDef()->AddChild(staffGrp);
    m_staffGrpStack.push_back(staffGrp);

    short int staffOffset = 0;
    m_octDis.push_back(0);

    pugi::xpath_node scoreMidiBpm = root.select_node("/score-partwise/part[1]/measure[1]/sound[@tempo][1]");
    if (scoreMidiBpm) score->GetScoreDef()->SetMidiBpm(scoreMidiBpm.node().attribute("tempo").as_double());

    pugi::xpath_node_set partListChildren = root.select_nodes("/score-partwise/part-list/*");
    for (pugi::xpath_node_set::const_iterator it = partListChildren.begin(); it != partListChildren.end(); ++it) {
        pugi::xpath_node xpathNode = *it;
        if (IsElement(xpathNode.node(), "part-group")) {
            if (HasAttributeWithValue(xpathNode.node(), "type", "start")) {
                StaffGrp *staffGrp = new StaffGrp();
                // read the group-symbol (MEI @symbol)
                const std::string groupGymbol = xpathNode.node().child("group-symbol").text().as_string();
                if (!groupGymbol.empty()) {
                    GrpSym *grpSym = new GrpSym();
                    if (groupGymbol == "brace") {
                        grpSym->SetSymbol(staffGroupingSym_SYMBOL_brace);
                    }
                    else if (groupGymbol == "line") {
                        grpSym->SetSymbol(staffGroupingSym_SYMBOL_line);
                    }
                    else if (groupGymbol == "bracket") {
                        grpSym->SetSymbol(staffGroupingSym_SYMBOL_bracket);
                    }
                    else if (groupGymbol == "square") {
                        grpSym->SetSymbol(staffGroupingSym_SYMBOL_bracketsq);
                    }
                    staffGrp->AddChild(grpSym);
                }
                const std::string groupBarline = xpathNode.node().child("group-barline").text().as_string();
                if (!groupBarline.empty()) staffGrp->SetBarThru((groupBarline == "no") ? BOOLEAN_false : BOOLEAN_true);
                if (groupBarline == "Mensurstrich") staffGrp->SetBarMethod(BARMETHOD_mensur);
                // now stack it
                const std::string groupName
                    = GetContentOfChild(xpathNode.node(), "group-name[not(@print-object='no')]");
                const std::string groupAbbr
                    = GetContentOfChild(xpathNode.node(), "group-abbreviation[not(@print-object='no')]");
                if (!groupName.empty() && !m_label) {
                    m_label = new Label();
                    if (xpathNode.node().select_node("group-name-display[not(@print-object='no')]")) {
                        const std::string name = StyleLabel(xpathNode.node().child("group-name-display"));
                        Text *text = new Text();
                        text->SetText(UTF8to32(name));
                        m_label->AddChild(text);
                    }
                    else {
                        Text *text = new Text();
                        text->SetText(UTF8to32(groupName));
                        m_label->AddChild(text);
                    }
                    staffGrp->AddChild(m_label);
                    m_label = NULL;
                }
                if (!groupAbbr.empty() && !m_labelAbbr) {
                    m_labelAbbr = new LabelAbbr();
                    if (xpathNode.node().select_node("group-abbreviation-display[not(@print-object='no')]")) {
                        const std::string name = StyleLabel(xpathNode.node().child("group-abbreviation-display"));
                        Text *text = new Text();
                        text->SetText(UTF8to32(name));
                        m_labelAbbr->AddChild(text);
                    }
                    else {
                        Text *text = new Text();
                        text->SetText(UTF8to32(groupAbbr));
                        m_labelAbbr->AddChild(text);
                    }
                    staffGrp->AddChild(m_labelAbbr);
                    m_labelAbbr = NULL;
                }
                m_staffGrpStack.back()->AddChild(staffGrp);
                m_staffGrpStack.push_back(staffGrp);
            }
            // this is the end of a part-group - we assume each opened part-group to be closed
            else {
                m_staffGrpStack.pop_back();
            }
        }
        else if (IsElement(xpathNode.node(), "score-part")) {
            // get the attributes element of the first measure of the part
            const std::string partId = xpathNode.node().attribute("id").as_string();
            std::string xpath = StringFormat("/score-partwise/part[@id='%s']/measure[1]", partId.c_str());
            pugi::xpath_node partFirstMeasure = root.select_node(xpath.c_str());
            if (!partFirstMeasure.node().child("attributes")) {
                LogWarning("MusicXML import: Could not find the 'attributes' element in the first "
                           "measure of part '%s'",
                    partId.c_str());
                continue;
            }
            // part-name should be revised, as soon MEI can suppress labels
            const std::string partName = GetContentOfChild(xpathNode.node(), "part-name[not(@print-object='no')]");
            const std::string partAbbr
                = GetContentOfChild(xpathNode.node(), "part-abbreviation[not(@print-object='no')]");
            pugi::xml_node midiInstrument = xpathNode.node().child("midi-instrument");
            if (!partName.empty() && !m_label) {
                m_label = new Label();
                if (xpathNode.node().select_node("part-name-display[not(@print-object='no')]")) {
                    const std::string name = StyleLabel(xpathNode.node().child("part-name-display"));
                    Text *text = new Text();
                    text->SetText(UTF8to32(name));
                    m_label->AddChild(text);
                }
                else {
                    std::stringstream sstream(partName);
                    std::string line;
                    bool firstLine = true;
                    while (std::getline(sstream, line)) {
                        if (!firstLine) {
                            m_label->AddChild(new Lb());
                        }
                        Text *text = new Text();
                        text->SetText(UTF8to32(line));
                        m_label->AddChild(text);
                        firstLine = false;
                    }
                }
            }
            if (!partAbbr.empty() && !m_labelAbbr) {
                m_labelAbbr = new LabelAbbr();
                if (xpathNode.node().select_node("part-abbreviation-display[not(@print-object='no')]")) {
                    const std::string name = StyleLabel(xpathNode.node().child("part-abbreviation-display"));
                    Text *text = new Text();
                    text->SetText(UTF8to32(name));
                    m_labelAbbr->AddChild(text);
                }
                else {
                    std::stringstream sstream(partAbbr);
                    std::string line;
                    bool firstLine = true;
                    while (std::getline(sstream, line)) {
                        if (!firstLine) {
                            m_labelAbbr->AddChild(new Lb());
                        }
                        Text *text = new Text();
                        text->SetText(UTF8to32(line));
                        m_labelAbbr->AddChild(text);
                        firstLine = false;
                    }
                }
            }
            if (midiInstrument && !m_instrdef) {
                m_instrdef = new InstrDef;
                m_instrdef->SetMidiInstrname(m_instrdef->AttMidiInstrument::StrToMidinames(
                    midiInstrument.child("midi-name").text().as_string()));
                pugi::xml_node midiChannel = midiInstrument.child("midi-channel");
                if (midiChannel) m_instrdef->SetMidiChannel(midiChannel.text().as_int() - 1);
                // pugi::xml_node midiPan = midiInstrument.child("pan");
                // if (midiPan) instrdef->SetMidiPan(midiPan.text().as_int());
                pugi::xml_node midiProgram = midiInstrument.child("midi-program");
                if (midiProgram) m_instrdef->SetMidiInstrnum(midiProgram.text().as_int() - 1);
                pugi::xml_node midiVolume = midiInstrument.child("volume");
                if (midiVolume) m_instrdef->SetMidiVolume(midiVolume.text().as_int());
            }
            // create the staffDef(s)
            StaffGrp *partStaffGrp = new StaffGrp();
            partStaffGrp->SetID(partId.c_str());
            const short int nbStaves
                = ReadMusicXmlPartAttributesAsStaffDef(partFirstMeasure.node(), partStaffGrp, staffOffset);
            // if we have more than one staff in the part we create a new staffGrp
            if (nbStaves > 1) {
                partStaffGrp->SetBarThru(BOOLEAN_true);
                if (!m_staffGrpStack.back()->GetChild(0, GRPSYM)) {
                    GrpSym *partGrpSym = new GrpSym();
                    partGrpSym->SetSymbol(staffGroupingSym_SYMBOL_brace);
                    partStaffGrp->AddChild(partGrpSym);
                }
                m_staffGrpStack.back()->AddChild(partStaffGrp);
            }
            else {
                m_staffGrpStack.back()->MoveChildrenFrom(partStaffGrp);
                delete partStaffGrp;
            }

            // find the part and read it
            xpath = StringFormat("/score-partwise/part[@id='%s']", partId.c_str());
            pugi::xpath_node part = root.select_node(xpath.c_str());
            if (!part) {
                LogWarning("MusicXML import: Could not find the part '%s'", partId.c_str());
                continue;
            }
            ReadMusicXmlPart(part.node(), section, nbStaves, staffOffset);
            // increment the staffOffset for reading the next part
            staffOffset += nbStaves;
        }
        else {
            // do nothing
        }
    }
    // here we could check that there is only one staffGrp left in m_staffGrpStack

    ProcessClefChangeQueue(section);

    Measure *measure = NULL;
    for (auto iter = m_controlElements.begin(); iter != m_controlElements.end(); ++iter) {
        if (!measure || (measure->GetN() != iter->first)) {
            AttNNumberLikeComparison comparisonMeasure(MEASURE, iter->first);
            measure = vrv_cast<Measure *>(section->FindDescendantByComparison(&comparisonMeasure, 1));
        }
        if (!measure) {
            LogWarning("MusicXML import: Element '%s' could not be added to measure %s",
                iter->second->GetClassName().c_str(), iter->first.c_str());
            delete iter->second;
            continue;
        }
        measure->AddChild(iter->second);
    }

    // manage endings stack: create new <ending> elements and move the corresponding measures into them
    if (!m_endingStack.empty()) {
        for (auto iter = m_endingStack.begin(); iter != m_endingStack.end(); ++iter) {
            std::string logString = "";
            logString = logString + "MusicXML import: Ending number='" + iter->second.m_endingNumber.c_str()
                + "', type='" + iter->second.m_endingType.c_str() + "', text='" + iter->second.m_endingText + "' (";
            std::vector<Measure *> measureList = iter->first;
            Ending *ending = new Ending();
            if (iter->second.m_endingText
                    .empty()) { // some musicXML exporters tend to ignore the <ending> text, so take @number instead.
                ending->SetN(iter->second.m_endingNumber);
            }
            else {
                ending->SetN(iter->second.m_endingText);
            }
            ending->SetLendsym(LINESTARTENDSYMBOL_angledown); // default, does not need to be written
            if (iter->second.m_endingType == "discontinue") {
                ending->SetLendsym(LINESTARTENDSYMBOL_none); // no ending symbol
            }
            // replace first <measure> with <ending> element
            section->ReplaceChild(measureList.front(), ending);
            // go through measureList of that ending and remove remaining measures from <section> and add them to
            // <ending>
            std::vector<Measure *>::iterator jter;
            for (jter = measureList.begin(); jter != measureList.end(); ++jter) {
                logString = logString + (*jter)->GetID().c_str();
                // remove other measures from <section> that are not already removed above (first measure)
                if ((*jter)->GetID() != measureList.front()->GetID()) {
                    int idx = section->GetChildIndex(*jter);
                    section->DetachChild(idx);
                }
                ending->AddChild(*jter); // add <measure> to <ending>
                logString = logString + ((*jter == measureList.back()) ? ")." : ", ");
            }
            LogDebug(logString.c_str());
        }
        m_endingStack.clear();
    }

    m_doc->ConvertToPageBasedDoc();

    // clean up stacks
    if (!m_beamspanStack.empty()) {
        LogWarning("MusicXML import: There are %d beamspans left without ending", m_beamspanStack.size());
        m_beamspanStack.clear();
    }

    if (!m_tieStack.empty()) {
        LogWarning("MusicXML import: There are %d ties left open", m_tieStack.size());
        m_tieStack.clear();
    }
    if (!m_slurStack.empty()) { // There are slurs left open
        for (auto iter = m_slurStack.begin(); iter != m_slurStack.end(); ++iter) {
            LogWarning("MusicXML import: slur %d from measure %s could not be ended", iter->second.m_number,
                iter->second.m_measureNum.c_str());
        }
        m_slurStack.clear();
    }
    if (!m_slurStopStack.empty()) { // There are slurs ends without opening
        for (auto iter = m_slurStopStack.begin(); iter != m_slurStopStack.end(); ++iter) {
            LogWarning("MusicXML import: slur ending for element '%s' could not be "
                       "matched to a start element",
                iter->first->GetID().c_str());
        }
        m_slurStopStack.clear();
    }
    if (!m_glissStack.empty()) {
        for (auto iter = m_glissStack.begin(); iter != m_glissStack.end(); ++iter) {
            LogWarning("MusicXML import: gliss for '%s' could not be closed", (*iter)->GetID().c_str());
        }
        m_glissStack.clear();
    }
    if (!m_trillStack.empty()) { // open trills without ending
        for (auto iter = m_trillStack.begin(); iter != m_trillStack.end(); ++iter) {
            LogWarning("MusicXML import: trill extender for '%s' could not be ended", iter->first->GetID().c_str());
        }
        m_trillStack.clear();
    }

    return true;
}

void MusicXmlInput::ReadMusicXmlTitle(pugi::xml_node root)
{
    assert(root);
    pugi::xpath_node workTitle = root.select_node("/score-partwise/work/work-title");
    pugi::xpath_node movementTitle = root.select_node("/score-partwise/movement-title");
    pugi::xpath_node workNumber = root.select_node("/score-partwise/work/work-number");
    pugi::xpath_node movementNumber = root.select_node("/score-partwise/movement-number");
    pugi::xml_node meiHead = m_doc->m_header.append_child("meiHead");

    // <fileDesc> /////////////
    pugi::xml_node fileDesc = meiHead.append_child("fileDesc");
    pugi::xml_node titleStmt = fileDesc.append_child("titleStmt");
    pugi::xml_node meiTitle = titleStmt.append_child("title");
    if (movementTitle)
        meiTitle.text().set(movementTitle.node().text().as_string());
    else if (workTitle)
        meiTitle.text().set(workTitle.node().text().as_string());

    if (movementNumber) {
        pugi::xml_node meiSubtitle = titleStmt.append_child("title");
        meiSubtitle.text().set(movementNumber.node().text().as_string());
        meiSubtitle.append_attribute("type").set_value("subordinate");
    }
    else if (workNumber) {
        pugi::xml_node meiSubtitle = titleStmt.append_child("title");
        meiSubtitle.text().set(workNumber.node().text().as_string());
        meiSubtitle.append_attribute("type").set_value("subordinate");
    }

    pugi::xml_node pubStmt = fileDesc.append_child("pubStmt");

    pugi::xml_node respStmt = titleStmt.append_child("respStmt");

    pugi::xpath_node_set creators = root.select_nodes("/score-partwise/identification/creator");
    for (pugi::xpath_node_set::const_iterator it = creators.begin(); it != creators.end(); ++it) {
        pugi::xpath_node creator = *it;
        pugi::xml_node persName = respStmt.append_child("persName");
        persName.text().set(creator.node().text().as_string());
        persName.append_attribute("role").set_value(creator.node().attribute("type").as_string());
    }

    pugi::xpath_node_set dateSet = root.select_nodes("/score-partwise/identification/encoding/encoding-date");
    for (pugi::xpath_node_set::const_iterator it = dateSet.begin(); it != dateSet.end(); ++it) {
        pugi::xpath_node encodingDate = *it;
        pugi::xml_node date = pubStmt.append_child("date");
        date.text().set(encodingDate.node().text().as_string());
        date.append_attribute("isodate").set_value(encodingDate.node().text().as_string());
        date.append_attribute("type").set_value(encodingDate.node().name());
    }

    // Convert rights into availability
    pugi::xpath_node_set rightsSet = root.select_nodes("/score-partwise/identification/rights");
    if (!rightsSet.empty()) {
        pugi::xml_node availability = pubStmt.append_child("availability");
        for (pugi::xpath_node_set::const_iterator it = rightsSet.begin(); it != rightsSet.end(); ++it) {
            pugi::xpath_node rights = *it;
            availability.append_child("distributor").text().set(rights.node().text().as_string());
        }
    }

    pugi::xml_node encodingDesc = meiHead.append_child("encodingDesc");
    pugi::xml_node appInfo = encodingDesc.append_child("appInfo");
    pugi::xml_node app = appInfo.append_child("application");
    pugi::xml_node appName = app.append_child("name");
    appName.text().set("Verovio");
    pugi::xml_node appText = app.append_child("p");
    appText.text().set("Transcoded from MusicXML");

    if (!m_doc->GetOptions()->m_removeIds.GetValue()) {
        GenerateID(meiHead);
        GenerateID(fileDesc);
        GenerateID(titleStmt);
        GenerateID(pubStmt);
        GenerateID(encodingDesc);
        GenerateID(appInfo);
        GenerateID(app);
        GenerateID(appName);
        GenerateID(appText);
    }

    // isodate and version
    time_t t = time(0); // get time now
    struct tm *now = localtime(&t);
    std::string dateStr = StringFormat("%d-%02d-%02dT%02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1,
        now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    app.append_attribute("isodate").set_value(dateStr.c_str());
    app.append_attribute("version").set_value(GetVersion().c_str());
}

short int MusicXmlInput::ReadMusicXmlPartAttributesAsStaffDef(
    pugi::xml_node node, StaffGrp *staffGrp, const short int staffOffset)
{
    assert(node);
    assert(staffGrp);

    // First get the number of staves in the part
    short int nbStaves = 1;
    pugi::xpath_node staves = node.select_node("attributes[1]/staves");
    if (staves) {
        nbStaves = staves.node().text().as_int();
    }
    if (nbStaves > 1) {
        if (m_label) staffGrp->AddChild(m_label);
        if (m_labelAbbr) staffGrp->AddChild(m_labelAbbr);
        if (m_instrdef) staffGrp->AddChild(m_instrdef);
        m_label = NULL;
        m_labelAbbr = NULL;
        m_instrdef = NULL;
    }

    for (pugi::xml_node::iterator it = node.begin(); it != node.end(); ++it) {

        // We read all attribute elements until we reach something else
        // barline, direction, print, and sound elements may be present
        if (!IsElement(*it, "attributes") && !IsElement(*it, "barline") && !IsElement(*it, "direction")
            && !IsElement(*it, "print") && !IsElement(*it, "sound")) {
            break;
        }

        // we do not want to read it again, just change the name
        if (IsElement(*it, "attributes")) {
            it->set_name("mei-read");
        }
        else {
            continue;
        }

        std::string xpath;
        // Create as many staffDef
        for (int i = 0; i < nbStaves; ++i) {
            // Find or create the staffDef
            AttNIntegerComparison comparisonStaffDef(STAFFDEF, i + 1 + staffOffset);
            StaffDef *staffDef = vrv_cast<StaffDef *>(staffGrp->FindDescendantByComparison(&comparisonStaffDef, 1));
            if (!staffDef) {
                staffDef = new StaffDef();
                staffDef->SetN(i + 1 + staffOffset);
                if (nbStaves == 1) {
                    staffDef->SetID(staffGrp->GetID());
                    if (m_label) staffDef->AddChild(m_label);
                    if (m_labelAbbr) staffDef->AddChild(m_labelAbbr);
                    if (m_instrdef) staffDef->AddChild(m_instrdef);
                    m_label = NULL;
                    m_labelAbbr = NULL;
                    m_instrdef = NULL;
                }
                staffGrp->AddChild(staffDef);
                // set initial octave shift
                m_octDis.push_back(0);
            }

            // clef sign - first look if we have a clef-sign with the corresponding staff @number
            std::string xpath = StringFormat("clef[@number='%d']", i + 1);
            pugi::xpath_node clef = it->select_node(xpath.c_str());
            // if not, look at a common one
            if (!clef) {
                clef = it->select_node("clef[not(@number)]");
                if (nbStaves > 1) clef.node().remove_attribute("id");
            }
            Clef *meiClef = ConvertClef(clef.node());
            if (meiClef) {
                staffDef->AddChild(meiClef);
                // if TAB assume guitar tablature until we examine <staff-details>, if any
                if (meiClef->GetShape() == CLEFSHAPE_TAB) staffDef->SetNotationtype(NOTATIONTYPE_tab_guitar);
            }

            // key sig
            xpath = StringFormat("key[@number='%d']", i + 1);
            pugi::xpath_node key = it->select_node(xpath.c_str());
            if (!key) {
                key = it->select_node("key[not(@number)]");
                if (nbStaves > 1) key.node().remove_attribute("id");
            }
            if (key) {
                KeySig *meiKey = ConvertKey(key.node());
                staffDef->AddChild(meiKey);
                if (staffDef->GetNotationtype() == NOTATIONTYPE_tab_guitar) meiKey->IsAttribute(true);
            }

            // staff details
            pugi::xpath_node staffDetails;
            xpath = StringFormat("staff-details[@number='%d']", i + 1);
            staffDetails = it->select_node(xpath.c_str());
            if (!staffDetails) {
                staffDetails = it->select_node("staff-details[not(@number)]");
            }
            short int staffLines = staffDetails.node().select_node("staff-lines").node().text().as_int();
            if (staffLines) {
                staffDef->SetLines(staffLines);
            }
            else if (!staffDef->HasLines()) {
                staffDef->SetLines(5);
            }
            std::string scaleStr = staffDetails.node().select_node("staff-size").node().text().as_string();
            if (!scaleStr.empty()) {
                staffDef->SetScale(staffDef->AttScalable::StrToPercent(scaleStr + "%"));
            }
            // Tablature?
            if (staffDetails.node().child("staff-tuning") || (staffDef->GetNotationtype() == NOTATIONTYPE_tab_guitar)) {
                // tablature type.  MusicXML does not support German tablature.
                if (HasAttributeWithValue(staffDetails.node(), "show-frets", "letters")) {
                    staffDef->SetNotationtype(NOTATIONTYPE_tab_lute_french);
                }
                else {
                    // Frets are notated with numbers.
                    // Italian tablature if the top staff line has a lower pitch than the bottom line
                    // else guitar tablature.
                    pugi::xml_node topLine = staffDetails.node().find_child_by_attribute(
                        "staff-tuning", "line", std::to_string(staffLines).c_str());
                    pugi::xml_node botLine = staffDetails.node().find_child_by_attribute("staff-tuning", "line", "1");
                    if (topLine && botLine
                        && PitchToMidi(topLine.child("tuning-step").text().as_string(),
                               topLine.child("tuning-alter").text().as_int(),
                               topLine.child("tuning-octave").text().as_int())
                            < PitchToMidi(botLine.child("tuning-step").text().as_string(),
                                botLine.child("tuning-alter").text().as_int(),
                                botLine.child("tuning-octave").text().as_int())) {
                        staffDef->SetNotationtype(NOTATIONTYPE_tab_lute_italian);
                    }
                    else {
                        staffDef->SetNotationtype(NOTATIONTYPE_tab_guitar);
                    }
                }

                // MusicXML specifies the tuning of the staff rather than the instrument,
                // but this will at least give the tuning of some of the courses.
                // For French tablature MusicXML ought to allow zero and negative values
                // of @line to give the tuning for diapasons: 0 => 7, -1 => 8 etc.  But it doesn't.
                // However, we can add the tuning of diapasons as we encounter them in <note>s.
                Tuning *tuning = new Tuning();
                staffDef->AddChild(tuning);

                for (pugi::xml_node staffTuning : staffDetails.node().children("staff-tuning")) {
                    Course *courseTuning = new Course();
                    tuning->AddChild(courseTuning);

                    const int line = staffTuning.attribute("line").as_int();
                    const std::string stepStr = staffTuning.child("tuning-step").text().as_string();
                    const int alterNum = staffTuning.child("tuning-alter").text().as_int();
                    const int octaveNum = staffTuning.child("tuning-octave").text().as_int();

                    if (staffDef->GetNotationtype() == NOTATIONTYPE_tab_lute_italian) {
                        // Italian tablature, line 1 is course 1
                        courseTuning->SetN(std::to_string(line));
                    }
                    else {
                        // guitar and French tablature, line 1 is course = staffLines (typically 6)
                        courseTuning->SetN(std::to_string(staffLines - line + 1));
                    }
                    courseTuning->SetPname(ConvertStepToPitchName(stepStr));
                    courseTuning->SetOct(octaveNum);

                    if (alterNum != 0) {
                        static_assert(
                            static_cast<int>(ACCIDENTAL_WRITTEN_f) == static_cast<int>(ACCIDENTAL_GESTURAL_f));
                        static_assert(
                            static_cast<int>(ACCIDENTAL_WRITTEN_s) == static_cast<int>(ACCIDENTAL_GESTURAL_s));
                        courseTuning->SetAccid(static_cast<data_ACCIDENTAL_WRITTEN>(ConvertAlterToAccid(alterNum)));
                    }
                }
            }

            // time
            pugi::xpath_node time;
            xpath = StringFormat("time[@number='%d']", i + 1);
            time = it->select_node(xpath.c_str());
            if (!time) {
                time = it->select_node("time[not(@number)]");
                if (nbStaves > 1) time.node().remove_attribute("id");
            }
            if (time) {
                ReadMusicXMLMeterSig(time.node(), staffDef);
            }
            // add it if necessary

            // transpose
            pugi::xpath_node transpose;
            xpath = StringFormat("transpose[@number='%d']", i + 1);
            transpose = it->select_node(xpath.c_str());
            if (!transpose) {
                transpose = it->select_node("transpose");
            }
            if (transpose) {
                staffDef->SetTransDiat(transpose.node().child("diatonic").text().as_int());
                staffDef->SetTransSemi(transpose.node().child("chromatic").text().as_int());
                if (transpose.node().child("octave-change")) {
                    staffDef->SetTransDiat(transpose.node().child("chromatic").text().as_int()
                        + 7 * transpose.node().child("octave-change").text().as_int());
                    staffDef->SetTransSemi(transpose.node().child("chromatic").text().as_int()
                        + 12 * transpose.node().child("octave-change").text().as_int());
                }
            }
            // ppq
            pugi::xpath_node divisions = it->select_node("divisions");
            if (divisions) {
                m_ppq = divisions.node().text().as_int();
                staffDef->SetPpq(m_ppq);
            }
            // measure style
            pugi::xpath_node measureSlash = it->select_node("measure-style/slash");
            if (measureSlash) {
                m_slash = (HasAttributeWithValue(measureSlash.node(), "type", "start")) ? true : false;
            }
        }
    }

    return nbStaves;
}

void MusicXmlInput::ReadMusicXMLMeterSig(const pugi::xml_node &time, Object *parent)
{
    if ((time.select_nodes("beats").size() > 1) || time.select_node("interchangeable")) {
        MeterSigGrp *meterSigGrp = new MeterSigGrp();
        if (time.attribute("id")) {
            meterSigGrp->SetID(time.attribute("id").as_string());
        }
        pugi::xpath_node interchangeable = time.select_node("interchangeable");
        meterSigGrp->SetFunc(interchangeable ? meterSigGrpLog_FUNC_interchanging : meterSigGrpLog_FUNC_mixed);

        std::tie(m_meterCount, m_meterUnit) = this->GetMeterSigGrpValues(time, meterSigGrp);
        if (interchangeable) {
            std::tie(std::ignore, std::ignore) = this->GetMeterSigGrpValues(interchangeable.node(), meterSigGrp);
        }
        parent->AddChild(meterSigGrp);
    }
    else {
        MeterSig *meterSig = new MeterSig();
        if (time.attribute("id")) {
            meterSig->SetID(time.attribute("id").as_string());
        }
        const std::string symbol = time.attribute("symbol").as_string();
        if (!symbol.empty()) {
            if (symbol == "cut" || symbol == "common") {
                meterSig->SetSym(meterSig->AttMeterSigVis::StrToMetersign(symbol.c_str()));
            }
            else if (symbol == "single-number") {
                meterSig->SetForm(METERFORM_num);
            }
            else {
                meterSig->SetForm(METERFORM_norm);
            }
        }
        pugi::xml_node beats = time.child("beats");
        pugi::xml_node beatType = time.child("beat-type");
        if (beats) {
            std::tie(m_meterCount, m_meterSign)
                = meterSig->AttMeterSigLog::StrToMetercountPair(beats.text().as_string());
            meterSig->SetCount({ m_meterCount, m_meterSign });
            m_meterUnit = beatType.text().as_int();
            meterSig->SetUnit(m_meterUnit);
        }
        else if (time.child("senza-misura")) {
            if (time.child("senza-misura").text()) {
                meterSig->SetSym(METERSIGN_open);
            }
            else {
                meterSig->SetVisible(BOOLEAN_false);
            }
        }
        parent->AddChild(meterSig);
    }
}

bool MusicXmlInput::ReadMusicXmlPart(pugi::xml_node node, Section *section, short int nbStaves, short int staffOffset)
{
    assert(node);
    assert(section);

    pugi::xpath_node_set measures = node.select_nodes("measure");
    if (measures.size() == 0) {
        LogWarning("MusicXML import: No measure to load");
        return false;
    }

    int i = 0;
    for (pugi::xpath_node_set::const_iterator it = measures.begin(); it != measures.end(); ++it) {
        pugi::xpath_node xmlMeasure = *it;
        if (!IsMultirestMeasure(i)) {
            Measure *measure = new Measure();
            m_measureCounts[measure] = i;
            ReadMusicXmlMeasure(xmlMeasure.node(), section, measure, nbStaves, staffOffset, i);
            // Add the measure to the system - if already there from a previous part we'll just merge the content
            AddMeasure(section, measure, i);
        }
        else {
            // Handle barline parsing for the multirests (where barline would be defined in last measure of the mRest)
            // If this is last measure, find starting measure of the mRest (based on the Id) and add barline to it
            auto lastElementIter = std::find_if(m_multiRests.begin(), m_multiRests.end(),
                [i](const std::pair<int, int> &elem) { return i == elem.second; });
            if (lastElementIter != m_multiRests.end()) {
                auto measureIter = std::find_if(m_measureCounts.begin(), m_measureCounts.end(),
                    [lastElementIter](
                        const std::pair<Measure *, int> &elem) { return lastElementIter->first == elem.second; });
                if (measureIter != m_measureCounts.end()) {
                    for (auto it = xmlMeasure.node().begin(); it != xmlMeasure.node().end(); ++it) {
                        if (IsElement(*it, "barline")) {
                            ReadMusicXmlBarLine(*it, measureIter->first, std::to_string(lastElementIter->first));
                        }
                    }
                }
            }
        }
        ++i;
    }

    // clean up part specific stacks
    if (!m_openDashesStack.empty()) { // open dashes without ending
        for (auto iter = m_openDashesStack.begin(); iter != m_openDashesStack.end(); ++iter) {
            LogWarning(
                "MusicXML import: dashes/extender lines for '%s' could not be closed", iter->first->GetID().c_str());
        }
        m_openDashesStack.clear();
    }
    if (!m_bracketStack.empty()) { // open brackets without ending
        for (auto iter = m_bracketStack.begin(); iter != m_bracketStack.end(); ++iter) {
            LogWarning("MusicXML import: bracketSpan for '%s' could not be closed", iter->first->GetID().c_str());
        }
        m_bracketStack.clear();
    }
    if (!m_hairpinStack.empty()) {
        LogWarning("MusicXML import: There are %d hairpins left open", m_hairpinStack.size());
        m_hairpinStack.clear();
    }

    return false;
}

bool MusicXmlInput::ReadMusicXmlMeasure(
    pugi::xml_node node, Section *section, Measure *measure, short int nbStaves, const short int staffOffset, int index)
{
    assert(node);
    assert(measure);

    const std::string measureNum = node.attribute("number").as_string();
    if (node.attribute("id")) measure->SetID(node.attribute("id").as_string());
    if (measure != NULL) measure->SetN(measureNum);

    bool implicit = node.attribute("implicit").as_bool();
    if (implicit) {
        MNum *mNum = new MNum();
        // An empty mNum means that we like to render this measure number as blank.
        measure->AddChild(mNum);
    }

    for (int i = 0; i < nbStaves; ++i) {
        // the staff @n must take into account the staffOffset
        Staff *staff = new Staff();
        staff->SetN(i + 1 + staffOffset);
        staff->SetVisible(
            ConvertWordToBool(node.child("attributes").child("staff-details").attribute("print-object").value()));
        measure->AddChild(staff);
        // layers will be added in SelectLayer
    }

    // Normally the stack should be empty
    // LogDebug("Measure %s", node.attribute("number").as_string());
    // assert(m_elementStack.empty());
    m_elementStackMap.erase(m_elementStackMap.begin(), m_elementStackMap.end());

    // reset measure time
    m_durTotal = 0;

    const auto mrestPositonIter = m_multiRests.find(index);
    bool isMRestInOtherSystem = (mrestPositonIter != m_multiRests.end());
    int multiRestStaffNumber = 1;

    // read the content of the measure
    for (pugi::xml_node::iterator it = node.begin(); it != node.end(); ++it) {
        // first check if there is a multi measure rest
        if (it->select_node(".//multiple-rest")) {
            const pugi::xml_node multiRestNode = it->select_node(".//multiple-rest").node();
            const int multiRestLength = multiRestNode.text().as_int();
            const std::string symbols = multiRestNode.attribute("use-symbols").as_string();
            MultiRest *multiRest = new MultiRest;
            if (symbols == "no") {
                // default by MusicXML specification
                multiRest->SetBlock(BOOLEAN_true);
            }
            else if (symbols == "yes") {
                multiRest->SetBlock(BOOLEAN_false);
            }
            multiRest->SetNum(multiRestLength);
            Layer *layer = SelectLayer(1, measure);
            AddLayerElement(layer, multiRest);
            m_multiRests[index] = index + multiRestLength - 1;
            break;
        }
        else if (isMRestInOtherSystem) {
            if ((multiRestStaffNumber > 1) && !IsElement(*it, "backup")) continue;
            MultiRest *multiRest = new MultiRest;
            multiRest->SetNum(mrestPositonIter->second - mrestPositonIter->first + 1);
            Layer *layer = SelectLayer(multiRestStaffNumber, measure);
            AddLayerElement(layer, multiRest);
            if (multiRestStaffNumber < nbStaves) multiRestStaffNumber++;
            continue;
        }
        if (IsElement(*it, "attributes")) {
            ReadMusicXmlAttributes(*it, section, measure, measureNum);
        }
        else if (IsElement(*it, "backup")) {
            ReadMusicXmlBackup(*it, measure, measureNum);
        }
        else if (IsElement(*it, "barline")) {
            ReadMusicXmlBarLine(*it, measure, measureNum);
        }
        else if (IsElement(*it, "direction")) {
            ReadMusicXmlDirection(*it, measure, measureNum, staffOffset);
        }
        else if (IsElement(*it, "figured-bass")) {
            ReadMusicXmlFigures(*it, measure, measureNum);
        }
        else if (IsElement(*it, "forward")) {
            ReadMusicXmlForward(*it, measure, measureNum);
        }
        else if (IsElement(*it, "harmony")) {
            ReadMusicXmlHarmony(*it, measure, measureNum);
        }
        else if (IsElement(*it, "note")) {
            ReadMusicXmlNote(*it, measure, measureNum, staffOffset, section);
        }
        // for now only check first part
        else if (IsElement(*it, "print") && node.select_node("parent::part[not(preceding-sibling::part)]")) {
            ReadMusicXmlPrint(*it, section);
        }
    }

    this->MatchTies(true);
    if (!m_tieStack.empty()) this->MatchTies(false);
    for (auto openTie : m_tieStack) {
        openTie.m_note->SetScoreTimeOnset(-1); // make scoreTimeOnset small for next measure
    }

    // clear stop stacks after each measure
    m_hairpinStopStack.clear();
    m_tieStopStack.clear();

    for (auto staff : measure->GetChildren()) {
        if (!staff->Is(STAFF)) {
            continue;
        }
        assert(staff);
        if (staff->GetChildCount() == 0) { // add a default layer, if staff completely empty at the end of a measure.
            Layer *emptyLayer = new Layer();
            emptyLayer->AddChild(new MSpace());
            staff->AddChild(emptyLayer);
        }
    }

    // clear arpeggio stack so no other notes may be added.
    if (!m_ArpeggioStack.empty()) m_ArpeggioStack.clear();

    // clear prevLayer
    m_prevLayer = NULL;

    // clear current layer
    m_isLayerInitialized = false;
    m_currentLayer = NULL;

    return true;
}

void MusicXmlInput::MatchTies(bool matchLayers)
{
    // match open ties with close ties
    std::vector<musicxml::OpenTie>::iterator iter = m_tieStack.begin();
    while (iter != m_tieStack.end()) {
        double lastScoreTimeOnset = 9999; // __DBL_MAX__;
        bool tieMatched = false;
        std::vector<musicxml::CloseTie>::iterator jter;
        for (jter = m_tieStopStack.begin(); jter != m_tieStopStack.end(); ++jter) {
            // match tie stop with pitch/oct identity, with start note earlier than end note,
            // and with earliest end note.
            if ((iter->m_note->IsEnharmonicWith(jter->m_note))
                && (iter->m_note->GetRealTimeOnsetMilliseconds() < jter->m_note->GetRealTimeOnsetMilliseconds())
                && (jter->m_note->GetRealTimeOnsetMilliseconds() < lastScoreTimeOnset)
                && (!matchLayers || (iter->m_layerNum == jter->m_layerNum))) {
                iter->m_tie->SetEndid("#" + jter->m_note->GetID());
                lastScoreTimeOnset = jter->m_note->GetRealTimeOnsetMilliseconds();
                tieMatched = true;
                break;
            }
        }
        if (tieMatched) {
            iter = m_tieStack.erase(iter);
            m_tieStopStack.erase(jter);
        }
        else {
            ++iter;
        }
    }
}

void MusicXmlInput::ReadMusicXmlAttributes(
    pugi::xml_node node, Section *section, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(section);
    assert(measure);
    bool divisionChange = false;

    // check for changes in divisions
    pugi::xml_node divisions = node.child("divisions");
    if (divisions) {
        // we'll only convert this to MEI if it actually changes
        divisionChange = (m_ppq != divisions.text().as_int());
        m_ppq = divisions.text().as_int();
    }

    // read clef changes as MEI clef and add them to the stack
    pugi::xml_node clef = node.child("clef");
    if (clef) {
        // check if we have a staff number
        short int staffNum = clef.attribute("number").as_int();
        staffNum = (staffNum < 1) ? 1 : staffNum;
        Staff *staff = vrv_cast<Staff *>(measure->GetChild(staffNum - 1, STAFF));
        assert(staff);
        Clef *meiClef = ConvertClef(clef);
        if (meiClef) {
            const bool afterBarline = clef.attribute("after-barline").as_bool();
            m_clefChangeQueue.push(
                musicxml::ClefChange(measureNum, staff, m_currentLayer, meiClef, m_durTotal, afterBarline));
        }
    }

    // key and time change
    pugi::xml_node key = node.child("key");
    pugi::xml_node time = node.child("time");

    // for now only read first key change in first part and update scoreDef
    if ((key || time || divisionChange) && node.select_node("ancestor::part[not(preceding-sibling::part)]")
        && !node.select_node("preceding-sibling::attributes/key")) {
        ScoreDef *scoreDef = new ScoreDef();
        if (key) {
            KeySig *meiKey = ConvertKey(key);
            scoreDef->AddChild(meiKey);
        }

        if (time) {
            ReadMusicXMLMeterSig(time, scoreDef);
        }

        if (divisions) {
            scoreDef->SetPpq(divisions.text().as_int());
        }

        section->AddChild(scoreDef);
    }
    else if (time && node.select_node("ancestor::part[(preceding-sibling::part)]")) {
        m_meterUnit = time.child("beat-type").text().as_int();
    }

    pugi::xpath_node measureRepeat = node.select_node("measure-style/measure-repeat");
    pugi::xpath_node measureSlash = node.select_node("measure-style/slash");
    if (measureRepeat) {
        m_mRpt = (HasAttributeWithValue(measureRepeat.node(), "type", "start")) ? true : false;
    }
    if (measureSlash) {
        m_slash = (HasAttributeWithValue(measureSlash.node(), "type", "start")) ? true : false;
    }
}

void MusicXmlInput::ReadMusicXmlBackup(pugi::xml_node node, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(measure);

    m_durTotal -= node.child("duration").text().as_int();

    m_isLayerInitialized = false;
}

void MusicXmlInput::ReadMusicXmlBarLine(pugi::xml_node node, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(measure);

    Staff *staff = vrv_cast<Staff *>(measure->GetFirst(STAFF));
    assert(staff);

    const std::string barStyle = node.child("bar-style").text().as_string();
    pugi::xpath_node repeat = node.select_node("repeat");
    if (!barStyle.empty()) {
        data_BARRENDITION barRendition = ConvertStyleToRend(barStyle, repeat);
        if (HasAttributeWithValue(node, "location", "left")) {
            measure->SetLeft(barRendition);
        }
        else if (HasAttributeWithValue(node, "location", "middle")) {
            BarLine *barLine = new BarLine();
            barLine->SetColor(node.child("bar-style").attribute("color").as_string());
            barLine->SetForm(barRendition);
            Layer *layer = SelectLayer(node, measure);
            AddLayerElement(layer, barLine);
        }
        else {
            measure->SetRight(barRendition);
            if (barStyle == "short" || barStyle == "tick") {
                measure->SetBarLen(4);
                if (barStyle == "short") {
                    measure->SetBarPlace(2);
                }
                else {
                    // bar.place counts in note order (high values are vertically higher).
                    measure->SetBarPlace(6);
                }
            }
        }
    }
    if (barStyle.empty() && repeat) { // add repeat information, also when bar-style is not provided
        if (HasAttributeWithValue(node, "location", "left")) {
            measure->SetLeft(BARRENDITION_rptstart);
        }
        else if (HasAttributeWithValue(node, "location", "middle")) {
            LogWarning("MusicXML import: Unsupported barline location 'middle' in %s", measure->GetN().c_str());
        }
        else {
            measure->SetRight(BARRENDITION_rptend);
        }
    }

    // parse endings (prima volta, seconda volta...)
    pugi::xml_node ending = node.child("ending");
    if (ending) {
        std::string endingNumber = ending.attribute("number").as_string();
        std::string endingType = ending.attribute("type").as_string();
        std::string endingText = ending.text().as_string();
        // LogInfo("ending number/type/text: %s/%s/%s.", endingNumber.c_str(), endingType.c_str(),
        // endingText.c_str());
        if (endingType == "start") {
            // check for corresponding stop points
            std::string xpath = StringFormat("following::ending[@number='%s'][@type != 'start']", endingNumber.c_str());
            pugi::xpath_node endingEnd = node.select_node(xpath.c_str());
            if (endingEnd) {
                m_currentEndingStart = musicxml::EndingInfo(endingNumber, endingType, endingText);
            }
        }
        else if (endingType == "stop" || endingType == "discontinue") {
            if (m_endingStack.empty()) {
                LogWarning("MusicXML import: Dangling ending tag skipped");
            }
            else {
                m_currentEndingStop = musicxml::EndingInfo(endingNumber, endingType, endingText);
            }
        }
    }

    // fermatas
    short int fermataCounter = 0;
    for (pugi::xml_node xmlFermata : node.children("fermata")) {
        ++fermataCounter;
        Fermata *fermata = new Fermata();
        m_controlElements.push_back({ measureNum, fermata });
        if (HasAttributeWithValue(node, "location", "left")) {
            fermata->SetTstamp(0);
        }
        else if (HasAttributeWithValue(node, "location", "middle")) {
            LogWarning("MusicXML import: Unsupported barline location 'middle'");
        }
        else {
            fermata->SetTstamp((double)(m_durTotal) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
        }
        if (xmlFermata.attribute("id")) fermata->SetID(xmlFermata.attribute("id").as_string());

        if (fermataCounter < 2) {
            fermata->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
        else {
            Staff *lastStaff = vrv_cast<Staff *>(measure->GetLast());
            assert(lastStaff);
            fermata->SetStaff(lastStaff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(lastStaff->GetN())));
        }

        ShapeFermata(fermata, xmlFermata);
    }
}

void MusicXmlInput::ReadMusicXmlDirection(
    pugi::xml_node node, Measure *measure, const std::string &measureNum, const short int staffOffset)
{
    assert(node);
    assert(measure);

    const std::string placeStr = node.attribute("placement").as_string();
    const std::string directionId = node.attribute("id").as_string();

    const pugi::xml_node typeNode = node.child("direction-type");
    const pugi::xpath_node voice = node.select_node("voice");
    const short int offset = node.child("offset").text().as_int();
    const pugi::xml_node staffNode = node.child("staff");
    const pugi::xml_node soundNode = node.child("sound");

    const double timeStamp = (double)(m_durTotal + offset) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0;

    if (voice) m_prevLayer = SelectLayer(node, measure);

    // Bracket
    pugi::xml_node bracket = typeNode.child("bracket");
    if (bracket) {
        short int voiceNumber = bracket.attribute("number").as_int();
        voiceNumber = (voiceNumber < 1) ? 1 : voiceNumber;
        if (HasAttributeWithValue(bracket, "type", "stop")) {
            if (m_bracketStack.empty()) {
                // if this is empty, most likely we're dealing with an extender
            }
            else {
                const int measureDifference
                    = m_measureCounts.at(measure) - m_bracketStack.front().second.m_lastMeasureCount;
                m_bracketStack.front().first->SetLendsym(
                    ConvertLineEndSymbol(bracket.attribute("line-end").as_string()));
                if (measureDifference >= 0) {
                    m_bracketStack.front().first->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
                }
                m_bracketStack.erase(m_bracketStack.begin());
            }
        }
        else {
            BracketSpan *bracketSpan = new BracketSpan();
            musicxml::OpenSpanner openBracket(voiceNumber, m_measureCounts.at(measure));
            bracketSpan->SetColor(bracket.attribute("color").as_string());
            bracketSpan->SetLform(
                bracketSpan->AttLineRendBase::StrToLineform(bracket.attribute("line-type").as_string()));
            // bracketSpan->SetPlace(bracketSpan->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
            bracketSpan->SetFunc("unclear");
            bracketSpan->SetLstartsym(ConvertLineEndSymbol(bracket.attribute("line-end").as_string()));
            bracketSpan->SetTstamp(timeStamp);
            m_controlElements.push_back({ measureNum, bracketSpan });
            m_bracketStack.push_back({ bracketSpan, openBracket });
        }
    }

    // Coda
    pugi::xml_node xmlCoda = typeNode.child("coda");
    if (xmlCoda) {
        Dir *dir = new Dir();
        dir->SetPlace(dir->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
        dir->SetTstamp(timeStamp - 1.0);
        dir->SetType("coda");
        dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList("1"));
        if (xmlCoda.attribute("id")) dir->SetID(xmlCoda.attribute("id").as_string());
        Rend *rend = new Rend;
        rend->SetGlyphAuth("smufl");
        rend->SetFontstyle(FONTSTYLE_normal);
        rend->SetHalign(HORIZONTALALIGNMENT_center);
        Text *text = new Text();
        std::u32string codaSign = UTF8to32("\xF0\x9D\x84\x8C");
        text->SetText(codaSign);
        rend->AddChild(text);
        dir->AddChild(rend);
        m_controlElements.push_back({ measureNum, dir });
    }

    // Dashes (to be connected with previous <dir> or <dynam> as @extender and @tstamp2 attribute
    pugi::xpath_node dashes = typeNode.select_node("bracket|dashes");
    if (dashes) {
        short int dashesNumber = dashes.node().attribute("number").as_int();
        dashesNumber = (dashesNumber < 1) ? 1 : dashesNumber;
        short int staffNum = 1;
        if (staffNode) staffNum = staffNode.text().as_int() + staffOffset;
        if (HasAttributeWithValue(dashes.node(), "type", "stop")) {
            std::vector<std::pair<ControlElement *, musicxml::OpenDashes>>::iterator iter = m_openDashesStack.begin();
            while (iter != m_openDashesStack.end()) {
                if (iter->second.m_dirN == dashesNumber && iter->second.m_staffNum == staffNum) {
                    const int measureDifference = m_measureCounts.at(measure) - iter->second.m_measureCount;
                    if (measureDifference >= 0) {
                        if (iter->first->Is(DYNAM))
                            dynamic_cast<Dynam *>(iter->first)
                                ->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
                        if (iter->first->Is(DIR))
                            dynamic_cast<Dir *>(iter->first)
                                ->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
                    }
                    iter = m_openDashesStack.erase(iter);
                }
                else {
                    ++iter;
                }
            }
        }
        else if (std::strncmp(dashes.node().name(), "dashes", 6) == 0) {
            ControlElement *controlElement = nullptr;
            // find last ControlElement of type dynam or dir and activate extender
            // this is bad MusicXML and shouldn't happen
            std::vector<std::pair<std::string, ControlElement *>>::reverse_iterator riter;
            for (riter = m_controlElements.rbegin(); riter != m_controlElements.rend(); ++riter) {
                if (riter->second->Is(DYNAM)) {
                    Dynam *dynam = dynamic_cast<Dynam *>(riter->second);
                    std::vector<int> staffAttr = dynam->GetStaff();
                    if (std::find(staffAttr.begin(), staffAttr.end(), staffNum + staffOffset) != staffAttr.end()
                        && dynam->GetPlace() == dynam->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str())
                        && riter->first == measureNum) {
                        dynam->SetExtender(BOOLEAN_true);
                        controlElement = dynam;
                        break;
                    }
                }
                else if (riter->second->Is(DIR)) {
                    Dir *dir = dynamic_cast<Dir *>(riter->second);
                    std::vector<int> staffAttr = dir->GetStaff();
                    if (std::find(staffAttr.begin(), staffAttr.end(), staffNum + staffOffset) != staffAttr.end()
                        && dir->GetPlace() == dir->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str())
                        && riter->first == measureNum) {
                        dir->SetExtender(BOOLEAN_true);
                        controlElement = dir;
                        break;
                    }
                }
            }
            if (controlElement != nullptr) {
                musicxml::OpenDashes openDashes(dashesNumber, staffNum, m_measureCounts.at(measure));
                m_openDashesStack.push_back({ controlElement, openDashes });
            }
            else {
                LogInfo("MusicXmlImport: dashes could not be matched to <dir> or <dynam> in measure %s.",
                    measureNum.c_str());
            }
        }
    }

    pugi::xpath_node_set words = node.select_nodes("direction-type/words");
    const bool containsWords = !words.empty();
    bool containsDynamics
        = !node.select_node("direction-type/dynamics").node().empty() || soundNode.attribute("dynamics");
    bool containsTempo = !node.select_node("direction-type/metronome").node().empty() || soundNode.attribute("tempo");

    // Directive
    int defaultY = 0; // y position attribute, only for directives and dynamics
    if (containsWords && !containsTempo && !containsDynamics) {
        pugi::xpath_node_set words = node.select_nodes("direction-type/*[self::words or self::coda or self::segno]");
        defaultY = words.first().node().attribute("default-y").as_int();
        defaultY = (defaultY * 10) + words.first().node().attribute("relative-y").as_int();
        std::string wordStr = words.first().node().text().as_string();
        if (wordStr.rfind("cresc", 0) == 0 || wordStr.rfind("dim", 0) == 0 || wordStr.rfind("decresc", 0) == 0) {
            containsDynamics = true;
        }
        else {
            Dir *dir = new Dir();
            if (words.size() == 1) {
                dir->SetLang(words.first().node().attribute("xml:lang").as_string());
            }
            dir->SetPlace(dir->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
            dir->SetTstamp(timeStamp);
            dir->SetType(soundNode.first_attribute().name());
            if (staffNode) {
                dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(staffNode.text().as_int() + staffOffset)));
            }
            else if (m_prevLayer) {
                dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(dynamic_cast<Staff *>(m_prevLayer->GetParent())->GetN())));
            }
            else {
                dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(1 + staffOffset)));
            }

            TextRendition(words, dir);
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 2000;
            dir->SetVgrp(defaultY);
            m_controlElements.push_back({ measureNum, dir });
            m_dirStack.push_back(dir);

            pugi::xpath_node extender = (words.end() - 1)->parent().next_sibling("direction-type").first_child();
            if (!strcmp(extender.node().name(), "bracket") || !strcmp(extender.node().name(), "dashes")) {
                short int extNumber = extender.node().attribute("number").as_int();
                extNumber = (extNumber < 1) ? 1 : extNumber;
                short int staffNum = staffNode.text().as_int() + staffOffset;
                staffNum = (staffNum < 1) ? 1 : staffNum;
                dir->SetExtender(BOOLEAN_true);
                if (std::strncmp(extender.node().name(), "bracket", 7) == 0) {
                    dir->SetLform(
                        dir->AttLineRendBase::StrToLineform(extender.node().attribute("line-type").as_string()));
                }
                else {
                    dir->SetLform(LINEFORM_dashed);
                }
                musicxml::OpenDashes openDashes(extNumber, staffNum, m_measureCounts.at(measure));
                m_openDashesStack.push_back({ dir, openDashes });
            }
        }
    }

    // Dynamics
    if (containsDynamics) {
        pugi::xpath_node_set dynamics = node.select_nodes(
            containsWords ? "direction-type/dynamics|direction-type/words" : "direction-type/dynamics");

        dynamics.sort();

        Dynam *dynam = new Dynam();
        dynam->SetPlace(dynam->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
        dynam->SetTstamp(timeStamp);
        if (staffNode) {
            dynam->SetStaff(dynam->AttStaffIdent::StrToXsdPositiveIntegerList(
                std::to_string(staffNode.text().as_int() + staffOffset)));
        }
        else if (m_prevLayer) {
            dynam->SetStaff(dynam->AttStaffIdent::StrToXsdPositiveIntegerList(
                std::to_string(dynamic_cast<Staff *>(m_prevLayer->GetParent())->GetN())));
        }
        else {
            dynam->SetStaff(dynam->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(1 + staffOffset)));
        }

        if (soundNode) {
            const float dynamics = soundNode.attribute("dynamics").as_float(-1.0);
            if (dynamics >= 0.0) {
                dynam->SetVal(ConvertDynamicsToMidiVal(dynamics));
            }
        }

        TextRendition(dynamics, dynam);
        if (defaultY == 0) {
            defaultY = dynamics.first().node().attribute("default-y").as_int();
            defaultY = (defaultY * 10) + dynamics.first().node().attribute("relative-y").as_int();
        }
        // parse the default_y attribute and transform to vgrp value, to vertically align dynamics and directives
        defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 2000;
        dynam->SetVgrp(defaultY);
        m_controlElements.push_back({ measureNum, dynam });
        m_dynamStack.push_back(dynam);

        if (!dynamics.empty()) {
            pugi::xpath_node extender = (dynamics.end() - 1)->parent().next_sibling("direction-type").first_child();
            if (!strcmp(extender.node().name(), "bracket") || !strcmp(extender.node().name(), "dashes")) {
                short int extNumber = extender.node().attribute("number").as_int();
                extNumber = (extNumber < 1) ? 1 : extNumber;
                short int staffNum = staffNode.text().as_int() + staffOffset;
                staffNum = (staffNum < 1) ? 1 : staffNum;
                dynam->SetExtender(BOOLEAN_true);
                if (std::strncmp(extender.node().name(), "bracket", 7) == 0) {
                    dynam->SetLform(
                        dynam->AttLineRendBase::StrToLineform(extender.node().attribute("line-type").as_string()));
                }
                else {
                    dynam->SetLform(LINEFORM_dashed);
                }
                musicxml::OpenDashes openDashes(extNumber, staffNum, m_measureCounts.at(measure));
                m_openDashesStack.push_back({ dynam, openDashes });
            }
        }
    }

    // Hairpins
    pugi::xpath_node_set wedges = node.select_nodes("direction-type/wedge");
    for (pugi::xpath_node_set::const_iterator wedge = wedges.begin(); wedge != wedges.end(); ++wedge) {
        short int hairpinNumber = wedge->node().attribute("number").as_int();
        hairpinNumber = (hairpinNumber < 1) ? 1 : hairpinNumber;
        bool matchedWedge = false;
        if (HasAttributeWithValue(wedge->node(), "type", "stop")) {
            // match wedge type=stop to open hairpin
            std::vector<std::pair<Hairpin *, musicxml::OpenSpanner>>::iterator iter;
            for (iter = m_hairpinStack.begin(); iter != m_hairpinStack.end(); ++iter) {
                if (iter->second.m_dirN == hairpinNumber) {
                    const int measureDifference = m_measureCounts.at(measure) - iter->second.m_lastMeasureCount;
                    if (measureDifference >= 0) {
                        iter->first->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
                    }
                    if (wedge->node().attribute("niente")) {
                        iter->first->SetNiente(ConvertWordToBool(wedge->node().attribute("niente").as_string()));
                    }
                    if (iter->first->GetForm() == hairpinLog_FORM_cres) {
                        if (wedge->node().attribute("spread")) {
                            data_MEASUREMENTSIGNED opening;
                            opening.SetVu(wedge->node().attribute("spread").as_double() / 5);
                            iter->first->SetOpening(opening);
                        }
                    }
                    matchedWedge = true;
                    m_hairpinStack.erase(iter);
                    break;
                }
            }
            if (!matchedWedge) {
                m_hairpinStopStack.push_back(std::tuple<int, double, musicxml::OpenSpanner>(
                    0, timeStamp, musicxml::OpenSpanner(hairpinNumber, m_measureCounts.at(measure))));
            }
        }
        else {
            Hairpin *hairpin = new Hairpin();
            musicxml::OpenSpanner openHairpin(hairpinNumber, m_measureCounts.at(measure));
            if (HasAttributeWithValue(wedge->node(), "type", "crescendo")) {
                hairpin->SetForm(hairpinLog_FORM_cres);
            }
            else if (HasAttributeWithValue(wedge->node(), "type", "diminuendo")) {
                hairpin->SetForm(hairpinLog_FORM_dim);
                if (wedge->node().attribute("spread")) {
                    data_MEASUREMENTSIGNED opening;
                    opening.SetVu(wedge->node().attribute("spread").as_double() / 5);
                    hairpin->SetOpening(opening);
                }
            }
            else {
                delete hairpin;
                return;
            }
            hairpin->SetLform(
                hairpin->AttLineRendBase::StrToLineform(wedge->node().attribute("line-type").as_string()));
            if (wedge->node().attribute("niente")) {
                hairpin->SetNiente(ConvertWordToBool(wedge->node().attribute("niente").as_string()));
            }
            hairpin->SetColor(wedge->node().attribute("color").as_string());
            hairpin->SetPlace(hairpin->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
            hairpin->SetTstamp(timeStamp);
            if (wedge->node().attribute("id")) hairpin->SetID(wedge->node().attribute("id").as_string());
            if (staffNode) {
                hairpin->SetStaff(hairpin->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(staffNode.text().as_int() + staffOffset)));
            }
            else if (m_prevLayer) {
                hairpin->SetStaff(hairpin->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(dynamic_cast<Staff *>(m_prevLayer->GetParent())->GetN())));
            }
            else {
                hairpin->SetStaff(hairpin->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(1 + staffOffset)));
            }
            int defaultY = wedge->node().attribute("default-y").as_int();
            defaultY = (defaultY * 10) + wedge->node().attribute("relative-y").as_int();
            // parse the default_y attribute and transform to vgrp value, to vertically align hairpins
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 2000;
            hairpin->SetVgrp(defaultY);
            // match new hairpin to existing hairpin stop
            for (auto iter = m_hairpinStopStack.begin(); iter != m_hairpinStopStack.end(); ++iter) {
                const int measureDifference = std::get<2>(*iter).m_lastMeasureCount - m_measureCounts.at(measure);
                if ((std::get<2>(*iter).m_dirN == hairpinNumber) && (measureDifference == 0)) {
                    if (measureDifference >= 0) {
                        hairpin->SetTstamp2(std::pair<int, double>(measureDifference, std::get<1>(*iter)));
                        m_controlElements.push_back({ measureNum, hairpin });
                    }
                    matchedWedge = true;
                    m_hairpinStopStack.erase(iter);
                    break;
                }
            }
            if (!matchedWedge) {
                m_controlElements.push_back({ measureNum, hairpin });
                m_hairpinStack.push_back({ hairpin, openHairpin });
            }
        }
    }

    // Ottava
    pugi::xml_node xmlShift = typeNode.child("octave-shift");
    if (xmlShift) {
        const short int staffNum = (!staffNode) ? 1 : staffNode.text().as_int() + staffOffset;
        if (HasAttributeWithValue(xmlShift, "type", "stop")) {
            m_octDis[staffNum] = 0;
            for (auto iter = m_controlElements.begin(); iter != m_controlElements.end(); ++iter) {
                if (iter->second->Is(OCTAVE)) {
                    Octave *octave = dynamic_cast<Octave *>(iter->second);
                    if (octave->HasEndid()) continue;
                    std::vector<int> staffAttr = octave->GetStaff();
                    if (std::find(staffAttr.begin(), staffAttr.end(), staffNum) != staffAttr.end()) {
                        octave->SetEndid(m_ID);
                    }
                    else if (xmlShift.attribute("number").as_string() == octave->GetN()) {
                        octave->SetEndid(m_ID);
                    }
                    else {
                        LogWarning("MusicXML import: octave for '%s' could not be closed", octave->GetID().c_str());
                    }
                }
            }
        }
        else {
            Octave *octave = new Octave();
            octave->SetColor(xmlShift.attribute("color").as_string());
            octave->SetDisPlace(octave->AttOctaveDisplacement::StrToStaffrelBasic(placeStr.c_str()));
            octave->SetN(xmlShift.attribute("number").as_string());
            const short int octDisNum = xmlShift.attribute("size") ? xmlShift.attribute("size").as_int() : 8;
            octave->SetDis(octave->AttOctaveDisplacement::StrToOctaveDis(std::to_string(octDisNum)));
            m_octDis[staffNum] = (octDisNum + 2) / 8;
            if (HasAttributeWithValue(xmlShift, "type", "up")) {
                octave->SetDisPlace(STAFFREL_basic_below);
                m_octDis[staffNum] *= -1;
            }
            else {
                octave->SetDisPlace(STAFFREL_basic_above);
            }
            m_controlElements.push_back({ measureNum, octave });
            m_octaveStack.push_back(octave);
        }
    }

    // Pedal
    pugi::xml_node xmlPedal = typeNode.child("pedal");
    if (xmlPedal) {
        std::string pedalType = xmlPedal.attribute("type").as_string();
        bool pedalLine = xmlPedal.attribute("line").as_bool();
        if (pedalType != "continue") {
            Pedal *pedal = new Pedal();
            pedal->SetColor(xmlPedal.attribute("color").as_string());
            // pedal->SetN(xmlPedal.attribute("number").as_string());
            if (!placeStr.empty()) pedal->SetPlace(pedal->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
            pedal->SetDir(ConvertPedalTypeToDir(pedalType));
            if (pedalLine) pedal->SetForm(PEDALSTYLE_line);
            if (xmlPedal.attribute("abbreviated")) {
                pedal->SetGlyphAuth("smufl");
                AttModule::SetExternalsymbols(pedal, "glyph.num", "U+E651");
            }
            if (pedalType == "sostenuto") {
                pedal->SetFunc("sostenuto");
                if (xmlPedal.attribute("abbreviated")) {
                    pedal->SetGlyphAuth("smufl");
                    AttModule::SetExternalsymbols(pedal, "glyph.num", "U+E65A");
                }
            }
            if (staffNode) {
                pedal->SetStaff(pedal->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(staffNode.text().as_int() + staffOffset)));
            }
            else if (m_prevLayer) {
                pedal->SetStaff(pedal->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(dynamic_cast<Staff *>(m_prevLayer->GetParent())->GetN())));
            }
            else {
                pedal->SetStaff(pedal->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(1 + staffOffset)));
            }
            pedal->SetTstamp(timeStamp);
            if (pedalType == "stop") pedal->SetTstamp(timeStamp - 0.1);
            int defaultY = xmlPedal.attribute("default-y").as_int();
            defaultY = (defaultY * 10) + xmlPedal.attribute("relative-y").as_int();
            // parse the default_y attribute and transform to vgrp value, to vertically align pedal starts and stops
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 2000;
            pedal->SetVgrp(defaultY);
            m_controlElements.push_back({ measureNum, pedal });
            m_pedalStack.push_back(pedal);
        }
    }

    // Principal voice
    pugi::xml_node lead = typeNode.child("principal-voice");
    if (lead) {
        short int voiceNumber = lead.attribute("number").as_int();
        voiceNumber = (voiceNumber < 1) ? 1 : voiceNumber;
        if (HasAttributeWithValue(lead, "type", "stop")) {
            const int measureDifference
                = m_measureCounts.at(measure) - m_bracketStack.front().second.m_lastMeasureCount;
            if (measureDifference >= 0) {
                m_bracketStack.front().first->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
            }
            m_bracketStack.erase(m_bracketStack.begin());
        }
        else {
            // std::string symbol = lead.node().attribute("symbol").as_string();
            BracketSpan *bracketSpan = new BracketSpan();
            musicxml::OpenSpanner openBracket(voiceNumber, m_measureCounts.at(measure));
            bracketSpan->SetColor(lead.attribute("color").as_string());
            // bracketSpan->SetPlace(bracketSpan->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
            bracketSpan->SetFunc("analytical");
            bracketSpan->SetLstartsym(ConvertLineEndSymbol(lead.attribute("symbol").as_string()));
            bracketSpan->SetTstamp(timeStamp);
            bracketSpan->SetType("principal-voice");
            m_controlElements.push_back({ measureNum, bracketSpan });
            m_bracketStack.push_back({ bracketSpan, openBracket });
        }
    }

    // Rehearsal
    pugi::xml_node rehearsal = typeNode.child("rehearsal");
    if (rehearsal) {
        Reh *reh = new Reh();
        reh->SetPlace(reh->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
        const std::string halign = rehearsal.attribute("halign").as_string();
        const std::string lang = rehearsal.attribute("xml:lang") ? rehearsal.attribute("xml:lang").as_string() : "it";
        const std::string textStr = GetContent(rehearsal);
        reh->SetColor(rehearsal.attribute("color").as_string());
        short int staffNum = staffNode.text().as_int() + staffOffset;
        staffNum = (staffNum < 1) ? 1 : staffNum;
        reh->SetStaff(reh->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(staffNum)));
        reh->SetLang(lang);
        Rend *rend = new Rend();
        rend->SetFontweight(rend->AttTypography::StrToFontweight(rehearsal.attribute("font-weight").as_string()));
        rend->SetHalign(rend->AttHorizontalAlign::StrToHorizontalalignment(halign));
        const std::string enclosure = rehearsal.attribute("enclosure").as_string();
        rend->SetRend(enclosure.empty() ? TEXTRENDITION_box : ConvertEnclosure(enclosure));
        Text *text = new Text();
        text->SetText(UTF8to32(textStr));
        rend->AddChild(text);
        reh->AddChild(rend);
        m_controlElements.push_back({ measureNum, reh });
    }

    // Segno
    pugi::xml_node xmlSegno = typeNode.child("segno");
    if (xmlSegno) {
        Dir *dir = new Dir();
        dir->SetPlace(dir->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
        dir->SetTstamp(timeStamp - 1.0);
        dir->SetType("segno");
        dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList("1"));
        if (xmlSegno.attribute("id")) dir->SetID(xmlSegno.attribute("id").as_string());
        Rend *rend = new Rend;
        rend->SetGlyphAuth("smufl");
        rend->SetFontstyle(FONTSTYLE_normal);
        rend->SetHalign(HORIZONTALALIGNMENT_center);
        Text *text = new Text();
        std::u32string segnoSign = UTF8to32("\xF0\x9D\x84\x8B");
        text->SetText(segnoSign);
        rend->AddChild(text);
        dir->AddChild(rend);
        m_controlElements.push_back({ measureNum, dir });
    }

    // Tempo
    if (containsTempo) {
        Tempo *tempo = new Tempo();
        if (!words.empty()) {
            const std::string lang = words.first().node().attribute("xml:lang")
                ? words.first().node().attribute("xml:lang").as_string()
                : "it";
            tempo->SetLang(lang);
        }
        tempo->SetPlace(tempo->AttPlacementRelStaff::StrToStaffrel(placeStr.c_str()));
        if (words.size() != 0) TextRendition(words, tempo);
        pugi::xpath_node metronome = node.select_node("direction-type/metronome[not(@print-object='no')]");
        if (metronome) PrintMetronome(metronome.node(), tempo);
        if (soundNode.attribute("tempo")) {
            tempo->SetMidiBpm(soundNode.attribute("tempo").as_double());
        }
        tempo->SetTstamp(timeStamp);
        if (staffNode) {
            tempo->SetStaff(tempo->AttStaffIdent::StrToXsdPositiveIntegerList(
                std::to_string(staffNode.text().as_int() + staffOffset)));
        }
        m_controlElements.push_back({ measureNum, tempo });
        m_tempoStack.push_back(tempo);
    }

    // other cases
    if (!containsDynamics && !containsTempo && !containsWords && !xmlCoda && !bracket && !lead && !xmlSegno && !xmlShift
        && !xmlPedal && wedges.empty() && !dashes && !rehearsal) {
        LogWarning("MusicXML import: Unsupported direction-type '%s'", typeNode.first_child().name());
    }
}

void MusicXmlInput::ReadMusicXmlFigures(pugi::xml_node node, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(measure);

    if (HasAttributeWithValue(node, "print-object", "no")) return;

    std::vector<F *> figures;

    const bool paren = node.attribute("parentheses").as_bool();

    // std::string textColor = node.attribute("color").as_string();
    // std::string textStyle = node.attribute("font-style").as_string();
    // std::string textWeight = node.attribute("font-weight").as_string();
    for (pugi::xml_node figure : node.children("figure")) {
        std::string textStr;
        if (paren) textStr.append("(");
        textStr.append(ConvertFigureGlyph(figure.child("prefix").text().as_string()));
        textStr.append(figure.child("figure-number").text().as_string());
        textStr.append(ConvertFigureGlyph(figure.child("suffix").text().as_string()));
        if (paren) textStr.append(")");
        if (textStr.empty()) continue;
        F *f = new F();
        pugi::xml_node extend = figure.child("extend");
        if (extend && !HasAttributeWithValue(extend, "type", "stop")) {
            f->SetExtender(BOOLEAN_true);
        }
        Text *text = new Text();
        text->SetText(UTF8to32(textStr));
        f->AddChild(text);
        figures.push_back(f);
    }
    if (figures.empty()) return;

    Harm *harm = new Harm();
    Fb *fb = new Fb();
    for (auto &fig : figures) {
        fb->AddChild(fig);
    }
    harm->AddChild(fb);
    harm->SetTstamp((double)(m_durTotal + m_durFb) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
    m_durFb += node.child("duration").text().as_int();
    m_controlElements.push_back({ measureNum, harm });
    m_harmStack.push_back(harm);
    figures.clear();
}

void MusicXmlInput::ReadMusicXmlForward(pugi::xml_node node, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(measure);

    if (!node.next_sibling()) {
        // fill the layer, if forward element is last sibling
        FillSpace(SelectLayer(node, measure), node.child("duration").text().as_int());
    }
    else {
        m_durTotal += node.child("duration").text().as_int();
    }
}

void MusicXmlInput::ReadMusicXmlHarmony(pugi::xml_node node, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(measure);

    int durOffset = 0;

    std::string harmText = GetContentOfChild(node, "root/root-step");
    pugi::xpath_node alter = node.select_node("root/root-alter");
    if (harmText.empty()) {
        pugi::xml_node numeral = node.select_node("numeral/numeral-root").node();
        harmText = numeral.attribute("text") ? numeral.attribute("text").as_string() : numeral.text().as_string();
        alter = node.select_node("numeral/numeral-alter");
    }
    if (alter) harmText += ConvertAlterToSymbol(GetContent(alter.node()));
    pugi::xml_node kind = node.child("kind");
    if (kind) {
        if (HasAttributeWithValue(kind, "use-symbols", "yes")) {
            harmText = harmText + ConvertKindToSymbol(GetContent(kind));
        }
        else if (kind.attribute("text") && std::strcmp(kind.text().as_string(), "none")) {
            harmText = harmText + kind.attribute("text").as_string();
        }
        else {
            harmText = harmText + ConvertKindToText(GetContent(kind));
        }
    }
    harmText += ConvertDegreeToText(node);
    pugi::xml_node bass = node.child("bass");
    if (bass) {
        harmText += "/";
        harmText += bass.child("bass-step").text().as_string()
            + ConvertAlterToSymbol(bass.child("bass-alter").text().as_string());
    }
    Harm *harm = new Harm();
    Text *text = new Text();
    text->SetText(UTF8to32(harmText));
    harm->SetPlace(harm->AttPlacementRelStaff::StrToStaffrel(node.attribute("placement").as_string()));
    harm->SetType(node.attribute("type").as_string());
    harm->AddChild(text);
    pugi::xml_node offset = node.child("offset");
    if (offset) durOffset = offset.text().as_int();
    harm->SetTstamp((double)(m_durTotal + durOffset) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
    m_controlElements.push_back({ measureNum, harm });
    m_harmStack.push_back(harm);
}

void MusicXmlInput::ReadMusicXmlNote(
    pugi::xml_node node, Measure *measure, const std::string &measureNum, const short int staffOffset, Section *section)
{
    assert(node);
    assert(measure);

    Layer *layer = SelectLayer(node, measure);
    assert(layer);
    m_prevLayer = layer;

    Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);
    // find staff's staffDef
    // TODO Tablature: is this the correct way to find a staff's staffDef?
    AttNIntegerComparison cnc(STAFFDEF, staff->GetN());
    StaffDef *staffDef = vrv_cast<StaffDef *>(m_doc->GetFirstScoreDef()->FindDescendantByComparison(&cnc));
    bool isTablature = false;
    Tuning *tuning = NULL;

    if (staffDef) {
        tuning = vrv_cast<Tuning *>(staffDef->FindDescendantByType(TUNING));
        const data_NOTATIONTYPE notationType = staffDef->GetNotationtype();
        isTablature = (notationType == NOTATIONTYPE_tab || notationType == NOTATIONTYPE_tab_guitar
            || notationType == NOTATIONTYPE_tab_lute_italian || notationType == NOTATIONTYPE_tab_lute_french
            || notationType == NOTATIONTYPE_tab_lute_german);
    }

    bool isChord = node.child("chord");

    // reset figured bass offset
    m_durFb = 0;

    LayerElement *element = NULL;
    Note *note = NULL;

    bool nextIsChord = false;
    double onset = m_durTotal; // keep note onsets for later

    // for measure repeats add a single <mRpt> and return
    if (m_mRpt) {
        MRpt *mRpt = vrv_cast<MRpt *>(layer->GetFirst(MRPT));
        if (!mRpt) {
            mRpt = new MRpt();
            AddLayerElement(layer, mRpt);
        }
        return;
    }

    const pugi::xpath_node notations = node.select_node("notations[not(@print-object='no')]");

    const bool cue = (node.child("cue") || node.select_node("type[@size='cue']")) ? true : false;
    pugi::xml_node grace = node.child("grace");

    // duration string and dots
    const std::string typeStr = node.child("type").text().as_string();
    const int dots = (int)node.select_nodes("dot").size();

    short int tremSlashNum = -1;

    const bool readBeamsAndTuplets = ReadMusicXmlBeamsAndTuplets(node, layer, isChord);

    // beam start
    bool beamStart = node.select_node("beam[@number='1'][text()='begin']");
    // tremolos
    pugi::xpath_node tremolo = notations.node().select_node("ornaments/tremolo");

    if (tremolo) {
        if (HasAttributeWithValue(tremolo.node(), "type", "start")) {
            if (!isChord) {
                FTrem *fTrem = new FTrem();
                AddLayerElement(layer, fTrem);
                m_elementStackMap.at(layer).push_back(fTrem);
                int beamFloatNum = tremolo.node().text().as_int(); // number of floating beams
                int beamAttachedNum = 0; // number of attached beams
                while (beamStart && beamAttachedNum < 8) { // count number of (attached) beams, max 8
                    std::ostringstream o;
                    o << "beam[@number='" << ++beamAttachedNum + 1 << "'][text()='begin']";
                    beamStart = node.select_node(o.str().c_str());
                }
                fTrem->SetBeams(beamFloatNum + beamAttachedNum);
                fTrem->SetBeamsFloat(beamFloatNum);
            }
        }
        else if (!HasAttributeWithValue(tremolo.node(), "type", "stop")) {
            // this is default tremolo type in MusicXML
            tremSlashNum = tremolo.node().text().as_int();
            if (!isChord) {
                BTrem *bTrem = new BTrem();
                AddLayerElement(layer, bTrem);
                m_elementStackMap.at(layer).push_back(bTrem);
                if (HasAttributeWithValue(tremolo.node(), "type", "unmeasured")) {
                    bTrem->SetForm(tremForm_FORM_unmeas);
                    tremSlashNum = 0;
                }
                else {
                    bTrem->SetForm(tremForm_FORM_meas);
                }
            }
        }
    }

    const std::string noteID = node.attribute("id").as_string();
    const int duration = node.child("duration").text().as_int();
    const int noteStaffNum = node.child("staff").text().as_int();
    const pugi::xml_node rest = node.child("rest");
    if (m_ppq < 0 && duration && !typeStr.empty()) {
        // if divisions are missing, try to calculate
        m_ppq = (double)duration * pow(2, ConvertTypeToDur(typeStr) - 2) / 4;
    }

    if (rest) {
        const std::string stepStr = rest.child("display-step").text().as_string();
        const std::string octaveStr = rest.child("display-octave").text().as_string();
        if (HasAttributeWithValue(node, "print-object", "no")) {
            Space *space = new Space();
            element = space;
            if (!typeStr.empty()) {
                space->SetDur(ConvertTypeToDur(typeStr));
                space->SetDurPpq(duration);
                if (dots > 0) space->SetDots(dots);
                if (!noteID.empty()) {
                    space->SetID(noteID);
                }
                // set @staff attribute, if existing and different from parent staff number
                if (noteStaffNum > 0 && noteStaffNum + staffOffset != staff->GetN())
                    space->SetStaff(
                        space->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(noteStaffNum + staffOffset)));
                AddLayerElement(layer, space, duration);
            }
            else {
                MSpace *mSpace = new MSpace();
                if (!noteID.empty()) {
                    mSpace->SetID(noteID);
                }
                AddLayerElement(layer, mSpace);
            }
        }
        // we assume /note without /type or with duration of an entire bar to be mRest
        else if (typeStr.empty() || rest.attribute("measure").as_bool()) {
            if (m_slash) {
                MeterSig tmpMeterSig;
                tmpMeterSig.SetCount({ m_meterCount, m_meterSign });
                const int totalCount = tmpMeterSig.GetTotalCount();
                for (int i = totalCount; i > 0; --i) {
                    BeatRpt *slash = new BeatRpt;
                    AddLayerElement(layer, slash, duration);
                }
                return;
            }
            else {
                MRest *mRest = new MRest();
                element = mRest;
                if (cue) mRest->SetCue(BOOLEAN_true);
                if (!stepStr.empty()) mRest->SetPloc(ConvertStepToPitchName(stepStr));
                if (!octaveStr.empty()) mRest->SetOloc(std::stoi(octaveStr));
                if (!noteID.empty()) {
                    mRest->SetID(noteID);
                }
                AddLayerElement(layer, mRest, duration);
            }
        }
        else {
            if (isTablature) {
                // rest
                TabGrp *tabGrp = new TabGrp();
                element = tabGrp;
                tabGrp->SetDur(ConvertTypeToDur(typeStr));
                tabGrp->SetDurPpq(duration);
                if (dots > 0) tabGrp->SetDots(dots);
                tabGrp->AddChild(new TabDurSym());
                AddLayerElement(layer, tabGrp, duration);
            }
            else {
                Rest *rest = new Rest();
                element = rest;
                rest->SetColor(node.attribute("color").as_string());
                rest->SetDur(ConvertTypeToDur(typeStr));
                rest->SetDurPpq(duration);
                if (dots > 0) rest->SetDots(dots);
                if (cue) rest->SetCue(BOOLEAN_true);
                if (!stepStr.empty()) rest->SetPloc(ConvertStepToPitchName(stepStr));
                if (!octaveStr.empty()) rest->SetOloc(std::stoi(octaveStr));
                if (!noteID.empty()) {
                    rest->SetID(noteID);
                }
                // set @staff attribute, if existing and different from parent staff number
                if (noteStaffNum > 0 && noteStaffNum + staffOffset != staff->GetN())
                    rest->SetStaff(
                        rest->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(noteStaffNum + staffOffset)));
                AddLayerElement(layer, rest, duration);
            }
        }
    }
    else {
        note = new Note();
        element = note;
        note->SetVisible(ConvertWordToBool(node.attribute("print-object").as_string()));
        note->SetColor(node.attribute("color").as_string());
        if (!noteID.empty()) {
            note->SetID(noteID);
        }
        note->SetRealTimeOnsetSeconds(onset); // remember the MIDI onset within that measure
        // set @staff attribute, if existing and different from parent staff number
        if (noteStaffNum > 0 && noteStaffNum + staffOffset != staff->GetN())
            note->SetStaff(
                note->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(noteStaffNum + staffOffset)));

        // accidental
        pugi::xml_node accidental = node.child("accidental");
        if (!accidental) {
            accidental = node.select_node("notations/accidental-mark").node();
        }
        if (accidental) {
            Accid *accid = new Accid();
            accid->SetAccid(ConvertAccidentalToAccid(accidental.text().as_string()));
            accid->SetColor(accidental.attribute("color").as_string());
            accid->SetGlyphName(accidental.attribute("smufl").as_string());
            accid->SetPlace(accid->AttPlacementRelEvent::StrToStaffrel(accidental.attribute("placement").as_string()));
            if (accidental.attribute("id")) accid->SetID(accidental.attribute("id").as_string());
            if (HasAttributeWithValue(accidental, "cautionary", "yes")) accid->SetFunc(accidLog_FUNC_caution);
            if (HasAttributeWithValue(accidental, "editorial", "yes")) accid->SetFunc(accidLog_FUNC_edit);
            if (HasAttributeWithValue(accidental, "bracket", "yes")) accid->SetEnclose(ENCLOSURE_brack);
            if (HasAttributeWithValue(accidental, "parentheses", "yes")) accid->SetEnclose(ENCLOSURE_paren);
            if (!strcmp(accidental.name(), "accidental-mark")) accid->SetOnstaff(BOOLEAN_false);
            note->AddChild(accid);
        }

        // stem direction - taken into account below for the chord or the note
        data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
        const pugi::xml_node stem = node.child("stem");
        const std::string stemText = stem.text().as_string();
        if (stemText == "down") {
            stemDir = STEMDIRECTION_down;
        }
        else if (stemText == "up") {
            stemDir = STEMDIRECTION_up;
        }

        // pitch and octave, optional, not needed for tablature
        pugi::xml_node pitch = node.child("pitch");
        if (pitch && !isTablature) {
            const std::string stepStr = pitch.child("step").text().as_string();
            const float alterVal = pitch.child("alter").text().as_float();
            const int octaveNum = pitch.child("octave").text().as_int();
            if (!stepStr.empty()) note->SetPname(ConvertStepToPitchName(stepStr));
            if (pitch.child("alter")) {
                Accid *accid = vrv_cast<Accid *>(note->GetFirst(ACCID));
                if (!accid) {
                    accid = new Accid();
                    note->AddChild(accid);
                    accid->IsAttribute(true);
                }
                const data_ACCIDENTAL_GESTURAL accidGes = ConvertAlterToAccid(alterVal);
                if (!IsSameAccidWrittenGestural(accid->GetAccid(), accidGes)) {
                    accid->SetAccidGes(accidGes);
                }
            }
            if (m_octDis[staff->GetN()] != 0) {
                note->SetOct(octaveNum - m_octDis[staff->GetN()]);
                note->SetOctGes(octaveNum);
            }
            else {
                note->SetOct(octaveNum);
            }
        }
        else if (node.child("unpitched")) {
            pugi::xml_node unpitched = node.child("unpitched");
            const std::string stepStr = unpitched.child("display-step").text().as_string();
            const int octaveNum = unpitched.child("display-octave").text().as_int();
            const int loc = note->CalcLoc(ConvertStepToPitchName(stepStr), octaveNum, -2);
            note->SetLoc(loc);
        }

        // dynamics (MIDI velocity)
        const float dynamics = node.attribute("dynamics").as_float(-1.0);
        if (dynamics >= 0.0) {
            note->SetVel(ConvertDynamicsToMidiVal(dynamics));
        }

        // notehead
        const pugi::xml_node notehead = node.child("notehead");
        if (notehead) {
            note->SetHeadColor(notehead.attribute("color").as_string());
            note->SetHeadShape(ConvertNotehead(notehead.text().as_string()));
            if (notehead.attribute("parentheses").as_bool()) note->SetHeadMod(NOTEHEADMODIFIER_paren);
            note->SetGlyphName(notehead.attribute("smufl").as_string());
            auto noteHeadFill = notehead.attribute("filled");
            if (noteHeadFill) note->SetHeadFill(noteHeadFill.as_bool() ? FILL_solid : FILL_void);
            if (!std::strncmp(notehead.text().as_string(), "none", 4)) note->SetHeadVisible(BOOLEAN_false);
        }
        if (node.child("notehead-text")) LogWarning("MusicXML import: notehead-text is not supported");

        // look at the next note to see if we are starting or ending a chord
        pugi::xpath_node nextNote = node.select_node("./following-sibling::note");
        if (nextNote.node().child("chord")) nextIsChord = true;
        Chord *chord = NULL;
        TabGrp *tabGrp = NULL;
        if (isTablature) {
            // create the tabGrp if we are starting a new tabGrp
            if (m_elementStackMap.at(layer).empty() || !m_elementStackMap.at(layer).back()->Is(TABGRP)) {
                tabGrp = new TabGrp();
                tabGrp->SetDur(ConvertTypeToDur(typeStr));
                tabGrp->SetDurPpq(duration);
                if (dots > 0) tabGrp->SetDots(dots);
                tabGrp->AddChild(new TabDurSym());
                AddLayerElement(layer, tabGrp, duration);
                m_elementStackMap.at(layer).push_back(tabGrp);
                element = tabGrp;
            }
        }
        else if (nextIsChord) {
            // create the chord if we are starting a new chord
            if (m_elementStackMap.at(layer).empty() || !m_elementStackMap.at(layer).back()->Is(CHORD)) {
                chord = new Chord();
                chord->SetDur(ConvertTypeToDur(typeStr));
                chord->SetDurPpq(duration);
                if (dots > 0) chord->SetDots(dots);
                chord->SetStemDir(stemDir);
                if (!strcmp(notehead.text().as_string(), "cluster")) chord->SetCluster(CLUSTER_white);
                if (stemText == "none") chord->SetStemVisible(BOOLEAN_false);
                if (tremSlashNum > 0) {
                    chord->SetStemMod(chord->AttStems::StrToStemmodifier(std::to_string(tremSlashNum) + "slash"));
                }
                else if (tremSlashNum == 0) {
                    chord->SetStemMod(STEMMODIFIER_z);
                }
                AddLayerElement(layer, chord, duration);
                m_elementStackMap.at(layer).push_back(chord);
                element = chord;
                if (cue) chord->SetCue(BOOLEAN_true);
                if (grace) {
                    if (grace.attribute("slash")) {
                        chord->SetGrace(GRACE_unacc);
                        chord->SetStemMod(STEMMODIFIER_1slash);
                    }
                    else {
                        chord->SetGrace(GRACE_acc);
                    }
                }
            }
        }
        // If the current note is part of a chord.
        if (!isTablature && (nextIsChord || node.child("chord"))) {
            if (chord == NULL && m_elementStackMap.at(layer).size() > 0
                && m_elementStackMap.at(layer).back()->Is(CHORD)) {
                chord = vrv_cast<Chord *>(m_elementStackMap.at(layer).back());
            }
            if (!chord) {
                LogError("MusicXML import: Chord starting point has not been found");
                return;
            }
            // Mark a chord as cue=true if and only if all its child notes are cue.
            // (This causes it to have a smaller stem).
            if (!cue) {
                chord->SetCue(BOOLEAN_NONE);
            }
            else if (cue && chord->GetCue() != BOOLEAN_NONE) {
                chord->SetCue(BOOLEAN_true);
            }
            grace = pugi::xml_node();
        }

        // single grace note
        if (grace) {
            if (grace.attribute("slash")) {
                note->SetGrace(GRACE_unacc);
                note->SetStemMod(STEMMODIFIER_1slash);
            }
            else {
                note->SetGrace(GRACE_acc);
            }
        }
        if (cue) note->SetCue(BOOLEAN_true);

        // set attributes to the note if we are not in a chord
        if (!isTablature && (m_elementStackMap.at(layer).empty() || !m_elementStackMap.at(layer).back()->Is(CHORD))) {
            if (!typeStr.empty()) note->SetDur(ConvertTypeToDur(typeStr));
            note->SetDurPpq(duration);
            if (dots > 0) note->SetDots(dots);
            note->SetStemDir(stemDir);
            if (node.attribute("default-y") && stem.attribute("default-y")) {
                float stemLen
                    = abs(node.attribute("default-y").as_float() - stem.attribute("default-y").as_float()) / 5;
                note->SetStemLen(stemLen);
            }
            if (stemText == "none") note->SetStemVisible(BOOLEAN_false);
            if (tremSlashNum > 0) {
                note->SetStemMod(chord->AttStems::StrToStemmodifier(std::to_string(tremSlashNum) + "slash"));
            }
            else if (tremSlashNum == 0) {
                note->SetStemMod(STEMMODIFIER_z);
            }
        }

        // beamspan
        if (!readBeamsAndTuplets) {
            BeamSpan *meiBeamSpan = new BeamSpan();
            meiBeamSpan->SetStartid("#" + element->GetID());
            m_controlElements.push_back({ measureNum, meiBeamSpan });
            m_beamspanStack.push_back({ meiBeamSpan, { staff->GetN(), layer->GetN() } });
        }

        // verse / syl
        for (pugi::xml_node lyric : node.children("lyric")) {
            short int lyricNumber = lyric.attribute("number").as_int();
            lyricNumber = (lyricNumber < 1) ? 1 : lyricNumber;
            Verse *verse = new Verse();
            verse->SetColor(lyric.attribute("color").as_string());
            // verse->SetPlace(verse->AttPlacementRelStaff::StrToStaffrelBasic(lyric.attribute("placement").as_string()));
            verse->SetLabel(lyric.attribute("name").as_string());
            verse->SetN(lyricNumber);
            std::string syllabic = "single";
            for (pugi::xml_node childNode : lyric.children()) {
                if (!strcmp(childNode.name(), "syllabic")) syllabic = GetContent(childNode);
                if (!strcmp(childNode.name(), "text") && !HasAttributeWithValue(lyric, "print-object", "no")) {
                    // const std::string textColor = textNode.attribute("color").as_string();
                    const std::string textStyle = childNode.attribute("font-style").as_string();
                    const std::string textWeight = childNode.attribute("font-weight").as_string();
                    const short int lineThrough = childNode.attribute("line-through").as_int();
                    const std::string lang = childNode.attribute("xml:lang").as_string();
                    std::string textStr = childNode.text().as_string();

                    // convert verse numbers to labels
                    std::regex labelSearch("^([^[:alpha:]]*\\d[^[:alpha:]]*)$");
                    std::smatch labelSearchMatches;
                    std::regex labelPrefixSearch("^([^[:alpha:]]*\\d[^[:alpha:]]*)[\\s\\u00A0]+");
                    std::smatch labelPrefixSearchMatches;
                    if (!textStr.empty() && std::regex_search(textStr, labelSearchMatches, labelSearch)
                        && labelSearchMatches.ready() && childNode.next_sibling("elision")) {
                        // entire textStr is a label (MusicXML from Finale)

                        Label *label = new Label();

                        Text *text = new Text();
                        text->SetText(UTF8to32(labelSearchMatches[0]));
                        label->AddChild(text);
                        verse->AddChild(label);

                        continue;
                    }
                    else if (!textStr.empty() && std::regex_search(textStr, labelPrefixSearchMatches, labelPrefixSearch)
                        && labelPrefixSearchMatches.ready()) {
                        // first part of textStr is a label (MusicXML from Sibelius, MuseScore)

                        Label *label = new Label();

                        Text *text = new Text();
                        text->SetText(UTF8to32(labelPrefixSearchMatches[0].str().erase(
                            labelPrefixSearchMatches[0].str().find_last_not_of(" \f\n\r\t\v\u00A0") + 1)));
                        label->AddChild(text);
                        verse->AddChild(label);

                        textStr = textStr.erase(0, labelPrefixSearchMatches[0].length());
                    }

                    Syl *syl = new Syl();
                    syl->SetLang(lang.c_str());
                    if (syllabic == "single") {
                        syl->SetWordpos(sylLog_WORDPOS_s);
                        syl->SetCon(sylLog_CON_s);
                    }
                    else if (syllabic == "begin") {
                        syl->SetWordpos(sylLog_WORDPOS_i);
                        syl->SetCon(sylLog_CON_d);
                    }
                    else if (syllabic == "middle") {
                        syl->SetWordpos(sylLog_WORDPOS_m);
                        syl->SetCon(sylLog_CON_d);
                    }
                    else if (syllabic == "end") {
                        syl->SetWordpos(sylLog_WORDPOS_t);
                        syl->SetCon(sylLog_CON_s);
                    }

                    // override @con if we have elisions or extensions
                    if (childNode.next_sibling("elision")) {
                        syl->SetCon(sylLog_CON_b);
                    }
                    else if (lyric.child("extend")) {
                        syl->SetCon(sylLog_CON_u);
                    }

                    if (!textStyle.empty()) syl->SetFontstyle(syl->AttTypography::StrToFontstyle(textStyle.c_str()));
                    if (!textWeight.empty())
                        syl->SetFontweight(syl->AttTypography::StrToFontweight(textWeight.c_str()));

                    Text *text = new Text();
                    text->SetText(UTF8to32(textStr));
                    if (lineThrough) {
                        Rend *rend = new Rend();
                        rend->AddChild(text);
                        rend->SetRend(TEXTRENDITION_line_through);
                        syl->AddChild(rend);
                    }
                    else {
                        syl->AddChild(text);
                    }
                    verse->AddChild(syl);
                }
            }
            // TODO Tablature: <tabGrp> does not support child <verse>
            if (element->Is(CHORD) || element->Is(NOTE)) {
                element->AddChild(verse);
            }
            else {
                // this should not happen
                delete verse;
            }
        }

        // slurs
        pugi::xpath_node_set slurs = node.select_nodes("notations/slur");
        for (pugi::xpath_node_set::const_iterator it = slurs.begin(); it != slurs.end(); ++it) {
            pugi::xml_node slur = it->node();
            short int slurNumber = slur.attribute("number").as_int();
            slurNumber = (slurNumber < 1) ? 1 : slurNumber;
            const curvature_CURVEDIR dir = InferCurvedir(slur);
            if (HasAttributeWithValue(slur, "type", "stop")) {
                CloseSlur(measure, slurNumber, note, dir);
            }
            else if (HasAttributeWithValue(slur, "type", "start")) {
                Slur *meiSlur = new Slur();
                // color
                meiSlur->SetColor(slur.attribute("color").as_string());
                // lineform
                meiSlur->SetLform(meiSlur->AttLineRendBase::StrToLineform(slur.attribute("line-type").as_string()));
                if (slur.attribute("id")) meiSlur->SetID(slur.attribute("id").as_string());
                meiSlur->SetStartid("#" + note->GetID());
                // add it to the stack
                m_controlElements.push_back({ measureNum, meiSlur });
                OpenSlur(measure, slurNumber, meiSlur, dir);
            }
        }

        // ties
        ReadMusicXmlTies(node, layer, note, measureNum);

        // articulation
        std::vector<data_ARTICULATION> artics;
        for (pugi::xml_node articulations : notations.node().children("articulations")) {
            for (pugi::xml_node articulation : articulations.children()) {
                Artic *artic = new Artic();
                artics.push_back(ConvertArticulations(articulation.name()));
                if (!std::strcmp(articulation.name(), "detached-legato")) {
                    // we need to split up this one
                    artic->SetArtic(artics);
                    artic->SetColor(articulation.attribute("color").as_string());
                    artic->SetPlace(
                        artic->AttPlacementRelEvent::StrToStaffrel(articulation.attribute("placement").as_string()));
                    element->AddChild(artic);
                    artics.clear();
                    artic = new Artic();
                    artics.push_back(ARTICULATION_ten);
                }
                if (artics.back() == ARTICULATION_NONE) {
                    delete artic;
                    continue;
                }
                artic->SetArtic(artics);
                artic->SetColor(articulation.attribute("color").as_string());
                artic->SetPlace(
                    artic->AttPlacementRelEvent::StrToStaffrel(articulation.attribute("placement").as_string()));
                element->AddChild(artic);
                artics.clear();
            }
        }

        // technical
        for (pugi::xml_node technical : notations.node().children("technical")) {
            for (pugi::xml_node technicalChild : technical.children()) {
                const std::string technicalChildName = technicalChild.name();

                // fingering is handled on the same level as breath marks, dynamics, etc. so we skip it here
                if ((technicalChildName == "fingering") || (technicalChildName == "thumb-position")) continue;
                if (technicalChildName == "string") continue; // handled with fret

                if (technicalChildName == "fret") {
                    assert(isTablature);

                    // set @tab.string and @tab.fret
                    const int fret = technicalChild.text().as_int();
                    const int course = technical.child("string").text().as_int();
                    note->SetTabFret(fret);
                    note->SetTabCourse(course);

                    // Do we have the pitch for this note, if so do we have the tuning for this course?
                    pugi::xml_node pitch = node.child("pitch");
                    if (tuning && pitch) {
                        AttNNumberLikeComparison cnc(COURSE, std::to_string(course));
                        Course *courseTuning = vrv_cast<Course *>(tuning->FindDescendantByComparison(&cnc));

                        if (!courseTuning) {
                            // we have the note's pitch, but not the course's tuning, set it

                            const int midi = PitchToMidi(pitch.child("step").text().as_string(),
                                pitch.child("alter").text().as_int(),
                                pitch.child("octave").text().as_int()); // note's midi note number

                            // course's pitch
                            std::string stepStr;
                            int alterNum = 0;
                            int octaveNum = 0;
                            MidiToPitch(midi - fret, stepStr, alterNum, octaveNum);

                            courseTuning = new Course();
                            tuning->AddChild(courseTuning);

                            courseTuning->SetN(std::to_string(course));
                            courseTuning->SetPname(ConvertStepToPitchName(stepStr));
                            courseTuning->SetOct(octaveNum);

                            if (alterNum != 0) {
                                static_assert(
                                    static_cast<int>(ACCIDENTAL_WRITTEN_f) == static_cast<int>(ACCIDENTAL_GESTURAL_f));
                                static_assert(
                                    static_cast<int>(ACCIDENTAL_WRITTEN_s) == static_cast<int>(ACCIDENTAL_GESTURAL_s));
                                courseTuning->SetAccid(
                                    static_cast<data_ACCIDENTAL_WRITTEN>(ConvertAlterToAccid(alterNum)));
                            }
                        }
                    }
                }
                else {
                    pugi::xml_node articulation = technicalChild;
                    Artic *artic = new Artic();
                    artics.push_back(ConvertArticulations(articulation.name()));
                    if (artics.back() != ARTICULATION_NONE) artic->SetArtic(artics);
                    artic->SetColor(articulation.attribute("color").as_string());
                    artic->SetGlyphName(articulation.attribute("smufl").as_string());
                    artic->SetPlace(
                        artic->AttPlacementRelEvent::StrToStaffrel(articulation.attribute("placement").as_string()));
                    artic->SetType("technical");
                    element->AddChild(artic);
                    artics.clear();
                }
            }
        }

        // add the note to the layer or to the current container
        AddLayerElement(layer, note, duration);

        // if we are ending a chord or tabGrp remove it from the stack
        if (!nextIsChord) {
            const ClassId classId = isTablature ? TABGRP : CHORD;
            if (!m_elementStackMap.at(layer).empty() && m_elementStackMap.at(layer).back()->Is(classId)) {
                SetChordStaff(layer);

                RemoveLastFromStack(classId, layer);
            }
        }
    }

    // add duration to measure time
    if (!nextIsChord) m_durTotal += duration;

    m_ID = "#" + element->GetID();

    // breath marks
    pugi::xpath_node xmlBreath = notations.node().select_node("articulations/breath-mark");
    if (xmlBreath) {
        Breath *breath = new Breath();
        m_controlElements.push_back({ measureNum, breath });
        breath->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        breath->SetPlace(
            breath->AttPlacementRelStaff::StrToStaffrel(xmlBreath.node().attribute("placement").as_string()));
        breath->SetColor(xmlBreath.node().attribute("color").as_string());
        breath->SetTstamp((double)(m_durTotal) * (double)m_meterUnit / (double)(4 * m_ppq) + 0.5);
    }

    // caesura
    pugi::xpath_node xmlCaesura = notations.node().select_node("articulations/caesura");
    if (xmlCaesura) {
        Caesura *caesura = new Caesura();
        m_controlElements.push_back({ measureNum, caesura });
        caesura->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        caesura->SetPlace(
            caesura->AttPlacementRelStaff::StrToStaffrel(xmlCaesura.node().attribute("placement").as_string()));
        caesura->SetColor(xmlCaesura.node().attribute("color").as_string());
        caesura->SetTstamp((double)(m_durTotal) * (double)m_meterUnit / (double)(4 * m_ppq) + 0.5);
    }

    // dynamics
    pugi::xml_node xmlDynam = notations.node().child("dynamics");
    if (xmlDynam) {
        Dynam *dynam = new Dynam();
        m_controlElements.push_back({ measureNum, dynam });
        dynam->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        dynam->SetStartid(m_ID);
        if (xmlDynam.attribute("id")) dynam->SetID(xmlDynam.attribute("id").as_string());
        // place
        dynam->SetPlace(dynam->AttPlacementRelStaff::StrToStaffrel(xmlDynam.attribute("placement").as_string()));
        int defaultY = xmlDynam.attribute("default-y").as_int();
        defaultY = (defaultY * 10) + xmlDynam.attribute("relative-y").as_int();
        defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 2000;
        dynam->SetVgrp(defaultY);
        std::string dynamStr;
        for (pugi::xml_node xmlDynamPart : xmlDynam.children()) {
            if (std::string(xmlDynamPart.name()) == "other-dynamics") {
                dynamStr += xmlDynamPart.text().as_string();
            }
            else {
                dynamStr += xmlDynamPart.name();
            }
            if (xmlDynamPart != xmlDynam.last_child()) dynamStr += " ";
        }
        Text *text = new Text();
        text->SetText(UTF8to32(dynamStr));
        dynam->AddChild(text);
    }

    // fermatas
    pugi::xml_node xmlFermata = notations.node().child("fermata");
    if (xmlFermata) {
        Fermata *fermata = new Fermata();
        m_controlElements.push_back({ measureNum, fermata });
        fermata->SetStartid(m_ID);
        fermata->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        if (xmlFermata.attribute("id")) fermata->SetID(xmlFermata.attribute("id").as_string());
        ShapeFermata(fermata, xmlFermata);
    }

    // fingering
    auto xmlFing = notations.node().select_node("technical/fingering");
    if (xmlFing) {
        const std::string fingText = xmlFing.node().text().as_string();
        Fing *fing = new Fing();
        Text *text = new Text();
        text->SetText(UTF8to32(fingText));
        m_controlElements.push_back({ measureNum, fing });
        const std::string startID = note ? ("#" + note->GetID()) : m_ID;
        fing->SetStartid(startID);
        fing->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        fing->SetPlace(fing->AttPlacementRelStaff::StrToStaffrel(xmlFing.node().attribute("placement").as_string()));
        fing->AddChild(text);
    }

    // glissando and slide
    pugi::xpath_node_set glissandi = notations.node().select_nodes("glissando|slide");
    for (pugi::xpath_node_set::const_iterator it = glissandi.begin(); it != glissandi.end(); ++it) {
        std::string noteID = m_ID;
        // prevent from using chords or tabGrps
        if (element->Is(CHORD) || element->Is(TABGRP)) noteID = "#" + element->GetChild(0)->GetID();
        pugi::xml_node xmlGlissando = it->node();
        if (HasAttributeWithValue(xmlGlissando, "type", "start")) {
            Gliss *gliss = new Gliss();
            m_controlElements.push_back({ measureNum, gliss });
            gliss->SetColor(xmlGlissando.attribute("color").as_string());
            gliss->SetLform(gliss->AttLineRendBase::StrToLineform(xmlGlissando.attribute("line-type").as_string()));
            gliss->SetN(xmlGlissando.attribute("number").as_string());
            gliss->SetStartid(noteID);
            gliss->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            gliss->SetType(xmlGlissando.name());
            if (xmlGlissando.attribute("id")) gliss->SetID(xmlGlissando.attribute("id").as_string());
            m_glissStack.push_back(gliss);
        }
        else if (!m_glissStack.empty()) {
            short int extNumber = xmlGlissando.attribute("number").as_int();
            std::vector<Gliss *>::iterator iter = m_glissStack.begin();
            while (iter != m_glissStack.end()) {
                if ((atoi(((*iter)->GetN()).c_str()) == extNumber) && ((*iter)->GetType() == xmlGlissando.name())) {
                    (*iter)->SetEndid(noteID);
                    iter = m_glissStack.erase(iter);
                }
                else {
                    ++iter;
                }
            }
        }
    }

    // mordents
    pugi::xpath_node xmlMordent = notations.node().select_node("ornaments/*[contains(name(), 'mordent')]");
    if (xmlMordent) {
        Mordent *mordent = new Mordent();
        m_controlElements.push_back({ measureNum, mordent });
        mordent->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        mordent->SetStartid(m_ID);
        // color
        mordent->SetColor(xmlMordent.node().attribute("color").as_string());
        // long
        mordent->SetLong(ConvertWordToBool(xmlMordent.node().attribute("long").as_string()));
        // place
        mordent->SetPlace(
            mordent->AttPlacementRelStaff::StrToStaffrel(xmlMordent.node().attribute("placement").as_string()));
        // form
        mordent->SetForm(mordentLog_FORM_lower);
        if (!std::strncmp(xmlMordent.node().name(), "inverted", 7)) {
            mordent->SetForm(mordentLog_FORM_upper);
        }
        for (pugi::xml_node xmlAccidMark : notations.node().children("accidental-mark")) {
            const data_ACCIDENTAL_WRITTEN accid = ConvertAccidentalToAccid(xmlAccidMark.text().as_string());
            if (HasAttributeWithValue(xmlAccidMark, "placement", "above")) {
                mordent->SetAccidupper(accid);
            }
            else if (HasAttributeWithValue(xmlAccidMark, "placement", "below")) {
                mordent->SetAccidlower(accid);
            }
            else {
                if (mordent->GetForm() == mordentLog_FORM_upper) mordent->SetAccidupper(accid);
                if (mordent->GetForm() == mordentLog_FORM_lower) mordent->SetAccidlower(accid);
            }
        }
        if (BOOLEAN_true == mordent->GetLong()) {
            int mordentFlags = (mordentLog_FORM_upper == mordent->GetForm()) ? FORM_Inverted : FORM_Normal;
            if (xmlMordent.node().attribute("approach")) {
                mordentFlags |= (std::string(xmlMordent.node().attribute("approach").as_string()) == "above")
                    ? APPR_Above
                    : APPR_Below;
            }
            if (xmlMordent.node().attribute("departure")) {
                mordentFlags |= (std::string(xmlMordent.node().attribute("departure").as_string()) == "above")
                    ? DEP_Above
                    : DEP_Below;
            }
            const std::string smuflCode = GetOrnamentGlyphNumber(mordentFlags);
            if (!smuflCode.empty()) {
                AttModule::SetExternalsymbols(mordent, "glyph.num", smuflCode);
                AttModule::SetExternalsymbols(mordent, "glyph.auth", "smufl");
            }
        }
    }

    // schleifer/haydn (counts as mordent with different glyph)
    pugi::xpath_node xmlExtOrnament
        = notations.node().select_node("ornaments/*[contains(name(), 'schleifer') or contains(name(), 'haydn')]");
    if (xmlExtOrnament) {
        Mordent *mordent = new Mordent();
        m_controlElements.push_back({ measureNum, mordent });
        mordent->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        mordent->SetStartid(m_ID);
        // color
        mordent->SetColor(xmlExtOrnament.node().attribute("color").as_string());
        // place
        mordent->SetPlace(
            mordent->AttPlacementRelStaff::StrToStaffrel(xmlExtOrnament.node().attribute("placement").as_string()));
        const bool isHaydn = std::string(xmlExtOrnament.node().name()) == "haydn";
        AttModule::SetExternalsymbols(mordent, "glyph.num", isHaydn ? "U+E56F" : "U+E587");
        AttModule::SetExternalsymbols(mordent, "glyph.auth", "smufl");
    }

    // trill
    pugi::xpath_node xmlTrill = notations.node().select_node("ornaments/trill-mark");
    pugi::xpath_node xmlTrillLine = notations.node().select_node("ornaments/wavy-line[@type='start']");
    if (xmlTrill || xmlTrillLine) {
        Trill *trill = new Trill();
        m_controlElements.push_back({ measureNum, trill });
        trill->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        trill->SetStartid(m_ID);
        // color
        trill->SetColor(xmlTrill.node().attribute("color").as_string());
        // place
        trill->SetPlace(trill->AttPlacementRelStaff::StrToStaffrel(xmlTrill.node().attribute("placement").as_string()));
        if (xmlTrillLine) {
            trill->SetExtender(BOOLEAN_true);
            trill->SetN(xmlTrillLine.node().attribute("number").as_string());
            if (!xmlTrill) {
                trill->SetLstartsym(LINESTARTENDSYMBOL_none);
                trill->SetColor(xmlTrillLine.node().attribute("color").as_string());
                trill->SetPlace(
                    trill->AttPlacementRelStaff::StrToStaffrel(xmlTrillLine.node().attribute("placement").as_string()));
            }
            musicxml::OpenSpanner openTrill(1, m_measureCounts.at(measure));
            m_trillStack.push_back({ trill, openTrill });
        }
        for (pugi::xml_node xmlAccidMark = xmlTrill.node().next_sibling("accidental-mark"); xmlAccidMark;
             xmlAccidMark = xmlAccidMark.next_sibling("accidental-mark")) {
            if (HasAttributeWithValue(xmlAccidMark, "placement", "below")) {
                trill->SetAccidlower(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
            else {
                trill->SetAccidupper(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
        }
    }
    if (!m_trillStack.empty() && notations.node().select_node("ornaments/wavy-line[@type='stop']")) {
        short int extNumber
            = notations.node().select_node("ornaments/wavy-line[@type='stop']").node().attribute("number").as_int();
        std::vector<std::pair<Trill *, musicxml::OpenSpanner>>::iterator iter = m_trillStack.begin();
        while (iter != m_trillStack.end()) {
            const int measureDifference = m_measureCounts.at(measure) - iter->second.m_lastMeasureCount;
            if (atoi(((iter->first)->GetN()).c_str()) == extNumber) {
                (iter->first)
                    ->SetTstamp2(std::pair<int, double>(
                        measureDifference, (double)(m_durTotal) * (double)m_meterUnit / (double)(4 * m_ppq) + 1));
                iter = m_trillStack.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }

    // turns
    pugi::xpath_node xmlTurn = notations.node().select_node("ornaments/*[contains(name(), 'turn')]");
    if (xmlTurn) {
        Turn *turn = new Turn();
        m_controlElements.push_back({ measureNum, turn });
        turn->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        turn->SetStartid(m_ID);
        turn->SetColor(xmlTurn.node().attribute("color").as_string());
        turn->SetPlace(turn->AttPlacementRelStaff::StrToStaffrel(xmlTurn.node().attribute("placement").as_string()));
        turn->SetForm(turnLog_FORM_upper);
        for (pugi::xml_node xmlAccidMark = xmlTurn.node().next_sibling("accidental-mark"); xmlAccidMark;
             xmlAccidMark = xmlAccidMark.next_sibling("accidental-mark")) {
            if (HasAttributeWithValue(xmlAccidMark, "placement", "above")) {
                turn->SetAccidupper(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
            else if (HasAttributeWithValue(xmlAccidMark, "placement", "below")) {
                turn->SetAccidlower(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
            else {
                LogWarning("MusicXML import: Cannot add an accidental to a turn without placement");
            }
        }
        if (xmlTurn.node().attribute("slash").as_bool()) {
            AttModule::SetExternalsymbols(turn, "glyph.auth", "smufl");
            AttModule::SetExternalsymbols(turn, "glyph.num", "U+E569");
        }
        if (!std::strncmp(xmlTurn.node().name(), "inverted", 8)) {
            turn->SetForm(turnLog_FORM_lower);
            if (std::string(xmlTurn.node().name()).find("vertical") != std::string::npos) {
                turn->SetType("vertical");
                AttModule::SetExternalsymbols(turn, "glyph.auth", "smufl");
                AttModule::SetExternalsymbols(turn, "glyph.num", "U+E56B");
            }
        }
        if (!std::strncmp(xmlTurn.node().name(), "delayed", 7)) {
            turn->SetDelayed(BOOLEAN_true);
        }
        if (!std::strncmp(xmlTurn.node().name(), "vertical", 8)) {
            turn->SetType("vertical");
            AttModule::SetExternalsymbols(turn, "glyph.auth", "smufl");
            AttModule::SetExternalsymbols(turn, "glyph.num", "U+E56A");
        }
    }

    // arpeggio
    pugi::xpath_node xmlArpeggiate = notations.node().select_node("*[contains(name(), 'arpeggiate')]");
    if (xmlArpeggiate) {
        short int arpegN = xmlArpeggiate.node().attribute("number").as_int();
        arpegN = (arpegN < 1) ? 1 : arpegN;
        const std::string direction = xmlArpeggiate.node().attribute("direction").as_string();
        bool added = false;
        if (!m_ArpeggioStack.empty()) { // check existing arpeggios
            for (auto iter = m_ArpeggioStack.begin(); iter != m_ArpeggioStack.end(); ++iter) {
                if (iter->second.m_arpegN == arpegN && onset == iter->second.m_timeStamp) {
                    // don't add other chord notes, because the chord is already referenced.
                    if (!isChord) iter->first->GetPlistInterface()->AddRef("#" + element->GetID());
                    added = true; // so that no new Arpeg gets created below
                    break;
                }
            }
        }
        if (!added) {
            Arpeg *arpeggio = new Arpeg();
            arpeggio->GetPlistInterface()->AddRef("#" + element->GetID());
            // color
            arpeggio->SetColor(xmlArpeggiate.node().attribute("color").as_string());
            // direction (up/down) and in MEI arrow
            if (!direction.empty()) {
                arpeggio->SetArrow(BOOLEAN_true);
                if (direction == "up") {
                    arpeggio->SetOrder(arpegLog_ORDER_up);
                }
                else if (direction == "down") {
                    arpeggio->SetOrder(arpegLog_ORDER_down);
                }
                else {
                    arpeggio->SetOrder(arpegLog_ORDER_NONE);
                }
            }
            if (!std::strncmp(xmlArpeggiate.node().name(), "non", 3)) {
                arpeggio->SetOrder(arpegLog_ORDER_nonarp);
            }
            m_ArpeggioStack.push_back({ arpeggio, musicxml::OpenArpeggio(arpegN, onset) });
            m_controlElements.push_back({ measureNum, arpeggio });
        }
    }

    // tremolo end
    if (tremolo) {
        if (HasAttributeWithValue(tremolo.node(), "type", "stop")) {
            RemoveLastFromStack(FTREM, layer);
        }
        else if (!HasAttributeWithValue(tremolo.node(), "type", "start") && !isChord) {
            RemoveLastFromStack(BTREM, layer);
        }
    }

    // tuplet end
    pugi::xpath_node tupletEnd = node.select_node("notations/tuplet[@type='stop']");
    if (tupletEnd) {
        RemoveLastFromStack(TUPLET, layer);
    }

    // beam end
    bool beamEnd = node.select_node("beam[text()='end']");
    if (beamEnd) {
        int breakSec = (int)node.select_nodes("beam[text()='continue']").size();
        if (breakSec) {
            if (element->Is(NOTE)) {
                Note *note = vrv_cast<Note *>(element);
                note->SetBreaksec(breakSec);
            }
            else if (element->Is(CHORD)) {
                Chord *chord = vrv_cast<Chord *>(element);
                chord->SetBreaksec(breakSec);
            }
            else if (element->Is(TABGRP)) {
                TabGrp *tabGrp = vrv_cast<TabGrp *>(element);
                tabGrp->SetBreaksec(breakSec);
            }
            if (element->Is(REST)) {
                Rest *rest = vrv_cast<Rest *>(element);
                rest->SetBreaksec(breakSec);
            }
        }
        else {
            if (IsInStack(BEAM, layer)) {
                RemoveLastFromStack(BEAM, layer);
            }
            else {
                CloseBeamSpan(staff, layer, element);
            }
        }
    }

    // add StartIDs to dir, dynam, and pedal
    if (!m_dirStack.empty()) {
        for (auto &dir : m_dirStack) {
            if (!dir->HasStaff()) {
                dir->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            }
        }
        m_dirStack.clear();
    }
    if (!m_dynamStack.empty()) {
        for (auto &dynam : m_dynamStack) {
            if (!dynam->HasStaff()) {
                dynam->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            }
        }
        m_dynamStack.clear();
    }
    if (!m_harmStack.empty()) {
        for (auto &harm : m_harmStack) {
            harm->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
        m_harmStack.clear();
    }
    if (!m_octaveStack.empty()) {
        for (auto &oct : m_octaveStack) {
            oct->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            oct->SetStartid(m_ID);
        }
        m_octaveStack.clear();
    }
    if (!m_pedalStack.empty()) {
        for (auto &ped : m_pedalStack) {
            if (!ped->HasStaff()) {
                ped->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            }
        }
        m_pedalStack.clear();
    }
    if (!m_bracketStack.empty()) {
        for (auto iter = m_bracketStack.begin(); iter != m_bracketStack.end(); ++iter) {
            if (!(iter->first)->HasStaff())
                iter->first->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
    }
    if (!m_tempoStack.empty()) {
        for (auto iter = m_tempoStack.begin(); iter != m_tempoStack.end(); ++iter) {
            if (!(*iter)->HasStaff())
                (*iter)->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
        m_tempoStack.clear();
    }
}

void MusicXmlInput::ReadMusicXmlPrint(pugi::xml_node node, Section *section)
{
    assert(node);
    assert(section);

    if (node.attribute("new-page").as_bool()) {
        const int pageBreaks = node.attribute("blank-page").as_int() + 1;
        for (int i = 0; i < pageBreaks; ++i) {
            Pb *pb = new Pb();
            section->AddChild(pb);
        }
    }

    if (node.attribute("new-system").as_bool()) {
        Sb *sb = new Sb();
        section->AddChild(sb);
    }

    if (std::string(node.child("measure-numbering").text().as_string()) == "none") {
        m_doc->GetFirstScoreDef()->SetMnumVisible(BOOLEAN_false);
    }
}

bool MusicXmlInput::ReadMusicXmlBeamsAndTuplets(const pugi::xml_node &node, Layer *layer, bool isChord)
{
    pugi::xpath_node beamStart = node.select_node("beam[@number='1' and text()='begin']");
    pugi::xpath_node tupletStart = node.select_node("notations/tuplet[@type='start']");
    pugi::xpath_node currentMeasure = node.select_node("ancestor::measure");

    pugi::xml_node beamEnd = node.select_node("./following-sibling::note[beam[@number='1' and text()='end']]").node();
    pugi::xml_node tupletEnd = node.select_node("./following-sibling::note[notations/tuplet[@type='stop']]").node();

    const auto measureNodeChildren = currentMeasure.node().children();
    std::vector<pugi::xml_node> currentMeasureNodes(measureNodeChildren.begin(), measureNodeChildren.end());
    // in case note is a start of both beam and tuplet - need to figure which one is longer
    if (beamStart && tupletStart) {
        const auto beamEndIterator = std::find(currentMeasureNodes.begin(), currentMeasureNodes.end(), beamEnd);
        const auto tupletEndIterator = std::find(currentMeasureNodes.begin(), currentMeasureNodes.end(), tupletEnd);

        // find distance between iterator, i.e. whether beam or tuplet ends first.
        // Negative number - beam ends first, positive - tuplet, zero - both are of the same length
        const int distance = static_cast<int>(std::distance(beamEndIterator, tupletEndIterator));
        if (distance > 0) {
            if (!isChord) ReadMusicXmlTupletStart(node, tupletStart.node(), layer);
            ReadMusicXmlBeamStart(node, beamStart.node(), layer);
        }
        else {
            ReadMusicXmlBeamStart(node, beamStart.node(), layer);
            if (!isChord) ReadMusicXmlTupletStart(node, tupletStart.node(), layer);
        }
    }
    // If note is a start of the beam only - check if there is a tuplet starting/ending in the span of
    // the whole duration of this beam
    else if (beamStart) {
        // find whether there is a tuplet that starts during the span of the beam
        pugi::xpath_node nextTupletStart
            = node.select_node("./following-sibling::note[notations/tuplet[@type='start']]").node();

        // find start and end of the beam
        const auto beamStartIterator = std::find(currentMeasureNodes.begin(), currentMeasureNodes.end(), node);
        const auto beamEndIterator = std::find(beamStartIterator, currentMeasureNodes.end(), beamEnd);

        // find staff number for the corresponding elements - we do not want to match beam start on one staff with beam
        // end on another
        pugi::xpath_node nodeStaff = node.select_node("staff");
        pugi::xpath_node endBeamStaff = beamEnd.select_node("staff");

        if (beamEndIterator == currentMeasureNodes.end()
            || (nodeStaff && endBeamStaff
                && (nodeStaff.node().text().as_int() != endBeamStaff.node().text().as_int()))) {
            std::string measureName = (currentMeasure.node().attribute("id"))
                ? currentMeasure.node().attribute("id").as_string()
                : currentMeasure.node().attribute("number").as_string();
            LogDebug("MusicXML import: Beam without end in measure %s treated as <beamSpan>", measureName.c_str());
            return false;
        }
        // form vector of the beam nodes and find whether there are tuplets that start or end within the beam
        std::vector<pugi::xml_node> beamNodes(beamStartIterator, beamEndIterator + 1);
        bool isTupletStartInBeam
            = (beamNodes.end() != std::find(beamNodes.begin(), beamNodes.end(), nextTupletStart.node()));
        bool isTupletEndInBeam = (beamNodes.end() != std::find(beamNodes.begin(), beamNodes.end(), tupletEnd));
        // in case if there is only start/end of the tuplet in the beam, then we need to use beamSpan instead
        if ((tupletEnd != beamEnd) && (isTupletStartInBeam != isTupletEndInBeam)) {
            // TODO: same call as in else-case is intentional. Proper beamSpan support will need to be implemented
            // before this case can be handled correctly
            ReadMusicXmlBeamStart(node, beamStart.node(), layer);
        }
        else {
            ReadMusicXmlBeamStart(node, beamStart.node(), layer);
        }
    }
    // no special logic needed if we have just tupletStart - just read it as is
    else if (tupletStart) {
        if (!isChord) ReadMusicXmlTupletStart(node, tupletStart.node(), layer);
    }

    return true;
}

void MusicXmlInput::ReadMusicXmlTupletStart(const pugi::xml_node &node, const pugi::xml_node &tupletStart, Layer *layer)
{
    // TODO <tuplet> does not support child <tabGrp>

    if (!tupletStart) return;

    Tuplet *tuplet = new Tuplet();
    AddLayerElement(layer, tuplet);
    m_elementStackMap.at(layer).push_back(tuplet);
    short int num = node.select_node("time-modification/actual-notes").node().text().as_int();
    short int numbase = node.select_node("time-modification/normal-notes").node().text().as_int();
    if (tupletStart.first_child()) {
        num = tupletStart.select_node("tuplet-actual/tuplet-number").node().text().as_int();
        numbase = tupletStart.select_node("tuplet-normal/tuplet-number").node().text().as_int();
    }
    if (num) tuplet->SetNum(num);
    if (numbase) tuplet->SetNumbase(numbase);
    tuplet->SetNumPlace(tuplet->AttTupletVis::StrToStaffrelBasic(tupletStart.attribute("placement").as_string()));
    tuplet->SetBracketPlace(tuplet->AttTupletVis::StrToStaffrelBasic(tupletStart.attribute("placement").as_string()));
    tuplet->SetNumFormat(ConvertTupletNumberValue(tupletStart.attribute("show-number").as_string()));
    if (HasAttributeWithValue(tupletStart, "show-number", "none")) tuplet->SetNumVisible(BOOLEAN_false);
    tuplet->SetBracketVisible(ConvertWordToBool(tupletStart.attribute("bracket").as_string()));
}

void MusicXmlInput::ReadMusicXmlBeamStart(const pugi::xml_node &node, const pugi::xml_node &beamStart, Layer *layer)
{
    if (!beamStart || (node.select_node("notations/ornaments/tremolo[@type='start']"))) return;
    if (m_elementStackMap.at(layer).size() > 0 && m_elementStackMap.at(layer).back()->Is(BEAM)) {
        LogDebug("MusicXML import: Adding a beam to a beam");
        if (!node.child("grace")) return;
    }

    Beam *beam = new Beam();
    if (beamStart.attribute("id")) beam->SetID(beamStart.attribute("id").as_string());
    if (beamStart.attribute("fan")) beam->SetForm(ConvertBeamFanToForm(beamStart.attribute("fan").as_string()));
    AddLayerElement(layer, beam);
    m_elementStackMap.at(layer).push_back(beam);
}

void MusicXmlInput::ReadMusicXmlTies(
    const pugi::xml_node &node, Layer *layer, Note *note, const std::string &measureNum)
{
    pugi::xpath_node_set xmlTies = node.select_nodes("notations/tied");
    for (pugi::xpath_node_set::const_iterator it = xmlTies.begin(); it != xmlTies.end(); ++it) {
        pugi::xml_node xmlTie = (*it).node();
        std::string tieType = xmlTie.attribute("type").as_string();

        if (tieType.empty()) {
            continue;
        }
        else if (tieType == "stop") { // add to stack if (endTie) or if pitch/oct match to open tie on m_tieStack
            if (!m_tieStack.empty() && note->IsEnharmonicWith(m_tieStack.back().m_note)
                && (m_tieStack.back().m_layerNum == layer->GetN())) {
                m_tieStack.back().m_tie->SetEndid("#" + note->GetID());
                m_tieStack.pop_back();
            }
            else {
                CloseTie(note, layer->GetN());
            }
        }
        // if we have start attribute - start new tie
        else if (tieType == "start") {
            Tie *tie = new Tie();
            // color
            tie->SetColor(xmlTie.attribute("color").as_string());
            // placement and orientation
            tie->SetCurvedir(InferCurvedir(xmlTie));
            tie->SetLform(tie->AttLineRendBase::StrToLineform(xmlTie.attribute("line-type").as_string()));
            if (xmlTie.attribute("id")) tie->SetID(xmlTie.attribute("id").as_string());
            // add it to the stack
            m_controlElements.push_back({ measureNum, tie });
            OpenTie(note, tie, layer->GetN());
        }
        // or add lv element if let-ring attribute present
        else if (tieType == "let-ring") {
            Lv *lv = new Lv();
            // color
            lv->SetColor(xmlTie.attribute("color").as_string());
            // placement and orientation
            lv->SetCurvedir(InferCurvedir(xmlTie));
            lv->SetLform(lv->AttLineRendBase::StrToLineform(xmlTie.attribute("line-type").as_string()));
            if (xmlTie.attribute("id")) lv->SetID(xmlTie.attribute("id").as_string());
            // add it to the stack
            m_controlElements.push_back({ measureNum, lv });
            // set startid to the current note and set second timestamp (endpoint) right away, since we're going to link
            // <lv> not to another element, but to timestamp
            lv->SetStartid("#" + note->GetID());
            double tstamp = std::min(static_cast<double>(m_layerEndTimes[layer]), m_durTotal + 2.0);
            tstamp = std::max(tstamp, m_durTotal + 1.25);
            lv->SetTstamp2({ 0, tstamp * (double)m_meterUnit / (4.0 * m_ppq) + 1 });
        }
    }
}

Clef *MusicXmlInput::ConvertClef(const pugi::xml_node &clef)
{
    pugi::xml_node clefSign = clef.child("sign");
    if (clefSign && (std::string(clefSign.text().as_string()) != "none")) {
        Clef *meiClef = new Clef();
        meiClef->SetColor(clef.attribute("color").as_string());
        meiClef->SetVisible(ConvertWordToBool(clef.attribute("print-object").as_string()));
        if (clef.attribute("id")) {
            meiClef->SetID(clef.attribute("id").as_string());
        }
        meiClef->SetShape(meiClef->AttClefShape::StrToClefshape(GetContent(clefSign).substr(0, 4)));

        // clef line
        pugi::xml_node clefLine = clef.child("line");
        if (clefLine.text()) {
            if (meiClef->GetShape() != CLEFSHAPE_perc) {
                meiClef->SetLine(clefLine.text().as_int());
            }
        }
        else {
            switch (meiClef->GetShape()) {
                case CLEFSHAPE_C: meiClef->SetLine(3); break;
                case CLEFSHAPE_F: meiClef->SetLine(4); break;
                case CLEFSHAPE_G: meiClef->SetLine(2); break;
                case CLEFSHAPE_TAB: meiClef->SetLine(5); break;
                default: break;
            }
        }

        // clef octave change
        pugi::xml_node clefOctaveChange = clef.child("clef-octave-change");
        if (clefOctaveChange) {
            short int change = clefOctaveChange.text().as_int();
            switch (abs(change)) {
                case 1: meiClef->SetDis(OCTAVE_DIS_8); break;
                case 2: meiClef->SetDis(OCTAVE_DIS_15); break;
                case 3: meiClef->SetDis(OCTAVE_DIS_22); break;
                default: break;
            }
            if (change < 0) {
                meiClef->SetDisPlace(STAFFREL_basic_below);
            }
            else if (change > 0) {
                meiClef->SetDisPlace(STAFFREL_basic_above);
            }
        }
        return meiClef;
    }

    return NULL;
}

KeySig *MusicXmlInput::ConvertKey(const pugi::xml_node &key)
{
    KeySig *keySig = new KeySig();
    keySig->SetVisible(ConvertWordToBool(key.attribute("print-object").as_string()));
    if (key.attribute("id")) {
        keySig->SetID(key.attribute("id").as_string());
    }
    if (key.child("fifths")) {
        short int fifths = key.child("fifths").text().as_int();
        std::string keySigStr;
        if (fifths < 0) {
            keySigStr = StringFormat("%df", abs(fifths));
        }
        else if (fifths > 0) {
            keySigStr = StringFormat("%ds", fifths);
        }
        else {
            keySigStr = "0";
        }
        keySig->SetSig(keySig->AttKeySigLog::StrToKeysignature(keySigStr));

        if (key.child("cancel")) {
            keySig->SetCancelaccid(CANCELACCID_before);
        }
        if (key.child("mode")) {
            const std::string xmlMode = key.child("mode").text().as_string();
            if (std::strncmp(xmlMode.c_str(), "none", 4)) {
                keySig->SetMode(keySig->AttKeyMode::StrToMode(xmlMode));
            }
        }
    }
    else if (key.child("key-step")) {
        for (pugi::xml_node keyStep : key.children("key-step")) {
            KeyAccid *keyAccid = new KeyAccid();
            keyAccid->SetPname(ConvertStepToPitchName(keyStep.text().as_string()));
            if (std::strncmp(keyStep.next_sibling().name(), "key-alter", 9) == 0) {
                data_ACCIDENTAL_GESTURAL accidValue = ConvertAlterToAccid(keyStep.next_sibling().text().as_float());
                keyAccid->SetAccid(AreaPosInterface::AccidentalGesturalToWritten(accidValue));
                if (std::strncmp(keyStep.next_sibling().next_sibling().name(), "key-accidental", 14) == 0) {
                    keyAccid->SetAccid(
                        ConvertAccidentalToAccid(keyStep.next_sibling().next_sibling().text().as_string()));
                    keyAccid->SetGlyphName(keyStep.next_sibling().next_sibling().attribute("smufl").as_string());
                }
                else if (!keyAccid->HasAccid()) {
                    LogWarning("MusicXML import: Could not properly set keyAccid");
                }
            }
            keySig->AddChild(keyAccid);
        }
    }

    return keySig;
}

bool MusicXmlInput::IsSameAccidWrittenGestural(data_ACCIDENTAL_WRITTEN written, data_ACCIDENTAL_GESTURAL gestural)
{
    const std::map<data_ACCIDENTAL_WRITTEN, data_ACCIDENTAL_GESTURAL> writtenToGesturalMap{
        { ACCIDENTAL_WRITTEN_tf, ACCIDENTAL_GESTURAL_tf }, //
        { ACCIDENTAL_WRITTEN_ff, ACCIDENTAL_GESTURAL_ff }, //
        { ACCIDENTAL_WRITTEN_fd, ACCIDENTAL_GESTURAL_fd }, //
        { ACCIDENTAL_WRITTEN_f, ACCIDENTAL_GESTURAL_f }, //
        { ACCIDENTAL_WRITTEN_fu, ACCIDENTAL_GESTURAL_fu }, //
        { ACCIDENTAL_WRITTEN_n, ACCIDENTAL_GESTURAL_n }, //
        { ACCIDENTAL_WRITTEN_sd, ACCIDENTAL_GESTURAL_sd }, //
        { ACCIDENTAL_WRITTEN_s, ACCIDENTAL_GESTURAL_s }, //
        { ACCIDENTAL_WRITTEN_su, ACCIDENTAL_GESTURAL_su }, //
        { ACCIDENTAL_WRITTEN_ss, ACCIDENTAL_GESTURAL_ss }, //
        { ACCIDENTAL_WRITTEN_ts, ACCIDENTAL_GESTURAL_ts }
    };

    const auto result = writtenToGesturalMap.find(written);
    return ((result != writtenToGesturalMap.end()) && (result->second == gestural));
}

beamRend_FORM MusicXmlInput::ConvertBeamFanToForm(const std::string &value)
{
    if (value == "accel") return beamRend_FORM_acc;
    if (value == "none") return beamRend_FORM_norm;
    if (value == "rit") return beamRend_FORM_rit;

    return beamRend_FORM_NONE;
}

curvature_CURVEDIR MusicXmlInput::CombineCurvedir(curvature_CURVEDIR startDir, curvature_CURVEDIR stopDir)
{
    if (startDir == curvature_CURVEDIR_NONE) {
        return stopDir;
    }
    else if ((startDir != stopDir) && (stopDir != curvature_CURVEDIR_NONE)) {
        return curvature_CURVEDIR_mixed;
    }
    else {
        return startDir;
    }
}

//////////////////////////////////////////////////////////////////////////////
// String to attribute converters

data_ACCIDENTAL_WRITTEN MusicXmlInput::ConvertAccidentalToAccid(const std::string &value)
{
    static const std::map<std::string, data_ACCIDENTAL_WRITTEN> Accidental2Accid{ //
        { "sharp", ACCIDENTAL_WRITTEN_s }, //
        { "natural", ACCIDENTAL_WRITTEN_n }, //
        { "flat", ACCIDENTAL_WRITTEN_f }, //
        { "double-sharp", ACCIDENTAL_WRITTEN_x }, //
        { "sharp-sharp", ACCIDENTAL_WRITTEN_ss }, //
        { "flat-flat", ACCIDENTAL_WRITTEN_ff }, //
        { "natural-sharp", ACCIDENTAL_WRITTEN_ns }, //
        { "natural-flat", ACCIDENTAL_WRITTEN_nf }, //
        { "quarter-flat", ACCIDENTAL_WRITTEN_1qf }, //
        { "quarter-sharp", ACCIDENTAL_WRITTEN_1qs }, //
        { "three-quarters-flat", ACCIDENTAL_WRITTEN_3qf }, //
        { "three-quarters-sharp", ACCIDENTAL_WRITTEN_3qs }, //
        { "sharp-down", ACCIDENTAL_WRITTEN_sd }, //
        { "sharp-up", ACCIDENTAL_WRITTEN_su }, //
        { "natural-down", ACCIDENTAL_WRITTEN_nd }, //
        { "natural-up", ACCIDENTAL_WRITTEN_nu }, //
        { "flat-down", ACCIDENTAL_WRITTEN_fd }, //
        { "flat-up", ACCIDENTAL_WRITTEN_fu }, //
        { "double-sharp-down", ACCIDENTAL_WRITTEN_xd }, //
        { "double-sharp-up", ACCIDENTAL_WRITTEN_xu }, //
        { "flat-flat-down", ACCIDENTAL_WRITTEN_ffd }, //
        { "flat-flat-up", ACCIDENTAL_WRITTEN_ffu }, //
        { "triple-sharp", ACCIDENTAL_WRITTEN_ts }, //
        { "triple-flat", ACCIDENTAL_WRITTEN_tf }, //
        { "slash-quarter-sharp", ACCIDENTAL_WRITTEN_bms }, //
        { "slash-sharp", ACCIDENTAL_WRITTEN_ks }, //
        { "slash-flat", ACCIDENTAL_WRITTEN_bf }, //
        { "double-slash-flat", ACCIDENTAL_WRITTEN_bmf }, //
        { "sori", ACCIDENTAL_WRITTEN_sori }, //
        { "koron", ACCIDENTAL_WRITTEN_koron }, //
        { "other", ACCIDENTAL_WRITTEN_NONE }
    };

    const auto result = Accidental2Accid.find(value);
    if (result != Accidental2Accid.end()) {
        return result->second;
    }

    LogWarning("MusicXML import: Unsupported accidental value '%s'", value.c_str());
    return ACCIDENTAL_WRITTEN_NONE;
}

data_ACCIDENTAL_GESTURAL MusicXmlInput::ConvertAlterToAccid(const float value)
{
    static const std::map<float, data_ACCIDENTAL_GESTURAL> Alter2Accid{
        { -3, ACCIDENTAL_GESTURAL_tf }, //
        { -2.5, ACCIDENTAL_GESTURAL_ffd }, //
        { -2, ACCIDENTAL_GESTURAL_ff }, //
        { -1.5, ACCIDENTAL_GESTURAL_fd }, //
        { -1, ACCIDENTAL_GESTURAL_f }, //
        { -0.5, ACCIDENTAL_GESTURAL_fu }, //
        { 0, ACCIDENTAL_GESTURAL_n }, //
        { 0.5, ACCIDENTAL_GESTURAL_sd }, //
        { 1, ACCIDENTAL_GESTURAL_s }, //
        { 1.5, ACCIDENTAL_GESTURAL_su }, //
        { 2, ACCIDENTAL_GESTURAL_ss }, //
        { 2.5, ACCIDENTAL_GESTURAL_xu }, //
        { 2, ACCIDENTAL_GESTURAL_ts } //
    };

    const auto result = Alter2Accid.find(value);
    if (result != Alter2Accid.end()) {
        return result->second;
    }

    return ACCIDENTAL_GESTURAL_NONE;
}

data_ARTICULATION MusicXmlInput::ConvertArticulations(const std::string &value)
{
    static const std::map<std::string, data_ARTICULATION> Articulations2Id{
        // articulations
        { "accent", ARTICULATION_acc }, //
        { "detached-legato", ARTICULATION_stacc }, //
        { "doit", ARTICULATION_doit }, //
        { "falloff", ARTICULATION_fall }, //
        { "plop", ARTICULATION_plop }, //
        { "scoop", ARTICULATION_scoop }, //
        { "soft-accent", ARTICULATION_acc_soft }, //
        { "spiccato", ARTICULATION_spicc }, //
        { "staccatissimo", ARTICULATION_stacciss }, //
        { "staccato", ARTICULATION_stacc }, //
        { "strong-accent", ARTICULATION_marc }, //
        { "tenuto", ARTICULATION_ten }, //
        // technical
        { "bend", ARTICULATION_bend }, //
        { "double-tongue", ARTICULATION_dbltongue }, //
        { "down-bow", ARTICULATION_dnbow }, //
        { "fingernails", ARTICULATION_fingernail }, //
        { "harmonic", ARTICULATION_harm }, //
        { "heel", ARTICULATION_heel }, //
        { "open-string", ARTICULATION_open }, //
        { "snap-pizzicato", ARTICULATION_snap }, //
        { "stopped", ARTICULATION_stop }, //
        { "toe", ARTICULATION_toe }, //
        { "triple-tongue", ARTICULATION_trpltongue }, //
        { "up-bow", ARTICULATION_upbow } //
    };

    const auto result = Articulations2Id.find(value);
    if (result != Articulations2Id.end()) {
        return result->second;
    }

    return ARTICULATION_NONE;
}

data_BARRENDITION MusicXmlInput::ConvertStyleToRend(const std::string &value, const bool repeat)
{
    if (value == "dashed") return BARRENDITION_dashed;
    if (value == "dotted") return BARRENDITION_dotted;
    if (value == "light-light") return BARRENDITION_dbl;
    if (value == "heavy-heavy") return BARRENDITION_dblheavy;
    if ((value == "light-heavy") && !repeat) return BARRENDITION_end;
    if (value == "heavy") return BARRENDITION_heavy;
    if (value == "none") return BARRENDITION_invis;
    if ((value == "heavy-light") && repeat) return BARRENDITION_rptstart;
    // if (value == "") return BARRENDITION_rptboth;
    if ((value == "light-heavy") && repeat) return BARRENDITION_rptend;
    if (value == "regular") return BARRENDITION_single;
    if (value == "short") return BARRENDITION_single;
    if (value == "tick") return BARRENDITION_single;
    LogWarning("MusicXML import: Unsupported bar-style '%s'", value.c_str());
    return BARRENDITION_NONE;
}

data_BOOLEAN MusicXmlInput::ConvertWordToBool(const std::string &value)
{
    if (value == "yes") return BOOLEAN_true;
    if (value == "no") return BOOLEAN_false;

    return BOOLEAN_NONE;
}

data_DURATION MusicXmlInput::ConvertTypeToDur(const std::string &value)
{
    static const std::map<std::string, data_DURATION> Type2Dur{
        { "maxima", DURATION_maxima }, // this is a mensural MEI value
        { "long", DURATION_long }, // mensural MEI value longa isn't supported
        { "breve", DURATION_breve }, //
        { "whole", DURATION_1 }, //
        { "half", DURATION_2 }, //
        { "quarter", DURATION_4 }, //
        { "eighth", DURATION_8 }, //
        { "16th", DURATION_16 }, //
        { "32nd", DURATION_32 }, //
        { "64th", DURATION_64 }, //
        { "128th", DURATION_128 }, //
        { "256th", DURATION_256 }, //
        { "512th", DURATION_512 }, //
        { "1024th", DURATION_1024 } //
    };

    const auto result = Type2Dur.find(value);
    if (result != Type2Dur.end()) {
        return result->second;
    }

    LogWarning("MusicXML import: Unsupported note-type-value '%s'", value.c_str());
    return DURATION_NONE;
}

data_TEXTRENDITION MusicXmlInput::ConvertEnclosure(const std::string &value)
{
    static const std::map<std::string, data_TEXTRENDITION> Enclosure2Id{
        { "rectangle", TEXTRENDITION_box }, //
        { "square", TEXTRENDITION_box }, //
        { "oval", TEXTRENDITION_circle }, //
        { "circle", TEXTRENDITION_circle }, //
        { "triangle", TEXTRENDITION_tbox }, //
        { "diamond", TEXTRENDITION_dbox }, //
        { "none", TEXTRENDITION_none } //
    };

    const auto result = Enclosure2Id.find(value);
    if (result != Enclosure2Id.end()) {
        return result->second;
    }

    return TEXTRENDITION_NONE;
}

std::u32string MusicXmlInput::ConvertTypeToVerovioText(const std::string &value)
{
    static const std::map<std::string, std::u32string> Type2VerovioText{
        { "long", U"\xE1D0" }, // there is no matching glyph in this SMuFL range
        { "breve", U"\xE1D1" }, //
        { "whole", U"\xE1D2" }, //
        { "half", U"\xE1D3" }, //
        { "quarter", U"\xE1D5" }, //
        { "eighth", U"\xE1D7" }, //
        { "16th", U"\xE1D9" }, //
        { "32nd", U"\xE1DB" }, //
        { "64th", U"\xE1DD" }, //
        { "128th", U"\xE1DF" }, //
        { "256th", U"\xE1E1" }, //
        { "512th", U"\xE1E3" }, //
        { "1024th", U"\xE1E5" } //
    };

    const auto result = Type2VerovioText.find(value);
    if (result != Type2VerovioText.end()) {
        return result->second;
    }

    LogWarning("MusicXML import: Unsupported type '%s'", value.c_str());
    return std::u32string();
}

data_HEADSHAPE MusicXmlInput::ConvertNotehead(const std::string &value)
{
    static const std::map<std::string, data_HEADSHAPE> Notehead2Id{
        { "slash", HEADSHAPE_slash }, //
        { "triangle", HEADSHAPE_rtriangle }, //
        { "diamond", HEADSHAPE_diamond }, //
        { "square", HEADSHAPE_square }, //
        { "cross", HEADSHAPE_plus }, //
        { "x", HEADSHAPE_x }, //
        { "circle-x", HEADSHAPE_slash }, //
        { "inverted triangle", HEADSHAPE_slash }, //
        { "arrow down", HEADSHAPE_slash }, //
        { "arrow up", HEADSHAPE_slash }, //
        { "circle dot", HEADSHAPE_circle } //
    };

    const auto result = Notehead2Id.find(value);
    if (result != Notehead2Id.end()) {
        return result->second;
    }

    return HEADSHAPE_NONE;
}

data_LINESTARTENDSYMBOL MusicXmlInput::ConvertLineEndSymbol(const std::string &value)
{
    static const std::map<std::string, data_LINESTARTENDSYMBOL> LineEndSymbol2Id{
        { "up", LINESTARTENDSYMBOL_angleup }, //
        { "down", LINESTARTENDSYMBOL_angledown }, //
        { "arrow", LINESTARTENDSYMBOL_arrow }, //
        { "Hauptstimme", LINESTARTENDSYMBOL_H }, //
        { "Nebenstimme", LINESTARTENDSYMBOL_N }, //
        { "none", LINESTARTENDSYMBOL_none }, //
        { "plain", LINESTARTENDSYMBOL_NONE } //
    };

    const auto result = LineEndSymbol2Id.find(value);
    if (result != LineEndSymbol2Id.end()) {
        return result->second;
    }

    return LINESTARTENDSYMBOL_NONE;
}

data_MIDIVALUE MusicXmlInput::ConvertDynamicsToMidiVal(const float dynamics)
{
    if (dynamics > 0.0) {
        int mididynam = int(dynamics * 90.0 / 100.0 + 0.5);
        return std::max(1, std::min(127, mididynam));
    }
    return 0;
}

data_PITCHNAME MusicXmlInput::ConvertStepToPitchName(const std::string &value)
{
    static const std::map<std::string, data_PITCHNAME> Step2PitchName{
        { "C", PITCHNAME_c }, //
        { "D", PITCHNAME_d }, //
        { "E", PITCHNAME_e }, //
        { "F", PITCHNAME_f }, //
        { "G", PITCHNAME_g }, //
        { "A", PITCHNAME_a }, //
        { "B", PITCHNAME_b } //
    };

    const auto result = Step2PitchName.find(value);
    if (result != Step2PitchName.end()) {
        return result->second;
    }

    LogWarning("MusicXML import: Unsupported step value '%s'", value.c_str());
    return PITCHNAME_NONE;
}

curvature_CURVEDIR MusicXmlInput::InferCurvedir(const pugi::xml_node slurOrTie)
{
    const std::string orientation = slurOrTie.attribute("orientation").as_string();
    if (orientation == "over") return curvature_CURVEDIR_above;
    if (orientation == "under") return curvature_CURVEDIR_below;

    const std::string placement = slurOrTie.attribute("placement").as_string();
    if (placement == "above") return curvature_CURVEDIR_above;
    if (placement == "below") return curvature_CURVEDIR_below;

    return curvature_CURVEDIR_NONE;
}

fermataVis_SHAPE MusicXmlInput::ConvertFermataShape(const std::string &value)
{
    static const std::map<std::string, fermataVis_SHAPE> FermataShape2Id{
        { "normal", fermataVis_SHAPE_curved }, //
        { "angled", fermataVis_SHAPE_angular }, //
        { "square", fermataVis_SHAPE_square }, //
        { "double-angled", fermataVis_SHAPE_angular }, //
        { "double-square", fermataVis_SHAPE_square } //
    };

    const auto result = FermataShape2Id.find(value);
    if (result != FermataShape2Id.end()) {
        return result->second;
    }

    return fermataVis_SHAPE_NONE;
}

pedalLog_DIR MusicXmlInput::ConvertPedalTypeToDir(const std::string &value)
{
    static const std::map<std::string, pedalLog_DIR> PedalType2Dir{
        { "start", pedalLog_DIR_down }, //
        { "stop", pedalLog_DIR_up }, //
        { "sostenuto", pedalLog_DIR_down }, //
        { "change", pedalLog_DIR_bounce } //
    };

    const auto result = PedalType2Dir.find(value);
    if (result != PedalType2Dir.end()) {
        return result->second;
    }

    LogWarning("MusicXML import: Unsupported type '%s' for pedal", value.c_str());
    return pedalLog_DIR_NONE;
}

tupletVis_NUMFORMAT MusicXmlInput::ConvertTupletNumberValue(const std::string &value)
{
    if (value == "actual") return tupletVis_NUMFORMAT_count;
    if (value == "both") return tupletVis_NUMFORMAT_ratio;
    return tupletVis_NUMFORMAT_NONE;
}

std::string MusicXmlInput::ConvertAlterToSymbol(const std::string &value, bool plusMinus)
{
    static const std::map<std::string, std::string> Alter2Symbol{
        { "-2", "𝄫" }, //
        { "-1", "♭" }, //
        { "0", "♮" }, //
        { "1", "♯" }, //
        { "2", "𝄪" } //
    };

    static const std::map<std::string, std::string> Alter2PlusMinus{
        { "-2", "--" }, //
        { "-1", "-" }, //
        { "0", "" }, //
        { "1", "+" }, //
        { "2", "++" } //
    };

    if (plusMinus) {
        const auto result = Alter2PlusMinus.find(value);
        if (result != Alter2PlusMinus.end()) {
            return result->second;
        }
    }
    else {
        const auto result = Alter2Symbol.find(value);
        if (result != Alter2Symbol.end()) {
            return result->second;
        }
    }

    return std::string();
}

std::string MusicXmlInput::ConvertKindToSymbol(const std::string &value)
{
    static const std::map<std::string, std::string> Kind2Symbol{
        { "major", "" }, // Use no symbol to avoid ambiguity of "C△".
        { "minor", "-" }, //
        { "augmented", "+" }, //
        { "diminished", "°" }, //
        { "dominant", "7" }, //
        { "major-seventh", "△7" }, //
        { "minor-seventh", "-7" }, //
        { "diminished-seventh", "°7" }, //
        { "augmented-seventh", "+7" }, //
        { "half-diminished", "ø" }, //
        { "major-minor", "-△7" }, //
        { "major-sixth", "6" }, //
        { "minor-sixth", "-6" }, //
        { "dominant-ninth", "9" }, //
        { "major-ninth", "△9" }, //
        { "minor-ninth", "-9" }, //
        { "dominant-11th", "11" }, //
        { "major-11th", "△11" }, //
        { "minor-11th", "-11" }, //
        { "dominant-13th", "13" }, //
        { "major-13th", "△13" }, //
        { "minor-13th", "-13" }, //
        { "suspended-second", "sus2" }, //
        { "suspended-fourth", "sus4" }, //
        // Skipping "functional sixths": Neapolitan, Italian, French, German.
        // Skipping pedal (pedal-point bass)
        { "power", "5" } //
        // Skipping Tristan
    };

    const auto result = Kind2Symbol.find(value);
    if (result != Kind2Symbol.end()) {
        return result->second;
    }

    return std::string();
}

std::string MusicXmlInput::ConvertKindToText(const std::string &value)
{
    static const std::map<std::string, std::string> Kind2Text{
        { "major", "" }, //
        { "minor", "m" }, //
        { "augmented", "aug" }, //
        { "diminished", "dim" }, //
        { "dominant", "7" }, //
        { "major-seventh", "Maj7" }, //
        { "minor-seventh", "m7" }, //
        { "diminished-seventh", "dim7" }, //
        { "augmented-seventh", "aug7" }, //
        { "half-diminished", "m7♭5" }, //
        { "major-minor", "mMaj7" }, //
        { "major-sixth", "6" }, //
        { "minor-sixth", "m6" }, //
        { "dominant-ninth", "9" }, //
        { "major-ninth", "Maj9" }, //
        { "minor-ninth", "m9" }, //
        { "dominant-11th", "11" }, //
        { "major-11th", "Maj11" }, //
        { "minor-11th", "m11" }, //
        { "dominant-13th", "13" }, //
        { "major-13th", "Maj13" }, //
        { "minor-13th", "m13" }, //
        { "suspended-second", "sus2" }, //
        { "suspended-fourth", "sus4" }, //
        // Skipping "functional sixths": Neapolitan, Italian, French, German.
        // Skipping pedal (pedal-point bass)
        { "power", "5" } //
        // Skipping Tristan
    };

    const auto result = Kind2Text.find(value);
    if (result != Kind2Text.end()) {
        return result->second;
    }

    return std::string();
}

std::string MusicXmlInput::ConvertDegreeToText(pugi::xml_node harmony)
{
    // Maps <kind> values to the first interval that can get an "add" prefix
    static const std::map<std::string, int> Kind2FirstAddable{
        { "major", 9 }, //
        { "minor", 9 }, //
        { "augmented", 9 }, //
        { "diminished", 9 }, //
        { "dominant", 11 }, //
        { "major-seventh", 11 }, //
        { "minor-seventh", 11 }, //
        { "diminished-seventh", 11 }, //
        { "augmented-seventh", 11 }, //
        { "half-diminished", 11 }, //
        { "major-minor", 11 }, //
        { "major-sixth", 11 }, //
        { "minor-sixth", 11 },

        // Skipping "dominant-ninth", "major-ninth" and "minor-ninth". An
        // additional 13 would not get an "add", implying to omit the 11, as the
        // 11 is regularly omitted anyway. Compare:
        //   https://music.stackexchange.com/questions/3732/

        // Skipping "dominant-11th", "major-11th" and "minor-11th".
        // 13 would no longer get an "add".

        // Skipping "dominant-13th", "major-13th" and "minor-13th". Nothing to
        // add anyway.

        { "suspended-second", 11 }, //
        { "suspended-fourth", 9 }, //

        // Skipping "functional sixths": Neapolitan, Italian, French, German.
        // Skipping pedal (pedal-point bass)

        { "power", 7 } //

        // Skipping Tristan
    };

    std::string degreeText = "";

    for (pugi::xml_node degree : harmony.children("degree")) {
        if (degreeText == "") {
            degreeText = "(";
        }

        pugi::xml_node typeNode = degree.child("degree-type");
        const std::string type = typeNode.text().as_string();
        pugi::xml_node valueNode = degree.child("degree-value");
        if (!valueNode) {
            // <degree-value> is required. Signal something is missing.
            degreeText += "?";
            continue;
        }
        const std::string degreeValue = valueNode.text().as_string();

        if (typeNode.attribute("text")) {
            degreeText += typeNode.attribute("text").as_string();
        }
        else {
            if (type == "subtract") {
                degreeText += "no";
            }
            else if (type == "add") {
                const std::string kind = harmony.child("kind").text().as_string();
                const auto result = Kind2FirstAddable.find(kind);

                if (result != Kind2FirstAddable.end()) {
                    int firstAddable = result->second;
                    if (std::stoi(degreeValue) >= firstAddable) {
                        degreeText += "add";
                    }
                }
            }
        }

        pugi::xml_node alterNode = degree.child("degree-alter");
        const std::string alter = alterNode.text().as_string();
        // degree-alter value of 0 is not rendered as natural, it's omitted.
        // (<degree-alter> is a required element, so assume it's there.)
        if (alter != "0") {
            const std::string plusMinus = alterNode.attribute("plus-minus").as_string();
            degreeText += ConvertAlterToSymbol(alter, plusMinus == "yes");
        }
        degreeText += degreeValue;
    }

    if (degreeText != "") {
        degreeText += ")";
    }

    return degreeText;
}

std::string MusicXmlInput::ConvertFigureGlyph(const std::string &value)
{
    static const std::map<std::string, std::string> FigureGlyphMap{
        { "sharp", "♯" }, //
        { "flat", "♭" }, //
        { "natural", "♮" }, //
        { "double-sharp", "𝄪" }, //
        { "flat-flat", "𝄫" }, //
        { "sharp-sharp", "♯♯" }, //
        { "backslash", "\xE2\x83\xA5" }, // utf-8 byte encoding for non-spacing backslash U+20E5
        { "slash", "\xCC\xB8" }, // utf-8 byte encoding for non-spacing slash U+0338
        { "cross", "+" } //
    };

    const auto result = FigureGlyphMap.find(value);
    if (result != FigureGlyphMap.end()) {
        return result->second;
    }

    return std::string();
}

bool MusicXmlInput::NotInEndingStack(const Measure *measure) const
{
    for (const auto &endingItem : m_endingStack) {
        for (Measure *endingMeasure : endingItem.first) {
            if (endingMeasure->GetID() == measure->GetID()) return false;
        }
    }
    return true;
}

void MusicXmlInput::SetFermataExternalSymbols(Fermata *fermata, const std::string &shape)
{
    // When MEI adds support for all of these shapes, this can be merged with ConvertFermataShape()
    static const std::map<std::string, std::string> fermataExtSymbolsAbove = { { "double-angled", "U+E4C2" },
        { "double-square", "U+E4C8" }, { "double-dot", "U+E4CA" }, { "half-curve", "U+E4CC" }, { "curlew", "U+E4D6" } };
    static const std::map<std::string, std::string> fermataExtSymbolsBelow = { { "double-angled", "U+E4C3" },
        { "double-square", "U+E4C9" }, { "double-dot", "U+E4CB" }, { "half-curve", "U+E4CD" }, { "curlew", "U+E4D6" } };

    if (const auto result = fermataExtSymbolsBelow.find(shape);
        (fermata->GetForm() == fermataVis_FORM_inv) && (result != fermataExtSymbolsBelow.end())) {
        AttModule::SetExternalsymbols(fermata, "glyph.num", result->second);
        AttModule::SetExternalsymbols(fermata, "glyph.auth", "smufl");
    }
    else if (const auto result = fermataExtSymbolsAbove.find(shape); result != fermataExtSymbolsAbove.end()) {
        AttModule::SetExternalsymbols(fermata, "glyph.num", result->second);
        AttModule::SetExternalsymbols(fermata, "glyph.auth", "smufl");
    }
}

void MusicXmlInput::ShapeFermata(Fermata *fermata, pugi::xml_node node)
{
    assert(fermata);

    // color
    fermata->SetColor(node.attribute("color").as_string());
    // shape
    fermata->SetShape(ConvertFermataShape(node.text().as_string()));
    // form and place
    if (HasAttributeWithValue(node, "type", "inverted")) {
        fermata->SetForm(fermataVis_FORM_inv);
        fermata->SetPlace(STAFFREL_below);
    }
    else if (HasAttributeWithValue(node, "type", "upright")) {
        fermata->SetForm(fermataVis_FORM_norm);
        fermata->SetPlace(STAFFREL_above);
    }
    SetFermataExternalSymbols(fermata, node.text().as_string());
}

bool MusicXmlInput::IsMultirestMeasure(int index) const
{
    for (const auto &multiRest : m_multiRests) {
        if (index <= multiRest.first) return false;
        if (index <= multiRest.second) return true;
    }
    return false;
}

std::pair<std::vector<int>, int> MusicXmlInput::GetMeterSigGrpValues(const pugi::xml_node &node, MeterSigGrp *parent)
{
    pugi::xpath_node_set beats = node.select_nodes("beats");
    pugi::xpath_node_set beat_type = node.select_nodes("beat-type");
    int maxUnit = 0;
    std::vector<int> meterCounts;
    for (auto iter1 = beats.begin(), iter2 = beat_type.begin(); (iter1 != beats.end()) && (iter2 != beat_type.end());
         ++iter1, ++iter2) {
        // Process current beat/beat-type combination and add it to the meterSigGrp
        MeterSig *meterSig = new MeterSig();
        data_METERCOUNT_pair count = meterSig->AttMeterSigLog::StrToMetercountPair(iter1->node().text().as_string());
        meterSig->SetCount(count);
        int currentUnit = iter2->node().text().as_int();
        meterSig->SetUnit(currentUnit);
        parent->AddChild(meterSig);
        std::vector<int> currentCount;
        std::tie(currentCount, std::ignore) = meterSig->GetCount();
        // Process meterCount and meterUnit based on current/previous beats
        if (maxUnit == 0) maxUnit = currentUnit;
        if (maxUnit == currentUnit) {
            meterCounts.insert(meterCounts.end(), currentCount.begin(), currentCount.end());
        }
        else if (maxUnit > currentUnit) {
            int ratio = maxUnit / currentUnit;
            std::transform(currentCount.begin(), currentCount.end(), currentCount.begin(),
                [&ratio](int elem) -> int { return elem * ratio; });
            meterCounts.insert(meterCounts.end(), currentCount.begin(), currentCount.end());
        }
        else if (maxUnit < currentUnit) {
            int ratio = currentUnit / maxUnit;
            std::transform(meterCounts.begin(), meterCounts.end(), meterCounts.begin(),
                [&ratio](int elem) -> int { return elem * ratio; });
            meterCounts.insert(meterCounts.end(), currentCount.begin(), currentCount.end());
            maxUnit = currentUnit;
        }
    }

    return { meterCounts, maxUnit };
}

int MusicXmlInput::GetMrestMeasuresCountBeforeIndex(int index) const
{
    int count = 0;
    for (const auto &multiRest : m_multiRests) {
        if (index <= multiRest.first) break;
        count += multiRest.second - multiRest.first;
    }
    return count;
}

std::string MusicXmlInput::GetOrnamentGlyphNumber(int attributes) const
{
    static std::map<int, std::string> precomposedNames = {
        { APPR_Above | FORM_Inverted, "U+E5C6" }, { APPR_Below | FORM_Inverted, "U+E5B5" },
        { APPR_Above | FORM_Normal, "U+E5C7" }, { APPR_Below | FORM_Normal, "U+E5B8" },
        { FORM_Inverted | DEP_Above, "U+E5BB" }, { FORM_Inverted | DEP_Below, "U+E5C8" }
        // these values need to be matched with proper SMuFL codes first
        /*, { FORM_Normal | DEP_Above, "U+????" },
        { FORM_Normal | DEP_Below, "U+????" }, { APPR_Above | FORM_Normal | DEP_Above, "U+????" },
        { APPR_Above | FORM_Normal | DEP_Above, "U+????" }, { APPR_Above | FORM_Normal | DEP_Below, "U+????" },
        { APPR_Below | FORM_Normal | DEP_Above, "U+????" }, { APPR_Below | FORM_Normal | DEP_Below, "U+????" },
        { APPR_Above | FORM_Inverted | DEP_Above, "U+????" }, { APPR_Above | FORM_Inverted | DEP_Below, "U+????" },
        { APPR_Below | FORM_Inverted | DEP_Above, "U+????" }, { APPR_Below | FORM_Inverted | DEP_Below, "U+????" }*/
    };

    return precomposedNames.end() != precomposedNames.find(attributes) ? precomposedNames[attributes] : "";
}

void MusicXmlInput::SetChordStaff(Layer *layer)
{
    // if all notes in the chord have @staff attribute set one for the chord as well
    Chord *chord = vrv_cast<Chord *>(m_elementStackMap.at(layer).back());
    if (!chord) return;

    const ArrayOfObjects &children = chord->GetChildren();
    auto it = find_if(children.begin(), children.end(), [](Object *object) {
        if (!object->Is(NOTE)) return false;
        Note *note = vrv_cast<Note *>(object);
        return !note->HasStaff();
    });
    if (it != chord->GetChildren().end()) return;

    // if all notes have @staff attribute, but it's not the same staff at least of one note - leave it as is
    const auto chordStaff = vrv_cast<Note *>(chord->GetFirst(NOTE))->GetStaff();
    it = find_if(children.begin(), children.end(), [&chordStaff](Object *object) {
        if (!object->Is(NOTE)) return false;
        Note *note = vrv_cast<Note *>(object);
        return (chordStaff != note->GetStaff());
    });
    if (it != chord->GetChildren().end()) return;

    // Now that we're sure that cross-staff is the same for all notes, we can set it to chord and clear of notes
    chord->SetStaff(chordStaff);
    for_each(children.begin(), children.end(), [](Object *object) {
        if (!object->Is(NOTE)) return;
        Note *note = vrv_cast<Note *>(object);
        note->ResetStaffIdent();
    });
}

int MusicXmlInput::PitchToMidi(const std::string &step, int alter, int octave)
{
    if (step.empty() || step[0] < 'A' || step[0] > 'G') return 0;

    // Distance in semitones from the octave's starting C to the given step
    //                                 A   B  C  D  E  F  G
    static const int octaveStart[] = { 9, 11, 0, 2, 4, 5, 7 };
    const int semitones = octave * 12 + octaveStart[step[0] - 'A'] + alter; // semitones from C0
    return semitones + 12; // MIDI note C4 = 60
}

void MusicXmlInput::MidiToPitch(int midi, std::string &step, int &alter, int &octave)
{
    const int semitones = midi - 12; // C0 = 0

    // 12 notes in an octave.  Ignore enharmonics, prefer B flat over A sharp.
    static const std::pair<const char *, int> octaveNotes[] = { { "C", 0 }, { "C", 1 }, { "D", 0 }, { "D", 1 },
        { "E", 0 }, { "F", 0 }, { "F", 1 }, { "G", 0 }, { "G", 1 }, { "A", 0 }, { "B", -1 }, { "B", 0 } };
    static_assert(sizeof(octaveNotes) / sizeof(octaveNotes[0]) == 12);

    octave = semitones / 12;
    step = octaveNotes[semitones % 12].first;
    alter = octaveNotes[semitones % 12].second;
}

#endif // NO_MUSICXML_SUPPORT

} // namespace vrv
