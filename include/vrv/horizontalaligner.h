/////////////////////////////////////////////////////////////////////////////
// Name:        horizontalaligner.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_HORIZONTAL_ALIGNER_H__
#define __VRV_HORIZONTAL_ALIGNER_H__

#include "atts_shared.h"
#include "fraction.h"
#include "object.h"
#include "vrv.h"

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
    ALIGNMENT_PROPORT,
    ALIGNMENT_DOT,
    ALIGNMENT_CUSTOS,
    ALIGNMENT_ACCID,
    ALIGNMENT_GRACENOTE,
    ALIGNMENT_BARLINE,
    ALIGNMENT_DIVLINE,
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
 * This class stores an alignment position elements will point to
 */
class Alignment : public Object {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    Alignment();
    Alignment(const Fraction &time, AlignmentType type = ALIGNMENT_DEFAULT);
    virtual ~Alignment();
    void Reset() override;
    ///@}

    /**
     * Delete the grace aligners in the map
     */
    void ClearGraceAligners();

    /**
     * Override the method of adding AlignmentReference children
     */
    bool IsSupportedChild(ClassId classId) override;

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
    void SetTime(const Fraction &time) { m_time = time; }
    Fraction GetTime() const { return m_time; }
    ///@}

    /**
     * @name Weak ordering: for alignments in the same measure it is based on time, otherwise on the measure order
     */
    ///@{
    bool operator==(const Alignment &other) const;
    std::weak_ordering operator<=>(const Alignment &other) const;
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
    bool IsOfType(const std::vector<AlignmentType> &types) const;

    /**
     * Retrieve the minimum left and maximum right position for the objects in an alignment.
     * Returns (-)VRV_UNSET if there is nothing for the staff specified.
     * Internally uses GetAlignmentLeftRightFunctor
     */
    void GetLeftRight(
        const std::vector<int> &staffNs, int &minLeft, int &maxRight, const std::vector<ClassId> &excludes = {}) const;
    void GetLeftRight(int staffN, int &minLeft, int &maxRight, const std::vector<ClassId> &excludes = {}) const;

    /**
     * Return all GraceAligners for the Alignment.
     */
    const MapOfIntGraceAligners &GetGraceAligners() { return m_graceAligners; }

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
     * Returns true for Alignment for which we want to do bounding box alignment
     */
    bool PerformBoundingBoxAlignment() const;

    /**
     * Return the AlignmentReference holding the element.
     * If staffN is provided, uses the AlignmentReference->GetN() to accelerate the search.
     */
    ///@{
    AlignmentReference *GetReferenceWithElement(const LayerElement *element, int staffN = VRV_UNSET);
    const AlignmentReference *GetReferenceWithElement(const LayerElement *element, int staffN = VRV_UNSET) const;
    ///@}

    /**
     * Return pair of max and min Y value within alignment. Elements will be counted by alignment references.
     */
    std::pair<int, int> GetAlignmentTopBottom() const;

    /**
     * Return true if there is vertical overlap with accidentals from another alignment for specific staffN
     */
    bool HasAccidVerticalOverlap(const Alignment *otherAlignment, int staffN) const;

    /**
     * Return true if the alignment contains at least one reference with staffN
     */
    bool HasAlignmentReference(int staffN) const;

    /**
     * Return true if the alignment contains only references to timestamp attributes.
     */
    bool HasTimestampOnly() const;

    /**
     * Debug message
     */
    std::string LogDebugTreeMsg() override
    {
        return StringFormat("%d %f", this->GetXRel(), this->GetTime().ToDouble());
    }

    //----------------//
    // Static methods //
    //----------------//

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
    static int HorizontalSpaceForDuration(
        const Fraction &intervalTime, data_DURATION maxActualDur, double spacingLinear, double spacingNonLinear);

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
     * Retrieve the AlignmentReference with staffN.
     * Create and add it as child if not found.
     */
    AlignmentReference *GetAlignmentReference(int staffN);

public:
    //
private:
    /**
     * Stores the position relative to the measure.
     * This is instanciated by the CalcAlignmentXPosFunctor.
     * It takes into account a non-linear according to the time interval with
     * the previous Alignement
     */
    int m_xRel;
    /**
     * Stores the time at which the alignment occur.
     * It is set by the AlignHorizontallyFunctor.
     */
    Fraction m_time;
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
 * The staff identification (@n) is given by the attCommon and takes into account
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
    void Reset() override;
    ///@}

    /**
     * Override the method of adding Alignment children
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Overwritten method for AlignmentReference children
     */
    void AddChild(Object *object) override;

    /**
     * Return true if one of objects overlaps with accidentals from current reference (i.e. if there are accidentals)
     */
    bool HasAccidVerticalOverlap(const ArrayOfConstObjects &objects) const;

