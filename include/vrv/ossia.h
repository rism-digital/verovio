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
     * Retrieve the original staff corresponding to the ossia staff
     */
    const Staff *GetOriginalStaffForOssia(const Staff *ossia) const;

    /**
     * Methods for adding allowed content
     */
    bool IsSupportedChild(ClassId classId) override;

    void GetStavesAbove(MapOfOssiaStaffNs &map) const;
    void GetStavesBelow(MapOfOssiaStaffNs &map) const;

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

public:
    //
protected:
    //
private:
    //
    void GetStaves(MapOfOssiaStaffNs &map, ListOfConstObjects &staves) const;
};

} // namespace vrv

#endif
