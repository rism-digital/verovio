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

#include "atts_edittrans.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAgentident
//----------------------------------------------------------------------------

AttAgentident::AttAgentident() {
    ResetAgentident();
}

AttAgentident::~AttAgentident() {

}

void AttAgentident::ResetAgentident() {
    m_agent = "";
}

/* include <attagent> */

//----------------------------------------------------------------------------
// AttEdit
//----------------------------------------------------------------------------

AttEdit::AttEdit() {
    ResetEdit();
}

AttEdit::~AttEdit() {

}

void AttEdit::ResetEdit() {
    m_cert = "";
    m_evidence = "";
}

/* include <attevidence> */

//----------------------------------------------------------------------------
// AttExtent
//----------------------------------------------------------------------------

AttExtent::AttExtent() {
    ResetExtent();
}

AttExtent::~AttExtent() {

}

void AttExtent::ResetExtent() {
    m_extent = "";
}

/* include <attextent> */

//----------------------------------------------------------------------------
// AttReasonident
//----------------------------------------------------------------------------

AttReasonident::AttReasonident() {
    ResetReasonident();
}

AttReasonident::~AttReasonident() {

}

void AttReasonident::ResetReasonident() {
    m_reason = "";
}

/* include <attreason> */

} // vrv namespace

