/////////////////////////////////////////////////////////////////////////////
// Name:        view.cpp
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
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
    m_tieThicknessCoeficient = 0.0;
    m_slurThicknessCoeficient = 0.0;

    m_currentColour = AxNONE;
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
        m_doc->SetCurrentScoreDefDoc();
        // if we once deal with multiple views, it would be better
        // to redo the layout only when necessary?
        if (m_doc->GetType() == Transcription || m_doc->GetType() == Facs)
            m_currentPage->LayOutTranscription();
        else
            m_currentPage->LayOut();
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
    SetPage(m_pageIdx);
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

std::wstring View::IntToTupletFigures(unsigned short number)
{
    return IntToSmuflFigures(number, 0xE880);
}

std::wstring View::IntToTimeSigFigures(unsigned short number)
{
    return IntToSmuflFigures(number, 0xE080);
}

std::wstring View::IntToSmuflFigures(unsigned short number, int offset)
{
    std::wostringstream stream;
    stream << number;
    std::wstring str = stream.str();

    int i;
    for (i = 0; i < (int)str.size(); ++i) {
        str[i] += offset - 48;
    }
    return str;
}

} // namespace vrv
