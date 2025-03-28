/////////////////////////////////////////////////////////////////////////////
// Name:        ftrem.h
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FTREM_H__
#define __VRV_FTREM_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "beam.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// FTrem (fingered tremolo)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fTrem> element.
 */
class FTrem : public LayerElement, public BeamDrawingInterface, public AttFTremVis, public AttTremMeasured {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    FTrem();
    virtual ~FTrem();
    Object *Clone() const override { return new FTrem(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "fTrem"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    BeamDrawingInterface *GetBeamDrawingInterface() override { return vrv_cast<BeamDrawingInterface *>(this); }
    const BeamDrawingInterface *GetBeamDrawingInterface() const override
    {
        return vrv_cast<const BeamDrawingInterface *>(this);
    }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoords();

    /**
     * See DrawingInterface::GetAdditionalBeamCount
     */
    std::pair<int, int> GetAdditionalBeamCount() const override;

    /**
     * See DrawingInterface::GetFloatingBeamCount
     */
    std::pair<int, int> GetFloatingBeamCount() const override;

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
protected:
    /**
     * Filter the flat list and keep only Note or Chords elements.
     */
    void FilterList(ListOfConstObjects &childList) const override;

public:
    /** */
    BeamSegment m_beamSegment;

private:
};

} // namespace vrv

#endif
