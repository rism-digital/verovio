/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei 
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////// 
// NOTE: this file was generated with the Verovio libmei version and 
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#include "atts_layout.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttBarline
//----------------------------------------------------------------------------

AttBarline::AttBarline() {
    ResetBarline();
}

AttBarline::~AttBarline() {

}

void AttBarline::ResetBarline() {
    m_barline = "";
}

/* include <attbarline> */

//----------------------------------------------------------------------------
// AttIgnore
//----------------------------------------------------------------------------

AttIgnore::AttIgnore() {
    ResetIgnore();
}

AttIgnore::~AttIgnore() {

}

void AttIgnore::ResetIgnore() {
    m_ignored = "";
}

/* include <attignored> */

//----------------------------------------------------------------------------
// AttLayout
//----------------------------------------------------------------------------

AttLayout::AttLayout() {
    ResetLayout();
}

AttLayout::~AttLayout() {

}

void AttLayout::ResetLayout() {
    m_ignored = "";
}

/* include <attignored> */

//----------------------------------------------------------------------------
// AttMeasureRef
//----------------------------------------------------------------------------

AttMeasureRef::AttMeasureRef() {
    ResetMeasureRef();
}

AttMeasureRef::~AttMeasureRef() {

}

void AttMeasureRef::ResetMeasureRef() {
    m_measureref = "";
}

/* include <attmeasureref> */

//----------------------------------------------------------------------------
// AttPageRef
//----------------------------------------------------------------------------

AttPageRef::AttPageRef() {
    ResetPageRef();
}

AttPageRef::~AttPageRef() {

}

void AttPageRef::ResetPageRef() {
    m_pbrefs = "";
}

/* include <attpbrefs> */

//----------------------------------------------------------------------------
// AttSurface
//----------------------------------------------------------------------------

AttSurface::AttSurface() {
    ResetSurface();
}

AttSurface::~AttSurface() {

}

void AttSurface::ResetSurface() {
    m_surface = "";
}

/* include <attsurface> */

//----------------------------------------------------------------------------
// AttSystemRef
//----------------------------------------------------------------------------

AttSystemRef::AttSystemRef() {
    ResetSystemRef();
}

AttSystemRef::~AttSystemRef() {

}

void AttSystemRef::ResetSystemRef() {
    m_sbrefs = "";
}

/* include <attsbrefs> */

} // vrv namespace

