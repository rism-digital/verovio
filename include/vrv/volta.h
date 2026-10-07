/////////////////////////////////////////////////////////////////////////////
// Name:        volta.h
// Author:      Simon Waloschek
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_VOLTA_H__
#define __VRV_VOLTA_H__

#include "atts_shared.h"
#include "layerelement.h"
#include "offsetinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Volta
//----------------------------------------------------------------------------

/** Sung text for a specific iteration of a repeated section of music. */
class Volta : public LayerElement,
              public OffsetInterface,
              public AttColor,
              public AttLang,
              public AttNNumberLike,
              public AttTypography {
public:
    Volta();
    virtual ~Volta();
    Object *Clone() const override { return new Volta(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "volta"; }

    /** Add an element (a syl) to a volta. */
    bool IsSupportedChild(ClassId classId) override;

    OffsetInterface *GetOffsetInterface() override { return vrv_cast<OffsetInterface *>(this); }
    const OffsetInterface *GetOffsetInterface() const override { return vrv_cast<const OffsetInterface *>(this); }

    /** The normalized one-based alternative index used only for drawing and lyric processing. */
    int GetDrawingVoltaN() const { return m_drawingVoltaN; }
    void SetDrawingVoltaN(int drawingVoltaN) const
    {
        m_drawingVoltaN = drawingVoltaN;
        m_drawingVoltaNSet = true;
    }
    bool HasDrawingVoltaN() const { return m_drawingVoltaNSet; }
    void ResetDrawingVoltaN()
    {
        m_drawingVoltaN = 1;
        m_drawingVoltaNSet = false;
    }

    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;

private:
    mutable int m_drawingVoltaN;
    mutable bool m_drawingVoltaNSet;
};

} // namespace vrv

#endif
