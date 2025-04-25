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

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// StructFunctor
//----------------------------------------------------------------------------

StructFunctor::StructFunctor(Object *object, bool scoreContext) : Functor()
{
    // m_stack.push_back(node);
    m_current = object;

    m_scoreContext = scoreContext;
    this->SetVisibleOnly(!m_scoreContext);
}

FunctorCode StructFunctor::VisitObject(Object *object)
{
    // In any case do not go beyond these
    if (object->GetParent() && object->GetParent()->Is({ DIV, MEASURE, SCOREDEF })) {
        return FUNCTOR_SIBLINGS;
    }

    if (m_scoreContext) {
        if (!object->Is({ MDIV, SCORE })) {
            return FUNCTOR_CONTINUE;
        }
    }
    else {
        if (object->Is({ DOC, PAGE, PAGES, PAGE_MILESTONE_END, SYSTEM, SYSTEM_MILESTONE_END })) {
            return FUNCTOR_CONTINUE;
        }
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

FunctorCode StructFunctor::VisitObjectEnd(Object *object)
{
    // LogInfo("%s", object->GetClassName().c_str());
    if (object->IsMilestoneElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (m_scoreContext) {
        if (object->Is(PAGE_MILESTONE_END)) {
            m_current = m_current->GetParent();
        }
        if (!object->Is({ MDIV, SCORE })) {
            return FUNCTOR_CONTINUE;
        }
    }
    else {
        if (object->Is({ PAGE_MILESTONE_END, SYSTEM_MILESTONE_END })) {
            m_current = m_current->GetParent();
        }
        if (object->Is({ DOC, PAGE, PAGES, PAGE_MILESTONE_END, SYSTEM, SYSTEM_MILESTONE_END })) {
            return FUNCTOR_CONTINUE;
        }
    }

    m_current = m_current->GetParent();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
