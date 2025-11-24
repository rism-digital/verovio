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

void View::StartOffset(DeviceContext *dc, Object *object, int staffSize)
{
    if (!dc->ApplyOffset()) return;

    if (!object->HasInterface(INTERFACE_OFFSET)) return;

    const OffsetInterface *interface = object->GetOffsetInterface();
    assert(interface);

    const int unit = m_doc->GetOptions()->m_unit.GetValue();

    if (!interface->HasHo() && !interface->HasVo()) return;
    Offset offset;
    offset.m_ho = interface->GetHo().GetVu() * unit * staffSize / 100;
    offset.m_vo = interface->GetVo().GetVu() * unit * staffSize / 100;
    offset.m_object = object;

    m_currentOffsets.push_front(offset);
}

void View::EndOffset(DeviceContext *dc, Object *object)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    if (m_currentOffsets.front().m_object == object) m_currentOffsets.pop_front();
}

void View::CalcOffset(DeviceContext *dc, int &x, int &y)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        x = x + offset.m_ho;
        y = y + offset.m_vo;
    }
}

void View::CalcOffsetX(DeviceContext *dc, int &x)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        x = x + offset.m_ho;
    }
}

void View::CalcOffsetY(DeviceContext *dc, int &y)
{
    if (!dc->ApplyOffset() || m_currentOffsets.empty()) return;

    for (Offset &offset : m_currentOffsets) {
        y = y + offset.m_vo;
    }
}

} // namespace vrv
