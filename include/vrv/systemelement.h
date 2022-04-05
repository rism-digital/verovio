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
    SystemElement(ClassId classId);
    SystemElement(ClassId classId, const std::string &classIdStr);
    virtual ~SystemElement();
    void Reset() override;
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    int ConvertToPageBased(FunctorParams *functorParams) override;

    /**
     * See Object::ConvertToCastOffMensural
     */
    int ConvertToCastOffMensural(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffSystems
     */
    int CastOffSystems(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffEncoding
     */
    int CastOffEncoding(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffToSelection
     */
    int CastOffToSelection(FunctorParams *) override;

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
