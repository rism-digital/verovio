/////////////////////////////////////////////////////////////////////////////
// Name:        lb.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LB_H__
#define __VRV_LB_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lb
//----------------------------------------------------------------------------

/**
 * This class models the MEI <lb>
 */
class Lb : public TextElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Lb();
    virtual ~Lb();
    Object *Clone() const override { return new Lb(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Lb"; }
    ///@}

    /**
     * Lb is an empty element
     */
    void AddChild(Object *object) override{};

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
