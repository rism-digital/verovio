/////////////////////////////////////////////////////////////////////////////
// Name:        rend.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REND_H__
#define __VRV_REND_H__

#include "areaposinterface.h"
#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rend
//----------------------------------------------------------------------------

/**
 * This class models the MEI <rend> element.
 */
class Rend : public TextElement,
             public AreaPosInterface,
             public AttColor,
             public AttLang,
             public AttTextRendition,
             public AttTypography,
             public AttWhitespace {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Rend();
    virtual ~Rend();
    Object *Clone() const override { return new Rend(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Rend"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    AreaPosInterface *GetAreaPosInterface() override { return dynamic_cast<AreaPosInterface *>(this); }
    const AreaPosInterface *GetAreaPosInterface() const override
    {
        return dynamic_cast<const AreaPosInterface *>(this);
    }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a rend.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(Object *object) override;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(MutableFunctor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(MutableFunctor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
