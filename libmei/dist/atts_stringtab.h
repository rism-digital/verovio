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

#ifndef __LIBMEI_ATTS_STRINGTAB_H__
#define __LIBMEI_ATTS_STRINGTAB_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttStaffDefVisTablature
//----------------------------------------------------------------------------

class AttStaffDefVisTablature : public Att {
protected:
    AttStaffDefVisTablature();
    ~AttStaffDefVisTablature() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetStaffDefVisTablature();

    /** Read the values for the attribute class **/
    bool ReadStaffDefVisTablature(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteStaffDefVisTablature(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTabAlign(data_VERTICALALIGNMENT tabAlign_) { m_tabAlign = tabAlign_; }
    data_VERTICALALIGNMENT GetTabAlign() const { return m_tabAlign; }
    bool HasTabAlign() const;
    //
    void SetTabAnchorline(char tabAnchorline_) { m_tabAnchorline = tabAnchorline_; }
    char GetTabAnchorline() const { return m_tabAnchorline; }
    bool HasTabAnchorline() const;
    ///@}

private:
    /**
     * Attribute that describes the vertical alignment of tablature symbols.
     * Only applicable in cases where the symbols' vertical position does not
     * communicate other information, such as courses (i.e., only in German lute
     * tablature). Typical values are top and bottom.
     **/
    data_VERTICALALIGNMENT m_tabAlign;
    /**
     * Used in German lute tablature where the vertical alignment of tab notes is
     * consistent but cannot be identified using a typical value of tab.align (i.e.,
     * top or bottom).
     * Specifies the horizontal strand corresponding to the lines attribute on staffDef
     * that anchors the vertical position of tab notes. This anchorline is used as the
     * vertical starting position when stacking tab notes into chords. Single tab notes
     * simply occupy this position. Chords grow upwards from this position. If the
     * chord extends further than the number of available horizontal strands (lines)
     * above the anchorline, the entire chord is shifted downward until its top tab
     * note is positioned on the top-most line. (Note that in German lute tablature,
     * the lines are conceptual rather than visible).
     **/
    char m_tabAnchorline;
};

//----------------------------------------------------------------------------
// InstStaffDefVisTablature
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttStaffDefVisTablature
 */

class InstStaffDefVisTablature : public AttStaffDefVisTablature {
public:
    InstStaffDefVisTablature() = default;
    virtual ~InstStaffDefVisTablature() = default;
};

//----------------------------------------------------------------------------
// AttStringtab
//----------------------------------------------------------------------------

class AttStringtab : public Att {
protected:
    AttStringtab();
    ~AttStringtab() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetStringtab();

    /** Read the values for the attribute class **/
    bool ReadStringtab(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteStringtab(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTabFing(std::string tabFing_) { m_tabFing = tabFing_; }
    std::string GetTabFing() const { return m_tabFing; }
    bool HasTabFing() const;
    //
    void SetTabFret(int tabFret_) { m_tabFret = tabFret_; }
    int GetTabFret() const { return m_tabFret; }
    bool HasTabFret() const;
    //
    void SetTabLine(char tabLine_) { m_tabLine = tabLine_; }
    char GetTabLine() const { return m_tabLine; }
    bool HasTabLine() const;
    //
    void SetTabString(std::string tabString_) { m_tabString = tabString_; }
    std::string GetTabString() const { return m_tabString; }
    bool HasTabString() const;
    //
    void SetTabCourse(int tabCourse_) { m_tabCourse = tabCourse_; }
    int GetTabCourse() const { return m_tabCourse; }
    bool HasTabCourse() const;
    ///@}

private:
    /**
     * This attribute is deprecated and will be removed in a future version.
     * Indicates which finger, if any, should be used to play an individual string. The
     * index, middle, ring, and little fingers are represented by the values 1-4, while
     * t is for the thumb. The values x and o indicate muffled and open strings,
     * respectively.
     **/
    std::string m_tabFing;
    /** Records the location at which a string should be stopped against a fret. **/
    int m_tabFret;
    /**
     * Used in German lute tablature in cases where vertical positioning deviates from
     * the norm which can be specified by tab.align.
     * Indicates the position of the tab note on one of the horizontal strands
     * corresponding to the lines attribute on staffDef. (Note that in this case, the
     * lines are conceptual rather than visible).
     **/
    char m_tabLine;
    /**
     * This attribute is deprecated in favor of tab.course and will be removed in a
     * future version.
     * Records which string is to be played.
     **/
    std::string m_tabString;
    /** Records which course is to be played. **/
    int m_tabCourse;
};

//----------------------------------------------------------------------------
// InstStringtab
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttStringtab
 */

class InstStringtab : public AttStringtab {
public:
    InstStringtab() = default;
    virtual ~InstStringtab() = default;
};

//----------------------------------------------------------------------------
// AttStringtabPosition
//----------------------------------------------------------------------------

class AttStringtabPosition : public Att {
protected:
    AttStringtabPosition();
    ~AttStringtabPosition() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetStringtabPosition();

    /** Read the values for the attribute class **/
    bool ReadStringtabPosition(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteStringtabPosition(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTabPos(int tabPos_) { m_tabPos = tabPos_; }
    int GetTabPos() const { return m_tabPos; }
    bool HasTabPos() const;
    ///@}

private:
    /** Records fret position. **/
    int m_tabPos;
};

//----------------------------------------------------------------------------
// InstStringtabPosition
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttStringtabPosition
 */

class InstStringtabPosition : public AttStringtabPosition {
public:
    InstStringtabPosition() = default;
    virtual ~InstStringtabPosition() = default;
};

//----------------------------------------------------------------------------
// AttStringtabTuning
//----------------------------------------------------------------------------

class AttStringtabTuning : public Att {
protected:
    AttStringtabTuning();
    ~AttStringtabTuning() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetStringtabTuning();

    /** Read the values for the attribute class **/
    bool ReadStringtabTuning(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteStringtabTuning(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTabStrings(std::string tabStrings_) { m_tabStrings = tabStrings_; }
    std::string GetTabStrings() const { return m_tabStrings; }
    bool HasTabStrings() const;
    //
    void SetTabCourses(std::string tabCourses_) { m_tabCourses = tabCourses_; }
    std::string GetTabCourses() const { return m_tabCourses; }
    bool HasTabCourses() const;
    ///@}

private:
    /**
     * This attribute is deprecated in favor of the new tuning element and will be
     * removed in a future version.
     * Provides a *written* pitch and octave for each open string or course of strings.
     **/
    std::string m_tabStrings;
    /**
     * This attribute is deprecated in favor of the new tuning element and will be
     * removed in a future version.
     * Provides a *written* pitch and octave for each open string or course of strings.
     **/
    std::string m_tabCourses;
};

//----------------------------------------------------------------------------
// InstStringtabTuning
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttStringtabTuning
 */

class InstStringtabTuning : public AttStringtabTuning {
public:
    InstStringtabTuning() = default;
    virtual ~InstStringtabTuning() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_STRINGTAB_H__
