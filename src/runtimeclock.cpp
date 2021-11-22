/////////////////////////////////////////////////////////////////////////////
// Name:        runtimeclock.cpp
// Author:      David Bauer
// Created:     06/08/2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "runtimeclock.h"

#ifndef NO_RUNTIME

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// RuntimeClock
//----------------------------------------------------------------------------

RuntimeClock::RuntimeClock()
{
    this->Reset();
}

void RuntimeClock::Reset()
{
    m_start = std::chrono::steady_clock::now();
}

double RuntimeClock::GetSeconds() const
{
    using namespace std::chrono;
    steady_clock::duration timeDiff = steady_clock::now() - m_start;
    return duration<double, seconds::period>(timeDiff).count();
}

} // namespace vrv

#endif // NO_RUNTIME
