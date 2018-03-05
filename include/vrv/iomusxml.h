/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.h
// Author:      Laurent Pugin and Klaus Rettinghaus
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
class Dir;
class Dynam;
class F;
class Fb;
class Hairpin;
class Harm;
class Layer;
class LayerElement;
class Measure;
class Octave;
class Pedal;
class Section;
class Slur;
class StaffGrp;
class Tempo;
class Tie;

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

    class OpenHairpin {
    public:
        OpenHairpin(int dirN, std::string endID)
        {
            m_dirN = dirN;
            m_endID = endID;
        }

        int m_dirN;
        std::string m_endID;
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
    /*
     * Top level method called from ImportFile or ImportString
     */
    bool ReadMusicXml(pugi::xml_node root);

    /*
     * Method to fill MEI header with title
     */
    void ReadMusicXmlTitle(pugi::xml_node title);

    /*
     * @name Top level methods for reading MusicXml part and measure elements.
     */
    ///@{
    bool ReadMusicXmlPart(pugi::xml_node node, Section *section, int nbStaves, int staffOffset);
    bool ReadMusicXmlMeasure(pugi::xml_node node, Section *section, Measure *measure, int nbStaves, int staffOffset);
    ///@}

    /*
     * Methods for reading the first MusicXml attributes element as MEI staffDef.
     * Returns the number of staves in the part.
     */
    int ReadMusicXmlPartAttributesAsStaffDef(pugi::xml_node node, StaffGrp *staffGrp, int staffOffset);

    /*
     * @name Methods for reading the content of a MusicXml measure.
     */
    ///@{
    void ReadMusicXmlAttributes(pugi::xml_node, Section *section, Measure *measure, std::string measureNum);
    void ReadMusicXmlBackup(pugi::xml_node, Measure *measure, std::string measureNum);
    void ReadMusicXmlBarLine(pugi::xml_node, Measure *measure, std::string measureNum);
    void ReadMusicXmlDirection(pugi::xml_node, Measure *measure, std::string measureNum);
    void ReadMusicXmlFigures(pugi::xml_node node, Measure *measure, std::string measureNum);
    void ReadMusicXmlForward(pugi::xml_node, Measure *measure, std::string measureNum);
    void ReadMusicXmlHarmony(pugi::xml_node, Measure *measure, std::string measureNum);
    void ReadMusicXmlNote(pugi::xml_node, Measure *measure, std::string measureNum);
    void ReadMusicXmlPrint(pugi::xml_node, Section *section);
    ///@}

    /*
     * Add a Measure to the section.
     * If the measure already exists it will move all its content.
     * The measure can contain only staves. Other elements must be stacked on m_floatingElements.
     */
    void AddMeasure(Section *section, Measure *measure, int i);

    /*
     * Add a Layer element to the layer or to the LayerElement at the top of m_elementStack.
     */
    void AddLayerElement(Layer *layer, LayerElement *element);

    /*
     * Returns the appropriate layer for a node looking at its MusicXml staff and voice elements.
     */
    Layer *SelectLayer(pugi::xml_node node, Measure *measure);

    /*
     * Returns the appropriate first layer of a staff.
     */
    Layer *SelectLayer(int staffNb, Measure *measure);

    /*
     * Returns the layer with @n=layerNb on the staff.
     * Creates the layer if not found.
     */
    Layer *SelectLayer(int layerNb, Staff *staff);

    /*
     * Remove the last ClassId element on top of m_elementStack.
     * For example, when closing a beam, we need to remove it from the stack, but it is not
     * necessary the top one (for example we can have an opened chord there).
     */
    void RemoveLastFromStack(ClassId classId);

    /*
     * @name Helper methods for checking presence of values of attributes or elements
     */
    ///@{
    bool HasAttributeWithValue(pugi::xml_node node, std::string attribute, std::string value);
    bool IsElement(pugi::xml_node node, std::string name);
    bool HasContentWithValue(pugi::xml_node node, std::string value);
    ///@}

    /*
     * @name Helper methods for retrieving attribute values or element content
     */
    ///@{
    std::string GetContent(pugi::xml_node node);
    std::string GetContentOfChild(pugi::xml_node node, std::string child);
    ///@}

    /*
     * @name Methods for opening and closing ties and slurs.
     * Opened ties and slurs are stacked together with musicxml::OpenTie
     * and musicxml::OpenSlur objects.
     * For now: only slurs starting and ending on the same staff/voice are
     * supported
     */
    ///@{
    void OpenTie(Staff *staff, Layer *layer, Note *note, Tie *tie);
    void CloseTie(Staff *staff, Layer *layer, Note *note, bool isClosingTie);
    void OpenSlur(Staff *staff, Layer *layer, int number, Slur *slur);
    void CloseSlur(Staff *staff, Layer *layer, int number, LayerElement *element);
    ///@}

    /*
     * @name Helper methods for rendering text elements
     */
    ///@{
    ///@}
    void TextRendition(pugi::xpath_node_set words, ControlElement *element);
    void PrintMetronome(pugi::xml_node metronome, Tempo *tempo);

    /*
     * @name Helper methods for filling in space elements
     */
    ///@{
    ///@}
    void FillSpace(Layer *layer, int dur);

    /*
     * @name Helper method for generating additional IDs
     */
    ///@{
    ///@}
    void GenerateUuid(pugi::xml_node node);

    /*
     * @name Methods for converting MusicXML string values to MEI attributes.
     */
    ///@{
    data_ACCIDENTAL_WRITTEN ConvertAccidentalToAccid(std::string value);
    data_ACCIDENTAL_GESTURAL ConvertAlterToAccid(float value);
    data_BARRENDITION ConvertStyleToRend(std::string value, bool repeat);
    data_BOOLEAN ConvertWordToBool(std::string value);
    data_DURATION ConvertTypeToDur(std::string value);
    data_PITCHNAME ConvertStepToPitchName(std::string value);
    curvature_CURVEDIR ConvertOrientationToCurvedir(std::string);
    fermataVis_SHAPE ConvertFermataShape(std::string);
    pedalLog_DIR ConvertPedalTypeToDir(std::string value);
    tupletVis_NUMFORMAT ConvertTupletNumberValue(std::string value);
    std::string ConvertAlterToSymbol(std::string value);
    std::string ConvertKindToSymbol(std::string value);
    ///@}

private:
    /* The filename */
    std::string m_filename;
    /* octave offset **/
    std::vector<int> m_octDis;
    /* measure repeats **/
    bool m_mRpt = false;
    /* MIDI ticks */
    int m_ppq;
    /* meaure time */
    int m_durTotal = 0;
    /* meter signature */
    int m_meterCount = 0;
    int m_meterUnit = 0;
    /* LastElementID **/
    std::string m_ID;
    /* The stack for piling open LayerElements (beams, tuplets, chords, etc.)  */
    std::vector<LayerElement *> m_elementStack;
    /* The stack for open slurs */
    std::vector<std::pair<Slur *, musicxml::OpenSlur> > m_slurStack;
    /* The stack for open ties */
    std::vector<std::pair<Tie *, musicxml::OpenTie> > m_tieStack;
    /* The stack for hairpins */
    std::vector<std::pair<Hairpin *, musicxml::OpenHairpin> > m_hairpinStack;
    /* The stacks for ControlElements */
    std::vector<Dir *> m_dirStack;
    std::vector<Dynam *> m_dynamStack;
    std::vector<Harm *> m_harmStack;
    std::vector<Octave *> m_octaveStack;
    std::vector<Pedal *> m_pedalStack;
    std::vector<Tempo *> m_tempoStack;
    /*
     * The stack of floating elements (tie, slur, etc.) to be added at the
     * end of each measure
     */
    std::vector<std::pair<std::string, ControlElement *> > m_controlElements;
};

} // namespace vrv

#endif // __VRV_IOMUSXML_H__
