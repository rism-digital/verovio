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
    std::string GetClassName() const override { return "Staff"; }
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
     * Delete all the legder line arrays.
     */
    void ClearLedgerLines();

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
    ///@}

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
     * Calculate the yRel for the staff given a @loc value
     */
    int CalcPitchPosYRel(const Doc *doc, int loc) const;

    /**
     * Getter for the StaffAlignment
     */
    ///@{
    StaffAlignment *GetAlignment() { return m_staffAlignment; }
    const StaffAlignment *GetAlignment() const { return m_staffAlignment; }
    ///@}

    /**
     * Return the ledger line arrays
     */
    ///@{
    const ArrayOfLedgerLines &GetLedgerLinesAbove() const { return m_ledgerLinesAbove; }
    const ArrayOfLedgerLines &GetLedgerLinesAboveCue() const { return m_ledgerLinesAboveCue; }
    const ArrayOfLedgerLines &GetLedgerLinesBelow() const { return m_ledgerLinesBelow; }
    const ArrayOfLedgerLines &GetLedgerLinesBelowCue() const { return m_ledgerLinesBelowCue; }
    ///@}

    /**
     * Add the ledger lines above or below.
     * If necessary creates the ledger line array.
     */
    ///@{
    void AddLedgerLineAbove(int count, int left, int right, int extension, bool cueSize);
    void AddLedgerLineBelow(int count, int left, int right, int extension, bool cueSize);
    ///@}

    /**
     * Used for calculating clustered information/dot position.
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
     * See Object::ConvertToCastOffMensural
     */
    int ConvertToCastOffMensural(FunctorParams *functorParams) override;

    /**
     * See Object::UnscoreDefSetCurrent
     */
    int ScoreDefUnsetCurrent(FunctorParams *functorParams) override;

    /**
     * See Object::ScoreDefOptimize
     */
    int ScoreDefOptimize(FunctorParams *functorParams) override;

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
     * See Object::AlignVertically
     */
    int AlignVertically(FunctorParams *functorParams) override;

    /**
     * See Object::CalcLedgerLinesEnd
     */
    int CalcLedgerLinesEnd(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareStaffCurrentTimeSpanning
     */
    int PrepareStaffCurrentTimeSpanning(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffEncoding
     */
    int CastOffEncoding(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareRpt
     */
    int PrepareRpt(FunctorParams *functorParams) override;

    /**
     * See Object::InitOnsetOffset
     */
    ///@{
    int InitOnsetOffset(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustSylSpacing
     */
    int AdjustSylSpacing(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateMIDI
     */
    int GenerateMIDI(FunctorParams *functorParams) override;

    /**
     * See Object::Transpose
     */
    int Transpose(FunctorParams *functorParams) override;

private:
    /**
     * Add the ledger line dashes to the legderline array.
     */
    void AddLedgerLines(ArrayOfLedgerLines &lines, int count, int left, int right, int extension);

    /**
     * Shorten ledger lines which overlap with neighbors
     */
    void AdjustLedgerLines(
        ArrayOfLedgerLines &lines, ArrayOfLedgerLines &cueLines, double cueScaling, int extension, int minExtension);

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
    void AddDash(int left, int right, int extension);

protected:
    //
private:
    //
public:
    /**
     * A list of dashes relative to the staff position.
     */
    std::list<std::pair<int, int>> m_dashes;

protected:
    //
private:
    //
};

} // namespace vrv

#endif
