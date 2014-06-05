/////////////////////////////////////////////////////////////////////////////
// Name:        attributes.h
// Author:      Laurent Pugin
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

#include "atts_harmony.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttFretlocation
//----------------------------------------------------------------------------

AttFretlocation::AttFretlocation() {
    ResetFretlocation();
}

AttFretlocation::~AttFretlocation() {

}

void AttFretlocation::ResetFretlocation() {
    m_fret = "";
}

/* include <attfret> */

//----------------------------------------------------------------------------
// AttHarmLog
//----------------------------------------------------------------------------

AttHarmLog::AttHarmLog() {
    ResetHarmLog();
}

AttHarmLog::~AttHarmLog() {

}

void AttHarmLog::ResetHarmLog() {
    m_chordref = "";
}

/* include <attchordref> */

//----------------------------------------------------------------------------
// AttHarmVis
//----------------------------------------------------------------------------

AttHarmVis::AttHarmVis() {
    ResetHarmVis();
}

AttHarmVis::~AttHarmVis() {

}

void AttHarmVis::ResetHarmVis() {
    m_extender = "";
    m_rendgrid = "";
}

/* include <attrendgrid> */

} // vrv namespace

