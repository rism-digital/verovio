/////////////////////////////////////////////////////////////////////////////
// Name:        subst.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SUBST_H__
#define __VRV_SUBST_H__

#include "atts_critapp.h"
#include "atts_shared.h"
#include "boundary.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Subst
//----------------------------------------------------------------------------

class Subst : public EditorialElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Subst();
    Subst(EditorialLevel level);
    virtual ~Subst();
    virtual Object *Clone() const { return new Subst(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Subst"; }
    virtual ClassId GetClassId() const { return SUBST; }
    ///@}

    /** Getter for level **/
    EditorialLevel GetLevel() { return m_level; }

    /**
     * Add children to a apparatus.
     */
    virtual bool IsSupportedChild(Object *object);

protected:
    /** We store the level of the <subst> for integrity check */
    EditorialLevel m_level;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
