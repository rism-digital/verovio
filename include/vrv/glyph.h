/////////////////////////////////////////////////////////////////////////////
// Name:        glyph.h
// Author:      Rodolfo Zitellini
// Created:     30/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_GLYPH_H__
#define __VRV_GLYPH_H__

#include "object.h"

namespace vrv {


/**
 * This class is used for storing a music font glyph.
 */
class Glyph
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    Glyph() {};
    Glyph(std::string path, std::string codeStr);
    virtual ~Glyph();
    ///@}
    
    /** Get the bounds of the glyph */
    void GetBoundingBox(int *x, int *y, int *w, int *h);
    void SetBoundingBox(double x, double y, double w, double h);
    
    /** Get the units per EM */
    int GetUnitsPerEm() { return m_unitsPerEm; };
    
    /** Get the path */
    std::string GetPath() { return m_path; };
    
    /** Get the code string */
    std::string GetCodeStr() { return m_codeStr; };

private:
    
    
public:
    
    
private:
    /** The bounding box values of the glyph */
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    /** Units per EM for the glyph */
    int m_unitsPerEm;
    /** Path to the file */
    std::string m_path;
    /** The Unicode code in hexa as string */
    std::string m_codeStr;
};


} // namespace vrv
    
#endif
