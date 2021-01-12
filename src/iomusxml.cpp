/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.cpp
// Author:      Laurent Pugin and Klaus Rettinghaus
// Created:     22/09/2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iomusxml.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <regex>
#include <sstream>

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "beam.h"
#include "beatrpt.h"
#include "bracketspan.h"
#include "breath.h"
#include "btrem.h"
#include "chord.h"
#include "clef.h"
#include "comparison.h"
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
#include "mdiv.h"
#include "measure.h"
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
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "trill.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

// Using flags mordent can be easily visualised with the value. E.g.
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

//----------------------------------------------------------------------------
// MusicXmlInput
//----------------------------------------------------------------------------

MusicXmlInput::MusicXmlInput(Doc *doc) : Input(doc) {}

MusicXmlInput::~MusicXmlInput() {}

bool MusicXmlInput::Import(std::string const &musicxml)
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

void MusicXmlInput::AddClef(Section *section, Measure *measure, Staff *staff, const std::string &measureNum)
{
    Layer *layer = m_currentLayer;
    assert(layer);
    for (auto iter = m_ClefChangeStack.begin(); iter != m_ClefChangeStack.end(); ++iter) {
        if ((iter->m_measureNum == measureNum) && (iter->m_scoreOnset <= m_durTotal)) {
            if (iter->isFirst) { // add clef when first in staff
                if (iter->m_staff != staff) {
                    layer = SelectLayer(-1, iter->m_staff);
                }
                // if afterBarline is false at beginning of measure, move before barline
                if (!iter->m_afterBarline && m_durTotal == 0) {
                    AttNNumberLikeComparison comparisonMeasure(MEASURE, measureNum);
                    Object *currentMeasure = section->FindDescendantByComparison(&comparisonMeasure);
                    Object *previousMeasure = section->GetPrevious(currentMeasure, MEASURE);
                    if (!previousMeasure) {
                        AddLayerElement(layer, iter->m_clef);
                        iter->isFirst = false;
                        continue;
                    }
                    AttNIntegerComparison comparisonStaff(STAFF, staff->GetN());
                    Object *previousStaff = previousMeasure->FindDescendantByComparison(&comparisonStaff);
                    AttNIntegerComparison comparisonLayer(LAYER, layer->GetN());
                    Object *prevLayer = previousStaff->FindDescendantByComparison(&comparisonLayer);
                    if (prevLayer == NULL) {
                        AddLayerElement(layer, iter->m_clef);
                    }
                    else {
                        prevLayer->AddChild(iter->m_clef);
                    }
                }
                else {
                    AddLayerElement(layer, iter->m_clef);
                }
                iter->isFirst = false;
            }
            else { // add clef with @sameas attribute, if no other sameas clef or original clef in that layer
                bool addSameas = true;
                ListOfObjects objects;
                ClassIdComparison matchClassId(CLEF);
                layer->FindAllDescendantByComparison(&objects, &matchClassId);
                for (auto o : objects) {
                    Clef *clef = dynamic_cast<Clef *>(o);
                    if (strcmp(clef->GetSameas().c_str(), ("#" + iter->m_clef->GetUuid()).c_str()) == 0) {
                        addSameas = false;
                        break;
                    }
                    if (strcmp(o->GetUuid().c_str(), iter->m_clef->GetUuid().c_str()) == 0) {
                        addSameas = false;
                        break;
                    }
                }
                if (addSameas) {
                    Clef *sameasClef = new Clef(); // add clef with @sameas referring to original clef
                    sameasClef->SetSameas("#" + iter->m_clef->GetUuid());
                    AddLayerElement(layer, sameasClef);
                }
            }
        }
    }
}

void MusicXmlInput::AddMeasure(Section *section, Measure *measure, int i)
{
    assert(section);
    assert(i >= 0);

    // we just need to add a measure
    if (section->GetChildCount(MEASURE) <= i - GetMrestMeasuresCountBeforeIndex(i)) {
        section->AddChild(measure);
    }
    // otherwise copy the content to the corresponding existing measure
    else {
        AttNNumberLikeComparison comparisonMeasure(MEASURE, measure->GetN());
        Measure *existingMeasure = vrv_cast<Measure *>(section->FindDescendantByComparison(&comparisonMeasure, 1));
        assert(existingMeasure);
        for (auto current : *measure->GetChildren()) {
            if (!current->Is(STAFF)) {
                continue;
            }
            Staff *staff = dynamic_cast<Staff *>(measure->Relinquish(current->GetIdx()));
            assert(staff);
            existingMeasure->AddChild(staff);
        }
    }

    // add this measure to `m_endingStack` if within an ending
    if (!m_endingStack.empty()) {
        if (m_endingStack.back().second.m_endingType == "start"
            && m_endingStack.back().first.back()->GetUuid() != measure->GetUuid()) {
            m_endingStack.back().first.push_back(measure);
        }
    }
}

void MusicXmlInput::AddLayerElement(Layer *layer, LayerElement *element, int duration)
{
    assert(layer);
    assert(element);

    int currTime = 0;
    if (m_layerEndTimes.count(layer) > 0) currTime = m_layerEndTimes.at(layer);
    if ((layer->GetChildren()->size() == 0 && m_durTotal > 0) || currTime < m_durTotal) {
        FillSpace(layer, m_durTotal - currTime);
    }

    if (m_elementStackMap.at(layer).empty()) {
        layer->AddChild(element);
    }
    else {
        m_elementStackMap.at(layer).back()->AddChild(element);
    }
    m_layerEndTimes[layer] = m_durTotal + duration;
}

Layer *MusicXmlInput::SelectLayer(pugi::xml_node node, Measure *measure)
{
    // If value is iniatialized - get current layer
    if (m_isLayerInitialized) return m_currentLayer;

    // Find voice number of node
    int layerNum = (node.child("voice")) ? node.child("voice").text().as_int() : 1;
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
    int staffNum = (node.child("staff")) ? node.child("staff").text().as_int() : 1;
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

Layer *MusicXmlInput::SelectLayer(int staffNum, Measure *measure)
{
    staffNum--;
    Staff *staff = vrv_cast<Staff *>(measure->GetChild(staffNum, STAFF));
    assert(staff);
    // layer -1 means the first one
    return SelectLayer(-1, staff);
}

Layer *MusicXmlInput::SelectLayer(int layerNum, Staff *staff)
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
        layer = dynamic_cast<Layer *>(staff->FindDescendantByComparison(&comparisonLayer, 1));
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
        space->SetDur(space->AttDurationLogical::StrToDuration(durStr));
        space->SetDurPpq(m_ppq * quarters);
        if (m_elementStackMap.at(layer).empty())
            layer->AddChild(space);
        else
            m_elementStackMap.at(layer).back()->AddChild(space);
        dur -= m_ppq * quarters;
    }
}

void MusicXmlInput::GenerateUuid(pugi::xml_node node)
{
    int nr = std::rand();
    char str[17];
    // I do not want to use a stream for doing this!
    snprintf(str, 17, "%016d", nr);

    std::string uuid = StringFormat("%s-%s", node.name(), str).c_str();
    std::transform(uuid.begin(), uuid.end(), uuid.begin(), ::tolower);
    node.append_attribute("xml:id").set_value(uuid.c_str());
}

//////////////////////////////////////////////////////////////////////////////
// Tie and slurs stack management

void MusicXmlInput::OpenTie(Note *note, Tie *tie)
{
    tie->SetStartid("#" + note->GetUuid());
    m_tieStack.push_back(std::make_pair(tie, note));
}

void MusicXmlInput::CloseTie(Note *note)
{
    // add all notes with identical pitch/oct to m_tieStopStack
    std::vector<std::pair<Tie *, Note *> >::iterator iter;
    for (iter = m_tieStack.begin(); iter != m_tieStack.end(); ++iter) {
        if (note->GetPname() == iter->second->GetPname() && note->GetOct() == iter->second->GetOct()) {
            m_tieStopStack.push_back(note);
        }
    }
}

void MusicXmlInput::OpenSlur(Measure *measure, int number, Slur *slur)
{
    // try to match open slur with slur stops within that measure
    std::vector<std::pair<LayerElement *, musicxml::CloseSlur> >::iterator iter;
    for (iter = m_slurStopStack.begin(); iter != m_slurStopStack.end(); ++iter) {
        if ((iter->second.m_number == number) && ((iter->second.m_measureNum).compare(measure->GetN()) == 0)) {
            slur->SetStartid(m_ID);
            slur->SetEndid("#" + iter->first->GetUuid());
            m_slurStopStack.erase(iter);
            return;
        }
    }
    // create new slur otherwise
    slur->SetStartid(m_ID);
    musicxml::OpenSlur openSlur(number);
    m_slurStack.push_back(std::make_pair(slur, openSlur));
}

void MusicXmlInput::CloseSlur(Measure *measure, int number, LayerElement *element)
{
    // try to match slur stop to open slurs by slur number
    std::vector<std::pair<Slur *, musicxml::OpenSlur> >::reverse_iterator riter;
    for (riter = m_slurStack.rbegin(); riter != m_slurStack.rend(); ++riter) {
        if (riter->second.m_number == number) {
            riter->first->SetEndid("#" + element->GetUuid());
            m_slurStack.erase(std::next(riter).base());
            return;
        }
    }
    // add to m_slurStopStack, if not able to be closed
    musicxml::CloseSlur closeSlur(measure->GetN(), number);
    m_slurStopStack.push_back(std::make_pair(element, closeSlur));
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
            if (xmlDynamPart.text()) {
                dynamStr += xmlDynamPart.text().as_string();
            }
            else {
                dynamStr += xmlDynamPart.name();
            }
        }
        return dynamStr;
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
        else if (soundNode && !soundNode.attribute("tempo")) {
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
            text->SetText(UTF8to16(line));
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
    if (HasAttributeWithValue(metronome, "parentheses", "yes")) {
        rawText = "(";
        paren = true;
    }
    Text *text;
    if (!rawText.empty()) {
        text = new Text();
        text->SetText(UTF8to16(rawText));
        tempo->AddChild(text);
    }

    int dotCount = (int)metronome.select_nodes("beat-unit-dot").size();
    if (dotCount) {
        tempo->SetMmDots(dotCount);
    }

    pugi::xml_node beatunit = metronome.select_node("beat-unit").node();
    if (beatunit) {
        std::wstring verovioText;
        std::string content = GetContent(beatunit);
        tempo->SetMmUnit(ConvertTypeToDur(content));
        verovioText = ConvertTypeToVerovioText(content);
        for (int i = 0; i < dotCount; i++) {
            verovioText += L"\xE1E7"; // SMUFL augmentation dot
        }
        if (!verovioText.empty()) {
            Rend *rend = new Rend;
            rend->SetFontname("VerovioText");
            text = new Text();
            text->SetText(verovioText);
            rend->AddChild(text);
            tempo->AddChild(rend);
        }
    }

    rawText = "";
    pugi::xml_node perminute = metronome.select_node("per-minute").node();
    if (perminute) {
        std::string mm = GetContent(perminute);
        double mmval = 0.0;
        std::smatch matches;
        // Use the first floating-point number on the line to set @mm:
        if (std::regex_search(mm, matches, std::regex("(\\d+\\.?\\d*)"))) {
            mmval = std::stod(matches[1]);
        }
        if ((!isnan(mmval)) && (mmval > 0.5)) {
            int mmint = int(mmval + 0.5);
            tempo->SetMm(mmint);
        }
        if (!mm.empty()) {
            std::stringstream sstream;
            if (beatunit) {
                sstream << " = ";
            }
            sstream << mm;
            rawText = sstream.str();
        }
    }
    if (paren) {
        rawText += ")";
    }

    if (!rawText.empty()) {
        text = new Text();
        text->SetText(UTF8to16(rawText));
        tempo->AddChild(text);
    }
}

//////////////////////////////////////////////////////////////////////////////
// Parsing methods

