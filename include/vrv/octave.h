/////////////////////////////////////////////////////////////////////////////
// Name:        octave.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OCTAVE_H__
#define __VRV_OCTAVE_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Octave
//----------------------------------------------------------------------------

/**
 * This class models the MEI <octave> element.
 */
class Octave : public ControlElement,
               public TimeSpanningInterface,
               public AttExtender,
               public AttLineRend,
               public AttLineRendBase,
               public AttNNumberLike,
               public AttOctaveDisplacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Octave();
    virtual ~Octave();
    Object *Clone() const override { return new Octave(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "octave"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return vrv_cast<TimeSpanningInterface *>(this); }
    const TimeSpanningInterface *GetTimeSpanningInterface() const override
    {
        return vrv_cast<const TimeSpanningInterface *>(this);
    }
    ///@}

    /**
     * Store the horizontal extender line coordinates
     */
    ///@{
    void ResetDrawingExtenderX();
    void SetDrawingExtenderX(int left, int right);
    ///@}

    /**
     * Get the SMuFL glyph.
     */
    char32_t GetOctaveGlyph(bool withAltaBassa) const;

    /**
     * Calculate the octave line width.
     */
    int GetLineWidth(const Doc *doc, int unit) const;

    /**
     * Determine the vertical content boundary.
     * For refined layout this can take the overlapping bbox into account.
     * Returns a pair consisting of the boundary (relative to the object position)
     * and a flag indicating whether refined layout was used.
     */
    std::pair<int, bool> GetVerticalContentBoundaryRel(const Doc *doc, const FloatingPositioner *positioner,
        const BoundingBox *horizOverlappingBBox, bool contentTop) const override;

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

protected:
    //
private:
    //
public:
    //
private:
    /**
     * The left and right X coordinates of the drawn horizontal extender line
     */
    std::map<const FloatingPositioner *, std::pair<int, int>> m_drawingExtenderX;
};

} // namespace vrv

#endif
