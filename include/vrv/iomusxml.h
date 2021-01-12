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

class Arpeg;
class BracketSpan;
class Clef;
class ControlElement;
class Dir;
class Dynam;
class F;
class Fermata;
class Fb;
class Gliss;
class Hairpin;
class Harm;
class InstrDef;
class Label;
class LabelAbbr;
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
class Trill;

//----------------------------------------------------------------------------
// namespace for local MusicXml classes
//----------------------------------------------------------------------------

namespace musicxml {

    class OpenSlur {
    public:
        OpenSlur(const int &number) { m_number = number; }

        int m_number;
    };

    class CloseSlur {
    public:
        CloseSlur(const std::string &measureNum, const int &number)
        {
            m_measureNum = measureNum;
            m_number = number;
        }

        std::string m_measureNum;
        int m_number;
    };

    class OpenSpanner {
    public:
        OpenSpanner(const int &dirN, const int &lastMeasureCount)
        {
            m_dirN = dirN;
            m_lastMeasureCount = lastMeasureCount;
        }

        int m_dirN;
        int m_lastMeasureCount;
    };

    class OpenArpeggio {
    public:
        OpenArpeggio(const int &arpegN, const int &timeStamp)
        {
            m_arpegN = arpegN;
            m_timeStamp = timeStamp;
        }

        int m_arpegN;
        int m_timeStamp;
    };

    class EndingInfo {
    public:
        EndingInfo(const std::string &endingNumber, const std::string &endingType, const std::string &endingText)
        {
            m_endingNumber = endingNumber;
            m_endingType = endingType;
            m_endingText = endingText;
        }

        std::string m_endingNumber;
        std::string m_endingType;
        std::string m_endingText;
    };

    class ClefChange {
    public:
        ClefChange(const std::string &measureNum, Staff *staff, Clef *clef, const int &scoreOnset, bool afterBarline)
        {
            m_measureNum = measureNum;
            m_staff = staff;
            m_clef = clef;
            m_scoreOnset = scoreOnset;
            m_afterBarline = afterBarline;
        }

        std::string m_measureNum;
        Staff *m_staff;
        Clef *m_clef;
        int m_scoreOnset; // the score position of clef change
        bool isFirst = true; // insert clef change at first layer, others use @sameas
        bool m_afterBarline = false; // musicXML attribute
    };

    class OpenDashes {
    public:
        OpenDashes(const int dirN, int staffNum, const int measureCount)
        {
            m_dirN = dirN;
            m_staffNum = staffNum;
            m_measureCount = measureCount;
        }

        int m_dirN; // direction number
        int m_staffNum;
        int m_measureCount; // measure number of dashes start
    };

} // namespace musicxml

//----------------------------------------------------------------------------
// MusicXmlInput
//----------------------------------------------------------------------------

class MusicXmlInput : public Input {
public:
    // constructors and destructors
    MusicXmlInput(Doc *doc);
    virtual ~MusicXmlInput();

    virtual bool Import(std::string const &musicxml);

private:
    /*
     * Top level method called from ImportFile or Import
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
    bool ReadMusicXmlMeasure(
        pugi::xml_node node, Section *section, Measure *measure, int nbStaves, int staffOffset, int index);
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
    void ReadMusicXmlAttributes(pugi::xml_node, Section *section, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlBackup(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlBarLine(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlDirection(pugi::xml_node, Measure *measure, const std::string &measureNum, const int staffOffset);
    void ReadMusicXmlFigures(pugi::xml_node node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlForward(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlHarmony(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlNote(
        pugi::xml_node, Measure *measure, const std::string &measureNum, const int staffOffset, Section *section);
    void ReadMusicXmlPrint(pugi::xml_node, Section *section);
    void ReadMusicXmlBeamsAndTuplets(const pugi::xml_node &node, Layer *layer, bool isChord);
    void ReadMusicXmlTupletStart(const pugi::xml_node &node, const pugi::xml_node &tupletStart, Layer *layer);
    void ReadMusicXmlBeamStart(const pugi::xml_node &node, const pugi::xml_node &beamStart, Layer *layer);
    ///@}

    /*
     * Add clef changes to all layers of a given measure, staff, and time stamp
     */
    void AddClef(Section *section, Measure *measure, Staff *staff, const std::string &measureNum);

