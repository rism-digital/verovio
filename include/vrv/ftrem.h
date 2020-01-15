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
class FTrem : public LayerElement, public ObjectListInterface, public AttFTremVis, public AttTremMeasured {
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
    virtual void AddChild(Object *object);

    /**
     * Initializes the m_beamElementCoords vector objects.
     * This is called by FTrem::FilterList
     */
    void InitCoords(ArrayOfObjects *childList);

    /**
     * Clear the m_beamElementCoords vector and delete all the objects.
     */
    void ClearCoords();

    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoords() const { return &m_beamElementCoords; }

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

private:
    //
protected:
    /**
     * Filter the flat list and keep only Note or Chords elements.
     */
    virtual void FilterList(ArrayOfObjects *childList);

public:
    /** */
    BeamDrawingParams m_drawingParams;

private:
    /**
     * An array of coordinates for each element
     **/
    mutable ArrayOfBeamElementCoords m_beamElementCoords;
};

} // namespace vrv

#endif
