/////////////////////////////////////////////////////////////////////////////
// Name:        glyph.cpp
// Author:      Rodolfo Zitellini
// Created:     30/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "glyph.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"
#include "smufl.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Glyph
//----------------------------------------------------------------------------

Glyph::Glyph(std::string path)
{
    m_x = 0.0;
    m_y = 0.0;
    m_width = 0.0;
    m_height = 0.0;
    m_unitsPerEm = 2048;
    m_path = path;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file( path.c_str() );
    if (!result)
    {
        LogError("Font file '%s' could not be loaded", path.c_str() );
        return;
    }
    pugi::xml_node root = doc.first_child();
    
    if (!root.attribute("viewBox"))
    {
        LogMessage("Font file '%s' does not contain a viewBox attribute", path.c_str() );
        return;
    }
    
    std::string viewBox(root.attribute("viewBox").value());
    
    if (std::count(viewBox.begin(), viewBox.end(), ' ') < 3)
    {
        LogMessage("Font file viewBox attribute '%s' is not valid", viewBox.c_str() );
        return;
    }
    
    m_unitsPerEm = atoi( viewBox.substr( viewBox.find_last_of(' ') ).c_str() );
}

Glyph::~Glyph()
{
}
    
void Glyph::SetBoundingBox(double x, double y, double w, double h)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;
}


} // namespace vrv