    /*
     * Add a Measure to the section.
     * If the measure already exists it will move all its content.
     * The measure can contain only staves. Other elements must be stacked on m_floatingElements.
     */
    void AddMeasure(Section *section, Measure *measure, int i);

    /*
     * Add a Layer element to the layer or to the LayerElement at the top of m_elementStack.
     */
    void AddLayerElement(Layer *layer, LayerElement *element, int duration = 0);

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
    void RemoveLastFromStack(ClassId classId, Layer *layer);

    /*
     * @name Helper methods for checking presence of values of attributes or elements
     */
    ///@{
    bool HasAttributeWithValue(const pugi::xml_node node, const std::string &attribute, const std::string &value) const;
    bool IsElement(const pugi::xml_node node, const std::string &name) const;
    bool HasContentWithValue(const pugi::xml_node node, const std::string &value) const;
    ///@}

    /*
     * @name Helper method to check whether a ending measure number is already present in m_endingStack.
     */
    ///@{
    bool NotInEndingStack(std::string const &measureN);
    ///@}

    /*
     * @name Helper methods for retrieving attribute values or element content
     */
    ///@{
    std::string GetContent(const pugi::xml_node node) const;
    std::string GetContentOfChild(const pugi::xml_node node, const std::string &child) const;
    ///@}

    /*
     * @name Methods for opening and closing ties and slurs.
     * Opened ties and slurs are stacked together with musicxml::OpenTie
     * and musicxml::OpenSlur objects.
     * Slur starts and ends are matched based on its number.
     */
    ///@{
    void OpenTie(Note *note, Tie *tie);
    void CloseTie(Note *note);
    void OpenSlur(Measure *measure, int number, Slur *slur);
    void CloseSlur(Measure *measure, int number, LayerElement *element);
    ///@}

    /*
     * @name Helper methods for rendering text elements
     */
    ///@{
    ///@}
    std::string GetWordsOrDynamicsText(const pugi::xml_node node) const;
    void TextRendition(const pugi::xpath_node_set words, ControlElement *element) const;
    std::string StyleLabel(pugi::xml_node display);
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
     * @name Helper method for multirests. Returns number of measure hidden by MRest before
     * measure with certain index
     */
    ///@{
    int GetMrestMeasuresCountBeforeIndex(int index) const;
    ///@}

    /*
     * @name Helper method for multirests. Checks whether measure should be hidden as part of MRest
     */
    ///@{
    bool IsMultirestMeasure(int index) const;
    ///@}

    /*
     * @name Helper method for styling fermatas
     */
    ///@{
    void SetFermataExternalSymbols(Fermata *fermata, const std::string &shape);
    void ShapeFermata(Fermata *fermata, pugi::xml_node node);
    ///@}

    /*
     * @name Helper method for getting glyph numbers for ornaments based on approach/depart attributes
     */
    ///@{
    std::string GetOrnamentGlyphNumber(int attributes) const;
    ///@}

    /*
     * @name Helper method for setting @staff attribute for chords
     */
    ///@{
    void SetChordStaff(Layer *layer);
    ///@}

