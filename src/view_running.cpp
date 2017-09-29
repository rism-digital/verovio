/////////////////////////////////////////////////////////////////////////////
// Name:        view_running.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "bboxdevicecontext.h"
#include "devicecontext.h"
#include "doc.h"
#include "pgfoot.h"
#include "pgfoot2.h"
#include "pghead.h"
#include "pghead2.h"
#include "text.h"
#include "vrv.h"

namespace vrv {
    
void View::DrawRunningElements(DeviceContext *dc, Page *page)
{
    assert(dc);
    assert(page);
    
    if (dc->Is(BBOX_DEVICE_CONTEXT)) {
        BBoxDeviceContext *bBoxDC = dynamic_cast<BBoxDeviceContext *>(dc);
        assert(bBoxDC);
        if (!bBoxDC->UpdateVerticalValues()) return;
    }
    
    PgHead *pgHead = m_doc->m_scoreDef.GetPgHead();
    if (pgHead) {
        DrawPgHeader(dc, pgHead);
    }
    PgHead2 *pgHead2 = m_doc->m_scoreDef.GetPgHead2();
    if (pgHead2) {
        DrawPgHeader(dc, pgHead);
    }
    PgFoot *pgFoot = m_doc->m_scoreDef.GetPgFoot();
    if (pgFoot) {
        DrawPgHeader(dc, pgFoot);
    }
    PgFoot2 *pgFoot2 = m_doc->m_scoreDef.GetPgFoot2();
    if (pgFoot2) {
        DrawPgHeader(dc, pgFoot2);
    }
}
    
void View::DrawPgHeader(DeviceContext *dc, RunningElement *pgHeader)
{
    assert(dc);
    assert(pgHeader);
    
    dc->StartGraphic(pgHeader, "", pgHeader->GetUuid());
    
    FontInfo pgHeadTxt;
    
    TextDrawingParams params;
    
    // If we have not timestamp
    params.m_x = pgHeader->GetDrawingX();
    params.m_y = pgHeader->GetDrawingY();
    params.m_width = pgHeader->GetWidth();
    params.m_alignment = HORIZONTALALIGNMENT_left;
    params.m_laidOut = true;
    
    pgHeadTxt.SetPointSize(m_doc->GetDrawingLyricFont(100)->GetPointSize());
    
    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(&pgHeadTxt);
    
    DrawRunningChildren(dc, pgHeader, params);
    
    dc->ResetFont();
    dc->ResetBrush();

    dc->EndGraphic(pgHeader, this);
}

} // namespace vrv
