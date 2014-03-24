/////////////////////////////////////////////////////////////////////////////
// Name:        staff.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_STAFF_H__
#define __VRV_STAFF_H__

#include "object.h"

namespace vrv {

#define STAFF_OFFSET 190
#define MAXCLE 100	// maximum number of clef on one staff

class Clef;
class DeviceContext;
class Layer;
class StaffAlignment;

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

/**
 * This class represents a staff in a laid-out score (Doc).
 * A Staff is contained in a System.
 * It contains Measure objects.
 * For unmeasured music, on single Measure is added for simplifying internal processing
*/
class Staff: public DocObject
{
    
public:
    // constructors and destructors
    Staff( int n = -1 );
	Staff( const Staff& staff ); // copy contructor
    virtual ~Staff();

    virtual std::string GetClassName( ) { return "Staff"; };	
    
    void Clear();
    
    void AddLayer( Layer *layer );
	
	int GetLayerCount() const { return (int)m_children.size(); };
    
    /**
     * Return the index position of the staff in its measure parent
     */
    int GetStaffIdx() const { return Object::GetIdx(); };
    
    /**
     * @name Set and get  the staff number which is 1-based.
     * This can be different from the index position in the parent measure.
     */
    ///@{
    int GetStaffNo() const { return m_n; };
    void SetStaffNo( int n ) { m_n = n; };
    ///@}
    
    /**
     * Return the default horizontal spacing of staves.
     */
    int GetVerticalSpacing();
    
    /**
     * Reset the alignment values (m_drawingY, m_drawingYRel, etc.)
     * Called by  AlignVertically
     */
    virtual void ResetVerticalAlignment( );
    
    StaffAlignment *GetAlignment() { return m_staffAlignment; };
    
    int GetYRel();

    // functors
    virtual int Save( ArrayPtrVoid params );
    virtual bool GetPosOnPage( ArrayPtrVoid params );
    
	void CopyAttributes( Staff *staff ); // copy all attributes but none of the elements

	Layer *GetFirst( );
	Layer *GetLast( );
	Layer *GetNext( Layer *layer );
	Layer *GetPrevious( Layer *layer );
    Layer *GetLayerWithIdx( int LayerIdx );
    
    /**
     * Align the content of a staff vertically.
     */
    virtual int AlignVertically( ArrayPtrVoid params );
    
public:
	/**
     * Number of lines copied from the staffDef for fast access when drawing
     */
	int m_drawingLines;

	/** 
     * The Y absolute position of the staff for facsimile (transcription) encodings.
     * This is the top left corner of the staff (the X position is the position of the system).
     */
    int m_yAbs;
    
	/** portee en notation ancienne (wolfgang legacy) */
	char notAnc;
	/** portee grise (wolfgang legacy) */
	char grise;
	/** portee invisible (wolfgang legacy) */
	char invisible;
	/** taille. 0 = normale1 = petite (wolfgang legacy) */
	unsigned char staffSize;

private:
    /** The staff number */
    int m_n;
    
    /**
     * A pointer to a StaffAlignment for aligning the staves
     */
    StaffAlignment *m_staffAlignment;

    
};

} // namespace vrv

#endif
