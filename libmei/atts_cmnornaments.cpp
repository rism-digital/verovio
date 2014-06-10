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

#include "atts_cmnornaments.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttMordentLog
//----------------------------------------------------------------------------

AttMordentLog::AttMordentLog() {
    ResetMordentLog();
}

AttMordentLog::~AttMordentLog() {

}

void AttMordentLog::ResetMordentLog() {
    m_form = "";
    m_long = "";
}

/* include <attlong> */

//----------------------------------------------------------------------------
// AttOrnam
//----------------------------------------------------------------------------

AttOrnam::AttOrnam() {
    ResetOrnam();
}

AttOrnam::~AttOrnam() {

}

void AttOrnam::ResetOrnam() {
    m_ornam = "";
}

/* include <attornam> */

//----------------------------------------------------------------------------
// AttOrnamentaccid
//----------------------------------------------------------------------------

AttOrnamentaccid::AttOrnamentaccid() {
    ResetOrnamentaccid();
}

AttOrnamentaccid::~AttOrnamentaccid() {

}

void AttOrnamentaccid::ResetOrnamentaccid() {
    m_accidupper = "";
    m_accidlower = "";
}

/* include <attaccidlower> */

//----------------------------------------------------------------------------
// AttTurnLog
//----------------------------------------------------------------------------

AttTurnLog::AttTurnLog() {
    ResetTurnLog();
}

AttTurnLog::~AttTurnLog() {

}

void AttTurnLog::ResetTurnLog() {
    m_delayed = "";
    m_form = "";
}

/* include <attform> */

} // vrv namespace

