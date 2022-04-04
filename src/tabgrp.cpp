/////////////////////////////////////////////////////////////////////////////
// Name:        tabgrp.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabgrp.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functorparams.h"
#include "note.h"
#include "tabdursym.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabGrp
//----------------------------------------------------------------------------

static const ClassRegistrar<TabGrp> s_factory("tabGrp", TABGRP);

TabGrp::TabGrp() : LayerElement(TABGRP, "tabgrp-"), ObjectListInterface(), DurationInterface()
{
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());

    this->Reset();
}

TabGrp::~TabGrp() {}

void TabGrp::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

bool TabGrp::IsSupportedChild(Object *child)
{
    if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is(TABDURSYM)) {
        assert(dynamic_cast<TabDurSym *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void TabGrp::FilterList(ArrayOfConstObjects &childList) const
{
    // Retain only note children of chords
    ArrayOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        iter = ((*iter)->Is(NOTE)) ? iter + 1 : childList.erase(iter);
    }

    std::sort(childList.begin(), childList.end(), TabCourseSort());
}

int TabGrp::GetYTop()
{
    // The last note is the top
    return this->GetListBack(this)->GetDrawingY();
}

int TabGrp::GetYBottom()
{
    // The first note is the bottom
    return this->GetListFront(this)->GetDrawingY();
}

Note *TabGrp::GetTopNote()
{
    Note *topNote = vrv_cast<Note *>(this->GetListBack(this));
    assert(topNote);
    return topNote;
}

Note *TabGrp::GetBottomNote()
{
    // The first note is the bottom
    Note *bottomNote = vrv_cast<Note *>(this->GetListFront(this));
    assert(bottomNote);
    return bottomNote;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int TabGrp::InitOnsetOffsetEnd(FunctorParams *functorParams)
{
    InitOnsetOffsetParams *params = vrv_params_cast<InitOnsetOffsetParams *>(functorParams);
    assert(params);

    LayerElement *element = this->ThisOrSameasAsLink();

    double incrementScoreTime = element->GetAlignmentDuration(
        params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
    incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
    double realTimeIncrementSeconds = incrementScoreTime * 60.0 / params->m_currentTempo;

    params->m_currentScoreTime += incrementScoreTime;
    params->m_currentRealTimeSeconds += realTimeIncrementSeconds;

    return FUNCTOR_CONTINUE;
}

int TabGrp::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    params->m_dur = this->GetActualDur();
    params->m_tabGrpWithNoNote = (!this->FindDescendantByType(NOTE));

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
