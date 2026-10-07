/////////////////////////////////////////////////////////////////////////////
// Name:        editfunctor.cpp
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editfunctor.h"

//----------------------------------------------------------------------------

#include "cursor.h"
#include "editorial.h"
#include "editortoolkit_cmn.h"
#include "layer.h"
#include "score.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// StructFunctor
//----------------------------------------------------------------------------

CursorFunctor::CursorFunctor(Layer *layer, LayerElement *position) : Functor()
{
    m_layer = layer;
    m_position = position;
    m_cursor = NULL;
    m_previous = NULL;
}

CursorFunctor::~CursorFunctor()
{
    if (m_previous) delete m_previous;
}

FunctorCode CursorFunctor::VisitLayer(Layer *layer)
{
    if (layer->HasCursor()) {
        // Store the values for when moving to the next layer
        m_previous = vrv_cast<Cursor *>(layer->GetCursor()->Clone());
        m_previous->CloneReset();
    }

    if (layer != m_layer) {
        layer->ResetCursor();
    }
    else {
        if (m_previous) {
            m_cursor = vrv_cast<Cursor *>(m_previous->Clone());
            m_cursor->CloneReset();
        }
        else {
            m_cursor = new Cursor();
            // Default it G2 clef
            int clefLocOffset = (layer->GetCurrentClef()) ? layer->GetCurrentClef()->GetClefLocOffset() : -4;
            // Place it on loc 4 (middle line assuming five lines for now)
            auto [pname, oct] = PitchInterface::CalcPitch(4, clefLocOffset);
            m_cursor->SetPname(pname);
            m_cursor->SetOct(oct);
        }
        assert(m_cursor);
        if (m_position) {
            m_cursor->SetPosition(m_position);
            if (m_position->Is(NOTE)) {
                const Note *note = vrv_cast<const Note *>(m_position);
                assert(note);
                m_cursor->SetPname(note->GetPname());
                m_cursor->SetOct(note->GetOct());
                m_cursor->SetAccidValue(note->GetDrawingAccid());
            }
        }
        layer->SetCursor(m_cursor);
    }

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// SectionContextFunctor
//----------------------------------------------------------------------------

SectionContextFunctor::SectionContextFunctor(Object *object) : Functor()
{
    m_current = object;
}

FunctorCode SectionContextFunctor::VisitObject(Object *object)
{
    // In any case do not go beyond these
    if (object->GetParent() && object->GetParent()->IsAnyOf(std::array{ DIV, MEASURE, SCOREDEF })) {
        return FUNCTOR_SIBLINGS;
    }

    if (object->IsAnyOf(std::array{ DOC, PAGE, PAGES, PAGE_MILESTONE_END, SYSTEM, SYSTEM_MILESTONE_END })) {
        return FUNCTOR_CONTINUE;
    }

    bool ownChildren = false;
    if (object->IsAnyOf(std::array{ DIV, MEASURE, SCOREDEF })) {
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

    if (object->IsAnyOf(std::array{ PAGE_MILESTONE_END, SYSTEM_MILESTONE_END })) {
        m_current = m_current->GetParent();
    }
    if (object->IsAnyOf(std::array{ DOC, PAGE, PAGES, PAGE_MILESTONE_END, SYSTEM, SYSTEM_MILESTONE_END })) {
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
    if (object->GetParent() && object->GetParent()->IsAnyOf(std::array{ DIV, MEASURE, SCOREDEF })) {
        return FUNCTOR_SIBLINGS;
    }
    if (m_inScoreLevel == INCLUDED) {
        // Just return here, the level will be set back in VisitObjectEnd
        return FUNCTOR_SIBLINGS;
    }

    // Do not include in the tree
    if ((m_inScoreLevel == NOT_IN_SCORE) && !object->IsAnyOf(std::array{ MDIV, SCORE })) {
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
    if ((m_inScoreLevel == NOT_IN_SCORE) && !object->IsAnyOf(std::array{ MDIV, SCORE })) {
        return FUNCTOR_CONTINUE;
    }

    m_current = m_current->GetParent();

    // We included the first one of the scoreDef substree with its own children, we should stop adding them
    if (m_inScoreLevel == INCLUDED) m_inScoreLevel = NOT_IN_SCORE;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
