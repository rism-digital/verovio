/////////////////////////////////////////////////////////////////////////////
// Name:        io.cpp
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "io.h"

//----------------------------------------------------------------------------

#include <assert.h>

namespace vrv {

//----------------------------------------------------------------------------
// FileOutputStream
//----------------------------------------------------------------------------

FileOutputStream::FileOutputStream(Doc *doc, std::string filename) : std::ofstream(filename.c_str())
{
    assert(doc);
    m_doc = doc;
}

FileOutputStream::FileOutputStream(Doc *doc) : std::ofstream()
{
    assert(doc);
    m_doc = doc;
}

FileOutputStream::~FileOutputStream()
{
    if (this->is_open()) {
        this->close();
    }
}

//----------------------------------------------------------------------------
// FileInputStream
//----------------------------------------------------------------------------

FileInputStream::FileInputStream(Doc *doc, std::string filename) : std::ifstream(filename.c_str())
{
    assert(doc);
    m_doc = doc;
    Init();
}

FileInputStream::FileInputStream(Doc *doc) : std::ifstream()
{
    assert(doc);
    m_doc = doc;
    Init();
}

void FileInputStream::Init()
{
    m_hasLayoutInformation = false;
}

FileInputStream::~FileInputStream()
{
    if (this->is_open()) {
        this->close();
    }
}

} // namespace vrv
