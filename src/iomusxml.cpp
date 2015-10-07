/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.cpp
// Author:      Laurent Pugin
// Created:     22/09/2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iomusxml.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "att_comparison.h"
#include "beam.h"
#include "chord.h"
#include "doc.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "mrest.h"
#include "page.h"
#include "rest.h"
#include "slur.h"
#include "staff.h"
#include "system.h"
#include "tie.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MusicXmlInput
//----------------------------------------------------------------------------

MusicXmlInput::MusicXmlInput(Doc *doc, std::string filename) :
    FileInputStream(doc)
{
    m_filename = filename;

}

MusicXmlInput::~MusicXmlInput()
{
}
    
bool MusicXmlInput::ImportFile()
{
    try {
        m_doc->Reset(Raw);
        pugi::xml_document xmlDoc;
        pugi::xml_parse_result result = xmlDoc.load_file(m_filename.c_str());
        if (!result)
        {
            return false;
        }
        pugi::xml_node root = xmlDoc.first_child();
        return ReadMusicXml(root);
    }
    catch(char * str) {
        LogError("%s", str);
        return false;
    }
}

bool MusicXmlInput::ImportString(const std::string musicxml)
{
    try {
        m_doc->Reset( Raw );
        pugi::xml_document xmlDoc;
        xmlDoc.load(musicxml.c_str());
        pugi::xml_node root = xmlDoc.first_child();
        return ReadMusicXml(root);
    }
    catch(char * str) {
        LogError("%s", str);
        return false;
    }
}
  
//////////////////////////////////////////////////////////////////////////////
// XML helpers
    
