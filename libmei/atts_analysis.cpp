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

#include "atts_analysis.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttCommonAnl
//----------------------------------------------------------------------------

AttCommonAnl::AttCommonAnl() : Att()
{
    ResetCommonAnl();
}

AttCommonAnl::~AttCommonAnl()
{
}

void AttCommonAnl::ResetCommonAnl()
{
    m_copyof = "";
    m_corresp = "";
    m_next = "";
    m_prev = "";
    m_sameas = "";
    m_synch = "";
}

bool AttCommonAnl::ReadCommonAnl(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("copyof")) {
        this->SetCopyof(StrToStr(element.attribute("copyof").value()));
        element.remove_attribute("copyof");
        hasAttribute = true;
    }
    if (element.attribute("corresp")) {
        this->SetCorresp(StrToStr(element.attribute("corresp").value()));
        element.remove_attribute("corresp");
        hasAttribute = true;
    }
    if (element.attribute("next")) {
        this->SetNext(StrToStr(element.attribute("next").value()));
        element.remove_attribute("next");
        hasAttribute = true;
    }
    if (element.attribute("prev")) {
        this->SetPrev(StrToStr(element.attribute("prev").value()));
        element.remove_attribute("prev");
        hasAttribute = true;
    }
    if (element.attribute("sameas")) {
        this->SetSameas(StrToStr(element.attribute("sameas").value()));
        element.remove_attribute("sameas");
        hasAttribute = true;
    }
    if (element.attribute("synch")) {
        this->SetSynch(StrToStr(element.attribute("synch").value()));
        element.remove_attribute("synch");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCommonAnl::WriteCommonAnl(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCopyof()) {
        element.append_attribute("copyof") = StrToStr(this->GetCopyof()).c_str();
        wroteAttribute = true;
    }
    if (this->HasCorresp()) {
        element.append_attribute("corresp") = StrToStr(this->GetCorresp()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNext()) {
        element.append_attribute("next") = StrToStr(this->GetNext()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPrev()) {
        element.append_attribute("prev") = StrToStr(this->GetPrev()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSameas()) {
        element.append_attribute("sameas") = StrToStr(this->GetSameas()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSynch()) {
        element.append_attribute("synch") = StrToStr(this->GetSynch()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCommonAnl::HasCopyof()
{
    return (m_copyof != "");
}

bool AttCommonAnl::HasCorresp()
{
    return (m_corresp != "");
}

bool AttCommonAnl::HasNext()
{
    return (m_next != "");
}

bool AttCommonAnl::HasPrev()
{
    return (m_prev != "");
}

bool AttCommonAnl::HasSameas()
{
    return (m_sameas != "");
}

bool AttCommonAnl::HasSynch()
{
    return (m_synch != "");
}

/* include <attsynch> */

//----------------------------------------------------------------------------
// AttHarmonicfunction
//----------------------------------------------------------------------------

AttHarmonicfunction::AttHarmonicfunction() : Att()
{
    ResetHarmonicfunction();
}

AttHarmonicfunction::~AttHarmonicfunction()
{
}

void AttHarmonicfunction::ResetHarmonicfunction()
{
    m_deg = SCALEDEGREE_NONE;
}

bool AttHarmonicfunction::ReadHarmonicfunction(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("deg")) {
        this->SetDeg(StrToScaledegree(element.attribute("deg").value()));
        element.remove_attribute("deg");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmonicfunction::WriteHarmonicfunction(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDeg()) {
        element.append_attribute("deg") = ScaledegreeToStr(this->GetDeg()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmonicfunction::HasDeg()
{
    return (m_deg != SCALEDEGREE_NONE);
}

/* include <attdeg> */

//----------------------------------------------------------------------------
// AttIntervalharmonic
//----------------------------------------------------------------------------

AttIntervalharmonic::AttIntervalharmonic() : Att()
{
    ResetIntervalharmonic();
}

AttIntervalharmonic::~AttIntervalharmonic()
{
}

void AttIntervalharmonic::ResetIntervalharmonic()
{
    m_inth = "";
}

bool AttIntervalharmonic::ReadIntervalharmonic(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("inth")) {
        this->SetInth(StrToStr(element.attribute("inth").value()));
        element.remove_attribute("inth");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIntervalharmonic::WriteIntervalharmonic(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasInth()) {
        element.append_attribute("inth") = StrToStr(this->GetInth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttIntervalharmonic::HasInth()
{
    return (m_inth != "");
}

/* include <attinth> */

//----------------------------------------------------------------------------
// AttIntervallicdesc
//----------------------------------------------------------------------------

AttIntervallicdesc::AttIntervallicdesc() : Att()
{
    ResetIntervallicdesc();
}

AttIntervallicdesc::~AttIntervallicdesc()
{
}

void AttIntervallicdesc::ResetIntervallicdesc()
{
    m_intm = INTERVAL_AMOUNT_NONE;
}

bool AttIntervallicdesc::ReadIntervallicdesc(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("intm")) {
        this->SetIntm(StrToIntervalAmount(element.attribute("intm").value()));
        element.remove_attribute("intm");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIntervallicdesc::WriteIntervallicdesc(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasIntm()) {
        element.append_attribute("intm") = IntervalAmountToStr(this->GetIntm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttIntervallicdesc::HasIntm()
{
    return (m_intm != INTERVAL_AMOUNT_NONE);
}

/* include <attintm> */

//----------------------------------------------------------------------------
// AttMelodicfunction
//----------------------------------------------------------------------------

AttMelodicfunction::AttMelodicfunction() : Att()
{
    ResetMelodicfunction();
}

AttMelodicfunction::~AttMelodicfunction()
{
}

void AttMelodicfunction::ResetMelodicfunction()
{
    m_mfunc = MELODICFUNCTION_NONE;
}

bool AttMelodicfunction::ReadMelodicfunction(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mfunc")) {
        this->SetMfunc(StrToMelodicfunction(element.attribute("mfunc").value()));
        element.remove_attribute("mfunc");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMelodicfunction::WriteMelodicfunction(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMfunc()) {
        element.append_attribute("mfunc") = MelodicfunctionToStr(this->GetMfunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMelodicfunction::HasMfunc()
{
    return (m_mfunc != MELODICFUNCTION_NONE);
}

/* include <attmfunc> */

//----------------------------------------------------------------------------
// AttPitchclass
//----------------------------------------------------------------------------

AttPitchclass::AttPitchclass() : Att()
{
    ResetPitchclass();
}

AttPitchclass::~AttPitchclass()
{
}

void AttPitchclass::ResetPitchclass()
{
    m_pclass = PITCHCLASS_NONE;
}

bool AttPitchclass::ReadPitchclass(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("pclass")) {
        this->SetPclass(StrToPitchclass(element.attribute("pclass").value()));
        element.remove_attribute("pclass");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPitchclass::WritePitchclass(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPclass()) {
        element.append_attribute("pclass") = PitchclassToStr(this->GetPclass()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPitchclass::HasPclass()
{
    return (m_pclass != PITCHCLASS_NONE);
}

/* include <attpclass> */

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

AttSolfa::AttSolfa() : Att()
{
    ResetSolfa();
}

AttSolfa::~AttSolfa()
{
}

void AttSolfa::ResetSolfa()
{
    m_psolfa = "";
}

bool AttSolfa::ReadSolfa(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("psolfa")) {
        this->SetPsolfa(StrToStr(element.attribute("psolfa").value()));
        element.remove_attribute("psolfa");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSolfa::WriteSolfa(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPsolfa()) {
        element.append_attribute("psolfa") = StrToStr(this->GetPsolfa()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSolfa::HasPsolfa()
{
    return (m_psolfa != "");
}

/* include <attpsolfa> */

bool Att::SetAnalysis(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_COMMONANL)) {
        AttCommonAnl *att = dynamic_cast<AttCommonAnl *>(element);
        assert(att);
        if (attrType == "copyof") {
            att->SetCopyof(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "corresp") {
            att->SetCorresp(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "next") {
            att->SetNext(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "prev") {
            att->SetPrev(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "sameas") {
            att->SetSameas(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "synch") {
            att->SetSynch(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARMONICFUNCTION)) {
        AttHarmonicfunction *att = dynamic_cast<AttHarmonicfunction *>(element);
        assert(att);
        if (attrType == "deg") {
            att->SetDeg(att->StrToScaledegree(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_INTERVALHARMONIC)) {
        AttIntervalharmonic *att = dynamic_cast<AttIntervalharmonic *>(element);
        assert(att);
        if (attrType == "inth") {
            att->SetInth(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_INTERVALLICDESC)) {
        AttIntervallicdesc *att = dynamic_cast<AttIntervallicdesc *>(element);
        assert(att);
        if (attrType == "intm") {
            att->SetIntm(att->StrToIntervalAmount(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MELODICFUNCTION)) {
        AttMelodicfunction *att = dynamic_cast<AttMelodicfunction *>(element);
        assert(att);
        if (attrType == "mfunc") {
            att->SetMfunc(att->StrToMelodicfunction(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PITCHCLASS)) {
        AttPitchclass *att = dynamic_cast<AttPitchclass *>(element);
        assert(att);
        if (attrType == "pclass") {
            att->SetPclass(att->StrToPitchclass(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SOLFA)) {
        AttSolfa *att = dynamic_cast<AttSolfa *>(element);
        assert(att);
        if (attrType == "psolfa") {
            att->SetPsolfa(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetAnalysis(Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_COMMONANL)) {
        AttCommonAnl *att = dynamic_cast<AttCommonAnl *>(element);
        assert(att);
        if (att->HasCopyof()) {
            attributes->push_back(std::make_pair("copyof", att->StrToStr(att->GetCopyof())));
        }
        if (att->HasCorresp()) {
            attributes->push_back(std::make_pair("corresp", att->StrToStr(att->GetCorresp())));
        }
        if (att->HasNext()) {
            attributes->push_back(std::make_pair("next", att->StrToStr(att->GetNext())));
        }
        if (att->HasPrev()) {
            attributes->push_back(std::make_pair("prev", att->StrToStr(att->GetPrev())));
        }
        if (att->HasSameas()) {
            attributes->push_back(std::make_pair("sameas", att->StrToStr(att->GetSameas())));
        }
        if (att->HasSynch()) {
            attributes->push_back(std::make_pair("synch", att->StrToStr(att->GetSynch())));
        }
    }
    if (element->HasAttClass(ATT_HARMONICFUNCTION)) {
        AttHarmonicfunction *att = dynamic_cast<AttHarmonicfunction *>(element);
        assert(att);
        if (att->HasDeg()) {
            attributes->push_back(std::make_pair("deg", att->ScaledegreeToStr(att->GetDeg())));
        }
    }
    if (element->HasAttClass(ATT_INTERVALHARMONIC)) {
        AttIntervalharmonic *att = dynamic_cast<AttIntervalharmonic *>(element);
        assert(att);
        if (att->HasInth()) {
            attributes->push_back(std::make_pair("inth", att->StrToStr(att->GetInth())));
        }
    }
    if (element->HasAttClass(ATT_INTERVALLICDESC)) {
        AttIntervallicdesc *att = dynamic_cast<AttIntervallicdesc *>(element);
        assert(att);
        if (att->HasIntm()) {
            attributes->push_back(std::make_pair("intm", att->IntervalAmountToStr(att->GetIntm())));
        }
    }
    if (element->HasAttClass(ATT_MELODICFUNCTION)) {
        AttMelodicfunction *att = dynamic_cast<AttMelodicfunction *>(element);
        assert(att);
        if (att->HasMfunc()) {
            attributes->push_back(std::make_pair("mfunc", att->MelodicfunctionToStr(att->GetMfunc())));
        }
    }
    if (element->HasAttClass(ATT_PITCHCLASS)) {
        AttPitchclass *att = dynamic_cast<AttPitchclass *>(element);
        assert(att);
        if (att->HasPclass()) {
            attributes->push_back(std::make_pair("pclass", att->PitchclassToStr(att->GetPclass())));
        }
    }
    if (element->HasAttClass(ATT_SOLFA)) {
        AttSolfa *att = dynamic_cast<AttSolfa *>(element);
        assert(att);
        if (att->HasPsolfa()) {
            attributes->push_back(std::make_pair("psolfa", att->StrToStr(att->GetPsolfa())));
        }
    }
}

} // vrv namespace
