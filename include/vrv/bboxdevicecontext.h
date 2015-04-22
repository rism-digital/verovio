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

namespace vrv {
    
class Object;

//----------------------------------------------------------------------------
// BBoxDeviceContext
//----------------------------------------------------------------------------

/** 
 * This class calculates the bouding box of the object that are drawn.
 * It can be used when doing the layout of the object in order to manage object spacing.
 * The drawing primitives do not draw anything but update the bounding box values of the 
 * layout objects currently drawn. The layout objects store their own bounding box and a
 * bounding box of their content. The own bouding box is updated only for the object being
 * drawn (the top one on the stack). The content bounding box is updated for all objects
 * the stack
 */
class BBoxDeviceContext: public DeviceContext
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    BBoxDeviceContext ( View *view, int width, int height );
    virtual ~BBoxDeviceContext();
    ///@}
    
    /**
     * @name Setters
     */
    ///@{
    virtual void SetBackground( int colour, int style = AxSOLID );
    virtual void SetBackgroundImage( void *image, double opacity = 1.0 ) {};
    virtual void SetBackgroundMode( int mode );
    virtual void SetTextForeground( int colour );
    virtual void SetTextBackground( int colour );
    virtual void SetLogicalOrigin( int x, int y );
    virtual void SetUserScale( double xScale, double yScale );
    ///@}
    
    /**
     * @name Getters
     */
    ///@{
    virtual Point GetLogicalOrigin( );
    ///@}

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
    virtual void DrawRotatedText(const std::string& text, int x, int y, double angle);
    virtual void DrawRoundedRectangle(int x, int y, int width, int height, double radius);
    virtual void DrawText(const std::string& text, const std::wstring wtext = L"");
    virtual void DrawMusicText(const std::wstring& text, int x, int y);
    virtual void DrawSpline(int n, Point points[]);
    virtual void DrawBackgroundImage( int x = 0, int y = 0 ) {};
    ///@}
    
    /**
     * Special method for forcing bounding boxes to be updated
     * Used for invisible elements (e.g. <space>) that needs to be take into account in spacing
     */
    virtual void DrawPlaceholder( int x, int y );
    
    /**
     * @name Method for starting and ending a text
     */
    ///@{
    virtual void StartText(int x, int y, char alignement = LEFT );
    virtual void EndText();
    
    /**
     * @name Method for starting and ending a graphic
     */
    ///@{
    virtual void StartGraphic( DocObject *object, std::string gClass, std::string gId );
    virtual void EndGraphic(DocObject *object, View *view );
    ///@}
    
    /**
     * @name Methods for re-starting and ending a graphic for objects drawn in separate steps
     */
    ///@{
    virtual void ResumeGraphic( DocObject *object, std::string gId );
    virtual void EndResumedGraphic( DocObject *object, View *view  );
    ///@}
    
    /**
     * @name Method for starting and ending page
     */
    ///@{
    virtual void StartPage();
    virtual void EndPage();
    ///@}
    
    
private:
    int m_width, m_height;
    double m_userScaleX, m_userScaleY;
    
    /**
     * members for keeping track of the text bounding box.
     * Set values are reset in StartText
     */
    int m_textX, m_textY, m_textWidth, m_textHeight;
    bool m_drawingText;
    
    /**
     * The array containing the object for which the bounding box needs to be updated
     */ 
    std::vector<DocObject*> m_objects;
    
    /**
     * The view are calling from - used to flip back the Y coordinates
     */
    View *m_view;
    
    void UpdateBB(int x1, int y1, int x2, int y2);
    
    void FindPointsForBounds(Point P0, Point P1, Point P2, Point P3, int *ret);
};

} // namespace vrv

#endif // __VRV_BBOX_DC_H__
