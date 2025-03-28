/////////////////////////////////////////////////////////////////////////////
// Name:        dot.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DOT_H__
#define __VRV_DOT_H__

#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

class Note;

//----------------------------------------------------------------------------
// Dot
//----------------------------------------------------------------------------

class Dot : public LayerElement, public PositionInterface, public AttColor, public AttDotLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Dot();
    virtual ~Dot();
    Object *Clone() const override { return new Dot(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "dot"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PositionInterface *GetPositionInterface() override { return vrv_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override { return vrv_cast<const PositionInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

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
    /**
     * A pointer to the previous LayerElement it relates to.
     */
    LayerElement *m_drawingPreviousElement;

    /**
     * A pointer to the next LayerElement (note, barLine, etc.)
     */
    LayerElement *m_drawingNextElement;

private:
};

} // namespace vrv

#endif
