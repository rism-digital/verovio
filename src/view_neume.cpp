/////////////////////////////////////////////////////////////////////////////
// Name:        view_syllable.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Author and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "syllable.h"
#include "devicecontext.h"
#include "doc.h"
#include "layer.h"
#include "layerelement.h"
#include "note.h"
#include "rpt.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"
#include "vrv.h"

namespace vrv {

void View::DrawSyllable(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Syllable *syllable = dynamic_cast<Syllable *>(element);
    assert(syllable);


    /******************************************************************/
    // initialization

//    ListOfObjects *beamChildren = beam->GetList(beam);


    /******************************************************************/
    // Start the Beam graphic and draw the children

    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/
    // Draw the children
    DrawLayerChildren(dc, syllable, layer, staff, measure);


    dc->EndGraphic(element, this);
}

} // namespace vrv