    /**
     * Return true if the reference has elements from multiple layers.
     */
    bool HasMultipleLayer() const { return (m_layerCount > 1); }

    /**
     * Return true if the reference has elements from cross-staff.
     */
    bool HasCrossStaffElements() const;

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
public:
    //
private:
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
    HorizontalAligner(ClassId classId);
    virtual ~HorizontalAligner();
    void Reset() override;
    ///@}

    /**
     * Do not copy children for HorizontalAligner
     */
    bool CopyChildren() const override { return false; }

    int GetAlignmentCount() const { return this->GetChildCount(); }

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

protected:
    /**
     * Search if an alignment of the type is already there at the time.
     * If not, return in idx the position where it needs to be inserted (-1 if it is the end)
     */
    ///@{
    Alignment *SearchAlignmentAtTime(const Fraction &time, AlignmentType type, int &idx);
    const Alignment *SearchAlignmentAtTime(const Fraction &time, AlignmentType type, int &idx) const;
    ///@}

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
    ///@{
    MeasureAligner();
    virtual ~MeasureAligner();
    void Reset() override;
    ///@}

    /**
     * Override the method of adding AlignmentReference children
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Retrieve the alignmnet of the type at that time.
     * The alignment object is added if not found.
     * The maximum time position is also adjusted accordingly for end barline positioning
     */
    Alignment *GetAlignmentAtTime(const Fraction &time, AlignmentType type);

    /**
     * Keep the maximum time of the measure.
     * This corresponds to the whole duration of the measure and
     * should be the same for all staves/layers.
     */
    void SetMaxTime(const Fraction &time);

    /**
     * Return the max time of the measure (i.e., the right measure alignment time)
     */
    Fraction GetMaxTime() const;

    /**
     * @name Set and Get the non-justifiable margin (right and left scoreDefs)
     */
    ///@{
    int GetNonJustifiableMargin() const { return m_nonJustifiableLeftMargin; }
    ///@}

    /**
     * @name Set and Get the initial tstamp duration.
     * Setter takes a meter unit parameter.
     */
    ///@{
    void SetInitialTstamp(data_DURATION meterUnit);
    Fraction GetInitialTstampDur() const { return m_initialTstampDur; }
    ///@}

    /**
     * Get left Alignment for the measure and for the left BarLine.
     * For each MeasureAligner, we keep and Alignment for the left position.
     * The Alignment time will be always -1.0 and will appear first in the list.
     */
    ///@{
    Alignment *GetLeftAlignment() { return m_leftAlignment; }
    const Alignment *GetLeftAlignment() const { return m_leftAlignment; }
    Alignment *GetLeftBarLineAlignment() { return m_leftBarLineAlignment; }
    const Alignment *GetLeftBarLineAlignment() const { return m_leftBarLineAlignment; }
    ///@}

    /**
     * Get right Alignment for the measure.
     * For each MeasureAligner, we keep and Alignment for the right position.
     * The Alignment time will be increased whenever necessary when values are added.
     */
    ///@{
    Alignment *GetRightAlignment() { return m_rightAlignment; }
    const Alignment *GetRightAlignment() const { return m_rightAlignment; }
    Alignment *GetRightBarLineAlignment() { return m_rightBarLineAlignment; }
    const Alignment *GetRightBarLineAlignment() const { return m_rightBarLineAlignment; }
    ///@}

    /**
     * Adjust the spacing of the measure looking at each tuple of start / end alignment and a distance.
     * The distance is an expansion value (positive) of compression (negative).
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
    void AdjustGraceNoteSpacing(const Doc *doc, Alignment *alignment, int staffN);

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

    /**
     * The time duration of the timestamp between 0.0 and 1.0.
     * This depends on the meter signature in the preceeding scoreDef
     */
    Fraction m_initialTstampDur;
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
    void Reset() override;
    ///@}

    /**
     * Retrieve the alignmnet of the type at that time.
     * The alignment object is added if not found.
     */
    Alignment *GetAlignmentAtTime(const Fraction &time, AlignmentType type);

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
    int GetGraceGroupLeft(int staffN) const;
    int GetGraceGroupRight(int staffN) const;
    ///@{

    /**
     * Set an linear defaut position for each grace note
     * This is called from the CalcAlignmentXPosFunctor.
     */
    void SetGraceAlignmentXPos(const Doc *doc);

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

    /**
     * Reset the aligner (clear the content)
     */
    void Reset() override;

    /**
     * Override the method of adding TimestampAttr children
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Look for an existing TimestampAttr at a certain time.
     * Creates it if not found
     */
    TimestampAttr *GetTimestampAtTime(double time);

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
public:
    //
private:
    //
};

} // namespace vrv

#endif
