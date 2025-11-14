/////////////////////////////////////////////////////////////////////////////
// Name:        ossia.h
// Author:      Klaus Rettinghaus
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OSSIA_H__
#define __VRV_OSSIA_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ossia
//----------------------------------------------------------------------------

/**
 * This class represents an ossia in a page-based score (Doc).
 */
class Ossia : public Object, public AttTyped {

private:
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Ossia();
    virtual ~Ossia();
    Object *Clone() const override { return new Ossia(*this); };
    void Reset() override;
    std::string GetClassName() const override { return "ossia"; }
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    void CloneReset() override;

    /**
     * Methods for adding allowed content
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Specific method for ossias
     */
    void AddChildBack(Object *object);

public:
protected:
private:
};

} // namespace vrv

#endif
