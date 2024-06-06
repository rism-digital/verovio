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
    m_pageIdx = 0;
    m_slurHandling = SlurHandling::Initialize;

    m_currentColor = AxNONE;
    m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
    m_currentStaff = NULL;
    m_currentSystem = NULL;
}

View::~View() {}

void View::SetDoc(Doc *doc)
{
    // Unset the doc
    if (doc == NULL) {
        m_doc = NULL;
        m_options = NULL;
        DoReset();
    }
    else {
        m_doc = doc;
        m_options = doc->GetOptions();
    }
    m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
    m_currentStaff = NULL;
    m_currentSystem = NULL;
    m_currentPage = NULL;
    m_pageIdx = 0;
}

void View::SetPage(int pageIdx, bool doLayout)
{
    assert(m_doc); // Page cannot be NULL
    assert(m_doc->HasPage(pageIdx));

    m_pageIdx = pageIdx;
    m_currentPage = m_doc->SetDrawingPage(pageIdx);

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

    m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
    m_currentStaff = NULL;
    m_currentSystem = NULL;

    OnPageChange();
    DoRefresh();
}

bool View::HasNext(bool forward)
{
    if (forward) return (m_doc && (m_doc->HasPage(m_pageIdx + 1)));
    return (m_doc && (m_doc->HasPage(m_pageIdx - 1)));
}

void View::Next(bool forward)
{
    if (!m_doc) return;

    if (forward && this->HasNext(true)) {
        m_pageIdx++;
    }
    else if (!forward && this->HasNext(false)) {
        m_pageIdx--;
    }
    this->SetPage(m_pageIdx);
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
    return Point(ToDeviceContextX(p.x), ToDeviceContextY(p.y));
}

Point View::ToLogical(Point p)
{
    return Point(ToLogicalX(p.x), ToLogicalY(p.y));
}

std::u32string View::IntToTupletFigures(unsigned short number)
{
    return IntToSmuflFigures(number, 0xE880);
}

std::u32string View::IntToTimeSigFigures(unsigned short number)
{
    return IntToSmuflFigures(number, 0xE080);
}

std::u32string View::IntToProlationFigures(unsigned short number)
{
    std::ostringstream stream;
    stream << number;
    std::u32string str = UTF8to32(stream.str());

    for (char32_t &c : str) {
        switch (c) {
            case 48: c = SMUFL_E080_timeSig0; break; // No 0 in SMuFL, use CMW one...
            case 49: c = SMUFL_E926_mensuralProportion1; break;
            case 50: c = SMUFL_E927_mensuralProportion2; break;
            case 51: c = SMUFL_E928_mensuralProportion3; break;
            case 52: c = SMUFL_E929_mensuralProportion4; break;
            case 53: c = SMUFL_EE90_mensuralProportion5; break;
            case 54: c = SMUFL_EE91_mensuralProportion6; break;
            case 55: c = SMUFL_EE92_mensuralProportion7; break;
            case 56: c = SMUFL_EE93_mensuralProportion8; break;
            case 57: c = SMUFL_EE94_mensuralProportion9; break;
            default: c = SMUFL_E926_mensuralProportion1;
        }
    }
    return str;
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

} // namespace vrv
