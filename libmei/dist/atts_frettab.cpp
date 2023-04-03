/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// NOTE: this file was generated with the Verovio libmei version and
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#include "atts_frettab.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttCourseLog
//----------------------------------------------------------------------------

AttCourseLog::AttCourseLog() : Att()
{
    ResetCourseLog();
}

void AttCourseLog::ResetCourseLog()
{
    m_tuningStandard = COURSETUNING_NONE;
}

bool AttCourseLog::ReadCourseLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tuning.standard")) {
        this->SetTuningStandard(StrToCoursetuning(element.attribute("tuning.standard").value()));
        if (removeAttr) element.remove_attribute("tuning.standard");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCourseLog::WriteCourseLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTuningStandard()) {
        element.append_attribute("tuning.standard") = CoursetuningToStr(this->GetTuningStandard()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCourseLog::HasTuningStandard() const
{
    return (m_tuningStandard != COURSETUNING_NONE);
}

//----------------------------------------------------------------------------
// AttNoteGesTab
//----------------------------------------------------------------------------

AttNoteGesTab::AttNoteGesTab() : Att()
{
    ResetNoteGesTab();
}

void AttNoteGesTab::ResetNoteGesTab()
{
    m_tabCourse = MEI_UNSET;
    m_tabFret = -1;
}

bool AttNoteGesTab::ReadNoteGesTab(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tab.course")) {
        this->SetTabCourse(StrToInt(element.attribute("tab.course").value()));
        if (removeAttr) element.remove_attribute("tab.course");
        hasAttribute = true;
    }
    if (element.attribute("tab.fret")) {
        this->SetTabFret(StrToInt(element.attribute("tab.fret").value()));
        if (removeAttr) element.remove_attribute("tab.fret");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteGesTab::WriteNoteGesTab(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTabCourse()) {
        element.append_attribute("tab.course") = IntToStr(this->GetTabCourse()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTabFret()) {
        element.append_attribute("tab.fret") = IntToStr(this->GetTabFret()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteGesTab::HasTabCourse() const
{
    return (m_tabCourse != MEI_UNSET);
}

bool AttNoteGesTab::HasTabFret() const
{
    return (m_tabFret != -1);
}

} // namespace vrv
