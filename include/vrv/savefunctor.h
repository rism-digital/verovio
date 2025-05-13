/////////////////////////////////////////////////////////////////////////////
// Name:        savefunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SAVEFUNCTOR_H__
#define __VRV_SAVEFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// SaveFunctor
//----------------------------------------------------------------------------

/**
 * This class saves the content of any object by calling the appropriate FileOutputStream method.
 */
class SaveFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    SaveFunctor(Output *output);
    virtual ~SaveFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDots(Dots *dots) override;
    FunctorCode VisitDotsEnd(Dots *dots) override;
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitEditorialElementEnd(EditorialElement *editorialElement) override;
    FunctorCode VisitFlag(Flag *flag) override;
    FunctorCode VisitFlagEnd(Flag *flag) override;
    FunctorCode VisitMdiv(Mdiv *mdiv) override;
    FunctorCode VisitMdivEnd(Mdiv *mdiv) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitMNum(MNum *mNum) override;
    FunctorCode VisitMNumEnd(MNum *mNum) override;
    FunctorCode VisitObject(Object *object) override;
    FunctorCode VisitObjectEnd(Object *object) override;
    FunctorCode VisitRunningElement(RunningElement *runningElement) override;
    FunctorCode VisitRunningElementEnd(RunningElement *runningElement) override;
    FunctorCode VisitText(Text *text) override;
    FunctorCode VisitTextEnd(Text *text) override;
    FunctorCode VisitTupletBracket(TupletBracket *tupletBracket) override;
    FunctorCode VisitTupletBracketEnd(TupletBracket *tupletBracket) override;
    FunctorCode VisitTupletNum(TupletNum *tupletNum) override;
    FunctorCode VisitTupletNumEnd(TupletNum *tupletNum) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The output stream
    Output *m_output;
};

} // namespace vrv

#endif // __VRV_SAVEFUNCTOR_H__
