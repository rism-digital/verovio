/////////////////////////////////////////////////////////////////////////////
// Name:        view_running.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "bboxdevicecontext.h"
#include "devicecontext.h"
#include "doc.h"
#include "page.h"
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
        BBoxDeviceContext *bBoxDC = vrv_cast<BBoxDeviceContext *>(dc);
        assert(bBoxDC);
        if (!bBoxDC->UpdateVerticalValues()) return;
    }

    RunningElement *header = page->GetHeader();
    if (header) {
        this->DrawPgHeader(dc, header);
    }
    RunningElement *footer = page->GetFooter();
    if (footer) {
        this->DrawPgHeader(dc, footer);
    }
}

void View::DrawPgHeader(DeviceContext *dc, RunningElement *pgHeader)
{
    assert(dc);
    assert(pgHeader);

    dc->StartGraphic(pgHeader, "", pgHeader->GetID());

    FontInfo pgHeadTxt;
    if (!dc->UseGlobalStyling()) {
        pgHeadTxt.SetFaceName("Times");
    }

    TextDrawingParams params;

    // If we have not timestamp
    params.m_x = pgHeader->GetDrawingX();
    params.m_y = pgHeader->GetDrawingY();
    params.m_width = pgHeader->GetWidth();
    params.m_alignment = HORIZONTALALIGNMENT_NONE;
    params.m_laidOut = true;
    params.m_pointSize = m_doc->GetDrawingLyricFont(100)->GetPointSize();

    pgHeadTxt.SetPointSize(params.m_pointSize);

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(&pgHeadTxt);

    this->DrawRunningChildren(dc, pgHeader, params);

    dc->ResetFont();
    dc->ResetBrush();

    dc->EndGraphic(pgHeader, this);
}

} // namespace vrv
