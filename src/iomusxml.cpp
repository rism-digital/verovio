/////////////////////////////////////////////////////////////////////////////
// Name:        musiomusxml.cpp
// Author:      Rodolfo Zitellini
// Created:     10/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iomusxml.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>

//----------------------------------------------------------------------------

//#include "beam.h"
//#include "clef.h"
#include "doc.h"
#include "layer.h"
#include "measure.h"
#include "page.h"
#include "staff.h"
#include "system.h"
//#include "tie.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MusicXmlInput
//----------------------------------------------------------------------------

MusicXmlInput::MusicXmlInput(Doc *doc, std::string filename) :
// This is pretty bad. We open a bad fileoutputstream as we don't use it
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
    
    pugi::xpath_node_set partListChildren = root.select_nodes("part-list/*");
    for (pugi::xpath_node_set::const_iterator it = partListChildren.begin(); it != partListChildren.end(); ++it)
    {
        pugi::xpath_node xpathNode = *it;
        if (IsElement(xpathNode.node(), "part-group")) {
            if (HasAttributeWithValue(xpathNode.node(), "type", "start")) {
                StaffGrp *staffGrp = new StaffGrp();
                pugi::xpath_node groupGymbol = xpathNode.node().select_single_node("group-symbol");
                if (groupGymbol) {
                    if (HasContentWithValue(groupGymbol.node(), "bracket")) staffGrp->SetSymbol(SYMBOL_bracket);
                    else if (HasContentWithValue(groupGymbol.node(), "brace")) staffGrp->SetSymbol(SYMBOL_brace);
                    else if (HasContentWithValue(groupGymbol.node(), "line")) staffGrp->SetSymbol(SYMBOL_line);
                }
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
            std::string xpath = StringFormat("/score-partwise/part[@id='%s']/measure[@number='1']/attributes", partId.c_str());
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
    
    page->AddSystem( system );
    m_doc->AddPage( page );
    
    return true;
}
    
    
int MusicXmlInput::ReadMusicXmlPartAttributesAsStaffDef(pugi::xml_node node, StaffGrp *staffGrp, int staffOffset)
{
    assert(node);
    assert(staffGrp);
    
    // First get the number of staves in the part
    int nbStaves = 1;
    pugi::xpath_node staves = node.select_single_node("staves");
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
        clefSign = node.select_single_node(xpath.c_str());
        // if not, look at a common one
        if (!clefSign) {
            clefSign = node.select_single_node("clef/sign");
        }
        if (clefSign && HasContent(clefSign.node())) {
            staffDef->SetClefShape(staffDef->AttCleffingLog::StrToClefShape(GetContent(clefSign.node())));
        }
        // clef line
        pugi::xpath_node clefLine;
        xpath = StringFormat("clef[@number='%d']/line", i+1);
        clefLine = node.select_single_node(xpath.c_str());
        if (!clefLine) {
            clefLine = node.select_single_node("clef/line");
        }
        if (clefLine && HasContent(clefLine.node())) {
            staffDef->SetClefLine(staffDef->AttCleffingLog::StrToInt(clefLine.node().text().as_string()));
        }
        // key sig
        pugi::xpath_node keyFifths;
        xpath = StringFormat("key[@number='%d']/fifths", i+1);
        keyFifths = node.select_single_node(xpath.c_str());
        if (!keyFifths) {
            keyFifths = node.select_single_node("key/fifths");
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
        time = node.select_single_node(xpath.c_str());
        if (!time) {
            time = node.select_single_node("time");
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
    
bool MusicXmlInput::ReadMusicXmlMeasure(pugi::xml_node node, Measure *measure, int nbStaves, int staffOffset)
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

        // read the content of the measure
        for (pugi::xml_node::iterator it = node.begin(); it != node.end(); ++it)
        {
            if (IsElement(*it, "barline")) {
                ReadMusicXmlBarline(*it, measure);
            }
            else if (IsElement(*it, "note")) {
                ReadMusicXmlNote(*it, measure);
            }
        }
        
    }
    
    //LogDebug("Reading measure!");
    return true;
}
    
void MusicXmlInput::ReadMusicXmlAttributes(pugi::xml_node node, Measure *measure)
{
}

void MusicXmlInput::ReadMusicXmlBackup(pugi::xml_node node, Measure *measure)
{
}

void MusicXmlInput::ReadMusicXmlBarline(pugi::xml_node node, Measure *measure)
{
    data_BARRENDITION barRendition = BARRENDITION_NONE;
    pugi::xpath_node barStyle = node.select_single_node("bar-style");
    if (barStyle) {
        if (HasContentWithValue(barStyle.node(), "light-light")) {
            barRendition = BARRENDITION_dbl;
        }
        else if (HasContentWithValue(barStyle.node(), "light-heavy")) {
           barRendition = BARRENDITION_end;
        }
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

void MusicXmlInput::ReadMusicXmlForward(pugi::xml_node node, Measure *measure)
{
}
    
void MusicXmlInput::ReadMusicXmlNote(pugi::xml_node node, Measure *measure)
{
}
    
} // namespace vrv