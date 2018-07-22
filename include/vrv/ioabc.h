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
class ControlElement;
class Harm;
class KeySig;
class Layer;
class LayerElement;
class Mdiv;
class Measure;
class MeterSig;
class Section;
class Slur;
class Staff;
class Tie;

//----------------------------------------------------------------------------
// AbcInput
//----------------------------------------------------------------------------

class AbcInput : public FileInputStream {
public:
    // constructors and destructors
    AbcInput(Doc *doc, std::string filename);
    virtual ~AbcInput();

    virtual bool ImportFile();
    virtual bool ImportString(std::string const &abc);

#ifndef NO_ABC_SUPPORT

private:
    // function declarations:

    void parseABC(std::istream &infile);

    // parsing functions
    int getBarLine(const char *incipit, data_BARRENDITION *output, int index);

    void calcUnitNoteLength();
    void AddBeam();
    void AddTuplet();

    // parse information fields
    void parseInstruction(std::string keyString); // I:
    void parseKey(std::string keyString); // K:
    void parseUnitNoteLength(std::string unitNoteLength); // L:
    void parseMeter(std::string meterString); // M:
    void parseTempo(std::string tempoString); // Q:
    void parseReferenceNumber(std::string referenceNumberString); // X:

    // input functions
    void readInformationField(char dataKey, std::string dataValue);
    void readMusicCode(const char *musicCode, Section *section);

    void parseDecoration(std::string decorationString);
    void startSlur(std::string measureId);
    void endSlur();
    void addTie(std::string measureId);

    // additional functions
    void printInformationFields();
    void createHeader();
    void createWorkEntry();

#endif // NO_ABC_SUPPORT

public:
    //
private:
    std::string m_filename;
    Mdiv *m_mdiv = NULL;
    MeterSig *m_meter = NULL;
    Layer *m_layer = NULL;

    std::vector<data_ARTICULATION> m_artic;
    data_DURATION m_durDefault = DURATION_NONE; // todo: switch to MEI
    std::string m_ID;
    int m_unitDur;
    char m_decoration = '!';
    char m_linebreak = '$';
    int m_lineNum = 0;
    int m_broken = 0;
    int m_gracecount = 0;
    std::vector<std::pair<std::string, int> > m_composer; // C:
    std::vector<std::pair<std::string, int> > m_history; // H:
    std::vector<std::pair<std::string, int> > m_notes; // N:
    std::vector<std::pair<std::string, int> > m_origin; // O:
    std::vector<std::pair<std::string, int> > m_title; // T:

    std::vector<ControlElement *> m_tempoStack;
    std::vector<Harm *> m_harmStack;
    std::vector<Slur *> m_slurStack;
    std::vector<Tie *> m_tieStack;

    std::vector<LayerElement *> m_layerElements;
    std::vector<LayerElement *> m_noteStack;
    /*
     * The stack of floating elements (tie, slur, etc.) to be added at the
     * end of each measure
     */
    std::vector<std::pair<std::string, ControlElement *> > m_controlElements;
    /*
     * container for work entries
     */
    pugi::xml_node m_workDesc;
};

} // namespace vrv

#endif
