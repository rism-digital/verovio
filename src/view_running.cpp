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
#include "div.h"
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
        this->DrawTextLayoutElement(dc, header);
    }
    RunningElement *footer = page->GetFooter();
    if (footer) {
        this->DrawTextLayoutElement(dc, footer);
    }
}

void View::DrawTextLayoutElement(DeviceContext *dc, TextLayoutElement *textLayoutElement)
{
    assert(dc);
    assert(textLayoutElement);

    dc->StartGraphic(textLayoutElement, "", textLayoutElement->GetID());

    FontInfo textElementFont;
    if (!dc->UseGlobalStyling()) {
        textElementFont.SetFaceName("Times");
    }

    TextDrawingParams params;

    // If we have not timestamp
    params.m_x = textLayoutElement->GetDrawingX();
    params.m_y = textLayoutElement->GetDrawingY();
    params.m_width = textLayoutElement->GetTotalWidth(m_doc);
    params.m_alignment = HORIZONTALALIGNMENT_NONE;
    params.m_laidOut = true;
    params.m_pointSize = m_doc->GetDrawingLyricFont(100)->GetPointSize();

    textElementFont.SetPointSize(params.m_pointSize);

    dc->SetBrush(m_currentColor, AxSOLID);
    dc->SetFont(&textElementFont);

    this->DrawRunningChildren(dc, textLayoutElement, params);

    dc->ResetFont();
    dc->ResetBrush();

    dc->EndGraphic(textLayoutElement, this);
}

void View::DrawDiv(DeviceContext *dc, Div *div, System *system)
{
    this->DrawTextLayoutElement(dc, div);
}

} // namespace vrv
