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

class FloatingElement;
class Layer;
class LayerElement;
class Measure;
class Slur;
class StaffGrp;
class System;
class Tie;
///class Tuplet;
    
//----------------------------------------------------------------------------
// namespace for local MusicXml classes
//----------------------------------------------------------------------------
    
namespace musicxml {

class OpenTie
{
public:
    OpenTie(int staffN, int layerN, data_PITCHNAME pname, char oct) {
        m_staffN = staffN;
        m_layerN = layerN;
        m_pname = pname;
        m_oct = oct;
    }
    
    int m_staffN;
    int m_layerN;
    data_PITCHNAME m_pname;
    char m_oct;
};
    
class OpenSlur
{
public:
    OpenSlur(int staffN, int layerN, int number) {
        m_staffN = staffN;
        m_layerN = layerN;
        m_number = number;
    }
    
    int m_staffN;
    int m_layerN;
    int m_number;
};
    
} // namespace musicxml

//----------------------------------------------------------------------------
// MusicXmlInput
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
    
    void ReadMusicXmlAttributes(pugi::xml_node, Measure *measure, int measureNb);
    void ReadMusicXmlBackup(pugi::xml_node, Measure *measure, int measureNb);
    void ReadMusicXmlBarline(pugi::xml_node, Measure *measure, int measureNb);
    void ReadMusicXmlForward(pugi::xml_node, Measure *measure, int measureNb);
    void ReadMusicXmlNote(pugi::xml_node, Measure *measure, int measureNb);
    
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
    
    /**
     * @name Methods for openning and closing tie and slurs.
     * Openned ties and slurs are stacks together with a musicxml::OpenTie
     * and musicxml::OpenSlur objects.
     * For now: only slur starting and ending on the same staff/voice are
     * supported
     */
    ///@{
    void OpenTie(Staff *staff, Layer *layer, Note *note, Tie *tie);
    void CloseTie(Staff *staff, Layer *layer, Note *note, bool isClosingTie);
    void OpenSlur(Staff *staff, Layer *layer, int number, LayerElement *element, Slur *slur);
    void CloseSlur(Staff *staff, Layer *layer, int number, LayerElement *element);
    
    /**
     * @name Methods for converting MusicXML string values to MEI attributes.
     */
    ///@{
    data_ACCIDENTAL_EXPLICIT ConvertAccidentalToAccid(std::string value);
    data_ACCIDENTAL_EXPLICIT ConvertAlterToAccid(std::string value);
    data_DURATION ConvertTypeToDur(std::string value);
    data_PITCHNAME ConvertStepToPitchName(std::string value);
    ///@}
    
private:
    std::string m_filename;
    
    /** The stack for piling open LayerElements (beams, tuplets, chords, etc.)  */
    std::vector<LayerElement*> m_elementStack;
    /** The stack for open slurs */
    std::vector<std::pair<Slur*, musicxml::OpenSlur> > m_slurStack;
    /** The stack for open ties */
    std::vector<std::pair<Tie*, musicxml::OpenTie> > m_tieStack;
    
    /** 
     * The stack of floating elements (tie, slur, etc.) to be added at the
     * end of each measure
     */
    std::vector<std::pair<int, FloatingElement*> > m_floatingElements;
};

} // namespace vrv {

#endif // __VRV_IOMUSXML_H__
