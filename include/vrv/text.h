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
// Rend
//----------------------------------------------------------------------------

/**
 * This class models the MEI <rend>
 */
class Rend : public TextElement, public AttCommon, public AttTypography {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Rend();
    virtual ~Rend();
    virtual void Reset();
    virtual std::string GetClassName() { return "Rend"; };
    ;
    virtual ClassId Is() { return REND; };
    ///@}

    /**
     * Add an element (text, rend. etc.) to a rend.
     * Only supported elements will be actually added to the child list.
     */
    void AddTextElement(TextElement *element);

private:
    //
public:
    //
private:
};

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
    virtual void Reset();
    virtual std::string GetClassName() { return "Text"; };
    ;
    virtual ClassId Is() { return TEXT; };
    ///@}

    /**
     * @name Set and get the text content.
     * The text content is a std::wstring that needs to be converted to UTF16.
     * See MeiInput::ReadText and MeiInput ::WriteText
     */
    ///@{
    void SetText(std::wstring text) { m_text = text; };
    std::wstring GetText() const { return m_text; };
    ///@}

private:
    //
public:
    //
protected:
    /** The text content */
    std::wstring m_text;

private:
};

} // namespace vrv

#endif
