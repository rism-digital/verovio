/////////////////////////////////////////////////////////////////////////////
// Name:        dynam.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dynam.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <sstream>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "hairpin.h"
#include "rend.h"
#include "smufl.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

#define DYNAM_CHARS 7
const std::u32string dynamChars[] = { U"p", U"m", U"f", U"r", U"s", U"z", U"n" };
const std::u32string dynamSmufl[] = { U"\uE520", U"\uE521", U"\uE522", U"\uE523", U"\uE524", U"\uE525", U"\uE526" };

//----------------------------------------------------------------------------
// Dynam
//----------------------------------------------------------------------------

static const ClassRegistrar<Dynam> s_factory("dynam", DYNAM);

Dynam::Dynam()
    : ControlElement(DYNAM)
    , TextListInterface()
    , TextDirInterface()
    , TimeSpanningInterface()
    , AttEnclosingChars()
    , AttExtender()
    , AttLineRendBase()
    , AttMidiValue()
    , AttMidiValue2()
    , AttVerticalGroup()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_EXTENDER);
    this->RegisterAttClass(ATT_LINERENDBASE);
    this->RegisterAttClass(ATT_MIDIVALUE);
    this->RegisterAttClass(ATT_MIDIVALUE2);
    this->RegisterAttClass(ATT_VERTICALGROUP);

    this->Reset();
}

Dynam::~Dynam() {}

void Dynam::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    this->ResetEnclosingChars();
    this->ResetExtender();
    this->ResetLineRendBase();
    this->ResetVerticalGroup();
}

bool Dynam::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LB, REND, TEXT };

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

bool Dynam::IsSymbolOnly() const
{
    m_symbolStr = U"";
    std::u32string str = this->GetText();
    if (Dynam::IsSymbolOnly(str)) {
        m_symbolStr = str;
        return true;
    }
    return false;
}

std::u32string Dynam::GetSymbolStr(const bool singleGlyphs) const
{
    return Dynam::GetSymbolStr(m_symbolStr, singleGlyphs);
}

std::pair<char32_t, char32_t> Dynam::GetEnclosingGlyphs() const
{
    if (this->HasEnclose()) {
        switch (this->GetEnclose()) {
            case ENCLOSURE_brack: return { SMUFL_E26C_accidentalBracketLeft, SMUFL_E26D_accidentalBracketRight };
            case ENCLOSURE_paren: return { SMUFL_E26A_accidentalParensLeft, SMUFL_E26B_accidentalParensRight };
            default: break;
        }
    }
    return { 0, 0 };
}

//----------------------------------------------------------------------------
// Static methods for Dynam
//----------------------------------------------------------------------------

bool Dynam::GetSymbolsInStr(std::u32string str, ArrayOfStringDynamTypePairs &tokens)
{
    tokens.clear();

    std::u32string token = U"";
    bool hasSymbols = false;
    while (str.compare(token) != 0) {
        auto index = str.find_first_of(U" ");
        token = str.substr(0, index);
        if (Dynam::IsSymbolOnly(token)) {
            hasSymbols = true;
            if (tokens.size() > 0) {
                // previous one is not a symbol, add a space to it
                if (tokens.back().second == false) {
                    tokens.back().first = tokens.back().first + U" ";
                }
                // previous one it also a symbol, add a space in between
                else {
                    tokens.push_back({ U" ", false });
                }
            }
            // Add it in all cases
            tokens.push_back({ token, true });
        }
        else {
            if (tokens.size() > 0) {
                // previous one is not a symbol, append token to it with a space
                if (tokens.back().second == false) {
                    tokens.back().first = tokens.back().first + U" " + token;
                }
                // previous one is not a symbol, add it separately but with a space
                else {
                    tokens.push_back({ U" " + token, false });
                }
            }
            // First one, just add it
            else {
                tokens.push_back({ token, false });
            }
        }
        // no ' '
        if (index == std::string::npos) break;
        // next dynam token
        token = U"";
        str = str.substr(index + 1, str.length());
    }

    return hasSymbols;
}

bool Dynam::IsSymbolOnly(const std::u32string &str)
{
    if (str.empty()) return false;

    if (str.find_first_not_of(U"fpmrszn") == std::string::npos) {
        return true;
    }
    return false;
}

