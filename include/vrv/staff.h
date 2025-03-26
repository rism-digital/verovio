/////////////////////////////////////////////////////////////////////////////
// Name:        staff.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_STAFF_H__
#define __VRV_STAFF_H__

#include "atts_shared.h"
#include "facsimileinterface.h"
#include "object.h"

namespace vrv {

class DeviceContext;
class Layer;
class LedgerLine;
class StaffAlignment;
class StaffDef;
class Syl;
class TimeSpanningInterface;
class Tuning;

//----------------------------------------------------------------------------
// LedgerLine
//----------------------------------------------------------------------------

/**
 * This is a class with no MEI equivalent for representing legder lines.
 * A ledger line is represented by a list of dashes.
 * Each dash is represented by a pair of points (left - right).
 */
class LedgerLine {
public:
    /**
     * @name Constructors, destructors, reset methods
     */
    LedgerLine() = default;

    /**
     * Add a dash to the ledger line object.
     * If necessary merges overlapping dashes.
     */
    void AddDash(int left, int right, int extension, const Object *event);

    class Dash {
    public:
        int m_x1;
        int m_x2;
        ListOfConstObjects m_events;

        // Constructor
        Dash(int x1, int x2, const Object *object)
        {
            m_x1 = x1;
            m_x2 = x2;
            m_events.push_back(object);
        }

