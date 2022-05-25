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
    PageElement(ClassId classId, const std::string &classIdStr);
    virtual ~PageElement();
    void Reset() override;
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CastOffSystems
     */
    int CastOffSystems(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffPages
     */
    int CastOffPages(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffEncoding
     */
    int CastOffEncoding(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffToSelection
     */
    int CastOffToSelection(FunctorParams *) override;

    /**
     * See Object::UnCastOff
     */
    int UnCastOff(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
