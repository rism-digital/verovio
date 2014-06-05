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

#include "atts_critapp.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttCrit
//----------------------------------------------------------------------------

AttCrit::AttCrit() {
    ResetCrit();
}

AttCrit::~AttCrit() {

}

void AttCrit::ResetCrit() {
    m_cause = "";
}

/* include <attcause> */

//----------------------------------------------------------------------------
// AttSource
//----------------------------------------------------------------------------

AttSource::AttSource() {
    ResetSource();
}

AttSource::~AttSource() {

}

void AttSource::ResetSource() {
    m_source = "";
}

/* include <attsource> */

} // vrv namespace

