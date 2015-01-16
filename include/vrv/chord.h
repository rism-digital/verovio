/////////////////////////////////////////////////////////////////////////////
// Name:        chord.h
// Author:      Andrew Horwitz
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_CHORD_H__
#define __VRV_CHORD_H__

#include <typeinfo>

//----------------------------------------------------------------------------

#include "atts_shared.h"
#include "clef.h"
#include "drawinglistinterface.h"
#include "object.h"

namespace vrv {
    
    
//----------------------------------------------------------------------------
// Chord
//----------------------------------------------------------------------------

/**
 * This class represents a collection of notes in the same layer with the
 *    same onset time.
 * A chord is contained in a layer.
 * It contains notes.
 */
class Chord: public DocObject, public DrawingListInterface, public ObjectListInterface,
public AttCommon
{
public:
    
    Chord( );
    virtual ~Chord();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Chord"; };
};

} // namespace vrv

#endif