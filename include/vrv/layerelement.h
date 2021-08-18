/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LAYER_ELEMENT_H__
#define __VRV_LAYER_ELEMENT_H__

#include <set>

//----------------------------------------------------------------------------

#include "atts_shared.h"
#include "facsimileinterface.h"
#include "linkinginterface.h"
#include "object.h"

namespace vrv {

class Alignment;
class Beam;
class BeamElementCoord;
class FTrem;
class Layer;
class Mensur;
class MeterSig;
class Staff;
class StaffAlignment;

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

/**
 * This class is a base class for the Layer (<layer>) content.
 * It is not an abstract class but should not be instantiated directly.
 */
class LayerElement : public Object,
                     public FacsimileInterface,
                     public LinkingInterface,
                     public AttLabelled,
                     public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    LayerElement();
    LayerElement(const std::string &classid);
    virtual ~LayerElement();
    virtual void Reset();
    virtual ClassId GetClassId() const { return LAYER_ELEMENT; }
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    virtual void CloneReset();

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual FacsimileInterface *GetFacsimileInterface() { return dynamic_cast<FacsimileInterface *>(this); }
    virtual LinkingInterface *GetLinkingInterface() { return dynamic_cast<LinkingInterface *>(this); }
    ///@}

    /**
     * Return true if the element has to be aligned horizontally
     * It typically set to false for mRest, mRpt, etc.
     */
    virtual bool HasToBeAligned() const { return false; }

    /**
     * Return true if the element is part of a scoreDef or staffDef
     */
    virtual bool IsScoreDefElement() const { return false; }

    /**
     * Return true if the element is relative to the staff and not to its parent.
     * It typically set to true for syl or artic.
     */
    virtual bool IsRelativeToStaff() const { return false; }

    /**
     * Return itself or the resolved @sameas (if any)
     */
    LayerElement *ThisOrSameasAsLink();

    /**
     * @name Set and get the flag for indication whether it is a ScoreDef or StaffDef attribute.
     * The value is false by default. It is set to true by ScoreDef and StaffDef and used when
     * drawing the element.
     * NB In the scoreDef or staffDef itself, it can be either an attribute or an element.
     */
    ///@{
    ElementScoreDefRole GetScoreDefRole() const { return m_scoreDefRole; }
    void SetScoreDefRole(ElementScoreDefRole scoreDefRole) { m_scoreDefRole = scoreDefRole; }
    ///@}

    /**
     * @name Child type checkers.
     */
    ///@{
    /** Return true if the element is a grace note */
    bool IsGraceNote();
    /** Return true if the element is has to be rederred as cue sized */
    bool GetDrawingCueSize() const;
    /** Return true if the element is a note within a ligature */
    bool IsInLigature() const;
    /** Return the FTrem parten if the element is a note or a chord within a fTrem */
    FTrem *IsInFTrem();
    /**
     * Return the beam parent if in beam
     * Look if the note or rest is in a beam.
     * Look for the first beam parent and check if the note is in its content list.
     * Looking in the content list is necessary for grace notes or imbricated beams.
     */
    Beam *IsInBeam();
    ///@}

    /**
     * @name Get and set the layerN drawing value
     */
    ///@{
    int GetAlignmentLayerN() const { return m_alignmentLayerN; }
    void SetAlignmentLayerN(int alignmentLayerN) { m_alignmentLayerN = alignmentLayerN; }
    ///@}

    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;
    ///@}

    /**
     * @name Get and set the X and Y drawing relative positions
     */
    ///@{
    int GetDrawingXRel() const { return m_drawingXRel; }
    virtual void SetDrawingXRel(int drawingXRel);
    int GetDrawingYRel() const { return m_drawingYRel; }
    virtual void SetDrawingYRel(int drawingYRel);
    ///@}

    /**
     * Ajust the m_drawingYRel for the element to be centered on the inner content of the measure
     */
    void CenterDrawingX();

    /**
     * Returns the drawing top and bottom taking into accound stem, etc.
     * We pass the doc as parameter in order to have access to the current drawing parameters.
     * withArtic specifies if the articulation sign needs to be taken into account or not.
     * articType indicates if the inside or outside artic part has to be taken into account (inside is taken
     * into account in any case)
     */
    int GetDrawingTop(Doc *doc, int staffSize, bool withArtic = true, ArticType articType = ARTIC_INSIDE);
    int GetDrawingBottom(Doc *doc, int staffSize, bool withArtic = true, ArticType articType = ARTIC_INSIDE);

