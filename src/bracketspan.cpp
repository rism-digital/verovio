/////////////////////////////////////////////////////////////////////////////
// Name:        bracketspan.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "bracketspan.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "functor.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BracketSpan
//----------------------------------------------------------------------------

static const ClassRegistrar<BracketSpan> s_factory("bracketSpan", BRACKETSPAN);

BracketSpan::BracketSpan()
    : ControlElement(BRACKETSPAN), TimeSpanningInterface(), AttBracketSpanLog(), AttLineRend(), AttLineRendBase()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_BRACKETSPANLOG);
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_LINERENDBASE);

    this->Reset();
}

BracketSpan::~BracketSpan() {}

void BracketSpan::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetBracketSpanLog();
    this->ResetLineRend();
    this->ResetLineRendBase();
}

int BracketSpan::GetLineWidth(const Doc *doc, int unit) const
{
    int lineWidth = doc->GetOptions()->m_octaveLineThickness.GetValue() * unit;
    if (this->HasLwidth()) {
        if (this->GetLwidth().GetType() == LINEWIDTHTYPE_lineWidthTerm) {
            switch (this->GetLwidth().GetLineWithTerm()) {
                case LINEWIDTHTERM_narrow: lineWidth *= LINEWIDTHTERM_factor_narrow; break;
                case LINEWIDTHTERM_medium: lineWidth *= LINEWIDTHTERM_factor_medium; break;
                case LINEWIDTHTERM_wide: lineWidth *= LINEWIDTHTERM_factor_wide; break;
                default: break;
            }
        }
        else if (this->GetLwidth().GetType() == LINEWIDTHTYPE_measurementunsigned) {
            if (this->GetLwidth().GetMeasurementunsigned().GetType() == MEASUREMENTTYPE_px) {
                lineWidth = this->GetLwidth().GetMeasurementunsigned().GetPx();
            }
            else {
                lineWidth = this->GetLwidth().GetMeasurementunsigned().GetVu() * unit;
            }
        }
    }
    return lineWidth;
}

//----------------------------------------------------------------------------
// BracketSpan functor methods
//----------------------------------------------------------------------------

FunctorCode BracketSpan::Accept(Functor &functor)
{
    return functor.VisitBracketSpan(this);
}

FunctorCode BracketSpan::Accept(ConstFunctor &functor) const
{
    return functor.VisitBracketSpan(this);
}

FunctorCode BracketSpan::AcceptEnd(Functor &functor)
{
    return functor.VisitBracketSpanEnd(this);
}

FunctorCode BracketSpan::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBracketSpanEnd(this);
}

} // namespace vrv
