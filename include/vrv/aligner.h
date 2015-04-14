/////////////////////////////////////////////////////////////////////////////
// Name:        aligner.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_ALIGNER_H__
#define __VRV_ALIGNER_H__

#include "object.h"

namespace vrv {

class SystemAligner;
class StaffAlignment;
class MeasureAligner;

/**
 * Alignment types for aligning types together.
 * For example, we align notes and rests (default) together, clef separately, etc.
 * The container is a generic alignment for tuplet, chords, beams, etc.; we needs
 * this to avoid notes aligning to it
 */
enum AlignmentType {
    ALIGNMENT_MEASURE_START = 0,
    ALIGNMENT_BARLINE,
    ALIGNMENT_CLEF_ATTR,
    ALIGNMENT_KEYSIG_ATTR,
    ALIGNMENT_MENSUR_ATTR,
    ALIGNMENT_METERSIG_ATTR,
    ALIGNMENT_CLEF,
    ALIGNMENT_KEYSIG,
    ALIGNMENT_MENSUR,
    ALIGNMENT_METERSIG,
    ALIGNMENT_DOT,
    ALIGNMENT_GRACENOTE,
    ALIGNMENT_CONTAINER,
    ALIGNMENT_MULTIREST,
    ALIGNMENT_DEFAULT,
    ALIGNMENT_MEASURE_END
};

//----------------------------------------------------------------------------
// SystemAligner
//----------------------------------------------------------------------------

/**
 * This class aligns the content of a system
 * It contains a vector of StaffAlignment
 */
class SystemAligner: public Object
{
public:
    // constructors and destructors
    SystemAligner();
    virtual ~SystemAligner();
    
    int GetStaffAlignmentCount() const { return (int)m_children.size(); };
    
    /**
     * Reset the aligner (clear the content) and creates the end (bottom) alignement
     */
    virtual void Reset();
    
    /**
     * Get bottom StaffAlignment for the system.
     * For each SystemAligner, we keep a StaffAlignment for the bottom position.
     */
    StaffAlignment *GetBottomAlignment( ) { return m_bottomAlignment; };
    
    /**
     * Get the StaffAlignment at index idx.
     * Creates the StaffAlignment if not there yet.
     * Checks the they are created incrementally (without gap).
     */
    StaffAlignment* GetStaffAlignment( int idx );
    
private:
    
public:
    
private:
    /**
     * A pointer to the left StaffAlignment object kept for the system bottom position
     */
    StaffAlignment *m_bottomAlignment;
    
};

//----------------------------------------------------------------------------
// StaffAlignment
//----------------------------------------------------------------------------

/**
 * This class stores an alignement position staves will point to
 */
class StaffAlignment: public Object
{
public:
    // constructors and destructors
    StaffAlignment();
    virtual ~StaffAlignment();
    
    void SetYRel( int yRel ) { m_yRel = yRel; };
    int GetYRel() { return m_yRel; };
    
    void SetYShift( int yShift );
    int GetYShift() { return m_yShift; };
    
    void SetMaxHeight( int max_height );
    int GetMaxHeight() { return m_maxHeight; };
    
    /**
     * @name Set and get verse count.
     * When setting a value of 0, then 1 is assumed. This occurs
     * Typically with one single verse and no @n in <verse>
     */
    ///@{
    void SetVerseCount( int verse_count );
    int GetVerseCount() { return m_verseCount; };
    
    /**
     * Set the position of the StaffAlignment.
     * Functor redirected from System.
     */
    virtual int SetAligmentYPos( ArrayPtrVoid params );
    
    /**
     * Correct the Y alignment once the the content of a system has been aligned and laid out.
     * Special case of functor redirected from System.
     */
    virtual int IntegrateBoundingBoxYShift( ArrayPtrVoid params );
    
private:
    
public:
    
private:
    /**
     * Stores the position relative to the system.
     */
    int m_yRel;
    int m_yShift;
    /**
     * Stores temporally the maximum height of the of the staff pointing to it.
     * It is set and integrated as m_yShift.
     */
    int m_maxHeight;
    /**
     * Stores the number of verse of the staves attached to the aligner
     */
    int m_verseCount;
};


//----------------------------------------------------------------------------
// Alignment
//----------------------------------------------------------------------------

/** 
 * This class stores an alignement position elements will point to
 */
class Alignment: public Object
{
public:
    // constructors and destructors
    Alignment( );
    Alignment( double time, AlignmentType type = ALIGNMENT_DEFAULT );
    virtual ~Alignment();
    
    void SetXRel( int x_rel );
    int GetXRel() { return m_xRel; };
    
    void SetXShift( int xShift );
    int GetXShift() { return m_xShift; };
    
    void SetMaxWidth( int max_width );
    int GetMaxWidth() { return m_maxWidth; };

