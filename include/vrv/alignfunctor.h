/////////////////////////////////////////////////////////////////////////////
// Name:        alignfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ALIGNFUNCTOR_H__
#define __VRV_ALIGNFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AlignHorizontallyFunctor
//----------------------------------------------------------------------------

/**
 * This class aligns horizontally the content of a page.
 * For each LayerElement, instanciate its Alignment.
 * It creates it if no other note or event occurs at its position.
 * At the end, for each Layer, align the grace note stacked in GraceAlignment.
 */
class AlignHorizontallyFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AlignHorizontallyFunctor(Doc *doc);
    virtual ~AlignHorizontallyFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitLayerEnd(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitMeterSigGrp(MeterSigGrp *meterSigGrp) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The measureAligner
    MeasureAligner *m_measureAligner;
    // The time
    double m_time;
    // The current Mensur
    Mensur *m_currentMensur;
    // The current MeterSig
    MeterSig *m_currentMeterSig;
    // The current notation type
    data_NOTATIONTYPE m_notationType;
    // Indicates the state in processing the caution scoreDef
    ElementScoreDefRole m_scoreDefRole;
    // Indicates if we are in the first measure (for the scoreDef role)
    bool m_isFirstMeasure;
    // Indicates if we have multiple layer alignment references in the measure
    bool m_hasMultipleLayer;
};

//----------------------------------------------------------------------------
// AlignMeasuresFunctor
//----------------------------------------------------------------------------

/**
 * This class aligns the measures by adjusting the m_drawingXRel position looking at the MeasureAligner.
 * At the end, store the width of the system in the MeasureAligner for justification.
 */
class AlignMeasuresFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AlignMeasuresFunctor(Doc *doc);
    virtual ~AlignMeasuresFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Set if castoff system widths should be stored
     */
    void StoreCastOffSystemWidths(bool storeWidths) { m_storeCastOffSystemWidths = storeWidths; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitSection(Section *section) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemEnd(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The cumulated shift
    int m_shift;
    // The cumulated justifiable width
    int m_justifiableWidth;
    // Shift next measure due to section restart?
    bool m_applySectionRestartShift;
    // Store castoff system widths if true
    bool m_storeCastOffSystemWidths;
};

} // namespace vrv

#endif // __VRV_ALIGNFUNCTOR_H__
