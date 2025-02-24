/////////////////////////////////////////////////////////////////////////////
// Name:        devicecontext.h
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DC_H__
#define __VRV_DC_H__

#include <stack>
#include <string>

//----------------------------------------------------------------------------

#include "devicecontextbase.h"
#include "resources.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

namespace vrv {

class Glyph;
class Object;
class View;
class Zone;

extern "C" {
static inline double DegToRad(double deg)
{
    return (deg * M_PI) / 180.0;
}
static inline double RadToDeg(double deg)
{
    return (deg * 180.0) / M_PI;
}
}

// ---------------------------------------------------------------------------
// DeviceContext
// ---------------------------------------------------------------------------

/**
 * This class is an abstract device context.
 * It enables different types of concrete classes to be implemented.
 * For example:
 *  MusWxDC - a wrapper to wxDCs with wxWidgets;
 *  SvgDeviceContext - a non-gui file DC;
 *  MusCairoDC - a wrapper to a Cairo surface;
 * The class uses int-based color encoding (instead of wxColor in wxDC).
 * It uses FontInfo (instead of wxFont in wxDC).
 */

class DeviceContext {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    DeviceContext()
    {
        m_classId = DEVICE_CONTEXT;
        m_resources = NULL;
        m_isDeactivatedX = false;
        m_isDeactivatedY = false;
        m_width = 0;
        m_height = 0;
        m_contentHeight = 0;
        m_userScaleX = 1.0;
        m_userScaleY = 1.0;
        m_baseWidth = 0;
        m_baseHeight = 0;
        m_pushBack = false;
        m_viewBoxFactor = (double)DEFINITION_FACTOR;
        this->SetBrush(1.0);
        this->SetPen(1, PEN_SOLID);
    }
    DeviceContext(ClassId classId)
    {
        m_classId = classId;
        m_resources = NULL;
        m_isDeactivatedX = false;
        m_isDeactivatedY = false;
        m_width = 0;
        m_height = 0;
        m_contentHeight = 0;
        m_userScaleX = 1.0;
        m_userScaleY = 1.0;
        m_baseWidth = 0;
        m_baseHeight = 0;
        m_pushBack = false;
        m_viewBoxFactor = (double)DEFINITION_FACTOR;
        this->SetBrush(1.0);
        this->SetPen(1, PEN_SOLID);
    }
    virtual ~DeviceContext();
    ClassId GetClassId() const { return m_classId; }
    bool Is(ClassId classId) const { return (m_classId == classId); }
    ///@}

    /**
     * @name Getter and setter for the resources
     * Resources must be set before drawing
     */
    ///@{
    const Resources *GetResources(bool showWarning = false) const;
    bool HasResources() const { return (m_resources != NULL); }
    void SetResources(const Resources *resources) { m_resources = resources; }
    void ResetResources() { m_resources = NULL; }
    ///@}

    /**
     * @name Getters and setters for common attributes.
     * Non-virtual methods cannot be overridden and manage the width, height and user-scale
     */
    ///@{
    void SetWidth(int width) { m_width = width; }
    void SetHeight(int height) { m_height = height; }
    void SetContentHeight(int height) { m_contentHeight = height; }
    void SetUserScale(double scaleX, double scaleY)
    {
        m_userScaleX = scaleX;
        m_userScaleY = scaleY;
    }
    void SetBaseSize(int width, int height)
    {
        m_baseWidth = width;
        m_baseHeight = height;
    }
    void SetViewBoxFactor(double ppuFactor);
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetContentHeight() const { return m_contentHeight; }
    double GetUserScaleX() { return m_userScaleX; }
    double GetUserScaleY() { return m_userScaleY; }
    std::pair<int, int> GetBaseSize() const { return std::make_pair(m_baseWidth, m_baseHeight); }
    double GetViewBoxFactor() const { return m_viewBoxFactor; }
    ///@}

