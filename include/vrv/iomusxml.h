/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.h
// Author:      Rodolfo Zitellini
// Created:     10/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_IOMUSXML_H__
#define __VRV_IOMUSXML_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "io.h"
#include "pugixml.hpp"

namespace vrv {

class Barline;
class Beam;
class Clef;
class Doc;
class Layer;
class LayerElement;
class MeterSig;
class MRest;
class MultiRest;
class Note;
class Page;
class Rest;
class Staff;
class System;
class Tuplet;


//----------------------------------------------------------------------------
// This class is not up-to-date
//----------------------------------------------------------------------------

class XmlInput: public FileInputStream
{
public:
    // constructors and destructors
    XmlInput(Doc *doc, std::string filename);
    virtual ~XmlInput();
    
    virtual bool ImportFile( );
    virtual bool ImportString(std::string musicxml);
    
private:
    bool ReadXml(pugi::xml_node root);
    
private:
    std::string m_filename;
    /** The parts are read as a vector of systems */
    std::vector<System*> m_parts;
};

} // namespace vrv {

#endif // __VRV_IOMUSXML_H__
