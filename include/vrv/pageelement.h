/////////////////////////////////////////////////////////////////////////////
// Name:        pageelement.h
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PAGE_ELEMENT_H__
#define __VRV_PAGE_ELEMENT_H__

#include "atts_shared.h"
#include "devicecontextbase.h"
#include "floatingobject.h"

namespace vrv {

//----------------------------------------------------------------------------
// PageElement
//----------------------------------------------------------------------------

/**
 * This class represents elements appearing within a page.
 * It is not an abstract class but should not be instanciated directly.
 */
class PageElement : public Object, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    PageElement();
    PageElement(ClassId classId);
    virtual ~PageElement();
    void Reset() override;
    ///@}

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
