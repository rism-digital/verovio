/////////////////////////////////////////////////////////////////////////////
// Name:        divline.cpp
// Author:      Yinan Zhou
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "divline.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "layer.h"
#include "measure.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// DivLine
//----------------------------------------------------------------------------

DivLine::DivLine() : LayerElement(DIVLINE, "dline-"), AttDivLineLog(), AttColor(), AttVisibility()
{
    this->RegisterAttClass(ATT_DIVLINELOG);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

DivLine::~DivLine() {}

void DivLine::Reset()
{
    LayerElement::Reset();

    // Reset();
    this->ResetColor();
    this->ResetVisibility();
}

std::wstring DivLine::GetSymbolStr() const
{
    if (!this->HasForm()) return L"";

    wchar_t symc = GetDivLineGlyph(this->GetForm());
    std::wstring symbolStr;

    symbolStr.push_back(symc);

    return symbolStr;
}

wchar_t DivLine::GetDivLineGlyph(divLineLog_FORM divLine)
{
    int symc = SMUFL_E8F5_chantDivisioMaxima;
    switch (divLine) {
        case divLineLog_FORM_minima: symc = SMUFL_E8F3_chantDivisioMinima; break;
        case divLineLog_FORM_maior: symc = SMUFL_E8F4_chantDivisioMaior; break;
        case divLineLog_FORM_maxima: symc = SMUFL_E8F5_chantDivisioMaxima; break;
        case divLineLog_FORM_finalis: symc = SMUFL_E8F6_chantDivisioFinalis; break;
        case divLineLog_FORM_caesura: symc = SMUFL_E8F8_chantCaesura; break;
        case divLineLog_FORM_virgula: symc = SMUFL_E8F7_chantVirgula; break;
        default: break;
    }
    return symc;
}
DivLineAttr::~DivLineAttr() {}

} // namespace vrv
