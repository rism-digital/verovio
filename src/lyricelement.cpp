/////////////////////////////////////////////////////////////////////////////
// Name:        lyricelement.cpp
// Author:      Simon Waloschek
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "lyricelement.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "syl.h"
#include "volta.h"

namespace vrv {

//----------------------------------------------------------------------------
// LyricElement
//----------------------------------------------------------------------------

LyricElement::LyricElement(ClassId classId)
    : LayerElement(classId)
    , OffsetInterface()
    , AttColor()
    , AttLang()
    , AttPlacementRelStaff()
    , AttTypography()
    , AttVoltaGroupingSym()
{
    this->RegisterInterface(OffsetInterface::GetAttClasses(), OffsetInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);
    this->RegisterAttClass(ATT_TYPOGRAPHY);
    this->RegisterAttClass(ATT_VOLTAGROUPINGSYM);
}

LyricElement::~LyricElement() {}

void LyricElement::Reset()
{
    LayerElement::Reset();
    OffsetInterface::Reset();
    this->ResetColor();
    this->ResetLang();
    this->ResetPlacementRelStaff();
    this->ResetTypography();
    this->ResetVoltaGroupingSym();
    this->ResetDrawingLyricGroup();
}

bool LyricElement::IsSupportedChild(ClassId classId)
{
    if ((classId == SYL) || (classId == VOLTA)) {
        return true;
    }
    return Object::IsEditorialElement(classId);
}

int LyricElement::GetVoltaCount() const
{
    return (int)this->FindAllDescendantsByType(VOLTA).size();
}

std::pair<int, int> LyricElement::GetVoltaDrawingRange() const
{
    int first = 0;
    int last = 0;
    for (const Object *object : this->FindAllDescendantsByType(VOLTA)) {
        const Volta *volta = vrv_cast<const Volta *>(object);
        assert(volta);
        const int drawingN = volta->GetDrawingVoltaN();
        first = (first == 0) ? drawingN : std::min(first, drawingN);
        last = std::max(last, drawingN);
    }
    return { first, last };
}

bool LyricElement::HasDirectSyl() const
{
    const ListOfConstObjects syls = this->FindAllDescendantsByType(SYL);
    return std::any_of(
        syls.begin(), syls.end(), [](const Object *syl) { return (syl->GetFirstAncestor(VOLTA) == NULL); });
}

int LyricElement::GetLyricLineCount() const
{
    const int lastVoltaTrack = this->GetVoltaDrawingRange().second;
    return std::max(1, lastVoltaTrack + ((lastVoltaTrack && this->HasDrawingDirectSylTrack()) ? 1 : 0));
}

int LyricElement::GetVoltaLineN(const Volta *volta) const
{
    assert(volta);
    return volta->GetDrawingVoltaN() + (this->HasDrawingDirectSylTrack() ? 1 : 0);
}

int LyricElement::AdjustPosition(int &overlap, int freeSpace, const Doc *doc)
{
    assert(doc);

    int nextFreeSpace = 0;
    if (overlap > 0) {
        if (freeSpace > overlap) {
            this->SetDrawingXRel(this->GetDrawingXRel() - overlap);
            overlap = 0;
        }
        else if (freeSpace > 0) {
            this->SetDrawingXRel(this->GetDrawingXRel() - freeSpace);
            overlap -= freeSpace;
        }
    }
    else {
        nextFreeSpace = std::min(-overlap, 3 * doc->GetDrawingUnit(100));
    }
    return nextFreeSpace;
}

} // namespace vrv
