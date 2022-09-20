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

// Helper enums
enum StaffSearch { ANCESTOR_ONLY = 0, RESOLVE_CROSS_STAFF };

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
                     public AttCoordX1,
                     public AttLabelled,
                     public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    LayerElement();
    LayerElement(ClassId classId);
    LayerElement(ClassId classId, const std::string &classIdStr);
    virtual ~LayerElement();
    void Reset() override;
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    void CloneReset() override;

    /**
     * @name Getter to interfaces
     */
    ///@{
    FacsimileInterface *GetFacsimileInterface() override { return vrv_cast<FacsimileInterface *>(this); }
    const FacsimileInterface *GetFacsimileInterface() const override
    {
        return vrv_cast<const FacsimileInterface *>(this);
    }
    LinkingInterface *GetLinkingInterface() override { return vrv_cast<LinkingInterface *>(this); }
    const LinkingInterface *GetLinkingInterface() const override { return vrv_cast<const LinkingInterface *>(this); }
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
    ///@{
    LayerElement *ThisOrSameasLink();
    const LayerElement *ThisOrSameasLink() const;
    ///@}

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
    bool IsGraceNote() const;
    /** Return true if the element is has to be rederred as cue sized */
    bool GetDrawingCueSize() const;
    /** Return true if the element is a note within a ligature */
    bool IsInLigature() const;
    /** Return the FTrem parten if the element is a note or a chord within a fTrem */
    FTrem *GetAncestorFTrem();
    const FTrem *GetAncestorFTrem() const;
    /**
     * Return the beam parent if in beam
     * Look if the note or rest is in a beam.
     * Look for the first beam parent and check if the note is in its content list.
     * Looking in the content list is necessary for grace notes or imbricated beams.
     */
    Beam *GetAncestorBeam();
    const Beam *GetAncestorBeam() const;
    bool IsInBeam() const;
    ///@}

    /**
     * @name Setter and getter for isInBeamspan flag
     */
    ///@{
    void SetIsInBeamSpan(bool isInBeamSpan);
    bool GetIsInBeamSpan() const { return m_isInBeamspan; }
    ///@}

    /**
     * @name Get and set the layerN drawing value
     */
    ///@{
    int GetAlignmentLayerN() const { return m_alignmentLayerN; }
    void SetAlignmentLayerN(int alignmentLayerN) { m_alignmentLayerN = alignmentLayerN; }
    ///@}

    /**
     * @return (cross) layer number, parent layer number for cross staff elements
     */
    int GetOriginalLayerN() const;

    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    int GetDrawingX() const override;
    int GetDrawingY() const override;
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
     * Adjust the m_drawingYRel for the element to be centered on the inner content of the measure
     */
    void CenterDrawingX();

    /**
     * Returns the drawing top and bottom taking into accound stem, etc.
     * We pass the doc as parameter in order to have access to the current drawing parameters.
     * withArtic specifies if the articulation sign needs to be taken into account or not.
     * articType indicates if the inside or outside artic part has to be taken into account (inside is taken
     * into account in any case)
     */
    int GetDrawingTop(const Doc *doc, int staffSize, bool withArtic = true, ArticType articType = ARTIC_INSIDE) const;
    int GetDrawingBottom(
        const Doc *doc, int staffSize, bool withArtic = true, ArticType articType = ARTIC_INSIDE) const;

    /**
     * Return the drawing radius for notes and chords
     */
    int GetDrawingRadius(const Doc *doc, bool isInLigature = false) const;

    /**
     * Alignment getter
     */
    ///@{
    Alignment *GetAlignment() { return m_alignment; }
    const Alignment *GetAlignment() const { return m_alignment; }
    ///@}

    /**
     * Get the ancestor or cross staff
     */
    ///@{
    Staff *GetAncestorStaff(StaffSearch strategy = ANCESTOR_ONLY, bool assertExistence = true);
    const Staff *GetAncestorStaff(StaffSearch strategy = ANCESTOR_ONLY, bool assertExistence = true) const;
    ///@}

    /**
     * Look for a cross or a a parent LayerElement (note, chord, rest) with a cross staff.
     * Also set the corresponding m_crossLayer to layer if a cross staff is found.
     * Return NULL if there is no cross-staff in the element or a parent.
     */
    ///@{
    Staff *GetCrossStaff(Layer *&layer);
    const Staff *GetCrossStaff(const Layer *&layer) const;
    ///@}

    /**
     * Retrieve the direction of a cross-staff situation
     */
    data_STAFFREL_basic GetCrossStaffRel() const;

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
    Alignment *GetGraceAlignment();
    const Alignment *GetGraceAlignment() const;
    void SetGraceAlignment(Alignment *graceAlignment);
    bool HasGraceAlignment() const { return (m_graceAlignment != NULL); }
    ///@}

    /**
     * Returns the duration if the element has a DurationInterface
     */
    double GetAlignmentDuration(const Mensur *mensur = NULL, const MeterSig *meterSig = NULL, bool notGraceOnly = true,
        data_NOTATIONTYPE notationType = NOTATIONTYPE_cmn) const;

    /**
     * Returns the duration if the content of the layer element with a @sameas attribute.
     * Used only on beam, tuplet or ftrem have.
     */
    double GetSameAsContentAlignmentDuration(const Mensur *mensur = NULL, const MeterSig *meterSig = NULL,
        bool notGraceOnly = true, data_NOTATIONTYPE notationType = NOTATIONTYPE_cmn) const;

    double GetContentAlignmentDuration(const Mensur *mensur = NULL, const MeterSig *meterSig = NULL,
        bool notGraceOnly = true, data_NOTATIONTYPE notationType = NOTATIONTYPE_cmn) const;

    /**
     * Get zone bounds using child elements with facsimile information.
     * Returns true if bounds can be constructed, false otherwise.
     */
    bool GenerateZoneBounds(int *ulx, int *uly, int *lrx, int *lry) const;

    /**
     * Helper to adjust overlapping layers for notes, chords, stems, etc.
     * Returns the shift of the adjustment
     */
    virtual int AdjustOverlappingLayers(const Doc *doc, const std::vector<LayerElement *> &otherElements,
        bool areDotsAdjusted, bool &isUnison, bool &stemSameAs);

    /**
     * Calculate note horizontal overlap with elemenents from another layers. Returns overlapMargin and index of other
     * element if it's in unison with it
     */
    std::pair<int, bool> CalcElementHorizontalOverlap(const Doc *doc, const std::vector<LayerElement *> &otherElements,
        bool areDotsAdjusted, bool isChordElement, bool isLowerElement = false, bool unison = true);

    /**
     * Helper function to set shortening for elements with beam interface
     */
    virtual void SetElementShortening(int shortening){};

    /**
     * Get the stem mod for the element (if any)
     */
    virtual data_STEMMODIFIER GetDrawingStemMod() const;

    /**
     * Return true if cross-staff is set
     */
    virtual bool HasCrossStaff() const { return (m_crossStaff != NULL); }

    /**
     * Convert stem mode to corresponding glyph code
     */
    char32_t StemModToGlyph(data_STEMMODIFIER stemMod) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustBeams
     */
    int AdjustBeams(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustDots
     */
    int AdjustDots(FunctorParams *functorParams) override;

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::ResetVerticalAlignment
     */
    int ResetVerticalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::ApplyPPUFactor
     */
    int ApplyPPUFactor(FunctorParams *functorParams) override;

    /**
     * See Object::AlignHorizontally
     */
    int AlignHorizontally(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustLayers
     */
    int AdjustLayers(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustGraceXPos
     */
    ///@{
    int AdjustGraceXPos(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::AdjustTupletNumOverlap
     */
    int AdjustTupletNumOverlap(FunctorParams *functorParams) const override;

    /**
     * See Object::AdjustXPos
     */
    int AdjustXPos(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustXRelForTranscription
     */
    int AdjustXRelForTranscription(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareCueSize
     */
    int PrepareCueSize(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareCrossStaff
     */
    ///@{
    int PrepareCrossStaff(FunctorParams *functorParams) override;
    int PrepareCrossStaffEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::PreparePointersByLayer
     */
    int PreparePointersByLayer(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareDelayedTurns
     */
    int PrepareDelayedTurns(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareTimePointing
     */
    int PrepareTimePointing(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareTimeSpanning
     */
    int PrepareTimeSpanning(FunctorParams *functorParams) override;

    /**
     * See Object::CalcAlignmentPitchPos
     */
    int CalcAlignmentPitchPos(FunctorParams *functorParams) override;

    /**
     * See Object::FindSpannedLayerElements
     */
    int FindSpannedLayerElements(FunctorParams *functorParams) const override;

    /**
     * See Object::LayerCountInTimeSpan
     */
    int LayerCountInTimeSpan(FunctorParams *functorParams) const override;

    /**
     * See Object::LayerElementsInTimeSpan
     */
    int LayerElementsInTimeSpan(FunctorParams *functorParams) const override;

    /**
     * See Object::InitOnsetOffset
     */
    ///@{
    int InitOnsetOffset(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::InitTimemapTies
     */
    ///@{
    int InitTimemapTies(FunctorParams *functorParams) override;
    ///@}

    /**
     * @name See Object::GenerateMIDI
     */
    ///@{
    int GenerateMIDI(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::GenerateTimemap
     */
    int GenerateTimemap(FunctorParams *functorParams) override;

    /**
     * See Object::CalcMaxMeasureDuration
     */
    int InitMaxMeasureDuration(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::GetRelativeLayerElement
     */
    int GetRelativeLayerElement(FunctorParams *functorParams) const override;

    /**
     * See Object::CalcSlurDirection
     */
    int CalcSlurDirection(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareDuration
     */
    int PrepareDuration(FunctorParams *functorParams) override;

    /**
     * See Object::CacheHorizontalLayout
     */
    int CacheHorizontalLayout(FunctorParams *functorParams) override;

protected:
    /**
     * Helper to figure whether two chords are in fully in unison based on the locations of the notes.
     * This function assumes that two chords are already in unison and checks whether chords can overlap with
     * their unison notes or if they should be placed separately.
     * Returns vector with all locations of elements in unison.
     */
    std::vector<int> GetElementsInUnison(
        const std::set<int> &firstChord, const std::set<int> &secondChord, data_STEMDIRECTION stemDirection) const;

    /**
     * The note locations w.r.t. each staff, implemented for note and chord
     */
    virtual MapOfNoteLocs CalcNoteLocations(NotePredicate predicate = NULL) const { return {}; }

    /**
     * The dot locations w.r.t. each staff, implemented for note and chord
     * Since dots for notes on staff lines can be shifted upwards or downwards, there are two choices: primary and
     * secondary
     */
    virtual MapOfDotLocs CalcDotLocations(int layerCount, bool primary) const { return {}; }

    /**
     * Helper function to calculate overlap with layer elements that
     * are placed within the duration of element
     */
    int CalcLayerOverlap(const Doc *doc, int direction, int y1, int y2);

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
    int GetDrawingArticulationTopOrBottom(data_STAFFREL place, ArticType type) const;

    /**
     * Get above/below overflow for the chord elements
     */
    void GetChordOverflow(StaffAlignment *&above, StaffAlignment *&below, int staffN);

    /**
     * Calculate offset and left overlap of the element
     */
    std::pair<int, int> CalculateXPosOffset(FunctorParams *functorParams);

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
     * The cached value for m_drawingYRel for caching horizontal layout
     */
    int m_cachedYRel;

    /**
     * The X drawing relative position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingXRel;

    /**
     * The cached value for m_drawingXRel for caching horizontal layout
     */
    int m_cachedXRel;

    /**
     * The cached drawing cue size set by PrepareCueSize
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

    // flag to indicate that layerElement belongs to the beamSpan
    bool m_isInBeamspan;
};

} // namespace vrv

#endif
