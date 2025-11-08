/////////////////////////////////////////////////////////////////////////////
// Name:        ostaff.h
// Author:      Klaus Rettinghaus
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OSTAFF_H__
#define __VRV_OSTAFF_H__

#include "atts_shared.h"
#include "object.h"
#include "staff.h"

namespace vrv {

class DeviceContext;
class Layer;
class LedgerLine;
class StaffAlignment;
class StaffDef;
class Syl;
class TimeSpanningInterface;
class Tuning;

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

/**
 * This class represents a oStaff in a laid-out score (Doc).
 * A OStaff is contained in a System.
 * It contains Measure objects.
 * For unmeasured music, one single Measure is added for simplifying internal processing
 */
class OStaff : public Staff {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    OStaff(int n = 1);
    virtual ~OStaff();
    Object *Clone() const override { return new OStaff(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "ostaff"; }
    ///@}

    bool IsOssia() const override { return true; };

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{

    ///@}

private:
public:
private:
    /**
     * The legder lines (above / below and normal / cue)
     */
    ///@{
    ArrayOfLedgerLines m_ledgerLinesAbove;
    ArrayOfLedgerLines m_ledgerLinesBelow;
    ArrayOfLedgerLines m_ledgerLinesAboveCue;
    ArrayOfLedgerLines m_ledgerLinesBelowCue;
    ///@}
};

} // namespace vrv

#endif
