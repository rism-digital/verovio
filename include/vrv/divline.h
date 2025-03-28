/////////////////////////////////////////////////////////////////////////////
// Name:        divline.h
// Author:      Yinan Zhou
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DIVLINE_H__
#define __VRV_DIVLINE_H__

#include "atts_externalsymbols.h"
#include "atts_neumes.h"
#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// DivLine
//----------------------------------------------------------------------------

/**
 * This class models the MEI <divLine> element.
 */
class DivLine : public LayerElement,
                public AttColor,
                public AttDivLineLog,
                public AttExtSymAuth,
                public AttExtSymNames,
                public AttNNumberLike,
                public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    DivLine();
    virtual ~DivLine();
    Object *Clone() const override { return new DivLine(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "divLine"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Use to set the alignment for the Measure BarLine members.
     * This is as special case where we need to add to the measure aligner.
     */
    bool SetAlignment(Alignment *alignment);

    /**
     * Retrieve SMuFL string for the divLine.
     */
    std::wstring GetSymbolStr() const;

    /**
     * @name Method used for drawing divLine on ornaments
     */
    static wchar_t GetDivLineGlyph(divLineLog_FORM);

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
    Object *Clone() const override { return new DivLineAttr(*this); }
    std::string GetClassName() const override { return "divLineAttr"; }
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
