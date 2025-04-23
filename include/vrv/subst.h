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
#include "editorial.h"
#include "systemmilestone.h"

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
    Object *Clone() const override { return new Subst(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "subst"; }
    ///@}

    /** Getter for level **/
    EditorialLevel GetLevel() const { return m_level; }

    /**
     * Add children to a apparatus.
     */
    bool IsSupportedChild(ClassId classId) override;

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
