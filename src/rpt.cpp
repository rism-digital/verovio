/////////////////////////////////////////////////////////////////////////////
// Name:        rpt.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rpt.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "functorparams.h"
#include "note.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

BeatRpt::BeatRpt() : LayerElement("beatrpt-"), AttBeatRptVis()
{
    RegisterAttClass(ATT_BEATRPTVIS);
    Reset();
}

BeatRpt::~BeatRpt()
{
}

void BeatRpt::Reset()
{
    LayerElement::Reset();
    ResetBeatRptVis();
}

double BeatRpt::GetBeatRptAlignmentDuration(int meterUnit) const
{
    return DUR_MAX / meterUnit;
}

//----------------------------------------------------------------------------
// BTrem
//----------------------------------------------------------------------------

BTrem::BTrem() : LayerElement("btrem-")
{
    Reset();
}

BTrem::~BTrem()
{
}

void BTrem::Reset()
{
    LayerElement::Reset();
}

void BTrem::AddChild(Object *child)
{
    if (child->Is() == CHORD) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is() == CLEF) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is() == NOTE) {
        assert(dynamic_cast<Note *>(child));
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
//----------------------------------------------------------------------------
// FTrem
//----------------------------------------------------------------------------

FTrem::FTrem() : LayerElement("ftrem-"), ObjectListInterface(), AttSlashcount()
{
    RegisterAttClass(ATT_SLASHCOUNT);

    Reset();
}

FTrem::~FTrem()
{
}

void FTrem::Reset()
{
    LayerElement::Reset();
    ResetSlashcount();
}

void FTrem::AddChild(Object *child)
{
    if (child->Is() == CHORD) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is() == CLEF) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is() == NOTE) {
        assert(dynamic_cast<Note *>(child));
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

void FTrem::FilterList(ListOfObjects *childList)
{
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (((*iter)->Is() != NOTE) && ((*iter)->Is() != CHORD)) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList->erase(iter);
            continue;
        }
        iter++;
    }
}

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

MRpt::MRpt() : LayerElement("mrpt-")
{
    Reset();
}

MRpt::~MRpt()
{
}

void MRpt::Reset()
{
    LayerElement::Reset();

    m_drawingMeasureCount = 0;
}

//----------------------------------------------------------------------------
// MRpt2
//----------------------------------------------------------------------------

MRpt2::MRpt2() : LayerElement("mrpt2-")
{
    Reset();
}

MRpt2::~MRpt2()
{
}

void MRpt2::Reset()
{
    LayerElement::Reset();
}

//----------------------------------------------------------------------------
// MultiRpt
//----------------------------------------------------------------------------

MultiRpt::MultiRpt() : LayerElement("multirpt-"), AttNumbered()
{
    RegisterAttClass(ATT_NUMBERED);
    Reset();
}

MultiRpt::~MultiRpt()
{
}

void MultiRpt::Reset()
{
    LayerElement::Reset();
    ResetNumbered();
}

//----------------------------------------------------------------------------
// MRpt functor methods
//----------------------------------------------------------------------------

int MRpt::PrepareRpt(FunctorParams *functorParams)
{
    PrepareRptParams *params = dynamic_cast<PrepareRptParams *>(functorParams);
    assert(params);

    // If multiNumber is not true, nothing needs to be done
    if (params->m_multiNumber != BOOLEAN_true) {
        return FUNCTOR_CONTINUE;
    }

    // If this is the first one, number has to be 2
    if (params->m_currentMRpt == NULL) {
        this->m_drawingMeasureCount = 2;
    }
    // Otherwise increment it
    else {
        this->m_drawingMeasureCount = params->m_currentMRpt->m_drawingMeasureCount + 1;
    }
    params->m_currentMRpt = this;
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
