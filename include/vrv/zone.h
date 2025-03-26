/////////////////////////////////////////////////////////////////////////////
// Name:        zone.h
// Author:      Juliette Regimbal
// Created:     05/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ZONE_H__
#define __VRV_ZONE_H__

#include <cassert>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_shared.h"
#include "object.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Zone
//----------------------------------------------------------------------------
/**
 * Implements the zone element
 * in MEI
 */

class Zone : public Object, public AttTyped, public AttCoordinated, public AttCoordinatedUl {
public:
    /**
     * @name Constructors, destructors, reset, and class name methods
     */
    ///@{
    Zone();
    virtual ~Zone();
    Object *Clone() const override { return new Zone(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "zone"; }
    ///@}
    void ShiftByXY(int xDiff, int yDiff);
    int GetLogicalUly() const;
    int GetLogicalLry() const;

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
};

} // namespace vrv

#endif
