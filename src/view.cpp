/////////////////////////////////////////////////////////////////////////////
// Name:        view.cpp
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <sstream>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "page.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View
//----------------------------------------------------------------------------

View::View()
{
    m_doc = NULL;
    m_options = NULL;
    m_slurHandling = SlurHandling::Initialize;

    m_currentColor = COLOR_NONE;
}

View::~View() {}

void View::SetDoc(Doc *doc)
{
    // Unset the doc
    if (doc == NULL) {
        m_doc = NULL;
        m_options = NULL;
    }
    else {
        m_doc = doc;
        m_options = doc->GetOptions();
    }
    m_currentPage = NULL;
}

void View::SetPage(Page *page, bool doLayout)
{
    assert(page); // Page cannot be NULL

    m_currentPage = page;

    if (doLayout) {
        m_doc->ScoreDefSetCurrentDoc();
        // if we once deal with multiple views, it would be better
        // to redo the layout only when necessary?
        if (m_doc->IsTranscription() || m_doc->IsFacs()) {
            m_currentPage->LayOutTranscription();
        }
        else {
            m_currentPage->LayOut();
        }
    }
}

int View::ToDeviceContextX(int i)
{
    return i;
} // the same

/** x value in the Logical world */
int View::ToLogicalX(int i)
{
    return i;
}

/** y value in the View */
int View::ToDeviceContextY(int i)
{
    if (!m_doc) {
        return 0;
    }

    return (m_doc->m_drawingPageContentHeight - i); // flipped
}

/** y value in the Logical world  */
int View::ToLogicalY(int i)
{
    if (!m_doc) {
        return 0;
    }

    return m_doc->m_drawingPageContentHeight - i; // flipped
}

Point View::ToDeviceContext(Point p)
{
    return Point(this->ToDeviceContextX(p.x), this->ToDeviceContextY(p.y));
}

Point View::ToLogical(Point p)
{
    return Point(this->ToLogicalX(p.x), this->ToLogicalY(p.y));
}

std::u32string View::IntToTupletFigures(unsigned short number)
{
    return this->IntToSmuflFigures(number, 0xE880);
}

std::u32string View::IntToTimeSigFigures(unsigned short number)
{
    return this->IntToSmuflFigures(number, 0xE080);
}

std::u32string View::IntToSmuflFigures(unsigned short number, int offset)
{
    std::ostringstream stream;
    stream << number;
    std::u32string str = UTF8to32(stream.str());

    for (char32_t &c : str) {
        c += offset - 48;
    }
    return str;
}

void View::StartOffset(DeviceContext *dc, const Object *object, int staffSize)
{
    if (!dc->ApplyOffset()) return;

    const int unit = m_doc->GetOptions()->m_unit.GetValue();

    Offset offset;

    if (object->HasInterface(INTERFACE_OFFSET)) {
        const OffsetInterface *interface = object->GetOffsetInterface();
        assert(interface);

        if (interface->HasHo() || interface->HasVo()) {
            offset.m_ho = (interface->HasHo()) ? interface->GetHo().GetVu() * unit : 0;
            offset.m_vo = (interface->HasVo()) ? interface->GetVo().GetVu() * unit : 0;
            offset.m_object = object;
            offset.m_staffSize = staffSize;
        }
    }

    if (object->HasInterface(INTERFACE_OFFSET_SPANNING)) {
        const OffsetSpanningInterface *interface = object->GetOffsetSpanningInterface();
        assert(interface);

        if (interface->HasStartho() || interface->HasStartvo() || interface->HasEndho() || interface->HasEndvo()) {
            offset.m_startho = (interface->HasStartho()) ? interface->GetStartho().GetVu() * unit : 0;
            offset.m_startvo = (interface->HasStartvo()) ? interface->GetStartvo().GetVu() * unit : 0;
            offset.m_endho = (interface->HasEndho()) ? interface->GetEndho().GetVu() * unit : 0;
            offset.m_endvo = (interface->HasEndvo()) ? interface->GetEndvo().GetVu() * unit : 0;
            offset.m_object = object;
            offset.m_staffSize = staffSize;
        }
    }

    // This means we have at least one offset value
    if (offset.m_object) m_currentOffsets.push_front(offset);
}

void View::EndOffset(DeviceContext *dc, const Object *object)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    if (m_currentOffsets.front().m_object == object) m_currentOffsets.pop_front();
}

void View::SetOffsetStaffSize(const Object *object, int staffSize)
{
    if (m_currentOffsets.empty()) return;

    if (m_currentOffsets.front().m_object == object) m_currentOffsets.front().m_staffSize = staffSize;
}

void View::CalcOffset(DeviceContext *dc, int &x, int &y)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        x = x + offset.m_ho * offset.m_staffSize / 100;
        y = y + offset.m_vo * offset.m_staffSize / 100;
    }
}

void View::CalcOffsetX(DeviceContext *dc, int &x)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        x = x + offset.m_ho * offset.m_staffSize / 100;
    }
}

void View::CalcOffsetY(DeviceContext *dc, int &y)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        y = y + offset.m_vo * offset.m_staffSize / 100;
    }
}

