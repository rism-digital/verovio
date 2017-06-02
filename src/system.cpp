/////////////////////////////////////////////////////////////////////////////
// Name:        system.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "system.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "boundary.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "functorparams.h"
#include "measure.h"
#include "page.h"
#include "section.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

System::System() : Object("system-"), DrawingListInterface(), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    // We set parent to it because we want to access the parent doc from the aligners
    m_systemAligner.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;

    Reset();
}

System::~System()
{
    // We need to delete own objects
    Reset();
}

void System::Reset()
{
    Object::Reset();
    DrawingListInterface::Reset();
    ResetTyped();

    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    m_systemLeftMar = 0;
    m_systemRightMar = 0;
    m_xAbs = VRV_UNSET;
    m_drawingXRel = 0;
    m_yAbs = VRV_UNSET;
    m_drawingYRel = 0;
    m_drawingTotalWidth = 0;
    m_drawingJustifiableWidth = 0;
    m_drawingLabelsWidth = 0;
    m_drawingAbbrLabelsWidth = 0;
}

void System::AddChild(Object *child)
{
    if (child->Is(MEASURE)) {
        assert(dynamic_cast<Measure *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->IsSystemElement()) {
        assert(dynamic_cast<SystemElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

int System::GetDrawingX() const
{
    if (m_xAbs != VRV_UNSET) return m_xAbs;

    m_cachedDrawingX = 0;
    return m_drawingXRel;
}

int System::GetDrawingY() const
{
    if (m_yAbs != VRV_UNSET) return m_yAbs;

    m_cachedDrawingY = 0;
    return m_drawingYRel;
}

void System::SetDrawingXRel(int drawingXRel)
{
    ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void System::SetDrawingYRel(int drawingYRel)
{
    ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

int System::GetHeight() const
{
    if (m_systemAligner.GetBottomAlignment()) {
        return -m_systemAligner.GetBottomAlignment()->GetYRel();
    }
    return 0;
}

void System::SetDrawingLabelsWidth(int width)
{
    if (m_drawingLabelsWidth < width) {
        m_drawingLabelsWidth = width;
    }
}

void System::SetDrawingAbbrLabelsWidth(int width)
{
    if (m_drawingAbbrLabelsWidth < width) {
        m_drawingAbbrLabelsWidth = width;
    }
}

void System::SetCurrentFloatingPositioner(int staffN, FloatingObject *object, Object *objectX, Object *objectY)
{
    assert(object);

    // If we have only the bottom alignment, then nothing to do (yet)
    if (m_systemAligner.GetChildCount() == 1) return;
    StaffAlignment *alignment = m_systemAligner.GetStaffAlignmentForStaffN(staffN);
    assert(alignment);
    alignment->SetCurrentFloatingPositioner(object, objectX, objectY);
}

void System::SetDrawingScoreDef(ScoreDef *drawingScoreDef)
{
    assert(!m_drawingScoreDef); // We should always call UnsetCurrentScoreDef before

    m_drawingScoreDef = new ScoreDef();
    *m_drawingScoreDef = *drawingScoreDef;
}

//----------------------------------------------------------------------------
// System functor methods
//----------------------------------------------------------------------------

int System::UnsetCurrentScoreDef(FunctorParams *functorParams)
{
    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    return FUNCTOR_CONTINUE;
};

int System::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    SetDrawingXRel(0);
    m_drawingLabelsWidth = 0;
    m_drawingAbbrLabelsWidth = 0;

    return FUNCTOR_CONTINUE;
}

int System::ResetVerticalAlignment(FunctorParams *functorParams)
{
    SetDrawingYRel(0);

    m_systemAligner.Reset();

    return FUNCTOR_CONTINUE;
}

int System::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = dynamic_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (m_xAbs != VRV_UNSET) m_xAbs /= params->m_page->GetPPUFactor();
    if (m_yAbs != VRV_UNSET) m_yAbs /= params->m_page->GetPPUFactor();
    m_systemLeftMar *= params->m_page->GetPPUFactor();
    m_systemRightMar *= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int System::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // since we are starting a new system its first scoreDef will need to be a SYSTEM_SCOREDEF
    params->m_isFirstMeasure = true;

    return FUNCTOR_CONTINUE;
}

int System::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    params->m_systemAligner = &m_systemAligner;

    return FUNCTOR_CONTINUE;
}

int System::AlignVerticallyEnd(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    params->m_cumulatedShift = params->m_doc->GetSpacingStaff() * params->m_doc->GetDrawingUnit(100);

    m_systemAligner.Process(params->m_functorEnd, params);

    return FUNCTOR_SIBLINGS;
}

int System::AdjustYPos(FunctorParams *functorParams)
{
    AdjustYPosParams *params = dynamic_cast<AdjustYPosParams *>(functorParams);
    assert(params);

    params->m_previousOverflowBelow = 0;
    params->m_previousVerseCount = 0;
    params->m_cumulatedShift = 0;

    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::AlignMeasures(FunctorParams *functorParams)
{
    AlignMeasuresParams *params = dynamic_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    SetDrawingXRel(this->m_systemLeftMar + this->GetDrawingLabelsWidth());
    params->m_shift = 0;
    params->m_justifiableWidth = 0;

    return FUNCTOR_CONTINUE;
}

int System::AlignMeasuresEnd(FunctorParams *functorParams)
{
    AlignMeasuresParams *params = dynamic_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    m_drawingTotalWidth = params->m_shift + this->GetDrawingLabelsWidth();
    m_drawingJustifiableWidth = params->m_justifiableWidth;

    return FUNCTOR_CONTINUE;
}

int System::AlignSystems(FunctorParams *functorParams)
{
    AlignSystemsParams *params = dynamic_cast<AlignSystemsParams *>(functorParams);
    assert(params);

    SetDrawingYRel(params->m_shift);

    assert(m_systemAligner.GetBottomAlignment());

    params->m_shift += m_systemAligner.GetBottomAlignment()->GetYRel() - params->m_systemMargin;

    return FUNCTOR_SIBLINGS;
}

int System::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = dynamic_cast<JustifyXParams *>(functorParams);
    assert(params);

    assert(GetParent());
    assert(GetParent()->GetParent());

    Object *parent = GetParent();

    params->m_measureXRel = 0;
    int margins = this->m_systemLeftMar + this->m_systemRightMar;
    int nonJustifiableWidth
        = margins + (m_drawingTotalWidth - m_drawingJustifiableWidth); // m_drawingTotalWidth includes the labels
    params->m_justifiableRatio
        = (double)(params->m_systemFullWidth - nonJustifiableWidth) / ((double)m_drawingJustifiableWidth);

    if (params->m_justifiableRatio < 0.8) {
        // Arbitrary value for avoiding over-compressed justification
        LogWarning("Justification stop because of a ratio smaller than 0.8");
    }

    // Check if we are on the last page and on the last system - do no justify it if ratio > 1.25
    // Eventually we should make this a parameter
    if ((parent->GetIdx() == parent->GetParent()->GetChildCount() - 1)
        && (this->GetIdx() == parent->GetChildCount() - 1)) {
        // HARDCODED
        if (params->m_justifiableRatio > 1.25) {
            return FUNCTOR_STOP;
        }
    }

    return FUNCTOR_CONTINUE;
}

int System::AdjustStaffOverlap(FunctorParams *functorParams)
{
    AdjustStaffOverlapParams *params = dynamic_cast<AdjustStaffOverlapParams *>(functorParams);
    assert(params);

    params->m_previous = NULL;
    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::AdjustFloatingPostioners(FunctorParams *functorParams)
{
    AdjustFloatingPostionersParams *params = dynamic_cast<AdjustFloatingPostionersParams *>(functorParams);
    assert(params);

    AdjustFloatingPostionerGrpsParams adjustFloatingPostionerGrpsParams(params->m_doc);
    Functor adjustFloatingPostionerGrps(&Object::AdjustFloatingPostionerGrps);

    params->m_classId = TIE;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = SLUR;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = DYNAM;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = HAIRPIN;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = OCTAVE;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = DIR;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = MORDENT;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = TURN;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = TRILL;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = BREATH;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = FERMATA;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = TEMPO;
    m_systemAligner.Process(params->m_functor, params);
    params->m_classId = PEDAL;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = HARM;
    m_systemAligner.Process(params->m_functor, params);
    adjustFloatingPostionerGrpsParams.m_classIds.clear();
    adjustFloatingPostionerGrpsParams.m_classIds.push_back(HARM);
    m_systemAligner.Process(&adjustFloatingPostionerGrps, &adjustFloatingPostionerGrpsParams);

    params->m_classId = ENDING;
    m_systemAligner.Process(params->m_functor, params);
    adjustFloatingPostionerGrpsParams.m_classIds.clear();
    adjustFloatingPostionerGrpsParams.m_classIds.push_back(ENDING);
    m_systemAligner.Process(&adjustFloatingPostionerGrps, &adjustFloatingPostionerGrpsParams);

    // SYL check if they are some lyrics and make space for them if any
    params->m_classId = SYL;
    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::CastOffPages(FunctorParams *functorParams)
{
    CastOffPagesParams *params = dynamic_cast<CastOffPagesParams *>(functorParams);
    assert(params);

    if ((params->m_currentPage->GetChildCount() > 0)
        && (this->m_drawingYRel - this->GetHeight() - params->m_shift < 0)) {
        params->m_currentPage = new Page();
        params->m_doc->AddChild(params->m_currentPage);
        params->m_shift = this->m_drawingYRel - params->m_pageHeight;
    }

    // Special case where we use the Relinquish method.
    // We want to move the system to the currentPage. However, we cannot use DetachChild
    // from the contentPage because this screws up the iterator. Relinquish gives up
    // the ownership of the system - the contentPage itself will be deleted afterwards.
    System *system = dynamic_cast<System *>(params->m_contentPage->Relinquish(this->GetIdx()));
    assert(system);
    params->m_currentPage->AddChild(system);

    return FUNCTOR_SIBLINGS;
}

int System::UnCastOff(FunctorParams *functorParams)
{
    UnCastOffParams *params = dynamic_cast<UnCastOffParams *>(functorParams);
    assert(params);

    // Just move all the content of the system to the continous one (parameter)
    // Use the MoveChildrenFrom method that moves and relinquishes them
    // See Object::Relinquish
    params->m_currentSystem->MoveChildrenFrom(this);

    return FUNCTOR_CONTINUE;
}

int System::CastOffSystemsEnd(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    if (params->m_pendingObjects.empty()) return FUNCTOR_STOP;

    // Otherwise add all pendings objects
    ArrayOfObjects::iterator iter;
    for (iter = params->m_pendingObjects.begin(); iter != params->m_pendingObjects.end(); iter++) {
        params->m_currentSystem->AddChild(*iter);
    }

    return FUNCTOR_STOP;
}

} // namespace vrv
