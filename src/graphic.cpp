//
//  graphic.cpp
//  Verovio
//
//  Created by Werner Goebl on 26.09.22.
//

#include "graphic.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Graphic
//----------------------------------------------------------------------------

static const ClassRegistrar<Graphic> s_factory("graphic", GRAPHIC);

Graphic::Graphic() : Object(GRAPHIC), AttPointing(), AttWidth(), AttHeight(), AttTyped()
{
    this->RegisterAttClass(ATT_POINTING);
    this->RegisterAttClass(ATT_WIDTH);
    this->RegisterAttClass(ATT_HEIGHT);
    this->RegisterAttClass(ATT_TYPED);
    this->Reset();
}

Graphic::~Graphic() {}

void Graphic::Reset()
{
    this->ResetPointing();
    this->ResetWidth();
    this->ResetHeight();
    this->ResetTyped();
}

int Graphic::GetDrawingWidth(int unit, int staffSize) const
{
    if (!this->HasWidth() || staffSize == 0) return 0;
    if (this->GetWidth().GetType() == MEASUREMENTTYPE_px) return (this->GetWidth().GetPx() * staffSize / 100);
    // The staffSize is already taken into account in the unit
    return (this->GetWidth().GetVu() * unit);
}

int Graphic::GetDrawingHeight(int unit, int staffSize) const
{
    if (!this->HasHeight()) return 0;
    if (this->GetHeight().GetType() == MEASUREMENTTYPE_px) return (this->GetHeight().GetPx() * staffSize / 100);
    // The staffSize is already taken into account in the unit
    return (this->GetHeight().GetVu() * unit);
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Graphic::Accept(Functor &functor)
{
    return functor.VisitGraphic(this);
}

FunctorCode Graphic::Accept(ConstFunctor &functor) const
{
    return functor.VisitGraphic(this);
}

FunctorCode Graphic::AcceptEnd(Functor &functor)
{
    return functor.VisitGraphicEnd(this);
}

FunctorCode Graphic::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitGraphicEnd(this);
}

} // namespace vrv
