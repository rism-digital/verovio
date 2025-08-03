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
// AlignMeterParams
//----------------------------------------------------------------------------
/**
 * Regroup pointers to meterSig, mensur and proport objects
 */
struct AlignMeterParams {
    const MeterSig *meterSig = NULL;
    const Mensur *mensur = NULL;
    // Not const since we are cumulating proportion
    Proport *proport = NULL;
    data_DURATION equivalence = DURATION_brevis;
    bool metcon = true;
};

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
    Fraction m_time;
    // The current MeterSig, Mensur and Proport
    AlignMeterParams m_currentParams;
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
    FunctorCode VisitDiv(Div *div) override;
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

//----------------------------------------------------------------------------
// AlignVerticallyFunctor
//----------------------------------------------------------------------------

/**
 * This class vertically aligns the content of a page.
 * For each staff instanciate its StaffAlignment.
 */
class AlignVerticallyFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AlignVerticallyFunctor(Doc *doc);
    virtual ~AlignVerticallyFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDiv(Div *div) override;
    FunctorCode VisitFig(Fig *fig) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPageEnd(Page *page) override;
    FunctorCode VisitRend(Rend *rend) override;
    FunctorCode VisitRunningElement(RunningElement *runningElement) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitStaffAlignmentEnd(StaffAlignment *staffAlignment) override;
    FunctorCode VisitSyllable(Syllable *syllable) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemEnd(System *system) override;
    FunctorCode VisitVerse(Verse *verse) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The systemAligner
    SystemAligner *m_systemAligner;
    // The staff index
    int m_staffIdx;
    // The staffN
    int m_staffN;
    // The cumulated shift for the default alignment
    int m_cumulatedShift;
    // The sum of justification factors per page
    int m_justificationSum;
    // The page width - used for setting the horizontal position of <rend> withing text layout elements
    int m_pageWidth;
};

//----------------------------------------------------------------------------
// AlignSystemsFunctor
//----------------------------------------------------------------------------

/**
 * This class aligns the system by adjusting the m_drawingYRel position looking at the SystemAligner.
 */
class AlignSystemsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AlignSystemsFunctor(Doc *doc);
    virtual ~AlignSystemsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Setter for shift and spacing
     */
    ///@{
    void SetShift(int shift) { m_shift = shift; }
    void SetSystemSpacing(int spacing) { m_systemSpacing = spacing; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitPageEnd(Page *page) override;
    FunctorCode VisitSystem(System *system) override;
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
    // The system margin
    int m_systemSpacing;
    // The (clef) overflow below of the previous system
    int m_prevBottomOverflow;
    int m_prevBottomClefOverflow;
    // The sum of justification factors per page
    double m_justificationSum;
};

} // namespace vrv

#endif // __VRV_ALIGNFUNCTOR_H__
