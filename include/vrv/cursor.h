/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.h
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CURSOR_H__
#define __VRV_CURSOR_H__

#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Cursor
//----------------------------------------------------------------------------

/**
 * This class implements a curor for the editor
 */
class Cursor : public Note {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Cursor();
    virtual ~Cursor();
    Object *Clone() const override { return new Cursor(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "cursor"; }
    ///@}

    bool IsCursor() const override { return true; }
    
    void SetPosition(LayerElement *position) { m_position = position; }
    LayerElement *GetPosition() const { return m_position; }
    bool HasPosition() const { return (m_position); }
    
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
    /** The reference postion */
    LayerElement *m_position;
};

} // namespace vrv

#endif
