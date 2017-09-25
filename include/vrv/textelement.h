/////////////////////////////////////////////////////////////////////////////
// Name:        textelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEXT_ELEMENT_H__
#define __VRV_TEXT_ELEMENT_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextElement
//----------------------------------------------------------------------------

class TextElement : public Object, public AttTyped {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    TextElement();
    TextElement(std::string classid);
    virtual ~TextElement();
    virtual void Reset();
    virtual std::string GetClassName() const { return "TextElement"; }
    virtual ClassId GetClassId() const { return TEXT_ELEMENT; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// TextDrawingParams
//----------------------------------------------------------------------------
  
    
/**
 * This class stores current drawing parameters for text.
 */
class TextDrawingParams {
public:
    TextDrawingParams()
    {
        m_width = 0;
        m_setX = false;
        m_setY = false;
        m_x = 0;
        m_y = 0;
        m_alignment = HORIZONTALALIGNMENT_left;
    }
    virtual ~TextDrawingParams(){};

    int m_width;
    bool m_setX;
    bool m_setY;
    int m_x;
    int m_y;
    data_HORIZONTALALIGNMENT m_alignment;
};
    
} // namespace vrv

#endif