    /*
     * @name Methods for converting MusicXML values to MEI attributes.
     */
    ///@{
    static data_ACCIDENTAL_WRITTEN ConvertAccidentalToAccid(const std::string &value);
    static data_ACCIDENTAL_GESTURAL ConvertAlterToAccid(const float value);
    static data_ARTICULATION ConvertArticulations(const std::string &value);
    static data_BARRENDITION ConvertStyleToRend(const std::string &value, const bool repeat);
    static data_BOOLEAN ConvertWordToBool(const std::string &value);
    static data_DURATION ConvertTypeToDur(const std::string &value);
    static data_HEADSHAPE ConvertNotehead(const std::string &value);
    static data_LINESTARTENDSYMBOL ConvertLineEndSymbol(const std::string &value);
    static data_MIDIVALUE ConvertDynamicsToMidiVal(const float dynamics);
    static data_PITCHNAME ConvertStepToPitchName(const std::string &value);
    static data_TEXTRENDITION ConvertEnclosure(const std::string &value);
    static curvature_CURVEDIR InferCurvedir(const pugi::xml_node slurOrTie);
    static fermataVis_SHAPE ConvertFermataShape(const std::string &value);
    static pedalLog_DIR ConvertPedalTypeToDir(const std::string &value);
    static tupletVis_NUMFORMAT ConvertTupletNumberValue(const std::string &value);
    static std::wstring ConvertTypeToVerovioText(const std::string &value);
    static std::string ConvertAlterToSymbol(const std::string &value);
    static std::string ConvertKindToSymbol(const std::string &value);
    static std::string ConvertKindToText(const std::string &value);
    static std::string ConvertFigureGlyph(const std::string &value);
    ///@}

private:
    /* octave offset */
    std::vector<int> m_octDis;
    /* measure repeats */
    bool m_mRpt = false;
    /* measure repeats */
    bool m_slash = false;
    /* MIDI ticks */
    int m_ppq;
    /* measure time */
    int m_durTotal = 0;
    /* measure time */
    int m_durFb = 0;
    /* meter signature */
    int m_meterCount = 4;
    int m_meterUnit = 4;
    /* part information */
    Label *m_label = NULL;
    LabelAbbr *m_labelAbbr = NULL;
    InstrDef *m_instrdef = NULL;
    /* LastElementID */
    std::string m_ID;
    /* A map of stacks for piling open LayerElements (beams, tuplets, chords, btrem, ftrem) separately per layer */
    std::map<Layer *, std::vector<LayerElement *> > m_elementStackMap;
    /* A maps of time stamps (score time) to indicate write pointer of a given layer */
    std::map<Layer *, int> m_layerEndTimes;
    /* To remember layer of last element (note) to handle chords */
    Layer *m_prevLayer = NULL;
    /* To remember current layer to properly handle layers/staves/cross-staff elements */
    Layer *m_currentLayer = NULL;
    bool m_isLayerInitialized = false;
    /* The stack for open slurs */
    std::vector<std::pair<Slur *, musicxml::OpenSlur> > m_slurStack;
    /* The stack for slur stops that might come before the slur has been opened */
    std::vector<std::pair<LayerElement *, musicxml::CloseSlur> > m_slurStopStack;
    /* The stack for open ties */
    std::vector<std::pair<Tie *, Note *> > m_tieStack;
    /* The stack for tie stops that might come before that tie was opened */
    std::vector<Note *> m_tieStopStack;
    /* The stack for hairpins */
    std::vector<std::pair<Hairpin *, musicxml::OpenSpanner> > m_hairpinStack;
    /* The stack for hairpin stops that might occur before a hairpin was started staffNumber, tStamp2, (hairpinNumber,
     * measureCount) */
    std::vector<std::tuple<int, double, musicxml::OpenSpanner> > m_hairpinStopStack;
    std::vector<std::pair<BracketSpan *, musicxml::OpenSpanner> > m_bracketStack;
    std::vector<std::pair<Trill *, musicxml::OpenSpanner> > m_trillStack;
    /* The stack of endings to be inserted at the end of XML import */
    std::vector<std::pair<std::vector<Measure *>, musicxml::EndingInfo> > m_endingStack;
    /* The stack of open dashes (direction-type) containing *ControlElement, OpenDashes */
    std::vector<std::pair<ControlElement *, musicxml::OpenDashes> > m_openDashesStack;
    /* The stacks for ControlElements */
    std::vector<Dir *> m_dirStack;
    std::vector<Dynam *> m_dynamStack;
    std::vector<Gliss *> m_glissStack;
    std::vector<Harm *> m_harmStack;
    std::vector<Octave *> m_octaveStack;
    std::vector<Pedal *> m_pedalStack;
    std::vector<Tempo *> m_tempoStack;
    /*
     * The stack of floating elements (tie, slur, etc.) to be added at the
     * end of each measure
     */
    std::vector<std::pair<std::string, ControlElement *> > m_controlElements;
    /* stack of clef changes to be inserted to all layers of a given staff */
    std::vector<musicxml::ClefChange> m_ClefChangeStack;
    /* stack of new arpeggios that get more notes added. */
    std::vector<std::pair<Arpeg *, musicxml::OpenArpeggio> > m_ArpeggioStack;
    /* a map for the measure counts storing the index of each measure created */
    std::map<Measure *, int> m_measureCounts;
    /* measure rests */
    std::map<int, int> m_multiRests;
};

} // namespace vrv

#endif // __VRV_IOMUSXML_H__
