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

class MusicXmlInput: public FileInputStream
{
public:
    // constructors and destructors
    MusicXmlInput(Doc *doc, std::string filename);
    virtual ~MusicXmlInput();
    
    virtual bool ImportFile( );
    virtual bool ImportString(std::string musicxml);
    
private:
    bool ReadMusicXml(pugi::xml_node root);
    
    bool ReadMusicXmlPart(pugi::xml_node node, System *system,  int nbStaves, int staffOffset);
    bool ReadMusicXmlMeasure(pugi::xml_node node, Measure *measure, int nbStaves, int staffOffset);

    /** return the number of staves in the part */
    int ReadMusicXmlPartAttributesAsStaffDef(pugi::xml_node node, StaffGrp *staffGrp, int staffOffset);
    
    void ReadMusicXmlAttributes(pugi::xml_node, Measure *measure);
    void ReadMusicXmlBackup(pugi::xml_node, Measure *measure);
    void ReadMusicXmlBarline(pugi::xml_node, Measure *measure);
    void ReadMusicXmlForward(pugi::xml_node, Measure *measure);
    void ReadMusicXmlNote(pugi::xml_node, Measure *measure);
    
    bool HasAttributeWithValue(pugi::xml_node node, std::string attribute, std::string value);
    bool IsElement(pugi::xml_node node, std::string name);
    bool HasContentWithValue(pugi::xml_node node, std::string value);
    bool HasContent(pugi::xml_node);
    
    std::string GetAttributeValue(pugi::xml_node node, std::string attribute);
    std::string GetContent(pugi::xml_node node);
    
    void AddMeasure(System *system, Measure *measure, int i);
    
private:
    std::string m_filename;
};

} // namespace vrv {

#endif // __VRV_IOMUSXML_H__
