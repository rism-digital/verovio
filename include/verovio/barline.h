/////////////////////////////////////////////////////////////////////////////
// Name:        musbarline.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __MUS_BARLINE_H__
#define __MUS_BARLINE_H__

#include "vrvdef.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Barline
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <barline> element. 
 */
class Barline: public LayerElement
{
public:
    // constructors and destructors
    Barline();
    virtual ~Barline();
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Barline and compare attributes
     */
    virtual bool operator==(Object& other);
    
    virtual std::string MusClassName( ) { return "Barline"; };
    
    /**
     * Use to set the alignment for the Measure Barline members.
     * This is as special case where we need to add to the measure aligner.
     */
    void SetAlignment( Alignment *alignment ) { m_alignment = alignment; };
    
    /*
     * Return true if the barline type requires repetition dots to be drawn.
     */
    bool HasRepetitionDots( );
    
    /**
     * Return the default horizontal spacing of notes.
     */
    virtual int GetHorizontalSpacing( ) { return 0; }
    
private:
    
public:
    /** Indicates the barline type (@rend) */
    BarlineType m_barlineType;
    /** Indicates a partial barline (inbetween the staves) - no MEI equivalent */
    bool m_partialBarline;
    /** Indicates a barline displayed only on the staff - no MEI equivalent */
    bool m_onStaffOnly;

private:
    
};
    
} // namespace vrv

#endif
