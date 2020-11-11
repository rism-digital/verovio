/////////////////////////////////////////////////////////////////////////////
// Name:        horizontalaligner.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_HORIZONTAL_ALIGNER_H__
#define __VRV_HORIZONTAL_ALIGNER_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

class Accid;
class AlignmentReference;
class FloatingObject;
class GraceAligner;
class LedgerLine;
class MeasureAligner;
class Note;
class TimestampAttr;

/**
 * Alignment types for aligning types together.
 * For example, we align notes and rests (default) together, clefs separately, etc.
 */
enum AlignmentType {
    ALIGNMENT_MEASURE_START = 0,
    // Non-justifiable
    ALIGNMENT_SCOREDEF_CLEF,
    ALIGNMENT_SCOREDEF_KEYSIG,
    ALIGNMENT_SCOREDEF_MENSUR,
    ALIGNMENT_SCOREDEF_METERSIG,
    ALIGNMENT_MEASURE_LEFT_BARLINE,
    // Justifiable
    ALIGNMENT_FULLMEASURE,
    ALIGNMENT_FULLMEASURE2,
    ALIGNMENT_CLEF,
    ALIGNMENT_KEYSIG,
    ALIGNMENT_MENSUR,
    ALIGNMENT_METERSIG,
    ALIGNMENT_DOT,
    ALIGNMENT_ACCID,
    ALIGNMENT_GRACENOTE,
    ALIGNMENT_BARLINE,
    ALIGNMENT_DEFAULT,
    // Non-justifiable
    ALIGNMENT_MEASURE_RIGHT_BARLINE,
    ALIGNMENT_SCOREDEF_CAUTION_CLEF,
    ALIGNMENT_SCOREDEF_CAUTION_KEYSIG,
    ALIGNMENT_SCOREDEF_CAUTION_MENSUR,
    ALIGNMENT_SCOREDEF_CAUTION_METERSIG,
    ALIGNMENT_MEASURE_END
};

#define BARLINE_REFERENCES -1
#define TSTAMP_REFERENCES -2

//----------------------------------------------------------------------------
// Alignment
//----------------------------------------------------------------------------

/**
 * This class stores an alignement position elements will point to
 */
class Alignment : public Object {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    Alignment();
    Alignment(double time, AlignmentType type = ALIGNMENT_DEFAULT);
    virtual ~Alignment();
    virtual void Reset();
    virtual ClassId GetClassId() const { return ALIGNMENT; }
    ///@}

    /**
     * Delete the grace aligners in the map
     */
    void ClearGraceAligners();

    /**
     * Override the method of adding AlignmentReference children
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * @name Set and get the xRel value of the alignment
     */
    ///@{
    void SetXRel(int xRel);
    int GetXRel() const { return m_xRel; }
    ///@}

    /**
     * @name Set and get the time value of the alignment
     */
    ///@{
    void SetTime(double time) { m_time = time; }
    double GetTime() const { return m_time; }
    ///@}

    /**
     * Add the LayerElement to the appropriate AlignmentReference child.
     * Looks at the cross-staff situation (@staff or parent @staff).
     * Return true if the AlignmentReference holds more than one layer
     */
    bool AddLayerElementRef(LayerElement *element);

    /**
     * @name Set and get the type of the alignment
     */
    ///@{
    void SetType(AlignmentType type) { m_type = type; }
    AlignmentType GetType() const { return m_type; }
    ///@}

    /**
     * Check if the element is of on of the types
     */
    bool IsOfType(const std::vector<AlignmentType> &types);

    /**
     * Retrive the minimum left and maximum right position for the objects in an alignment.
     * Returns (-)VRV_UNSET in nothing for the staff specified.
     * Uses Object::GetAlignmentLeftRight
     */
    void GetLeftRight(int staffN, int &minLeft, int &maxRight);

    /**
     * Returns the GraceAligner for the Alignment.
     * Create it if necessary.
     */
    GraceAligner *GetGraceAligner(int id);

    /**
     * Returns true if the aligner has a GraceAligner
     */
    bool HasGraceAligner(int id) const;

