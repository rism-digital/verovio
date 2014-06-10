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

#include "atts_mensural.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttLigatureLog
//----------------------------------------------------------------------------

AttLigatureLog::AttLigatureLog() {
    ResetLigatureLog();
}

AttLigatureLog::~AttLigatureLog() {

}

void AttLigatureLog::ResetLigatureLog() {
    m_form = "";
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttMensurVis
//----------------------------------------------------------------------------

AttMensurVis::AttMensurVis() {
    ResetMensurVis();
}

AttMensurVis::~AttMensurVis() {

}

void AttMensurVis::ResetMensurVis() {
    m_form = "";
    m_orient = "";
}

/* include <attorient> */

//----------------------------------------------------------------------------
// AttMensurDefaultLog
//----------------------------------------------------------------------------

AttMensurDefaultLog::AttMensurDefaultLog() {
    ResetMensurDefaultLog();
}

AttMensurDefaultLog::~AttMensurDefaultLog() {

}

void AttMensurDefaultLog::ResetMensurDefaultLog() {
    m_mensurDot = "";
    m_mensurLocInt = 0;
    m_mensurSign = "";
    m_mensurSlashInt = 0;
}

/* include <attmensur.slash> */

//----------------------------------------------------------------------------
// AttMensurDefaultVis
//----------------------------------------------------------------------------

AttMensurDefaultVis::AttMensurDefaultVis() {
    ResetMensurDefaultVis();
}

AttMensurDefaultVis::~AttMensurDefaultVis() {

}

void AttMensurDefaultVis::ResetMensurDefaultVis() {
    m_mensurColor = "";
    m_mensurForm = "";
    m_mensurOrient = "";
    m_mensurSize = "";
}

/* include <attmensur.size> */

//----------------------------------------------------------------------------
// AttNoteLogMensural
//----------------------------------------------------------------------------

AttNoteLogMensural::AttNoteLogMensural() {
    ResetNoteLogMensural();
}

AttNoteLogMensural::~AttNoteLogMensural() {

}

void AttNoteLogMensural::ResetNoteLogMensural() {
    m_lig = "";
}

/* include <attlig> */

//----------------------------------------------------------------------------
// AttRestVisMensural
//----------------------------------------------------------------------------

AttRestVisMensural::AttRestVisMensural() {
    ResetRestVisMensural();
}

AttRestVisMensural::~AttRestVisMensural() {

}

void AttRestVisMensural::ResetRestVisMensural() {
    m_spacesInt = 0;
}

/* include <attspaces> */

//----------------------------------------------------------------------------
// AttStaffDefLogMensural
//----------------------------------------------------------------------------

AttStaffDefLogMensural::AttStaffDefLogMensural() {
    ResetStaffDefLogMensural();
}

AttStaffDefLogMensural::~AttStaffDefLogMensural() {

}

void AttStaffDefLogMensural::ResetStaffDefLogMensural() {
    m_proportNumInt = 0;
    m_proportNumbaseInt = 0;
}

/* include <attproport.numbase> */

//----------------------------------------------------------------------------
// AttStafflocPitched
//----------------------------------------------------------------------------

AttStafflocPitched::AttStafflocPitched() {
    ResetStafflocPitched();
}

AttStafflocPitched::~AttStafflocPitched() {

}

void AttStafflocPitched::ResetStafflocPitched() {
    m_ploc = "";
    m_oloc = "";
}

/* include <attoloc> */

} // vrv namespace