    /**
     * Return the drawing radius for notes and chords
     */
    int GetDrawingRadius(Doc *doc, bool isInLigature = false);

    /**
     * Alignment getter
     */
    Alignment *GetAlignment() const { return m_alignment; }

    /**
     * Look for a cross or a a parent LayerElement (note, chord, rest) with a cross staff.
     * Also set the corresponding m_crossLayer to layer if a cross staff is found.
     * Return NULL if there is no cross-staff in the element or a parent.
     */
    Staff *GetCrossStaff(Layer *&layer) const;

    /**
     * Retrieve the direction of a cross-staff situation
     */
    data_STAFFREL_basic GetCrossStaffRel();

    /**
     * Get the StaffAlignment for which overflows need to be calculated against.
     * Set to NULL when the overflow needs to be ignored (e.g., for something between the staves in
     * cross-staff situations.)
     */
    void GetOverflowStaffAlignments(StaffAlignment *&above, StaffAlignment *&below);

    /**
     * @name Setter and getter for the Alignment the grace note is pointing to (NULL by default)
     */
    ///@{
    Alignment *GetGraceAlignment() const;
    void SetGraceAlignment(Alignment *graceAlignment);
    bool HasGraceAlignment() const { return (m_graceAlignment != NULL); }
    ///@}

    /**
     * Returns the duration if the element has a DurationInterface
     */
    double GetAlignmentDuration(Mensur *mensur = NULL, MeterSig *meterSig = NULL, bool notGraceOnly = true,
        data_NOTATIONTYPE notationType = NOTATIONTYPE_cmn);

    /**
     * Returns the duration if the content of the layer element with a @sameas attribute.
     * Used only on beam, tuplet or ftrem have.
     */
    double GetSameAsContentAlignmentDuration(Mensur *mensur = NULL, MeterSig *meterSig = NULL, bool notGraceOnly = true,
        data_NOTATIONTYPE notationType = NOTATIONTYPE_cmn) const;

    double GetContentAlignmentDuration(Mensur *mensur = NULL, MeterSig *meterSig = NULL, bool notGraceOnly = true,
        data_NOTATIONTYPE notationType = NOTATIONTYPE_cmn) const;

    /**
     * Get zone bounds using child elements with facsimile information.
     * Returns true if bounds can be constructed, false otherwise.
     */
    bool GenerateZoneBounds(int *ulx, int *uly, int *lrx, int *lry);

    /**
     * Helper to adjust overlapping layers for notes, chords, stems, etc.
     * Returns the shift of the adjustment
     */
    virtual int AdjustOverlappingLayers(
        Doc *doc, const std::vector<LayerElement *> &otherElements, bool areDotsAdjusted, bool &isUnison);

    /**
     * Calculate note horizontal overlap with elemenents from another layers. Returns overlapMargin and index of other
     * element if it's in unison with it
     */
    std::pair<int, bool> CalcElementHorizontalOverlap(Doc *doc, const std::vector<LayerElement *> &otherElements,
        bool areDotsAdjusted, bool isChordElement, bool isLowerElement = false, bool unison = true);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustBeams
     */
    virtual int AdjustBeams(FunctorParams *);

    /**
     * See Object::AdjustDots
     */
    virtual int AdjustDots(FunctorParams *);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ApplyPPUFactor
     */
    virtual int ApplyPPUFactor(FunctorParams *functorParams);

    /**
     * See Object::AlignHorizontally
     */
    virtual int AlignHorizontally(FunctorParams *functorParams);

    /**
     * See Object::AdjustLayers
     */
    virtual int AdjustLayers(FunctorParams *functorParams);

