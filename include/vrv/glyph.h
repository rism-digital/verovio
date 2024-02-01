/////////////////////////////////////////////////////////////////////////////
// Name:        glyph.h
// Author:      Rodolfo Zitellini
// Created:     30/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GLYPH_H__
#define __VRV_GLYPH_H__

#include <algorithm>
#include <string>

//----------------------------------------------------------------------------

#include "devicecontextbase.h"
#include "vrvdef.h"

namespace vrv {

/**
 * This class is used for storing a music font glyph.
 * All glyph values are integers. However, for keeping precision as high
 * as possible, they are 10 times the original values. Since the unit
 * per em value is also 10 times the original, there is no incident on
 * calculations elsewhere. However, it does increase the precision because
 * units are always multiplied by a point size before being divided by the
 * unit per em. See
 * Ex: 10.2 become 102, with a unit per em of 20480 (instead of 2048)
 */
class Glyph {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    Glyph();
    Glyph(std::string path, std::string codeStr);
    Glyph(int unitsPerEm);
    virtual ~Glyph();
    ///@}

    /** Get the bounds of the glyph */
    void GetBoundingBox(int &x, int &y, int &w, int &h) const;

    /**
     * Set the bounds of the glyph
     * These are original values from the font and will be
     * stored as (int)(10.0 * x) in the glyph
     */
    void SetBoundingBox(double x, double y, double w, double h);

    /**
     * @name Setter and getter for the units per EM
     */
    ///@{
    int GetUnitsPerEm() const { return m_unitsPerEm; }
    void SetUnitsPerEm(int units) { m_unitsPerEm = units; }
    ///@}

    /**
     * @name Setter and getter for the code string
     */
    ///@{
    std::string GetCodeStr() const { return m_codeStr; }
    void SetCodeStr(const std::string &codeStr) { m_codeStr = codeStr; }
    ///@}

    /**
     * @name Setter and getter for the path
     */
    ///@{
    std::string GetPath() const { return m_path; }
    void SetPath(const std::string &path) { m_path = path; }
    ///@}

    /**
     * @name Setter and getter for the horizAdvX
     */
    ///@{
    int GetHorizAdvX() const { return m_horizAdvX; }
    void SetHorizAdvX(double horizAdvX) { m_horizAdvX = (int)(horizAdvX * 10.0); }
    ///@}

    /**
     * @name Setter and getter for the fallback falg
     */
    ///@{
    bool GetFallback() const { return m_isFallback; }
    void SetFallback(bool isFallback) { m_isFallback = isFallback; }
    ///@}

    /**
     * Add an anchor for the glyph.
     * The string is turn into a SMuFLGlyphAnchor ("cutOutNE" => SMUFL_cutOutNE)
     */
    void SetAnchor(std::string anchorStr, double x, double y);

    /**
     * Check if the glyph has anchor provided.
     */
    bool HasAnchor(SMuFLGlyphAnchor anchor) const;

    /**
     * Return the SMuFL anchor for the glyph.
     */
    const Point *GetAnchor(SMuFLGlyphAnchor anchor) const;

    /**
     * Set the XML (content) of the glyph.
     * This is used only for glyph added from zip archive custom fonts.
     */
    void SetXML(const std::string &xml) { m_xml = xml; }

    /**
     * Return the XML (content) of the glyph.
     * Return the stored XML or load it from the path.
     */
    std::string GetXML() const;

private:
    //
public:
    //
private:
    /** The bounding box values of the glyph */
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    /** The horizontal adv x (if any) */
    int m_horizAdvX;
    /** Units per EM for the glyph */
    int m_unitsPerEm;
    /** The Unicode code in hexa as string */
    std::string m_codeStr;
    /** Path to the glyph XML file */
    std::string m_path;
    /** XML of the content for files loaded from zip archive custom font */
    std::string m_xml;
    /** A map of the available anchors */
    std::map<SMuFLGlyphAnchor, Point> m_anchors;
    /** A flag indicating it is a fallback */
    bool m_isFallback;
};

} // namespace vrv

#endif
