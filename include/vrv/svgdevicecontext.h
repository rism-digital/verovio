/////////////////////////////////////////////////////////////////////////////
// Name:        svgdevicecontext.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SVG_DC_H__
#define __VRV_SVG_DC_H__

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "pugixml.hpp"

namespace vrv {

//----------------------------------------------------------------------------
// BBoxDeviceContext
//----------------------------------------------------------------------------

/**
 * This class implements a drawing context for generating SVG files.
 * The music font is embedded by incorporating ./data/[fontname]/[glyph].xml glyphs within
 * the SVG file.
 */
class SvgDeviceContext : public DeviceContext {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    SvgDeviceContext(int width, int height);
    virtual ~SvgDeviceContext();
    ///@}

    /**
     * @name Setters
     */
    ///@{
    virtual void SetBackground(int colour, int style = AxSOLID);
    virtual void SetBackgroundImage(void *image, double opacity = 1.0);
    virtual void SetBackgroundMode(int mode);
    virtual void SetTextForeground(int colour);
    virtual void SetTextBackground(int colour);
    virtual void SetLogicalOrigin(int x, int y);
    virtual void SetUserScale(double xScale, double yScale);
    ///@}

    /**
     * @name Getters
     */
    ///@{
    virtual Point GetLogicalOrigin();
    ///}

    /**
     * Get the the SVG into a string
     * Add the xml tag if necessary.
     */
    std::string GetStringSVG(bool xml_declaration = false);

    /**
     * @name Drawing methods
     */
    ///@{
    virtual void DrawComplexBezierPath(int x, int y, int bezier1_coord[6], int bezier2_coord[6]);
    virtual void DrawCircle(int x, int y, int radius);
    virtual void DrawEllipse(int x, int y, int width, int height);
    virtual void DrawEllipticArc(int x, int y, int width, int height, double start, double end);
    virtual void DrawLine(int x1, int y1, int x2, int y2);
    virtual void DrawPolygon(int n, Point points[], int xoffset, int yoffset, int fill_style = AxODDEVEN_RULE);
    virtual void DrawRectangle(int x, int y, int width, int height);
    virtual void DrawRotatedText(const std::string &text, int x, int y, double angle);
    virtual void DrawRoundedRectangle(int x, int y, int width, int height, double radius);
    virtual void DrawText(const std::string &text, const std::wstring wtext = L"");
    virtual void DrawMusicText(const std::wstring &text, int x, int y);
    virtual void DrawSpline(int n, Point points[]);
    virtual void DrawBackgroundImage(int x = 0, int y = 0);
    ///@}

    /**
     * @name Method for starting and ending a text
     */
    ///@{
    virtual void StartText(int x, int y, char alignement = LEFT);
    virtual void EndText();

    /**
     * Move a text to the specified position, for example when starting a new line.
     */
    virtual void MoveTextTo(int x, int y);

    /**
     * @name Method for starting and ending a graphic
     */
    ///@{
    virtual void StartGraphic(DocObject *object, std::string gClass, std::string gId);
    virtual void EndGraphic(DocObject *object, View *view);
    ///@}

    /**
     * @name Methods for re-starting and ending a graphic for objects drawn in separate steps
     */
    ///@{
    virtual void ResumeGraphic(DocObject *object, std::string gId);
    virtual void EndResumedGraphic(DocObject *object, View *view);
    ///@}

    /**
     * @name Method for starting and ending a text (<tspan>) text graphic.
     */
    ///@{
    virtual void StartTextGraphic(DocObject *object, std::string gClass, std::string gId);
    virtual void EndTextGraphic(DocObject *object, View *view);
    ///@}

    /**
     * @name Method for starting and ending page
     */
    ///@{
    virtual void StartPage();
    virtual void EndPage();
    ///@}

private:
    /**
     * Copy the content of a file to the output stream.
     * This is used for copying <defs> items.
     */
    bool CopyFileToStream(const std::string &filename, std::ostream &dest);

    /**
     * Internal method for drawing debug SVG bounding box
     */
    void DrawSvgBoundingBox(DocObject *object, View *view);

    /**
     * Change the flag for indicating the use of the VerovioText font
     */
    void VrvTextFont() { m_vrvTextFont = true; };

    /**
     * Flush the data to the internal buffer.
     * Adds the xml tag if necessary and the <defs> from m_smufl_glyphs
     */
    void Commit(bool xml_declaration);

    void WriteLine(std::string);

    std::string GetColour(int colour);

public:
    //
private:
    /**
     * Flag for indicating if the VerovioText font is currently used.
     * If used, it has to be initialized to false (e.g., in the overriden version of StartPage) and will be changed in
     * DeviceContext::VrvTextFont
     */
    bool m_vrvTextFont;

    // we use a std::stringstream because we want to prepend the <defs> which will know only when we reach the end of
    // the page
    // some viewer seem to support to have the <defs> at the end, but some do not (pdf2svg, for example)
    // for this reason, the full svg is finally written a string from the destructor or when Flush() is called
    std::ostringstream m_outdata;

    bool m_committed; // did we flushed the file?
    int m_width, m_height;
    int m_originX, m_originY;
    double m_userScaleX, m_userScaleY;

    // holds the list of glyphs from the smufl font used so far
    // they will be added at the end of the file as <defs>
    std::vector<std::string> m_smufl_glyphs;

    // pugixml data
    pugi::xml_document m_svgDoc;
    pugi::xml_node m_svgNode;
    pugi::xml_node m_currentNode;
    std::list<pugi::xml_node> m_svgNodeStack;
};

} // namespace vrv

#endif // __VRV_SVG_DC_H__
