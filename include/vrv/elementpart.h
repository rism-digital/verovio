/////////////////////////////////////////////////////////////////////////////
// Name:        elementpart.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ELEMENT_PART_H__
#define __VRV_ELEMENT_PART_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Flag
//----------------------------------------------------------------------------

/**
 * This class models a stem as a layer element part and has not direct MEI equivlatent.
 */
class Flag : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Flag();
    virtual ~Flag();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Flag"; }
    virtual ClassId GetClassId() const { return FLAG; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// NoteHead
//----------------------------------------------------------------------------

/**
 * This class models a stem as a layer element part and has not direct MEI equivlatent.
 */
class NoteHead : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    NoteHead();
    virtual ~NoteHead();
    virtual void Reset();
    virtual std::string GetClassName() const { return "NoteHead"; }
    virtual ClassId GetClassId() const { return NOTEHEAD; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

/**
 * This class models a stem as a layer element part and has not direct MEI equivlatent.
 */
class Stem : public LayerElement, public AttStems, public AttStemsCmn {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Stem();
    virtual ~Stem();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Stem"; }
    virtual ClassId GetClassId() const { return STEM; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
