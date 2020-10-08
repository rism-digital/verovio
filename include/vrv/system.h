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

class BoundaryEnd;
class DeviceContext;
class Ending;
class Measure;
class ScoreDef;
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
    virtual void Reset();
    virtual std::string GetClassName() const { return "System"; }
    virtual ClassId GetClassId() const { return SYSTEM; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
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
     * @name Set and get the labels drawing width (normal and abbreviated)
     */
    ///@{
    int GetDrawingLabelsWidth() const { return m_drawingLabelsWidth; }
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
    ScoreDef *GetDrawingScoreDef() const { return m_drawingScoreDef; }
    void SetDrawingScoreDef(ScoreDef *drawingScoreDef);
    ///@}

    /**
     * Check if the notes between the start and end have mixed drawing stem directions.
     * The start and end element are expected to be on the same staff and same layer.
     */
    bool HasMixedDrawingStemDir(LayerElement *start, LayerElement *end);

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
    void AddToDrawingListIfNeccessary(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::UnsetCurrentScoreDef
     */
    virtual int UnsetCurrentScoreDef(FunctorParams *functorParams);

    /**
     * See Object::OptimizeScoreDef
     */
    ///@{
    virtual int OptimizeScoreDef(FunctorParams *functorParams);
    virtual int OptimizeScoreDefEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

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
     * See Object::AdjustXOverflow
     */
    ///@{
    virtual int AdjustXOverflow(FunctorParams *functorParams);
    virtual int AdjustXOverflowEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AdjustHarmGrpsSpacing
     */
    ///@{
    virtual int AdjustHarmGrpsSpacing(FunctorParams *functorParams);
    virtual int AdjustHarmGrpsSpacingEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AdjustSylSpacing
     */
    ///@{
    virtual int AdjustSylSpacing(FunctorParams *functorParams);
    virtual int AdjustSylSpacingEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AlignVertically
     */
    ///@{
    virtual int AlignVertically(FunctorParams *functorParams);
    virtual int AlignVerticallyEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AdjustYPos
     */
    virtual int AdjustYPos(FunctorParams *functorParams);

    /**
     * See Object::AlignSystems
     */
    virtual int AlignSystems(FunctorParams *functorParams);

    /**
     * See Object::AlignMeasures
     */
    virtual int AlignMeasures(FunctorParams *functorParams);
    virtual int AlignMeasuresEnd(FunctorParams *functorParams);

    /**
     * See Object::JustifyX
     */
    virtual int JustifyX(FunctorParams *functorParams);

    /**
     * See Object::JustifyY
     */
    virtual int JustifyY(FunctorParams *functorParams);

    /**
     * See Object::AdjustStaffOverlap
     */
    virtual int AdjustStaffOverlap(FunctorParams *functorParams);

    /**
     * See Object::AdjustFloatingPositioners
     */
    virtual int AdjustFloatingPositioners(FunctorParams *functorParams);

    /**
     * See Object::AdjustFloatingPositionersBetween
     */
    virtual int AdjustFloatingPositionersBetween(FunctorParams *functorParams);

    /**
     * See Object::AdjustSlurs
     */
    virtual int AdjustSlurs(FunctorParams *functorParams);

    /**
     * See Object::CastOffPages
     */
    virtual int CastOffPages(FunctorParams *functorParams);

    /**
     * See Object::UnCastOff
     */
    virtual int UnCastOff(FunctorParams *functorParams);

    /**
     * See Object::CastOffSystemsEnd
     */
    virtual int CastOffSystemsEnd(FunctorParams *functorParams);

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
     * The width used by the labels at the left of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingLabelsWidth;
    /** The width used by the abbreviated labels */
    int m_drawingAbbrLabelsWidth;
    /**
     * @name The total width of the system.
     * It is computed during the layout processed and used for calculating the justification ratio.
     */
    ///@{
    int m_drawingTotalWidth;
    int m_drawingJustifiableWidth;
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
