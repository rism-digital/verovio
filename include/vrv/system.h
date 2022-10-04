/////////////////////////////////////////////////////////////////////////////
// Name:        system.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYSTEM_H__
#define __VRV_SYSTEM_H__

#include "drawinginterface.h"
#include "editorial.h"
#include "object.h"
#include "verticalaligner.h"
#include "vrvdef.h"

namespace vrv {

class SystemMilestoneEnd;
class DeviceContext;
class Ending;
class Measure;
class ScoreDef;
class Slur;
class Staff;

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

/**
 * This class represents a system in a laid-out score (Doc).
 * A System is contained in a Page.
 * It contains Staff objects.
 */
class System : public Object, public DrawingListInterface, public AttTyped {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    System();
    virtual ~System();
    void Reset() override;
    std::string GetClassName() const override { return "System"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
    ///@}

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
     * @name Set and get the labels drawing width (normal and abbreviated)
     */
    ///@{
    int GetDrawingLabelsWidth() const;
    void SetDrawingLabelsWidth(int width);
    int GetDrawingAbbrLabelsWidth() const { return m_drawingAbbrLabelsWidth; }
    void SetDrawingAbbrLabelsWidth(int width);
    ///@}

    /**
     * Return the height of the system
     */
    int GetHeight() const;

    /**
     * Return the minimus system spacing
     */
    int GetMinimumSystemSpacing(const Doc *doc) const;

    /**
     * Return the index position of the system in its page parent
     */
    int GetSystemIdx() const { return Object::GetIdx(); }

    bool SetCurrentFloatingPositioner(
        int staffN, FloatingObject *object, Object *objectX, Object *objectY, char spanningType = SPANNING_START_END);

    /**
     * @name Setter and getter of the drawing scoreDef
     */
    ///@{
    ScoreDef *GetDrawingScoreDef() { return m_drawingScoreDef; }
    const ScoreDef *GetDrawingScoreDef() const { return m_drawingScoreDef; }
    void SetDrawingScoreDef(ScoreDef *drawingScoreDef);
    ///@}

    /**
     * Check if the notes between the start and end have mixed drawing stem directions.
     * The start and end element are expected to be on the same staff and same layer.
     */
    bool HasMixedDrawingStemDir(const LayerElement *start, const LayerElement *end) const;

    /**
     * Get preferred curve direction based on the starting and ending point of the slur
     */
    curvature_CURVEDIR GetPreferredCurveDirection(
        const LayerElement *start, const LayerElement *end, const Slur *slur) const;

    /**
     * @name Setter and getter of the drawing visible flag
     */
    ///@{
    bool IsDrawingOptimized() const { return m_drawingIsOptimized; }
    void IsDrawingOptimized(bool drawingIsOptimized) { m_drawingIsOptimized = drawingIsOptimized; }
    ///@}

    /**
     * Add an object to the drawing list but only if necessary.
     * Check types but also links (dynam, dir) and extensions (trill).
     */
    void AddToDrawingListIfNecessary(Object *object);

    /**
     * @name Check if the system is the first or last in page or of a selection or of an mdiv by looking at the next
     * sibling
     */
    ///@{
    bool IsFirstInPage() const;
    bool IsLastInPage() const;
    bool IsFirstOfMdiv() const;
    bool IsLastOfMdiv() const;
    bool IsFirstOfSelection() const;
    bool IsLastOfSelection() const;
    ///@}

    /**
     * Estimate the justification ratio from the castoff system widths and the desired page width
     */
    double EstimateJustificationRatio(const Doc *doc) const;

    /**
     * Convert mensural MEI into cast-off (measure) segments looking at the barLine objects.
     * Segment positions occur where a barLine is set on all staves.
     */
    void ConvertToCastOffMensuralSystem(Doc *doc, System *targetSystem);

    /**
     * Reverse of ConvertToCastOffMensural()
     */
    void ConvertToUnCastOffMensuralSystem();

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::UnscoreDefSetCurrent
     */
    int ScoreDefUnsetCurrent(FunctorParams *functorParams) override;

    /**
     * @name See Object::ScoreDefOptimize
     */
    ///@{
    int ScoreDefOptimize(FunctorParams *functorParams) override;
    int ScoreDefOptimizeEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::ScoreDefSetGrpSym
     */
    int ScoreDefSetGrpSym(FunctorParams *functorParams) override;

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
     * See Object::CalcAlignmentXPos
     */
    int CalcAlignmentXPos(FunctorParams *functorParams) override;

    /**
     * @name See Object::AdjustXOverflow
     */
    ///@{
    int AdjustXOverflow(FunctorParams *functorParams) override;
    int AdjustXOverflowEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * @name See Object::AdjustHarmGrpsSpacing
     */
    ///@{
    int AdjustHarmGrpsSpacing(FunctorParams *functorParams) override;
    int AdjustHarmGrpsSpacingEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * @name See Object::AdjustSylSpacing
     */
    ///@{
    int AdjustSylSpacing(FunctorParams *functorParams) override;
    int AdjustSylSpacingEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::AdjustTempo
     */
    int AdjustTempo(FunctorParams *functorParams) override;

    /**
     * @name See Object::AlignVertically
     */
    ///@{
    int AlignVertically(FunctorParams *functorParams) override;
    int AlignVerticallyEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::AdjustYPos
     */
    int AdjustYPos(FunctorParams *functorParams) override;

    /**
     * See Object::AlignSystems
     */
    int AlignSystems(FunctorParams *functorParams) override;

    /**
     * @name See Object::AlignMeasures
     */
    ///@{
    int AlignMeasures(FunctorParams *functorParams) override;
    int AlignMeasuresEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::JustifyX
     */
    int JustifyX(FunctorParams *functorParams) override;

    /**
     * See Object::JustifyY
     */
    int JustifyY(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustCrossStaffYPos
     */
    int AdjustCrossStaffYPos(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustStaffOverlap
     */
    int AdjustStaffOverlap(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustFloatingPositioners
     */
    int AdjustFloatingPositioners(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustFloatingPositionersBetween
     */
    int AdjustFloatingPositionersBetween(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustSlurs
     */
    int AdjustSlurs(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffPages
     */
    int CastOffPages(FunctorParams *functorParams) override;

    /**
     * @name See Object::CastOffSystems
     */
    ///@{
    int CastOffSystems(FunctorParams *functorParams) override;
    int CastOffSystemsEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::CastOffEncoding
     */
    int CastOffEncoding(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffToSelection
     */
    int CastOffToSelection(FunctorParams *) override;

    /**
     * See Object::UnCastOff
     */
    int UnCastOff(FunctorParams *functorParams) override;

    /**
     * See Object::Transpose
     */
    int Transpose(FunctorParams *functorParams) override;

public:
    SystemAligner m_systemAligner;

public:
    /** System left margin (MEI scoredef@system.leftmar). Saved if != 0 */
    int m_systemLeftMar;
    /** System right margin (MEI scoredef@system.rightmar). Saved if != 0 */
    int m_systemRightMar;
    /**
     * The Y absolute position of the staff for facsimile (transcription) encodings.
     * This is the top left corner of the system.
     */
    int m_yAbs;
    /**
     * The x absolute position of the  system for facsimile layouts.
     * This is the top left corner of the system.
     */
    int m_xAbs;
    /**
     * The width used by the abbreviated labels at the left of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingAbbrLabelsWidth;
    /**
     * @name The total width of the system.
     * It is computed during the layout processed and used for calculating the justification ratio.
     */
    ///@{
    int m_drawingTotalWidth;
    int m_drawingJustifiableWidth;
    ///@}
    /**
     * @name The cast off width of the system.
     * It is computed during castoff and used for adjusting the horizontal spacing for a given duration.
     * This technique prevents large justification ratios and improves the horizontal layout.
     */
    ///@{
    int m_castOffTotalWidth;
    int m_castOffJustifiableWidth;
    ///@}

protected:
    /**
     * The X relative position of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingXRel;
    /**
     * The Y relative position of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingYRel;

private:
    /**
     * The drawing scoreDef at the beginning of the system.
     */
    ScoreDef *m_drawingScoreDef;

    /**
     * A flag indicating if the system is optimized.
     * This does not mean that a staff is hidden, but only that it can be optimized.
     */
    bool m_drawingIsOptimized;
};

} // namespace vrv

#endif
