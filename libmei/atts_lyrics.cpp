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

#include "atts_lyrics.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttVerseLog
//----------------------------------------------------------------------------

AttVerseLog::AttVerseLog(): Att() {
    ResetVerseLog();
}

AttVerseLog::~AttVerseLog() {

}

void AttVerseLog::ResetVerseLog() {
    m_refrain = BOOLEAN_NONE;
    m_rhythm = "";
}

bool AttVerseLog::ReadVerseLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("refrain")) {
        this->SetRefrain(StrToBoolean(element.attribute("refrain").value()));
        element.remove_attribute("refrain");
        hasAttribute = true;
    }
    if (element.attribute("rhythm")) {
        this->SetRhythm(StrToStr(element.attribute("rhythm").value()));
        element.remove_attribute("rhythm");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVerseLog::WriteVerseLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRefrain()) {
        element.append_attribute("refrain") = BooleanToStr(this->GetRefrain()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRhythm()) {
        element.append_attribute("rhythm") = StrToStr(this->GetRhythm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVerseLog::HasRefrain( )
{
    return (m_refrain != BOOLEAN_NONE);
}

bool AttVerseLog::HasRhythm( )
{
    return (m_rhythm != "");
}


/* include <attrhythm> */

bool Att::SetLyrics( Object *element, std::string attrType, std::string attrValue ) {
    if (element->HasAttClass( ATT_VERSELOG ) ) {
        AttVerseLog *att = dynamic_cast<AttVerseLog*>(element);
        assert( att );
        if (attrType == "refrain") {
            att->SetRefrain(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "rhythm") {
            att->SetRhythm(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetLyrics( Object *element, ArrayOfStrAttr *attributes ) {
    if (element->HasAttClass( ATT_VERSELOG ) ) {
        AttVerseLog *att = dynamic_cast<AttVerseLog*>(element);
        assert( att );
        if (att->HasRefrain()) {
            attributes->push_back(std::make_pair("refrain", att->BooleanToStr(att->GetRefrain())));
        }
        if (att->HasRhythm()) {
            attributes->push_back(std::make_pair("rhythm", att->StrToStr(att->GetRhythm())));
        }
    }

}
    
} // vrv namespace
    
