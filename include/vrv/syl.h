/////////////////////////////////////////////////////////////////////////////
// Name:        syl.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SYL_H__
#define __VRV_SYL_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {
    
class Note;

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

class Syl: public LayerElement,
    public AttTypography,
    public AttSylLog
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Syl();
    virtual ~Syl();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Syl"; };
    ///@}
    
    /**
     * Add syl to a syl.
     */
    void AddSyl(Syl *syl);
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Functor for setting wordpos and connector ends
     * The functor is process by staff/layer/verse using an ArrayOfAttComparisons filter
     * See PrepareDarwing
     */
    virtual int PrepareLyrics( ArrayPtrVoid params );
    
    /**
     * Functor for setting running lyrics in staves
     * This is necessary for <syl> that starts in one measure and ends in another one
     * The functor is process by staff/layer/verse using an ArrayOfAttComparisons filter.
     */
    virtual int FillStaffCurrentLyrics( ArrayPtrVoid params );
    
private:
    
public:
    /** 
     * The first and last note of the syl to be used when we have @wordpos and @con 
     * The first note is usually the parent.
     */
    Note *m_drawingFirstNote, *m_drawingLastNote;
    /**
     * The verse number with multiple verses
     * Value is 1 by default, set in PrepareLyrics
     */
    int m_drawingVerse;
    
private:

};

} // namespace vrv    
    
#endif
