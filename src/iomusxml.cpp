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
#include "ftrem.h"
#include "hairpin.h"
#include "harm.h"
#include "instrdef.h"
#include "keyaccid.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "mnum.h"
#include "mordent.h"
#include "mrest.h"
#include "mrpt.h"
#include "multirest.h"
#include "note.h"
#include "octave.h"
#include "pb.h"
#include "pedal.h"
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

//----------------------------------------------------------------------------
// MusicXmlInput
//----------------------------------------------------------------------------

MusicXmlInput::MusicXmlInput(Doc *doc, std::string filename) : FileInputStream(doc)
{
    m_filename = filename;
}

MusicXmlInput::~MusicXmlInput() {}

bool MusicXmlInput::ImportFile()
{
    try {
        m_doc->Reset();
        m_doc->SetType(Raw);
        pugi::xml_document xmlDoc;
        pugi::xml_parse_result result = xmlDoc.load_file(m_filename.c_str());
        if (!result) {
            return false;
        }
        pugi::xml_node root = xmlDoc.first_child();
        return ReadMusicXml(root);
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }
}

bool MusicXmlInput::ImportString(std::string const &musicxml)
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

bool MusicXmlInput::HasAttributeWithValue(pugi::xml_node node, std::string attribute, std::string value)
{
    assert(node);

    if (node.attribute(attribute.c_str()).value() == value) {
        return true;
    }
    return false;
}

bool MusicXmlInput::IsElement(pugi::xml_node node, std::string name)
{
    assert(node);

    if (std::string(node.name()) == name) {
        return true;
    }
    return false;
}

bool MusicXmlInput::HasContentWithValue(pugi::xml_node node, std::string value)
{
    assert(node);

    if (GetContent(node) == value) {
        return true;
    }
    return false;
}

std::string MusicXmlInput::GetContent(pugi::xml_node node)
{
    assert(node);

    if (node.text()) {
        return std::string(node.text().as_string());
    }
    return "";
}

std::string MusicXmlInput::GetContentOfChild(pugi::xml_node node, std::string child)
{
    pugi::xpath_node childNode = node.select_node(child.c_str());
    if (childNode.node()) {
        return GetContent(childNode.node());
    }
    return "";
}

