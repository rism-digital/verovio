/////////////////////////////////////////////////////////////////////////////
// Name:        staff.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "staff.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <vector>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "hairpin.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "syl.h"
#include "system.h"
#include "timeinterface.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

Staff::Staff(int n) : Object("staff-"), AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    Reset();
    SetN(n);
}

Staff::~Staff()
{
}

void Staff::Reset()
{
    Object::Reset();
    ResetCommon();

    m_drawingStaffSize = 100;
    m_drawingLines = 5;
    m_drawingNotationType = NOTATIONTYPE_NONE;
    m_yAbs = VRV_UNSET;
    m_staffAlignment = NULL;
    m_timeSpanningElements.clear();
    m_drawingStaffDef = NULL;
}

void Staff::AddChild(Object *child)
{
    if (child->Is(LAYER)) {
        Layer *layer = dynamic_cast<Layer *>(child);
        assert(layer);
        if (layer && (layer->GetN() < 1)) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one layer as a previous child.
            layer->SetN(this->GetChildCount());
        }
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}
    
int Staff::GetDrawingY() const
{
    if (!m_staffAlignment) return 0;
    
    System *system = dynamic_cast<System*>(this->GetFirstParent(SYSTEM));
    assert(system);

    return (system->GetDrawingY() + m_staffAlignment->GetYRel());
}
    
int Staff::CalcPitchPosYRel(Doc *doc, int loc)
{
    assert(doc);
    
    // the staff loc offset is based on the number of lines: 0 with 1 line, 2 with 2, etc
    int staffLocOffset = (this->m_drawingLines - 1) * 2;
    return (loc - staffLocOffset) * doc->GetDrawingUnit(this->m_drawingStaffSize);
}

//----------------------------------------------------------------------------
// Staff functor methods
//----------------------------------------------------------------------------

int Staff::UnsetCurrentScoreDef(FunctorParams *functorParams)
{
    m_drawingStaffDef = NULL;

    return FUNCTOR_CONTINUE;
};

int Staff::ResetVerticalAlignment(FunctorParams *functorParams)
{
    m_staffAlignment = NULL;

    return FUNCTOR_CONTINUE;
}

int Staff::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    params->m_staffN = this->GetN();

    // this gets (or creates) the measureAligner for the measure
    StaffAlignment *alignment = params->m_systemAligner->GetStaffAlignment(params->m_staffIdx, this, params->m_doc);

    assert(alignment);

    // Set the pointer of the m_alignment
    m_staffAlignment = alignment;

    std::vector<Object *>::iterator it;
    it = std::find_if(m_timeSpanningElements.begin(), m_timeSpanningElements.end(), ObjectComparison(VERSE));
    if (it != m_timeSpanningElements.end()) {
        Verse *v = dynamic_cast<Verse *>(*it);
        assert(v);
        alignment->SetVerseCount(v->GetN());
    }

    // for next staff
    params->m_staffIdx++;

    return FUNCTOR_CONTINUE;
}

int Staff::FillStaffCurrentTimeSpanning(FunctorParams *functorParams)
{
    FillStaffCurrentTimeSpanningParams *params = dynamic_cast<FillStaffCurrentTimeSpanningParams *>(functorParams);
    assert(params);

    std::vector<Object *>::iterator iter = params->m_timeSpanningElements.begin();
    while (iter != params->m_timeSpanningElements.end()) {
        TimeSpanningInterface *interface = (*iter)->GetTimeSpanningInterface();
        assert(interface);
        Measure *currentMeasure = dynamic_cast<Measure *>(this->GetFirstParent(MEASURE));
        assert(currentMeasure);
        // We need to make sure we are in the next measure (and not just a staff below because of some cross staff
        // notation
        if ((interface->GetStartMeasure() != currentMeasure) && (interface->IsOnStaff(this->GetN()))) {
            m_timeSpanningElements.push_back(*iter);
        }
        iter++;
    }
    return FUNCTOR_CONTINUE;
}

int Staff::ResetDrawing(FunctorParams *functorParams)
{
    this->m_timeSpanningElements.clear();
    return FUNCTOR_CONTINUE;
};


int Staff::PrepareRpt(FunctorParams *functorParams)
{
    PrepareRptParams *params = dynamic_cast<PrepareRptParams *>(functorParams);
    assert(params);

    // If multiNumber is set, we already know that nothing needs to be done
    // Futhermore, if @multi.number is false, the functor should have stopped (see below)
    if (params->m_multiNumber != BOOLEAN_NONE) {
        return FUNCTOR_CONTINUE;
    }

    // This is happening only for the first staff element of the staff @n
    if (StaffDef *staffDef = params->m_currentScoreDef->GetStaffDef(this->GetN())) {
        if ((staffDef->HasMultiNumber()) && (staffDef->GetMultiNumber() == BOOLEAN_false)) {
            // Set it just in case, but stopping the functor should do it for this staff @n
            params->m_multiNumber = BOOLEAN_false;
            return FUNCTOR_STOP;
        }
    }
    params->m_multiNumber = BOOLEAN_true;
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
