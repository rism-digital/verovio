/////////////////////////////////////////////////////////////////////////////
// Name:        measure.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_MEASURE_H__
#define __VRV_MEASURE_H__

#include "aligner.h"
#include "atts_shared.h"
#include "barline.h"
#include "object.h"

namespace vrv {

class MeasureElement;

//----------------------------------------------------------------------------
// Measure
//----------------------------------------------------------------------------

/**
 * This class represents a measure in a page-based score (Doc).
 * A Measure is contained in a Staff.
 * It contains Layer objects.
 * For internally simplication of processing, unmeasure music is contained in one single measure object
 */
class Measure: public DocObject,
    public AttCommon,
    public AttMeasureLog
{
    
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Measure( bool measuredMusic = true, int logMeasureNb = -1 );
    virtual ~Measure();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Measure"; };
    ///@}
    
    /**
     * Return true if measured music (otherwise we have fakes measures)
     */
    bool IsMeasuredMusic() { return m_measuredMusic; };
    
	void AddMeasureElement( MeasureElement *element );
	
	int GetStaffCount() const { return (int)m_children.size(); };
    
    /**
     * Return the index position of the measure in its system parent
     */
    int GetMeasureIdx() const { return Object::GetIdx(); };
    
    /**
     * @name Set and get the left and right barLine types
     * This somehow conflicts with AttMeasureLog, which are transfered from and to the 
     * Barline object when reading and writing MEI. See MeiInput::ReadMeiMeasure and
     * MeiOutput::ReadMeiMeasure
     * Alternatively, we could keep them in sync here:
     * data_BARRENDITION GetLeftBarlineType() { m_leftBarline.SetRend( GetRight() ); return m_leftBarline.GetRend(); };
     * void SetLeftBarlineType( data_BARRENDITION type ) { m_leftBarline.SetRend(type); SetLeft(type); };
     */
    ///@{
    data_BARRENDITION GetLeftBarlineType() const { return m_leftBarline.GetRend(); };
    void SetLeftBarlineType( data_BARRENDITION type ) { m_leftBarline.SetRend(type); };
    data_BARRENDITION GetRightBarlineType() const { return m_rightBarline.GetRend(); };
    void SetRightBarlineType( data_BARRENDITION type ) { m_rightBarline.SetRend(type); };
    ///@}
    
    /**
     * @name Set and get the barlines. 
     * Careful - the barlines are owned by the measure and will be destroy by it.
     * This method should be used only for acessing them (e.g., when drawing) and 
     * not for creating other measure objects.
     */
    ///@{
    Barline *GetLeftBarline() { return &m_leftBarline; };
    Barline *GetRightBarline() { return &m_rightBarline; };
    ///@}
    
    int GetXRel( );
    
    /** 
     * Return the X rel position of the right barLine (without its width)
     */
    int GetRightBarlineX( );
    
    /**
     * Return the width of the measure, including the barLine width
     */
    int GetWidth( );
    
    //----------//
    // Functors //
    //----------//
        
    /**
     * Reset the alignment values (m_drawingX, m_drawingXRel, etc.)
     * Called by AlignHorizontally
     */
    virtual void ResetHorizontalAlignment( );
    
    /**
     * AlignHorizontally the content of a measure.
     */
    virtual int AlignHorizontally( ArrayPtrVoid params );

    /**
     * AlignVertically the content of a measure.
     */
    virtual int AlignVertically( ArrayPtrVoid params );
    
    /**
     * Correct the X alignment once the the content of a system has been aligned and laid out.
     * Special case that redirects the functor to the MeasureAligner.
     */
    virtual int IntegrateBoundingBoxXShift( ArrayPtrVoid params );
    
    /**
     * Set the position of the Alignment.
     * Special case that redirects the functor to the MeasureAligner.
     */
    virtual int SetAligmentXPos( ArrayPtrVoid params );
    
    /**
     * Align the measures by adjusting the m_drawingXRel position looking at the MeasureAligner.
     * This method also moves the end position of the measure according to the barLine width.
     */
    virtual int AlignMeasures( ArrayPtrVoid params );
    
    /**
     * Justify the X positions
     * Special case that redirects the functor to the MeasureAligner.
     */
    virtual int JustifyX( ArrayPtrVoid params );
    
    /**
     * Fill a page by adding systems with the appropriate length
     *
     */
    virtual int CastOffSystems( ArrayPtrVoid params );
    
    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY( ArrayPtrVoid params );
    
        
public:
	/**
     * The X absolute position of the measure for facsimile (transcription) encodings.
     * This is the top left position of the measure.
     */
    int m_xAbs;
    /**
     * The X relative position of the measure.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingXRel;
    
private:
    bool m_measuredMusic;
    
    MeasureAligner m_measureAligner;
    
    /**
     * @name The measure barlines (left and right) used when drawing
     */
    ///@{
    BarlineAttr m_leftBarline;
    BarlineAttr m_rightBarline;
    ///@}
};

} // namespace vrv

#endif
