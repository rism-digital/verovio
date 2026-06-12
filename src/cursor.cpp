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
    m_accid.Reset();

    m_accid.SetParent(this);
    m_accid.SetDrawingCueSize(true);
    m_accid.SetFunc(accidLog_FUNC_edit);

    m_position = NULL;
    m_isAccidImplicit = false;
}

void Cursor::SetAccidImplicit(bool isAccidImplicit)
{
    m_isAccidImplicit = isAccidImplicit;
    data_ENCLOSURE enclosure = (m_isAccidImplicit) ? ENCLOSURE_paren : ENCLOSURE_NONE;
    m_accid.SetEnclose(enclosure);
}

void Cursor::SetAccid(data_ACCIDENTAL_WRITTEN accid)
{
    m_accid.SetAccid(accid);
    this->SetAccidImplicit(false);
}

void Cursor::SetCursorAlignment(Alignment *alignment)
{
    this->SetAlignment(alignment);
    m_accid.SetAlignment(alignment);
}

void Cursor::ResetCursorAlignment()
{
    m_accid.SetAlignment(NULL);
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
