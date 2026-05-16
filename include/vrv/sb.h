/////////////////////////////////////////////////////////////////////////////
// Name:        sb.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SB_H__
#define __VRV_SB_H__

#include "atts_shared.h"
#include "facsimileinterface.h"
#include "systemelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Sb
//----------------------------------------------------------------------------

/**
 * This class represents a MEI sb in score-based MEI.
 * In page-based MEI, it remains as it is. Actual systems are represented by System objects.
 */
class Sb : public SystemElement, public FacsimileInterface, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Sb();
    virtual ~Sb();
    Object *Clone() const override { return new Sb(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "sb"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    FacsimileInterface *GetFacsimileInterface() override { return vrv_cast<FacsimileInterface *>(this); }
    const FacsimileInterface *GetFacsimileInterface() const override
    {
        return vrv_cast<const FacsimileInterface *>(this);
    }

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

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
