/////////////////////////////////////////////////////////////////////////////
// Name:        musio.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __MUS_IO_H__
#define __MUS_IO_H__

#include <fstream>
#include <iostream>
#include <string>

namespace vrv {

class Doc;
class Layer;
class LayerApp;
class LayerElement;
class LayerRdg;
class Measure;
class Page;
class Staff;
class StaffDef;
class StaffGrp;
class System;

//----------------------------------------------------------------------------
// FileOutputStream
//----------------------------------------------------------------------------

/** 
 * This class is a base class for file output stream classes.
 * It is not an abstract class but should not be instanciate directly.
 */ 
class FileOutputStream: public std::ofstream
{
public:
    // constructors and destructors
    FileOutputStream( Doc *doc, std::string filename );
	FileOutputStream( Doc *doc );
    FileOutputStream() {};
	//FileOutputStream( Doc *file, wxFile *wxfile );
    virtual ~FileOutputStream();
    
    virtual bool ExportFile( ) { return true; }
    
    /** @name Writing element methods
     * The following methods actually write the elements.
     * They must be overriden in the child classes.
     * The children of the elements do not have to be writen from theses methods.
     * This actually happen in the Save method of the Object classes
     */
    ///@{
    virtual bool WriteDoc( Doc *doc ) { return true; };
    virtual bool WritePage( Page *page ) { return true; };
    virtual bool WriteSystem( System *system ) { return true; };
    virtual bool WriteStaffGrp( StaffGrp *system ) { return true; };
    virtual bool WriteStaffDef( StaffDef *system ) { return true; };
    virtual bool WriteMeasure( Measure *measure ) { return true; };
    virtual bool WriteStaff( Staff *staff ) { return true; };
    virtual bool WriteLayer( Layer *layer ) { return true; };
    virtual bool WriteLayerElement( LayerElement *element ) { return true; };
    // app
    virtual bool WriteLayerApp( LayerApp *app ) { return true; };
    virtual bool WriteLayerRdg( LayerRdg *rdg ) { return true; };
    ///@}
    
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
 * It is not an abstract class but should not be instanciate directly.
 */ 
class FileInputStream: public std::ifstream
{
public:
    // constructors and destructors
    FileInputStream( Doc *doc, std::string filename );
    FileInputStream( Doc *doc );
    FileInputStream() {};
    virtual ~FileInputStream();
    
    // read
    virtual bool ImportFile( ) { return true; }
    virtual bool ImportString( std::string data ) { return true; }
    
    bool HasLayoutInformation() { return m_hasLayoutInformation; };
    
public:
    
protected:
    Doc *m_doc;
    
    /**
     * Becomes true if layout information is found during the import.
     * This will be true if the file is page-based MEI or if the MEI
     * file contains <pb> and <sb>. This will stay wrong with PAE import
     */
    bool m_hasLayoutInformation;
    
    
};

} //namespace vrv

#endif