    /**
     * @name Setters
     * Non-virtual methods cannot be overridden and manage the Pen, Brush and FontInfo stacks
     */
    ///@{
    void SetBrush(float opacity, int color = COLOR_NONE);
    void SetPen(int width, PenStyle style, int dashLength = 0, int gapLength = 0,
        LineCapStyle lineCap = LINECAP_DEFAULT, LineJoinStyle lineJoin = LINEJOIN_DEFAULT, float opacity = 1.0,
        int color = COLOR_NONE);
    void SetFont(FontInfo *font);
    void SetPushBack() { m_pushBack = true; }
    void ResetBrush();
    void ResetPen();
    void ResetFont();
    void ResetPushBack() { m_pushBack = false; }
    virtual void SetBackground(int color, int style = PEN_SOLID) = 0;
    virtual void SetBackgroundImage(void *image, double opacity = 1.0) = 0;
    virtual void SetBackgroundMode(int mode) = 0;
    virtual void SetTextForeground(int color) = 0;
    virtual void SetTextBackground(int color) = 0;
    virtual void SetLogicalOrigin(int x, int y) = 0;
    ///}

    /**
     * @name Getters
     */
    ///@{
    FontInfo *GetFont();
    bool HasFont() const { return !m_fontStack.empty(); }
    ///@}

    /**
     * @name Getters for text extend (non-virtual)
     */
    ///@{
    virtual void GetTextExtent(const std::string &string, TextExtend *extend, bool typeSize);
    virtual void GetTextExtent(const std::u32string &string, TextExtend *extend, bool typeSize);
    virtual void GetSmuflTextExtent(const std::u32string &string, TextExtend *extend);

    /**
     * @name Getters
     */
    ///@{
    virtual Point GetLogicalOrigin() = 0;
    ///@}

    /**
     * @name Drawing methods
     */
    ///@{
    virtual void DrawQuadBezierPath(Point bezier[3]) = 0;
    virtual void DrawCubicBezierPath(Point bezier[4]) = 0;
    virtual void DrawCubicBezierPathFilled(Point bezier1[4], Point bezier2[4]) = 0;
    virtual void DrawBentParallelogramFilled(Point side[4], int height) = 0;
    virtual void DrawCircle(int x, int y, int radius) = 0;
    virtual void DrawEllipse(int x, int y, int width, int height) = 0;
    virtual void DrawEllipticArc(int x, int y, int width, int height, double start, double end) = 0;
    virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;
    virtual void DrawPolyline(int n, Point points[], bool close = false) = 0;
    virtual void DrawPolygon(int n, Point points[]) = 0;
    virtual void DrawRectangle(int x, int y, int width, int height) = 0;
    virtual void DrawRotatedText(const std::string &text, int x, int y, double angle) = 0;
    virtual void DrawRoundedRectangle(int x, int y, int width, int height, int radius) = 0;
    virtual void DrawText(const std::string &text, const std::u32string &wtext = U"", int x = VRV_UNSET,
        int y = VRV_UNSET, int width = VRV_UNSET, int height = VRV_UNSET)
        = 0;
    virtual void DrawMusicText(const std::u32string &text, int x, int y, bool setSmuflGlyph = false) = 0;
    virtual void DrawSpline(int n, Point points[]) = 0;
    virtual void DrawGraphicUri(int x, int y, int width, int height, const std::string &uri) = 0;
    virtual void DrawSvgShape(int x, int y, int width, int height, double scale, pugi::xml_node svg) = 0;
    virtual void DrawBackgroundImage(int x = 0, int y = 0) = 0;
    ///@}

    /**
     * Special method for forcing bounding boxes to be updated
     * Used for invisible elements (e.g., <space>) that needs to be take into account in spacing
     */
    virtual void DrawPlaceholder(int x, int y) {}

    /**
     * @name Method for starting and ending a text
     * Once started, DrawText should be called for actually drawing the text.
     * Font can be changed between called for DrawText
     */
    ///@{
    virtual void StartText(int x, int y, data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_left) = 0;
    virtual void EndText() = 0;

    /**
     * @name Move a text to the specified position, for example when starting a new line.
     * These methods should be called only between a StartText and EndText call.
     */
    ///@{
    virtual void MoveTextTo(int x, int y, data_HORIZONTALALIGNMENT alignment) = 0;
    virtual void MoveTextVerticallyTo(int y) = 0;
    ///@}

