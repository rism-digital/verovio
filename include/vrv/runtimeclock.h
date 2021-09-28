/////////////////////////////////////////////////////////////////////////////
// Name:        runtimeclock.h
// Author:      David Bauer
// Created:     06/08/2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RUNTIMECLOCK_H__
#define __VRV_RUNTIMECLOCK_H__

#ifndef NO_RUNTIME

#include <chrono>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// RuntimeClock
//----------------------------------------------------------------------------

/**
 * This class represents a clock to measure runtime.
 */
class RuntimeClock {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    RuntimeClock();
    ///@}

    /** Resets the clock */
    void Reset();

    /** Get current runtime in seconds */
    double GetSeconds() const;

private:
    //
public:
    //
private:
    /** The time point at which the clock was started */
    std::chrono::time_point<std::chrono::steady_clock> m_start;

}; // class RuntimeClock

} // namespace vrv

#endif // NO_RUNTIME
#endif // __VRV_RUNTIMECLOCK_H__
