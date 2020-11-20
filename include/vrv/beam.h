/////////////////////////////////////////////////////////////////////////////
// Name:        beam.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BEAM_H__
#define __VRV_BEAM_H__

#include "atts_cmn.h"
#include "drawinginterface.h"
#include "layerelement.h"

namespace vrv {

class BeamElementCoord;

// the maximum allowed number of partials
#define MAX_DURATION_PARTIALS 16

enum { PARTIAL_NONE = 0, PARTIAL_THROUGH, PARTIAL_RIGHT, PARTIAL_LEFT };

//----------------------------------------------------------------------------
// BeamSegment
//----------------------------------------------------------------------------

/**
 * Class for storing drawing parameters when calculating beams.
 * See View::DrawBeam and View::CalcBeam
 */

class BeamSegment {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    BeamSegment();
    virtual ~BeamSegment();

    void Reset();

    void CalcBeam(Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface,
        data_BEAMPLACE place = BEAMPLACE_NONE, bool init = true);

    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoordRefs();

    /**
     * Initializes the m_beamElementCoords vector objects.
     * This is called by Beam::FilterList
     */
    void InitCoordRefs(const ArrayOfBeamElementCoords *beamElementCoords);

    /**
     * Clear the m_beamElementCoords vector and delete all the objects.
     */
    void ClearCoordRefs();

private:
    void CalcBeamInit(Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, data_BEAMPLACE place);

    bool CalcBeamSlope(
        Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, bool &shorten, int &step);

    void CalcBeamPosition(Doc *doc, Staff *staff, Layer *layer, BeamDrawingInterface *beamInterface, bool isHorizontal);

    void CalcAdjustSlope(Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, bool shorten, int &step);

    void CalcStemLenInHalfUnitsgth(Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface);

    void CalcBeamPlace(Layer *layer, BeamDrawingInterface *beamInterface, data_BEAMPLACE place);

    // Helper to calculate the longest stem length of the beam (which will be used uniformely)
    void CalcBeamStemLength(Staff *staff, data_STEMDIRECTION stemDir);

    // Helper to calculate relative position of the beam to for each of the coordinates
    void CalcMixedBeamPlace(Staff *staff);

    // Helper to calculate proper positioning of the additional beamlines for notes
    void CalcPartialFlagPlace();

    // Helper to simply set the values of each BeamElementCoord according the the first position and the slope
    void CalcSetValues();

    // Helper to check wheter beam fits within certain bounds
    bool DoesBeamOverlap(int staffTop, int topOffset, int staffBottom, int bottomOffset);

    // Helper to check mixed beam positioning compared to other elements (ledger lines, staff) and adjust it accordingly
    bool NeedToResetPosition(Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface);

public:
    // values set by CalcBeam
    int m_startingX; // the initial X position of the beam
    int m_startingY; // the initial Y position of the beam
    int m_nbNotesOrChords;
    bool m_extendedToCenter; // the stem where extended to touch the center staff line
    double m_beamSlope; // the slope of the beam
    int m_verticalCenter;
    int m_avgY;
    int m_ledgerLinesAbove;
    int m_ledgerLinesBelow;
    int m_uniformStemLength;

    BeamElementCoord *m_firstNoteOrChord;
    BeamElementCoord *m_lastNoteOrChord;

    /**
     * An array of coordinates for each element
     **/
    ArrayOfBeamElementCoords m_beamElementCoordRefs;
};

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

class Beam : public LayerElement,
             public ObjectListInterface,
             public BeamDrawingInterface,
             public AttColor,
             public AttBeamedWith,
             public AttBeamRend {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Beam();
    virtual ~Beam();
    virtual Object *Clone() const { return new Beam(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Beam"; }
    virtual ClassId GetClassId() const { return BEAM; }
    ///@}

    int GetNoteCount() const { return this->GetChildCount(NOTE); }

    /**
     * Add an element (a note or a rest) to a beam.
     * Only Note or Rest elements will be actually added to the beam.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Return information about the position in the beam.
     * (no const since the cached list is updated)
     */
    ///@{
    bool IsFirstInBeam(LayerElement *element);
    bool IsLastInBeam(LayerElement *element);
    ///@}

    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoords();

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustBeams
     */
    virtual int AdjustBeams(FunctorParams *);

    /**
     * See Object::AdjustBeamsEnd
     */
    virtual int AdjustBeamsEnd(FunctorParams *);

    /**
     * See Object::CalcStem
     */
    virtual int CalcStem(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

protected:
    /**
     * Filter the flat list and keep only Note and Chords elements.
     * This also initializes the m_beamElementCoords vector
     */
    virtual void FilterList(ArrayOfObjects *childList);

    /**
     * Return the position of the element in the beam.
     * For notes, lookup the position of the parent chord.
     */
    int GetPosition(LayerElement *element);

private:
    //
public:
    /** */
    BeamSegment m_beamSegment;
};

//----------------------------------------------------------------------------
// BeamElementCoord
//----------------------------------------------------------------------------

class BeamElementCoord {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    BeamElementCoord()
    {
        m_element = NULL;
        m_closestNote = NULL;
        m_stem = NULL;
        m_overlapMargin = 0;
        m_beamRelativePlace = BEAMPLACE_NONE;
        m_partialFlagPlace = BEAMPLACE_NONE;
    }
    virtual ~BeamElementCoord();

    /**
     * Return the encoded stem direction.
     * Access the value in the Stem element if already set.
     */
    data_STEMDIRECTION GetStemDir();

    void SetDrawingStemDir(
        data_STEMDIRECTION stemDir, Staff *staff, Doc *doc, BeamSegment *segment, BeamDrawingInterface *interface);
    void SetClosestNote(data_STEMDIRECTION stemDir);

    int CalculateStemLength(Staff *staff, data_STEMDIRECTION stemDir);

    int m_x;
    int m_yBeam; // y value of stem top position
    int m_dur; // drawing duration
    int m_breaksec;
    int m_overlapMargin;
    bool m_centered; // beam is centered on the line
    bool m_shortened; // stem is shortened because pointing oustide the staff
    data_BEAMPLACE m_beamRelativePlace;
    char m_partialFlags[MAX_DURATION_PARTIALS];
    data_BEAMPLACE m_partialFlagPlace;
    LayerElement *m_element;
    Note *m_closestNote;
    Stem *m_stem; // a pointer to the stem in order to avoid to have to re-cast it
};

} // namespace vrv

#endif
