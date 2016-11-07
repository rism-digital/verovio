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
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

namespace vrv {

class ControlElement;
class Layer;
class LayerElement;
class Measure;
class Section;
class Slur;
class StaffGrp;
class Tie;
/// class Tuplet;

//----------------------------------------------------------------------------
// namespace for local MusicXml classes
//----------------------------------------------------------------------------

namespace musicxml {

    class OpenTie {
    public:
        OpenTie(int staffN, int layerN, data_PITCHNAME pname, char oct)
        {
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

    class OpenSlur {
    public:
        OpenSlur(int staffN, int layerN, int number)
        {
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

class MusicXmlInput : public FileInputStream {
public:
    // constructors and destructors
    MusicXmlInput(Doc *doc, std::string filename);
    virtual ~MusicXmlInput();

    virtual bool ImportFile();
    virtual bool ImportString(std::string const &musicxml);

private:
    /**
     * Top level method called from ImportFile or ImportString
     */
    bool ReadMusicXml(pugi::xml_node root);

    /**
     * @name Top level methods for reading MusicXml part and measure elements.
     */
    ///@{
    bool ReadMusicXmlPart(pugi::xml_node node, Section *section, int nbStaves, int staffOffset);
    bool ReadMusicXmlMeasure(pugi::xml_node node, Measure *measure, int nbStaves, int staffOffset);
    ///@}

    /**
     * Methods for reading the first MusicXml attributes element as MEI staffDef.
     * Returns the number of staves in the part.
     */
    int ReadMusicXmlPartAttributesAsStaffDef(pugi::xml_node node, StaffGrp *staffGrp, int staffOffset);

    /**
     * @name Methods for reading the content of a MusicXml measure.
     */
    ///@{
    void ReadMusicXmlAttributes(pugi::xml_node, Measure *measure, int measureNb);
    void ReadMusicXmlBackup(pugi::xml_node, Measure *measure, int measureNb);
    void ReadMusicXmlBarLine(pugi::xml_node, Measure *measure, int measureNb);
    void ReadMusicXmlForward(pugi::xml_node, Measure *measure, int measureNb);
    void ReadMusicXmlNote(pugi::xml_node, Measure *measure, int measureNb);
    ///@}

    /**
     * Add a Measure to the section.
     * If the measure already exists it will move all its content.
     * The measure can contain only staves. Other elements must be stacked on m_floatingElements.
     */
    void AddMeasure(Section *section, Measure *measure, int i);

    /**
     * Add a Layer element to the layer or to the LayerElement at the top of m_elementStack.
     */
    void AddLayerElement(Layer *layer, LayerElement *element);

    /**
     * Returns the appropriate layer for a node looking at its MusicXml staff and voice elements.
     */
    Layer *SelectLayer(pugi::xml_node node, Measure *measure);

    /**
     * Returns the appropriate first layer of a staff.
     */
    Layer *SelectLayer(int staffNb, Measure *measure);

    /**
     * Returns the layer with @n=layerNb on the staff.
     * Creates the layer if not found.
     */
    Layer *SelectLayer(int layerNb, Staff *staff);

    /**
     * Remove the last ClassId element on top of m_elementStack.
     * For example, when closing a beam, we need to remove it from the stack, but it is not
     * necessary the top one (for example we can have an opened chord there).
     */
    void RemoveLastFromStack(ClassId classId);

    /**
     * @name Helper methods for checking presence of values of attributes or elements
     */
    ///@{
    bool HasAttributeWithValue(pugi::xml_node node, std::string attribute, std::string value);
    bool IsElement(pugi::xml_node node, std::string name);
    bool HasContentWithValue(pugi::xml_node node, std::string value);
    bool HasContent(pugi::xml_node);
    ///@}

    /**
     * @name Helper methods for retrieving attribute values or element content
     */
    ///@{
    std::string GetAttributeValue(pugi::xml_node node, std::string attribute);
    std::string GetContent(pugi::xml_node node);
    std::string GetContentOfChild(pugi::xml_node node, std::string child);
    ///@}

    /**
     * @name Methods for opening and closing ties and slurs.
     * Opened ties and slurs are stacked together with musicxml::OpenTie
     * and musicxml::OpenSlur objects.
     * For now: only slurs starting and ending on the same staff/voice are
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
    data_PLACE ConvertTypeToPlace(std::string value);
    ///@}

private:
    /** The filename */
    std::string m_filename;
    /** The stack for piling open LayerElements (beams, tuplets, chords, etc.)  */
    std::vector<LayerElement *> m_elementStack;
    /** The stack for open slurs */
    std::vector<std::pair<Slur *, musicxml::OpenSlur> > m_slurStack;
    /** The stack for open ties */
    std::vector<std::pair<Tie *, musicxml::OpenTie> > m_tieStack;

    /**
     * The stack of floating elements (tie, slur, etc.) to be added at the
     * end of each measure
     */
    std::vector<std::pair<int, ControlElement *> > m_controlElements;
};

} // namespace vrv {

#endif // __VRV_IOMUSXML_H__