    /**
     * @name Temporarily deactivate a graphic
     * This can be used for example for not taking into account the bounding box of parts of the graphic.
     * One example is the connectors in lyrics.
     * This is a non-virtual method and hence cannot be overridden. In only changes a flag. The effect of the flag
     * has to be defined in the child class. It should not be called twice in a row.
     * Is it also possible to deactivate only X or Y axis. Reactivate will reactivate both axis.
     */
    ///@{
    void DeactivateGraphic();
    void DeactivateGraphicX();
    void DeactivateGraphicY();
    void ReactivateGraphic();
    ///@}

    /**
     * @name Method for starting and ending a graphic
     * For example, the method can be used for grouping shapes in <g></g> in SVG
     */
    ///@{
    virtual void StartGraphic(Object *object, const std::string &gClass, const std::string &gId,
        GraphicID graphicID = PRIMARY, bool preprend = false)
        = 0;
    virtual void EndGraphic(Object *object, View *view) = 0;
    ///@}

    /**
     * @name Method for starting and ending a graphic custom graphic that do not correspond to an Object
     * For example, the method can be used for grouping shapes in <g></g> in SVG
     */
    ///@{
    virtual void StartCustomGraphic(const std::string &name, std::string gClass = "", std::string gId = "") {}
    virtual void EndCustomGraphic() {}
    ///@}

    /**
     * Method for changing the color of a custom graphic
     */
    virtual void SetCustomGraphicColor(const std::string &color) {}

    /**
     * Method for adding custom graphic data-* attributes
     */
    virtual void SetCustomGraphicAttributes(const std::string &data, const std::string &value) {}

    /**
     * @name Methods for re-starting and ending a graphic for objects drawn in separate steps
     * The methods can be used to the output together, for example for a Beam
     */
    ///@{
    virtual void ResumeGraphic(Object *object, std::string gId) = 0;
    virtual void EndResumedGraphic(Object *object, View *view) = 0;
    ///@}

    /**
     * @name Method for starting and ending a text graphic when it needs to be different from a normal graphic.
     * For example, in SVG, a text graphic is a <tspan> (and not a <g>)
     */
    ///@{
    virtual void StartTextGraphic(Object *object, const std::string &gClass, const std::string &gId)
    {
        StartGraphic(object, gClass, gId);
    }
    virtual void EndTextGraphic(Object *object, View *view) { EndGraphic(object, view); }
    ///@}

    /**
     * @name Method for rotating a graphic (clockwise).
     * This should be called only once per graphic and before drawing anything in it.
     */
    ///@{
    virtual void RotateGraphic(Point const &orig, double angle) = 0;
    ///@}

    /**
     * @name Method for starting and ending page
     */
    ///@{
    virtual void StartPage() = 0;
    virtual void EndPage() = 0;
    ///@}

    /**
     * @name Method for adding description element
     */
    ///@{
    virtual void AddDescription(const std::string &text) {}
    ///@}

    /**
     * Method indicating if default global styling is used. Typically this is the case with SVG and CSS.
     * When global styling is used, some elements will not set corresponding styles.
     * Global styling is false by default.
     */
    virtual bool UseGlobalStyling() { return false; }

    //----------------//
    // Static methods //
    //----------------//

    /** Color conversion method **/
    static int RGB2Int(char red, char green, char blue) { return (red << 16 | green << 8 | blue); }

private:
    void AddGlyphToTextExtend(const Glyph *glyph, TextExtend *extend);

public:
    //
protected:
    std::stack<Pen> m_penStack;
    std::stack<Brush> m_brushStack;
    std::stack<FontInfo *> m_fontStack;

    /** flag for indicating if the graphic is deactivated */
    bool m_isDeactivatedX;
    bool m_isDeactivatedY;

    /** push back mode */
    bool m_pushBack;

    Zone *m_facsimile = NULL;

private:
    /** The class id representing the actual (derived) class */
    ClassId m_classId;

    /** The resources (not owned by the device context) */
    const Resources *m_resources;

    /** stores the width and height of the device context */
    int m_width;
    int m_height;

    /** stores base width and height of the device context before application of scale */
    int m_baseWidth;
    int m_baseHeight;

    /** stores the height of graphic content */
    int m_contentHeight;

    /** stores the scale as requested by the used */
    double m_userScaleX;
    double m_userScaleY;

    /** stores the viewbox factor taking into account the DEFINTION_FACTOR and the PPU */
    double m_viewBoxFactor;
};

} // namespace vrv

#endif // __AX_DC_H__
