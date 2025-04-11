/////////////////////////////////////////////////////////////////////////////
// Name:        nc.h
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NC_H__
#define __VRV_NC_H__

#include <cassert>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_neumes.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "pitchinterface.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Nc
//----------------------------------------------------------------------------

/**
 * This class models the MEI <nc> element.
 */

class Nc : public LayerElement,
           public DurationInterface,
           public PitchInterface,
           public PositionInterface,
           public AttColor,
           public AttCurvatureDirection,
           public AttIntervalMelodic,
           public AttNcForm {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Nc();
    virtual ~Nc();
    Object *Clone() const override { return new Nc(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "nc"; }
    ///@}

    bool IsSupportedChild(ClassId classId) override;

    /**
     * @name Getter to interfaces
     */
    ///@{
    DurationInterface *GetDurationInterface() override { return vrv_cast<DurationInterface *>(this); }
    const DurationInterface *GetDurationInterface() const override { return vrv_cast<const DurationInterface *>(this); }
    PitchInterface *GetPitchInterface() override { return vrv_cast<PitchInterface *>(this); }
    const PitchInterface *GetPitchInterface() const override { return vrv_cast<const PitchInterface *>(this); }
    PositionInterface *GetPositionInterface() override { return vrv_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override { return vrv_cast<const PositionInterface *>(this); }
    ///@}

    /**
     * Calclulate the pitch or loc difference between to nc.
     * The Pitch difference takes precedence over the loc difference.
     */
    int PitchOrLocDifferenceTo(const Nc *nc) const;

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

    /**
     * A Structure holding a glyph paramter for the nc.
     * One single nc might need more than one glyph (e.g., liquescent).
     * Set in CalcLigatureOrNeumePosFunctor::VisitNeume
     */
    struct DrawingGlyph {
        wchar_t m_fontNo = 0;
        float m_xOffset = 0.0;
        float m_yOffset = 0.0;
    };

private:
    //
public:
    /** Drawing glyphs */
    std::vector<DrawingGlyph> m_drawingGlyphs;

private:
    //
};

} // namespace vrv

#endif
