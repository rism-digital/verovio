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

#include "atts_tablature.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttNoteGesTablature
//----------------------------------------------------------------------------

AttNoteGesTablature::AttNoteGesTablature() {
    ResetNoteGesTablature();
}

AttNoteGesTablature::~AttNoteGesTablature() {

}

void AttNoteGesTablature::ResetNoteGesTablature() {
    m_tabFret = "";
    m_tabString = "";
}

/* include <atttab.string> */

//----------------------------------------------------------------------------
// AttStaffDefGesTablature
//----------------------------------------------------------------------------

AttStaffDefGesTablature::AttStaffDefGesTablature() {
    ResetStaffDefGesTablature();
}

AttStaffDefGesTablature::~AttStaffDefGesTablature() {

}

void AttStaffDefGesTablature::ResetStaffDefGesTablature() {
    m_tabStrings = "";
}

/* include <atttab.strings> */

} // vrv namespace

