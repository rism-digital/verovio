/////////////////////////////////////////////////////////////////////////////
// Name:        filereader.cpp
// Author:      Laurent Pugin
// Created:     31/01/2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "filereader.h"

//----------------------------------------------------------------------------

#include <fstream>

//----------------------------------------------------------------------------

#include "vrv.h"

//----------------------------------------------------------------------------

#include "zip_file.hpp"

namespace vrv {

//----------------------------------------------------------------------------
// ZipFileReader
//----------------------------------------------------------------------------

ZipFileReader::ZipFileReader()
{
    m_file = NULL;

    this->Reset();
}

ZipFileReader::~ZipFileReader()
{
    this->Reset();
}

void ZipFileReader::Reset()
{
    if (m_file) {
        delete m_file;
        m_file = NULL;
    }
}

bool ZipFileReader::Load(const std::string &filename)
{
#ifdef __EMSCRIPTEN__
    std::string data = filename;
    // Remove the mimetype prefix if any
    if (data.starts_with("data:")) {
        data = data.substr(data.find("base64,") + 7);
    }
    std::vector<unsigned char> bytes = Base64Decode(data);
    return this->LoadBytes(bytes);
#else
    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
        LogError("File archive '%s' could not be opened.", filename.c_str());
        return false;
    }

    fin.seekg(0, std::ios::end);
    std::streamsize fileSize = (std::streamsize)fin.tellg();
    fin.clear();
    fin.seekg(0, std::wios::beg);

    std::vector<unsigned char> bytes;
    bytes.reserve(fileSize + 1);

    unsigned char buffer;
    while (fin.read((char *)&buffer, sizeof(unsigned char))) {
        bytes.push_back(buffer);
    }
    return this->LoadBytes(bytes);
#endif
}

bool ZipFileReader::LoadBytes(const std::vector<unsigned char> &bytes)
{
    this->Reset();

    m_file = new miniz_cpp::zip_file(bytes);

    return true;
}

std::list<std::string> ZipFileReader::GetFileList() const
{
    assert(m_file);

    std::list<std::string> list;
    for (const miniz_cpp::zip_info &member : m_file->infolist()) {
        list.push_back(member.filename);
    }
    return list;
}

bool ZipFileReader::HasFile(const std::string &filename)
{
    assert(m_file);

    // Look for the file in the zip
    const std::vector<miniz_cpp::zip_info> &fileInfoList = m_file->infolist();
    return std::any_of(fileInfoList.cbegin(), fileInfoList.cend(),
        [&filename](const auto &info) { return info.filename == filename; });
}

std::string ZipFileReader::ReadTextFile(const std::string &filename)
{
    assert(m_file);

    // Look for the meta file in the zip
    for (const miniz_cpp::zip_info &member : m_file->infolist()) {
        if (member.filename == filename) {
            return m_file->read(member.filename);
        }
    }

    LogError("No file '%s' to read found in the archive", filename.c_str());
    return "";
}

} // namespace vrv
