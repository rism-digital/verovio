/////////////////////////////////////////////////////////////////////////////
// Name:        io.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IO_H__
#define __VRV_IO_H__

#include <fstream>
#include <vector>

namespace vrv {

class Doc;
class Object;

//----------------------------------------------------------------------------
// FileOutputStream
//----------------------------------------------------------------------------

/**
 * This class is a base class for file output stream classes.
 * It is not an abstract class but should not be instanciated directly.
 */
class FileOutputStream : public std::ofstream {
public:
    // constructors and destructors
    FileOutputStream(Doc *doc, std::string filename);
    FileOutputStream(Doc *doc);
    FileOutputStream(){};
    virtual ~FileOutputStream();

    /**
     * Dummy export method that must be overridden in child class.
     */
    virtual bool ExportFile() { return true; }

    /**
     * Dummy object method that must be overridden in child class.
     */
    virtual bool WriteObject(Object *object) { return true; }

    /**
     * Dummy object method that must be overridden in child class.
     */
    virtual bool WriteObjectEnd(Object *object) { return true; }

public:
    //
protected:
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// FileInputStream
//----------------------------------------------------------------------------

/**
 * This class is a base class for file input stream classes.
 * It is not an abstract class but should not be instanciated directly.
 */
class FileInputStream : public std::ifstream {
public:
    // constructors and destructors
    FileInputStream(Doc *doc, std::string filename);
    FileInputStream(Doc *doc);
    virtual ~FileInputStream();

    void SetOutputFormat(const std::string &format) { m_outformat = format; }
    std::string GetOutputFormat() { return m_outformat; }

    // read
    virtual bool ImportFile() { return true; }
    virtual bool ImportString(std::string const &data) { return true; }

    /**
     * Getter for layoutInformation flag that is set to true during import
     * if layout information is found (and not to be ignored).
     */
    bool HasLayoutInformation() { return m_hasLayoutInformation; }

private:
    /**
     * Init values (called by the constructors)
     */
    void Init();

public:
    //

protected:
    Doc *m_doc;

    /**
     * Becomes true if layout information is found during the import.
     * This will be true if the file is page-based MEI or if the MEI
     * file contains <pb> and <sb>. This will stay wrong with PAE import
     */
    bool m_hasLayoutInformation;

    std::string m_outformat = "mei";
};

} // namespace vrv

#endif
