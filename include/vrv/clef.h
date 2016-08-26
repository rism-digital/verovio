/////////////////////////////////////////////////////////////////////////////
// Name:        clef.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CLEF_H__
#define __VRV_CLEF_H__

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
class Clef : public LayerElement, public AttClefshape, public AttLineloc, public AttOctavedisplacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Clef();
    Clef(const ScoreDefInterface *clefAttr);
    void Init();
    virtual ~Clef();
    virtual void Reset();
    virtual Object *Clone() const { return new Clef(*this); }
    virtual std::string GetClassName() const { return "Clef"; }
    virtual ClassId Is() const { return CLEF; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Return the offset of the clef
     */
    int GetClefOffset() const;

    /**
     * This is used for false clef offset calculation.
     * Returns 4 bytes with, from right to left, line, shape, dis and displace
     */
    int GetClefId() const;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
