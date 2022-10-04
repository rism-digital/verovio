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
             public AttEnclosingChars,
             public AttExtSym,
             public AttLineLoc,
             public AttOctaveDisplacement,
             public AttStaffIdent,
             public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Clef();
    virtual ~Clef();
    Object *Clone() const override { return new Clef(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Clef"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /** Override the method since check is required */
    bool IsScoreDefElement() const override { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

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
    char32_t GetClefGlyph(const data_NOTATIONTYPE notationType) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustBeams
     */
    int AdjustBeams(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustClefChanges
     */
    int AdjustClefChanges(FunctorParams *functorParams) override;

private:
public:
    //
private:
};

} // namespace vrv

#endif
