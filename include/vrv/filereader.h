/////////////////////////////////////////////////////////////////////////////
// Name:        filereader.h
// Author:      Laurent Pugin
// Created:     31/01/2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FILEREADER_H__
#define __VRV_FILEREADER_H__

#include <list>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

/** Forward declaration of the zip_file.hpp class */
namespace miniz_cpp {
class zip_file;
}

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ZipFileReader
//----------------------------------------------------------------------------

/**
 * This class is a reader for zip archives.
 */
class ZipFileReader {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    ZipFileReader();
    ~ZipFileReader();
    ///@}

    /**
     * Reset a previously loaded file.
     */
    void Reset();

    /**
     * Load a file into memory.
     */
    bool Load(const std::string &filename);

    /**
     * Load a vector into memory
     */
    bool LoadBytes(const std::vector<unsigned char> &bytes);

    /**
     * Check if the archive contains the file
     */
    bool HasFile(const std::string &filename);

    /**
     * Read the text file.
     * Return an empty string if the file does not exist.
     */
    std::string ReadTextFile(const std::string &filename);

    /**
     * Return a list of all files (including directories)
     */
    std::list<std::string> GetFileList() const;

private:
    //
public:
    //
private:
    /** A pointer to the miniz zip file */
    miniz_cpp::zip_file *m_file;

}; // class ZipFileReader

} // namespace vrv

#endif // __VRV_FILEREADER_H__
