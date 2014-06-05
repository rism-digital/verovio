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

#include "atts_analysis.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttCommonAnl
//----------------------------------------------------------------------------

AttCommonAnl::AttCommonAnl() {
    ResetCommonAnl();
}

AttCommonAnl::~AttCommonAnl() {

}

void AttCommonAnl::ResetCommonAnl() {
    m_copyof = "";
    m_corresp = "";
    m_next = "";
    m_prev = "";
    m_sameas = "";
    m_synch = "";
}

/* include <attsynch> */

//----------------------------------------------------------------------------
// AttHarmonicfunction
//----------------------------------------------------------------------------

AttHarmonicfunction::AttHarmonicfunction() {
    ResetHarmonicfunction();
}

AttHarmonicfunction::~AttHarmonicfunction() {

}

void AttHarmonicfunction::ResetHarmonicfunction() {
    m_hfunc = "";
}

/* include <atthfunc> */

//----------------------------------------------------------------------------
// AttIntervalharmonic
//----------------------------------------------------------------------------

AttIntervalharmonic::AttIntervalharmonic() {
    ResetIntervalharmonic();
}

AttIntervalharmonic::~AttIntervalharmonic() {

}

void AttIntervalharmonic::ResetIntervalharmonic() {
    m_inth = "";
}

/* include <attinth> */

//----------------------------------------------------------------------------
// AttIntervallicdesc
//----------------------------------------------------------------------------

AttIntervallicdesc::AttIntervallicdesc() {
    ResetIntervallicdesc();
}

AttIntervallicdesc::~AttIntervallicdesc() {

}

void AttIntervallicdesc::ResetIntervallicdesc() {
    m_intm = "";
}

/* include <attintm> */

//----------------------------------------------------------------------------
// AttMelodicfunction
//----------------------------------------------------------------------------

AttMelodicfunction::AttMelodicfunction() {
    ResetMelodicfunction();
}

AttMelodicfunction::~AttMelodicfunction() {

}

void AttMelodicfunction::ResetMelodicfunction() {
    m_mfunc = "";
}

/* include <attmfunc> */

//----------------------------------------------------------------------------
// AttPitchclass
//----------------------------------------------------------------------------

AttPitchclass::AttPitchclass() {
    ResetPitchclass();
}

AttPitchclass::~AttPitchclass() {

}

void AttPitchclass::ResetPitchclass() {
    m_pclass = "";
}

/* include <attpclass> */

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

AttSolfa::AttSolfa() {
    ResetSolfa();
}

AttSolfa::~AttSolfa() {

}

void AttSolfa::ResetSolfa() {
    m_psolfa = "";
}

/* include <attpsolfa> */

} // vrv namespace

