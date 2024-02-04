/////////////////////////////////////////////////////////////////////////////
// Name:        filereader.cpp
// Author:      Laurent Pugin
// Created:     31/01/2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "filereader.h"

//----------------------------------------------------------------------------

#include <fstream>

#ifndef NO_POINTING
#include <curl/curl.h>
#endif

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
    return this->Load(bytes);
#else
    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
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
    return this->Load(bytes);
#endif
}

bool ZipFileReader::Load(const std::vector<unsigned char> &bytes)
{
    this->Reset();

    m_file = new miniz_cpp::zip_file(bytes);

    return true;
}

std::list<std::string> ZipFileReader::GetFileList() const
{
    assert(m_file);

    std::list<std::string> list;
    for (miniz_cpp::zip_info &member : m_file->infolist()) {
        list.push_back(member.filename);
    }
    return list;
}

bool ZipFileReader::HasFile(const std::string &filename)
{
    assert(m_file);

    // Look for the file in the zip
    for (miniz_cpp::zip_info &member : m_file->infolist()) {
        if (member.filename == filename) {
            return true;
        }
    }

    return true;
}

std::string ZipFileReader::ReadTextFile(const std::string &filename)
{
    assert(m_file);

    // Look for the meta file in the zip
    for (miniz_cpp::zip_info &member : m_file->infolist()) {
        if (member.filename == filename) {
            return m_file->read(member.filename);
        }
    }

    LogError("No file '%s' to read found in the archive", filename.c_str());
    return "";
}

//----------------------------------------------------------------------------
// HttpFileReader
//----------------------------------------------------------------------------

bool HttpFileReader::s_init = false;

HttpFileReader::HttpFileReader()
{
#ifndef NO_POINTING
    if (!HttpFileReader::s_init) {
        curl_global_init(CURL_GLOBAL_ALL);
        HttpFileReader::s_init = true;
    }
#endif

    this->Reset();
}

HttpFileReader::~HttpFileReader()
{
    this->Reset();
}

void HttpFileReader::Reset()
{
    m_textContent = "";
}

bool HttpFileReader::Load(const std::string &filename)
{
#ifndef NO_POINTING
    CURL *curlHandle;
    CURLcode curlCode;

    struct HttpFileReader::MemoryStruct chunk;

    chunk.memory = (char *)malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0; /* no data at this point */

    /* init the curl session */
    curlHandle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, filename.c_str());

    /* send all data to this function  */
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)&chunk);

    /* some servers do not like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    m_textContent = "";

    /* get it! */
    curlCode = curl_easy_perform(curlHandle);

    /* check for errors */
    if (curlCode != CURLE_OK) {
        LogError("Request failed: %s", curl_easy_strerror(curlCode));
    }
    else {
        m_textContent.reserve(chunk.size); // Reserve space to avoid reallocations
        std::copy(chunk.memory, chunk.memory + chunk.size, std::back_inserter(m_textContent));
        LogDebug("%lu bytes retrieved", (unsigned long)chunk.size);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curlHandle);

    free(chunk.memory);

    /* we are done with libcurl, so clean it up */
    // curl_global_cleanup();

    return (curlCode == CURLE_OK);
#else
    LogError("This method is not implemented in the current build.");
    return false;
#endif
}

size_t HttpFileReader::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct HttpFileReader::MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

} // namespace vrv
