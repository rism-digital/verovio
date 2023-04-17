/////////////////////////////////////////////////////////////////////////////
// Name:        labelabbr.h
// Author:      Klaus Rettinghaus
// Created:     07/03/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LABELABBR_H__
#define __VRV_LABELABBR_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// LabelAbbr
//----------------------------------------------------------------------------

/**
 * This class models the MEI <labelAbbr> element.
 */
class LabelAbbr : public Object, public TextListInterface {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    LabelAbbr();
    virtual ~LabelAbbr();
    Object *Clone() const override { return new LabelAbbr(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "LabelAbbr"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
    ///@}

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
    //
};

} // namespace vrv

#endif