std::u32string Dynam::GetSymbolStr(const std::u32string &str, const bool singleGlyphs)
{
    std::u32string dynam;
    if (!singleGlyphs) {
        if (str == U"p")
            dynam.push_back(SMUFL_E520_dynamicPiano);
        else if (str == U"m")
            dynam.push_back(SMUFL_E521_dynamicMezzo);
        else if (str == U"f")
            dynam.push_back(SMUFL_E522_dynamicForte);
        else if (str == U"r")
            dynam.push_back(SMUFL_E523_dynamicRinforzando);
        else if (str == U"s")
            dynam.push_back(SMUFL_E524_dynamicSforzando);
        else if (str == U"z")
            dynam.push_back(SMUFL_E525_dynamicZ);
        else if (str == U"n")
            dynam.push_back(SMUFL_E526_dynamicNiente);
        else if (str == U"pppppp")
            dynam.push_back(SMUFL_E527_dynamicPPPPPP);
        else if (str == U"ppppp")
            dynam.push_back(SMUFL_E528_dynamicPPPPP);
        else if (str == U"pppp")
            dynam.push_back(SMUFL_E529_dynamicPPPP);
        else if (str == U"ppp")
            dynam.push_back(SMUFL_E52A_dynamicPPP);
        else if (str == U"pp")
            dynam.push_back(SMUFL_E52B_dynamicPP);
        else if (str == U"mp")
            dynam.push_back(SMUFL_E52C_dynamicMP);
        else if (str == U"mf")
            dynam.push_back(SMUFL_E52D_dynamicMF);
        else if (str == U"pf")
            dynam.push_back(SMUFL_E52E_dynamicPF);
        else if (str == U"ff")
            dynam.push_back(SMUFL_E52F_dynamicFF);
        else if (str == U"fff")
            dynam.push_back(SMUFL_E530_dynamicFFF);
        else if (str == U"ffff")
            dynam.push_back(SMUFL_E531_dynamicFFFF);
        else if (str == U"fffff")
            dynam.push_back(SMUFL_E532_dynamicFFFFF);
        else if (str == U"ffffff")
            dynam.push_back(SMUFL_E533_dynamicFFFFFF);
        else if (str == U"fp")
            dynam.push_back(SMUFL_E534_dynamicFortePiano);
        else if (str == U"fz")
            dynam.push_back(SMUFL_E535_dynamicForzando);
        else if (str == U"sf")
            dynam.push_back(SMUFL_E536_dynamicSforzando1);
        else if (str == U"sfp")
            dynam.push_back(SMUFL_E537_dynamicSforzandoPiano);
        else if (str == U"sfpp")
            dynam.push_back(SMUFL_E538_dynamicSforzandoPianissimo);
        else if (str == U"sfz")
            dynam.push_back(SMUFL_E539_dynamicSforzato);
        else if (str == U"sfzp")
            dynam.push_back(SMUFL_E53A_dynamicSforzatoPiano);
        else if (str == U"sffz")
            dynam.push_back(SMUFL_E53B_dynamicSforzatoFF);
        else if (str == U"rf")
            dynam.push_back(SMUFL_E53C_dynamicRinforzando1);
        else if (str == U"rfz")
            dynam.push_back(SMUFL_E53D_dynamicRinforzando2);
    }

    if (!dynam.empty()) return dynam;

    // Otherwise replace it letter by letter
    dynam = str;
    std::u32string from, to;
    for (int i = 0; i < DYNAM_CHARS; ++i) {
        from = dynamChars[i];
        to = dynamSmufl[i];
        for (size_t pos = 0; (pos = dynam.find(from, pos)) != std::string::npos; pos += to.size())
            dynam.replace(pos, from.size(), to);
    }
    return dynam;
}

//----------------------------------------------------------------------------
// Dynam functor methods
//----------------------------------------------------------------------------

FunctorCode Dynam::Accept(Functor &functor)
{
    return functor.VisitDynam(this);
}

FunctorCode Dynam::Accept(ConstFunctor &functor) const
{
    return functor.VisitDynam(this);
}

FunctorCode Dynam::AcceptEnd(Functor &functor)
{
    return functor.VisitDynamEnd(this);
}

FunctorCode Dynam::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitDynamEnd(this);
}

} // namespace vrv
