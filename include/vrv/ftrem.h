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
    std::string GetClassName() const override { return "FTrem"; }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoords();

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

    /**
     * See Object::ResetDrawing
     */
    int ResetDrawing(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateMIDI
     */
    int GenerateMIDI(FunctorParams *functorParams) override;

private:
    //
protected:
    /**
     * Filter the flat list and keep only Note or Chords elements.
     */
    virtual void FilterList(ArrayOfObjects *childList);

public:
    /** */
    BeamSegment m_beamSegment;

private:
};

} // namespace vrv

#endif
