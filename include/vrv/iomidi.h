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
     * The main method for exporting the file to MEI.
     */
    virtual bool ExportFile();

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
