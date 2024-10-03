/////////////////////////////////////////////////////////////////////////////
// Name:        calcalignmentxposfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCALIGNMENTXPOSFUNCTOR_H__
#define __VRV_CALCALIGNMENTXPOSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcAlignmentXPosFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the position of the Alignment.
 * Looks at the time difference from the previous Alignment.
 */
class CalcAlignmentXPosFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcAlignmentXPosFunctor(Doc *doc);
    virtual ~CalcAlignmentXPosFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter and setter for the longest note duration
     */
    ///@{
    int GetLongestActualDur() const { return m_longestActualDur; }
    void SetLongestActualDur(data_DURATION dur) { m_longestActualDur = dur; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignment(Alignment *alignment) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureAligner(MeasureAligner *measureAligner) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The previous time position
    Fraction m_previousTime;
    // The previous x rel position
    int m_previousXRel;
    // Duration of the longest note
    data_DURATION m_longestActualDur;
    // The estimated justification ratio of the system
    double m_estimatedJustificationRatio;
    // The last alignment that was not timestamp-only
    Alignment *m_lastNonTimestamp;
    // The list of timestamp-only alignments that need to be adjusted
    std::list<Alignment *> m_timestamps;
    // The MeasureAligner
    MeasureAligner *m_measureAligner;
};

} // namespace vrv

#endif // __VRV_CALCALIGNMENTXPOSFUNCTOR_H__
