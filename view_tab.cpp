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
    
    int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    
    TextExtend extend;
    std::wstring notes;
    
    bool drawingCueSize = true; //tuplet->GetDrawingCueSize();
    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize * 0.8, drawingCueSize));
    notes = IntToTupletFigures((short int)note->GetTabFret());
    dc->GetSmuflTextExtent(notes, &extend);
    
    // adjust the baseline (to be improved with slanted brackets
    y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 6 / 5;
    
    DrawSmuflString(dc, x, y, notes, false, staff->m_drawingStaffSize * 0.8);
    
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
    
    TabGrp *tabGrp = dynamic_cast<TabGrp *>(tabRhythm->GetFirstParent(TABGRP));
    assert(tabGrp);
    
    dc->StartGraphic(tabRhythm, "", tabRhythm->GetUuid());
    
    int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    int drawingDur = tabGrp->GetActualDur();
    
    int symc = 0;
    switch (drawingDur) {
        case DUR_4: symc = SMUFL_E4E5_restQuarter; break;
        case DUR_8: symc = SMUFL_E4E6_rest8th; break;
        case DUR_16: symc = SMUFL_E4E7_rest16th; break;
        case DUR_32: symc = SMUFL_E4E8_rest32nd; break;
        case DUR_64: symc = SMUFL_E4E9_rest64th; break;
        case DUR_128: symc = SMUFL_E4EA_rest128th; break;
        case DUR_256: symc = SMUFL_E4EB_rest256th; break;
    }
    
    DrawSmuflCode(dc, x, y, symc, staff->m_drawingStaffSize, true);
    
    // Draw children (nothing yet)
    DrawLayerChildren(dc, tabRhythm, layer, staff, measure);
    
    dc->EndGraphic(tabRhythm, this);
}

} // namespace vrv
