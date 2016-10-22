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

#ifndef __VRV_ATTS_FIGTABLE_H__
#define __VRV_ATTS_FIGTABLE_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttTabular
//----------------------------------------------------------------------------

class AttTabular : public Att {
public:
    AttTabular();
    virtual ~AttTabular();

    /** Reset the default values for the attribute class **/
    void ResetTabular();

    /** Read the values for the attribute class **/
    bool ReadTabular(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTabular(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetColspan(int colspan_) { m_colspan = colspan_; }
    int GetColspan() const { return m_colspan; }
    bool HasColspan() const;
    //
    void SetRowspan(int rowspan_) { m_rowspan = rowspan_; }
    int GetRowspan() const { return m_rowspan; }
    bool HasRowspan() const;
    ///@}

private:
    /** The number of columns spanned by this cell. **/
    int m_colspan;
    /** The number of rows spanned by this cell. **/
    int m_rowspan;

    /* include <attrowspan> */
};

} // vrv namespace

#endif // __VRV_ATTS_FIGTABLE_H__
