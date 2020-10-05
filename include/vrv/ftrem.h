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
class FTrem : public LayerElement,
              public ObjectListInterface,
              public BeamDrawingInterface,
              public AttFTremVis,
              public AttTremMeasured {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    FTrem();
    virtual ~FTrem();
    virtual Object *Clone() const { return new FTrem(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "FTrem"; }
    virtual ClassId GetClassId() const { return FTREM; }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    virtual bool IsSupportedChild(Object *object);

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
    virtual int CalcStem(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::GenerateMIDI
     */
    virtual int GenerateMIDI(FunctorParams *functorParams);

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
