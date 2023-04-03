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

#ifndef __LIBMEI_ATTS_FRETTAB_H__
#define __LIBMEI_ATTS_FRETTAB_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttCourseLog
//----------------------------------------------------------------------------

class AttCourseLog : public Att {
protected:
    AttCourseLog();
    ~AttCourseLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetCourseLog();

    /** Read the values for the attribute class **/
    bool ReadCourseLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteCourseLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTuningStandard(data_COURSETUNING tuningStandard_) { m_tuningStandard = tuningStandard_; }
    data_COURSETUNING GetTuningStandard() const { return m_tuningStandard; }
    bool HasTuningStandard() const;
    ///@}

private:
    /** Indicates a tuning standard for the course. **/
    data_COURSETUNING m_tuningStandard;
};

//----------------------------------------------------------------------------
// InstCourseLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttCourseLog
 */

class InstCourseLog : public AttCourseLog {
public:
    InstCourseLog() = default;
    virtual ~InstCourseLog() = default;
};

//----------------------------------------------------------------------------
// AttNoteGesTab
//----------------------------------------------------------------------------

class AttNoteGesTab : public Att {
protected:
    AttNoteGesTab();
    ~AttNoteGesTab() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetNoteGesTab();

    /** Read the values for the attribute class **/
    bool ReadNoteGesTab(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteNoteGesTab(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTabCourse(int tabCourse_) { m_tabCourse = tabCourse_; }
    int GetTabCourse() const { return m_tabCourse; }
    bool HasTabCourse() const;
    //
    void SetTabFret(int tabFret_) { m_tabFret = tabFret_; }
    int GetTabFret() const { return m_tabFret; }
    bool HasTabFret() const;
    ///@}

private:
    /** Records which course is to be played. **/
    int m_tabCourse;
    /** Records which course is to be played. **/
    int m_tabFret;
};

//----------------------------------------------------------------------------
// InstNoteGesTab
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttNoteGesTab
 */

class InstNoteGesTab : public AttNoteGesTab {
public:
    InstNoteGesTab() = default;
    virtual ~InstNoteGesTab() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_FRETTAB_H__
