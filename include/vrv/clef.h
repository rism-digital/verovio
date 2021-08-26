/////////////////////////////////////////////////////////////////////////////
// Name:        clef.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CLEF_H__
#define __VRV_CLEF_H__

#include "atts_externalsymbols.h"
#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

class ScoreDefInterface;

//----------------------------------------------------------------------------
// Clef
//----------------------------------------------------------------------------

/**
 * This class models the MEI <clef> element.
 */
class Clef : public LayerElement,
             public AttClefShape,
             public AttColor,
             public AttExtSym,
             public AttLineLoc,
             public AttOctaveDisplacement,
             public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Clef();
    virtual ~Clef();
    virtual Object *Clone() const { return new Clef(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Clef"; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /** Override the method since check is required */
    virtual bool IsScoreDefElement() const { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

    /**
     * Return the offset of the clef
     */
    int GetClefLocOffset() const;

    //----------------//
    // Static methods //
    //----------------//

    /**
     * Retrieves the appropriate SMuFL code for a data_CLEFSHAPE
     */
    wchar_t GetClefGlyph(data_NOTATIONTYPE) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustBeams
     */
    virtual int AdjustBeams(FunctorParams *functorParams);

    /**
     * See Object::AdjustClefChanges
     */
    virtual int AdjustClefChanges(FunctorParams *functorParams);

private:
public:
    //
private:
};

} // namespace vrv

#endif
