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

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

/**
 * This class represents a staff in a laid-out score (Doc).
 * A Staff is contained in a System.
 * It contains Measure objects.
 * For unmeasured music, one single Measure is added for simplifying internal processing
 */
class Staff : public Object, public FacsimileInterface, public AttNInteger, public AttTyped, public AttVisibility {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Staff(int n = 1);
    virtual ~Staff();
    virtual Object *Clone() const { return new Staff(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Staff"; }
    virtual ClassId GetClassId() const { return STAFF; }
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    virtual void CloneReset();

    virtual FacsimileInterface *GetFacsimileInterface() { return dynamic_cast<FacsimileInterface *>(this); }

    /**
     * Return a const pointer to the children
     */
    virtual const ArrayOfObjects *GetChildren(bool docChildren = true) const;

    /**
     * Delete all the legder line arrays.
     */
    void ClearLedgerLines();

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
    ///@}

    /**
     * @name Get the X, Y, and angle of drawing position
     */
    ///@{
    virtual int GetDrawingY() const;
    virtual int GetDrawingX() const;
    virtual double GetDrawingRotate() const;
    ///@}

    /**
     * Adjust drawingStaffSize based on rotate angle
     */
    void AdjustDrawingStaffSize();

    /**
     * Check if the staff is currently visible.
     * Looks for the parent system and its current drawing scoreDef
     */
    bool DrawingIsVisible();

    /**
     * Return the index position of the staff in its measure parent
     */
    int GetStaffIdx() const { return Object::GetIdx(); }

    /**
     * Calculate the yRel for the staff given a @loc value
     */
    int CalcPitchPosYRel(Doc *doc, int loc);

    /**
     * Getter for the StaffAlignment
     */
    StaffAlignment *GetAlignment() const { return m_staffAlignment; }

    /**
     * Return the ledger line arrays (NULL if none)
     */
    ///@{
    ArrayOfLedgerLines *GetLedgerLinesAbove() { return m_ledgerLinesAbove; }
    ArrayOfLedgerLines *GetLedgerLinesAboveCue() { return m_ledgerLinesAboveCue; }
    ArrayOfLedgerLines *GetLedgerLinesBelow() { return m_ledgerLinesBelow; }
    ArrayOfLedgerLines *GetLedgerLinesBelowCue() { return m_ledgerLinesBelowCue; }
    ///@}

    /**
     * Add the ledger lines above or below.
     * If necessary creates the ledger line array.
     */
    ///@{
    void AddLedgerLineAbove(int count, int left, int right, bool cueSize);
    void AddLedgerLineBelow(int count, int left, int right, bool cueSize);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToCastOffMensural
     */
    virtual int ConvertToCastOffMensural(FunctorParams *params);

    /**
     * See Object::UnsetCurrentScoreDef
     */
    virtual int UnsetCurrentScoreDef(FunctorParams *functorParams);

    /**
     * See Object::OptimizeScoreDef
     */
    virtual int OptimizeScoreDef(FunctorParams *functorParams);

    /**
     * See Object::ResetVerticalAlignment
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
     * See Object::AlignVertically
     */
    virtual int AlignVertically(FunctorParams *functorParams);

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::PrepareRpt
     */
    virtual int PrepareRpt(FunctorParams *functorParams);

    /**
     * See Object::CalcOnsetOffset
     */
    ///@{
    virtual int CalcOnsetOffset(FunctorParams *functorParams);
    ///@}

    /**
     * Set staff parameters based on
     * facsimile information (if it
     * exists).
     */
    virtual void SetFromFacsimile(Doc *doc);

    /**
     * See Object::CalcStem
     */
    virtual int CalcStem(FunctorParams *);

    /**
     * See Object::AdjustSylSpacing
     */
    virtual int AdjustSylSpacing(FunctorParams *functorParams);

private:
    /**
     * Add the ledger line dashes to the legderline array.
     */
    void AddLedgerLines(ArrayOfLedgerLines *lines, int count, int left, int right);

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
    int m_yAbs;

    StaffDef *m_drawingStaffDef;

private:
    /**
     * A pointer to a StaffAlignment for aligning the staves
     */
    StaffAlignment *m_staffAlignment;

    /**
     * A pointer to the legder lines (above / below and normal / cue)
     */
    ///@{
    ArrayOfLedgerLines *m_ledgerLinesAbove;
    ArrayOfLedgerLines *m_ledgerLinesBelow;
    ArrayOfLedgerLines *m_ledgerLinesAboveCue;
    ArrayOfLedgerLines *m_ledgerLinesBelowCue;
    ///@}
};

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
     * Reset method reset all attribute classes
     */
    ///@{
    LedgerLine();
    virtual ~LedgerLine();
    virtual void Reset();
    ///@}

    /**
     * Add a dash to the ledger line object.
     * If necessary merges overlapping dashes.
     */
    void AddDash(int left, int right);

protected:
    //
private:
    //
public:
    /**
     * A list of dashes relative to the staff position.
     */
    std::list<std::pair<int, int> > m_dashes;

protected:
    //
private:
    //
};

} // namespace vrv

#endif
