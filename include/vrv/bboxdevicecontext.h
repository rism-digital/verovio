/////////////////////////////////////////////////////////////////////////////
// Name:        bboxdevicecontext.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BBOX_DC_H__
#define __VRV_BBOX_DC_H__

#include <vector>

//----------------------------------------------------------------------------

#include "devicecontext.h"

#define BBOX_BOTH 0
#define BBOX_HORIZONTAL_ONLY 1
#define BBOX_VERTICAL_ONLY 2

namespace vrv {

class Object;

//----------------------------------------------------------------------------
// BBoxDeviceContext
//----------------------------------------------------------------------------

/**
 * This class calculates the bounding box of the object being drawn.
 * It can be used when doing the layout of the object in order to manage object spacing.
 * The drawing primitives do not draw anything but update the bounding box values of the
 * layout objects currently drawn. The layout objects store their own bounding box and a
 * bounding box of their content. The own bouding box is updated only for the object being
 * drawn (the top one on the stack). The content bounding box is updated for all objects
 * on the stack
 */
class BBoxDeviceContext : public DeviceContext {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    BBoxDeviceContext(View *view, int width, int height, unsigned char update = BBOX_BOTH);
    virtual ~BBoxDeviceContext();
    virtual ClassId GetClassId() const { return BBOX_DEVICE_CONTEXT; }
    ///@}

    /**
     * @name Setters
     */
    ///@{
    virtual void SetBackground(int colour, int style = AxSOLID);
    virtual void SetBackgroundImage(void *image, double opacity = 1.0){};
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
    ///@}

    /**
     * @name Drawing methods
     */
    ///@{
    virtual void DrawSimpleBezierPath(Point bezier[4]);
    virtual void DrawComplexBezierPath(Point bezier1[4], Point bezier2[4]);
    virtual void DrawCircle(int x, int y, int radius);
    virtual void DrawEllipse(int x, int y, int width, int height);
    virtual void DrawEllipticArc(int x, int y, int width, int height, double start, double end);
    virtual void DrawLine(int x1, int y1, int x2, int y2);
    virtual void DrawPolygon(int n, Point points[], int xOffset, int yOffset, int fillStyle = AxODDEVEN_RULE);
    virtual void DrawRectangle(int x, int y, int width, int height);
    virtual void DrawRotatedText(const std::string &text, int x, int y, double angle);
    virtual void DrawRoundedRectangle(int x, int y, int width, int height, int radius);
    virtual void DrawText(const std::string &text, const std::wstring wtext = L"", int x = VRV_UNSET, int y = VRV_UNSET,
        int width = VRV_UNSET, int height = VRV_UNSET);
    virtual void DrawMusicText(const std::wstring &text, int x, int y, bool setSmuflGlyph = false);
    virtual void DrawSpline(int n, Point points[]);
    virtual void DrawSvgShape(int x, int y, int width, int height, pugi::xml_node svg);
    virtual void DrawBackgroundImage(int x = 0, int y = 0){};
    ///@}

    /**
     * Special method for forcing bounding boxes to be updated
     * Used for invisible elements (e.g. <space>) that needs to be take into account in spacing
     */
    virtual void DrawPlaceholder(int x, int y);

    /**
     * @name Method for starting and ending a text
     */
    ///@{
    virtual void StartText(int x, int y, data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_left);
    virtual void EndText();

    /**
     * @name Move a text to the specified position, for example when starting a new line.
     */
    ///@{
    virtual void MoveTextTo(int x, int y, data_HORIZONTALALIGNMENT alignment);
    virtual void MoveTextVerticallyTo(int y);
    ///@}

    /**
     * @name Method for starting and ending a graphic
     */
    ///@{
    virtual void StartGraphic(
        Object *object, std::string gClass, std::string gId, bool primary = true, bool prepend = false);
    virtual void EndGraphic(Object *object, View *view);
    ///@}

    /**
     * @name Methods for re-starting and ending a graphic for objects drawn in separate steps
     */
    ///@{
    virtual void ResumeGraphic(Object *object, std::string gId);
    virtual void EndResumedGraphic(Object *object, View *view);
    ///@}

    /**
     * @name Method for rotating a graphic (clockwise).
     */
    ///@{
    virtual void RotateGraphic(Point const &orig, double angle);
    ///@}

    /**
     * @name Method for starting and ending page
     */
    ///@{
    virtual void StartPage();
    virtual void EndPage();
    ///@}

    bool UpdateHorizontalValues() { return (m_update != BBOX_VERTICAL_ONLY); }
    bool UpdateVerticalValues() { return (m_update != BBOX_HORIZONTAL_ONLY); }

    /**
     * @name Method for adding description element
     */
    ///@{
    virtual void AddDescription(const std::string &text){};
    ///@}

private:
    /**
     *
     */
    void ResetGraphicRotation();

public:
    //
private:
    /**
     * Size and scale members
     */
    ///@{
    int m_width, m_height;
    double m_userScaleX, m_userScaleY;
    ///@}

    /**
     * Indicates whereas we need to update bounding boxes horizontally and/or vertically
     */
    unsigned char m_update;

    /**
     * The rotation values by RotateGraphic
     */
    ///@{{
    Point m_rotationOrigin;
    double m_rotationAngle;
    ///@}

    /**
     * members for keeping track of the text bounding box.
     * Set values are reset in StartText
     */
    int m_textX, m_textY, m_textWidth, m_textHeight, m_textAscent, m_textDescent;
    bool m_drawingText;
    data_HORIZONTALALIGNMENT m_textAlignment;

    /**
     * The array containing the object for which the bounding box needs to be updated
     */
    std::vector<Object *> m_objects;

    /**
     * The view are calling from - used to flip back the Y coordinates
     */
    View *m_view;

    void UpdateBB(int x1, int y1, int x2, int y2, wchar_t glyph = 0);
};

} // namespace vrv

#endif // __VRV_BBOX_DC_H__
