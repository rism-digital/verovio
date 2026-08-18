/////////////////////////////////////////////////////////////////////////////
// Name:        savefunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "savefunctor.h"

//----------------------------------------------------------------------------

#include "accid.h"
#include "editorial.h"
#include "iobase.h"
#include "mdiv.h"
#include "mnum.h"
#include "mrest.h"
#include "rest.h"
#include "runningelement.h"
#include "text.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// SaveFunctor
//----------------------------------------------------------------------------

SaveFunctor::SaveFunctor(Output *output) : Functor()
{
    assert(output);

    m_output = output;
}

FunctorCode SaveFunctor::VisitAccid(Accid *accid)
{
    // Ignore accid within accid added for showHidden
    if (accid->GetParent() && accid->GetParent()->Is(ACCID)) return FUNCTOR_CONTINUE;

    return this->VisitLayerElement(accid);
}

FunctorCode SaveFunctor::VisitAccidEnd(Accid *accid)
{
    // Ignore accid within accid added for showHidden
    if (accid->GetParent() && accid->GetParent()->Is(ACCID)) return FUNCTOR_CONTINUE;

    return this->VisitLayerElementEnd(accid);
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
    // When writing MEI Basic, only visible elements within editorial markup are saved
    if (m_output->Skip(editorialElement)) return FUNCTOR_SIBLINGS;

    return this->VisitObject(editorialElement);
}

FunctorCode SaveFunctor::VisitEditorialElementEnd(EditorialElement *editorialElement)
{
    if (m_output->Skip(editorialElement)) return FUNCTOR_SIBLINGS;

    return this->VisitObjectEnd(editorialElement);
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
    // Can be skipped in MEI output
    if (m_output->Skip(mdiv)) return FUNCTOR_SIBLINGS;

    return this->VisitObject(mdiv);
}

FunctorCode SaveFunctor::VisitMdivEnd(Mdiv *mdiv)
{
    // Can be skipped in MEI output
    if (m_output->Skip(mdiv)) return FUNCTOR_SIBLINGS;

    return this->VisitObjectEnd(mdiv);
}

FunctorCode SaveFunctor::VisitMeasure(Measure *measure)
{
    return (measure->IsMeasuredMusic() || measure->IsNeumeLine()) ? this->VisitObject(measure) : FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMeasureEnd(Measure *measure)
{
    return (measure->IsMeasuredMusic() || measure->IsNeumeLine()) ? this->VisitObjectEnd(measure) : FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMNum(MNum *mNum)
{
    // Can be skipped in MEI output
    if (m_output->Skip(mNum)) return FUNCTOR_SIBLINGS;

    return this->VisitObject(mNum);
}

FunctorCode SaveFunctor::VisitMNumEnd(MNum *mNum)
{
    // Can be skipped in MEI output
    if (m_output->Skip(mNum)) return FUNCTOR_SIBLINGS;

    return this->VisitObjectEnd(mNum);
}

FunctorCode SaveFunctor::VisitMRest(MRest *mRest)
{
    // Ignore mRest within mSpace added for showHidden
    if (mRest->GetParent() && mRest->GetParent()->Is(MSPACE)) return FUNCTOR_CONTINUE;

    return this->VisitLayerElement(mRest);
}

FunctorCode SaveFunctor::VisitMRestEnd(MRest *mRest)
{
    // Ignore mRest within mSpace added for showHidden
    if (mRest->GetParent() && mRest->GetParent()->Is(MSPACE)) return FUNCTOR_CONTINUE;

    return this->VisitLayerElementEnd(mRest);
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
    // Can be skipped in MEI output
    if (m_output->Skip(runningElement)) return FUNCTOR_SIBLINGS;

    return this->VisitTextLayoutElement(runningElement);
}

FunctorCode SaveFunctor::VisitRunningElementEnd(RunningElement *runningElement)
{
    // Can be skipped in MEI output
    if (m_output->Skip(runningElement)) return FUNCTOR_SIBLINGS;

    return this->VisitTextLayoutElementEnd(runningElement);
}

FunctorCode SaveFunctor::VisitRest(Rest *rest)
{
    // Ignore rest within space added for showHidden
    if (rest->GetParent() && rest->GetParent()->Is(SPACE)) return FUNCTOR_CONTINUE;

    return this->VisitLayerElement(rest);
}

FunctorCode SaveFunctor::VisitRestEnd(Rest *rest)
{
    // Ignore rest within space added for showHidden
    if (rest->GetParent() && rest->GetParent()->Is(SPACE)) return FUNCTOR_CONTINUE;

    return this->VisitLayerElementEnd(rest);
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
