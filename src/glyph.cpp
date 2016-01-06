/////////////////////////////////////////////////////////////////////////////
// Name:        glyph.cpp
// Author:      Rodolfo Zitellini
// Created:     30/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "glyph.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <stdlib.h>

//----------------------------------------------------------------------------

#include "pugixml.hpp"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Glyph
//----------------------------------------------------------------------------

Glyph::Glyph()
{
    m_x = 0.0;
    m_y = 0.0;
    m_width = 0.0;
    m_height = 0.0;
    m_unitsPerEm = 20480;
    m_path = "[unset]";
    m_codeStr = "[unset]";
}
    
Glyph::Glyph(std::string path, std::string codeStr)
{
    m_x = 0.0;
    m_y = 0.0;
    m_width = 0.0;
    m_height = 0.0;
    m_unitsPerEm = 20480;
    m_path = path;
    m_codeStr = codeStr;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    if (!result)
    {
        LogError("Font file '%s' could not be loaded", path.c_str());
        return;
    }
    pugi::xml_node root = doc.first_child();
    
    // look at the viewBox attribute for getting the units per em
    if (!root.attribute("viewBox"))
    {
        LogMessage("Font file '%s' does not contain a viewBox attribute", path.c_str());
        return;
    }
    
    std::string viewBox(root.attribute("viewBox").value());
    // the viewBox attribute is expected to be the for "0 0 2048 2048"
    // we are looking for the last value
    if (std::count(viewBox.begin(), viewBox.end(), ' ') < 3)
    {
        LogMessage("Font file viewBox attribute '%s' is not valid", viewBox.c_str());
        return;
    }
    
    m_unitsPerEm = atoi(viewBox.substr(viewBox.find_last_of(' ')).c_str()) * 10;
}
    
Glyph::Glyph(int unitsPerEm)
{
    m_x = 0.0;
    m_y = 0.0;
    m_width = 0.0;
    m_height = 0.0;
    m_unitsPerEm = unitsPerEm * 10;
    m_path = "[unset]";
    m_codeStr = "[unset]";
}    

Glyph::~Glyph()
{
}
    
void Glyph::SetBoundingBox(double x, double y, double w, double h)
{
    m_x = (int)(10.0  * x);
    m_y = (int)(10.0  * y);
    m_width = (int)(10.0  * w);
    m_height = (int)(10.0  * h);
}
    
void Glyph::GetBoundingBox(int *x, int *y, int *w, int *h)
{
    (*x) = m_x;
    (*y) = m_y;
    (*w) = m_width;
    (*h) = m_height;
}


} // namespace vrv
