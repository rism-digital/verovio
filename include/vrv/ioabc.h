/////////////////////////////////////////////////////////////////////////////
// Name:        ioabc.h
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Klaus Rettinghaus. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOABC_H__
#define __VRV_IOABC_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "io.h"
#include "pugixml.hpp"
#include "vrvdef.h"

namespace vrv {

class BarLine;
class Beam;
class Clef;
class ControlElement;
class Harm;
class KeySig;
class Layer;
class LayerElement;
class Mdiv;
class Measure;
class MeterSig;
class Note;
class Section;
class Slur;
class Staff;
class Tie;

//----------------------------------------------------------------------------
// ABCInput
//----------------------------------------------------------------------------

class ABCInput : public Input {
public:
    // constructors and destructors
    ABCInput(Doc *doc);
    virtual ~ABCInput();

    virtual bool Import(const std::string &abc);

#ifndef NO_ABC_SUPPORT

private:
    // function declarations:

    void parseABC(std::istream &infile);

    // parsing functions
    int SetBarLine(const std::string &musicCode, int index);
    void CalcUnitNoteLength();
    void AddAnnot(const std::string &remark);
    void AddBeam();
    void AddTuplet();
    void AddTie();
    void StartSlur();
    void EndSlur();

    // parse information fields
    void parseInstruction(const std::string &keyString); // I:
    void parseKey(std::string &keyString); // K:
    void parseUnitNoteLength(const std::string &unitNoteLength); // L:
    void parseMeter(const std::string &meterString); // M:
    void parseTempo(const std::string &tempoString); // Q:
    void parseReferenceNumber(const std::string &referenceNumberString); // X:

    // input functions
    void readInformationField(const char &dataKey, std::string dataValue);
    void readMusicCode(const std::string &musicCode, Section *section);

    // decoration functions
    void parseDecoration(const std::string &decorationString);
    void AddArticulation(LayerElement *element);
    void AddDynamic(LayerElement *element);
    void AddFermata(LayerElement *element);
    void AddOrnaments(LayerElement *element);

    // additional functions
    void PrintInformationFields();
    void CreateHeader();
    void CreateWorkEntry();

#endif // NO_ABC_SUPPORT

public:
    //
private:
    std::string m_filename;
    Mdiv *m_mdiv = NULL;
    Clef *m_clef = NULL;
    KeySig *m_key = NULL;
    MeterSig *m_meter = NULL;
    Layer *m_layer = NULL;

    data_DURATION m_durDefault; // todo: switch to MEI
    std::string m_ID;
    int m_unitDur;
    std::pair<data_BARRENDITION, data_BARRENDITION> m_barLines
        = std::make_pair(BARRENDITION_NONE, BARRENDITION_NONE); //
    /*
     * ABC variables with default values
     */
    char m_decoration = '!';
    char m_linebreak = '$';
    int m_lineNum = 1;
    int m_broken = 0;
    int m_gracecount = 0;
    int m_stafflines = 5;
    int m_transpose = 0;
    /*
     * ABC metadata stacks
     */
    std::vector<std::pair<std::string, int> > m_composer; // C:
    std::vector<std::pair<std::string, int> > m_history; // H:
    std::vector<std::pair<std::string, int> > m_notes; // N:
    std::vector<std::pair<std::string, int> > m_origin; // O:
    std::vector<std::pair<std::string, int> > m_title; // T:
    std::vector<std::pair<std::pair<std::string, int>, char> > m_info;

    std::vector<ControlElement *> m_tempoStack;
    std::vector<Harm *> m_harmStack;
    std::vector<Slur *> m_slurStack;
    std::vector<Tie *> m_tieStack;

    std::vector<LayerElement *> m_layerElements;
    std::vector<LayerElement *> m_noteStack;
    /*
     * ABC decoration stacks
     */
    std::vector<data_ARTICULATION> m_artic;
    std::vector<std::string> m_dynam;
    std::string m_ornam;
    data_STAFFREL m_fermata = STAFFREL_NONE;
    /*
     * The stack of control elements to be added at the end of each measure
     */
    std::vector<std::pair<std::string, ControlElement *> > m_controlElements;
    /*
     * container for work entries
     */
    pugi::xml_node m_workList;
};

} // namespace vrv

#endif
