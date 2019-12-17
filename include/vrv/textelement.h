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

class TextElement : public Object, public AttLabelled, public AttTyped {
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

    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;
    ///@}

    /**
     * @name Get and set the X and Y drawing relative positions
     */
    ///@{
    int GetDrawingXRel() const { return m_drawingXRel; }
    virtual void SetDrawingXRel(int drawingXRel);
    int GetDrawingYRel() const { return m_drawingYRel; }
    virtual void SetDrawingYRel(int drawingYRel);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    /**
     * The Y drawing relative position of the object.
     */
    int m_drawingYRel;

    /**
     * The X drawing relative position of the object.
     */
    int m_drawingXRel;
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
        m_x = 0;
        m_y = 0;
        m_width = 0;
        m_laidOut = false;
        m_newLine = false;
        m_verticalShift = false;
        m_alignment = HORIZONTALALIGNMENT_left;
        m_pointSize = 0;
    }
    virtual ~TextDrawingParams(){};

    int m_x;
    int m_y;
    int m_width;
    bool m_laidOut;
    bool m_newLine;
    bool m_verticalShift;
    data_HORIZONTALALIGNMENT m_alignment;
    int m_pointSize;
};

} // namespace vrv

#endif
