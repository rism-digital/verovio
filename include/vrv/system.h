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
    std::string GetClassName() const override { return "system"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
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
    void ResetDrawingAbbrLabelsWidth() { m_drawingAbbrLabelsWidth = 0; }
    ///@}

    /**
     * Return the height of the system
     */
    int GetHeight() const;

    /**
     * Return the top (first) visible staff in the measure (if any).
     * Takes into account system optimization
     */
    ///@{
    Staff *GetTopVisibleStaff();
    const Staff *GetTopVisibleStaff() const;
    ///@}

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
    void ResetDrawingScoreDef();
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
     * Reverse of ConvertToCastOffMensural()
     */
    void ConvertToUnCastOffMensuralSystem();

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
    int m_drawingFacsY;
    /**
     * The x absolute position of the  system for facsimile layouts.
     * This is the top left corner of the system.
     */
    int m_drawingFacsX;
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
