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
        DrawPgHead(dc, pgHead);
    }
}
    
void View::DrawPgHead(DeviceContext *dc, PgHead *pgHead)
{
    assert(dc);
    assert(pgHead);
    
}

} // namespace vrv
