/////////////////////////////////////////////////////////////////////////////
// Name:        bboxdevicecontext.h 
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.   
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_BBOX_DC_H__
#define __VRV_BBOX_DC_H__

#include "devicecontext.h"
#include "object.h"

namespace vrv {

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

    BBoxDeviceContext ( View *view, int width, int height );
    virtual ~BBoxDeviceContext();
    
    // Setters
    
    virtual void SetBrush( int colour, int style = AxSOLID );
    
    virtual void SetBackground( int colour, int style = AxSOLID );
    
    virtual void SetBackgroundImage( void *image, double opacity = 1.0 ) {};
    
    virtual void SetBackgroundMode( int mode );
    
    virtual void SetPen( int colour, int width = 1, int style = AxSOLID );
    
    virtual void SetFont( FontMetricsInfo *font_info );
        
    virtual void SetTextForeground( int colour );
    
    virtual void SetTextBackground( int colour );
    
    virtual void ResetBrush( );
    
    virtual void ResetPen( );
    
    virtual void SetLogicalOrigin( int x, int y );
    
    virtual void SetUserScale( double xScale, double yScale );
    
    // Getters
    
    virtual void GetTextExtent( const std::string& string, int *w, int *h );
    
    virtual MusPoint GetLogicalOrigin( );

    // Drawing methods
        
    virtual void DrawComplexBezierPath(int x, int y, int bezier1_coord[6], int bezier2_coord[6]);
    
    virtual void DrawCircle(int x, int y, int radius);
    
    virtual void DrawEllipse(int x, int y, int width, int height);
    
    virtual void DrawEllipticArc(int x, int y, int width, int height, double start, double end);
    
    virtual void DrawLine(int x1, int y1, int x2, int y2);
    
    virtual void DrawPolygon(int n, MusPoint points[], int xoffset, int yoffset, int fill_style = AxODDEVEN_RULE);
    
    virtual void DrawRectangle(int x, int y, int width, int height);
    
    virtual void DrawRotatedText(const std::string& text, int x, int y, double angle);
    
    virtual void DrawRoundedRectangle(int x, int y, int width, int height, double radius);
    
    virtual void DrawText(const std::string& text, int x, int y);
    
    virtual void DrawMusicText(const std::string& text, int x, int y);
    
    virtual void DrawSpline(int n, MusPoint points[]);
    
    virtual void DrawBackgroundImage( int x = 0, int y = 0 ) {};
    
    // 
    virtual void StartGraphic( DocObject *object, std::string gClass, std::string gId );
    
    virtual void EndGraphic(DocObject *object, View *view );
    
    virtual void StartPage();
    
    virtual void EndPage();
    
    
private:
    int m_width, m_height;
    double m_userScaleX, m_userScaleY;
    
    /**
     * The array containing the object for which the bounding box needs to be updated
     */ 
    std::vector<DocObject*> m_objects;
    
    FontMetricsInfo m_font;
    
    /**
     * The view are calling from - used to flip back the Y coordinates
     */
    View *m_view;
   
    //
    int m_penWidth;
    
    void UpdateBB(int x1, int y1, int x2, int y2);
    
    void FindPointsForBounds(MusPoint P0, MusPoint P1, MusPoint P2, MusPoint P3, int *ret);
};

} // namespace vrv

#endif // __VRV_BBOX_DC_H__
