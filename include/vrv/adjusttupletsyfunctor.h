/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttupletsyfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTTUPLETSYFUNCTOR_H__
#define __VRV_ADJUSTTUPLETSYFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTupletsYFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the Y position of tuplet brackets and num.
 */
class AdjustTupletsYFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustTupletsYFunctor(Doc *doc);
    virtual ~AdjustTupletsYFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    ///@}

protected:
    //
private:
    /**
     * Adjust tuplet relative positioning based on possible overlaps
     */
    ///@{
    void AdjustTupletBracketY(Tuplet *tuplet, const Staff *staff) const;
    void AdjustTupletNumY(Tuplet *tuplet, const Staff *staff) const;
    ///@}

    /**
     * Adjust tuplet relative positioning based on beams
     */
    void AdjustTupletBracketBeamY(Tuplet *tuplet, TupletBracket *bracket, const Beam *beam, const Staff *staff) const;

    // Calculate the vertical bracket adjustment based on a list of point obstacles
    int CalcBracketShift(Point referencePos, double slope, int sign, const std::list<Point> &obstacles) const;

public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// AdjustTupletNumOverlapFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the Y relative position of tupletNum based on overlaps with other elements.
 */
class AdjustTupletNumOverlapFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustTupletNumOverlapFunctor(
        const TupletNum *tupletNum, const Staff *staff, data_STAFFREL_basic drawingNumPos, int drawingY);
    virtual ~AdjustTupletNumOverlapFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Setter for the margins
     */
    ///@{
    void SetHorizontalMargin(int margin) { m_horizontalMargin = margin; }
    void SetVerticalMargin(int margin) { m_verticalMargin = margin; }
    ///@}

    /*
     * Getter for the drawing Y
     */
    int GetDrawingY() const { return m_drawingY; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(const LayerElement *layerElement) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The tupletNum for which the relative position is calculated
    const TupletNum *m_tupletNum;
    // The drawing position of tupletNum
    data_STAFFREL_basic m_drawingNumPos;
    // The margins for tupletNum overlap
    int m_horizontalMargin;
    int m_verticalMargin;
    // The staff relevant for positioning the tuplet
    const Staff *m_staff;
    // The drawing Y position
    int m_drawingY;
};

//----------------------------------------------------------------------------
// AdjustTupletWithSlursFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the Y position of tuplets with inner slurs.
 */
class AdjustTupletWithSlursFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustTupletWithSlursFunctor(Doc *doc);
    virtual ~AdjustTupletWithSlursFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_ADJUSTTUPLETSYFUNCTOR_H__
