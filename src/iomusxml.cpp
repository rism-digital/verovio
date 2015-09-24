/////////////////////////////////////////////////////////////////////////////
// Name:        musiomusxml.cpp
// Author:      Rodolfo Zitellini
// Created:     10/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iomusxml.h"

//----------------------------------------------------------------------------

#include <sstream>

//----------------------------------------------------------------------------

#include "beam.h"
#include "clef.h"
#include "doc.h"
#include "keysig.h"
#include "mensur.h"
#include "metersig.h"
#include "note.h"
#include "rest.h"
#include "tie.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// XmlInput
//----------------------------------------------------------------------------

XmlInput::XmlInput(Doc *doc, std::string filename) :
// This is pretty bad. We open a bad fileoutputstream as we don't use it
    FileInputStream(doc)
{
    m_filename = filename;

}

XmlInput::~XmlInput()
{
}
    
bool XmlInput::ImportFile()
{
    try {
        m_doc->Reset(Raw);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(m_filename.c_str());
        if (!result)
        {
            return false;
        }
        pugi::xml_node root = doc.first_child();
        return ReadXml(root);
    }
    catch(char * str) {
        LogError("%s", str);
        return false;
    }
}

bool XmlInput::ImportString(const std::string musicxml)
{
    try {
        m_doc->Reset( Raw );
        pugi::xml_document doc;
        doc.load(musicxml.c_str());
        pugi::xml_node root = doc.first_child();
        return ReadXml(root);
    }
    catch(char * str) {
        LogError("%s", str);
        return false;
    }
}
    
bool XmlInput::ReadXml(pugi::xml_node root)
{
    
}

} // namespace vrv