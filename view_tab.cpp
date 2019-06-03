/////////////////////////////////////////////////////////////////////////////
// Name:        view_tab.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "options.h"
#include "note.h"
#include "rend.h"
#include "smufl.h"
#include "staff.h"
#include "system.h"
#include "tabgrp.h"
#include "tabrhythm.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

void View::DrawTabGrp(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    
    TabGrp *tabGrp = dynamic_cast<TabGrp *>(element);
    assert(tabGrp);
    
    dc->StartGraphic(tabGrp, "", tabGrp->GetUuid());
    
    // Draw children (rhyhtm, notes)
    DrawLayerChildren(dc, tabGrp, layer, staff, measure);
    
    dc->EndGraphic(tabGrp, this);
}
    
void View::DrawTabNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    
    Note *note = dynamic_cast<Note *>(element);
    assert(note);
    
    TabGrp *tabGrp = note->IsTabGrpNote();
    assert(tabGrp);
    
    dc->StartGraphic(note, "", note->GetUuid());
    
    int noteY = element->GetDrawingY();
    int noteX = element->GetDrawingX();
    int drawingDur;
    wchar_t fontNo;
    
    drawingDur = tabGrp->GetActualDur();
    
    /************** Noteheads: **************/
    
    if (drawingDur == DUR_1) {
        fontNo = SMUFL_E0FA_noteheadWholeFilled;
    }
    else {
            fontNo = SMUFL_E0A2_noteheadWhole;
    }
    
    DrawSmuflCode(dc, noteX, noteY, fontNo, staff->m_drawingStaffSize, false, true);
    
    // Draw children (nothing yet)
    DrawLayerChildren(dc, note, layer, staff, measure);
    
    dc->EndGraphic(note, this);
}
    
void View::DrawTabRhythm(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    
    TabRhythm *tabRhythm = dynamic_cast<TabRhythm *>(element);
    assert(tabRhythm);
    
    dc->StartGraphic(tabRhythm, "", tabRhythm->GetUuid());
    
    // Draw children (nothing yet)
    DrawLayerChildren(dc, tabRhythm, layer, staff, measure);
    
    dc->EndGraphic(tabRhythm, this);
}

} // namespace vrv
