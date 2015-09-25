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
//#include "keysig.h"
#include "measure.h"
//#include "mensur.h"
//#include "metersig.h"
//#include "note.h"
//#include "rest.h"
#include "system.h"
//#include "tie.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// XmlInput
//----------------------------------------------------------------------------

XmlInput::XmlInput(Doc *doc, std::string filename) :
// This is pretty bad. We open a bad fileoutputstream as we don't use it
    FileInputStream(doc)
{
    m_filename = filename;

}

XmlInput::~XmlInput()
{
}
    
bool XmlInput::ImportFile()
{
    try {
        m_doc->Reset(Raw);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(m_filename.c_str());
        if (!result)
        {
            return false;
        }
        pugi::xml_node root = doc.first_child();
        return ReadXml(root);
    }
    catch(char * str) {
        LogError("%s", str);
        return false;
    }
}

bool XmlInput::ImportString(const std::string musicxml)
{
    try {
        m_doc->Reset( Raw );
        pugi::xml_document doc;
        doc.load(musicxml.c_str());
        pugi::xml_node root = doc.first_child();
        return ReadXml(root);
    }
    catch(char * str) {
        LogError("%s", str);
        return false;
    }
}
    
bool XmlInput::HasAttributeWithValue(pugi::xml_node node, std::string attribute, std::string value)
{
    assert(node);
    
    if (node.attribute(attribute.c_str()) && (std::string(node.attribute(attribute.c_str()).value()) == value)) {
        return true;
    }
    return false;
}

bool XmlInput::IsElement(pugi::xml_node node, std::string name)
{
    assert(node);
    
    if (std::string(node.name()) == name) {
        return true;
    }
    return false;
}
    
bool XmlInput::HasContent(pugi::xml_node node, std::string content)
{
    assert(node);
    
    if (node.text() && (std::string(node.text().as_string()) == content)) {
        return true;
    }
    return false;
}