bool MusicXmlInput::ReadMusicXml(pugi::xml_node root)
{
    assert(root);

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
    if (root.select_node("/score-partwise/part/measure/print")) {
        m_hasLayoutInformation = true;
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
            text->SetText(UTF8to16(words.node().text().as_string()));
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
            m_doc->m_mdivScoreDef.AddChild(head);
        }
        if (foot) {
            m_doc->m_mdivScoreDef.AddChild(foot);
        }
    }

    std::vector<StaffGrp *> m_staffGrpStack;
    StaffGrp *staffGrp = new StaffGrp();
    m_doc->m_mdivScoreDef.AddChild(staffGrp);
    m_staffGrpStack.push_back(staffGrp);

    int staffOffset = 0;
    m_octDis.push_back(0);

    pugi::xpath_node scoreMidiBpm = root.select_node("/score-partwise/part[1]/measure[1]/sound[@tempo][1]");
    if (scoreMidiBpm) m_doc->m_mdivScoreDef.SetMidiBpm(scoreMidiBpm.node().attribute("tempo").as_int());

    pugi::xpath_node_set partListChildren = root.select_nodes("/score-partwise/part-list/*");
    for (pugi::xpath_node_set::const_iterator it = partListChildren.begin(); it != partListChildren.end(); ++it) {
        pugi::xpath_node xpathNode = *it;
        if (IsElement(xpathNode.node(), "part-group")) {
            if (HasAttributeWithValue(xpathNode.node(), "type", "start")) {
                StaffGrp *staffGrp = new StaffGrp();
                // read the group-symbol (MEI @symbol)
                std::string groupGymbol = GetContentOfChild(xpathNode.node(), "group-symbol");
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
                const std::string groupBarline = GetContentOfChild(xpathNode.node(), "group-barline");
                staffGrp->SetBarThru(ConvertWordToBool(groupBarline));
                // now stack it
                const std::string groupName
                    = GetContentOfChild(xpathNode.node(), "group-name[not(@print-object='no')]");
                const std::string groupAbbr
                    = GetContentOfChild(xpathNode.node(), "group-abbreviation[not(@print-object='no')]");
                if (!groupName.empty() && !m_label) {
                    m_label = new Label();
                    if (xpathNode.node().select_node("group-name-display[not(@print-object='no')]")) {
                        const std::string name = StyleLabel(xpathNode.node().select_node("group-name-display").node());
                        Text *text = new Text();
                        text->SetText(UTF8to16(name));
                        m_label->AddChild(text);
                    }
                    else {
                        Text *text = new Text();
                        text->SetText(UTF8to16(groupName));
                        m_label->AddChild(text);
                        staffGrp->AddChild(m_label);
                        m_label = NULL;
                    }
                }
                if (!groupAbbr.empty() && !m_labelAbbr) {
                    m_labelAbbr = new LabelAbbr();
                    if (xpathNode.node().select_node("group-abbreviation-display[not(@print-object='no')]")) {
                        const std::string name
                            = StyleLabel(xpathNode.node().select_node("group-abbreviation-display").node());
                        Text *text = new Text();
                        text->SetText(UTF8to16(name));
                        m_labelAbbr->AddChild(text);
                    }
                    else {
                        Text *text = new Text();
                        text->SetText(UTF8to16(groupAbbr));
                        m_labelAbbr->AddChild(text);
                        staffGrp->AddChild(m_labelAbbr);
                        m_labelAbbr = NULL;
                    }
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
            if (!partFirstMeasure.node().select_node("attributes")) {
                LogWarning("MusicXML import: Could not find the 'attributes' element in the first "
                           "measure of part '%s'",
                    partId.c_str());
                continue;
            }
            // part-name should be revised, as soon MEI can suppress labels
            std::string partName = GetContentOfChild(xpathNode.node(), "part-name[not(@print-object='no')]");
            std::string partAbbr = GetContentOfChild(xpathNode.node(), "part-abbreviation[not(@print-object='no')]");
            pugi::xpath_node midiInstrument = xpathNode.node().select_node("midi-instrument");
            if (!partName.empty() && !m_label) {
                m_label = new Label();
                if (xpathNode.node().select_node("part-name-display[not(@print-object='no')]")) {
                    std::string name = StyleLabel(xpathNode.node().select_node("part-name-display").node());
                    Text *text = new Text();
                    text->SetText(UTF8to16(name));
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
                        text->SetText(UTF8to16(line));
                        m_label->AddChild(text);
                        firstLine = false;
                    }
                }
            }
            if (!partAbbr.empty() && !m_labelAbbr) {
                m_labelAbbr = new LabelAbbr();
                if (xpathNode.node().select_node("part-abbreviation-display[not(@print-object='no')]")) {
                    std::string name = StyleLabel(xpathNode.node().select_node("part-abbreviation-display").node());
                    Text *text = new Text();
                    text->SetText(UTF8to16(name));
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
                        text->SetText(UTF8to16(line));
                        m_labelAbbr->AddChild(text);
                        firstLine = false;
                    }
                }
            }
            if (midiInstrument && !m_instrdef) {
                m_instrdef = new InstrDef;
                m_instrdef->SetMidiInstrname(m_instrdef->AttMidiInstrument::StrToMidinames(
                    midiInstrument.node().select_node("midi-name").node().text().as_string()));
                pugi::xpath_node midiChannel = midiInstrument.node().select_node("midi-channel");
                if (midiChannel) m_instrdef->SetMidiChannel(midiChannel.node().text().as_int() - 1);
                // pugi::xpath_node midiPan = midiInstrument.node().select_node("pan");
                // if (midiPan) instrdef->SetMidiPan(midiPan.node().text().as_int());
                pugi::xpath_node midiProgram = midiInstrument.node().select_node("midi-program");
                if (midiProgram) m_instrdef->SetMidiInstrnum(midiProgram.node().text().as_int() - 1);
                pugi::xpath_node midiVolume = midiInstrument.node().select_node("volume");
                if (midiVolume) m_instrdef->SetMidiVolume(midiVolume.node().text().as_int());
            }
            // create the staffDef(s)
            StaffGrp *partStaffGrp = new StaffGrp();
            partStaffGrp->SetUuid(partId.c_str());
            const int nbStaves
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

    Measure *measure = NULL;
    std::vector<std::pair<std::string, ControlElement *> >::iterator iter;
    for (iter = m_controlElements.begin(); iter != m_controlElements.end(); ++iter) {
        if (!measure || (measure->GetN() != iter->first)) {
            AttNNumberLikeComparison comparisonMeasure(MEASURE, iter->first);
            measure = dynamic_cast<Measure *>(section->FindDescendantByComparison(&comparisonMeasure, 1));
        }
        if (!measure) {
            LogWarning("MusicXML import: Element '%s' could not be added to measure '%s'",
                iter->second->GetClassName().c_str(), iter->first.c_str());
            delete iter->second;
            continue;
        }
        measure->AddChild(iter->second);
    }

    // manage endings stack: create new <ending> elements and move the corresponding measures into them
    if (!m_endingStack.empty()) {
        std::vector<std::pair<std::vector<Measure *>, musicxml::EndingInfo> >::iterator iter;
        for (iter = m_endingStack.begin(); iter != m_endingStack.end(); ++iter) {
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
                logString = logString + (*jter)->GetUuid().c_str();
                // remove other measures from <section> that are not already removed above (first measure)
                if ((*jter)->GetUuid() != measureList.front()->GetUuid()) {
                    int idx = section->GetChildIndex(*jter);
                    section->DetachChild(idx);
                }
                ending->AddChild(*jter); // add <measure> to <ending>
                logString = logString + ((*jter == measureList.back()) ? ")." : ", ");
            }
            LogMessage(logString.c_str());
        }
        m_endingStack.clear();
    }

    m_doc->ConvertToPageBasedDoc();

    // clean up stacks
    if (!m_hairpinStack.empty() || !m_hairpinStopStack.empty()) {
        LogWarning(
            "MusicXML import: There are %d hairpins left open", m_hairpinStack.size() + m_hairpinStopStack.size());
        m_hairpinStack.clear();
        m_hairpinStopStack.clear();
    }
    if (!m_tieStack.empty()) {
        LogWarning("MusicXML import: There are %d ties left open", m_tieStack.size());
        m_tieStack.clear();
    }
    if (!m_slurStack.empty()) { // There are slurs left open
        std::vector<std::pair<Slur *, musicxml::OpenSlur> >::iterator iter;
        for (iter = m_slurStack.begin(); iter != m_slurStack.end(); ++iter) {
            LogWarning("MusicXML import: slur '%s' could not be ended", iter->first->GetUuid().c_str());
        }
        m_slurStack.clear();
    }
    if (!m_slurStopStack.empty()) { // There are slurs ends without opening
        std::vector<std::pair<LayerElement *, musicxml::CloseSlur> >::iterator iter;
        for (iter = m_slurStopStack.begin(); iter != m_slurStopStack.end(); ++iter) {
            LogWarning("MusicXML import: slur ending for element '%s' could not be "
                       "matched to a start element",
                iter->first->GetUuid().c_str());
        }
        m_slurStopStack.clear();
    }
    if (!m_openDashesStack.empty()) { // open dashes without ending
        std::vector<std::pair<ControlElement *, musicxml::OpenDashes> >::iterator iter;
        for (iter = m_openDashesStack.begin(); iter != m_openDashesStack.end(); ++iter) {
            LogWarning(
                "MusicXML import: dashes/extender lines for '%s' could not be closed", iter->first->GetUuid().c_str());
        }
        m_openDashesStack.clear();
    }
    if (!m_bracketStack.empty()) { // open brackets without ending
        std::vector<std::pair<BracketSpan *, musicxml::OpenSpanner> >::iterator iter;
        for (iter = m_bracketStack.begin(); iter != m_bracketStack.end(); ++iter) {
            LogWarning("MusicXML import: bracketSpan for '%s' could not be closed", iter->first->GetUuid().c_str());
        }
        m_bracketStack.clear();
    }
    if (!m_glissStack.empty()) {
        std::vector<Gliss *>::iterator iter;
        for (iter = m_glissStack.begin(); iter != m_glissStack.end(); ++iter) {
            LogWarning("MusicXML import: gliss for '%s' could not be closed", (*iter)->GetUuid().c_str());
        }
        m_glissStack.clear();
    }
    if (!m_trillStack.empty()) { // open trills without ending
        std::vector<std::pair<Trill *, musicxml::OpenSpanner> >::iterator iter;
        for (iter = m_trillStack.begin(); iter != m_trillStack.end(); ++iter) {
            LogWarning("MusicXML import: trill extender for '%s' could not be ended", iter->first->GetUuid().c_str());
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
    pubStmt.append_child(pugi::node_pcdata);

    pugi::xml_node respStmt = titleStmt.append_child("respStmt");

    pugi::xpath_node_set creators = root.select_nodes("/score-partwise/identification/creator");
    for (pugi::xpath_node_set::const_iterator it = creators.begin(); it != creators.end(); ++it) {
        pugi::xpath_node creator = *it;
        pugi::xml_node persName = respStmt.append_child("persName");
        persName.text().set(creator.node().text().as_string());
        persName.append_attribute("role").set_value(creator.node().attribute("type").as_string());
    }

    // Convert rights into availability
    pugi::xml_node availability = pubStmt.append_child("availability");

    pugi::xpath_node_set rightses = root.select_nodes("/score-partwise/identification/rights");
    for (pugi::xpath_node_set::const_iterator it = rightses.begin(); it != rightses.end(); ++it) {
        pugi::xpath_node rights = *it;
        availability.append_child("distributor")
            .append_child(pugi::node_pcdata)
            .set_value(rights.node().text().as_string());
    }

    pugi::xml_node encodingDesc = meiHead.append_child("encodingDesc");
    GenerateUuid(encodingDesc);
    pugi::xml_node appInfo = encodingDesc.append_child("appInfo");
    GenerateUuid(appInfo);
    pugi::xml_node app = appInfo.append_child("application");
    GenerateUuid(app);
    pugi::xml_node appName = app.append_child("name");
    GenerateUuid(appName);
    appName.append_child(pugi::node_pcdata).set_value("Verovio");
    pugi::xml_node appText = app.append_child("p");
    GenerateUuid(appText);
    appText.append_child(pugi::node_pcdata).set_value("Transcoded from MusicXML");

    // isodate and version
    time_t t = time(0); // get time now
    struct tm *now = localtime(&t);
    std::string dateStr = StringFormat("%d-%02d-%02dT%02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1,
        now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    app.append_attribute("isodate").set_value(dateStr.c_str());
    app.append_attribute("version").set_value(GetVersion().c_str());
}

int MusicXmlInput::ReadMusicXmlPartAttributesAsStaffDef(pugi::xml_node node, StaffGrp *staffGrp, int staffOffset)
{
    assert(node);
    assert(staffGrp);

    // First get the number of staves in the part
    int nbStaves = 1;
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
        if (IsElement(*it, "attributes")) it->set_name("mei-read");

        std::string xpath;
        // Create as many staffDef
        for (int i = 0; i < nbStaves; ++i) {
            // Find or create the staffDef
            AttNIntegerComparison comparisonStaffDef(STAFFDEF, i + 1 + staffOffset);
            StaffDef *staffDef = dynamic_cast<StaffDef *>(staffGrp->FindDescendantByComparison(&comparisonStaffDef, 1));
            if (!staffDef) {
                staffDef = new StaffDef();
                staffDef->SetN(i + 1 + staffOffset);
                if (nbStaves == 1) {
                    staffDef->SetUuid(staffGrp->GetUuid());
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
            Clef *clef = NULL;
            pugi::xpath_node clefSign;
            xpath = StringFormat("clef[@number='%d']/sign", i + 1);
            clefSign = it->select_node(xpath.c_str());
            // if not, look at a common one
            if (!clefSign) {
                clefSign = it->select_node("clef/sign");
            }
            if (clefSign.node().text()) {
                if (!clef) clef = new Clef();
                clef->SetShape(clef->AttClefShape::StrToClefshape(GetContent(clefSign.node()).substr(0, 4)));
            }
            // clef line
            pugi::xpath_node clefLine;
            xpath = StringFormat("clef[@number='%d']/line", i + 1);
            clefLine = it->select_node(xpath.c_str());
            if (!clefLine) {
                clefLine = it->select_node("clef/line[not(@number)]");
            }
            if (clefLine.node().text()) {
                if (!clef) clef = new Clef();
                if (clef->GetShape() != CLEFSHAPE_perc) {
                    clef->SetLine(clefLine.node().text().as_int());
                }
            }
            else if (clef) {
                switch (clef->GetShape()) {
                    case CLEFSHAPE_C: clef->SetLine(3); break;
                    case CLEFSHAPE_F: clef->SetLine(4); break;
                    case CLEFSHAPE_G: clef->SetLine(2); break;
                    case CLEFSHAPE_TAB: clef->SetLine(5); break;
                    default: break;
                }
            }
            // clef octave change
            pugi::xpath_node clefOctaveChange;
            xpath = StringFormat("clef[@number='%d']/clef-octave-change", i + 1);
            clefOctaveChange = it->select_node(xpath.c_str());
            if (!clefOctaveChange) {
                clefOctaveChange = it->select_node("clef/clef-octave-change");
            }
            if (clefOctaveChange.node().text()) {
                int change = clefOctaveChange.node().text().as_int();
                if (!clef) clef = new Clef();
                if (abs(change) == 1)
                    clef->SetDis(OCTAVE_DIS_8);
                else if (abs(change) == 2)
                    clef->SetDis(OCTAVE_DIS_15);
                else if (abs(change) == 3)
                    clef->SetDis(OCTAVE_DIS_22);
                if (change < 0)
                    clef->SetDisPlace(STAFFREL_basic_below);
                else if (change > 0)
                    clef->SetDisPlace(STAFFREL_basic_above);
            }
            // add it if necessary
            if (clef) {
                // Make it an attribute for now
                staffDef->AddChild(clef);
            }

            // key sig
            KeySig *keySig = NULL;
            xpath = StringFormat("key[@number='%d']", i + 1);
            pugi::xpath_node key = it->select_node(xpath.c_str());
            if (!key) {
                key = it->select_node("key");
            }
            if (key) {
                if (!keySig) keySig = new KeySig();
                if (key.node().select_node("fifths")) {
                    int fifths = atoi(key.node().select_node("fifths").node().text().as_string());
                    std::string keySigStr;
                    if (fifths < 0)
                        keySigStr = StringFormat("%df", abs(fifths));
                    else if (fifths > 0)
                        keySigStr = StringFormat("%ds", fifths);
                    else
                        keySigStr = "0";
                    keySig->SetSig(keySig->AttKeySigLog::StrToKeysignature(keySigStr));
                }
                else if (key.node().child("key-step")) {
                    for (pugi::xml_node keyStep : key.node().children("key-step")) {
                        KeyAccid *keyAccid = new KeyAccid();
                        keyAccid->SetPname(ConvertStepToPitchName(keyStep.text().as_string()));
                        if (std::strncmp(keyStep.next_sibling().name(), "key-alter", 9) == 0) {
                            data_ACCIDENTAL_GESTURAL accidValue
                                = ConvertAlterToAccid(std::atof(keyStep.next_sibling().text().as_string()));
                            keyAccid->SetAccid(AreaPosInterface::AccidentalGesturalToWritten(accidValue));
                            if (std::strncmp(keyStep.next_sibling().next_sibling().name(), "key-accidental", 14) == 0) {
                                keyAccid->SetAccid(
                                    ConvertAccidentalToAccid(keyStep.next_sibling().next_sibling().text().as_string()));
                            }
                        }
                        keySig->AddChild(keyAccid);
                    }
                }
                if (key.node().select_node("mode")) {
                    keySig->SetMode(
                        keySig->AttKeySigLog::StrToMode(key.node().select_node("mode").node().text().as_string()));
                }
            }
            // add it if necessary
            if (keySig) {
                staffDef->AddChild(keySig);
            }

            // staff details
            pugi::xpath_node staffDetails;
            xpath = StringFormat("staff-details[@number='%d']", i + 1);
            staffDetails = it->select_node(xpath.c_str());
            if (!staffDetails) {
                staffDetails = it->select_node("staff-details");
            }
            int staffLines = staffDetails.node().select_node("staff-lines").node().text().as_int();
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
            pugi::xpath_node staffTuning = staffDetails.node().select_node("staff-tuning");
            if (staffTuning) {
                staffDef->SetNotationtype(NOTATIONTYPE_tab);
            }

            // time
            MeterSig *meterSig = NULL;
            pugi::xpath_node time;
            xpath = StringFormat("time[@number='%d']", i + 1);
            time = it->select_node(xpath.c_str());
            if (!time) {
                time = it->select_node("time");
            }
            if (time) {
                if (!meterSig) meterSig = new MeterSig();
                std::string symbol = time.node().attribute("symbol").as_string();
                if (!symbol.empty()) {
                    if (symbol == "cut" || symbol == "common")
                        meterSig->SetSym(meterSig->AttMeterSigVis::StrToMetersign(symbol.c_str()));
                    else if (symbol == "single-number")
                        meterSig->SetForm(METERFORM_num);
                    else
                        meterSig->SetForm(METERFORM_norm);
                }
                if (time.node().select_node("senza-misura")) {
                    meterSig->SetForm(METERFORM_invis);
                }
                if (time.node().select_nodes("beats").size() > 1) {
                    LogWarning("MusicXML import: Compound meter signatures are not supported");
                }
                pugi::xpath_node beats = time.node().select_node("beats");
                if (beats.node().text()) {
                    m_meterCount = beats.node().text().as_int();
                    // staffDef->AttMeterSigDefaultLog::StrToInt(beats.node().text().as_string());
                    // this is a little "hack", until libMEI is fixed
                    std::string compound = beats.node().text().as_string();
                    if (compound.find("+") != std::string::npos) {
                        m_meterCount += atoi(compound.substr(compound.find("+")).c_str());
                        LogWarning("MusicXML import: Compound time is not supported");
                    }
                    meterSig->SetCount(m_meterCount);
                }
                pugi::xpath_node beatType = time.node().select_node("beat-type");
                if (beatType.node().text()) {
                    m_meterUnit = beatType.node().text().as_int();
                    meterSig->SetUnit(m_meterUnit);
                }
            }
            // add it if necessary
            if (meterSig) {
                // Make it an attribute for now
                staffDef->AddChild(meterSig);
            }

            // transpose
            pugi::xpath_node transpose;
            xpath = StringFormat("transpose[@number='%d']", i + 1);
            transpose = it->select_node(xpath.c_str());
            if (!transpose) {
                transpose = it->select_node("transpose");
            }
            if (transpose) {
                staffDef->SetTransDiat(atoi(GetContentOfChild(transpose.node(), "diatonic").c_str()));
                staffDef->SetTransSemi(atoi(GetContentOfChild(transpose.node(), "chromatic").c_str()));
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
                if (HasAttributeWithValue(measureSlash.node(), "type", "start"))
                    m_slash = true;
                else
                    m_slash = false;
            }
        }
    }

    return nbStaves;
}

bool MusicXmlInput::ReadMusicXmlPart(pugi::xml_node node, Section *section, int nbStaves, int staffOffset)
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
        i++;
    }

    // clean up stack of clefs, since we don't need them to persist between parts
    if (!m_ClefChangeStack.empty()) {
        for (musicxml::ClefChange iter : m_ClefChangeStack) {
            if (iter.isFirst)
                LogWarning("MusicXML import: Clef change at measure %s, staff %d, time %d not inserted",
                    iter.m_measureNum.c_str(), iter.m_staff->GetN(), iter.m_scoreOnset);
        }
        m_ClefChangeStack.clear();
    }

    return false;
}

bool MusicXmlInput::ReadMusicXmlMeasure(
    pugi::xml_node node, Section *section, Measure *measure, int nbStaves, int staffOffset, int index)
{
    assert(node);
    assert(measure);

    const std::string measureNum = node.attribute("number").as_string();
    if (node.attribute("id")) measure->SetUuid(node.attribute("id").as_string());
    if (measure != NULL) measure->SetN(measureNum);

    bool implicit = node.attribute("implicit").as_bool();
    if (implicit) {
        MNum *mNum = new MNum();
        // An empty mNum means that we like to render this measure number as blank.
        measure->AddChild(mNum);
    }

    int i = 0;
    for (i = 0; i < nbStaves; i++) {
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
            const int multiRestLength = it->select_node(".//multiple-rest").node().text().as_int();
            MultiRest *multiRest = new MultiRest;
            if (it->select_node(".//multiple-rest[@use-symbols='yes']")) multiRest->SetBlock(BOOLEAN_false);
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

    // match open ties with close ties
    std::vector<std::pair<Tie *, Note *> >::iterator iter = m_tieStack.begin();
    while (iter != m_tieStack.end()) {
        double lastScoreTimeOnset = 9999; // __DBL_MAX__;
        bool tieMatched = false;
        std::vector<Note *>::iterator jter;
        for (jter = m_tieStopStack.begin(); jter != m_tieStopStack.end(); ++jter) {
            // match tie stop with pitch/oct identity, with start note earlier than end note,
            // and with earliest end note.
            if (iter->second->GetPname() == (*jter)->GetPname() && iter->second->GetOct() == (*jter)->GetOct()
                && (iter->second->GetScoreTimeOnset() < (*jter)->GetScoreTimeOnset()
                    && (*jter)->GetScoreTimeOnset() < lastScoreTimeOnset)) {
                iter->first->SetEndid("#" + (*jter)->GetUuid());
                lastScoreTimeOnset = (*jter)->GetScoreTimeOnset();
                tieMatched = true;
            }
        }
        if (tieMatched) {
            iter = m_tieStack.erase(iter);
        }
        else {
            iter->second->SetScoreTimeOnset(-1); // make scoreTimeOnset small for next measure
            ++iter;
        }
    }
    if (!m_tieStopStack.empty()) { // clear m_tieStopStack after each measure
        m_tieStopStack.clear();
    }

    for (auto staff : *measure->GetChildren()) {
        if (!staff->Is(STAFF)) {
            continue;
        }
        assert(staff);
        if (staff->GetChildCount() == 0) { // add a default layer, if staff completely empty at the end of a measure.
            Layer *emptyLayer = new Layer();
            emptyLayer->AddChild(new MSpace());
            staff->AddChild(emptyLayer);
        }
        // add clef changes that might occur just before a bar line and remove inserted clefs from stack
        if (!m_ClefChangeStack.empty()) {
            for (auto layer : *staff->GetChildren()) {
                assert(layer);
                std::vector<musicxml::ClefChange>::iterator iter;
                for (iter = m_ClefChangeStack.begin(); iter != m_ClefChangeStack.end(); ++iter) {
                    if (iter->m_measureNum == measureNum && iter->m_staff == staff
                        && iter->m_scoreOnset == m_durTotal) {
                        if (iter->isFirst) { // add clef when first in staff
                            layer->AddChild(iter->m_clef);
                            iter->isFirst = false;
                        }
                        else {
                            Clef *sameasClef = new Clef(); // add clef with @sameas referring to original clef
                            sameasClef->SetSameas("#" + iter->m_clef->GetUuid());
                            layer->AddChild(sameasClef);
                        }
                    }
                }
            }
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

void MusicXmlInput::ReadMusicXmlAttributes(
    pugi::xml_node node, Section *section, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(section);
    assert(measure);

    // read clef changes as MEI clef and add them to the stack
    pugi::xpath_node clef = node.select_node("clef");
    if (clef) {
        // check if we have a staff number
        int staffNum = clef.node().attribute("number").as_int();
        staffNum = (staffNum < 1) ? 1 : staffNum;
        Staff *staff = dynamic_cast<Staff *>(measure->GetChild(staffNum - 1, STAFF));
        assert(staff);
        pugi::xpath_node clefSign = clef.node().select_node("sign");
        pugi::xpath_node clefLine = clef.node().select_node("line");
        if (clefSign && clefLine) {
            Clef *meiClef = new Clef();
            meiClef->SetShape(meiClef->AttClefShape::StrToClefshape(GetContent(clefSign.node()).substr(0, 4)));
            meiClef->SetLine(meiClef->AttClefShape::StrToInt(clefLine.node().text().as_string()));
            // clef octave change
            pugi::xpath_node clefOctaveChange = clef.node().select_node("clef-octave-change");
            if (clefOctaveChange.node().text()) {
                const int change = clefOctaveChange.node().text().as_int();
                if (abs(change) == 1)
                    meiClef->SetDis(OCTAVE_DIS_8);
                else if (abs(change) == 2)
                    meiClef->SetDis(OCTAVE_DIS_15);
                else if (abs(change) == 3)
                    meiClef->SetDis(OCTAVE_DIS_22);
                if (change < 0)
                    meiClef->SetDisPlace(STAFFREL_basic_below);
                else
                    meiClef->SetDisPlace(STAFFREL_basic_above);
            }
            const bool afterBarline = clef.node().attribute("after-barline").as_bool();
            m_ClefChangeStack.push_back(musicxml::ClefChange(measureNum, staff, meiClef, m_durTotal, afterBarline));
        }
    }

    // key and time change
    pugi::xpath_node key = node.select_node("key");
    pugi::xpath_node time = node.select_node("time");
    // for now only read first key change in first part and update scoreDef
    if ((key || time) && node.select_node("ancestor::part[not(preceding-sibling::part)]")
        && !node.select_node("preceding-sibling::attributes/key")) {
        ScoreDef *scoreDef = new ScoreDef();
        KeySig *keySig = NULL;
        if (key.node().select_node("fifths")) {
            if (!keySig) keySig = new KeySig();
            const int fifths = key.node().select_node("fifths").node().text().as_int();
            std::string keySigStr;
            if (fifths < 0)
                keySigStr = StringFormat("%df", abs(fifths));
            else if (fifths > 0)
                keySigStr = StringFormat("%ds", fifths);
            else
                keySigStr = "0";
            keySig->SetSig(keySig->AttKeySigLog::StrToKeysignature(keySigStr));
        }
        else if (key.node().child("key-step")) {
            if (!keySig) keySig = new KeySig();
            for (pugi::xml_node keyStep = key.node().child("key-step"); keyStep;
                 keyStep = keyStep.next_sibling("key-step")) {
                KeyAccid *keyAccid = new KeyAccid();
                keyAccid->SetPname(ConvertStepToPitchName(keyStep.text().as_string()));
                if (std::strncmp(keyStep.next_sibling().name(), "key-alter", 9) == 0) {
                    data_ACCIDENTAL_GESTURAL accidValue
                        = ConvertAlterToAccid(std::atof(keyStep.next_sibling().text().as_string()));
                    keyAccid->SetAccid(AreaPosInterface::AccidentalGesturalToWritten(accidValue));
                    if (std::strncmp(keyStep.next_sibling().next_sibling().name(), "key-accidental", 14) == 0) {
                        keyAccid->SetAccid(
                            ConvertAccidentalToAccid(keyStep.next_sibling().next_sibling().text().as_string()));
                    }
                }
                keySig->AddChild(keyAccid);
            }
        }
        if (key.node().select_node("mode")) {
            if (!keySig) keySig = new KeySig();
            keySig->SetMode(keySig->AttKeySigLog::StrToMode(key.node().select_node("mode").node().text().as_string()));
        }
        if (key.node().select_node("cancel")) {
            if (!keySig) keySig = new KeySig();
            keySig->SetSigShowchange(BOOLEAN_true);
        }
        if (key.node().attribute("id")) {
            if (!keySig) keySig = new KeySig();
            keySig->SetUuid(key.node().attribute("id").as_string());
        }
        // Add it if necessary
        if (keySig) {
            // Make it an attribute for now
            scoreDef->AddChild(keySig);
        }

        if (time) {
            MeterSig *meterSig = NULL;
            std::string symbol = time.node().attribute("symbol").as_string();
            if (!symbol.empty()) {
                if (!meterSig) meterSig = new MeterSig();
                if (symbol == "cut" || symbol == "common")
                    meterSig->SetSym(meterSig->AttMeterSigVis::StrToMetersign(symbol.c_str()));
                else if (symbol == "single-number")
                    meterSig->SetForm(METERFORM_num);
                else
                    meterSig->SetForm(METERFORM_norm);
            }
            if (time.node().select_nodes("beats").size() > 1) {
                LogWarning("MusicXML import: Compound meter signatures are not supported");
            }
            pugi::xpath_node beats = time.node().select_node("beats");
            if (beats.node().text()) {
                if (!meterSig) meterSig = new MeterSig();
                m_meterCount = beats.node().text().as_int();
                // staffDef->AttMeterSigDefaultLog::StrToInt(beats.node().text().as_string());
                // this is a little "hack", until libMEI is fixed
                std::string compound = beats.node().text().as_string();
                if (compound.find("+") != std::string::npos) {
                    m_meterCount += atoi(compound.substr(compound.find("+")).c_str());
                    LogWarning("MusicXML import: Compound time is not supported");
                }
                meterSig->SetCount(m_meterCount);
            }
            pugi::xpath_node beatType = time.node().select_node("beat-type");
            if (beatType.node().text()) {
                if (!meterSig) meterSig = new MeterSig();
                m_meterUnit = beatType.node().text().as_int();
                meterSig->SetUnit(m_meterUnit);
            }
            // add it if necessary
            if (meterSig) {
                // Make it an attribute for now
                meterSig->IsAttribute(true);
                scoreDef->AddChild(meterSig);
            }
        }

        section->AddChild(scoreDef);
    }

    pugi::xpath_node measureRepeat = node.select_node("measure-style/measure-repeat");
    pugi::xpath_node measureSlash = node.select_node("measure-style/slash");
    if (measureRepeat) {
        if (HasAttributeWithValue(measureRepeat.node(), "type", "start"))
            m_mRpt = true;
        else
            m_mRpt = false;
    }
    if (measureSlash) {
        if (HasAttributeWithValue(measureSlash.node(), "type", "start"))
            m_slash = true;
        else
            m_slash = false;
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

    std::string barStyle = GetContentOfChild(node, "bar-style");
    pugi::xpath_node repeat = node.select_node("repeat");
    if (!barStyle.empty()) {
        data_BARRENDITION barRendition = ConvertStyleToRend(barStyle, repeat);
        if (HasAttributeWithValue(node, "location", "left")) {
            measure->SetLeft(barRendition);
        }
        else if (HasAttributeWithValue(node, "location", "middle")) {
            LogWarning("MusicXML import: Unsupported barline location 'middle' in %s.", measure->GetN().c_str());
        }
        else {
            measure->SetRight(barRendition);
            if (barStyle == "short" || barStyle == "tick") {
                measure->SetBarLen(4);
                if (barStyle == "short")
                    measure->SetBarPlace(2);
                else
                    // bar.place counts in note order (high values are vertically higher).
                    measure->SetBarPlace(6);
            }
        }
    }
    if (barStyle.empty() && repeat) { // add repeat information, also when bar-style is not provided
        if (HasAttributeWithValue(node, "location", "left")) {
            measure->SetLeft(BARRENDITION_rptstart);
        }
        else if (HasAttributeWithValue(node, "location", "middle")) {
            LogWarning("MusicXML import: Unsupported barline location 'middle' in %s.", measure->GetN().c_str());
        }
        else {
            measure->SetRight(BARRENDITION_rptend);
        }
    }

    // parse endings (prima volta, seconda volta...)
    pugi::xpath_node ending = node.select_node("ending");
    if (ending) {
        std::string endingNumber = ending.node().attribute("number").as_string();
        std::string endingType = ending.node().attribute("type").as_string();
        std::string endingText = ending.node().text().as_string();
        // LogMessage("ending number/type/text: %s/%s/%s.", endingNumber.c_str(), endingType.c_str(),
        // endingText.c_str());
        if (endingType == "start") {
            if (m_endingStack.empty() || NotInEndingStack(measure->GetN())) {
                musicxml::EndingInfo endingInfo(endingNumber, endingType, endingText);
                std::vector<Measure *> measureList;
                measureList.push_back(measure);
                m_endingStack.push_back(std::make_pair(measureList, endingInfo));
            }
        }
        else if (endingType == "stop" || endingType == "discontinue") {
            m_endingStack.back().second.m_endingType = endingType;
            if (NotInEndingStack(measure->GetN())) {
                m_endingStack.back().first.push_back(measure);
            }
        }
    }

    // fermatas
    int fermataCounter = 0;
    for (pugi::xml_node xmlFermata : node.children("fermata")) {
        ++fermataCounter;
        Fermata *fermata = new Fermata();
        m_controlElements.push_back(std::make_pair(measureNum, fermata));
        if (HasAttributeWithValue(node, "location", "left")) {
            fermata->SetTstamp(0);
        }
        else if (HasAttributeWithValue(node, "location", "middle")) {
            LogWarning("MusicXML import: Unsupported barline location 'middle'");
        }
        else {
            fermata->SetTstamp((double)(m_durTotal) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
        }
        if (xmlFermata.attribute("id")) fermata->SetUuid(xmlFermata.attribute("id").as_string());

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
    pugi::xml_node node, Measure *measure, const std::string &measureNum, const int staffOffset)
{
    assert(node);
    assert(measure);

    const pugi::xpath_node staffNode = node.select_node("staff");

    const std::string directionId = node.attribute("id").as_string();
    const std::string placeStr = node.attribute("placement").as_string();
    const int offset = node.select_node("offset").node().text().as_int();
    const double timeStamp = (double)(m_durTotal + offset) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0;

    const pugi::xpath_node voice = node.select_node("voice");
    if (voice) m_prevLayer = SelectLayer(node, measure);

    const pugi::xml_node type = node.select_node("direction-type").node();

    // Bracket
    pugi::xpath_node bracket = type.select_node("bracket");
    if (bracket) {
        int voiceNumber = bracket.node().attribute("number").as_int();
        voiceNumber = (voiceNumber < 1) ? 1 : voiceNumber;
        if (HasAttributeWithValue(bracket.node(), "type", "stop")) {
            if (m_bracketStack.empty()) {
                // if this is empty, most likely we're dealing with an extender
            }
            else {
                const int measureDifference
                    = m_measureCounts.at(measure) - m_bracketStack.front().second.m_lastMeasureCount;
                m_bracketStack.front().first->SetLendsym(
                    ConvertLineEndSymbol(bracket.node().attribute("line-end").as_string()));
                if (measureDifference >= 0) {
                    m_bracketStack.front().first->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
                }
                m_bracketStack.erase(m_bracketStack.begin());
            }
        }
        else {
            BracketSpan *bracketSpan = new BracketSpan();
            musicxml::OpenSpanner openBracket(voiceNumber, m_measureCounts.at(measure));
            bracketSpan->SetColor(bracket.node().attribute("color").as_string());
            bracketSpan->SetLform(
                bracketSpan->AttLineRendBase::StrToLineform(bracket.node().attribute("line-type").as_string()));
            // bracketSpan->SetPlace(bracketSpan->AttPlacement::StrToStaffrel(placeStr.c_str()));
            bracketSpan->SetFunc("unclear");
            bracketSpan->SetLstartsym(ConvertLineEndSymbol(bracket.node().attribute("line-end").as_string()));
            bracketSpan->SetTstamp(timeStamp);
            m_controlElements.push_back(std::make_pair(measureNum, bracketSpan));
            m_bracketStack.push_back(std::make_pair(bracketSpan, openBracket));
        }
    }

    // Coda
    pugi::xpath_node coda = type.select_node("coda");
    if (coda) {
        Dir *dir = new Dir();
        dir->SetPlace(dir->AttPlacement::StrToStaffrel(placeStr.c_str()));
        dir->SetTstamp(timeStamp - 1.0);
        dir->SetType("coda");
        dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList("1"));
        if (coda.node().attribute("id")) dir->SetUuid(coda.node().attribute("id").as_string());
        Rend *rend = new Rend;
        rend->SetFontname("VerovioText");
        rend->SetFontstyle(FONTSTYLE_normal);
        rend->SetHalign(HORIZONTALALIGNMENT_center);
        Text *text = new Text();
        std::wstring codaSign = UTF8to16("\xF0\x9D\x84\x8C");
        text->SetText(codaSign);
        rend->AddChild(text);
        dir->AddChild(rend);
        m_controlElements.push_back(std::make_pair(measureNum, dir));
    }

    // Dashes (to be connected with previous <dir> or <dynam> as @extender and @tstamp2 attribute
    pugi::xpath_node dashes = type.select_node("bracket|dashes");
    if (dashes) {
        int dashesNumber = dashes.node().attribute("number").as_int();
        dashesNumber = (dashesNumber < 1) ? 1 : dashesNumber;
        int staffNum = 1;
        pugi::xpath_node staffNode = node.select_node("staff");
        if (staffNode) staffNum = staffNode.node().text().as_int() + staffOffset;
        if (HasAttributeWithValue(dashes.node(), "type", "stop")) {
            std::vector<std::pair<ControlElement *, musicxml::OpenDashes> >::iterator iter = m_openDashesStack.begin();
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
            std::vector<std::pair<std::string, ControlElement *> >::reverse_iterator riter;
            for (riter = m_controlElements.rbegin(); riter != m_controlElements.rend(); ++riter) {
                if (riter->second->Is(DYNAM)) {
                    Dynam *dynam = dynamic_cast<Dynam *>(riter->second);
                    std::vector<int> staffAttr = dynam->GetStaff();
                    if (std::find(staffAttr.begin(), staffAttr.end(), staffNum + staffOffset) != staffAttr.end()
                        && dynam->GetPlace() == dynam->AttPlacement::StrToStaffrel(placeStr.c_str())
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
                        && dir->GetPlace() == dir->AttPlacement::StrToStaffrel(placeStr.c_str())
                        && riter->first == measureNum) {
                        dir->SetExtender(BOOLEAN_true);
                        controlElement = dir;
                        break;
                    }
                }
            }
            if (controlElement != nullptr) {
                musicxml::OpenDashes openDashes(dashesNumber, staffNum, m_measureCounts.at(measure));
                m_openDashesStack.push_back(std::make_pair(controlElement, openDashes));
            }
            else {
                LogMessage("MusicXmlImport: dashes could not be matched to <dir> or <dynam> in measure %s.",
                    measureNum.c_str());
            }
        }
    }

    pugi::xpath_node_set words = node.select_nodes("direction-type/words");
    const bool containsWords = !words.empty();
    bool containsDynamics = !node.select_node("direction-type/dynamics").node().empty();

    // Directive
    int defaultY = 0; // y position attribute, only for directives and dynamics
    if (containsWords && !containsDynamics && !node.select_node("sound[@tempo]")) {
        pugi::xpath_node_set words = node.select_nodes("direction-type/words");
        defaultY = words.first().node().attribute("default-y").as_int();
        std::string wordStr = words.first().node().text().as_string();
        if (wordStr.rfind("cresc", 0) == 0 || wordStr.rfind("dim", 0) == 0 || wordStr.rfind("decresc", 0) == 0) {
            containsDynamics = true;
        }
        else {
            Dir *dir = new Dir();
            if (words.size() == 1) {
                dir->SetLang(words.first().node().attribute("xml:lang").as_string());
            }
            dir->SetPlace(dir->AttPlacement::StrToStaffrel(placeStr.c_str()));
            dir->SetTstamp(timeStamp);
            dir->SetType(node.child("sound").first_attribute().name());
            pugi::xpath_node staffNode = node.select_node("staff");
            if (staffNode) {
                dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(staffNode.node().text().as_int() + staffOffset)));
            }
            else if (m_prevLayer) {
                dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(dynamic_cast<Staff *>(m_prevLayer->GetParent())->GetN())));
            }

            TextRendition(words, dir);
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 200;
            dir->SetVgrp(defaultY);
            m_controlElements.push_back(std::make_pair(measureNum, dir));
            m_dirStack.push_back(dir);

            pugi::xpath_node extender = (words.end() - 1)->parent().next_sibling("direction-type").first_child();
            if (!strcmp(extender.node().name(), "bracket") || !strcmp(extender.node().name(), "dashes")) {
                int extNumber = extender.node().attribute("number").as_int();
                extNumber = (extNumber < 1) ? 1 : extNumber;
                int staffNum = staffNode.node().text().as_int() + staffOffset;
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
                m_openDashesStack.push_back(std::make_pair(dir, openDashes));
            }
        }
    }

    // Dynamics
    if (containsDynamics) {
        pugi::xpath_node_set dynamics = node.select_nodes(
            containsWords ? "direction-type/dynamics|direction-type/words" : "direction-type/dynamics");

        dynamics.sort();

        Dynam *dynam = new Dynam();
        dynam->SetPlace(dynam->AttPlacement::StrToStaffrel(placeStr.c_str()));
        dynam->SetTstamp(timeStamp);
        if (staffNode) {
            dynam->SetStaff(dynam->AttStaffIdent::StrToXsdPositiveIntegerList(
                std::to_string(staffNode.node().text().as_int() + staffOffset)));
        }
        else if (m_prevLayer) {
            dynam->SetStaff(dynam->AttStaffIdent::StrToXsdPositiveIntegerList(
                std::to_string(dynamic_cast<Staff *>(m_prevLayer->GetParent())->GetN())));
        }

        if (node.select_node("sound")) {
            const float dynamics = node.select_node("sound").node().attribute("dynamics").as_float(-1.0);
            if (dynamics >= 0.0) {
                dynam->SetVal(ConvertDynamicsToMidiVal(dynamics));
            }
        }

        TextRendition(dynamics, dynam);
        if (defaultY == 0) defaultY = dynamics.first().node().attribute("default-y").as_int();
        // parse the default_y attribute and transform to vgrp value, to vertically align dynamics and directives
        defaultY = (defaultY <= 0) ? std::abs(defaultY) : defaultY + 200;
        dynam->SetVgrp(defaultY);
        m_controlElements.push_back(std::make_pair(measureNum, dynam));
        m_dynamStack.push_back(dynam);

        pugi::xpath_node extender = (dynamics.end() - 1)->parent().next_sibling("direction-type").first_child();
        if (!strcmp(extender.node().name(), "bracket") || !strcmp(extender.node().name(), "dashes")) {
            int extNumber = extender.node().attribute("number").as_int();
            extNumber = (extNumber < 1) ? 1 : extNumber;
            int staffNum = staffNode.node().text().as_int() + staffOffset;
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
            m_openDashesStack.push_back(std::make_pair(dynam, openDashes));
        }
    }

    // Hairpins
    pugi::xpath_node_set wedges = node.select_nodes("direction-type/wedge");
    for (pugi::xpath_node_set::const_iterator wedge = wedges.begin(); wedge != wedges.end(); ++wedge) {
        int hairpinNumber = wedge->node().attribute("number").as_int();
        hairpinNumber = (hairpinNumber < 1) ? 1 : hairpinNumber;
        if (HasAttributeWithValue(wedge->node(), "type", "stop")) {
            // match wedge type=stop to open hairpin
            std::vector<std::pair<Hairpin *, musicxml::OpenSpanner> >::iterator iter;
            for (iter = m_hairpinStack.begin(); iter != m_hairpinStack.end(); ++iter) {
                if (iter->second.m_dirN == hairpinNumber) {
                    const int measureDifference = m_measureCounts.at(measure) - iter->second.m_lastMeasureCount;
                    if (measureDifference >= 0) {
                        iter->first->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
                    }
                    if (wedge->node().attribute("spread")) {
                        iter->first->SetOpening(wedge->node().attribute("spread").as_double() / 5);
                    }
                    m_hairpinStack.erase(iter);
                    return;
                }
            }
            // ...or push on hairpin stop stack, if not matched.
            m_hairpinStopStack.push_back(std::tuple<int, double, musicxml::OpenSpanner>(
                0, timeStamp, musicxml::OpenSpanner(hairpinNumber, m_measureCounts.at(measure))));
        }
        else {
            Hairpin *hairpin = new Hairpin();
            musicxml::OpenSpanner openHairpin(hairpinNumber, m_measureCounts.at(measure));
            if (HasAttributeWithValue(wedge->node(), "type", "crescendo")) {
                hairpin->SetForm(hairpinLog_FORM_cres);
            }
            else if (HasAttributeWithValue(wedge->node(), "type", "diminuendo")) {
                hairpin->SetForm(hairpinLog_FORM_dim);
            }
            else {
                delete hairpin;
                return;
            }
            // hairpin->SetLform(hairpin->AttLineRendBase::StrToLineform(wedge.node().attribute("line-type").as_string()));
            if (wedge->node().attribute("niente")) {
                hairpin->SetNiente(ConvertWordToBool(wedge->node().attribute("niente").as_string()));
            }
            hairpin->SetColor(wedge->node().attribute("color").as_string());
            hairpin->SetPlace(hairpin->AttPlacement::StrToStaffrel(placeStr.c_str()));
            hairpin->SetTstamp(timeStamp);
            if (wedge->node().attribute("id")) hairpin->SetUuid(wedge->node().attribute("id").as_string());
            int staffNum = node.select_node("staff").node().text().as_int();
            staffNum = (!staffNum && m_prevLayer) ? dynamic_cast<Staff *>(m_prevLayer->GetParent())->GetN() : staffNum;
            if (staffNum != 0) {
                hairpin->SetStaff(
                    hairpin->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(staffNum + staffOffset)));
            }
            int defaultY = wedge->node().attribute("default-y").as_int();
            // parse the default_y attribute and transform to vgrp value, to vertically align hairpins
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 200;
            hairpin->SetVgrp(defaultY);
            // match new hairpin to existing hairpin stop
            std::vector<std::tuple<int, double, musicxml::OpenSpanner> >::iterator iter;
            for (iter = m_hairpinStopStack.begin(); iter != m_hairpinStopStack.end(); ++iter) {
                const int measureDifference = std::get<2>(*iter).m_lastMeasureCount - m_measureCounts.at(measure);
                if (std::get<2>(*iter).m_dirN == hairpinNumber) {
                    if (measureDifference >= 0) {
                        hairpin->SetTstamp2(std::pair<int, double>(measureDifference, std::get<1>(*iter)));
                        m_controlElements.push_back(std::make_pair(measureNum, hairpin));
                    }
                    m_hairpinStopStack.erase(iter);
                    return;
                }
            }
            // ...or push to open hairpin stack.
            m_controlElements.push_back(std::make_pair(measureNum, hairpin));
            m_hairpinStack.push_back(std::make_pair(hairpin, openHairpin));
        }
    }

    // Ottava
    pugi::xpath_node xmlShift = type.select_node("octave-shift");
    if (xmlShift) {
        pugi::xpath_node staffNode = node.select_node("staff");
        const int staffN = (!staffNode) ? 1 : staffNode.node().text().as_int() + staffOffset;
        if (HasAttributeWithValue(xmlShift.node(), "type", "stop")) {
            m_octDis[staffN] = 0;
            std::vector<std::pair<std::string, ControlElement *> >::iterator iter;
            for (iter = m_controlElements.begin(); iter != m_controlElements.end(); ++iter) {
                if (iter->second->Is(OCTAVE)) {
                    Octave *octave = dynamic_cast<Octave *>(iter->second);
                    if (octave->HasEndid()) continue;
                    std::vector<int> staffAttr = octave->GetStaff();
                    if (std::find(staffAttr.begin(), staffAttr.end(), staffN) != staffAttr.end()) {
                        octave->SetEndid(m_ID);
                    }
                    else if (xmlShift.node().attribute("number").as_string() == octave->GetN()) {
                        octave->SetEndid(m_ID);
                    }
                    else {
                        LogWarning("MusicXML import: octave for '%s' could not be closed", octave->GetUuid().c_str());
                    }
                }
            }
        }
        else {
            Octave *octave = new Octave();
            octave->SetColor(xmlShift.node().attribute("color").as_string());
            octave->SetDisPlace(octave->AttOctaveDisplacement::StrToStaffrelBasic(placeStr.c_str()));
            octave->SetN(xmlShift.node().attribute("number").as_string());
            const int octDisNum = xmlShift.node().attribute("size") ? xmlShift.node().attribute("size").as_int() : 8;
            octave->SetDis(octave->AttOctaveDisplacement::StrToOctaveDis(std::to_string(octDisNum)));
            m_octDis[staffN] = (octDisNum + 2) / 8;
            if (HasAttributeWithValue(xmlShift.node(), "type", "up")) {
                octave->SetDisPlace(STAFFREL_basic_below);
                m_octDis[staffN] *= -1;
            }
            else
                octave->SetDisPlace(STAFFREL_basic_above);
            m_controlElements.push_back(std::make_pair(measureNum, octave));
            m_octaveStack.push_back(octave);
        }
    }

    // Pedal
    pugi::xpath_node xmlPedal = type.select_node("pedal");
    if (xmlPedal) {
        std::string pedalType = xmlPedal.node().attribute("type").as_string();
        bool pedalLine = xmlPedal.node().attribute("line").as_bool();
        if (pedalType != "continue") {
            Pedal *pedal = new Pedal();
            pedal->SetColor(xmlPedal.node().attribute("color").as_string());
            // pedal->SetN(xmlPedal.node().attribute("number").as_string());
            if (!placeStr.empty()) pedal->SetPlace(pedal->AttPlacement::StrToStaffrel(placeStr.c_str()));
            pedal->SetDir(ConvertPedalTypeToDir(pedalType));
            if (pedalLine) pedal->SetForm(pedalVis_FORM_line);
            if (xmlPedal.node().attribute("abbreviated")) {
                pedal->SetExternalsymbols(pedal, "glyph.auth", "smufl");
                pedal->SetExternalsymbols(pedal, "glyph.num", "U+E651");
            }
            if (pedalType == "sostenuto") {
                pedal->SetFunc("sostenuto");
                if (xmlPedal.node().attribute("abbreviated")) {
                    pedal->SetExternalsymbols(pedal, "glyph.auth", "smufl");
                    pedal->SetExternalsymbols(pedal, "glyph.num", "U+E65A");
                }
            }
            pugi::xpath_node staffNode = node.select_node("staff");
            if (staffNode) {
                pedal->SetStaff(pedal->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(staffNode.node().text().as_int() + staffOffset)));
            }
            else if (m_prevLayer) {
                pedal->SetStaff(pedal->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(dynamic_cast<Staff *>(m_prevLayer->GetParent())->GetN())));
            }
            pedal->SetTstamp(timeStamp);
            if (pedalType == "stop") pedal->SetTstamp(timeStamp - 0.1);
            int defaultY = xmlPedal.node().attribute("default-y").as_int();
            // parse the default_y attribute and transform to vgrp value, to vertically align pedal starts and stops
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 200;
            pedal->SetVgrp(defaultY);
            m_controlElements.push_back(std::make_pair(measureNum, pedal));
            m_pedalStack.push_back(pedal);
        }
    }

    // Principal voice
    pugi::xpath_node lead = type.select_node("principal-voice");
    if (lead) {
        int voiceNumber = lead.node().attribute("number").as_int();
        voiceNumber = (voiceNumber < 1) ? 1 : voiceNumber;
        if (HasAttributeWithValue(lead.node(), "type", "stop")) {
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
            bracketSpan->SetColor(lead.node().attribute("color").as_string());
            // bracketSpan->SetPlace(bracketSpan->AttPlacement::StrToStaffrel(placeStr.c_str()));
            bracketSpan->SetFunc("analytical");
            bracketSpan->SetLstartsym(ConvertLineEndSymbol(lead.node().attribute("symbol").as_string()));
            bracketSpan->SetTstamp(timeStamp);
            bracketSpan->SetType("principal-voice");
            m_controlElements.push_back(std::make_pair(measureNum, bracketSpan));
            m_bracketStack.push_back(std::make_pair(bracketSpan, openBracket));
        }
    }

    // Rehearsal
    pugi::xpath_node rehearsal = type.select_node("rehearsal");
    if (rehearsal) {
        Reh *reh = new Reh();
        reh->SetPlace(reh->AttPlacement::StrToStaffrel(placeStr.c_str()));
        std::string halign = rehearsal.node().attribute("halign").as_string();
        std::string lang = rehearsal.node().attribute("xml:lang").as_string();
        if (lang.empty()) lang = "it";
        std::string textStr = GetContent(rehearsal.node());
        reh->SetColor(rehearsal.node().attribute("color").as_string());
        reh->SetTstamp(timeStamp);
        pugi::xpath_node staffNode = node.select_node("staff");
        int staffNum = staffNode.node().text().as_int() + staffOffset;
        staffNum = (staffNum < 1) ? 1 : staffNum;
        reh->SetStaff(reh->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(staffNum)));
        reh->SetLang(lang);
        Rend *rend = new Rend();
        rend->SetFontweight(
            rend->AttTypography::StrToFontweight(rehearsal.node().attribute("font-weight").as_string()));
        rend->SetHalign(rend->AttHorizontalAlign::StrToHorizontalalignment(halign));
        rend->SetRend(ConvertEnclosure(rehearsal.node().attribute("enclosure").as_string()));
        Text *text = new Text();
        text->SetText(UTF8to16(textStr));
        rend->AddChild(text);
        reh->AddChild(rend);
        m_controlElements.push_back(std::make_pair(measureNum, reh));
    }

    // Segno
    pugi::xpath_node segno = type.select_node("segno");
    if (segno) {
        Dir *dir = new Dir();
        dir->SetPlace(dir->AttPlacement::StrToStaffrel(placeStr.c_str()));
        dir->SetTstamp(timeStamp - 1.0);
        dir->SetType("segno");
        dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList("1"));
        if (segno.node().attribute("id")) dir->SetUuid(segno.node().attribute("id").as_string());
        Rend *rend = new Rend;
        rend->SetFontname("VerovioText");
        rend->SetFontstyle(FONTSTYLE_normal);
        rend->SetHalign(HORIZONTALALIGNMENT_center);
        Text *text = new Text();
        std::wstring segnoSign = UTF8to16("\xF0\x9D\x84\x8B");
        text->SetText(segnoSign);
        rend->AddChild(text);
        dir->AddChild(rend);
        m_controlElements.push_back(std::make_pair(measureNum, dir));
    }

    // Tempo
    pugi::xpath_node metronome = type.select_node("metronome");
    if (node.select_node("sound[@tempo]") || metronome) {
        Tempo *tempo = new Tempo();
        if (!words.empty()) {
            tempo->SetLang(words.first().node().attribute("xml:lang").as_string());
        }
        tempo->SetPlace(tempo->AttPlacement::StrToStaffrel(placeStr.c_str()));
        if (words.size() != 0) TextRendition(words, tempo);
        if (metronome)
            PrintMetronome(metronome.node(), tempo);
        else {
            tempo->SetMidiBpm(node.select_node("sound").node().attribute("tempo").as_int());
        }
        tempo->SetTstamp(timeStamp);
        pugi::xpath_node staffNode = node.select_node("staff");
        if (staffNode) {
            tempo->SetStaff(tempo->AttStaffIdent::StrToXsdPositiveIntegerList(
                std::to_string(staffNode.node().text().as_int() + staffOffset)));
        }
        m_controlElements.push_back(std::make_pair(measureNum, tempo));
        m_tempoStack.push_back(tempo);
    }

    // other cases
    if (!containsWords && !containsDynamics && !coda && !bracket && !lead && !metronome && !segno && !xmlShift
        && !xmlPedal && wedges.empty() && !dashes && !rehearsal) {
        LogWarning("MusicXML import: Unsupported direction-type '%s'", type.first_child().name());
    }
}

