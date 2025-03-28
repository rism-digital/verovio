/////////////////////////////////////////////////////////////////////////////
// Name:        expansion.h
// Author:      Klaus Rettinghaus
// Created:     22/02/2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EXPANSION_H__
#define __VRV_EXPANSION_H__

#include "atts_shared.h"
#include "plistinterface.h"
#include "systemelement.h"

namespace vrv {

class Expansion;

//----------------------------------------------------------------------------
// Expansion
//----------------------------------------------------------------------------

/**
 * This class represents a MEI expansion.
 */
class Expansion : public SystemElement, public PlistInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Expansion();
    virtual ~Expansion();
    Object *Clone() const override { return new Expansion(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "expansion"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PlistInterface *GetPlistInterface() override { return vrv_cast<PlistInterface *>(this); }
    const PlistInterface *GetPlistInterface() const override { return vrv_cast<const PlistInterface *>(this); }
    ////@}

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
