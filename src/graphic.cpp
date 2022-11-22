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
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Graphic
//----------------------------------------------------------------------------

static const ClassRegistrar<Graphic> s_factory("graphic", GRAPHIC);

Graphic::Graphic() : Object(GRAPHIC, "graphic-"), AttPointing(), AttWidth(), AttHeight(), AttTyped()
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

} // namespace vrv
