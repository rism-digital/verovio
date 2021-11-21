/////////////////////////////////////////////////////////////////////////////
// Name:        barline.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DIVLINE_H__
#define __VRV_DIVLINE_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// DivLine
//----------------------------------------------------------------------------

/**
 * This class models the MEI <divLine> element.
 */
class DivLine : public LayerElement, public AttDivLineLog, public AttColor, public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    DivLine();
    virtual ~DivLine();
    virtual Object *Clone() const { return new DivLine(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "DivLine"; }
    virtual ClassId GetClassId() const { return DIVLINE; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Use to set the alignment for the Measure BarLine members.
     * This is as special case where we need to add to the measure aligner.
     */
    bool SetAlignment(Alignment *alignment);

    /*
     * Return true if the divLine type requires repetition dots to be drawn.
     */
    // bool HasRepetitionDots() const;

    /**
     * Retrieve SMuFL string for the divline.
     */
    std::wstring GetSymbolStr() const;

    /**
     * @name Method used for drawing divline on ornaments
     */
    static wchar_t GetDivLineGlyph(data_DIVLINE);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToCastOffMensural
     */
    // virtual int ConvertToCastOffMensural(FunctorParams *params);
    // static wchar_t GetDivLineGlyph(data_DIVISIO);
    
    /**
     * See Object::ResetDrawing
     */
    // virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    // virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// DivLineAttr
//----------------------------------------------------------------------------

/**
 * This class models the divLine related attributes of a MEI measure.
 */
class DivLineAttr : public DivLine {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * No Reset() method is required.
     */
    ///@{
    DivLineAttr();
    virtual ~DivLineAttr();
    virtual Object *Clone() const { return new DivLineAttr(*this); }
    virtual std::string GetClassName() const { return "DivLineAttr"; }
    virtual ClassId GetClassId() const { return DIVLINE; }
    ///@}

    // void SetLeft() { m_isLeft = true; }

private:
    //
public:
    //
private:
    /** A flag for left barlines (right if false) */
    // bool m_isLeft;
};

} // namespace vrv

#endif