    /**
     * @name Set and get the time value of the alignment
     */
    ///@{
    void SetTime( double time ) { m_time = time; };
    double GetTime() { return m_time; };
    ///@}
    
    /**
     * @name Set and get the type of the alignment
     */
    ///@{
    void SetType( AlignmentType type ) { m_type = type; };
    AlignmentType GetType() { return m_type; };
    ///@}
    
    /**
     * Correct the X alignment once the the content of a system has been aligned and laid out.
     * Special case of functor redirected from Measure.
     */
    virtual int IntegrateBoundingBoxXShift( ArrayPtrVoid params );
    
    /**
     * Set the position of the Alignment.
     * Looks at the time different with the previous Alignment.
     */
    virtual int SetAligmentXPos( ArrayPtrVoid params );
    
    /**
     * Justify the X positions
     * Special case of functor redirected from Measure.
     */
    virtual int JustifyX( ArrayPtrVoid params );
    
private:
    
public:
    
private:
    /**
     * Stores the position relative to the measure.
     * This is instanciated the Object::SetAligmentXPos functor.
     * It takes into account a non-linear according to the time interval with
     * the previous Alignement
     */
    int m_xRel;
    /**
     * Stores temporally the maximum amount we need to shift the element pointing to it for 
     * avoiding collisions. This is set in Object::SetBoundingBoxXShift and then
     * integrated for all aligment in Aligment::IntegrateBoundingBoxXShift.
     */
    int m_xShift;
    /**
     * Stores temporally the maximum width of the of the element pointing to it.
     * It is set and integrated as m_xShift and it is used only for shifting the
     * alignment of the end of the measure (ALIGNMENT_MEASURE_END).
     */
    int m_maxWidth;
    /**
     * Stores the time at which the alignment occur.
     * It is set by Object::AlignHorizontally.
     */
    double m_time;
    /**
     * Defines the type of alignment (see the AlignmentType enum).
     * We have different types because we want events occuring at the same
     * time to be alignnemt separately. Example: the clef needs to be aligned
     * togeter, but key signature together and then the notes, even if all
     * of them occur at time 0.
     */
    AlignmentType m_type;
    
};

//----------------------------------------------------------------------------
// MeasureAligner
//----------------------------------------------------------------------------

/**
 * This class aligns the content of a measure
 * It contains a vector of Alignment
 */
class MeasureAligner: public Object
{
public:
    // constructors and destructors
    MeasureAligner();
    virtual ~MeasureAligner();
    
    int GetAlignmentCount() const { return (int)m_children.size(); };
    
    /**
     * Reset the aligner (clear the content) and creates the start (left) and end (right) alignement
     */
    virtual void Reset();
    
    Alignment* GetAlignmentAtTime( double time, AlignmentType type );
    
    /**
     * Keep the maximum time of the measure.
     * This correspond to the whole duration of the measure and 
     * should be the same for all staves/layers.
     */
    void SetMaxTime( double time );
    
    /**
     * @name Set and get the non justifiable margin
     */
    ///@{
    void SetNonJustifiableMargin( int margin ) { m_nonJustifiableLeftMargin = margin; };
    int GetNonJustifiableMargin() { return m_nonJustifiableLeftMargin; };
    ///@}
    
    /**
     * Get left Alignment for the measure.
     * For each MeasureAligner, we keep and Alignment for the left position.
     * The Alignment time will be always stay 0.0 and be the first in the list.
     */
    Alignment *GetLeftAlignment( ) { return m_leftAlignment; };
    
    /**
     * Get right Alignment for the measure.
     * For each MeasureAligner, we keep and Alignment for the right position.
     * The Alignment time will be increased whenever necessary when values are added.
     */
    Alignment *GetRightAlignment( ) { return m_rightAlignment; };
    
    /**
     * Correct the X alignment once the the content of a system has been aligned and laid out.
     * Special case of functor redirected from Measure.
     */
    virtual int IntegrateBoundingBoxXShift( ArrayPtrVoid params );
    
    /**
     * Set the position of the Alignment.
     * Looks at the time different with the previous Alignment.
     * For each MeasureAlignment, we need to reset the previous time position.
     */
    virtual int SetAligmentXPos( ArrayPtrVoid params );
    
    /**
     * Justify the X positions
     * Special case of functor redirected from Measure.
     */
    virtual int JustifyX( ArrayPtrVoid params );
    
private:
    void AddAlignment( Alignment *alignment, int idx = -1 );
    
public:
    
private:
    /**
     * A pointer to the left Alignment object kept for the measure start position
     */
    Alignment *m_leftAlignment;
    
    /**
     * A pointer to the left Alignment object kept for the measure end position
     */
    Alignment *m_rightAlignment;
    
    /**
     * Store the measure non justifiable margin used by the scoreDef attributes.
     */
    int m_nonJustifiableLeftMargin;
};

} // namespace vrv
    
#endif
