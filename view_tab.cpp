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
    
    wchar_t code = note->GetTabSmuflCode(staff->m_drawingNotationType);
    
    // For some reason we need it to be cue size? Bravura is too large otherwise
    bool drawingCueSize = true;
    y -= (m_doc->GetGlyphHeight(code, staff->m_drawingStaffSize, drawingCueSize) / 2);
    
    DrawSmuflCode(dc, x, y, code, staff->m_drawingStaffSize, drawingCueSize);
    
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
    y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
    int drawingDur = tabGrp->GetActualDur();
    
    int symc = 0;
    switch (drawingDur) {
        case DUR_2: symc = SMUFL_EBA7_luteDurationWhole; break;
        case DUR_4: symc = SMUFL_EBA8_luteDurationHalf; break;
        case DUR_8: symc = SMUFL_EBA9_luteDurationQuarter; break;
        case DUR_16: symc = SMUFL_EBAA_luteDuration8th; break;
        case DUR_32: symc = SMUFL_EBAB_luteDuration16th; break;
        default: symc = SMUFL_EBA9_luteDurationQuarter;
    }
    
    DrawSmuflCode(dc, x, y, symc, staff->m_drawingStaffSize, true);
    
    // Draw children (nothing yet)
    DrawLayerChildren(dc, tabRhythm, layer, staff, measure);
    
    dc->EndGraphic(tabRhythm, this);
}

} // namespace vrv
