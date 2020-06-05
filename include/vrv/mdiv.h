/////////////////////////////////////////////////////////////////////////////
// Name:        mdiv.h
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MDIV_H__
#define __VRV_MDIV_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mdiv
//----------------------------------------------------------------------------

/**
 * This class represent a <pages> in page-based MEI.
 */
class Mdiv : public Object, public AttLabelled, public AttNNumberLike {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Mdiv();
    virtual ~Mdiv();
    virtual Object *Clone() const { return new Mdiv(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Mdiv"; }
    virtual ClassId GetClassId() const { return MDIV; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
    ///@}

    /**
     * Make the mdiv and its parent (recursively) visible
     */
    void MakeVisible();

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    /**
     * Holds the visibility (hidden or visible) for an mdiv element.
     * By default, a mdiv elements is hidden, and one <mdiv> branchn has to be made visible.
     * See Mdiv::MakeVisible();
     */
    VisibilityType m_visibility;

private:
    //
};

} // namespace vrv

#endif
