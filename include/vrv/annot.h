/////////////////////////////////////////////////////////////////////////////
// Name:        annot.h
// Author:      Klaus Rettinghaus
// Created:     2018/03/28
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ANNOT_H__
#define __VRV_ANNOT_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Annot
//----------------------------------------------------------------------------

/**
 * This class models the MEI <annot> element.
 */
class Annot : public EditorialElement, public TextListInterface, public AttPlist, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Annot();
    virtual ~Annot();
    // This fails because of the copy contructor in ObjectListInterface (TextListInterface parent)
    // Object *Clone() const override { return new Annot(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "annot"; }
    ///@}

    /**
     * Add a text element to an annotation.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(ClassId classId) override;

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    /**
     * A copy of the annot content tree stored as pugi::xml_document
     */
    pugi::xml_document m_content;

private:
};

} // namespace vrv

#endif
