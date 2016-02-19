/////////////////////////////////////////////////////////////////////////////
// Name:        io.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IO_H__
#define __VRV_IO_H__

#include <fstream>

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
class FileOutputStream: public std::ofstream
{
public:
    // constructors and destructors
    FileOutputStream( Doc *doc, std::string filename );
	FileOutputStream( Doc *doc );
    FileOutputStream() {};
    virtual ~FileOutputStream();
    
    /**
     * Dummy export method that must be overridden in child class.
     */
    virtual bool ExportFile( ) { return true; }
    
    /**
     * Dummy object method that must be overridden in child class.
     */
    virtual bool WriteObject( Object *object ) { return true; };
    
    /**
     * Dummy object method that must be overridden in child class.
     */
    virtual bool WriteObjectEnd( Object *object ) { return true; };
    
public:
    
protected:
    Doc *m_doc;
    
private:
    
};

//----------------------------------------------------------------------------
// FileInputStream
//----------------------------------------------------------------------------

/** 
 * This class is a base class for file input stream classes.
 * It is not an abstract class but should not be instanciated directly.
 */ 
class FileInputStream: public std::ifstream
{
public:
    // constructors and destructors
    FileInputStream( Doc *doc, std::string filename );
    FileInputStream( Doc *doc );
    virtual ~FileInputStream();
    
    
    
    // read
    virtual bool ImportFile( ) { return true; }
    virtual bool ImportString( std::string data ) { return true; }
    
    /**
     * Setter for the layoutInformation ignore flag
     */
    void IgnoreLayoutInformation() { m_ignoreLayoutInformation = true; };
    
    /**
     * Getter for layoutInformation flag that is set to true during import
     * if layout information is found (and not to be ignored).
     */
    bool HasLayoutInformation() { return m_hasLayoutInformation; };
    
    /**
     * Getter for <app> level flag that is set to true during import
     * if <measure> within editorial markup has been found
     */
    bool HasMeasureWithinEditoMarkup() { return m_hasMeasureWithinEditMarkup; };
    
    /**
     * Set XPath query for <rdg> (MEI only)
     */
    virtual void SetRdgXPathQuery( std::string xPathQuery ) {};
    
private:
    /**
     * Init values (called by the constructors)
     */
    void Init();
    
public:
    
protected:
    Doc *m_doc;
    
    /**
     * Becomes true if layout information is found during the import.
     * This will be true if the file is page-based MEI or if the MEI
     * file contains <pb> and <sb>. This will stay wrong with PAE import
     */
    bool m_hasLayoutInformation;
    
    /**
     * Becomes true if <measure> are included within editorial markup.
     * If this is true, the only continuous layout will be available.
     */
    bool m_hasMeasureWithinEditMarkup;
    
    /**
     * If set to true, the layout information found during the import
     * will be ignored. The file will be loaded in one single page with
     * one single system.
     */
    bool m_ignoreLayoutInformation;
    
    
};

} //namespace vrv

#endif
