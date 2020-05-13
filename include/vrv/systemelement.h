/////////////////////////////////////////////////////////////////////////////
// Name:        systemelement.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYSTEM_ELEMENT_H__
#define __VRV_SYSTEM_ELEMENT_H__

#include "atts_shared.h"
#include "devicecontextbase.h"
#include "floatingobject.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemElement
//----------------------------------------------------------------------------

/**
 * This class represents elements appearing within a measure.
 * It is not an abstract class but should not be instanciated directly.
 */
class SystemElement : public FloatingObject, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    SystemElement();
    SystemElement(const std::string &classid);
    virtual ~SystemElement();
    virtual void Reset();
    virtual ClassId GetClassId() const { return SYSTEM_ELEMENT; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    virtual int ConvertToPageBased(FunctorParams *functorParams);

    /**
     * See Object::ConvertToCastOffMensural
     */
    virtual int ConvertToCastOffMensural(FunctorParams *params);

    /**
     * See Object::CastOffSystems
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

private:
    //
public:
    /**
     * Holds the visibility (hidden or visible) for an system element.
     * By default, a system element is visible. It can be hidden when expansion are processed.
     */
    VisibilityType m_visibility;

private:
    //
};

} // namespace vrv

#endif