    /**
     * Return the AlignmentReference holding the element.
     * If staffN is provided, uses the AlignmentReference->GetN() to accelerate the search.
     */
    AlignmentReference *GetReferenceWithElement(LayerElement *element, int staffN = VRV_UNSET);

    /**
     * Add an accidental to the accidSpace of the AlignmentReference holding it.
     * The Alignment has to have a AlignmentReference holding it.
     */
    void AddToAccidSpace(Accid *accid);

    /**
     * Compute "ideal" horizontal space to allow for a given time interval, ignoring the need
     * to keep consecutive symbols from overlapping or nearly overlapping: we assume spacing
     * will be increased as necessary later to avoid that. For modern notation (CMN), ideal space
     * is a function of time interval.

     * For a discussion of the way engravers determine spacing, see Elaine Gould, _Behind Bars_,
     * p. 39. But we need something more flexible, because, for example: (1) We're interested in
     * music with notes of very long duration: say, music in mensural notation containing longas
     * or maximas; such music is usually not spaced by duration, but we support spacing by
     * duration if the user wishes, and standard engravers' rules would waste a lot of space.
     * (2) For some purposes, spacing strictly proportional to duration is desirable. The most
     * flexible solution might be to get ideal spacing from a user-definable table, but using a
     * formula with parameters can come close and has other advantages.
     */
    virtual int HorizontalSpaceForDuration(
        double intervalTime, int maxActualDur, double spacingLinear, double spacingNonLinear);

    /**
     * Return true if the alignment contains at least one reference with staffN
     */
    bool HasAlignmentReference(int staffN);

    //----------//
    // Functors //
    //----------//

    /**
     * Set the position of the Alignment.
     * Looks at the time different with the previous Alignment.
     */
    virtual int SetAlignmentXPos(FunctorParams *functorParams);

    /**
     * Justify the X positions
     * Special case of functor redirected from Measure.
     */
    virtual int JustifyX(FunctorParams *functorParams);

    /**
     * See Object::AdjustArpeg
     */
    virtual int AdjustArpeg(FunctorParams *functorParams);

    /**
     * See Object::AdjustGraceXPos
     */
    ///@{
    virtual int AdjustGraceXPos(FunctorParams *functorParams);
    virtual int AdjustGraceXPosEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AdjustXPos
     */
    ///@{
    virtual int AdjustXPos(FunctorParams *functorParams);
    virtual int AdjustXPosEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AjustAccidX
     */
    virtual int AdjustAccidX(FunctorParams *functorParams);

private:
    /**
     * Retrieve the AlignmentReference with staffN.
     * Create and add it as child if not found.
     */
    AlignmentReference *GetAlignmentReference(int staffN);

public:
    //
private:
    /**
     * Stores the position relative to the measure.
     * This is instanciated by the Object::SetAlignmentXPos functor.
     * It takes into account a non-linear according to the time interval with
     * the previous Alignement
     */
    int m_xRel;
    /**
     * Stores the time at which the alignment occur.
     * It is set by Object::AlignHorizontally.
     */
    double m_time;
    /**
     * Defines the type of alignment (see the AlignmentType enum).
     * We have different types because we want some events occuring at the same
     * time to be aligned separately. Examples: the clefs needs to be aligned
     * together, key signatures together, and then the notes, even if all of them
     * occur at time 0.
     */
    AlignmentType m_type;
    /**
     * A map of GraceAligners if any.
     * The Alignment owns them.
     */
    MapOfIntGraceAligners m_graceAligners;
};

//----------------------------------------------------------------------------
// AlignmentReference
//----------------------------------------------------------------------------

/**
 * This class stores a references of LayerElements for a staff.
 * The staff identification (@n) is given by the attCommon and takes into accound
 * cross-staff situations.
 * Its children of the alignment are references.
 */
class AlignmentReference : public Object, public AttNInteger {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    AlignmentReference();
    AlignmentReference(int staffN);
    virtual ~AlignmentReference();
    virtual void Reset();
    virtual ClassId GetClassId() const { return ALIGNMENT_REFERENCE; }
    ///@}

    /**
     * Override the method of adding AlignmentReference children
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Overwritten method for AlignmentReference children
     */
    virtual void AddChild(Object *object);

