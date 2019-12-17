/////////////////////////////////////////////////////////////////////////////
// Name:        rest.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REST_H__
#define __VRV_REST_H__

#include "atts_mensural.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rest
//----------------------------------------------------------------------------

/**
 * This class models the MEI <rest> element.
 */
class Rest : public LayerElement,
             public DurationInterface,
             public PositionInterface,
             public AttColor,
             public AttCue,
             public AttRestVisMensural {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rest();
    virtual ~Rest();
    virtual Object *Clone() const { return new Rest(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Rest"; }
    virtual ClassId GetClassId() const { return REST; }
    ///@}

    /**
     * Add an element to a rest.
     * Only Dots elements will be actually added to the rest.
     */
    virtual void AddChild(Object *object);

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PositionInterface *GetPositionInterface() { return dynamic_cast<PositionInterface *>(this); }
    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Get the SMuFL glyph or a rest considering its actual duration.
     * This is valid only for CMN and for duration shorter than half notes.
     */
    wchar_t GetRestGlyph() const;

    /**
     * Get the vertical offset for each glyph.
     */
    int GetRestLocOffset(int loc);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertAnalyticalMarkup
     */
    virtual int ConvertAnalyticalMarkup(FunctorParams *functorParams);

    /**
     * See Object::CalcDots
     */
    virtual int CalcDots(FunctorParams *functorParams);

    /**
     * See Object::PrepareLayerElementParts
     */
    virtual int PrepareLayerElementParts(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