        // Merge function to merge another Dash object into this one
        void MergeWith(const Dash &other)
        {
            // Keep the first int from this Dash object, and the second int from the other
            this->m_x1 = std::min(other.m_x1, this->m_x1);
            this->m_x2 = std::max(other.m_x2, this->m_x2);
            // Append the list from other to this
            if (!other.m_events.empty()) {
                this->m_events.insert(this->m_events.end(), other.m_events.begin(), other.m_events.end());
            }
        }
    };

protected:
    //
private:
    //
public:
    /**
     * A list of dashes relative to the staff position.
     */
    // std::list<std::pair<int, int>> m_dashes;
    std::list<Dash> m_dashes;

protected:
    //
private:
    //
};

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

/**
 * This class represents a staff in a laid-out score (Doc).
 * A Staff is contained in a System.
 * It contains Measure objects.
 * For unmeasured music, one single Measure is added for simplifying internal processing
 */
class Staff : public Object,
              public FacsimileInterface,
              public AttCoordY1,
              public AttNInteger,
              public AttTyped,
              public AttVisibility {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Staff(int n = 1);
    virtual ~Staff();
    Object *Clone() const override { return new Staff(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "staff"; }
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
    ///@}

    /**
     * @name Getters and setters for the rotation.
     * Used only with facsimile rendering.
     */
    ///@{
    void SetDrawingRotation(double drawingRotation) { m_drawingRotation = drawingRotation; }
    double GetDrawingRotation() const { return m_drawingRotation; }
    bool HasDrawingRotation() const { return (m_drawingRotation != 0.0); }
    int GetDrawingRotationOffsetFor(int x);
    ///@}

    /**
     * Delete all the legder line arrays.
     */
    void ClearLedgerLines();

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * Additional check when adding a child.
     */
    bool AddChildAdditionalCheck(Object *child) override;

    /**
     * @name Get the X, Y, and angle of drawing position
     */
    ///@{
    int GetDrawingX() const override;
    int GetDrawingY() const override;
    virtual double GetDrawingRotate() const;
    ///@}

    /**
     * Adjust drawingStaffSize based on rotate angle
     */
    void AdjustDrawingStaffSize();

    /**
     * Return the drawing staff size for staff notation, including for tablature staves
     */
    int GetDrawingStaffNotationSize() const;

    /**
     * Check if the staff is currently visible.
     * Looks for the parent system and its current drawing scoreDef
     */
    bool DrawingIsVisible() const;

    /**
     * @name Get notation type
     */
    ///@{
    bool IsMensural() const;
    bool IsNeume() const;
    bool IsTablature() const;
    bool IsTabGuitar() const { return m_drawingNotationType == NOTATIONTYPE_tab_guitar; }
    bool IsTabLuteFrench() const { return m_drawingNotationType == NOTATIONTYPE_tab_lute_french; }
    bool IsTabLuteGerman() const { return m_drawingNotationType == NOTATIONTYPE_tab_lute_german; }
    bool IsTabLuteItalian() const { return m_drawingNotationType == NOTATIONTYPE_tab_lute_italian; }
    bool IsTabWithStemsOutside() const;
    ///@}

    /**
     * Return the index position of the staff in its measure parent
     */
    int GetStaffIdx() const { return Object::GetIdx(); }

    /**
     * Calculate the yRel for the staff given a \@loc value
     */
    int CalcPitchPosYRel(const Doc *doc, int loc) const;

    /**
     * Getter and setter for the StaffAlignment
     */
    ///@{
    StaffAlignment *GetAlignment() { return m_staffAlignment; }
    const StaffAlignment *GetAlignment() const { return m_staffAlignment; }
    void SetAlignment(StaffAlignment *alignment) { m_staffAlignment = alignment; }
    ///@}

    /**
     * Return the ledger line arrays
     */
    ///@{
    ArrayOfLedgerLines &GetLedgerLinesAbove() { return m_ledgerLinesAbove; }
    const ArrayOfLedgerLines &GetLedgerLinesAbove() const { return m_ledgerLinesAbove; }
    ArrayOfLedgerLines &GetLedgerLinesAboveCue() { return m_ledgerLinesAboveCue; }
    const ArrayOfLedgerLines &GetLedgerLinesAboveCue() const { return m_ledgerLinesAboveCue; }
    ArrayOfLedgerLines &GetLedgerLinesBelow() { return m_ledgerLinesBelow; }
    const ArrayOfLedgerLines &GetLedgerLinesBelow() const { return m_ledgerLinesBelow; }
    ArrayOfLedgerLines &GetLedgerLinesBelowCue() { return m_ledgerLinesBelowCue; }
    const ArrayOfLedgerLines &GetLedgerLinesBelowCue() const { return m_ledgerLinesBelowCue; }
    ///@}

    /**
     * Add the ledger lines above or below.
     * If necessary creates the ledger line array.
     */
    ///@{
    void AddLedgerLineAbove(int count, int left, int right, int extension, bool cueSize, const Object *event);
    void AddLedgerLineBelow(int count, int left, int right, int extension, bool cueSize, const Object *event);
    ///@}

    /**
     * Used for calculating note groups information/dot position.
     * The *Doc is the parent doc but passed as param in order to avoid look-up
     */
    bool IsOnStaffLine(int y, const Doc *doc) const;

    /**
     * Find the nearest unit position in the direction indicated by place.
     * The *Doc is the parent doc but passed as param in order to avoid look-up
     */
    int GetNearestInterStaffPosition(int y, const Doc *doc, data_STAFFREL place) const;

    /**
     * Set staff parameters based on
     * facsimile information (if it
     * exists).
     */
    void SetFromFacsimile(Doc *doc);

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
     * Add the ledger line dashes to the legderline array.
     */
    void AddLedgerLines(ArrayOfLedgerLines &lines, int count, int left, int right, int extension, const Object *event);

public:
    /**
     * Number of lines copied from the staffDef for fast access when drawing
     */
    int m_drawingLines;

    /**
     * Notation type (CMN, mensural, black mensural, etc.), from the staffDef
     */
    data_NOTATIONTYPE m_drawingNotationType;

    /**
     * The drawing staff size (scale), from the staffDef
     */
    int m_drawingStaffSize;

    /**
     * A vector of all the spanning elements overlapping with the previous measure
     */
    std::vector<Object *> m_timeSpanningElements;

    /**
     * The Y absolute position of the staff for facsimile (transcription) encodings.
     * This is the top left corner of the staff (the X position is the position of the system).
     */
    int m_drawingFacsY;

    StaffDef *m_drawingStaffDef;

    Tuning *m_drawingTuning;

private:
    /**
     * A pointer to a StaffAlignment for aligning the staves
     */
    StaffAlignment *m_staffAlignment;

    /**
     * The legder lines (above / below and normal / cue)
     */
    ///@{
    ArrayOfLedgerLines m_ledgerLinesAbove;
    ArrayOfLedgerLines m_ledgerLinesBelow;
    ArrayOfLedgerLines m_ledgerLinesAboveCue;
    ArrayOfLedgerLines m_ledgerLinesBelowCue;
    ///@}

    /**
     * The drawing rotation.
     * Used only with facsimile rendering
     */
    double m_drawingRotation;
};

} // namespace vrv

#endif
