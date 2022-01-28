/////////////////////////////////////////////////////////////////////////////
// Name:        beam.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BEAM_H__
#define __VRV_BEAM_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "drawinginterface.h"
#include "layerelement.h"

namespace vrv {

class BeamElementCoord;
class StaffAlignment;

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

    void CalcTabBeam(Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, data_BEAMPLACE place);

    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoordRefs() const;

    /**
     * Initializes the m_beamElementCoords vector objects.
     * This is called by Beam::FilterList
     */
    void InitCoordRefs(const ArrayOfBeamElementCoords *beamElementCoords);

    /**
     * Clear the m_beamElementCoords vector and delete all the objects.
     */
    void ClearCoordRefs();

    /**
     * Get longest duration of the elements that are adjacent to the X coordinate passed
     */
    int GetAdjacentElementsDuration(int elementX) const;

    /**
     * @name Getters for the X and Y starting value of the beam;
     */
    ///@{
    int GetStartingX() const;
    int GetStartingY() const;
    ///@}

    /**
     * @name Getters for the stem same as role.
     */
    ///@{
    bool StemSameas() const { return (m_stemSameasRole != SAMEAS_NONE); }
    bool StemSameasIsUnset() const { return (m_stemSameasRole == SAMEAS_UNSET); }
    bool StemSameasIsPrimary() const { return (m_stemSameasRole == SAMEAS_PRIMARY); }
    bool StemSameasIsSecondary() const { return (m_stemSameasRole == SAMEAS_SECONDARY); }
    ///@}

    /**
     * @name Methods to intialize and update the role for stem.sameas context
     * Before the drawing beam place has been calculated, the role is marked as unset.
     * Then it is marked as primary or secondary depending on the beam place.
     */
    ///@{
    void InitSameasRoles(Beam *sameasBeam, data_BEAMPLACE &drawingPlace);
    void UpdateSameasRoles(data_BEAMPLACE place);
    ///@}

private:
    // Helper to adjust beam positioning with regards to ledger lines (top and bottom of the staff)
    void AdjustBeamToLedgerLines(Doc *doc, Staff *staff, BeamDrawingInterface *beamInterface);

    void CalcBeamInit(Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, data_BEAMPLACE place);

    void CalcBeamInitForNotePair(Note *note1, Note *note2, Staff *staff, int &yMax, int &yMin);

    bool CalcBeamSlope(
        Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, bool &shorten, int &step);

    void CalcBeamPosition(Doc *doc, Staff *staff, Layer *layer, BeamDrawingInterface *beamInterface, bool isHorizontal);

    void CalcAdjustSlope(Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, bool shorten, int &step);

    // Helper to adjust position of starting point to make sure that beam start-/endpoints touch the staff lines
    void CalcAdjustPosition(Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface);

    void CalcBeamPlace(Layer *layer, BeamDrawingInterface *beamInterface, data_BEAMPLACE place);

    // Helper to calculate the longest stem length of the beam (which will be used uniformely)
    void CalcBeamStemLength(Staff *staff, data_BEAMPLACE place, bool isHorizontal);

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
    int m_nbNotesOrChords;
    double m_beamSlope; // the slope of the beam
    int m_verticalCenter;
    int m_ledgerLinesAbove;
    int m_ledgerLinesBelow;
    int m_uniformStemLength;
    data_BEAMPLACE m_weightedPlace;

    BeamElementCoord *m_firstNoteOrChord;
    BeamElementCoord *m_lastNoteOrChord;

    /**
     * An array of coordinates for each element
     **/
    ArrayOfBeamElementCoords m_beamElementCoordRefs;

    /**
     * @name The role in a stem.sameas situation.
     * Set in BeamSegment::InitSameasRoles and then in UpdateSameasRoles
     * Used to determine if the beam is the primary one (normal stems and beams)
     * or the secondary one (linking both notes). This depends on the drawing beam place,
     * which can be encoded but otherwise calculated by CalcBeamPlace.
     * The pointer to the other beam m_stemSameasReverseRole is set only for the first of the
     * two beams and is not bi-directional. Is is set in InitSameasRoles.
     */
    ///@{
    StemSameasDrawingRole m_stemSameasRole;
    StemSameasDrawingRole *m_stemSameasReverseRole;
    ///@}
};

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

class Beam : public LayerElement,
             public BeamDrawingInterface,
             public AttBeamedWith,
             public AttBeamRend,
             public AttColor,
             public AttCue {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Beam();
    virtual ~Beam();
    Object *Clone() const override { return new Beam(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Beam"; }
    ///@}

    int GetNoteCount() const { return this->GetChildCount(NOTE); }

    /**
     * Add an element (a note or a rest) to a beam.
     * Only Note or Rest elements will be actually added to the beam.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoords();

    /**
     * Return true if the beam has a tabGrp child.
     * In that case, the ObjectList will only have tabGrp elements. See Beam::FilterList
     */
    bool IsTabBeam();

    /**
     * @name Checker, getter and setter for a beam with which the stems are shared
     */
    ///@{
    bool HasStemSameasBeam() const { return (m_stemSameas); }
    Beam *GetStemSameasBeam() const { return m_stemSameas; }
    void SetStemSameasBeam(Beam *stemSameas) { m_stemSameas = stemSameas; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustBeams
     */
    int AdjustBeams(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustBeamsEnd
     */
    int AdjustBeamsEnd(FunctorParams *functorParams) override;

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::ResetDrawing
     */
    int ResetDrawing(FunctorParams *functorParams) override;

protected:
    /**
     * Filter the flat list and keep only Note and Chords elements.
     * This also initializes the m_beamElementCoords vector
     */
    void FilterList(ArrayOfObjects *childList) override;

    /**
     * Helper function to calculate overlap with layer elements that
     * are placed within the duration of the beam
     */
    int CalcLayerOverlap(Doc *doc, Object *beam, int directionBias, int y1, int y2);

private:
    /**
     * A pointer to the beam with which stems are shared.
     * The pointer is bi-directional – each beam points to the other one.
     * It is set in Note::ResovleStemSameas()
     */
    Beam *m_stemSameas;

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
        m_maxShortening = -1;
        m_beamRelativePlace = BEAMPLACE_NONE;
        m_partialFlagPlace = BEAMPLACE_NONE;
    }
    virtual ~BeamElementCoord();

    /**
     * Return the encoded stem direction.
     * Access the value in the Stem element if already set.
     */
    data_STEMDIRECTION GetStemDir() const;

    void SetDrawingStemDir(
        data_STEMDIRECTION stemDir, Staff *staff, Doc *doc, BeamSegment *segment, BeamDrawingInterface *interface);
    void SetClosestNote(data_STEMDIRECTION stemDir);

    int CalculateStemLength(Staff *staff, data_STEMDIRECTION stemDir, bool isHorizontal);

    /**
     * Return stem length adjustment in half units, depending on the @stem.mode attribute
     */
    int CalculateStemModAdjustment(int stemLength, int directionBias);

    int m_x;
    int m_yBeam; // y value of stem top position
    int m_dur; // drawing duration
    int m_breaksec;
    int m_overlapMargin;
    int m_maxShortening; // maximum allowed shortening in half units
    bool m_centered; // beam is centered on the line
    data_BEAMPLACE m_beamRelativePlace;
    char m_partialFlags[MAX_DURATION_PARTIALS];
    data_BEAMPLACE m_partialFlagPlace;
    LayerElement *m_element;
    Note *m_closestNote;
    Stem *m_stem; // a pointer to the stem in order to avoid to have to re-cast it
};

} // namespace vrv

#endif
