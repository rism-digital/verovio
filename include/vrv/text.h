/////////////////////////////////////////////////////////////////////////////
// Name:        text.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEXT_H__
#define __VRV_TEXT_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Text
//----------------------------------------------------------------------------

/**
 * This class models CDATA (text)
 */
class Text : public TextElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Text();
    virtual ~Text();
    Object *Clone() const override { return new Text(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Text"; }
    ///@}

    /**
     * @name Set and get the text content.
     * The text content is a std::u32string that needs to be converted to UTF16.
     * See MEIInput::ReadText and MEIInput ::WriteText
     */
    ///@{
    void SetText(const std::u32string &text) { m_text = text; }
    std::u32string GetText() const { return m_text; }
    ///@}

private:
    //
public:
    //
protected:
    /** The text content */
    std::u32string m_text;

private:
};

} // namespace vrv

#endif
