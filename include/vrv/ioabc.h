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
#include "vrvdef.h"

namespace vrv {

class Beam;
class Clef;
class ControlElement;
class Layer;
class LayerElement;
class Harm;
class Mdiv;
class Measure;
class MeterSig;
class Note;
class Score;
class Section;
class Slur;
class Staff;
class Tie;
class Tuplet;
class KeySig;
class BarLine;

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

    void addLayerElement(LayerElement *element);
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

    // additional functions
    void printHeader();

#endif // NO_ABC_SUPPORT

public:
    //
private:
    std::string m_filename;
    Mdiv *m_mdiv;
    Measure *m_measure;
    Layer *m_layer;
    Staff *m_staff;

    std::vector<data_ARTICULATION> m_artic;
    data_DURATION m_durDefault = DURATION_NONE; // todo: switch to MEI
    int m_unitDur;
    char m_decoration = '!';
    char m_linebreak = '\n';
    int m_lineNum = 0;
    int m_broken = 0;
    std::vector<std::string> m_title;
    std::vector<std::string> m_composer;
    std::vector<std::string> m_origin;

    std::vector<ControlElement *> m_controlElements;

    Note *m_last_tied_note;
    bool m_is_in_chord;

    std::vector<LayerElement *> m_layerElements;
    std::vector<LayerElement *> m_noteStack;
    std::vector<Harm *> m_harmStack;
};

} // namespace vrv

#endif
