/////////////////////////////////////////////////////////////////////////////
// Name:        savefunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "savefunctor.h"

//----------------------------------------------------------------------------

#include "editorial.h"
#include "iomei.h"
#include "mdiv.h"
#include "mnum.h"
#include "runningelement.h"
#include "text.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// SaveFunctor
//----------------------------------------------------------------------------

SaveFunctor::SaveFunctor(Output *output, bool basic) : Functor()
{
    m_output = output;
    m_basic = basic;
}

FunctorCode SaveFunctor::VisitDots(Dots *dots)
{
    // Ignore dots during save
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitDotsEnd(Dots *dots)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    // When writing MEI basic, only visible elements within editorial markup are saved
    if (m_basic && (editorialElement->m_visibility == Hidden)) {
        return FUNCTOR_SIBLINGS;
    }
    else {
        return this->VisitObject(editorialElement);
    }
}

FunctorCode SaveFunctor::VisitEditorialElementEnd(EditorialElement *editorialElement)
{
    // Same as above
    if (m_basic && (editorialElement->m_visibility == Hidden)) {
        return FUNCTOR_SIBLINGS;
    }
    else {
        return this->VisitObjectEnd(editorialElement);
    }
}

FunctorCode SaveFunctor::VisitFlag(Flag *flag)
{
    // Ignore flags during save
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitFlagEnd(Flag *flag)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMdiv(Mdiv *mdiv)
{
    MEIOutput *meiOutput = dynamic_cast<MEIOutput *>(m_output);
    if ((mdiv->m_visibility == Hidden) && meiOutput) {
        // Do not output hidden mdivs in page-based MEI or when saving score-based MEI with filter
        if (!meiOutput->GetScoreBasedMEI() || meiOutput->HasFilter()) return FUNCTOR_SIBLINGS;
    }
    return this->VisitObject(mdiv);
}

FunctorCode SaveFunctor::VisitMdivEnd(Mdiv *mdiv)
{
    MEIOutput *meiOutput = dynamic_cast<MEIOutput *>(m_output);
    if ((mdiv->m_visibility == Hidden) && meiOutput) {
        // Do not output hidden mdivs in page-based MEI or when saving score-based MEI with filter
        if (!meiOutput->GetScoreBasedMEI() || meiOutput->HasFilter()) return FUNCTOR_SIBLINGS;
    }
    return this->VisitObjectEnd(mdiv);
}

FunctorCode SaveFunctor::VisitMeasure(Measure *measure)
{
    return (measure->IsMeasuredMusic()) ? this->VisitObject(measure) : FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMeasureEnd(Measure *measure)
{
    return (measure->IsMeasuredMusic()) ? this->VisitObjectEnd(measure) : FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMNum(MNum *mNum)
{
    return (mNum->IsGenerated()) ? FUNCTOR_SIBLINGS : this->VisitObject(mNum);
}

FunctorCode SaveFunctor::VisitMNumEnd(MNum *mNum)
{
    return (mNum->IsGenerated()) ? FUNCTOR_SIBLINGS : this->VisitObjectEnd(mNum);
}

FunctorCode SaveFunctor::VisitObject(Object *object)
{
    if (!m_output->WriteObject(object)) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitObjectEnd(Object *object)
{
    if (!m_output->WriteObjectEnd(object)) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitRunningElement(RunningElement *runningElement)
{
    if (runningElement->IsGenerated()) {
        return FUNCTOR_SIBLINGS;
    }
    else {
        return this->VisitTextLayoutElement(runningElement);
    }
}

FunctorCode SaveFunctor::VisitRunningElementEnd(RunningElement *runningElement)
{
    if (runningElement->IsGenerated()) {
        return FUNCTOR_SIBLINGS;
    }
    else {
        return this->VisitTextLayoutElementEnd(runningElement);
    }
}

FunctorCode SaveFunctor::VisitText(Text *text)
{
    if (text->IsGenerated()) {
        return FUNCTOR_SIBLINGS;
    }
    else {
        return this->VisitObject(text);
    }
}

FunctorCode SaveFunctor::VisitTextEnd(Text *text)
{
    if (text->IsGenerated()) {
        return FUNCTOR_SIBLINGS;
    }
    else {
        return this->VisitObjectEnd(text);
    }
}

FunctorCode SaveFunctor::VisitTupletBracket(TupletBracket *tupletBracket)
{
    // Ignore tuplet brackets during save
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitTupletBracketEnd(TupletBracket *tupletBracket)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitTupletNum(TupletNum *tupletNum)
{
    // Ignore tuplet numbers during save
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitTupletNumEnd(TupletNum *tupletNum)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
