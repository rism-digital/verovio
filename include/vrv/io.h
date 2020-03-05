/////////////////////////////////////////////////////////////////////////////
// Name:        io.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IO_H__
#define __VRV_IO_H__

#include <string>
#include <vector>

namespace vrv {

class Doc;
class Object;

//----------------------------------------------------------------------------
// Output
//----------------------------------------------------------------------------

/**
 * This class is a base class for output classes.
 * It is not an abstract class but should not be instanciated directly.
 */
class Output {
public:
    // constructors and destructors
    Output(Doc *doc, std::string filename);
    Output(Doc *doc);
    Output(){};
    virtual ~Output();

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
// Input
//----------------------------------------------------------------------------

/**
 * This class is a base class for input classes.
 * It is not an abstract class but should not be instanciated directly.
 */
class Input {
public:
    // constructors and destructors
    Input(Doc *doc);
    virtual ~Input();

    void SetOutputFormat(const std::string &format) { m_outformat = format; }
    std::string GetOutputFormat() { return m_outformat; }

    // read
    virtual bool Import(std::string const &data) { return true; }

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