void View::CalcOffsetSpanningStartX(DeviceContext *dc, int &x, char spanningType, double factor)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        if (spanningType == SPANNING_START_END) {
            x = x + offset.m_startho * offset.m_staffSize / 100 * factor;
        }
        else if (spanningType == SPANNING_START) {
            x = x + offset.m_startho * offset.m_staffSize / 100 * factor;
        }
    }
}

void View::CalcOffsetSpanningEndX(DeviceContext *dc, int &x, char spanningType, double factor)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        if (spanningType == SPANNING_START_END) {
            x = x + offset.m_endho * offset.m_staffSize / 100 * factor;
        }
        else if (spanningType == SPANNING_END) {
            x = x + offset.m_endho * offset.m_staffSize / 100 * factor;
        }
    }
}

void View::CalcOffsetSpanningStartY(DeviceContext *dc, int &y, char spanningType, double factor)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        if (spanningType == SPANNING_START_END) {
            y = y + offset.m_startvo * offset.m_staffSize / 100 * factor;
        }
        else if (spanningType == SPANNING_START) {
            y = y + offset.m_startvo * offset.m_staffSize / 100 * factor;
        }
        else if (spanningType == SPANNING_END) {
            y = y + (offset.m_startvo + offset.m_endvo) / 2 * offset.m_staffSize / 100 * factor;
        }
        else {
            const int diff = (offset.m_startvo - offset.m_endvo) / 2;
            y = y + ((offset.m_startvo + offset.m_endvo) / 2 + diff) * offset.m_staffSize / 100 * factor;
        }
    }
}

void View::CalcOffsetSpanningEndY(DeviceContext *dc, int &y, char spanningType, double factor)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        if (spanningType == SPANNING_START_END) {
            y = y + offset.m_endvo * offset.m_staffSize / 100 * factor;
        }
        else if (spanningType == SPANNING_START) {
            y = y + (offset.m_endvo + offset.m_startvo) / 2 * offset.m_staffSize / 100 * factor;
        }
        else if (spanningType == SPANNING_END) {
            y = y + offset.m_endvo * offset.m_staffSize / 100 * factor;
        }
        else {
            const int diff = (offset.m_endvo - offset.m_startvo) / 2;
            y = y + ((offset.m_startvo + offset.m_endvo) / 2 + diff) * offset.m_staffSize / 100 * factor;
        }
    }
}

void View::CalcOffsetBezier(DeviceContext *dc, Point points[4], char spanningType)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    double diff = double(points[3].x - points[0].x);
    // factor for start control point
    double factorStart = (diff != 0.0) ? double(points[3].x - points[1].x) / diff : 1.0;
    // factor for end control point
    double factorEnd = (diff != 0.0) ? (points[2].x - points[0].x) / diff : 1.0;

    // Adjust the start point y
    this->CalcOffsetSpanningStartY(dc, points[0].y, spanningType);
    // Adjust the start (first) control point y considering both the startho and endho - use opposite factor for endho
    this->CalcOffsetSpanningStartY(dc, points[1].y, spanningType, factorStart);
    this->CalcOffsetSpanningEndY(dc, points[1].y, spanningType, abs(1.0 - factorStart));
    // Adjust the end (second) control point y
    this->CalcOffsetSpanningStartY(dc, points[2].y, spanningType, abs(1.0 - factorEnd));
    this->CalcOffsetSpanningEndY(dc, points[2].y, spanningType, factorEnd);
    // Adjsut the end point y
    this->CalcOffsetSpanningEndY(dc, points[3].y, spanningType);

    // Adjust the x
    this->CalcOffsetSpanningStartX(dc, points[0].x, spanningType);
    this->CalcOffsetSpanningStartX(dc, points[1].x, spanningType, factorStart);
    this->CalcOffsetSpanningEndX(dc, points[1].x, spanningType, abs(1.0 - factorStart));
    this->CalcOffsetSpanningStartX(dc, points[2].x, spanningType, abs(1.0 - factorEnd));
    this->CalcOffsetSpanningEndX(dc, points[2].x, spanningType, factorEnd);
    this->CalcOffsetSpanningEndX(dc, points[3].x, spanningType);

    // Adjust considering ho and vo
    if (spanningType == SPANNING_START_END) {
        for (int i = 0; i < 4; i++) this->CalcOffset(dc, points[i].x, points[i].y);
    }
    // Do not apply the horizontal offset for system start or end points
    else if (spanningType == SPANNING_START) {
        // Adjust end points (could be improved with a factor for the control point)
        for (int i = 2; i < 4; i++) this->CalcOffset(dc, points[i].x, points[i].y);
        // Vertical offset still does need to be applied
        this->CalcOffsetY(dc, points[0].y);
        this->CalcOffsetY(dc, points[1].y);
    }
    else if (spanningType == SPANNING_END) {
        // Adjust start points
        for (int i = 0; i < 2; i++) this->CalcOffset(dc, points[i].x, points[i].y);
        // Vertical offset for end points
        this->CalcOffsetY(dc, points[2].y);
        this->CalcOffsetY(dc, points[3].y);
    }
    // Middle, only vertical offset
    else {
        this->CalcOffsetY(dc, points[0].y);
        this->CalcOffsetY(dc, points[1].y);
        this->CalcOffsetY(dc, points[2].y);
        this->CalcOffsetY(dc, points[3].y);
    }
}

} // namespace vrv