bool MusicXmlInput::HasAttributeWithValue(pugi::xml_node node, std::string attribute, std::string value)
{
    assert(node);
    
    if (GetAttributeValue(node, attribute) == value) {
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
    
bool MusicXmlInput::HasContent(pugi::xml_node node)
{
    assert(node);
    
    return (node.text());
}
    
std::string MusicXmlInput::GetAttributeValue(pugi::xml_node node, std::string attribute)
{
    assert(node);
    
    if (node.attribute(attribute.c_str())) {
        return std::string(node.attribute(attribute.c_str()).value());
    }
    return "";
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
    pugi::xpath_node childNode = node.select_single_node(child.c_str());
    if (childNode && HasContent(childNode.node())) {
        return GetContent(childNode.node());
    }
    return "";
    }
    
void MusicXmlInput::AddMeasure(System *system, Measure *measure, int i)
{
    assert(system);
    assert(i >= 0);
    
    // we just need to add a measure
    if (i == system->GetChildCount()) {
        system->AddMeasure(measure);
    }
    // otherwise copy the content to the corresponding existing measure
    else if (system->GetChildCount() > i) {
        Measure *existingMeasure = dynamic_cast<Measure*>(system->m_children[i]);
        assert(existingMeasure);
        ArrayOfObjects::iterator sIter = measure->m_children.begin();
        for (sIter = measure->m_children.begin(); sIter != measure->m_children.end(); sIter++) {
            Staff *staff = dynamic_cast<Staff*>( measure->Relinquish( (*sIter)->GetIdx()) );
            assert(staff);
            existingMeasure->AddStaff(staff);
        }
    }
    // there is a gap, this should not happen
    else {
        LogWarning("measures should be added in the right order");
    }
}
    
void MusicXmlInput::AddLayerElement(Layer *layer, LayerElement *element)
{
    assert(layer);
    assert(element);
    
    if (m_elementStack.empty()) {
        layer->AddLayerElement(element);
    }
    else if (m_elementStack.back()->Is() == CHORD) {
        Chord *chord = dynamic_cast<Chord*>(m_elementStack.back());
        assert(chord);
        chord->AddLayerElement(element);
    }
    else if (m_elementStack.back()->Is() == BEAM) {
        Beam *beam = dynamic_cast<Beam*>(m_elementStack.back());
        assert(beam);
        beam->AddLayerElement(element);
    }
}
    
Layer *MusicXmlInput::SelectLayer(pugi::xml_node node, vrv::Measure *measure)
{
    int staffNb = 1;
    std::string staffNbStr = GetContentOfChild(node, "staff");
    if (!staffNbStr.empty()) {
        staffNb = atoi(staffNbStr.c_str());
    }
    if ((staffNb < 1) || (staffNb > measure->m_children.size())) {
        LogWarning("Staff %d cannot be found", staffNb);
        staffNb = 1;
    }
    staffNb--;
    Staff *staff = dynamic_cast<Staff*>(measure->m_children.at(staffNb));
    assert(staff);
    Layer *layer = dynamic_cast<Layer*>(staff->m_children.at(0));
    return layer;
}
    
void MusicXmlInput::RemoveLastFromStack(ClassId classId)
{
    std::vector<LayerElement*>::reverse_iterator riter;
    for (riter = m_elementStack.rbegin(); riter != m_elementStack.rend(); riter++) {
        if ((*riter)->Is() == classId) {
            m_elementStack.erase((riter+1).base());
            return;
        }
    }
}
    
//////////////////////////////////////////////////////////////////////////////
// Tie and slurs stack management
    
void MusicXmlInput::OpenTie(Staff *staff, Layer *layer, Note *note, Tie *tie)
{
    tie->SetStartid(note->GetUuid());
    musicxml::OpenTie openTie(staff->GetN(), layer->GetN(), note->GetPname(), note->GetOct());
    m_tieStack.push_back(std::make_pair(tie, openTie));
}
    
void MusicXmlInput::CloseTie(Staff *staff, Layer *layer, Note *note, bool isClosingTie)
{
    std::vector<std::pair<Tie*, musicxml::OpenTie> >::iterator iter;
    for (iter = m_tieStack.begin(); iter != m_tieStack.end(); iter++) {
        if ((iter->second.m_staffN == staff->GetN()) && (iter->second.m_layerN == layer->GetN()) &&
            (iter->second.m_pname == note->GetPname()) && iter->second.m_oct == note->GetOct()) {
            iter->first->SetEndid(note->GetUuid());
            m_tieStack.erase(iter);
            if (!isClosingTie) {
                LogWarning("Closing tie for note '%s' even thought tie /tie@[type='stop'] is missing in the MusicXML", note->GetUuid().c_str());
            }
            return;
        }
    }
}
    
void MusicXmlInput::OpenSlur(Staff *staff, Layer *layer, int number, LayerElement *element, Slur *slur)
{
    slur->SetStartid(element->GetUuid());
    musicxml::OpenSlur openSlur(staff->GetN(), layer->GetN(), number);
    m_slurStack.push_back(std::make_pair(slur, openSlur));
}

void MusicXmlInput::CloseSlur(Staff *staff, Layer *layer, int number, LayerElement *element)
{
    std::vector<std::pair<Slur*, musicxml::OpenSlur> >::iterator iter;
    for (iter = m_slurStack.begin(); iter != m_slurStack.end(); iter++) {
        if ((iter->second.m_staffN == staff->GetN()) && (iter->second.m_layerN == layer->GetN()) &&
            (iter->second.m_number == number)) {
            iter->first->SetEndid(element->GetUuid());
            m_slurStack.erase(iter);
            return;
        }
    }
    LogWarning("Closing slur for element '%s' could not be matched", element->GetUuid().c_str());

}
    
//////////////////////////////////////////////////////////////////////////////
// Parsing methods

bool MusicXmlInput::ReadMusicXml(pugi::xml_node root)
{
    assert(root);
    
    std::vector<StaffGrp*> m_staffGrpStack;
    StaffGrp *staffGrp = new StaffGrp();
    m_doc->m_scoreDef.AddStaffGrp(staffGrp);
    m_staffGrpStack.push_back(staffGrp);
    
    int staffOffset = 0;
    Page *page = new Page();
    System *system = new System();
    
    pugi::xpath_node_set partListChildren = root.select_nodes("/score-partwise/part-list/*");
    for (pugi::xpath_node_set::const_iterator it = partListChildren.begin(); it != partListChildren.end(); ++it)
    {
        pugi::xpath_node xpathNode = *it;
        if (IsElement(xpathNode.node(), "part-group")) {
            if (HasAttributeWithValue(xpathNode.node(), "type", "start")) {
                StaffGrp *staffGrp = new StaffGrp();
                // read the group-symbol (MEI @symbol)
                std::string groupGymbol = GetContentOfChild(xpathNode.node(), "group-symbol");
                if (groupGymbol == "bracket") staffGrp->SetSymbol(SYMBOL_bracket);
                else if (groupGymbol == "brace") staffGrp->SetSymbol(SYMBOL_brace);
                else if (groupGymbol == "line") staffGrp->SetSymbol(SYMBOL_line);
                // now stack it
                m_staffGrpStack.back()->AddStaffGrp(staffGrp);
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
            std::string xpath = StringFormat("/score-partwise/part[@id='%s']/measure[@number='1']", partId.c_str());
            pugi::xpath_node partFirstMeasureAttributes = root.select_single_node(xpath.c_str());
            if (!partFirstMeasureAttributes) {
                LogWarning("Could not find the 'attributes' element in the first measure of part '%s'", partId.c_str());
                continue;
            }
            // create the staffDef(s)
            int nbStaves = ReadMusicXmlPartAttributesAsStaffDef(partFirstMeasureAttributes.node(), m_staffGrpStack.back(), staffOffset);
            // find the part and read it
            xpath = StringFormat("/score-partwise/part[@id='%s']", partId.c_str());
            pugi::xpath_node part = root.select_single_node(xpath.c_str());
            if (!part) {
                LogWarning("Could not find the part '%s'", partId.c_str());
                continue;
            }
            ReadMusicXmlPart(part.node(), system, nbStaves, staffOffset);
            // increment the staffOffset for reading the next part
            staffOffset += nbStaves;
        }
    }
    // here we could check that we have that there is only one staffGrp left in m_staffGrpStack
    
    Measure *measure = NULL;
    std::vector<std::pair<int, FloatingElement*> >::iterator iter;
    for(iter = m_floatingElements.begin(); iter != m_floatingElements.end(); iter++) {
        if (!measure || (measure->GetN() != iter->first)) {
            AttCommonNComparison comparisonMeasure(MEASURE, iter->first);
            measure = dynamic_cast<Measure*>(system->FindChildByAttComparison(&comparisonMeasure, 1));
        }
        if (!measure) {
            LogWarning("Element '%s' could not be added to measure '%d'",
                       iter->second->GetClassName().c_str(), iter->first);
            continue;
        }
        measure->AddFloatingElement(iter->second);
    }
    
    //assert(m_tieStack.empty());
    
    page->AddSystem( system );
    m_doc->AddPage( page );
    
    return true;
}
    
    
int MusicXmlInput::ReadMusicXmlPartAttributesAsStaffDef(pugi::xml_node node, StaffGrp *staffGrp, int staffOffset)
{
    assert(node);
    assert(staffGrp);
    
    int nbStaves = 1;
    
    for (pugi::xml_node::iterator it = node.begin(); it != node.end(); ++it)
    {
        // We read all attribute elements until we reach something else
        if (!IsElement(*it, "attributes")) break;
        
        // First get the number of staves in the part
        pugi::xpath_node staves = it->select_single_node("staves");
        if (staves) {
            if (staves.node().text()) {
                int values = atoi(staves.node().text().as_string());
                nbStaves = (values > 0) ? values : 1;
            }
        }
        
        int i;
        std::string xpath;
        // Create as many staffDef
        for (i = 0; i < nbStaves; i++) {
            StaffDef *staffDef = new StaffDef();
            staffDef->SetN(i + 1 + staffOffset);
            // by default five line staves
            staffDef->SetLines(5);
            
            // clef sign - first look if we have a clef-sign with the corresponding staff @number
            pugi::xpath_node clefSign;
            xpath = StringFormat("clef[@number='%d']/sign", i+1);
            clefSign = it->select_single_node(xpath.c_str());
            // if not, look at a common one
            if (!clefSign) {
                clefSign = it->select_single_node("clef/sign");
            }
            if (clefSign && HasContent(clefSign.node())) {
                staffDef->SetClefShape(staffDef->AttCleffingLog::StrToClefShape(GetContent(clefSign.node())));
            }
            // clef line
            pugi::xpath_node clefLine;
            xpath = StringFormat("clef[@number='%d']/line", i+1);
            clefLine = it->select_single_node(xpath.c_str());
            if (!clefLine) {
                clefLine = it->select_single_node("clef/line");
            }
            if (clefLine && HasContent(clefLine.node())) {
                staffDef->SetClefLine(staffDef->AttCleffingLog::StrToInt(clefLine.node().text().as_string()));
            }
            // key sig
            pugi::xpath_node keyFifths;
            xpath = StringFormat("key[@number='%d']/fifths", i+1);
            keyFifths = it->select_single_node(xpath.c_str());
            if (!keyFifths) {
                keyFifths = it->select_single_node("key/fifths");
            }
            if (keyFifths && HasContent(keyFifths.node())) {
                int key = atoi(keyFifths.node().text().as_string());
                std::string value;
                if (key < 0) value = StringFormat("%df", abs(key));
                else value = StringFormat("%ds", key);
                staffDef->SetKeySig(staffDef->AttKeySigDefaultLog::StrToKeySignature(value));
            }
            // time
            pugi::xpath_node time;
            xpath = StringFormat("time[@number='%d']", i+1);
            time = it->select_single_node(xpath.c_str());
            if (!time) {
                time = it->select_single_node("time");
            }
            if (time) {
                pugi::xpath_node symbol = time.node().select_single_node("symbol");
                if (symbol && HasContent(symbol.node())) {
                    staffDef->SetMeterSym(staffDef->AttMeterSigDefaultVis::StrToMeterSign(symbol.node().text().as_string()));
                }
                pugi::xpath_node beats = time.node().select_single_node("beats");
                if (beats && HasContent(beats.node())) {
                    staffDef->SetMeterCount(staffDef->AttMeterSigDefaultLog::StrToInt(beats.node().text().as_string()));
                }
                pugi::xpath_node beatType = time.node().select_single_node("beat-type");
                if (beatType && HasContent(beatType.node())) {
                    staffDef->SetMeterUnit(staffDef->AttMeterSigDefaultLog::StrToInt(beatType.node().text().as_string()));
                }
            }
            staffGrp->AddStaffDef(staffDef);
        }
    }
        
    return nbStaves;
}
    
bool MusicXmlInput::ReadMusicXmlPart(pugi::xml_node node, System *system, int nbStaves, int staffOffset)
{
    assert(node);
    assert(system);
    
    pugi::xpath_node_set measures = node.select_nodes("measure");
    if (measures.size() == 0) {
        LogWarning("No measure to load");
        return false;
    }

    int i = 0;
    for (pugi::xpath_node_set::const_iterator it = measures.begin(); it != measures.end(); ++it)
    {
        pugi::xpath_node xmlMeasure = *it;
        Measure *measure = new Measure();
        ReadMusicXmlMeasure(xmlMeasure.node(), measure, nbStaves, staffOffset);
        // Add the measure to the system - if already there from a previous part will just merge the content
        AddMeasure(system, measure, i);
        i++;
    }
    return false;
}
    
bool MusicXmlInput::ReadMusicXmlMeasure(pugi::xml_node node, Measure *measure,
                                        int nbStaves, int staffOffset)
{
    assert(node);
    assert(measure);
    
    int measureNb = atoi(GetAttributeValue(node, "number").c_str());
    if (measure > 0) measure->SetN(measureNb);
    
    int i = 0;
    for (i = 0; i < nbStaves; i++) {
        // the staff @n must take into account the staffOffset
        Staff *staff = new Staff();
        staff->SetN(i + 1 + staffOffset);
        
        // add at least one layer
        Layer *layer = new Layer();
        layer->SetN(1);
        staff->AddLayer(layer);
        measure->AddStaff(staff);
    }
    
    //Normally the stack should be empty
    //LogDebug("Measure %s", GetAttributeValue(node, "number").c_str());
    //assert(m_elementStack.empty());
    m_elementStack.clear();
    
    // read the content of the measure
    for (pugi::xml_node::iterator it = node.begin(); it != node.end(); ++it) {
        if (IsElement(*it, "barline")) {
            ReadMusicXmlBarline(*it, measure, measureNb);
        }
        else if (IsElement(*it, "note")) {
            ReadMusicXmlNote(*it, measure, measureNb);
        }
    }

    return true;
}
    
void MusicXmlInput::ReadMusicXmlAttributes(pugi::xml_node node, Measure *measure, int measureNb)
{
}

void MusicXmlInput::ReadMusicXmlBackup(pugi::xml_node node, Measure *measure, int measureNb)
{
}

void MusicXmlInput::ReadMusicXmlBarline(pugi::xml_node node, Measure *measure, int measureNb)
{
    data_BARRENDITION barRendition = BARRENDITION_NONE;
    std::string barStyle = GetContentOfChild(node, "bar-style");
    if (!barStyle.empty()) {
        if (barStyle == "light-light") barRendition = BARRENDITION_dbl;
        else if (barStyle ==  "light-heavy") barRendition = BARRENDITION_end;
        // we need to handle more style
        else {
            barRendition = BARRENDITION_single;
        }
    }
    // only left or right can be supported
    if (HasAttributeWithValue(node, "location", "left")) {
        measure->SetLeft(barRendition);
    }
    else {
        measure->SetRight(barRendition);
    }
}

void MusicXmlInput::ReadMusicXmlForward(pugi::xml_node node, Measure *measure, int measureNb)
{
}
    
void MusicXmlInput::ReadMusicXmlNote(pugi::xml_node node, Measure *measure, int measureNb)
{
    assert(node);
    assert(measure);
    
    Layer *layer = SelectLayer(node, measure);
    assert(layer);
    
    Staff *staff = dynamic_cast<Staff*>(layer->GetFirstParent(STAFF));
    assert(staff);
    
    LayerElement *element = NULL;
    
    pugi::xpath_node notations = node.select_single_node("notations");
    
    // duration string and dots
    std::string typeStr = GetContentOfChild(node, "type");
    int dots = (int)node.select_nodes("dot").size();
    
    // beam start
    bool beamStart = node.select_single_node("beam[@number='1'][text()='begin']");
    if (beamStart) {
        Beam *beam = new Beam();
        AddLayerElement(layer, beam);
        m_elementStack.push_back(beam);
    }
    
    if (node.select_single_node("rest")) {
        // we assume /note without /type to be mRest
        if (typeStr.empty()) {
            MRest *mRest = new MRest();
            layer->AddLayerElement(mRest);
        }
        else {
            Rest *rest = new Rest();
            element = rest;
            rest->SetDur(ConvertTypeToDur(typeStr));
            if (dots > 0) rest->SetDots(dots);
            layer->AddLayerElement(rest);
        }
    }
    else {
        Note *note = new Note();
        element = note;
        
        // Accidental
        std::string accidentalStr = GetContentOfChild(node, "accidental");
        if (!accidentalStr.empty()) {
            Accid *accid = new Accid();
            accid->SetAccid(ConvertAccidentalToAccid(accidentalStr));
            note->AddLayerElement(accid);
        }
        
        // Stem direction - taken into account below for the chord or the note
        data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
        std::string stemDirStr = GetContentOfChild(node, "stem");
        if (stemDirStr == "down") stemDir = STEMDIRECTION_down;
        else if (stemDirStr == "up") stemDir = STEMDIRECTION_up;
        
        // Pitch and octave
        pugi::xpath_node pitch = node.select_single_node("pitch");
        if (pitch) {
            std::string stepStr = GetContentOfChild(pitch.node(), "step");
            if (!stepStr.empty()) note->SetPname(ConvertStepToPitchName(stepStr));
            std::string octaveStr = GetContentOfChild(pitch.node(), "octave");
            if (!octaveStr.empty()) note->SetOct(atoi(octaveStr.c_str()));
            std::string alterStr = GetContentOfChild(node, "atler");
            //
            if (accidentalStr.empty() && !alterStr.empty()) {
                // add accid.ges once supported
                //note->SetAccidGes(ConvertAlterToAccid(alterStr));
            }
        }
        
        // Look at the next note to see if we are starting or ending a chord
        pugi::xpath_node nextNote = node.select_single_node("./following-sibling::note");
        bool nextIsChord = false;
        if (nextNote.node().select_single_node("chord")) nextIsChord = true;
        // Create the chord if we are starting a new chord
        if (nextIsChord) {
            if (m_elementStack.empty() || m_elementStack.back()->Is() != CHORD) {
                Chord *chord = new Chord();
                chord->SetDur(ConvertTypeToDur(typeStr));
                if (dots > 0) chord->SetDots(dots);
                chord->SetStemDir(stemDir);
                AddLayerElement(layer, chord);
                m_elementStack.push_back(chord);
            }
        }
        
        // Grace notes
        pugi::xpath_node grace = node.select_single_node("grace");
        if (grace) {
            std::string slashStr = GetAttributeValue(grace.node(), "slash");
            if (slashStr == "yes") note->SetGrace(GRACE_acc);
            else if (slashStr == "no") note->SetGrace(GRACE_unacc);
            else note->SetGrace(GRACE_unknown);
        }
        
        // Set the duration to the note if we are not in a chord
        if (m_elementStack.empty() || m_elementStack.back()->Is() != CHORD) {
            note->SetDur(ConvertTypeToDur(typeStr));
            if (dots > 0) note->SetDots(dots);
            note->SetStemDir(stemDir);
        }
        
        // Ties
        pugi::xpath_node tie1 = node.select_single_node("tie[1]");
        pugi::xpath_node tie2 = node.select_single_node("tie[2]");
        std::string tieStr1, tieStr2;
        if (tie1) tieStr1 = GetAttributeValue(tie1.node(), "type");
        if (tie2) tieStr2 = GetAttributeValue(tie2.node(), "type");
        // First close tie
        bool isClosingTie = ((tieStr1 == "stop") || (tieStr2 == "stop"));
        CloseTie(staff, layer, note, isClosingTie);
        // Then open a new tie
        if ((tieStr1 == "start") || (tieStr2 == "start")) {
            Tie *tie = new Tie();
            m_floatingElements.push_back(std::make_pair(measureNb, tie));
            OpenTie(staff, layer, note, tie);
        }

        // Add the note to the layer or to the current container
        AddLayerElement(layer, note);
        
        // If we are ending a chord remove it from the stack
        if (!nextIsChord) {
            if (!m_elementStack.empty() && m_elementStack.back()->Is() == CHORD) {
                RemoveLastFromStack(CHORD);
            }
        }
    }
    
    // element will be NULL in case of mRest
    if (!element) return;
    
    // slur
    pugi::xpath_node_set slurs = notations.node().select_nodes("slur");
    for (pugi::xpath_node_set::const_iterator it = slurs.begin(); it != slurs.end(); ++it)
    {
        pugi::xml_node slur = it->node();
        int slurNumber = atoi(GetAttributeValue(slur, "number").c_str());
        slurNumber = (slurNumber < 1) ? 1 : slurNumber;
        if (HasAttributeWithValue(slur, "type", "start")) {
            Slur *meiSlur = new Slur();
            // placement
            if (HasAttributeWithValue(slur, "placement", "above")) meiSlur->SetCurvedir(CURVEDIR_above);
            else if (HasAttributeWithValue(slur, "placement", "below")) meiSlur->SetCurvedir(CURVEDIR_below);
            // add it to the stack
            m_floatingElements.push_back(std::make_pair(measureNb, meiSlur));
            OpenSlur(staff, layer, slurNumber, element, meiSlur);
        }
        else if (HasAttributeWithValue(slur, "type", "stop")) {
            CloseSlur(staff, layer, slurNumber, element);
        }
    }
    
    // beam end
    bool beamEnd =  node.select_single_node("beam[@number='1'][text()='end']");
    if (beamEnd) {
        RemoveLastFromStack(BEAM);
    }
}
    
//////////////////////////////////////////////////////////////////////////////
// String to attribute converters
    
data_ACCIDENTAL_EXPLICIT MusicXmlInput::ConvertAccidentalToAccid(std::string value)
{
    if (value == "sharp") return ACCIDENTAL_EXPLICIT_s;
    else if (value == "flat") return ACCIDENTAL_EXPLICIT_f;
    else if (value == "sharp-sharp") return ACCIDENTAL_EXPLICIT_ss;
    else if (value == "double-sharp") return ACCIDENTAL_EXPLICIT_ss;
    else if (value == "flat-flat") return ACCIDENTAL_EXPLICIT_ff;
    else if (value == "double-flat") return ACCIDENTAL_EXPLICIT_ff;
    else if (value == "natural") return ACCIDENTAL_EXPLICIT_n;
    else {
        LogWarning("Unsupported accidental value '%s'", value.c_str() );
    }
    return ACCIDENTAL_EXPLICIT_NONE;
}
    
data_ACCIDENTAL_EXPLICIT MusicXmlInput::ConvertAlterToAccid(std::string value)
{
    if (value == "1") return ACCIDENTAL_EXPLICIT_s;
    else if (value == "-1") return ACCIDENTAL_EXPLICIT_f;
    else if (value == "2") return ACCIDENTAL_EXPLICIT_ss;
    else if (value == "-2") return ACCIDENTAL_EXPLICIT_ff;
    else if (value == "0") return ACCIDENTAL_EXPLICIT_n;
    else {
        LogWarning("Unsupported alter value '%s'", value.c_str() );
    }
    return ACCIDENTAL_EXPLICIT_NONE;
}
    
data_DURATION MusicXmlInput::ConvertTypeToDur(std::string value)
{
    if (value == "maxima") return DURATION_maxima; // this is a mensural MEI value
    else if (value == "longa") return DURATION_long;
    else if (value == "breve") return DURATION_breve;
    else if (value == "whole") return DURATION_1;
    else if (value == "half") return DURATION_2;
    else if (value == "quarter") return DURATION_4;
    else if (value == "eighth") return DURATION_8;
    else if (value == "16th") return DURATION_16;
    else if (value == "32nd") return DURATION_32;
    else if (value == "64th") return DURATION_64;
    else if (value == "128th") return DURATION_128;
    else if (value == "256th") return DURATION_256;
    else {
        LogWarning("Unsupported type '%s'", value.c_str());
    }
    return DURATION_NONE;
}

data_PITCHNAME MusicXmlInput::ConvertStepToPitchName(std::string value)
{
    if (value == "C") return PITCHNAME_c;
    else if (value == "D") return PITCHNAME_d;
    else if (value == "E") return PITCHNAME_e;
    else if (value == "F") return PITCHNAME_f;
    else if (value == "G") return PITCHNAME_g;
    else if (value == "A") return PITCHNAME_a;
    else if (value == "B") return PITCHNAME_b;
    else {
        LogWarning("Unsupported pitch name '%s'", value.c_str() );
    }
    // default
    return PITCHNAME_NONE;
}
        
    
} // namespace vrv