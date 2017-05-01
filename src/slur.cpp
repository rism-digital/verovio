/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "slur.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "layer.h"
#include "layerelement.h"
#include "staff.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

Slur::Slur() : ControlElement("slur-"), TimeSpanningInterface(), AttColor(), AttCurvature()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);

    Reset();
}

Slur::~Slur()
{
}

void Slur::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetCurvature();
}
    
void Slur::GetCrossStaffOverflows(StaffAlignment *alignment, curvature_CURVEDIR cuvreDir, bool &skipAbove, bool &skipBelow)
{
    assert(alignment);
    
    if (!this->GetStart() || !this->GetEnd() || !alignment->GetStaff()) return;
    
    Layer *layer = NULL;
    
    // If the starting point is a chord we need to select the appropriate extreme staff
    Staff *startStaff = NULL;
    if (this->GetStart()->Is(CHORD)) {
        Chord *chord = dynamic_cast<Chord*>(this->GetStart());
        assert(chord);
        Staff *staffAbove = NULL;
        Staff *staffBelow = NULL;
        chord->GetCrossStaffExtremes(staffAbove, staffBelow);
        startStaff = (cuvreDir == curvature_CURVEDIR_above) ? staffAbove : staffBelow;
    }
    else
        startStaff = this->GetStart()->GetCrossStaff(layer);
    
    // Same for the end point
    Staff *endStaff = NULL;
    if (this->GetEnd()->Is(CHORD)) {
        Chord *chord = dynamic_cast<Chord*>(this->GetEnd());
        assert(chord);
        Staff *staffAbove = NULL;
        Staff *staffBelow = NULL;
        chord->GetCrossStaffExtremes(staffAbove, staffBelow);
        endStaff = (cuvreDir == curvature_CURVEDIR_above) ? staffAbove : staffBelow;
    }
    else
        endStaff = this->GetEnd()->GetCrossStaff(layer);
    
    // No cross-staff endpoints, nothing to skip
    if (!startStaff && !endStaff) return;

    if (cuvreDir == curvature_CURVEDIR_above) {
        if (startStaff && (startStaff->GetN() < alignment->GetStaff()->GetN()))
            skipAbove = true;
        if (endStaff && (endStaff->GetN() < alignment->GetStaff()->GetN()))
            skipAbove = true;
    }
    else {
        if (startStaff && (startStaff->GetN() > alignment->GetStaff()->GetN()))
            skipBelow = true;
        if (endStaff && (endStaff->GetN() > alignment->GetStaff()->GetN()))
            skipBelow = true;
    }
}

} // namespace vrv
