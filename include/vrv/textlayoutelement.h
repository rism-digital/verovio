/////////////////////////////////////////////////////////////////////////////
// Name:        textlayoutelement.h
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEXT_LAYOUT_ELEMENT_H__
#define __VRV_TEXT_LAYOUT_ELEMENT_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextLayoutElement
//----------------------------------------------------------------------------

/**
 * This class represents a text element featuring horizontal and vertical layout.
 * It is not an abstract class but should not be instanciated directly.
 */
class TextLayoutElement : public Object, public ObjectListInterface, public AttTyped {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    TextLayoutElement(ClassId classId, const std::string &classIdStr);
    virtual ~TextLayoutElement();
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
