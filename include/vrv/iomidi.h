/////////////////////////////////////////////////////////////////////////////
// Name:        iomidi.h
// Author:      Klaus Rettinghaus
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOMIDI_H__
#define __VRV_IOMIDI_H__

#include <sstream>

//----------------------------------------------------------------------------

#include "doc.h"
#include "io.h"

//----------------------------------------------------------------------------

#ifndef NO_MIDI_SUPPORT
#include "MidiFile.h"
#endif /* NO_MIDI_SUPPORT */

//----------------------------------------------------------------------------

class MidiFile;

namespace vrv {

//----------------------------------------------------------------------------
// MidiOutput
//----------------------------------------------------------------------------

class MidiOutput : public FileOutputStream {
public:
    /** @name Constructors and destructor */
    ///@{
    MidiOutput(Doc *doc, std::string filename);
    virtual ~MidiOutput();
    ///@}

    /**
     * The main method for exporting the file to MIDI.
     */
    virtual bool ExportFile();

    /**
     * Return the output as a string by writing it to the stringstream member.
     */
    std::string GetOutput(int page = -1);

    /**
     * Export the document to a MIDI file.
     * Run trough all the layer and fill the midi file content.
     */
    void ExportMIDI(MidiFile *midiFile);

    bool GetMidiExportDone() const;

private:
    /**
     * A flag to indicate if the MIDI export has been done.
     * This is necessary for retrieving notes being played at a certain time.
     */
    bool m_midiExportDone;
    std::string m_filename;
    std::ostringstream m_streamStringOutput;
    bool m_writeToStreamString;
    int m_page;
    bool m_scoreBasedMEI;
    pugi::xml_node m_mei;
    /** @name Current element */
    pugi::xml_node m_currentNode;
    std::list<pugi::xml_node> m_nodeStack;
};

//----------------------------------------------------------------------------
// MidiInput
//----------------------------------------------------------------------------

/**
 * This class is just an empty container
*/
class MidiInput : public FileInputStream {
};

} // namespace vrv

#endif
