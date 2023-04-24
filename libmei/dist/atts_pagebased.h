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

#ifndef __LIBMEI_ATTS_PAGEBASED_H__
#define __LIBMEI_ATTS_PAGEBASED_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttMargins
//----------------------------------------------------------------------------

class AttMargins : public Att {
protected:
    AttMargins();
    ~AttMargins() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetMargins();

    /** Read the values for the attribute class **/
    bool ReadMargins(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteMargins(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTopmar(data_MEASUREMENTUNSIGNED topmar_) { m_topmar = topmar_; }
    data_MEASUREMENTUNSIGNED GetTopmar() const { return m_topmar; }
    bool HasTopmar() const;
    //
    void SetBotmar(data_MEASUREMENTUNSIGNED botmar_) { m_botmar = botmar_; }
    data_MEASUREMENTUNSIGNED GetBotmar() const { return m_botmar; }
    bool HasBotmar() const;
    //
    void SetLeftmar(data_MEASUREMENTUNSIGNED leftmar_) { m_leftmar = leftmar_; }
    data_MEASUREMENTUNSIGNED GetLeftmar() const { return m_leftmar; }
    bool HasLeftmar() const;
    //
    void SetRightmar(data_MEASUREMENTUNSIGNED rightmar_) { m_rightmar = rightmar_; }
    data_MEASUREMENTUNSIGNED GetRightmar() const { return m_rightmar; }
    bool HasRightmar() const;
    ///@}

private:
    /** Indicates the amount of whitespace at the top of a page. **/
    data_MEASUREMENTUNSIGNED m_topmar;
    /** Indicates the amount of whitespace at the bottom of a page. **/
    data_MEASUREMENTUNSIGNED m_botmar;
    /** Indicates the amount of whitespace at the left side of a page. **/
    data_MEASUREMENTUNSIGNED m_leftmar;
    /** Indicates the amount of whitespace at the right side of a page. **/
    data_MEASUREMENTUNSIGNED m_rightmar;
};

//----------------------------------------------------------------------------
// InstMargins
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttMargins
 */

class InstMargins : public AttMargins {
public:
    InstMargins() = default;
    virtual ~InstMargins() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_PAGEBASED_H__
