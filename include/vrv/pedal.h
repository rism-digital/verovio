/////////////////////////////////////////////////////////////////////////////
// Name:        pedal.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PEDAL_H__
#define __VRV_PEDAL_H__

#include "atts_cmn.h"
#include "atts_externalsymbols.h"
#include "atts_visual.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

class System;
//----------------------------------------------------------------------------
// Pedal
//----------------------------------------------------------------------------

/**
 * This class models the MEI <pedal> element.
 */
class Pedal : public ControlElement,
              public TimeSpanningInterface,
              public AttExtSymAuth,
              public AttExtSymNames,
              public AttPedalLog,
              public AttPedalVis,
              public AttPlacementRelStaff,
              public AttVerticalGroup {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Pedal();
    virtual ~Pedal();
    Object *Clone() const override { return new Pedal(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "pedal"; }
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
    ////@}

    /**
     * @name Setter and getter of the bounce flag
     */
    ///@{
    bool EndsWithBounce() const { return m_endsWithBounce; }
    void EndsWithBounce(bool endsWithBounce) { m_endsWithBounce = endsWithBounce; }
    ///@}

    /**
     * Get the SMuFL glyph for the pedal based on function or glyph.num
     */
    char32_t GetPedalGlyph() const;

    /**
     * Get the pedal form based on the options and corresponding attributes from <pedal> and <scoreDef>
     */
    data_PEDALSTYLE GetPedalForm(const Doc *doc, const System *system) const;

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
    /**
     * Flag indicating if following pedal mark is a bounce
     */
    bool m_endsWithBounce;
};

} // namespace vrv

#endif