void MusicXmlInput::AddMeasure(Section *section, Measure *measure, int i)
{
    assert(section);
    assert(i >= 0);

    // we just need to add a measure
    if (section->GetChildCount(MEASURE) <= i) {
        section->AddChild(measure);
    }
    // otherwise copy the content to the corresponding existing measure
    else {
        AttNNumberLikeComparison comparisonMeasure(MEASURE, measure->GetN());
        Measure *existingMeasure = dynamic_cast<Measure *>(section->FindDescendantByComparison(&comparisonMeasure, 1));
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

void MusicXmlInput::AddLayerElement(Layer *layer, LayerElement *element)
{
    assert(layer);
    assert(element);

    if (m_elementStack.empty()) {
        layer->AddChild(element);
    }
    else
        (m_elementStack.back()->AddChild(element));
}

Layer *MusicXmlInput::SelectLayer(pugi::xml_node node, Measure *measure)
{
    // Find voice number of node
    int layerNum = 1;
    std::string layerNumStr = GetContentOfChild(node, "voice");
    if (!layerNumStr.empty()) {
        layerNum = atoi(layerNumStr.c_str());
    }
    if (layerNum < 1) {
        LogWarning("MusicXML import: Layer %d cannot be found", layerNum);
        layerNum = 1;
    }
    // Check if voice number corresponds to an existing layer number in any staff (as with cross-staff notes).
    for (auto item : *measure->GetChildren()) {
        if (!item->Is(STAFF)) {
            continue;
        }
        Staff *staff = dynamic_cast<Staff *>(item);
        assert(staff);
        for (auto layer : *staff->GetChildren()) {
            assert(layer);
            // Now look for the layer with the corresponding voice
            if (layerNum == dynamic_cast<Layer *>(layer)->GetN()) {
                return SelectLayer(layerNum, staff);
            }
        }
    }
    // if not, take staff info of node element
    int staffNum = 1;
    std::string staffNumStr = GetContentOfChild(node, "staff");
    if (!staffNumStr.empty()) {
        staffNum = atoi(staffNumStr.c_str());
    }
    if ((staffNum < 1) || (staffNum > measure->GetChildCount(STAFF))) {
        LogWarning("MusicXML import: Staff %d cannot be found", staffNum);
        staffNum = 1;
    }
    staffNum--;
    Staff *staff = dynamic_cast<Staff *>(measure->GetChild(staffNum, STAFF));
    assert(staff);
    return SelectLayer(layerNum, staff);
}

Layer *MusicXmlInput::SelectLayer(int staffNum, Measure *measure)
{
    staffNum--;
    Staff *staff = dynamic_cast<Staff *>(measure->GetChild(staffNum, STAFF));
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
    return layer;
}

void MusicXmlInput::RemoveLastFromStack(ClassId classId)
{
    std::vector<LayerElement *>::reverse_iterator riter;
    for (riter = m_elementStack.rbegin(); riter != m_elementStack.rend(); ++riter) {
        if ((*riter)->Is(classId)) {
            m_elementStack.erase((riter + 1).base());
            return;
        }
    }
}

void MusicXmlInput::FillSpace(Layer *layer, int dur)
{
    std::string durStr;
    while (dur != 0) {
        float quaters = (double)dur / (double)m_ppq;
        if (quaters > 1) quaters = (int)quaters;
        // limit space for now
        if (quaters > 2) quaters = 2;
        durStr = std::to_string(int(4 / quaters));

        Space *space = new Space();
        space->SetDur(space->AttDurationLogical::StrToDuration(durStr));
        AddLayerElement(layer, space);
        dur -= m_ppq * quaters;
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
    std::vector<std::pair<Slur *, musicxml::OpenSlur> >::iterator iter;
    for (iter = m_slurStack.begin(); iter != m_slurStack.end(); ++iter) {
        if (iter->second.m_number == number) {
            iter->first->SetEndid("#" + element->GetUuid());
            m_slurStack.erase(iter);
            return;
        }
    }
    // add to m_slurStopStack, if not able to be closed
    musicxml::CloseSlur closeSlur(measure->GetN(), number);
    m_slurStopStack.push_back(std::make_pair(element, closeSlur));
}

//////////////////////////////////////////////////////////////////////////////
// Text rendering

void MusicXmlInput::TextRendition(pugi::xpath_node_set words, ControlElement *element)
{
    for (pugi::xpath_node_set::const_iterator it = words.begin(); it != words.end(); ++it) {
        pugi::xml_node textNode = it->node();
        std::string textStr = textNode.text().as_string();
        std::string textAlign = textNode.attribute("halign").as_string();
        std::string textColor = textNode.attribute("color").as_string();
        std::string textFont = textNode.attribute("font-family").as_string();
        std::string textStyle = textNode.attribute("font-style").as_string();
        std::string textWeight = textNode.attribute("font-weight").as_string();
        std::string lang = textNode.attribute("xml:lang").as_string();
        Text *text = new Text();
        text->SetText(UTF8to16(textStr));
        if (!textColor.empty() || !textFont.empty() || !textStyle.empty() || !textWeight.empty()) {
            Rend *rend = new Rend();
            if (words.size() > 1 && !lang.empty()) {
                rend->SetLang(lang.c_str());
            }
            rend->SetColor(textNode.attribute("color").as_string());
            if (!textAlign.empty())
                rend->SetHalign(rend->AttHorizontalAlign::StrToHorizontalalignment(textAlign.c_str()));
            if (!textFont.empty()) rend->SetFontfam(textFont.c_str());
            if (!textStyle.empty()) rend->SetFontstyle(rend->AttTypography::StrToFontstyle(textStyle.c_str()));
            if (!textWeight.empty()) rend->SetFontweight(rend->AttTypography::StrToFontweight(textWeight.c_str()));
            rend->AddChild(text);
            element->AddChild(rend);
        }
        else
            element->AddChild(text);
    }
}

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
    if (root.select_node("/score-partwise/identification/encoding/supports[@element='print']")) {
        m_hasLayoutInformation = true;
        // always start with a new page
        Pb *pb = new Pb();
        section->AddChild(pb);
    }

    std::vector<StaffGrp *> m_staffGrpStack;
    StaffGrp *staffGrp = new StaffGrp();
    m_doc->m_scoreDef.AddChild(staffGrp);
    m_staffGrpStack.push_back(staffGrp);

    int staffOffset = 0;
    m_octDis.push_back(0);

    pugi::xpath_node scoreMidiBpm = root.select_node("/score-partwise/part[1]/measure[1]/sound[@tempo][1]");
    if (scoreMidiBpm) m_doc->m_scoreDef.SetMidiBpm(scoreMidiBpm.node().attribute("tempo").as_int());

    pugi::xpath_node_set partListChildren = root.select_nodes("/score-partwise/part-list/*");
    for (pugi::xpath_node_set::const_iterator it = partListChildren.begin(); it != partListChildren.end(); ++it) {
        pugi::xpath_node xpathNode = *it;
        if (IsElement(xpathNode.node(), "part-group")) {
            if (HasAttributeWithValue(xpathNode.node(), "type", "start")) {
                StaffGrp *staffGrp = new StaffGrp();
                // read the group-symbol (MEI @symbol)
                std::string groupGymbol = GetContentOfChild(xpathNode.node(), "group-symbol");
                if (groupGymbol == "bracket") {
                    staffGrp->SetSymbol(staffGroupingSym_SYMBOL_bracket);
                }
                else if (groupGymbol == "brace") {
                    staffGrp->SetSymbol(staffGroupingSym_SYMBOL_brace);
                }
                else if (groupGymbol == "line") {
                    staffGrp->SetSymbol(staffGroupingSym_SYMBOL_line);
                }
                // now stack it
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
            std::string partId = xpathNode.node().attribute("id").as_string();
            std::string xpath = StringFormat("/score-partwise/part[@id='%s']/measure[1]", partId.c_str());
            pugi::xpath_node partFirstMeasure = root.select_node(xpath.c_str());
            if (!partFirstMeasure.node().select_node("attributes")) {
                LogWarning("MusicXML import: Could not find the 'attributes' element in the first "
                           "measure of part '%s'",
                    partId.c_str());
                continue;
            }
            int staves = partFirstMeasure.node().select_node("attributes/staves").node().text().as_int();
            Label *label = NULL;
            LabelAbbr *labelAbbr = NULL;
            InstrDef *instrdef = NULL;
            // part-name should be revised, as soon MEI can suppress labels
            std::string partName = GetContentOfChild(xpathNode.node(), "part-name[not(@print-object='no')]");
            std::string partAbbr = GetContentOfChild(xpathNode.node(), "part-abbreviation[not(@print-object='no')]");
            pugi::xpath_node midiInstrument = xpathNode.node().select_node("midi-instrument");
            pugi::xpath_node midiChannel = midiInstrument.node().select_node("midi-channel");
            pugi::xpath_node midiName = midiInstrument.node().select_node("midi-name");
            // pugi::xpath_node midiPan = midiInstrument.node().select_node("pan");
            pugi::xpath_node midiProgram = midiInstrument.node().select_node("midi-program");
            pugi::xpath_node midiVolume = midiInstrument.node().select_node("volume");
            if (!partName.empty()) {
                label = new Label();
                Text *text = new Text();
                text->SetText(UTF8to16(partName));
                label->AddChild(text);
            }
            if (!partAbbr.empty()) {
                labelAbbr = new LabelAbbr();
                Text *text = new Text();
                text->SetText(UTF8to16(partAbbr));
                labelAbbr->AddChild(text);
            }
            if (midiInstrument) {
                instrdef = new InstrDef;
                instrdef->SetMidiInstrname(
                    instrdef->AttMidiInstrument::StrToMidinames(midiName.node().text().as_string()));
                if (midiChannel) instrdef->SetMidiChannel(midiChannel.node().text().as_int() - 1);
                // if (midiPan) instrdef->SetMidiPan(midiPan.node().text().as_int());
                if (midiProgram) instrdef->SetMidiInstrnum(midiProgram.node().text().as_int() - 1);
                if (midiVolume) instrdef->SetMidiVolume(midiVolume.node().text().as_int());
            }
            // create the staffDef(s)
            StaffGrp *partStaffGrp = new StaffGrp();
            if (staves > 1) {
                if (label) partStaffGrp->AddChild(label);
                if (labelAbbr) partStaffGrp->AddChild(labelAbbr);
                if (instrdef) partStaffGrp->AddChild(instrdef);
            }
            int nbStaves = ReadMusicXmlPartAttributesAsStaffDef(partFirstMeasure.node(), partStaffGrp, staffOffset);
            // if we have more than one staff in the part we create a new staffGrp
            if (nbStaves > 1) {
                partStaffGrp->SetSymbol(staffGroupingSym_SYMBOL_brace);
                partStaffGrp->SetBarThru(BOOLEAN_true);
                m_staffGrpStack.back()->AddChild(partStaffGrp);
            }
            else {
                StaffDef *staffDef = dynamic_cast<StaffDef *>(partStaffGrp->FindDescendantByType(STAFFDEF));
                if (staffDef) {
                    if (label) staffDef->AddChild(label);
                    if (labelAbbr) staffDef->AddChild(labelAbbr);
                    if (instrdef) staffDef->AddChild(instrdef);
                }
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
        }
    }
    // here we could check that we have that there is only one staffGrp left in m_staffGrpStack

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

    int nbStaves = 1;

    for (pugi::xml_node::iterator it = node.begin(); it != node.end(); ++it) {

        // We read all attribute elements until we reach something else
        // barline, direction, print, and sound elements may be present
        if (!IsElement(*it, "attributes") && !IsElement(*it, "barline") && !IsElement(*it, "direction")
            && !IsElement(*it, "print") && !IsElement(*it, "sound")) {
            break;
        }

        // we do not want to read it again, just change the name
        if (IsElement(*it, "attributes")) it->set_name("mei-read");

        // First get the number of staves in the part
        pugi::xpath_node staves = it->select_node("staves");
        if (staves) {
            nbStaves = staves.node().text().as_int();
        }

        int i;
        std::string xpath;
        // Create as many staffDef
        for (i = 0; i < nbStaves; i++) {
            // Find or create the staffDef
            AttNIntegerComparison comparisonStaffDef(STAFFDEF, i + 1 + staffOffset);
            StaffDef *staffDef = dynamic_cast<StaffDef *>(staffGrp->FindDescendantByComparison(&comparisonStaffDef, 1));
            if (!staffDef) {
                staffDef = new StaffDef();
                staffDef->SetN(i + 1 + staffOffset);
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
                clefLine = it->select_node("clef/line");
            }
            if (clefLine.node().text()) {
                if (!clef) clef = new Clef();
                clef->SetLine(clef->AttLineLoc::StrToInt(clefLine.node().text().as_string()));
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
                if (change < 0)
                    clef->SetDisPlace(STAFFREL_basic_below);
                else if (change > 0)
                    clef->SetDisPlace(STAFFREL_basic_above);
            }
            // add it if necessary
            if (clef) {
                // Make it an attribute for now
                clef->IsAttribute(true);
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
                    keySig->SetSig(keySig->AttKeySigLog::StrToKeysignature("mixed"));
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
                meterSig->IsAttribute(true);
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
        if (m_multiRest != 0) {
            m_multiRest--;
        }
        else {
            Measure *measure = new Measure();
            m_measureCounts[measure] = i;
            ReadMusicXmlMeasure(xmlMeasure.node(), section, measure, nbStaves, staffOffset);
            // Add the measure to the system - if already there from a previous part we'll just merge the content
            AddMeasure(section, measure, i);
        }
        i++;
    }

    if (!m_ClefChangeStack.empty()) {
        for (musicxml::ClefChange iter : m_ClefChangeStack) {
            if (iter.isFirst)
                LogWarning("MusicXML import: Clef change at measure %s, staff %d, time %d not inserted.",
                    iter.m_measureNum.c_str(), iter.m_staff->GetN(), iter.m_scoreOnset);
        }
        m_ClefChangeStack.clear();
    }
    if (!m_tieStack.empty()) {
        LogWarning("MusicXML import: There are %d ties left open.", m_tieStack.size());
        m_tieStack.clear();
    }
    if (!m_slurStack.empty()) { // There are slurs left open
        std::vector<std::pair<Slur *, musicxml::OpenSlur> >::iterator iter;
        for (iter = m_slurStack.begin(); iter != m_slurStack.end(); ++iter) {
            LogWarning("MusicXML import: Slur element '%s' could not be ended.", iter->first->GetUuid().c_str());
        }
        m_slurStack.clear();
    }
    if (!m_slurStopStack.empty()) { // There are slurs ends without opening
        std::vector<std::pair<LayerElement *, musicxml::CloseSlur> >::iterator iter;
        for (iter = m_slurStopStack.begin(); iter != m_slurStopStack.end(); ++iter) {
            LogWarning("MusicXML import: Slur ending for element '%s' could not be "
                       "matched to a start element.",
                iter->first->GetUuid().c_str());
        }
        m_slurStopStack.clear();
    }
    if (!m_openDashesStack.empty()) { // open dashes without ending
        std::vector<std::pair<ControlElement *, musicxml::OpenDashes> >::iterator iter;
        for (iter = m_openDashesStack.begin(); iter != m_openDashesStack.end(); ++iter) {
            LogWarning(
                "MusicXML import: Dashes/extender lines for '%s' could not be closed.", iter->first->GetUuid().c_str());
        }
        m_openDashesStack.clear();
    }
    if (!m_bracketStack.empty()) { // open brackets without ending
        std::vector<std::pair<BracketSpan *, musicxml::OpenSpanner> >::iterator iter;
        for (iter = m_bracketStack.begin(); iter != m_bracketStack.end(); ++iter) {
            LogWarning("MusicXML import: BracketSpan for '%s' could not be closed.", iter->first->GetUuid().c_str());
        }
        m_bracketStack.clear();
    }

    return false;
}

bool MusicXmlInput::ReadMusicXmlMeasure(
    pugi::xml_node node, Section *section, Measure *measure, int nbStaves, int staffOffset)
{
    assert(node);
    assert(measure);

    std::string measureNum = node.attribute("number").as_string();
    if (measure != NULL) measure->SetN(measureNum);

    std::string implicit = node.attribute("implicit").as_string();
    if (implicit == "yes") {
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
    m_elementStack.clear();

    // reset measure time
    m_durTotal = 0;

    // read the content of the measure
    for (pugi::xml_node::iterator it = node.begin(); it != node.end(); ++it) {
        // first check if there is a multi measure rest
        if (it->select_node(".//multiple-rest")) {
            m_multiRest = it->select_node(".//multiple-rest").node().text().as_int();
            MultiRest *multiRest = new MultiRest;
            multiRest->SetNum(m_multiRest);
            Layer *layer = SelectLayer(1, measure);
            AddLayerElement(layer, multiRest);
            --m_multiRest;
            break;
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
            ReadMusicXmlNote(*it, measure, measureNum, staffOffset);
        }
        // for now only check first part
        else if (IsElement(*it, "print") && node.select_node("parent::part[not(preceding-sibling::part)]")) {
            ReadMusicXmlPrint(*it, section);
        }
    }

    // match open ties with close ties
    std::vector<std::pair<Tie *, Note *> >::iterator iter;
    for (iter = m_tieStack.begin(); iter != m_tieStack.end(); ++iter) {
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
            m_tieStack.erase(iter--);
        }
        else {
            iter->second->SetScoreTimeOnset(-1); // make scoreTimeOnset small for next measure
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
            staff->AddChild(new Layer());
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

    return true;
}

void MusicXmlInput::ReadMusicXmlAttributes(
    pugi::xml_node node, Section *section, Measure *measure, std::string measureNum)
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
        Staff *staff = dynamic_cast<Staff *>(measure->GetChild(staffNum - 1));
        assert(staff);
        pugi::xpath_node clefSign = clef.node().select_node("sign");
        pugi::xpath_node clefLine = clef.node().select_node("line");
        if (clefSign && clefLine) {
            Clef *meiClef = new Clef();
            meiClef->SetShape(meiClef->AttClefShape::StrToClefshape(clefSign.node().text().as_string()));
            meiClef->SetLine(meiClef->AttClefShape::StrToInt(clefLine.node().text().as_string()));
            // clef octave change
            pugi::xpath_node clefOctaveChange = clef.node().select_node("clef-octave-change");
            if (clefOctaveChange.node().text()) {
                int change = clefOctaveChange.node().text().as_int();
                if (abs(change) == 1)
                    meiClef->SetDis(OCTAVE_DIS_8);
                else if (abs(change) == 2)
                    meiClef->SetDis(OCTAVE_DIS_15);
                if (change < 0)
                    meiClef->SetDisPlace(STAFFREL_basic_below);
                else
                    meiClef->SetDisPlace(STAFFREL_basic_above);
            }
            m_ClefChangeStack.push_back(musicxml::ClefChange(measureNum, staff, meiClef, m_durTotal));
        }
    }

    // key and time change
    pugi::xpath_node key = node.select_node("key");
    pugi::xpath_node time = node.select_node("time");
    // for now only read first part and make it change in scoreDef
    if ((key || time) && node.select_node("ancestor::part[not(preceding-sibling::part)]")) {
        ScoreDef *scoreDef = new ScoreDef();
        KeySig *keySig = NULL;
        if (key.node().select_node("fifths")) {
            if (!keySig) keySig = new KeySig();
            int fifths = key.node().select_node("fifths").node().text().as_int();
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
            keySig->SetSig(keySig->AttKeySigLog::StrToKeysignature("mixed"));
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
        // Add it if necessary
        if (keySig) {
            // Make it an attribute for now
            keySig->IsAttribute(true);
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

void MusicXmlInput::ReadMusicXmlBackup(pugi::xml_node node, Measure *measure, std::string measureNum)
{
    assert(node);
    assert(measure);

    m_durTotal -= atoi(GetContentOfChild(node, "duration").c_str());

    pugi::xpath_node nextNote = node.next_sibling("note");
    if (nextNote && m_durTotal > 0) {
        // We need a <space> if a note follows that starts not at the beginning of the measure
        Layer *layer;
        if (node.select_node("voice"))
            layer = new Layer();
        else
            layer = SelectLayer(nextNote.node(), measure);
        FillSpace(layer, m_durTotal);
    }
}

void MusicXmlInput::ReadMusicXmlBarLine(pugi::xml_node node, Measure *measure, std::string measureNum)
{
    assert(node);
    assert(measure);

    Staff *staff = dynamic_cast<Staff *>(measure->GetFirst(STAFF));
    assert(staff);

    std::string barStyle = GetContentOfChild(node, "bar-style");
    pugi::xpath_node repeat = node.select_node("repeat");
    if (!barStyle.empty()) {
        data_BARRENDITION barRendition = ConvertStyleToRend(barStyle, repeat);
        if (HasAttributeWithValue(node, "location", "left")) {
            measure->SetLeft(barRendition);
        }
        else if (HasAttributeWithValue(node, "location", "middle")) {
            LogWarning("MusicXML import: Unsupported barline location 'middle'");
        }
        else {
            measure->SetRight(barRendition);
            if (barStyle == "short" || barStyle == "tick") {
                measure->SetBarLen(4);
                if (barStyle == "short")
                    measure->SetBarPlace(2);
                else
                    measure->SetBarPlace(-2);
            }
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
    pugi::xpath_node xmlFermata = node.select_node("fermata");
    if (xmlFermata) {
        Fermata *fermata = new Fermata();
        m_controlElements.push_back(std::make_pair(measureNum, fermata));
        if (HasAttributeWithValue(node, "location", "left")) {
            fermata->SetTstamp(0);
        }
        else if (HasAttributeWithValue(node, "location", "middle")) {
            LogWarning("MusicXML import: Unsupported barline location 'middle'");
        }
        else {
            fermata->SetTstamp(m_meterCount + 1);
        }
        fermata->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        // color
        fermata->SetColor(xmlFermata.node().attribute("color").as_string());
        // shape
        fermata->SetShape(ConvertFermataShape(xmlFermata.node().text().as_string()));
        // form and place
        if (HasAttributeWithValue(xmlFermata.node(), "type", "inverted")) {
            fermata->SetForm(fermataVis_FORM_inv);
            fermata->SetPlace(STAFFREL_below);
        }
        else if (HasAttributeWithValue(xmlFermata.node(), "type", "upright")) {
            fermata->SetForm(fermataVis_FORM_norm);
            fermata->SetPlace(STAFFREL_above);
        }
    }
}

void MusicXmlInput::ReadMusicXmlDirection(
    pugi::xml_node node, Measure *measure, std::string measureNum, int staffOffset)
{
    assert(node);
    assert(measure);

    pugi::xpath_node type = node.select_node("direction-type");
    std::string placeStr = node.attribute("placement").as_string();
    int offset = node.select_node("offset").node().text().as_int();
    double timeStamp = (double)(m_durTotal + offset) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0;

    // Directive
    std::string dynamStr = ""; // string containing dynamics information
    int defaultY = 0; // y position attribute, only for directives and dynamics
    pugi::xpath_node_set words = type.node().select_nodes("words");
    if (words.size() != 0 && !node.select_node("sound[@tempo]")) {
        defaultY = words.first().node().attribute("default-y").as_int();
        std::string wordStr = words.first().node().text().as_string();
        if (wordStr.rfind("cresc", 0) == 0 || wordStr.rfind("dim", 0) == 0 || wordStr.rfind("decresc", 0) == 0) {
            dynamStr = wordStr;
        }
        else {
            Dir *dir = new Dir();
            if (words.size() == 1) {
                dir->SetLang(words.first().node().attribute("xml:lang").as_string());
            }
            dir->SetPlace(dir->AttPlacement::StrToStaffrel(placeStr.c_str()));
            dir->SetTstamp(timeStamp);
            pugi::xpath_node staffNode = node.select_node("staff");
            if (staffNode)
                dir->SetStaff(dir->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(staffNode.node().text().as_int() + staffOffset)));
            TextRendition(words, dir);
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 200;
            dir->SetVgrp(defaultY);
            m_controlElements.push_back(std::make_pair(measureNum, dir));
            m_dirStack.push_back(dir);
        }
    }

    // Dynamics
    pugi::xpath_node dynamics = type.node().select_node("dynamics");
    if (dynamics || !dynamStr.empty()) {
        if (dynamStr.empty()) dynamStr = GetContentOfChild(dynamics.node(), "other-dynamics");
        if (dynamStr.empty()) dynamStr = dynamics.node().first_child().name();
        Dynam *dynam = new Dynam();
        dynam->SetPlace(dynam->AttPlacement::StrToStaffrel(placeStr.c_str()));
        Text *text = new Text();
        text->SetText(UTF8to16(dynamStr));
        dynam->AddChild(text);
        dynam->SetTstamp(timeStamp);
        pugi::xpath_node staffNode = node.select_node("staff");
        if (staffNode)
            dynam->SetStaff(dynam->AttStaffIdent::StrToXsdPositiveIntegerList(
                std::to_string(staffNode.node().text().as_int() + staffOffset)));
        if (defaultY == 0) defaultY = dynamics.node().attribute("default-y").as_int();
        // parse the default_y attribute and transform to vgrp value, to vertically align dynamics and directives
        defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 200;
        dynam->SetVgrp(defaultY);
        m_controlElements.push_back(std::make_pair(measureNum, dynam));
        m_dynamStack.push_back(dynam);
    }

    // Dashes (to be connected with previous <dir> or <dynam> as @extender and @tstamp2 attribute
    pugi::xpath_node dashes = type.node().select_node("dashes");
    if (dashes) {
        int dashesNumber = dashes.node().attribute("number").as_int();
        dashesNumber = (dashesNumber < 1) ? 1 : dashesNumber;
        int staffNum = 1;
        pugi::xpath_node staffNode = node.select_node("staff");
        if (staffNode) staffNum = staffNode.node().text().as_int() + staffOffset;
        if (HasAttributeWithValue(dashes.node(), "type", "stop")) {
            std::vector<std::pair<ControlElement *, musicxml::OpenDashes> >::iterator iter;
            for (iter = m_openDashesStack.begin(); iter != m_openDashesStack.end(); ++iter) {
                int measureDifference = m_measureCounts.at(measure) - iter->second.m_measureCount;
                if (iter->second.m_dirN == dashesNumber && iter->second.m_staffNum == staffNum) {
                    if (iter->first->Is(DYNAM))
                        dynamic_cast<Dynam *>(iter->first)
                            ->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
                    if (iter->first->Is(DIR))
                        dynamic_cast<Dir *>(iter->first)
                            ->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
                    m_openDashesStack.erase(iter--);
                }
            }
        }
        else {
            ControlElement *controlElement = nullptr;
            // find last ControlElement of type dynam or dir and activate extender
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

    // Hairpins
    pugi::xpath_node wedge = type.node().select_node("wedge");
    if (wedge) {
        int hairpinNumber = wedge.node().attribute("number").as_int();
        hairpinNumber = (hairpinNumber < 1) ? 1 : hairpinNumber;
        if (HasAttributeWithValue(wedge.node(), "type", "stop")) {
            // match wedge type=stop to open hairpin
            std::vector<std::pair<Hairpin *, musicxml::OpenSpanner> >::iterator iter;
            for (iter = m_hairpinStack.begin(); iter != m_hairpinStack.end(); ++iter) {
                if (iter->second.m_dirN == hairpinNumber) {
                    int measureDifference = m_measureCounts.at(measure) - iter->second.m_lastMeasureCount;
                    iter->first->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
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
            if (HasAttributeWithValue(wedge.node(), "type", "crescendo")) {
                hairpin->SetForm(hairpinLog_FORM_cres);
            }
            else if (HasAttributeWithValue(wedge.node(), "type", "diminuendo")) {
                hairpin->SetForm(hairpinLog_FORM_dim);
            }
            hairpin->SetColor(wedge.node().attribute("color").as_string());
            hairpin->SetPlace(hairpin->AttPlacement::StrToStaffrel(placeStr.c_str()));
            hairpin->SetTstamp(timeStamp);
            pugi::xpath_node staffNode = node.select_node("staff");
            if (staffNode)
                hairpin->SetStaff(hairpin->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(staffNode.node().text().as_int() + staffOffset)));
            int defaultY = wedge.node().attribute("default-y").as_int();
            // parse the default_y attribute and transform to vgrp value, to vertically align hairpins
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 200;
            hairpin->SetVgrp(defaultY);
            // match new hairpin to existing hairpin stop
            std::vector<std::tuple<int, double, musicxml::OpenSpanner> >::iterator iter;
            for (iter = m_hairpinStopStack.begin(); iter != m_hairpinStopStack.end(); ++iter) {
                if (std::get<2>(*iter).m_dirN == hairpinNumber) {
                    int measureDifference = std::get<2>(*iter).m_lastMeasureCount - m_measureCounts.at(measure);
                    hairpin->SetTstamp2(std::pair<int, double>(measureDifference, std::get<1>(*iter)));
                    Staff *staff = dynamic_cast<Staff *>(measure->FindDescendantByType(STAFF));
                    assert(staff);
                    hairpin->SetStaff(
                        staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(std::get<0>(*iter))));
                    m_controlElements.push_back(std::make_pair(measureNum, hairpin));
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
    pugi::xpath_node xmlShift = type.node().select_node("octave-shift");
    if (xmlShift) {
        pugi::xpath_node staffNode = node.select_node("staff");
        int staffN = (!staffNode) ? 1 : staffNode.node().text().as_int() + staffOffset;
        if (HasAttributeWithValue(xmlShift.node(), "type", "stop")) {
            m_octDis[staffN] = 0;
            std::vector<std::pair<std::string, ControlElement *> >::iterator iter;
            for (iter = m_controlElements.begin(); iter != m_controlElements.end(); ++iter) {
                if (iter->second->Is(OCTAVE)) {
                    Octave *octave = dynamic_cast<Octave *>(iter->second);
                    std::vector<int> staffAttr = octave->GetStaff();
                    if (std::find(staffAttr.begin(), staffAttr.end(), staffN) != staffAttr.end()
                        && !octave->HasEndid()) {
                        octave->SetEndid(m_ID);
                    }
                }
            }
        }
        else {
            Octave *octave = new Octave();
            octave->SetColor(xmlShift.node().attribute("color").as_string());
            octave->SetDisPlace(octave->AttOctaveDisplacement::StrToStaffrelBasic(placeStr.c_str()));
            octave->SetStaff(octave->AttStaffIdent::StrToXsdPositiveIntegerList(std::to_string(staffN)));
            octave->SetDis(
                octave->AttOctaveDisplacement::StrToOctaveDis(xmlShift.node().attribute("size").as_string()));
            m_octDis[staffN] = (xmlShift.node().attribute("size").as_int() + 2) / 8;
            if (HasAttributeWithValue(xmlShift.node(), "type", "up")) {
                octave->SetDisPlace(STAFFREL_basic_below);
                m_octDis[staffN] = -1 * m_octDis[staffN];
            }
            else
                octave->SetDisPlace(STAFFREL_basic_above);
            m_controlElements.push_back(std::make_pair(measureNum, octave));
            m_octaveStack.push_back(octave);
        }
    }

    // Pedal
    pugi::xpath_node xmlPedal = type.node().select_node("pedal");
    if (xmlPedal) {
        std::string pedalType = xmlPedal.node().attribute("type").as_string();
        std::string pedalLine = xmlPedal.node().attribute("line").as_string();
        // do not import pedal start lines until engraving supported, but import stops anyway
        if (pedalLine != "yes" || pedalType == "stop") {
            Pedal *pedal = new Pedal();
            pedal->SetTstamp(timeStamp);
            if (!placeStr.empty()) pedal->SetPlace(pedal->AttPlacement::StrToStaffrel(placeStr.c_str()));
            if (!pedalType.empty()) pedal->SetDir(ConvertPedalTypeToDir(pedalType));
            pugi::xpath_node staffNode = node.select_node("staff");
            if (staffNode)
                pedal->SetStaff(pedal->AttStaffIdent::StrToXsdPositiveIntegerList(
                    std::to_string(staffNode.node().text().as_int() + staffOffset)));
            int defaultY = xmlPedal.node().attribute("default-y").as_int();
            // parse the default_y attribute and transform to vgrp value, to vertically align pedal starts and stops
            defaultY = (defaultY < 0) ? std::abs(defaultY) : defaultY + 200;
            pedal->SetVgrp(defaultY);
            m_controlElements.push_back(std::make_pair(measureNum, pedal));
            m_pedalStack.push_back(pedal);
        }
    }

    // Principal voice
    pugi::xpath_node lead = type.node().select_node("principal-voice");
    if (lead) {
        int voiceNumber = wedge.node().attribute("number").as_int();
        voiceNumber = (voiceNumber < 1) ? 1 : voiceNumber;
        if (HasAttributeWithValue(lead.node(), "type", "stop")) {
            int measureDifference = m_measureCounts.at(measure) - m_bracketStack.front().second.m_lastMeasureCount;
            m_bracketStack.front().first->SetTstamp2(std::pair<int, double>(measureDifference, timeStamp));
            m_bracketStack.erase(m_bracketStack.begin());
        }
        else {
            // std::string symbol = lead.node().attribute("symbol").as_string();
            BracketSpan *bracketSpan = new BracketSpan();
            musicxml::OpenSpanner openBracket(voiceNumber, m_measureCounts.at(measure));
            bracketSpan->SetColor(lead.node().attribute("color").as_string());
            // bracketSpan->SetPlace(bracketSpan->AttPlacement::StrToStaffrel(placeStr.c_str()));
            bracketSpan->SetFunc("analytical");
            bracketSpan->SetTstamp(timeStamp);
            bracketSpan->SetType("principal-voice");
            m_controlElements.push_back(std::make_pair(measureNum, bracketSpan));
            m_bracketStack.push_back(std::make_pair(bracketSpan, openBracket));
        }
    }

    // Tempo
    pugi::xpath_node metronome = type.node().select_node("metronome");
    if (node.select_node("sound[@tempo]") || metronome) {
        Tempo *tempo = new Tempo();
        if (words.size() == 1) {
            tempo->SetLang(words.first().node().attribute("xml:lang").as_string());
        }
        tempo->SetPlace(tempo->AttPlacement::StrToStaffrel(placeStr.c_str()));
        if (words.size() != 0) TextRendition(words, tempo);
        if (metronome)
            PrintMetronome(metronome.node(), tempo);
        else
            tempo->SetMidiBpm(node.select_node("sound").node().attribute("tempo").as_int());
        m_controlElements.push_back(std::make_pair(measureNum, tempo));
        m_tempoStack.push_back(tempo);
    }

    // other cases
    if (words.size() == 0 && !dynamics && !lead && !metronome && !xmlShift && !xmlPedal && !wedge && !dashes) {
        LogWarning("MusicXML import: Unsupported direction-type '%s'", type.node().first_child().name());
    }
}

void MusicXmlInput::ReadMusicXmlFigures(pugi::xml_node node, Measure *measure, std::string measureNum)
{
    assert(node);
    assert(measure);

    if (!HasAttributeWithValue(node, "print-object", "no")) {
        Harm *harm = new Harm();
        Fb *fb = new Fb();

        int durOffset = 0;

        // std::string textColor = node.attribute("color").as_string();
        // std::string textStyle = node.attribute("font-style").as_string();
        // std::string textWeight = node.attribute("font-weight").as_string();
        for (pugi::xml_node figure = node.child("figure"); figure; figure = figure.next_sibling("figure")) {
            std::string textStr = GetContent(figure.select_node("figure-number").node());
            F *f = new F();
            Text *text = new Text();
            text->SetText(UTF8to16(textStr));
            f->AddChild(text);
            fb->AddChild(f);
        }
        harm->AddChild(fb);
        harm->SetTstamp((double)(m_durTotal + durOffset) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
        m_controlElements.push_back(std::make_pair(measureNum, harm));
        m_harmStack.push_back(harm);
    }
}

void MusicXmlInput::ReadMusicXmlForward(pugi::xml_node node, Measure *measure, std::string measureNum)
{
    assert(node);
    assert(measure);

    m_durTotal += atoi(GetContentOfChild(node, "duration").c_str());

    Layer *layer = SelectLayer(node, measure);

    pugi::xpath_node prevNote = node.select_node("preceding-sibling::note[1]");
    pugi::xpath_node nextNote = node.select_node("following-sibling::note[1]");
    if (nextNote) {
        // We need a <space> if a note follows
        if (!node.select_node("voice")) layer = SelectLayer(nextNote.node(), measure);
        FillSpace(layer, atoi(GetContentOfChild(node, "duration").c_str()));
    }
    else if (!prevNote && !node.select_node("preceding-sibling::backup")) {
        // If there is no previous or following note in the first layer, the measure seems to be empty
        // an invisible mRest is used, which should be replaced by mSpace, when available
        MRest *mRest = new MRest();
        mRest->SetVisible(BOOLEAN_false);
        AddLayerElement(layer, mRest);
    }
}

void MusicXmlInput::ReadMusicXmlHarmony(pugi::xml_node node, Measure *measure, std::string measureNum)
{
    assert(node);
    assert(measure);

    int durOffset = 0;

    std::string harmText = GetContentOfChild(node, "root/root-step");
    pugi::xpath_node alter = node.select_node("root/root-alter");
    harmText += ConvertAlterToSymbol(GetContent(alter.node()));
    pugi::xpath_node kind = node.select_node("kind");
    if (kind) {
        if (HasAttributeWithValue(kind.node(), "use-symbols", "yes")) {
            harmText = harmText + ConvertKindToSymbol(GetContent(kind.node()));
        }
        else if (kind.node().attribute("text")) {
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
        harmText += ConvertAlterToSymbol(GetContent(alter.node()));
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

void MusicXmlInput::ReadMusicXmlNote(pugi::xml_node node, Measure *measure, std::string measureNum, int staffOffset)
{
    assert(node);
    assert(measure);

    Layer *layer = SelectLayer(node, measure);
    assert(layer);

    Staff *staff = dynamic_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    pugi::xpath_node isChord = node.select_node("chord");

    // add clef changes to all layers of a given measure, staff, and time stamp
    if (!m_ClefChangeStack.empty()) {
        std::vector<musicxml::ClefChange>::iterator iter;
        for (iter = m_ClefChangeStack.begin(); iter != m_ClefChangeStack.end(); ++iter) {
            if (iter->m_measureNum == measureNum && iter->m_staff == staff && iter->m_scoreOnset == m_durTotal
                && !isChord) {
                if (iter->isFirst) { // add clef when first in staff
                    AddLayerElement(layer, iter->m_clef);
                    iter->isFirst = false;
                }
                else {
                    Clef *sameasClef = new Clef(); // add clef with @sameas referring to original clef
                    sameasClef->SetSameas("#" + iter->m_clef->GetUuid());
                    AddLayerElement(layer, sameasClef);
                }
            }
        }
    }

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

    bool cue = false;
    if (node.select_node("cue") || node.select_node("type[@size='cue']")) cue = true;

    // duration string and dots
    std::string typeStr = GetContentOfChild(node, "type");
    int dots = (int)node.select_nodes("dot").size();

    // beam start
    bool beamStart = node.select_node("beam[@number='1'][text()='begin']");
    if (beamStart) {
        Beam *beam = new Beam();
        AddLayerElement(layer, beam);
        m_elementStack.push_back(beam);
    }

    // tremolos
    pugi::xpath_node tremolo = notations.node().select_node("ornaments/tremolo");
    int tremSlashNum = 0;
    if (tremolo) {
        if (HasAttributeWithValue(tremolo.node(), "type", "single")) {
            BTrem *bTrem = new BTrem();
            AddLayerElement(layer, bTrem);
            m_elementStack.push_back(bTrem);
            tremSlashNum = tremolo.node().text().as_int();
        }
        else if (HasAttributeWithValue(tremolo.node(), "type", "start")) {
            FTrem *fTrem = new FTrem();
            AddLayerElement(layer, fTrem);
            m_elementStack.push_back(fTrem);
            fTrem->SetBeams(tremolo.node().text().as_int());
        }
    }

    // tuplet start
    // For now tuplet with beam if starting at the same time. However, this will
    // quite likely not work if we have a tuplet over serveral beams. We would need to check which
    // one is ending first in order to determine which one is on top of the hierarchy.
    // Also, it is not 100% sure that we can represent them as tuplet and beam elements.
    pugi::xpath_node tupletStart = notations.node().select_node("tuplet[@type='start']");
    if (tupletStart) {
        Tuplet *tuplet = new Tuplet();
        AddLayerElement(layer, tuplet);
        m_elementStack.push_back(tuplet);
        pugi::xpath_node actualNotes = node.select_node("time-modification/actual-notes");
        pugi::xpath_node normalNotes = node.select_node("time-modification/normal-notes");
        if (actualNotes && normalNotes) {
            tuplet->SetNum(actualNotes.node().text().as_int());
            tuplet->SetNumbase(normalNotes.node().text().as_int());
        }
        tuplet->SetNumPlace(
            tuplet->AttTupletVis::StrToStaffrelBasic(tupletStart.node().attribute("placement").as_string()));
        tuplet->SetBracketPlace(
            tuplet->AttTupletVis::StrToStaffrelBasic(tupletStart.node().attribute("placement").as_string()));
        tuplet->SetNumFormat(ConvertTupletNumberValue(tupletStart.node().attribute("show-number").as_string()));
        if (HasAttributeWithValue(tupletStart.node(), "show-number", "none")) tuplet->SetNumVisible(BOOLEAN_false);
        tuplet->SetBracketVisible(ConvertWordToBool(tupletStart.node().attribute("bracket").as_string()));
    }

    int duration = atoi(GetContentOfChild(node, "duration").c_str());
    pugi::xpath_node rest = node.select_node("rest");
    if (rest) {
        std::string stepStr = GetContentOfChild(rest.node(), "display-step");
        std::string octaveStr = GetContentOfChild(rest.node(), "display-octave");
        if (HasAttributeWithValue(node, "print-object", "no")) {
            Space *space = new Space();
            element = space;
            space->SetDur(ConvertTypeToDur(typeStr));
            AddLayerElement(layer, space);
        }
        // we assume /note without /type or with duration of an entire bar to be mRest
        else if (typeStr.empty() || duration == (m_ppq * 4 * m_meterCount / m_meterUnit)
            || HasAttributeWithValue(rest.node(), "measure", "yes")) {
            if (m_slash) {
                for (int i = m_meterCount; i > 0; --i) {
                    BeatRpt *slash = new BeatRpt;
                    AddLayerElement(layer, slash);
                }
                return;
            }
            else {
                MRest *mRest = new MRest();
                element = mRest;
                if (cue) mRest->SetCue(BOOLEAN_true);
                if (!stepStr.empty()) mRest->SetPloc(ConvertStepToPitchName(stepStr));
                if (!octaveStr.empty()) mRest->SetOloc(atoi(octaveStr.c_str()));
                AddLayerElement(layer, mRest);
            }
        }
        else {
            Rest *rest = new Rest();
            element = rest;
            rest->SetDur(ConvertTypeToDur(typeStr));
            rest->SetDurPpq(duration);
            if (dots > 0) rest->SetDots(dots);
            if (cue) rest->SetCue(BOOLEAN_true);
            if (!stepStr.empty()) rest->SetPloc(ConvertStepToPitchName(stepStr));
            if (!octaveStr.empty()) rest->SetOloc(atoi(octaveStr.c_str()));
            AddLayerElement(layer, rest);
        }
    }
    else {
        Note *note = new Note();
        element = note;
        note->SetVisible(ConvertWordToBool(node.append_attribute("print-object").as_string()));
        note->SetColor(node.attribute("color").as_string());
        if (node.attribute("xml:id")) {
            note->SetUuid(node.attribute("xml:id").as_string());
        }
        note->SetScoreTimeOnset(onset); // remember the MIDI onset within that measure
        int noteStaffNum = atoi(GetContentOfChild(node, "staff").c_str());
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
        std::string stemDirStr = GetContentOfChild(node, "stem");
        if (stemDirStr == "down") {
            stemDir = STEMDIRECTION_down;
        }
        else if (stemDirStr == "up") {
            stemDir = STEMDIRECTION_up;
        }

        // pitch and octave
        pugi::xpath_node pitch = node.select_node("pitch");
        if (pitch) {
            std::string stepStr = GetContentOfChild(pitch.node(), "step");
            if (!stepStr.empty()) note->SetPname(ConvertStepToPitchName(stepStr));
            std::string octaveStr = GetContentOfChild(pitch.node(), "octave");
            if (!octaveStr.empty()) {
                if (m_octDis[staff->GetN()] != 0) {
                    note->SetOct(atoi(octaveStr.c_str()) - m_octDis[staff->GetN()]);
                    note->SetOctGes(atoi(octaveStr.c_str()));
                }
                else
                    note->SetOct(atoi(octaveStr.c_str()));
            }
            std::string alterStr = GetContentOfChild(pitch.node(), "alter");
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

        // notehead
        pugi::xpath_node notehead = node.select_node("notehead");
        if (notehead) {
            // if (HasAttributeWithValue(notehead.node(), "parentheses", "yes")) note->SetEnclose(ENCLOSURE_paren);
        }

        // look at the next note to see if we are starting or ending a chord
        pugi::xpath_node nextNote = node.select_node("./following-sibling::note");
        if (nextNote.node().select_node("chord")) nextIsChord = true;
        // create the chord if we are starting a new chord
        if (nextIsChord) {
            if (m_elementStack.empty() || !m_elementStack.back()->Is(CHORD)) {
                Chord *chord = new Chord();
                chord->SetDur(ConvertTypeToDur(typeStr));
                chord->SetDurPpq(atoi(GetContentOfChild(node, "duration").c_str()));
                if (dots > 0) chord->SetDots(dots);
                chord->SetStemDir(stemDir);
                if (cue) chord->SetCue(BOOLEAN_true);
                if (tremSlashNum != 0)
                    chord->SetStemMod(chord->AttStems::StrToStemmodifier(std::to_string(tremSlashNum) + "slash"));
                AddLayerElement(layer, chord);
                m_elementStack.push_back(chord);
                element = chord;
            }
        }

        // grace notes
        pugi::xpath_node grace = node.select_node("grace");
        if (grace) {
            std::string slashStr = grace.node().attribute("slash").as_string();
            if (slashStr == "no") {
                note->SetGrace(GRACE_acc);
            }
            else if (slashStr == "yes") {
                note->SetGrace(GRACE_unacc);
                note->SetStemMod(STEMMODIFIER_1slash);
            }
            else {
                note->SetGrace(GRACE_unknown);
            }
        }

        // set attributes to the note if we are not in a chord
        if (m_elementStack.empty() || !m_elementStack.back()->Is(CHORD)) {
            note->SetDur(ConvertTypeToDur(typeStr));
            note->SetDurPpq(atoi(GetContentOfChild(node, "duration").c_str()));
            if (dots > 0) note->SetDots(dots);
            note->SetStemDir(stemDir);
            if (cue) note->SetCue(BOOLEAN_true);
            if (tremSlashNum != 0)
                note->SetStemMod(note->AttStems::StrToStemmodifier(std::to_string(tremSlashNum) + "slash"));
        }

        // verse / syl
        pugi::xpath_node_set lyrics = node.select_nodes("lyric");
        for (pugi::xpath_node_set::const_iterator it = lyrics.begin(); it != lyrics.end(); ++it) {
            pugi::xml_node lyric = it->node();
            int lyricNumber = lyric.attribute("number").as_int();
            lyricNumber = (lyricNumber < 1) ? 1 : lyricNumber;
            Verse *verse = new Verse();
            verse->SetColor(lyric.attribute("color").as_string());
            verse->SetLabel(lyric.attribute("name").as_string());
            verse->SetN(lyricNumber);
            for (pugi::xml_node textNode = lyric.child("text"); textNode; textNode = textNode.next_sibling("text")) {
                if (!HasAttributeWithValue(lyric, "print-object", "no")) {
                    // std::string textColor = textNode.attribute("color").as_string();
                    std::string textStyle = textNode.attribute("font-style").as_string();
                    std::string textWeight = textNode.attribute("font-weight").as_string();
                    std::string lang = textNode.attribute("xml:lang").as_string();
                    std::string textStr = textNode.text().as_string();
                    Syl *syl = new Syl();
                    syl->SetLang(lang.c_str());
                    if (lyric.select_node("extend")) {
                        syl->SetCon(sylLog_CON_u);
                    }
                    if (textNode.next_sibling("elision")) {
                        syl->SetCon(sylLog_CON_b);
                    }
                    if (GetContentOfChild(lyric, "syllabic") == "single") {
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
            note->AddChild(verse);
        }

        // ties
        pugi::xpath_node startTie = notations.node().select_node("tied[@type='start']");
        pugi::xpath_node endTie = notations.node().select_node("tied[@type='stop']");
        if (endTie) { // add to stack if (endTie) or if pitch/oct match to open tie on m_tieStack
            m_tieStopStack.push_back(note);
        }
        else {
            CloseTie(note);
        }
        if (startTie) {
            Tie *tie = new Tie();
            // color
            tie->SetColor(startTie.node().attribute("color").as_string());
            // placement and orientation
            tie->SetCurvedir(
                tie->AttCurvature::StrToCurvatureCurvedir(startTie.node().attribute("placement").as_string()));
            if (!startTie.node().attribute("orientation").empty()) { // override only with non-empty attribute
                tie->SetCurvedir(ConvertOrientationToCurvedir(startTie.node().attribute("orientation").as_string()));
            }
            tie->SetLform(tie->AttCurveRend::StrToLineform(startTie.node().attribute("line-type").as_string()));

            // add it to the stack
            m_controlElements.push_back(std::make_pair(measureNum, tie));
            OpenTie(note, tie);
        }

        // articulation
        std::vector<data_ARTICULATION> artics;
        for (pugi::xml_node articulations = notations.node().child("articulations"); articulations;
             articulations = articulations.next_sibling("articulations")) {
            Artic *artic = new Artic();
            if (articulations.select_node("accent")) artics.push_back(ARTICULATION_acc);
            if (articulations.select_node("spiccato")) artics.push_back(ARTICULATION_spicc);
            if (articulations.select_node("staccatissimo")) artics.push_back(ARTICULATION_stacciss);
            if (articulations.select_node("staccato")) artics.push_back(ARTICULATION_stacc);
            if (articulations.select_node("strong-accent")) artics.push_back(ARTICULATION_marc);
            if (articulations.select_node("tenuto")) artics.push_back(ARTICULATION_ten);
            artic->SetArtic(artics);
            element->AddChild(artic);
            artics.clear();
        }
        for (pugi::xml_node technical = notations.node().child("technical"); technical;
             technical = technical.next_sibling("technical")) {
            Artic *artic = new Artic();
            if (technical.select_node("down-bow")) artics.push_back(ARTICULATION_dnbow);
            if (technical.select_node("harmonic")) artics.push_back(ARTICULATION_harm);
            if (technical.select_node("open-string")) artics.push_back(ARTICULATION_open);
            if (technical.select_node("snap-pizzicato")) artics.push_back(ARTICULATION_snap);
            if (technical.select_node("stopped")) artics.push_back(ARTICULATION_stop);
            if (technical.select_node("up-bow")) artics.push_back(ARTICULATION_upbow);
            artic->SetArtic(artics);
            artic->SetType("technical");
            element->AddChild(artic);
        }

        // add the note to the layer or to the current container
        AddLayerElement(layer, note);

        // if we are ending a chord remove it from the stack
        if (!nextIsChord) {
            if (!m_elementStack.empty() && m_elementStack.back()->Is(CHORD)) {
                RemoveLastFromStack(CHORD);
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

    // Dynamics
    pugi::xpath_node xmlDynam = notations.node().select_node("dynamics");
    if (xmlDynam) {
        Dynam *dynam = new Dynam();
        m_controlElements.push_back(std::make_pair(measureNum, dynam));
        dynam->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        dynam->SetStartid(m_ID);
        std::string dynamStr = GetContentOfChild(xmlDynam.node(), "other-dynamics");
        if (dynamStr.empty()) dynamStr = xmlDynam.node().first_child().name();
        // place
        dynam->SetPlace(dynam->AttPlacement::StrToStaffrel(xmlDynam.node().attribute("placement").as_string()));
        Text *text = new Text();
        text->SetText(UTF8to16(dynamStr));
        dynam->AddChild(text);
    }

    // fermatas
    pugi::xpath_node xmlFermata = notations.node().select_node("fermata");
    if (xmlFermata) {
        Fermata *fermata = new Fermata();
        m_controlElements.push_back(std::make_pair(measureNum, fermata));
        fermata->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        fermata->SetStartid(m_ID);
        // color
        fermata->SetColor(xmlFermata.node().attribute("color").as_string());
        // shape
        fermata->SetShape(ConvertFermataShape(xmlFermata.node().text().as_string()));
        // form and place
        if (HasAttributeWithValue(xmlFermata.node(), "type", "inverted")) {
            fermata->SetForm(fermataVis_FORM_inv);
            fermata->SetPlace(STAFFREL_below);
        }
        else if (HasAttributeWithValue(xmlFermata.node(), "type", "upright")) {
            fermata->SetForm(fermataVis_FORM_norm);
            fermata->SetPlace(STAFFREL_above);
        }
    }

    // mordent
    pugi::xpath_node xmlMordent = notations.node().select_node("ornaments/mordent");
    if (xmlMordent) {
        Mordent *mordent = new Mordent();
        m_controlElements.push_back(std::make_pair(measureNum, mordent));
        mordent->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        mordent->SetStartid(m_ID);
        // color
        mordent->SetColor(xmlMordent.node().attribute("color").as_string());
        // form
        mordent->SetForm(mordentLog_FORM_lower);
        // long
        mordent->SetLong(ConvertWordToBool(xmlMordent.node().attribute("long").as_string()));
        // place
        mordent->SetPlace(mordent->AttPlacement::StrToStaffrel(xmlMordent.node().attribute("placement").as_string()));
    }
    pugi::xpath_node xmlMordentInv = notations.node().select_node("ornaments/inverted-mordent");
    if (xmlMordentInv) {
        Mordent *mordent = new Mordent();
        m_controlElements.push_back(std::make_pair(measureNum, mordent));
        mordent->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        mordent->SetStartid(m_ID);
        // color
        mordent->SetColor(xmlMordentInv.node().attribute("color").as_string());
        // form
        mordent->SetForm(mordentLog_FORM_upper);
        // long
        mordent->SetLong(ConvertWordToBool(xmlMordentInv.node().attribute("long").as_string()));
        // place
        mordent->SetPlace(
            mordent->AttPlacement::StrToStaffrel(xmlMordentInv.node().attribute("placement").as_string()));
    }

    // trill
    pugi::xpath_node xmlTrill = notations.node().select_node("ornaments/trill-mark");
    if (xmlTrill) {
        Trill *trill = new Trill();
        m_controlElements.push_back(std::make_pair(measureNum, trill));
        trill->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        trill->SetStartid(m_ID);
        // color
        trill->SetColor(xmlTrill.node().attribute("color").as_string());
        // place
        trill->SetPlace(trill->AttPlacement::StrToStaffrel(xmlTrill.node().attribute("placement").as_string()));
    }

    // turn
    pugi::xpath_node xmlTurn = notations.node().select_node("ornaments/turn");
    if (xmlTurn) {
        Turn *turn = new Turn();
        m_controlElements.push_back(std::make_pair(measureNum, turn));
        turn->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        turn->SetStartid(m_ID);
        // color
        turn->SetColor(xmlTurn.node().attribute("color").as_string());
        // form
        turn->SetForm(turnLog_FORM_upper);
        // place
        turn->SetPlace(turn->AttPlacement::StrToStaffrel(xmlTurn.node().attribute("placement").as_string()));
    }
    pugi::xpath_node xmlTurnInv = notations.node().select_node("ornaments/inverted-turn");
    if (xmlTurnInv) {
        Turn *turn = new Turn();
        m_controlElements.push_back(std::make_pair(measureNum, turn));
        turn->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        turn->SetStartid(m_ID);
        // color
        turn->SetColor(xmlTurnInv.node().attribute("color").as_string());
        // form
        turn->SetForm(turnLog_FORM_lower);
        // place
        turn->SetPlace(turn->AttPlacement::StrToStaffrel(xmlTurnInv.node().attribute("placement").as_string()));
    }
    pugi::xpath_node xmlDelayedTurn = notations.node().select_node("ornaments/delayed-turn");
    if (xmlDelayedTurn) {
        Turn *turn = new Turn();
        m_controlElements.push_back(std::make_pair(measureNum, turn));
        turn->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        turn->SetTstamp((double)(onset + duration / 2) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
        // delayed attribute
        turn->SetDelayed(BOOLEAN_true);
        // color
        turn->SetColor(xmlTurn.node().attribute("color").as_string());
        // form
        turn->SetForm(turnLog_FORM_upper);
        // place
        turn->SetPlace(turn->AttPlacement::StrToStaffrel(xmlTurn.node().attribute("placement").as_string()));
    }
    pugi::xpath_node xmlDelayedTurnInv = notations.node().select_node("ornaments/delayed-inverted-turn");
    if (xmlDelayedTurnInv) {
        Turn *turn = new Turn();
        m_controlElements.push_back(std::make_pair(measureNum, turn));
        turn->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        turn->SetTstamp((double)(onset + duration / 2) * (double)m_meterUnit / (double)(4 * m_ppq) + 1.0);
        // delayed attribute
        turn->SetDelayed(BOOLEAN_true);
        // color
        turn->SetColor(xmlTurnInv.node().attribute("color").as_string());
        // form
        turn->SetForm(turnLog_FORM_lower);
        // place
        turn->SetPlace(turn->AttPlacement::StrToStaffrel(xmlTurnInv.node().attribute("placement").as_string()));
    }

    // arpeggio
    pugi::xpath_node xmlArpeggiate = notations.node().select_node("arpeggiate");
    if (xmlArpeggiate) {
        int arpegN = xmlArpeggiate.node().attribute("number").as_int();
        arpegN = (arpegN < 1) ? 1 : arpegN;
        std::string direction = xmlArpeggiate.node().attribute("direction").as_string();
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
                if (direction == "up")
                    arpeggio->SetOrder(arpegLog_ORDER_up);
                else if (direction == "down")
                    arpeggio->SetOrder(arpegLog_ORDER_down);
                else
                    arpeggio->SetOrder(arpegLog_ORDER_NONE);
            }
            m_ArpeggioStack.push_back(std::make_pair(arpeggio, musicxml::OpenArpeggio(arpegN, onset)));
            m_controlElements.push_back(std::make_pair(measureNum, arpeggio));
        }
    }

    // slur
    pugi::xpath_node_set slurs = notations.node().select_nodes("slur");
    for (pugi::xpath_node_set::const_iterator it = slurs.begin(); it != slurs.end(); ++it) {
        pugi::xml_node slur = it->node();
        int slurNumber = slur.attribute("number").as_int();
        slurNumber = (slurNumber < 1) ? 1 : slurNumber;
        if (HasAttributeWithValue(slur, "type", "start")) {
            Slur *meiSlur = new Slur();
            // color
            meiSlur->SetColor(slur.attribute("color").as_string());
            // lineform
            meiSlur->SetLform(meiSlur->AttCurveRend::StrToLineform(slur.attribute("line-type").as_string()));
            // placement and orientation
            meiSlur->SetCurvedir(ConvertOrientationToCurvedir(slur.attribute("orientation").as_string()));
            meiSlur->SetCurvedir(
                meiSlur->AttCurvature::StrToCurvatureCurvedir(slur.attribute("placement").as_string()));
            // add it to the stack
            m_controlElements.push_back(std::make_pair(measureNum, meiSlur));
            OpenSlur(measure, slurNumber, meiSlur);
        }
        else if (HasAttributeWithValue(slur, "type", "stop")) {
            CloseSlur(measure, slurNumber, element);
        }
    }

    // tremolo end
    if (tremolo) {
        if (HasAttributeWithValue(tremolo.node(), "type", "single")) {
            RemoveLastFromStack(BTREM);
        }
        if (HasAttributeWithValue(tremolo.node(), "type", "stop")) {
            RemoveLastFromStack(FTREM);
        }
    }

    // tuplet end
    pugi::xpath_node tupletEnd = notations.node().select_node("tuplet[@type='stop']");
    if (tupletEnd) {
        RemoveLastFromStack(TUPLET);
    }

    // beam end
    bool beamEnd = node.select_node("beam[@number='1'][text()='end']");
    if (beamEnd) {
        RemoveLastFromStack(BEAM);
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
            if (!(iter->first)->HasStaff()) {
                iter->first->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            }
        }
    }
    if (!m_tempoStack.empty()) {
        std::vector<Tempo *>::iterator iter;
        for (iter = m_tempoStack.begin(); iter != m_tempoStack.end(); ++iter) {
            (*iter)->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
            (*iter)->SetStartid(m_ID);
        }
        m_tempoStack.clear();
    }
    // add staff to hairpins
    if (!m_hairpinStack.empty()) {
        std::vector<std::pair<Hairpin *, musicxml::OpenSpanner> >::iterator iter;
        for (iter = m_hairpinStack.begin(); iter != m_hairpinStack.end(); ++iter) {
            if (!iter->first->HasStaff())
                iter->first->SetStaff(staff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(staff->GetN())));
        }
    }
    if (!m_hairpinStopStack.empty()) {
        std::vector<std::tuple<int, double, musicxml::OpenSpanner> >::iterator iter;
        for (iter = m_hairpinStopStack.begin(); iter != m_hairpinStopStack.end(); ++iter) {
            if (std::get<0>(*iter) == 0) std::get<0>(*iter) = staff->GetN();
        }
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

//////////////////////////////////////////////////////////////////////////////
// String to attribute converters

data_ACCIDENTAL_WRITTEN MusicXmlInput::ConvertAccidentalToAccid(std::string value)
{
    if (value == "sharp")
        return ACCIDENTAL_WRITTEN_s;
    else if (value == "natural")
        return ACCIDENTAL_WRITTEN_n;
    else if (value == "flat")
        return ACCIDENTAL_WRITTEN_f;
    else if (value == "double-sharp")
        return ACCIDENTAL_WRITTEN_x;
    else if (value == "sharp-sharp")
        return ACCIDENTAL_WRITTEN_ss;
    else if (value == "flat-flat")
        return ACCIDENTAL_WRITTEN_ff;
    else if (value == "natural-sharp")
        return ACCIDENTAL_WRITTEN_ns;
    else if (value == "natural-flat")
        return ACCIDENTAL_WRITTEN_nf;
    else if (value == "quarter-flat")
        return ACCIDENTAL_WRITTEN_1qf;
    else if (value == "quarter-sharp")
        return ACCIDENTAL_WRITTEN_1qs;
    else if (value == "three-quarters-flat")
        return ACCIDENTAL_WRITTEN_3qf;
    else if (value == "three-quarters-sharp")
        return ACCIDENTAL_WRITTEN_3qs;
    else if (value == "sharp-down")
        return ACCIDENTAL_WRITTEN_sd;
    else if (value == "sharp-up")
        return ACCIDENTAL_WRITTEN_su;
    else if (value == "natural-down")
        return ACCIDENTAL_WRITTEN_nd;
    else if (value == "natural-up")
        return ACCIDENTAL_WRITTEN_nu;
    else if (value == "flat-down")
        return ACCIDENTAL_WRITTEN_fd;
    else if (value == "flat-up")
        return ACCIDENTAL_WRITTEN_fu;
    else if (value == "triple-sharp")
        return ACCIDENTAL_WRITTEN_ts;
    else if (value == "triple-flat")
        return ACCIDENTAL_WRITTEN_tf;
    LogWarning("MusicXML import: Unsupported accidental value '%s'", value.c_str());
    return ACCIDENTAL_WRITTEN_NONE;
}

data_ACCIDENTAL_GESTURAL MusicXmlInput::ConvertAlterToAccid(float value)
{
    if (value == -2) return ACCIDENTAL_GESTURAL_ff;
    if (value == -1.5) return ACCIDENTAL_GESTURAL_fd;
    if (value == -1) return ACCIDENTAL_GESTURAL_f;
    if (value == -0.5) return ACCIDENTAL_GESTURAL_fu;
    if (value == 0) return ACCIDENTAL_GESTURAL_n;
    if (value == 0.5) return ACCIDENTAL_GESTURAL_sd;
    if (value == 1) return ACCIDENTAL_GESTURAL_s;
    if (value == 1.5) return ACCIDENTAL_GESTURAL_su;
    if (value == 2) return ACCIDENTAL_GESTURAL_ss;
    LogWarning("MusicXML import: Unsupported alter value '%.1f'", value);
    return ACCIDENTAL_GESTURAL_NONE;
}

data_BARRENDITION MusicXmlInput::ConvertStyleToRend(std::string value, bool repeat)
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

data_BOOLEAN MusicXmlInput::ConvertWordToBool(std::string value)
{
    if (value == "yes")
        return BOOLEAN_true;
    else if (value == "no")
        return BOOLEAN_false;
    else
        return BOOLEAN_NONE;
}

data_DURATION MusicXmlInput::ConvertTypeToDur(std::string value)
{
    if (value == "maxima")
        return DURATION_maxima; // this is a mensural MEI value
    else if (value == "long")
        return DURATION_long; // mensural MEI value longa isn't supported
    else if (value == "breve")
        return DURATION_breve;
    else if (value == "whole")
        return DURATION_1;
    else if (value == "half")
        return DURATION_2;
    else if (value == "quarter")
        return DURATION_4;
    else if (value == "eighth")
        return DURATION_8;
    else if (value == "16th")
        return DURATION_16;
    else if (value == "32nd")
        return DURATION_32;
    else if (value == "64th")
        return DURATION_64;
    else if (value == "128th")
        return DURATION_128;
    else if (value == "256th")
        return DURATION_256;
    else {
        LogWarning("MusicXML import: Unsupported type '%s'", value.c_str());
        return DURATION_NONE;
    }
}

std::wstring MusicXmlInput::ConvertTypeToVerovioText(std::string value)
{
    if (value == "breve")
        return L"\xE1D1";
    else if (value == "whole")
        return L"\xE1D2";
    else if (value == "half")
        return L"\xE1D3";
    else if (value == "quarter")
        return L"\xE1D5";
    else if (value == "eighth")
        return L"\xE1D7";
    else if (value == "16th")
        return L"\xE1D9";
    else if (value == "32nd")
        return L"\xE1DB";
    else if (value == "64th")
        return L"\xE1DD";
    else if (value == "128th")
        return L"\xE1DF";
    else if (value == "256th")
        return L"\xE1E1";
    else if (value == "512th")
        return L"\xE1E3";
    else if (value == "1024th")
        return L"\xE1E5";
    else {
        LogWarning("MusicXML import: Unsupported type '%s'", value.c_str());
        return L"";
    }
}

data_PITCHNAME MusicXmlInput::ConvertStepToPitchName(std::string value)
{
    if (value == "C")
        return PITCHNAME_c;
    else if (value == "D")
        return PITCHNAME_d;
    else if (value == "E")
        return PITCHNAME_e;
    else if (value == "F")
        return PITCHNAME_f;
    else if (value == "G")
        return PITCHNAME_g;
    else if (value == "A")
        return PITCHNAME_a;
    else if (value == "B")
        return PITCHNAME_b;
    else {
        LogWarning("MusicXML import: Unsupported pitch name '%s'", value.c_str());
        return PITCHNAME_NONE;
    }
}

curvature_CURVEDIR MusicXmlInput::ConvertOrientationToCurvedir(std::string value)
{
    if (value == "over")
        return curvature_CURVEDIR_above;
    else if (value == "under")
        return curvature_CURVEDIR_below;
    else
        return curvature_CURVEDIR_NONE;
}

fermataVis_SHAPE MusicXmlInput::ConvertFermataShape(std::string value)
{
    if (value == "normal")
        return fermataVis_SHAPE_curved;
    else if (value == "angled")
        return fermataVis_SHAPE_angular;
    else if (value == "square")
        return fermataVis_SHAPE_square;
    else
        return fermataVis_SHAPE_NONE;
}

pedalLog_DIR MusicXmlInput::ConvertPedalTypeToDir(std::string value)
{
    if (value == "start")
        return pedalLog_DIR_down;
    else if (value == "stop")
        return pedalLog_DIR_up;
    else {
        LogWarning("MusicXML import: Unsupported type '%s' for pedal", value.c_str());
        return pedalLog_DIR_NONE;
    }
}

tupletVis_NUMFORMAT MusicXmlInput::ConvertTupletNumberValue(std::string value)
{
    if (value == "actual")
        return tupletVis_NUMFORMAT_count;
    else if (value == "both")
        return tupletVis_NUMFORMAT_ratio;
    else
        return tupletVis_NUMFORMAT_NONE;
}

std::string MusicXmlInput::ConvertAlterToSymbol(std::string value)
{
    if (value == "-2")
        return "𝄫";
    else if (value == "-1")
        return "♭";
    else if (value == "0")
        return "♮";
    else if (value == "1")
        return "♯";
    else if (value == "2")
        return "𝄪";
    else
        return "";
}

std::string MusicXmlInput::ConvertKindToSymbol(std::string value)
{
    if (value == "major")
        return ""; // Use no symbol to avoid ambiguity of "C△".
    else if (value == "minor")
        return "-";
    else if (value == "augmented")
        return "+";
    else if (value == "diminished")
        return "°";
    else if (value == "dominant")
        return "7";
    else if (value == "major-seventh")
        return "△7";
    else if (value == "minor-seventh")
        return "-7";
    else if (value == "diminished-seventh")
        return "°7";
    else if (value == "augmented-seventh")
        return "+7";
    else if (value == "half-diminished")
        return "ø";
    else if (value == "major-minor")
        return "-△7";
    else if (value == "major-sixth")
        return "6";
    else if (value == "minor-sixth")
        return "-6";
    else if (value == "dominant-ninth")
        return "9";
    else if (value == "major-ninth")
        return "△9";
    else if (value == "minor-ninth")
        return "-9";
    else if (value == "dominant-11th")
        return "11";
    else if (value == "major-11th")
        return "△11";
    else if (value == "minor-11th")
        return "-11";
    else if (value == "dominant-13th")
        return "13";
    else if (value == "major-13th")
        return "△13";
    else if (value == "minor-13th")
        return "-13";
    else if (value == "suspended-second")
        return "sus2";
    else if (value == "suspended-fourth")
        return "sus4";
    // Skipping "functional sixths": Neapolitan, Italian, French, German.
    // Skipping pedal (pedal-point bass)
    else if (value == "power")
        return "5";
    // Skipping Tristan
    else
        return "";
}

std::string MusicXmlInput::ConvertKindToText(std::string value)
{
    if (value == "major")
        return "";
    else if (value == "minor")
        return "m";
    else if (value == "augmented")
        return "aug";
    else if (value == "diminished")
        return "dim";
    else if (value == "dominant")
        return "7";
    else if (value == "major-seventh")
        return "Maj7";
    else if (value == "minor-seventh")
        return "m7";
    else if (value == "diminished-seventh")
        return "dim7";
    else if (value == "augmented-seventh")
        return "aug7";
    else if (value == "half-diminished")
        return "m7♭5";
    else if (value == "major-minor")
        return "mMaj7";
    else if (value == "major-sixth")
        return "6";
    else if (value == "minor-sixth")
        return "m6";
    else if (value == "dominant-ninth")
        return "9";
    else if (value == "major-ninth")
        return "Maj9";
    else if (value == "minor-ninth")
        return "m9";
    else if (value == "dominant-11th")
        return "11";
    else if (value == "major-11th")
        return "Maj11";
    else if (value == "minor-11th")
        return "m11";
    else if (value == "dominant-13th")
        return "13";
    else if (value == "major-13th")
        return "Maj13";
    else if (value == "minor-13th")
        return "m13";
    else if (value == "suspended-second")
        return "sus2";
    else if (value == "suspended-fourth")
        return "sus4";
    // Skipping "functional sixths": Neapolitan, Italian, French, German.
    // Skipping pedal (pedal-point bass)
    else if (value == "power")
        return "5";
    // Skipping Tristan
    else
        return "";
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

} // namespace vrv
