/////////////////////////////////////////////////////////////////////////////
// Name:        timestamp.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIMESTAMP_H__
#define __VRV_TIMESTAMP_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// TimestampAttr
//----------------------------------------------------------------------------

class TimestampAttr : public LayerElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    TimestampAttr();
    virtual ~TimestampAttr();
    void Reset() override;
    std::string GetClassName() const override { return "timestampAttr"; }
    ///@}

    /**
     * @name Setter and getter for the timestamp actual duration position.
     * The actual duration position is the timestamp position - 1.0.
     */
    ///@{
    double GetActualDurPos() const { return m_actualDurPos; }
    void SetDrawingPos(double pos) { m_actualDurPos = pos; }
    ///@}

    /**
     * Returns the duration (in Fraction) for the Timestamp.
     */
    Fraction GetTimestampAttrAlignmentDuration(data_DURATION meterUnit) const;

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
    /** The actual duration postion where 0.0 correspond to the first beat and -1.0 the beginning of the measure */
    double m_actualDurPos;
};

} // namespace vrv

#endif
