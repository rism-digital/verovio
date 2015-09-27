/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.h
// Author:      Laurent Pugin
// Created:     22/09/2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_IOMUSXML_H__
#define __VRV_IOMUSXML_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "attdef.h"
#include "io.h"
#include "pugixml.hpp"
#include "vrvdef.h"

namespace vrv {

//class Barline;
//class Beam;
//class Clef;
//class Doc;
class Layer;
class LayerElement;
class Measure;
//class MeterSig;
//class MRest;
//class MultiRest;
//class Note;
//class Page;
//class Rest;
class Slur;
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
    std::string GetContentOfChild(pugi::xml_node node, std::string child);
    
    void AddMeasure(System *system, Measure *measure, int i);
    void AddLayerElement(Layer *layer, LayerElement *element);
    Layer *SelectLayer(pugi::xml_node node, Measure *measure);
    void RemoveLastFromStack(ClassId classId);
    
    data_ACCIDENTAL_EXPLICIT ConvertAccidentalToAccid(std::string value);
    data_ACCIDENTAL_EXPLICIT ConvertAlterToAccid(std::string value);
    data_DURATION ConvertTypeToDur(std::string value);
    data_PITCHNAME ConvertStepToPitchName(std::string value);
    
private:
    std::string m_filename;
    
    std::vector<LayerElement*> m_elementStack;
    
    std::vector<std::pair<Slur*, int> > m_slurStack;
};

} // namespace vrv {

#endif // __VRV_IOMUSXML_H__
