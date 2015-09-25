/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.h
// Author:      Rodolfo Zitellini
// Created:     10/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_IOMUSXML_H__
#define __VRV_IOMUSXML_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "io.h"
#include "pugixml.hpp"

namespace vrv {

//class Barline;
//class Beam;
//class Clef;
//class Doc;
//class Layer;
//class LayerElement;
class Measure;
//class MeterSig;
//class MRest;
//class MultiRest;
//class Note;
//class Page;
//class Rest;
//class Staff;
class StaffGrp;
class System;
///class Tuplet;


//----------------------------------------------------------------------------
// This class is not up-to-date
//----------------------------------------------------------------------------

class XmlInput: public FileInputStream
{
public:
    // constructors and destructors
    XmlInput(Doc *doc, std::string filename);
    virtual ~XmlInput();
    
    virtual bool ImportFile( );
    virtual bool ImportString(std::string musicxml);
    
private:
    bool ReadXml(pugi::xml_node root);
    
    bool ReadXmlPart(pugi::xml_node node, System *system);
    bool ReadXmlMeasure(pugi::xml_node node, Measure *measure);

    bool ReadXmlPartAttributesAsStaffDef(pugi::xml_node node, StaffGrp *staffGrp);
    
    bool HasAttributeWithValue(pugi::xml_node node, std::string attribute, std::string value);
    bool IsElement(pugi::xml_node node, std::string name);
    bool HasContent(pugi::xml_node node, std::string content);
    
private:
    std::string m_filename;
    /** The parts are read as a vector of systems */
    std::vector<System*> m_parts;
};

} // namespace vrv {

#endif // __VRV_IOMUSXML_H__
