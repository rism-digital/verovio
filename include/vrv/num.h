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
    virtual Object *Clone() const { return new Num(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Num"; }
    virtual ClassId GetClassId() const { return NUM; }
    ///@}

    /**
     * Add an element (text) to a num.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Return a pointer to the current text object.
     */
    Text *GetCurrentText() { return &m_currentText; }

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
