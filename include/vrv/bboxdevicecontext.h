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
 * bounding box of their content. The own bounding box is updated only for the object being
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
    ///@}

    /**
     * @name Setters
     */
    ///@{
    void SetBackground(int color, int style = PEN_SOLID) override;
    void SetBackgroundImage(void *image, double opacity = 1.0) override {};
    void SetBackgroundMode(int mode) override;
    void SetTextForeground(int color) override;
    void SetTextBackground(int color) override;
    void SetLogicalOrigin(int x, int y) override;
    void SetUserScale(double xScale, double yScale);
    ///@}

    /**
     * @name Getters
     */
    ///@{
    Point GetLogicalOrigin() override;
    ///@}

    /**
     * @name Drawing methods
     */
    ///@{
    void DrawQuadBezierPath(Point bezier[3]) override;
    void DrawCubicBezierPath(Point bezier[4]) override;
    void DrawCubicBezierPathFilled(Point bezier1[4], Point bezier2[4]) override;
    void DrawBentParallelogramFilled(Point side[4], int height) override;
    void DrawCircle(int x, int y, int radius) override;
    void DrawEllipse(int x, int y, int width, int height) override;
    void DrawEllipticArc(int x, int y, int width, int height, double start, double end) override;
    void DrawLine(int x1, int y1, int x2, int y2) override;
    void DrawPolyline(int n, Point points[], bool close) override;
    void DrawPolygon(int n, Point points[]) override;
    void DrawRectangle(int x, int y, int width, int height) override;
    void DrawRotatedText(const std::string &text, int x, int y, double angle) override;
    void DrawRoundedRectangle(int x, int y, int width, int height, int radius) override;
    void DrawText(const std::string &text, const std::u32string &wtext = U"", int x = VRV_UNSET, int y = VRV_UNSET,
        int width = VRV_UNSET, int height = VRV_UNSET) override;
    void DrawMusicText(const std::u32string &text, int x, int y, bool setSmuflGlyph = false) override;
    void DrawSpline(int n, Point points[]) override;
    void DrawGraphicUri(int x, int y, int width, int height, const std::string &uri) override;
    void DrawSvgShape(int x, int y, int width, int height, double scale, pugi::xml_node svg) override;
    void DrawBackgroundImage(int x = 0, int y = 0) override {};
    ///@}

    /**
     * Special method for forcing bounding boxes to be updated
     * Used for invisible elements (e.g., <space>) that needs to be take into account in spacing
     */
    void DrawPlaceholder(int x, int y) override;

    /**
     * @name Method for starting and ending a text
     */
    ///@{
    void StartText(int x, int y, data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_left) override;
    void EndText() override;

    /**
     * @name Move a text to the specified position, for example when starting a new line.
     */
    ///@{
    void MoveTextTo(int x, int y, data_HORIZONTALALIGNMENT alignment) override;
    void MoveTextVerticallyTo(int y) override;
    ///@}

    /**
     * @name Method for starting and ending a graphic
     */
    ///@{
    void StartGraphic(Object *object, const std::string &gClass, const std::string &gId, GraphicID graphicID = PRIMARY,
        bool prepend = false) override;
    void EndGraphic(Object *object, View *view) override;
    ///@}

    /**
     * @name Methods for re-starting and ending a graphic for objects drawn in separate steps
     */
    ///@{
    void ResumeGraphic(Object *object, std::string gId) override;
    void EndResumedGraphic(Object *object, View *view) override;
    ///@}

    /**
     * @name Method for rotating a graphic (clockwise).
     */
    ///@{
    void RotateGraphic(Point const &orig, double angle) override;
    ///@}

    /**
     * @name Method for starting and ending page
     */
    ///@{
    void StartPage() override;
    void EndPage() override;
    ///@}

    bool UpdateHorizontalValues() { return (m_update != BBOX_VERTICAL_ONLY); }
    bool UpdateVerticalValues() { return (m_update != BBOX_HORIZONTAL_ONLY); }

    /**
     * @name Method for adding description element
     */
    ///@{
    void AddDescription(const std::string &text) override {};
    ///@}

private:
    /**
     *
     */
    void ResetGraphicRotation();

    /**
     * Get the overlap due to pen width on the left/right
     */
    std::pair<int, int> GetPenWidthOverlap() const;

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

    void UpdateBB(int x1, int y1, int x2, int y2, char32_t glyph = 0);
};

} // namespace vrv

#endif // __VRV_BBOX_DC_H__
