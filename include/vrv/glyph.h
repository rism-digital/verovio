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
    Glyph(std::string path);
    virtual ~Glyph();
    ///@}
    
    /** Get the bounds of the glyph */
    void GetBoundingBox(double *x, double *y, double *w, double *h);
    void SetBoundingBox(double x, double y, double w, double h);
    
    /** Get the units per EM */
    int GetUnitsPerEm() { return m_unitsPerEm; };

private:
    
    
public:
    
    
private:
    /** The bounding box values of the glyph */
    double m_x;
    double m_y;
    double m_width;
    double m_height;
    /** Units per EM for the glyph */
    int m_unitsPerEm;
    /** Path to the file */
    std::string m_path;
};


} // namespace vrv
    
#endif
