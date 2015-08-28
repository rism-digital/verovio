/////////////////////////////////////////////////////////////////////////////
// Name:        rpt.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "rpt.h"

namespace vrv {
    

//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

BeatRpt::BeatRpt( ):
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

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

MRpt::MRpt( ):
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

MRpt2::MRpt2( ):
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

MultiRpt::MultiRpt( ):
    LayerElement("multirpt-")
{
    Reset();
}

MultiRpt::~MultiRpt()
{
}

void MultiRpt::Reset()
{
    LayerElement::Reset();
}
    
//----------------------------------------------------------------------------
// MRpt functor methods
//----------------------------------------------------------------------------

int MRpt::PrepareRpt( ArrayPtrVoid *params )
{
    // param 0: a pointer to the current MRpt pointer
    // param 1: a pointer to the data_BOOLEAN indicating if multiNumber
    // param 2: a pointer to the doc scoreDef (unused)
    MRpt **currentMRpt =  static_cast<MRpt**>((*params)[0]);
    data_BOOLEAN *multiNumber = static_cast<data_BOOLEAN*>((*params)[1]);
    
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
