/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.cpp
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "cursor.h"
//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "functor.h"
#include "horizontalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Cursor
//----------------------------------------------------------------------------

Cursor::Cursor() : Note()
{
    this->Reset();
}

Cursor::~Cursor() {}

void Cursor::Reset()
{
    Note::Reset();

    m_position = NULL;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Cursor::Accept(Functor &functor)
{
    return functor.VisitCursor(this);
}

FunctorCode Cursor::Accept(ConstFunctor &functor) const
{
    return functor.VisitCursor(this);
}

FunctorCode Cursor::AcceptEnd(Functor &functor)
{
    return functor.VisitCursorEnd(this);
}

FunctorCode Cursor::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitCursorEnd(this);
}

} // namespace vrv
