/////////////////////////////////////////////////////////////////////////////
// Name:        justifyfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_JUSTIFYFUNCTOR_H__
#define __VRV_JUSTIFYFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// JustifyXFunctor
//----------------------------------------------------------------------------

/**
 * This class justifies the X positions.
 */
class JustifyXFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    JustifyXFunctor(Doc *doc);
    virtual ~JustifyXFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Set the full system width
     */
    void SetSystemFullWidth(int width) { m_systemFullWidth = width; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignment(Alignment *alignment) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureAligner(MeasureAligner *measureAligner) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitSection(Section *section) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The relative X position of the next measure
    int m_measureXRel;
    // The justification ratio
    double m_justifiableRatio;
    // The left/right barline X position
    int m_leftBarLineX;
    int m_rightBarLineX;
    // The system full width (without system margins)
    int m_systemFullWidth;
    // Indicates shift of next measure due to section restart
    bool m_applySectionRestartShift;
};

} // namespace vrv

#endif // __VRV_JUSTIFYFUNCTOR_H__
