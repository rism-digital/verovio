/////////////////////////////////////////////////////////////////////////////
// Name:        system.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SYSTEM_H__
#define __VRV_SYSTEM_H__

#include "aligner.h"
#include "drawinglistinterface.h"
#include "object.h"

namespace vrv {

class DeviceContext;
class Measure;
class ScoreDef;

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

/**
 * This class represents a system in a laid-out score (Doc).
 * A System is contained in a Page.
 * It contains Staff objects.
*/
class System: public DocObject, public DrawingListInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    System();
    virtual ~System();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "System"; };
    ///@}
	
	void AddMeasure( Measure *measure );
    
    void AddScoreDef( ScoreDef *scoreDef );
	
    Measure *GetAtPos( int x );
    
    /**
     * @name Reset the alignment values (m_drawingX, m_drawingXRel, etc.)
     * Called by AlignHorizontally and AlignVertically
     */
    ///@{
    virtual void ResetHorizontalAlignment( );
    virtual void ResetVerticalAlignment( );
    ///@}
    
    /**
     * @name Set and get the labels drawing width.
     */
    ///@{
    int GetDrawingLabelsWidth() const { return m_drawingLabelsWidth; };
    void SetDrawingLabelsWidth( int width );
    ///@}

    /**
     * Return the height of the system
     */
    int GetHeight( );
    
    /**
     * Return the default horizontal spacing of system.
     */
    int GetVerticalSpacing();
    
    /**
     * Return the index position of the system in its page parent
     */
    int GetSystemIdx() const { return Object::GetIdx(); };
    
    //----------//
    // Functors //
    //----------//
        
    /**
     * Align the content of a system vertically.
     */
    virtual int AlignVertically( ArrayPtrVoid params );
    
    /**
     * Set the position of the StaffAlignment.
     * Redirect the functor to the SytemAligner
     */
    virtual int SetAligmentYPos( ArrayPtrVoid params );
    
    /**
     * Correct the Y alignment once the the content of a system has been aligned and laid out.
     * Special case that redirects the functor to the SystemAligner.
     */
    virtual int IntegrateBoundingBoxYShift( ArrayPtrVoid params );
    
    /**
     * Align the system by adjusting the m_drawingYRel position looking at the SystemAligner.
     */
    virtual int AlignSystems( ArrayPtrVoid params );
    
    /**
     * Align the measures by adjusting the m_drawingXRel position looking at the MeasureAligner.
     * In System object resets the shift to 0;
     */
    virtual int AlignMeasures( ArrayPtrVoid params );
    
    /** 
     * Store the width of the system in the MeasureAligner for justification
     */
    virtual int AlignMeasuresEnd( ArrayPtrVoid params );
    
    /**
     * Justify the X positions
     * Special case that redirects the functor to the MeasureAligner.
     */
    virtual int JustifyX( ArrayPtrVoid params );
    
    /**
     * Lay out the Y positions of the staff looking that the bounding boxes of each staff
     * This is the Functor called at the end of the system or a measure.
     */
    virtual int SetBoundingBoxYShiftEnd( ArrayPtrVoid params );
    
    /**
     *
     */
    virtual int CastOffPages( ArrayPtrVoid params );
    
    /**
     * Undo the cast of the system.
     * This is used by Doc::ContinuousLayout
     */
    virtual int UnCastOff( ArrayPtrVoid params );
    
    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY( ArrayPtrVoid params );

    
private:
    SystemAligner m_systemAligner;
    
public:
    /** System left margin (MEI scoredef@system.leftmar). Saved if != 0 */
    int m_systemLeftMar;
    /** System right margin (MEI scoredef@system.rightmar). Saved if != 0 */
    int m_systemRightMar;
	/** 
     * The Y absolute position of the staff for facsimile (transcription) encodings.
     * This is the top left corner of the system.
     */
    int m_yAbs;
    /**
     * The Y relative position of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingYRel;
	/** 
     * The x absolute position of the  system for facsimile layouts.
     * This is the top left corner of the system.
     */
    int m_xAbs;
    /**
     * The X relative position of the system.
     * It is used internally when calculating the layout andd it is not stored in the file.
     */
    int m_drawingXRel;
    /**
     * The width used by the labels at the left of the system.
     * It is used internally when calculating the layout andd it is not stored in the file.
     */
    int m_drawingLabelsWidth;
    /**
     * @name The total width of the system.
     * It is computed during the layout processed and used for calculating the justification ratio.
     */
    ///@{
    int m_drawingTotalWidth;
    ///@}

private:
    
};

} // namespace vrv

#endif
