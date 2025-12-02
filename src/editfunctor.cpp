/////////////////////////////////////////////////////////////////////////////
// Name:        editfunctor.cpp
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editfunctor.h"

//----------------------------------------------------------------------------

#include "editorial.h"
#include "editortoolkit_cmn.h"
#include "score.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// StructFunctor
//----------------------------------------------------------------------------

SectionContextFunctor::SectionContextFunctor(Object *object) : Functor()
{
    m_current = object;
}

FunctorCode SectionContextFunctor::VisitObject(Object *object)
{
    // In any case do not go beyond these
    if (object->GetParent() && object->GetParent()->Is({ DIV, MEASURE, SCOREDEF })) {
        return FUNCTOR_SIBLINGS;
    }

    if (object->Is({ DOC, PAGE, PAGES, PAGE_MILESTONE_END, SYSTEM, SYSTEM_MILESTONE_END })) {
        return FUNCTOR_CONTINUE;
    }

    bool ownChildren = false;
    if (object->Is({ DIV, MEASURE, SCOREDEF })) {
        ownChildren = true;
    }

    EditorTreeObject *treeObject = new EditorTreeObject(object, ownChildren);
    m_current->AddChild(treeObject);
    m_current = treeObject;

    return FUNCTOR_CONTINUE;
}

FunctorCode SectionContextFunctor::VisitObjectEnd(Object *object)
{
    if (object->IsMilestoneElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (object->Is({ PAGE_MILESTONE_END, SYSTEM_MILESTONE_END })) {
        m_current = m_current->GetParent();
    }
    if (object->Is({ DOC, PAGE, PAGES, PAGE_MILESTONE_END, SYSTEM, SYSTEM_MILESTONE_END })) {
        return FUNCTOR_CONTINUE;
    }

    m_current = m_current->GetParent();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ScoreContextFunctor
//----------------------------------------------------------------------------

ScoreContextFunctor::ScoreContextFunctor(Object *object) : Functor()
{
    // m_stack.push_back(node);
    m_current = object;

    m_inScoreLevel = NOT_IN_SCORE;
    this->SetVisibleOnly(false);
}

FunctorCode ScoreContextFunctor::VisitObject(Object *object)
{
    // In any case do not go beyond these
    if (object->GetParent() && object->GetParent()->Is({ DIV, MEASURE, SCOREDEF })) {
        return FUNCTOR_SIBLINGS;
    }
    if (m_inScoreLevel == INCLUDED) {
        // Just return here, the level will be set back in VisitObjectEnd
        return FUNCTOR_SIBLINGS;
    }

    // Do not include in the tree
    if ((m_inScoreLevel == NOT_IN_SCORE) && !object->Is({ MDIV, SCORE })) {
        return FUNCTOR_CONTINUE;
    }

    bool ownChildren = false;
    // The first one in the scoreDef subtree - simply use own children and stop
    if (m_inScoreLevel == TO_INCLUDE) {
        ownChildren = true;
    }

    EditorTreeObject *treeObject = new EditorTreeObject(object, ownChildren);
    treeObject->SetVisibility(Visible);
    m_current->AddChild(treeObject);
    m_current = treeObject;

    if (object->Is(SCORE)) {
        Score *score = vrv_cast<Score *>(object);
        assert(score);
        // We are now in the score and process the scoreDef subtree
        // Include the first one in the context tree - and then own children
        m_inScoreLevel = TO_INCLUDE;
        score->GetScoreDef()->Process(*this);
        m_inScoreLevel = NOT_IN_SCORE;
    }

    if (m_inScoreLevel == TO_INCLUDE) m_inScoreLevel = INCLUDED;

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreContextFunctor::VisitObjectEnd(Object *object)
{
    if (object->IsMilestoneElement()) {
        return FUNCTOR_CONTINUE;
    }
    // End milestone for mdiv or score - pop it
    if (object->Is(PAGE_MILESTONE_END)) {
        m_current = m_current->GetParent();
    }
    // The have not been pushed, continue
    if ((m_inScoreLevel == NOT_IN_SCORE) && !object->Is({ MDIV, SCORE })) {
        return FUNCTOR_CONTINUE;
    }

    m_current = m_current->GetParent();

    // We included the first one of the scoreDef substree with its own children, we should stop adding them
    if (m_inScoreLevel == INCLUDED) m_inScoreLevel = NOT_IN_SCORE;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
