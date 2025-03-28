/////////////////////////////////////////////////////////////////////////////
// Name:        div.h
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DIV_H__
#define __VRV_DIV_H__

#include "textlayoutelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Div
//----------------------------------------------------------------------------

/**
 * This class represents an MEI Div.
 * The current implementation accepts rend as child of div, which is not valid
 * See https://github.com/music-encoding/music-encoding/issues/1189
 */
class Div : public TextLayoutElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Div();
    virtual ~Div();
    void Reset() override;
    std::string GetClassName() const override { return "div"; }
    ///@}

    /**
     * @name Get and set the inline drawing flag
     */
    ///@{
    bool GetDrawingInline() const { return m_drawingInline; }
    void SetDrawingInline(bool drawingInline) { m_drawingInline = drawingInline; }
    ///@}

    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    int GetDrawingX() const override;
    int GetDrawingY() const override;
    ///@}

    /**
     * @name Get and set the X and Y drawing relative positions
     */
    ///@{
    int GetDrawingXRel() const { return m_drawingXRel; }
    virtual void SetDrawingXRel(int drawingXRel);
    void CacheXRel(bool restore = false);
    int GetDrawingYRel() const { return m_drawingYRel; }
    virtual void SetDrawingYRel(int drawingYRel);
    void CacheYRel(bool restore = false);
    ///@}

    /**
     * Overriden to get the appropriate margin
     */
    int GetTotalHeight(const Doc *doc) const override;

    int GetTotalWidth(const Doc *doc) const override;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
    //
private:
    /**
     * The Y drawing relative position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingYRel;

    /**
     * The X drawing relative position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingXRel;

    /**
     * A flag indicating that the div should be displayed inline
     */
    bool m_drawingInline;
};

} // namespace vrv

#endif
