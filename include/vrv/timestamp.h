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
    virtual void Reset();
    virtual std::string GetClassName() const { return "TimestampAttr"; }
    virtual ClassId GetClassId() const { return TIMESTAMP_ATTR; }
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
     * Returns the duration (in double) for the Timestamp.
     */
    double GetTimestampAttrAlignmentDuration(int meterUnit) const;

    //----------//
    // Functors //
    //----------//

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