    /**
     * Add an accidental to the accidSpace of the AlignmentReference.
     */
    void AddToAccidSpace(Accid *accid);

    /**
     * See Object::AjustAccidX
     */
    void AdjustAccidWithAccidSpace(Accid *accid, Doc *doc, int staffSize);

    /**
     * Return true if the reference has elements from multiple layers.
     */
    bool HasMultipleLayer() const { return (m_layerCount > 1); }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustLayers
     */
    virtual int AdjustLayers(FunctorParams *functorParams);

    /**
     * See Object::AdjustGraceXPos
     */
    virtual int AdjustGraceXPos(FunctorParams *functorParams);

    /**
     * See Object::AjustAccidX
     */
    virtual int AdjustAccidX(FunctorParams *functorParams);

    /**
     * See Object::UnsetCurrentScoreDef
     */
    virtual int UnsetCurrentScoreDef(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    /**
     * The accid space of the AlignmentReference.
     */
    std::vector<Accid *> m_accidSpace;

    /**
     *
     */
    int m_layerCount;
};

//----------------------------------------------------------------------------
// HorizontalAligner
//----------------------------------------------------------------------------

/**
 * This class aligns the content horizontally
 * It contains a vector of Alignment.
 * It is not an abstract class but it should not be instanciated directly.
 */
class HorizontalAligner : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@(
    HorizontalAligner();
    virtual ~HorizontalAligner();
    virtual void Reset();
    ///@}

    /**
     * Do not copy children for HorizontalAligner
     */
    virtual bool CopyChildren() const { return false; }

    int GetAlignmentCount() const { return (int)GetChildren()->size(); }

    //----------//
    // Functors //
    //----------//

protected:
    /**
     * Search if an alignment of the type is already there at the time.
     * If not, return in idx the position where it needs to be inserted (-1 if it is the end)
     */
    Alignment *SearchAlignmentAtTime(double time, AlignmentType type, int &idx);

    /**
     * Add an alignment at the appropriate position (at the end if -1)
     */
    void AddAlignment(Alignment *alignment, int idx = -1);

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// MeasureAligner
//----------------------------------------------------------------------------

/**
 * This class aligns the content of a measure
 * It contains a vector of Alignment
 */
class MeasureAligner : public HorizontalAligner {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@(
    MeasureAligner();
    virtual ~MeasureAligner();
    virtual ClassId GetClassId() const { return MEASURE_ALIGNER; }
    virtual void Reset();
    ///@}

    /**
     * Retrieve the alignmnet of the type at that time.
     * The alignment object is added if not found.
     * The maximum time position is also adjusted accordingly for end barline positioning
     */
    Alignment *GetAlignmentAtTime(double time, AlignmentType type);

    /**
     * Keep the maximum time of the measure.
     * This corresponds to the whole duration of the measure and
     * should be the same for all staves/layers.
     */
    void SetMaxTime(double time);

    /**
     * Return the max time of the measure (i.e., the right measure alignment time)
     */
    double GetMaxTime() const;

    /**
     * @name Set and Get the non-justifiable margin (right and left scoreDefs)
     */
    ///@{
    int GetNonJustifiableMargin() const { return m_nonJustifiableLeftMargin; }
    ///@}

    /**
     * Get left Alignment for the measure and for the left BarLine.
     * For each MeasureAligner, we keep and Alignment for the left position.
     * The Alignment time will be always -1.0 * DUR_MAX and will appear first in the list.
     */
    Alignment *GetLeftAlignment() const { return m_leftAlignment; }
    Alignment *GetLeftBarLineAlignment() const { return m_leftBarLineAlignment; }

    /**
     * Get right Alignment for the measure.
     * For each MeasureAligner, we keep and Alignment for the right position.
     * The Alignment time will be increased whenever necessary when values are added.
     */
    Alignment *GetRightAlignment() const { return m_rightAlignment; }
    Alignment *GetRightBarLineAlignment() const { return m_rightBarLineAlignment; }

    /**
     * Adjust the spacing of the measure looking at each tuple of start / end alignment and a distance.
     * The distance is an expansion value (positive) of compression (negative).
     * Called from Measure::AdjustSylSpacingEnd.
     */
    void AdjustProportionally(const ArrayOfAdjustmentTuples &adjustments);

    /**
     * Push all the ALIGNMENT_GRACENOTE and ALIGNMENT_CONTAINER to the right.
     * This is necessary to make sure they align with the next alignment content.
     */
    void PushAlignmentsRight();

    /**
     * Adjust the spacing for the grace note group(s) of the alignment on staffN
     * The alignment need to be of ALIGNMENT_GRACENOTE type
     */
    void AdjustGraceNoteSpacing(Doc *doc, Alignment *alignment, int staffN);

    //----------//
    // Functors //
    //----------//

    /**
     * Set the position of the Alignment.
     * Looks at the time different with the previous Alignment.
     * For each MeasureAlignment, we need to reset the previous time position.
     */
    virtual int SetAlignmentXPos(FunctorParams *functorParams);

    /**
     * Justify the X positions
     * Special case of functor redirected from Measure.
     */
    virtual int JustifyX(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    /**
     * A pointer to the left and right Alignment object kept for the measure start and end position
     */
    Alignment *m_leftAlignment;
    Alignment *m_rightAlignment;

    /**
     * A pointer to the left and right Alignment object kept for the left and right barline position
     */
    Alignment *m_leftBarLineAlignment;
    Alignment *m_rightBarLineAlignment;

    /**
     * Store measure's non-justifiable margin used by the scoreDef attributes.
     */
    int m_nonJustifiableLeftMargin;
};

//----------------------------------------------------------------------------
// GraceAligner
//----------------------------------------------------------------------------

/**
 * This class aligns the content of a grace note group
 * It contains a vector of Alignment
 */
class GraceAligner : public HorizontalAligner {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@(
    GraceAligner();
    virtual ~GraceAligner();
    virtual ClassId GetClassId() const { return GRACE_ALIGNER; }
    virtual void Reset();
    ///@}

    /**
     * Retrieve the alignmnet of the type at that time.
     * The alignment object is added if not found.
     */
    Alignment *GetAlignmentAtTime(double time, AlignmentType type);

    /**
     * Because the grace notes appear from left to right but need to be aligned
     * from right to left, we first need to stack them and align them eventually
     * when we have all of them. This is done by GraceAligner::AlignStack called
     * at the end of each Layer in
     */
    void StackGraceElement(LayerElement *object);

    /**
     * Align the notes in the reverse order
     */
    void AlignStack();

    /**
     * @name Setter and getter for the width of the group of grace notes
     */
    ///@{
    void SetWidth(int totalWidth) { m_totalWidth = totalWidth; }
    int GetWidth() const { return m_totalWidth; }
    ///@}

    /**
     * @name Return the left / right position of the first / last note matching by staffN
     * Setting staffN as VRV_UNSET will look for and align all staves.
     */
    ///@{
    int GetGraceGroupLeft(int staffN);
    int GetGraceGroupRight(int staffN);
    ///@{

    /**
     * Set an linear defaut position for each grace note
     * This is called from the SetAlignmentXPos Functor.
     */
    void SetGraceAligmentXPos(Doc *doc);

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    /**
     * The stack of object where they are piled up before getting aligned
     */
    ArrayOfObjects m_graceStack;
    /**
     * The witdth of the group of grace notes instanciated after the bounding
     * boxes X are integrated in Alignment::IntegrateBoundingBoxGraceXShift
     */
    int m_totalWidth;
};

//----------------------------------------------------------------------------
// TimestampAligner
//----------------------------------------------------------------------------

/**
 * This class stores the timestamps (TimestampsAttr) in a measure.
 * It does not itself perform any alignment but only stores them and avoids duplicates
 * (i.e., having two objects at the same position.
 * It contains a vector of TimestampsAttr.
 */
class TimestampAligner : public Object {
public:
    // constructors and destructors
    TimestampAligner();
    virtual ~TimestampAligner();
    virtual ClassId GetClassId() const { return TIMESTAMP_ALIGNER; }

    /**
     * Reset the aligner (clear the content)
     */
    virtual void Reset();

    /**
     * Look for an existing TimestampAttr at a certain time.
     * Creates it if not found
     */
    TimestampAttr *GetTimestampAtTime(double time);

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
