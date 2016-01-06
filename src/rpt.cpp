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
#include "note.h"

namespace vrv {
    

//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

BeatRpt::BeatRpt():
    LayerElement("beatrpt-"),
    AttBeatRptVis()
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
    
double BeatRpt::GetAlignmentDuration(int meterUnit)
{
    return  DUR_MAX / meterUnit;
}

//----------------------------------------------------------------------------
// BTrem
//----------------------------------------------------------------------------

BTrem::BTrem():
LayerElement("btrem-")
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

void BTrem::AddLayerElement(LayerElement *element)
{
    assert(dynamic_cast<Note*>(element)
           || dynamic_cast<Chord*>(element)
           || dynamic_cast<EditorialElement*>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}
    
//----------------------------------------------------------------------------
// FTrem
//----------------------------------------------------------------------------

FTrem::FTrem():
    LayerElement("ftrem-"), ObjectListInterface(),
    AttSlashcount()
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
    
void FTrem::AddLayerElement(LayerElement *element)
{
    assert(dynamic_cast<Note*>(element)
           || dynamic_cast<Chord*>(element)
           || dynamic_cast<EditorialElement*>(element));
    element->SetParent(this);
    m_children.push_back(element);
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

MRpt::MRpt():
    LayerElement("mrpt-")
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

MRpt2::MRpt2():
    LayerElement("mrpt2-")
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

MultiRpt::MultiRpt():
    LayerElement("multirpt-"),
    AttNumbered()
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

int MRpt::PrepareRpt(ArrayPtrVoid *params)
{
    // param 0: a pointer to the current MRpt pointer
    // param 1: a pointer to the data_BOOLEAN indicating if multiNumber
    // param 2: a pointer to the doc scoreDef (unused)
    MRpt **currentMRpt =  static_cast<MRpt**>((*params).at(0));
    data_BOOLEAN *multiNumber = static_cast<data_BOOLEAN*>((*params).at(1));
    
    // If multiNumber is not true, nothing needs to be done
    if ((*multiNumber) != BOOLEAN_true) {
        return FUNCTOR_CONTINUE;
    }
    
    // If this is the first one, number has to be 2
    if ((*currentMRpt) == NULL) {
        this->m_drawingMeasureCount = 2;
    }
    // Otherwise increment it
    else {
        this->m_drawingMeasureCount = (*currentMRpt)->m_drawingMeasureCount + 1;
    }
    (*currentMRpt) = this;
    return FUNCTOR_CONTINUE;    
}

} // namespace vrv
