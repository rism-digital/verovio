/////////////////////////////////////////////////////////////////////////////
// Name:        verse.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_VERSE_H__
#define __VRV_VERSE_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {
    
class Syl;

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

class Verse: public DocObject, public AttCommon
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Verse();
    virtual ~Verse();
    void Reset();
    virtual std::string GetClassName( ) { return "Verse"; };
    ///@}
    
    /**
     * Add syl to a verse.
     */
    void AddSyl(Syl *syl);
    
    /**
     * Align the content of a staff vertically.
     * See Object::AlignVertically
     */
    virtual int AlignVertically( ArrayPtrVoid params );
    
protected:

private:
    
public:
    
private:

};

} // namespace vrv    
    
#endif