bool XmlInput::ReadXml(pugi::xml_node root)
{
    assert(root);
    
    std::vector<StaffGrp*> m_staffGrpStack;
    StaffGrp *staffGrp = new StaffGrp();
    m_doc->m_scoreDef.AddStaffGrp(staffGrp);
    m_staffGrpStack.push_back(staffGrp);
    
    pugi::xpath_node_set part_list_children = root.select_nodes("part-list/*");
    for (pugi::xpath_node_set::const_iterator it = part_list_children.begin(); it != part_list_children.end(); ++it)
    {
        pugi::xpath_node xpath_node = *it;
        if (IsElement(xpath_node.node(), "part-group")) {
            if (HasAttributeWithValue(xpath_node.node(), "type", "start")) {
                StaffGrp *staffGrp = new StaffGrp();
                pugi::xpath_node group_symbol = xpath_node.node().select_single_node("group-symbol");
                if (group_symbol) {
                    if (HasContent(group_symbol.node(), "bracket")) staffGrp->SetSymbol(SYMBOL_bracket);
                    else if (HasContent(group_symbol.node(), "brace")) staffGrp->SetSymbol(SYMBOL_brace);
                    else if (HasContent(group_symbol.node(), "line")) staffGrp->SetSymbol(SYMBOL_line);
                }
                m_staffGrpStack.back()->AddStaffGrp(staffGrp);
                m_staffGrpStack.push_back(staffGrp);
            }
            // this is the end of a part-group - we assume each opened part-group to be closed
            else {
                m_staffGrpStack.pop_back();
            }
        }
        else if (IsElement(xpath_node.node(), "score-part")) {
            std::string part = xpath_node.node().attribute("id").as_string();
            std::string xpath = StringFormat("/score-partwise/part[@id='%s']/measure[@number='1']/attributes", part.c_str());
            pugi::xpath_node part_first_measure_attributes = root.select_single_node(xpath.c_str());
            // create the staffDef(s)
            if (part_first_measure_attributes) ReadXmlPartAttributesAsStaffDef(part_first_measure_attributes.node(), m_staffGrpStack.back());
        }
    }
    // here we could check that we have that there is only one staffGrp left in m_staffGrpStack
    
    pugi::xpath_node_set parts = root.select_nodes("part");
    if (parts.size() == 0) {
        LogError("No parts to load");
        return false;
    }
    for (pugi::xpath_node_set::const_iterator it = parts.begin(); it != parts.end(); ++it)
    {
        pugi::xpath_node xmlPart = *it;
        System *system = new System();
        ReadXmlPart(xmlPart.node(), system);
    }
    return false;
}
    
    
bool XmlInput::ReadXmlPartAttributesAsStaffDef(pugi::xml_node node, StaffGrp *staffGrp)
{
    assert(node);
    assert(staffGrp);
    
    // First get the number of staves in the part
    int numberOfStaves = 1;
    pugi::xpath_node staves = node.select_single_node("staves");
    if (staves) {
        if (staves.node().text()) {
            int values = atoi(staves.node().text().as_string());
            numberOfStaves = (values > 0) ? values : 1;
        }
    }
    
    int i;
    std::string xpath;
    // Create as many staffDef
    for (i = 0; i < numberOfStaves; i++) {
        StaffDef *staffDef = new StaffDef();
        // clef sign - first look if we have a clef-sign with the corresponding staff @number
        pugi::xpath_node clef_sign;
        xpath = StringFormat("clef[@number='%d']/sign", i+1);
        clef_sign = node.select_single_node(xpath.c_str());
        // if not, look at a common one
        if (!clef_sign) {
            clef_sign = node.select_single_node("clef/sign");
        }
        if (clef_sign && clef_sign.node().text()) {
            staffDef->SetClefShape(staffDef->AttCleffingLog::StrToClefShape(clef_sign.node().text().as_string()));
        }
        // clef line
        pugi::xpath_node clef_line;
        xpath = StringFormat("clef[@number='%d']/line", i+1);
        clef_line = node.select_single_node(xpath.c_str());
        if (!clef_line) {
            clef_line = node.select_single_node("clef/line");
        }
        if (clef_line && clef_line.node().text()) {
            staffDef->SetClefLine(staffDef->AttCleffingLog::StrToInt(clef_line.node().text().as_string()));
        }
        // key sig
        pugi::xpath_node key_fifths;
        xpath = StringFormat("key[@number='%d']/fifths", i+1);
        key_fifths = node.select_single_node(xpath.c_str());
        if (!key_fifths) {
            key_fifths = node.select_single_node("key/fifths");
        }
        if (key_fifths && key_fifths.node().text()) {
            int key = atoi(key_fifths.node().text().as_string());
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
            if (symbol && symbol.node().text()) {
                staffDef->SetMeterSym(staffDef->AttMeterSigDefaultVis::StrToMeterSign(symbol.node().text().as_string()));
            }
            pugi::xpath_node beats = time.node().select_single_node("beats");
            if (beats && beats.node().text()) {
                staffDef->SetMeterCount(staffDef->AttMeterSigDefaultLog::StrToInt(beats.node().text().as_string()));
            }
            pugi::xpath_node beat_type = time.node().select_single_node("beat-type");
            if (beat_type && beat_type.node().text()) {
                staffDef->SetMeterUnit(staffDef->AttMeterSigDefaultLog::StrToInt(beat_type.node().text().as_string()));
            }
        }
        staffGrp->AddStaffDef(staffDef);
    }
}
    
bool XmlInput::ReadXmlPart(pugi::xml_node node, System *system)
{
    pugi::xpath_node_set measures = node.select_nodes("measure");
    if (measures.size() == 0) {
        LogWarning("No measure to load");
        return false;
    }
    for (pugi::xpath_node_set::const_iterator it = measures.begin(); it != measures.end(); ++it)
    {
        pugi::xpath_node xmlMeasure = *it;
        Measure *measure = new Measure();
        ReadXmlMeasure(xmlMeasure.node(), measure);
    }
    return false;
}
    
    bool XmlInput::ReadXmlMeasure(pugi::xml_node node, Measure *measure)
    {
        LogDebug("Reading measure!");
        return false;
    }

} // namespace vrv