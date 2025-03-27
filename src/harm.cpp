/////////////////////////////////////////////////////////////////////////////
// Name:        harm.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "harm.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "f.h"
#include "fb.h"
#include "functor.h"
#include "measure.h"
#include "system.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Harm
//----------------------------------------------------------------------------

static const ClassRegistrar<Harm> s_factory("harm", HARM);

Harm::Harm()
    : ControlElement(HARM)
    , TextListInterface()
    , TextDirInterface()
    , TimeSpanningInterface()
    , AttLang()
    , AttNNumberLike()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Harm::~Harm() {}

void Harm::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    this->ResetLang();
    this->ResetNNumberLike();
}

bool Harm::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ FB, LB, REND, TEXT };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

bool Harm::GetRootPitch(TransPitch &pitch, unsigned int &pos) const
{
    const Text *textObject = vrv_cast<const Text *>(this->FindDescendantByType(TEXT, 1));
    if (!textObject) return false;
    std::u32string text = textObject->GetText();

    if (text.length() > pos && text.at(pos) >= 'A' && text.at(pos) <= 'G') {
        int pname = (text.at(pos) - 'C' + 7) % 7;
        int accid = 0;
        for (pos++; pos < text.length(); pos++) {
            if (text.at(pos) == UNICODE_DOUBLE_FLAT) {
                accid -= 2;
            }
            else if (text.at(pos) == 'b' || text.at(pos) == UNICODE_FLAT) {
                accid--;
            }
            else if (text.at(pos) == '#' || text.at(pos) == UNICODE_SHARP) {
                accid++;
            }
            else if (text.at(pos) == UNICODE_DOUBLE_SHARP) {
                accid += 2;
            }
            else {
                break;
            }
        }
        pitch = TransPitch(pname, accid, 4);
        return true;
    }
    LogWarning("Failed to extract a pitch.");
    return false;
}

void Harm::SetRootPitch(const TransPitch &pitch, unsigned int endPos)
{
    Text *textObject = vrv_cast<Text *>(this->FindDescendantByType(TEXT, 1));
    if (!textObject) return;
    std::u32string text = textObject->GetText();

    if (text.length() > endPos) {
        textObject->SetText(pitch.GetPitchString() + &text.at(endPos));
    }
    else {
        textObject->SetText(pitch.GetPitchString());
    }
}

bool Harm::GetBassPitch(TransPitch &pitch) const
{
    const Text *textObject = vrv_cast<const Text *>(this->FindDescendantByType(TEXT, 1));
    if (!textObject) return false;
    std::u32string text = textObject->GetText();
    if (!text.length()) return false;

    for (unsigned int pos = 0; pos < text.length(); pos++) {
        if (text.at(pos) == U'/') {
            pos++;
            return this->GetRootPitch(pitch, pos);
        }
    }
    return false;
}

void Harm::SetBassPitch(const TransPitch &pitch)
{
    Text *textObject = vrv_cast<Text *>(this->FindDescendantByType(TEXT, 1));
    if (!textObject) return;
    std::u32string text = textObject->GetText();
    unsigned int pos;
    for (pos = 0; pos < text.length(); pos++) {
        if (text.at(pos) == U'/') {
            break;
        }
    }

    text = text.substr(0, pos) + U"/" + pitch.GetPitchString();
    textObject->SetText(text);
}

//----------------------------------------------------------------------------
// Harm functor methods
//----------------------------------------------------------------------------

FunctorCode Harm::Accept(Functor &functor)
{
    return functor.VisitHarm(this);
}

FunctorCode Harm::Accept(ConstFunctor &functor) const
{
    return functor.VisitHarm(this);
}

FunctorCode Harm::AcceptEnd(Functor &functor)
{
    return functor.VisitHarmEnd(this);
}

FunctorCode Harm::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitHarmEnd(this);
}

} // namespace vrv