void MusicXmlInput::ReadMusicXmlFigures(pugi::xml_node node, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(measure);

    if (!HasAttributeWithValue(node, "print-object", "no")) {
        Harm *harm = new Harm();
        Fb *fb = new Fb();

        const bool paren = node.attribute("parentheses").as_bool();

        // std::string textColor = node.attribute("color").as_string();
        // std::string textStyle = node.attribute("font-style").as_string();
        // std::string textWeight = node.attribute("font-weight").as_string();
        for (pugi::xml_node figure : node.children("figure")) {
            std::string textStr;
            if (paren) textStr.append("(");
            textStr.append(ConvertFigureGlyph(figure.select_node("prefix").node().text().as_string()));
            textStr.append(figure.select_node("figure-number").node().text().as_string());
            textStr.append(ConvertFigureGlyph(figure.select_node("suffix").node().text().as_string()));
            if (paren) textStr.append(")");
            F *f = new F();
            if (figure.select_node("extend[@type='start']")) f->SetExtender(BOOLEAN_true);
            Text *text = new Text();
            text->SetText(UTF8to16(textStr));
            f->AddChild(text);
            fb->AddChild(f);
        }
        harm->AddChild(fb);
        harm->SetTstamp((double)(m_durTotal + m_durFb) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
        m_durFb += node.select_node("duration").node().text().as_int();
        m_controlElements.push_back(std::make_pair(measureNum, harm));
        m_harmStack.push_back(harm);
    }
}

void MusicXmlInput::ReadMusicXmlForward(pugi::xml_node node, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(measure);

    m_durTotal += atoi(GetContentOfChild(node, "duration").c_str());
}

void MusicXmlInput::ReadMusicXmlHarmony(pugi::xml_node node, Measure *measure, const std::string &measureNum)
{
    assert(node);
    assert(measure);

    int durOffset = 0;

    std::string harmText = GetContentOfChild(node, "root/root-step");
    pugi::xpath_node alter = node.select_node("root/root-alter");
    if (alter) harmText += ConvertAlterToSymbol(GetContent(alter.node()));
    pugi::xpath_node kind = node.select_node("kind");
    if (kind) {
        if (HasAttributeWithValue(kind.node(), "use-symbols", "yes")) {
            harmText = harmText + ConvertKindToSymbol(GetContent(kind.node()));
        }
        else if (kind.node().attribute("text") && std::strcmp(kind.node().text().as_string(), "none")) {
            harmText = harmText + kind.node().attribute("text").as_string();
        }
        else {
            harmText = harmText + ConvertKindToText(GetContent(kind.node()));
        }
    }
    pugi::xpath_node degree = node.select_node("degree");
    if (degree) {
        pugi::xpath_node alter = node.select_node("degree/degree-alter");
        harmText += ConvertAlterToSymbol(GetContent(alter.node())) + GetContentOfChild(node, "degree/degree-value");
    }
    pugi::xpath_node bass = node.select_node("bass");
    if (bass) {
        harmText += "/" + GetContentOfChild(node, "bass/bass-step");
        pugi::xpath_node alter = node.select_node("bass/bass-alter");
        if (alter) harmText += ConvertAlterToSymbol(GetContent(alter.node()));
    }
    Harm *harm = new Harm();
    Text *text = new Text();
    text->SetText(UTF8to16(harmText));
    harm->SetPlace(harm->AttPlacement::StrToStaffrel(node.attribute("placement").as_string()));
    harm->SetType(node.attribute("type").as_string());
    harm->AddChild(text);
    pugi::xpath_node offset = node.select_node("offset");
    if (offset) durOffset = offset.node().text().as_int();
    harm->SetTstamp((double)(m_durTotal + durOffset) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
    m_controlElements.push_back(std::make_pair(measureNum, harm));
    m_harmStack.push_back(harm);
}

void MusicXmlInput::ReadMusicXmlNote(
    pugi::xml_node node, Measure *measure, const std::string &measureNum, const int staffOffset, Section *section)
{
    assert(node);
    assert(measure);

    Layer *layer = SelectLayer(node, measure);
    assert(layer);
    m_prevLayer = layer;

    Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    bool isChord = node.select_node("chord");

    // reset figured bass offset
    m_durFb = 0;

    if (!m_ClefChangeStack.empty() && !isChord) AddClef(section, measure, staff, measureNum);

    LayerElement *element = NULL;

    bool nextIsChord = false;
    double onset = m_durTotal; // keep note onsets for later

    // for measure repeats add a single <mRpt> and return
    if (m_mRpt) {
        MRpt *mRpt = dynamic_cast<MRpt *>((*layer).GetFirst(MRPT));
        if (!mRpt) {
            mRpt = new MRpt();
            AddLayerElement(layer, mRpt);
        }
        return;
    }

    pugi::xpath_node notations = node.select_node("notations[not(@print-object='no')]");

    const bool cue = (node.select_node("cue") || node.select_node("type[@size='cue']")) ? true : false;
    pugi::xml_node grace = node.select_node("grace").node();

    // duration string and dots
    std::string typeStr = GetContentOfChild(node, "type");
    int dots = (int)node.select_nodes("dot").size();

    ReadMusicXmlBeamsAndTuplets(node, layer, isChord);

    // beam start
    bool beamStart = node.select_node("beam[@number='1'][text()='begin']");
    // tremolos
    pugi::xpath_node tremolo = notations.node().select_node("ornaments/tremolo");
    int tremSlashNum = -1;
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
                    bTrem->SetForm(bTremLog_FORM_unmeas);
                    tremSlashNum = 0;
                }
                else {
                    bTrem->SetForm(bTremLog_FORM_meas);
                }
            }
        }
    }

    const std::string noteID = node.attribute("id").as_string();
    const int duration = node.select_node("duration").node().text().as_int();
    const int noteStaffNum = node.select_node("staff").node().text().as_int();
    pugi::xpath_node rest = node.select_node("rest");
    if (rest) {
        std::string stepStr = GetContentOfChild(rest.node(), "display-step");
        std::string octaveStr = GetContentOfChild(rest.node(), "display-octave");
        if (HasAttributeWithValue(node, "print-object", "no")) {
            Space *space = new Space();
            element = space;
            if (!typeStr.empty()) {
                space->SetDur(ConvertTypeToDur(typeStr));
                space->SetDurPpq(duration);
                if (dots > 0) space->SetDots(dots);
                if (!noteID.empty()) {
                    space->SetUuid(noteID);
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
                    mSpace->SetUuid(noteID);
                }
                AddLayerElement(layer, mSpace);
            }
        }
        // we assume /note without /type or with duration of an entire bar to be mRest
        else if (typeStr.empty() || HasAttributeWithValue(rest.node(), "measure", "yes")) {
            if (m_slash) {
                for (int i = m_meterCount; i > 0; --i) {
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
                if (!octaveStr.empty()) mRest->SetOloc(atoi(octaveStr.c_str()));
                if (!noteID.empty()) {
                    mRest->SetUuid(noteID);
                }
                AddLayerElement(layer, mRest, duration);
            }
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
            if (!octaveStr.empty()) rest->SetOloc(atoi(octaveStr.c_str()));
            if (!noteID.empty()) {
                rest->SetUuid(noteID);
            }
            // set @staff attribute, if existing and different from parent staff number
            if (noteStaffNum > 0 && noteStaffNum + staffOffset != staff->GetN())
                rest->SetStaff(
                    rest->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(noteStaffNum + staffOffset)));
            AddLayerElement(layer, rest, duration);
        }
    }
    else {
        Note *note = new Note();
        element = note;
        note->SetVisible(ConvertWordToBool(node.append_attribute("print-object").as_string()));
        note->SetColor(node.attribute("color").as_string());
        if (!noteID.empty()) {
            note->SetUuid(noteID);
        }
        note->SetScoreTimeOnset(onset); // remember the MIDI onset within that measure
        // set @staff attribute, if existing and different from parent staff number
        if (noteStaffNum > 0 && noteStaffNum + staffOffset != staff->GetN())
            note->SetStaff(
                note->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(noteStaffNum + staffOffset)));

        // accidental
        pugi::xpath_node accidental = node.select_node("accidental");
        if (accidental) {
            Accid *accid = new Accid();
            accid->SetAccid(ConvertAccidentalToAccid(accidental.node().text().as_string()));
            accid->SetColor(accidental.node().attribute("color").as_string());
            if (HasAttributeWithValue(accidental.node(), "cautionary", "yes")) accid->SetFunc(accidLog_FUNC_caution);
            if (HasAttributeWithValue(accidental.node(), "editorial", "yes")) accid->SetFunc(accidLog_FUNC_edit);
            if (HasAttributeWithValue(accidental.node(), "bracket", "yes")) accid->SetEnclose(ENCLOSURE_brack);
            if (HasAttributeWithValue(accidental.node(), "parentheses", "yes")) accid->SetEnclose(ENCLOSURE_paren);
            note->AddChild(accid);
        }

        // stem direction - taken into account below for the chord or the note
        data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
        pugi::xpath_node stem = node.select_node("stem");
        std::string stemText = stem.node().text().as_string();
        if (stemText == "down") {
            stemDir = STEMDIRECTION_down;
        }
        else if (stemText == "up") {
            stemDir = STEMDIRECTION_up;
        }

        // pitch and octave
        pugi::xpath_node pitch = node.select_node("pitch");
        if (pitch) {
            const std::string stepStr = GetContentOfChild(pitch.node(), "step");
            const std::string octaveStr = GetContentOfChild(pitch.node(), "octave");
            if (!stepStr.empty()) note->SetPname(ConvertStepToPitchName(stepStr));
            if (!octaveStr.empty()) {
                if (m_octDis[staff->GetN()] != 0) {
                    note->SetOct(atoi(octaveStr.c_str()) - m_octDis[staff->GetN()]);
                    note->SetOctGes(atoi(octaveStr.c_str()));
                }
                else {
                    note->SetOct(atoi(octaveStr.c_str()));
                }
            }
            const std::string alterStr = GetContentOfChild(pitch.node(), "alter");
            if (!alterStr.empty()) {
                Accid *accid = dynamic_cast<Accid *>(note->GetFirst(ACCID));
                if (!accid) {
                    accid = new Accid();
                    note->AddChild(accid);
                    accid->IsAttribute(true);
                }
                accid->SetAccidGes(ConvertAlterToAccid(std::atof(alterStr.c_str())));
            }
        }

        // dynamics (MIDI velocity)
        const float dynamics = node.attribute("dynamics").as_float(-1.0);
        if (dynamics >= 0.0) {
            note->SetVel(ConvertDynamicsToMidiVal(dynamics));
        }

        // notehead
        pugi::xpath_node notehead = node.select_node("notehead");
        if (notehead) {
            note->SetHeadColor(notehead.node().attribute("color").as_string());
            note->SetHeadShape(ConvertNotehead(notehead.node().text().as_string()));
            if (notehead.node().attribute("parentheses").as_bool()) note->SetHeadMod(NOTEHEADMODIFIER_paren);
            auto noteHeadFill = notehead.node().attribute("filled");
            if (noteHeadFill) note->SetHeadFill(noteHeadFill.as_bool() ? FILL_solid : FILL_void);
            if (!std::strncmp(notehead.node().text().as_string(), "none", 4)) note->SetHeadVisible(BOOLEAN_false);
        }

        // look at the next note to see if we are starting or ending a chord
        pugi::xpath_node nextNote = node.select_node("./following-sibling::note");
        if (nextNote.node().select_node("chord")) nextIsChord = true;
        Chord *chord = NULL;
        if (nextIsChord) {
            // create the chord if we are starting a new chord
            if (m_elementStackMap.at(layer).empty() || !m_elementStackMap.at(layer).back()->Is(CHORD)) {
                chord = new Chord();
                chord->SetDur(ConvertTypeToDur(typeStr));
                chord->SetDurPpq(atoi(GetContentOfChild(node, "duration").c_str()));
                if (dots > 0) chord->SetDots(dots);
                chord->SetStemDir(stemDir);
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
        if (nextIsChord || node.select_node("chord")) {
            if (chord == NULL && m_elementStackMap.at(layer).size() > 0
                && m_elementStackMap.at(layer).back()->Is(CHORD)) {
                chord = dynamic_cast<Chord *>(m_elementStackMap.at(layer).back());
            }
            if (!chord) {
                LogError("MusicXML import: Chord starting point has not been found.");
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
        if (m_elementStackMap.at(layer).empty() || !m_elementStackMap.at(layer).back()->Is(CHORD)) {
            note->SetDur(ConvertTypeToDur(typeStr));
            note->SetDurPpq(atoi(GetContentOfChild(node, "duration").c_str()));
            if (dots > 0) note->SetDots(dots);
            note->SetStemDir(stemDir);
            if (node.attribute("default-y") && stem.node().attribute("default-y")) {
                float stemLen
                    = abs(node.attribute("default-y").as_float() - stem.node().attribute("default-y").as_float()) / 5;
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

        // verse / syl
        pugi::xpath_node_set lyrics = node.select_nodes("lyric");
        for (pugi::xpath_node_set::const_iterator it = lyrics.begin(); it != lyrics.end(); ++it) {
            pugi::xml_node lyric = it->node();
            int lyricNumber = lyric.attribute("number").as_int();
            lyricNumber = (lyricNumber < 1) ? 1 : lyricNumber;
            Verse *verse = new Verse();
            verse->SetColor(lyric.attribute("color").as_string());
            // verse->SetPlace(verse->AttPlacement::StrToStaffrelBasic(lyric.attribute("placement").as_string()));
            verse->SetLabel(lyric.attribute("name").as_string());
            verse->SetN(lyricNumber);
            for (pugi::xml_node textNode : lyric.children("text")) {
                if (!HasAttributeWithValue(lyric, "print-object", "no")) {
                    // std::string textColor = textNode.attribute("color").as_string();
                    std::string textStyle = textNode.attribute("font-style").as_string();
                    std::string textWeight = textNode.attribute("font-weight").as_string();
                    std::string lang = textNode.attribute("xml:lang").as_string();
                    std::string textStr = textNode.text().as_string();
                    Syl *syl = new Syl();
                    syl->SetLang(lang.c_str());
                    if (textNode.next_sibling("elision")) {
                        syl->SetCon(sylLog_CON_b);
                    }
                    else if (lyric.select_node("extend")) {
                        syl->SetCon(sylLog_CON_u);
                    }
                    else if (GetContentOfChild(lyric, "syllabic") == "single") {
                        syl->SetCon(sylLog_CON_s);
                    }
                    else if (GetContentOfChild(lyric, "syllabic") == "begin") {
                        syl->SetWordpos(sylLog_WORDPOS_i);
                        syl->SetCon(sylLog_CON_d);
                    }
                    else if (GetContentOfChild(lyric, "syllabic") == "middle") {
                        syl->SetWordpos(sylLog_WORDPOS_m);
                        syl->SetCon(sylLog_CON_d);
                    }
                    else if (GetContentOfChild(lyric, "syllabic") == "end") {
                        syl->SetWordpos(sylLog_WORDPOS_t);
                        // Do not set @con when the syllable has an ellision
                        if (syl->GetCon() != sylLog_CON_b) {
                            syl->SetCon(sylLog_CON_s);
                        }
                    }
                    if (!textStyle.empty()) syl->SetFontstyle(syl->AttTypography::StrToFontstyle(textStyle.c_str()));
                    if (!textWeight.empty())
                        syl->SetFontweight(syl->AttTypography::StrToFontweight(textWeight.c_str()));

                    Text *text = new Text();
                    text->SetText(UTF8to16(textStr));
                    syl->AddChild(text);
                    verse->AddChild(syl);
                }
            }
            if (element->Is(CHORD) || element->Is(NOTE)) {
                element->AddChild(verse);
            }
            else {
                // this should not happen
                delete verse;
            }
        }

        // ties
        pugi::xpath_node startTie = notations.node().select_node("tied[@type='start']");
        pugi::xpath_node endTie = notations.node().select_node("tied[@type='stop']");
        if (endTie) { // add to stack if (endTie) or if pitch/oct match to open tie on m_tieStack
            if (!m_tieStack.empty() && note->GetPname() == m_tieStack.back().second->GetPname()
                && note->GetOct() == m_tieStack.back().second->GetOct()) {
                m_tieStack.back().first->SetEndid("#" + note->GetUuid());
                m_tieStack.pop_back();
            }
            else {
                m_tieStopStack.push_back(note);
            }
        }
        else if (m_tieStack.empty()) {
            CloseTie(note);
        }
        if (startTie) {
            Tie *tie = new Tie();
            // color
            tie->SetColor(startTie.node().attribute("color").as_string());
            // placement and orientation
            tie->SetCurvedir(InferCurvedir(startTie.node()));
            tie->SetLform(tie->AttCurveRend::StrToLineform(startTie.node().attribute("line-type").as_string()));
            if (startTie.node().attribute("id")) tie->SetUuid(startTie.node().attribute("id").as_string());
            // add it to the stack
            m_controlElements.push_back(std::make_pair(measureNum, tie));
            OpenTie(note, tie);
        }

        // articulation
        std::vector<data_ARTICULATION> artics;
        for (pugi::xml_node articulations : notations.node().children("articulations")) {
            if (notations.node().select_node("articulations/*[not(@placement)]")) {
                Artic *artic = new Artic();
                for (pugi::xml_node articulation : articulations.children()) {
                    artics.push_back(ConvertArticulations(articulation.name()));
                    if (!std::strcmp(articulation.name(), "detached-legato")) {
                        artics.push_back(ARTICULATION_stacc);
                        artics.push_back(ARTICULATION_ten);
                    }
                }
                artic->SetArtic(artics);
                element->AddChild(artic);
                artics.clear();
            }
            else {
                std::vector<data_ARTICULATION> articsAbove;
                std::vector<data_ARTICULATION> articsBelow;
                for (pugi::xml_node articulation : articulations.children()) {
                    if (HasAttributeWithValue(articulation, "placement", "above")) {
                        articsAbove.push_back(ConvertArticulations(articulation.name()));
                        if (!std::strcmp(articulation.name(), "detached-legato")) {
                            articsAbove.push_back(ARTICULATION_stacc);
                            articsAbove.push_back(ARTICULATION_ten);
                        }
                    }
                    else {
                        articsBelow.push_back(ConvertArticulations(articulation.name()));
                        if (!std::strcmp(articulation.name(), "detached-legato")) {
                            articsBelow.push_back(ARTICULATION_stacc);
                            articsBelow.push_back(ARTICULATION_ten);
                        }
                    }
                }
                if (!articsAbove.empty()) {
                    Artic *artic = new Artic();
                    artic->SetArtic(articsAbove);
                    artic->SetPlace(STAFFREL_above);
                    element->AddChild(artic);
                }
                if (!articsBelow.empty()) {
                    Artic *artic = new Artic();
                    artic->SetArtic(articsBelow);
                    artic->SetPlace(STAFFREL_below);
                    element->AddChild(artic);
                }
            }
        }

        // technical
        for (pugi::xml_node technical : notations.node().children("technical")) {
            // fingering is handled on the same level as breath marks, dynamics, etc. so we skip it here
            if (technical.child("fingering")) continue;
            if (technical.child("fret")) {
                // set @tab.string and @tab.fret
            }
            else {
                Artic *artic = new Artic();
                for (pugi::xml_node articulation : technical.children()) {
                    artics.push_back(ConvertArticulations(articulation.name()));
                }
                artic->SetArtic(artics);
                artic->SetType("technical");
                element->AddChild(artic);
                artics.clear();
            }
        }

        // add the note to the layer or to the current container
        AddLayerElement(layer, note, duration);

        // if we are ending a chord remove it from the stack
        if (!nextIsChord) {
            if (!m_elementStackMap.at(layer).empty() && m_elementStackMap.at(layer).back()->Is(CHORD)) {
                SetChordStaff(layer);

                RemoveLastFromStack(CHORD, layer);
            }
        }
    }

    // add duration to measure time
    if (!nextIsChord) m_durTotal += atoi(GetContentOfChild(node, "duration").c_str());

    m_ID = "#" + element->GetUuid();

    // breath marks
    pugi::xpath_node xmlBreath = notations.node().select_node("articulations/breath-mark");
    if (xmlBreath) {
        Breath *breath = new Breath();
        m_controlElements.push_back(std::make_pair(measureNum, breath));
        breath->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        breath->SetPlace(breath->AttPlacement::StrToStaffrel(xmlBreath.node().attribute("placement").as_string()));
        breath->SetColor(xmlBreath.node().attribute("color").as_string());
        breath->SetTstamp((double)(m_durTotal) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
    }

    // dynamics
    pugi::xpath_node xmlDynam = notations.node().select_node("dynamics");
    if (xmlDynam) {
        Dynam *dynam = new Dynam();
        m_controlElements.push_back(std::make_pair(measureNum, dynam));
        dynam->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        dynam->SetStartid(m_ID);
        if (xmlDynam.node().attribute("id")) dynam->SetUuid(xmlDynam.node().attribute("id").as_string());
        // place
        dynam->SetPlace(dynam->AttPlacement::StrToStaffrel(xmlDynam.node().attribute("placement").as_string()));
        std::string dynamStr;
        for (pugi::xml_node xmlDynamPart : xmlDynam.node().children()) {
            if (xmlDynamPart.text()) {
                dynamStr += xmlDynamPart.text().as_string();
            }
            else {
                dynamStr += xmlDynamPart.name();
            }
        }
        Text *text = new Text();
        text->SetText(UTF8to16(dynamStr));
        dynam->AddChild(text);
    }

    // fermatas
    pugi::xpath_node xmlFermata = notations.node().select_node("fermata");
    if (xmlFermata) {
        Fermata *fermata = new Fermata();
        m_controlElements.push_back(std::make_pair(measureNum, fermata));
        fermata->SetStartid(m_ID);
        fermata->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        if (xmlFermata.node().attribute("id")) fermata->SetUuid(xmlFermata.node().attribute("id").as_string());
        ShapeFermata(fermata, xmlFermata.node());
    }

    // fingering
    auto xmlFing = notations.node().select_node("technical/fingering");
    if (xmlFing) {
        std::string fingText = GetContent(xmlFing.node());
        Fing *fing = new Fing();
        Text *text = new Text();
        text->SetText(UTF8to16(fingText));
        m_controlElements.push_back(std::make_pair(measureNum, fing));
        fing->SetStartid(m_ID);
        fing->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        fing->SetPlace(fing->AttPlacement::StrToStaffrel(xmlFing.node().attribute("placement").as_string()));
        fing->AddChild(text);
    }

    // glissando and slide
    pugi::xpath_node_set glissandi = notations.node().select_nodes("glissando|slide");
    for (pugi::xpath_node_set::const_iterator it = glissandi.begin(); it != glissandi.end(); ++it) {
        std::string noteID = m_ID;
        // prevent from using chords
        if (element->Is(CHORD)) noteID = "#" + element->GetChild(0)->GetUuid();
        pugi::xml_node xmlGlissando = it->node();
        if (HasAttributeWithValue(xmlGlissando, "type", "start")) {
            Gliss *gliss = new Gliss();
            m_controlElements.push_back(std::make_pair(measureNum, gliss));
            gliss->SetColor(xmlGlissando.attribute("color").as_string());
            gliss->SetLform(gliss->AttLineRendBase::StrToLineform(xmlGlissando.attribute("line-type").as_string()));
            gliss->SetN(xmlGlissando.attribute("number").as_string());
            gliss->SetStartid(noteID);
            gliss->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            gliss->SetType(xmlGlissando.name());
            if (xmlGlissando.attribute("id")) gliss->SetUuid(xmlGlissando.attribute("id").as_string());
            m_glissStack.push_back(gliss);
        }
        else if (!m_glissStack.empty()) {
            int extNumber = xmlGlissando.attribute("number").as_int();
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
        m_controlElements.push_back(std::make_pair(measureNum, mordent));
        mordent->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        mordent->SetStartid(m_ID);
        // color
        mordent->SetColor(xmlMordent.node().attribute("color").as_string());
        // long
        mordent->SetLong(ConvertWordToBool(xmlMordent.node().attribute("long").as_string()));
        // place
        mordent->SetPlace(mordent->AttPlacement::StrToStaffrel(xmlMordent.node().attribute("placement").as_string()));
        // form
        mordent->SetForm(mordentLog_FORM_lower);
        for (pugi::xml_node xmlAccidMark : notations.node().children("accidental-mark")) {
            if (HasAttributeWithValue(xmlAccidMark, "placement", "above")) {
                mordent->SetAccidupper(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
            else if (HasAttributeWithValue(xmlAccidMark, "placement", "below")) {
                mordent->SetAccidlower(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
        }
        if (!std::strncmp(xmlMordent.node().name(), "inverted", 7)) {
            mordent->SetForm(mordentLog_FORM_upper);
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
                mordent->SetExternalsymbols(mordent, "glyph.num", smuflCode);
                mordent->SetExternalsymbols(mordent, "glyph.auth", "smufl");
            }
        }
    }

    // schleifer/haydn (counts as mordent with different glyph)
    pugi::xpath_node xmlExtOrnament
        = notations.node().select_node("ornaments/*[contains(name(), 'schleifer') or contains(name(), 'haydn')]");
    if (xmlExtOrnament) {
        Mordent *mordent = new Mordent();
        m_controlElements.push_back(std::make_pair(measureNum, mordent));
        mordent->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        mordent->SetStartid(m_ID);
        // color
        mordent->SetColor(xmlExtOrnament.node().attribute("color").as_string());
        // place
        mordent->SetPlace(
            mordent->AttPlacement::StrToStaffrel(xmlExtOrnament.node().attribute("placement").as_string()));
        const bool isHaydn = std::string(xmlExtOrnament.node().name()) == "haydn";
        mordent->SetExternalsymbols(mordent, "glyph.num", isHaydn ? "U+E56F" : "U+E587");
        mordent->SetExternalsymbols(mordent, "glyph.auth", "smufl");
    }

    // trill
    pugi::xpath_node xmlTrill = notations.node().select_node("ornaments/trill-mark");
    pugi::xpath_node xmlTrillLine = notations.node().select_node("ornaments/wavy-line[@type='start']");
    if (xmlTrill || xmlTrillLine) {
        Trill *trill = new Trill();
        m_controlElements.push_back(std::make_pair(measureNum, trill));
        trill->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        trill->SetStartid(m_ID);
        // color
        trill->SetColor(xmlTrill.node().attribute("color").as_string());
        // place
        trill->SetPlace(trill->AttPlacement::StrToStaffrel(xmlTrill.node().attribute("placement").as_string()));
        if (xmlTrillLine) {
            trill->SetExtender(BOOLEAN_true);
            trill->SetN(xmlTrillLine.node().attribute("number").as_string());
            if (!xmlTrill) {
                trill->SetLstartsym(LINESTARTENDSYMBOL_none);
                trill->SetColor(xmlTrillLine.node().attribute("color").as_string());
                trill->SetPlace(
                    trill->AttPlacement::StrToStaffrel(xmlTrillLine.node().attribute("placement").as_string()));
            }
            musicxml::OpenSpanner openTrill(1, m_measureCounts.at(measure));
            m_trillStack.push_back(std::make_pair(trill, openTrill));
        }
        for (pugi::xml_node xmlAccidMark : notations.node().children("accidental-mark")) {
            if (HasAttributeWithValue(xmlAccidMark, "placement", "below")) {
                trill->SetAccidlower(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
            else {
                trill->SetAccidupper(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
        }
    }
    if (!m_trillStack.empty() && notations.node().select_node("ornaments/wavy-line[@type='stop']")) {
        int extNumber
            = notations.node().select_node("ornaments/wavy-line[@type='stop']").node().attribute("number").as_int();
        std::vector<std::pair<Trill *, musicxml::OpenSpanner> >::iterator iter = m_trillStack.begin();
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
        m_controlElements.push_back(std::make_pair(measureNum, turn));
        turn->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        turn->SetStartid(m_ID);
        turn->SetColor(xmlTurn.node().attribute("color").as_string());
        turn->SetPlace(turn->AttPlacement::StrToStaffrel(xmlTurn.node().attribute("placement").as_string()));
        turn->SetForm(turnLog_FORM_upper);
        for (pugi::xml_node xmlAccidMark : notations.node().children("accidental-mark")) {
            if (HasAttributeWithValue(xmlAccidMark, "placement", "above")) {
                turn->SetAccidupper(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
            else if (HasAttributeWithValue(xmlAccidMark, "placement", "below")) {
                turn->SetAccidlower(ConvertAccidentalToAccid(xmlAccidMark.text().as_string()));
            }
        }
        if (!std::strncmp(xmlTurn.node().name(), "inverted", 8)) {
            turn->SetForm(turnLog_FORM_lower);
            if (std::string(xmlTurn.node().name()).find("vertical") != std::string::npos) {
                turn->SetType("vertical");
                turn->SetExternalsymbols(turn, "glyph.auth", "smufl");
                turn->SetExternalsymbols(turn, "glyph.num", "U+E56B");
            }
        }
        if (!std::strncmp(xmlTurn.node().name(), "delayed", 7)) {
            turn->SetDelayed(BOOLEAN_true);
        }
        if (!std::strncmp(xmlTurn.node().name(), "vertical", 8)) {
            turn->SetType("vertical");
            turn->SetExternalsymbols(turn, "glyph.auth", "smufl");
            turn->SetExternalsymbols(turn, "glyph.num", "U+E56A");
        }
    }

    // arpeggio
    pugi::xpath_node xmlArpeggiate = notations.node().select_node("*[contains(name(), 'arpeggiate')]");
    if (xmlArpeggiate) {
        int arpegN = xmlArpeggiate.node().attribute("number").as_int();
        arpegN = (arpegN < 1) ? 1 : arpegN;
        const std::string direction = xmlArpeggiate.node().attribute("direction").as_string();
        bool added = false;
        if (!m_ArpeggioStack.empty()) { // check existing arpeggios
            std::vector<std::pair<Arpeg *, musicxml::OpenArpeggio> >::iterator iter;
            for (iter = m_ArpeggioStack.begin(); iter != m_ArpeggioStack.end(); ++iter) {
                if (iter->second.m_arpegN == arpegN && onset == iter->second.m_timeStamp) {
                    // don't add other chord notes, because the chord is already referenced.
                    if (!isChord) iter->first->GetPlistInterface()->AddRef("#" + element->GetUuid());
                    added = true; // so that no new Arpeg gets created below
                    break;
                }
            }
        }
        if (!added) {
            Arpeg *arpeggio = new Arpeg();
            arpeggio->GetPlistInterface()->AddRef("#" + element->GetUuid());
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
            m_ArpeggioStack.push_back(std::make_pair(arpeggio, musicxml::OpenArpeggio(arpegN, onset)));
            m_controlElements.push_back(std::make_pair(measureNum, arpeggio));
        }
    }

    // slur
    pugi::xpath_node_set slurs = node.select_nodes("notations/slur");
    for (pugi::xpath_node_set::const_iterator it = slurs.begin(); it != slurs.end(); ++it) {
        pugi::xml_node slur = it->node();
        int slurNumber = slur.attribute("number").as_int();
        slurNumber = (slurNumber < 1) ? 1 : slurNumber;
        if (HasAttributeWithValue(slur, "type", "stop")) {
            CloseSlur(measure, slurNumber, element);
        }
        else if (HasAttributeWithValue(slur, "type", "start")) {
            Slur *meiSlur = new Slur();
            // color
            meiSlur->SetColor(slur.attribute("color").as_string());
            // lineform
            meiSlur->SetLform(meiSlur->AttCurveRend::StrToLineform(slur.attribute("line-type").as_string()));
            // placement and orientation
            meiSlur->SetCurvedir(InferCurvedir(slur));
            if (slur.attribute("id")) meiSlur->SetUuid(slur.attribute("id").as_string());
            // add it to the stack
            m_controlElements.push_back(std::make_pair(measureNum, meiSlur));
            OpenSlur(measure, slurNumber, meiSlur);
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
    pugi::xpath_node tupletEnd = notations.node().select_node("tuplet[@type='stop']");
    if (tupletEnd) {
        RemoveLastFromStack(TUPLET, layer);
    }

    // beam end
    bool beamEnd = node.select_node("beam[text()='end']");
    if (beamEnd) {
        int breakSec = (int)node.select_nodes("beam[text()='continue']").size();
        if (breakSec) {
            if (element->Is(NOTE)) {
                Note *note = dynamic_cast<Note *>(element);
                note->SetBreaksec(breakSec);
            }
            else if (element->Is(CHORD)) {
                Chord *chord = dynamic_cast<Chord *>(element);
                chord->SetBreaksec(breakSec);
            }
        }
        else {
            RemoveLastFromStack(BEAM, layer);
        }
    }

    // add StartIDs to dir, dynam, and pedal
    if (!m_dirStack.empty()) {
        std::vector<Dir *>::iterator iter;
        for (iter = m_dirStack.begin(); iter != m_dirStack.end(); ++iter) {
            if (!(*iter)->HasStaff())
                (*iter)->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
        m_dirStack.clear();
    }
    if (!m_dynamStack.empty()) {
        std::vector<Dynam *>::iterator iter;
        for (iter = m_dynamStack.begin(); iter != m_dynamStack.end(); ++iter) {
            if (!(*iter)->HasStaff())
                (*iter)->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
        m_dynamStack.clear();
    }
    if (!m_harmStack.empty()) {
        std::vector<Harm *>::iterator iter;
        for (iter = m_harmStack.begin(); iter != m_harmStack.end(); ++iter) {
            (*iter)->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
        m_harmStack.clear();
    }
    if (!m_octaveStack.empty()) {
        std::vector<Octave *>::iterator iter;
        for (iter = m_octaveStack.begin(); iter != m_octaveStack.end(); ++iter) {
            (*iter)->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            (*iter)->SetStartid(m_ID);
        }
        m_octaveStack.clear();
    }
    if (!m_pedalStack.empty()) {
        std::vector<Pedal *>::iterator iter;
        for (iter = m_pedalStack.begin(); iter != m_pedalStack.end(); ++iter) {
            if (!(*iter)->HasStaff())
                (*iter)->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
        m_pedalStack.clear();
    }
    if (!m_bracketStack.empty()) {
        std::vector<std::pair<BracketSpan *, musicxml::OpenSpanner> >::iterator iter;
        for (iter = m_bracketStack.begin(); iter != m_bracketStack.end(); ++iter) {
            if (!(iter->first)->HasStaff())
                iter->first->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
    }
    if (!m_tempoStack.empty()) {
        std::vector<Tempo *>::iterator iter;
        for (iter = m_tempoStack.begin(); iter != m_tempoStack.end(); ++iter) {
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

    if (HasAttributeWithValue(node, "new-system", "yes")) {
        Sb *sb = new Sb();
        section->AddChild(sb);
    }

    if (HasAttributeWithValue(node, "new-page", "yes")) {
        Pb *pb = new Pb();
        section->AddChild(pb);
    }
}

void MusicXmlInput::ReadMusicXmlBeamsAndTuplets(const pugi::xml_node &node, Layer *layer, bool isChord)
{
    pugi::xpath_node beamStart = node.select_node("beam[@number='1'][text()='begin']");
    pugi::xpath_node tupletStart = node.select_node("notations/tuplet[@type='start']");

    const auto measureNodeChildren = node.select_node("../.").node().children();
    std::vector<pugi::xml_node> currentMeasureNodes(measureNodeChildren.begin(), measureNodeChildren.end());
    // in case note is a start of both beam and tuplet - need to figure which one is longer
    if (beamStart && tupletStart) {
        pugi::xml_node beamEnd = node.select_node("./following-sibling::note[beam[@number='1'][text()='end']]").node();
        pugi::xml_node tupletEnd
            = node.select_node("./following-sibling::note[notations[tuplet[@type='stop']]]").node();

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
        pugi::xml_node beamEnd = node.select_node("./following-sibling::note[beam[@number='1'][text()='end']]").node();
        // find whether there is a tuplet that starts during the span of the beam
        pugi::xpath_node nextTupletStart
            = node.select_node("./following-sibling::note[notations[tuplet[@type='start']]]").node();
        pugi::xml_node tupletEnd
            = node.select_node("./following-sibling::note[notations[tuplet[@type='stop']]]").node();

        // find start and end of the beam
        const auto beamStartIterator = std::find(currentMeasureNodes.begin(), currentMeasureNodes.end(), node);
        const auto beamEndIterator = std::find(beamStartIterator, currentMeasureNodes.end(), beamEnd);

        if (beamEndIterator == currentMeasureNodes.end()) {
            LogError("MusicXML import: Beam ending point has not been found for <'%s'>", layer->GetUuid().c_str());
            return;
        }
        // form vector of the beam nodes and find whether there are tuplets that start or end within the beam
        std::vector<pugi::xml_node> beamNodes(beamStartIterator, beamEndIterator + 1);
        bool isTupletStartInBeam
            = (beamNodes.end() != std::find(beamNodes.begin(), beamNodes.end(), nextTupletStart.node()));
        bool isTupletEndInBeam = (beamNodes.end() != std::find(beamNodes.begin(), beamNodes.end(), tupletEnd));
        // in case if there is only start/end of the tuplet in the beam, then we need to use beamSpan instead
        if ((tupletEnd != beamEnd)
            && ((isTupletStartInBeam && !isTupletEndInBeam) || (!isTupletStartInBeam && isTupletEndInBeam))) {
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
}

void MusicXmlInput::ReadMusicXmlTupletStart(const pugi::xml_node &node, const pugi::xml_node &tupletStart, Layer *layer)
{
    if (!tupletStart) return;

    Tuplet *tuplet = new Tuplet();
    AddLayerElement(layer, tuplet);
    m_elementStackMap.at(layer).push_back(tuplet);
    int num = node.select_node("time-modification/actual-notes").node().text().as_int();
    int numbase = node.select_node("time-modification/normal-notes").node().text().as_int();
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

    Beam *beam = new Beam();
    if (beamStart.attribute("id")) beam->SetUuid(beamStart.attribute("id").as_string());
    AddLayerElement(layer, beam);
    m_elementStackMap.at(layer).push_back(beam);
}

//////////////////////////////////////////////////////////////////////////////
// String to attribute converters

data_ACCIDENTAL_WRITTEN MusicXmlInput::ConvertAccidentalToAccid(const std::string &value)
{
    static const std::map<std::string, data_ACCIDENTAL_WRITTEN> Accidental2Accid{
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
        { "triple-sharp", ACCIDENTAL_WRITTEN_ts }, //
        { "triple-flat", ACCIDENTAL_WRITTEN_tf } //
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
        { -2, ACCIDENTAL_GESTURAL_ff }, //
        { -1.5, ACCIDENTAL_GESTURAL_fd }, //
        { -1, ACCIDENTAL_GESTURAL_f }, //
        { -0.5, ACCIDENTAL_GESTURAL_fu }, //
        { 0, ACCIDENTAL_GESTURAL_n }, //
        { 0.5, ACCIDENTAL_GESTURAL_sd }, //
        { 1, ACCIDENTAL_GESTURAL_s }, //
        { 1.5, ACCIDENTAL_GESTURAL_su }, //
        { 2, ACCIDENTAL_GESTURAL_ss } //
    };

    const auto result = Alter2Accid.find(value);
    if (result != Alter2Accid.end()) {
        return result->second;
    }

    LogWarning("MusicXML import: Unsupported alter value '%.1f'", value);
    return ACCIDENTAL_GESTURAL_NONE;
}

data_ARTICULATION MusicXmlInput::ConvertArticulations(const std::string &value)
{
    static const std::map<std::string, data_ARTICULATION> Articulations2Id{
        // articulations
        { "accent", ARTICULATION_acc }, //
        { "detached-legato", ARTICULATION_NONE }, //
        { "doit", ARTICULATION_doit }, //
        { "falloff", ARTICULATION_fall }, //
        { "plop", ARTICULATION_plop }, //
        { "scoop", ARTICULATION_scoop }, //
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
    // if (value == "heavy-heavy") return; // TODO: Support Double thick barlines.
    if ((value == "light-heavy") && !repeat) return BARRENDITION_end;
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
        { "256th", DURATION_256 } //
    };

    const auto result = Type2Dur.find(value);
    if (result != Type2Dur.end()) {
        return result->second;
    }

    LogWarning("MusicXML import: Unsupported type '%s'", value.c_str());
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

    return TEXTRENDITION_none;
}

std::wstring MusicXmlInput::ConvertTypeToVerovioText(const std::string &value)
{
    static const std::map<std::string, std::wstring> Type2VerovioText{
        { "breve", L"\xE1D1" }, //
        { "whole", L"\xE1D2" }, //
        { "half", L"\xE1D3" }, //
        { "quarter", L"\xE1D5" }, //
        { "eighth", L"\xE1D7" }, //
        { "16th", L"\xE1D9" }, //
        { "32nd", L"\xE1DB" }, //
        { "64th", L"\xE1DD" }, //
        { "128th", L"\xE1DF" }, //
        { "256th", L"\xE1E1" }, //
        { "512th", L"\xE1E3" }, //
        { "1024th", L"\xE1E5" } //
    };

    const auto result = Type2VerovioText.find(value);
    if (result != Type2VerovioText.end()) {
        return result->second;
    }

    LogWarning("MusicXML import: Unsupported type '%s'", value.c_str());
    return std::wstring();
}

data_HEADSHAPE MusicXmlInput::ConvertNotehead(const std::string &value)
{
    static const std::map<std::string, data_HEADSHAPE> Notehead2Id{
        { "slash", HEADSHAPE_slash }, //
        { "triangle", HEADSHAPE_rtriangle }, //
        { "diamond", HEADSHAPE_diamond }, //
        { "square", HEADSHAPE_square }, //
        { "cross", HEADSHAPE_plus }, //
        { "x", HEADSHAPE_slash }, //
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

    LogWarning("MusicXML import: Unsupported pitch name '%s'", value.c_str());
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

std::string MusicXmlInput::ConvertAlterToSymbol(const std::string &value)
{
    static const std::map<std::string, std::string> Alter2Symbol{
        { "-2", "𝄫" }, //
        { "-1", "♭" }, //
        { "0", "♮" }, //
        { "1", "♯" }, //
        { "2", "𝄪" } //
    };

    const auto result = Alter2Symbol.find(value);
    if (result != Alter2Symbol.end()) {
        return result->second;
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

std::string MusicXmlInput::ConvertFigureGlyph(const std::string &value)
{
    static const std::map<std::string, std::string> FigureGlyphMap{
        { "sharp", "♯" }, //
        { "flat", "♭" }, //
        { "natural", "♮" }, //
        { "double-sharp", "𝄪" }, //
        { "flat-flat", "𝄫" }, //
        { "sharp-sharp", "♯♯" }, //
        { "backslash", "\u20E5" }, //
        { "slash", "\u0338" }, //
        { "cross", "+" } //
    };

    const auto result = FigureGlyphMap.find(value);
    if (result != FigureGlyphMap.end()) {
        return result->second;
    }

    return std::string();
}

bool MusicXmlInput::NotInEndingStack(const std::string &measureN)
{
    for (auto &endingItem : m_endingStack) {
        for (auto &measure : endingItem.first) {
            if (measure->GetN() == measureN) return false;
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
        fermata->SetExternalsymbols(fermata, "glyph.num", result->second);
        fermata->SetExternalsymbols(fermata, "glyph.auth", "smufl");
    }
    else if (const auto result = fermataExtSymbolsAbove.find(shape); result != fermataExtSymbolsAbove.end()) {
        fermata->SetExternalsymbols(fermata, "glyph.num", result->second);
        fermata->SetExternalsymbols(fermata, "glyph.auth", "smufl");
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

    const ArrayOfObjects *children = chord->GetChildren();
    auto it = find_if(children->begin(), children->end(), [](Object *object) {
        if (!object->Is(NOTE)) return false;
        Note *note = vrv_cast<Note *>(object);
        return !note->HasStaff();
    });
    if (it != chord->GetChildren()->end()) return;

    // if all notes have @staff attribute, but it's not the same staff at least of one note - leave it as is
    const auto chordStaff = vrv_cast<Note *>(chord->GetFirst(NOTE))->GetStaff();
    it = find_if(children->begin(), children->end(), [&chordStaff](Object *object) {
        if (!object->Is(NOTE)) return false;
        Note *note = vrv_cast<Note *>(object);
        return (chordStaff != note->GetStaff());
    });
    if (it != chord->GetChildren()->end()) return;

    // Now that we're sure that cross-staff is the same for all notes, we can set it to chord and clear of notes
    chord->SetStaff(chordStaff);
    for_each(children->begin(), children->end(), [](Object *object) {
        if (!object->Is(NOTE)) return;
        Note *note = vrv_cast<Note *>(object);
        note->ResetStaffIdent();
    });
}

} // namespace vrv
