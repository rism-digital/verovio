/////////////////////////////////////////////////////////////////////////////
// Name:        num.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NUM_H__
#define __VRV_NUM_H__

#include "atts_shared.h"
#include "text.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Num (number)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <num> element.
 */
class Num : public TextElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Num();
    virtual ~Num();
    Object *Clone() const override { return new Num(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "num"; }
    ///@}

    /**
     * Add an element (text) to a num.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Return a pointer to the current text object.
     */
    ///@{
    Text *GetCurrentText() { return &m_currentText; }
    const Text *GetCurrentText() const { return &m_currentText; }
    ///@}

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
    /**
     * Current text for templated number (<num label="page">#</num>).
     * Filled by RunningElement::SetCurrentPageNum and used by View::DrawNum
     */
    Text m_currentText;
};

} // namespace vrv

#endif
