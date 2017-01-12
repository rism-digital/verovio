/////////////////////////////////////////////////////////////////////////////
// Name:        iomidi.cpp
// Author:      Klaus Rettinghaus
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iomidi.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

#include "MidiFile.h"

//----------------------------------------------------------------------------

class MidiFile;

namespace vrv {

//----------------------------------------------------------------------------
// MidiOutput
//----------------------------------------------------------------------------

MidiOutput::MidiOutput(Doc *doc, std::string filename) : FileOutputStream(doc)
{
    m_filename = filename;
    m_writeToStreamString = false;
    m_page = -1;
    m_scoreBasedMEI = false;
}

MidiOutput::~MidiOutput()
{
}

bool MidiOutput::ExportFile()
{

    return false;
}

std::string MidiOutput::GetOutput(int page)
{
    m_writeToStreamString = true;
    m_page = page;
    this->ExportFile();
    m_writeToStreamString = false;
    m_page = -1;

    return m_streamStringOutput.str();
}

//----------------------------------------------------------------------------
// MidiInput
//----------------------------------------------------------------------------

} // namespace vrv
