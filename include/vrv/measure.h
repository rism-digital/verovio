/////////////////////////////////////////////////////////////////////////////
// Name:        measure.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_MEASURE_H__
#define __VRV_MEASURE_H__

#include "object.h"

#include "aligner.h"
#include "barline.h"

namespace vrv {

class Staff;

//----------------------------------------------------------------------------
// Measure
//----------------------------------------------------------------------------

/**
 * This class represents a measure in a page-based score (Doc).
 * A Measure is contained in a Staff.
 * It contains Layer objects.
 * For internally simplication of processing, unmeasure music is contained in one single measure object
 */
class Measure: public DocObject
{
    
public:
    // constructors and destructors
    Measure( bool measuredMusic = true, int logMeasureNb = -1 );
    virtual ~Measure();
    
    virtual std::string GetClassName( ) { return "Measure"; };
    
    /**
     * Return true if measured music (otherwise we have fakes measures)
     */
    bool IsMeasuredMusic() { return m_measuredMusic; };
    
    void Clear();
    
	void AddStaff( Staff *staff );
	
	int GetStaffCount() const { return (int)m_children.size(); };
    
    /**
     * Return the index position of the measure in its system parent
     */
    int GetMeasureNo() const { return Object::GetIdx(); };
    
    /**
     * @name Set and get the left and right barline types
     */
    ///@{
    BarlineType GetLeftBarlineType() const { return m_leftBarline.m_barlineType; };
    void SetLeftBarlineType( BarlineType type ) { m_leftBarline.m_barlineType = type; };
    BarlineType GetRightBarlineType() const { return m_rightBarline.m_barlineType; };
    void SetRightBarlineType( BarlineType type ) { m_rightBarline.m_barlineType = type; };
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
       
    // functors
    virtual int Save( ArrayPtrVoid params );
    
	void CopyAttributes( Measure *measure ); // copy all attributes but none of the elements
    
	Staff *GetFirst( );
	Staff *GetLast( );
	Staff *GetNext( Staff *layer );
	Staff *GetPrevious( Staff *layer );
    Staff *GetStaffWithIdx( int staffIdx );
    
    Staff *GetStaffWithNo( int staffNo );
    
    int GetXRel( );
    
    int GetXRelRight( );
    
    /**
     * Reset the drawing values (m_drawingX, m_drawingXRel, etc.)
     * Called by SetCurrentScoreDef functor.
     */
    virtual void ResetDrawingValues( );
    
    /**
     * Align the content of a system.
     */
    virtual int Align( ArrayPtrVoid params );
    
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
     * This method also moves the end position of the measure according to the barline width.
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
        
public:
    /** The logical staff */
    int m_logMeasureNb;
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
	/**
     * The X drawing position of the measure.
     * It is re-computed everytime the measure is drawn and it is not stored in the file.
     */
    int m_drawingX;
    
private:
    bool m_measuredMusic;
    
    MeasureAligner m_measureAligner;
    
    /**
     * @name The measure barlines (left and right) used when drawing
     */
    ///@{
    Barline m_leftBarline;
    Barline m_rightBarline;
    ///@}
};

} // namespace vrv

#endif