    /**
     * See Object::AdjustGraceXPos
     */
    ///@{
    virtual int AdjustGraceXPos(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AdjustTupletNumOverlap
     */
    virtual int AdjustTupletNumOverlap(FunctorParams *functorParams);

    /**
     * See Object::AdjustXPos
     */
    virtual int AdjustXPos(FunctorParams *functorParams);

    /**
     * See Object::AdjustXRelForTranscription
     */
    virtual int AdjustXRelForTranscription(FunctorParams *);

    /**
     * See Object::PrepareDrawingCueSize
     */
    virtual int PrepareDrawingCueSize(FunctorParams *functorParams);

    /**
     * See Object::PrepareCrossStaff
     */
    ///@{
    virtual int PrepareCrossStaff(FunctorParams *functorParams);
    virtual int PrepareCrossStaffEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::PreparePointersByLayer
     */
    virtual int PreparePointersByLayer(FunctorParams *functorParams);

    /**
     * See Object::PrepareDelayedTurns
     */
    virtual int PrepareDelayedTurns(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimePointing
     */
    virtual int PrepareTimePointing(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::SetAlignmentPitchPos
     */
    virtual int SetAlignmentPitchPos(FunctorParams *functorParams);

    /**
     * See Object::FindSpannedLayerElements
     */
    virtual int FindSpannedLayerElements(FunctorParams *functorParams);

    /**
     * See Object::LayerCountInTimeSpan
     */
    virtual int LayerCountInTimeSpan(FunctorParams *functorParams);

    /**
     * See Object::LayerElementsInTimeSpan
     */
    virtual int LayerElementsInTimeSpan(FunctorParams *functorParams);

    /**
     * See Object::CalcOnsetOffset
     */
    ///@{
    virtual int CalcOnsetOffset(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::ResolveMIDITies
     */
    ///@{
    virtual int ResolveMIDITies(FunctorParams *);
    ///@}

    /**
     * @name See Object::GenerateMIDI
     */
    ///@{
    virtual int GenerateMIDI(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::GenerateTimemap
     */
    virtual int GenerateTimemap(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::GetRelativeLayerElement
     */
    virtual int GetRelativeLayerElement(FunctorParams *functorParams);

protected:
    /**
     * Helper to figure whether two chords are in fully in unison based on the locations of the notes.
     * This function assumes that two chords are already in unison and checks whether chords can overlap with
     * their unison notes or if they should be placed separately.
     * Returns true if all elements can safely overlap.
     */
    virtual int CountElementsInUnison(
        const std::set<int> &firstChord, const std::set<int> &secondChord, data_STEMDIRECTION stemDirection);

    /**
     * The note locations w.r.t. each staff, implemented for note and chord
     */
    virtual MapOfNoteLocs CalcNoteLocations() { return {}; };

    /**
     * The dot locations w.r.t. each staff, implemented for note and chord
     * Since dots for notes on staff lines can be shifted upwards or downwards, there are two choices: primary and
     * secondary
     */
    virtual MapOfDotLocs CalcDotLocations(int layerCount, bool primary) { return {}; };

    /**
     * Calculate the optimal dot location for a note or chord
     * Takes two layers into account in order to avoid collisions of dots between corresponding notes/chords
     */
    MapOfDotLocs CalcOptimalDotLocations();

    //----------------//
    // Static methods //
    //----------------//

    /**
     * Helper to count the number of dots
     * This can be used as an indicator to choose between different sets of dots
     */
    static int GetDotCount(const MapOfDotLocs &dotLocations);

    /**
     * Helper to count the collisions between two sets of dots
     */
    static int GetCollisionCount(const MapOfDotLocs &dotLocs1, const MapOfDotLocs &dotLocs2);

private:
    int GetDrawingArticulationTopOrBottom(data_STAFFREL place, ArticType type);

public:
    /** Absolute position X. This is used for facsimile (transcription) encoding */
    int m_xAbs;
    /**
     * This stores a pointer to the cross-staff (if any) and the appropriate layer
     * See Object::PrepareCrossStaff
     */
    Staff *m_crossStaff;
    Layer *m_crossLayer;

protected:
    Alignment *m_alignment;

    /**
     * An alignment for grace notes
     */
    Alignment *m_graceAlignment;

    /**
     * The Y drawing relative position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingYRel;

    /**
     * The X drawing relative position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingXRel;

    /**
     * The cached drawing cue size set by PrepareDrawingCueSize
     */
    bool m_drawingCueSize;

private:
    /**
     * Indicates whether it is a ScoreDef or StaffDef attribute
     */
    ElementScoreDefRole m_scoreDefRole;

    /**
     * The cached alignment layer @n.
     * This also stores the negative values for identifying cross-staff
     */
    int m_alignmentLayerN;
};

} // namespace vrv

#endif
