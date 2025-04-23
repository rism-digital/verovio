/////////////////////////////////////////////////////////////////////////////
// Name:        system.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "system.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "beamspan.h"
#include "comparison.h"
#include "convertfunctor.h"
#include "dir.h"
#include "div.h"
#include "doc.h"
#include "dynam.h"
#include "ending.h"
#include "findfunctor.h"
#include "findlayerelementsfunctor.h"
#include "layer.h"
#include "measure.h"
#include "miscfunctor.h"
#include "page.h"
#include "pages.h"
#include "pedal.h"
#include "section.h"
#include "slur.h"
#include "staff.h"
#include "syl.h"
#include "systemmilestone.h"
#include "tempo.h"
#include "trill.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

System::System() : Object(SYSTEM), DrawingListInterface(), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    // We set parent to it because we want to access the parent doc from the aligners
    m_systemAligner.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;

    this->Reset();
}

System::~System()
{
    // We need to delete own objects
    this->Reset();
}

void System::Reset()
{
    Object::Reset();
    DrawingListInterface::Reset();
    this->ResetTyped();

    this->ResetDrawingScoreDef();

    m_systemLeftMar = 0;
    m_systemRightMar = 0;
    m_drawingFacsX = VRV_UNSET;
    m_drawingXRel = 0;
    m_drawingFacsY = VRV_UNSET;
    m_drawingYRel = 0;
    m_drawingTotalWidth = 0;
    m_drawingJustifiableWidth = 0;
    m_castOffTotalWidth = 0;
    m_castOffJustifiableWidth = 0;
    m_drawingAbbrLabelsWidth = 0;
    m_drawingIsOptimized = false;
}

bool System::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ DIV, MEASURE, SCOREDEF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsSystemElement(classId)) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

int System::GetDrawingX() const
{
    if (m_drawingFacsX != VRV_UNSET) return m_drawingFacsX;

    m_cachedDrawingX = 0;
    return m_drawingXRel;
}

int System::GetDrawingY() const
{
    if (m_drawingFacsY != VRV_UNSET) return m_drawingFacsY;

    m_cachedDrawingY = 0;
    return m_drawingYRel;
}

void System::SetDrawingXRel(int drawingXRel)
{
    this->ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void System::SetDrawingYRel(int drawingYRel)
{
    this->ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

int System::GetHeight() const
{
    if (m_systemAligner.GetBottomAlignment()) {
        return -m_systemAligner.GetBottomAlignment()->GetYRel();
    }
    return 0;
}

Staff *System::GetTopVisibleStaff()
{
    return const_cast<Staff *>(std::as_const(*this).GetTopVisibleStaff());
}

const Staff *System::GetTopVisibleStaff() const
{
    for (auto child : m_systemAligner.GetChildren()) {
        const StaffAlignment *alignment = vrv_cast<const StaffAlignment *>(child);
        if (alignment->GetStaff()) return alignment->GetStaff();
    }
    return NULL;
}

int System::GetMinimumSystemSpacing(const Doc *doc) const
{
    const auto &spacingSystem = doc->GetOptions()->m_spacingSystem;
    if (!spacingSystem.IsSet()) {
        assert(m_drawingScoreDef);
        if (m_drawingScoreDef->HasSpacingSystem()) {
            if (m_drawingScoreDef->GetSpacingSystem().GetType() == MEASUREMENTTYPE_px) {
                return m_drawingScoreDef->GetSpacingSystem().GetPx();
            }
            else {
                return m_drawingScoreDef->GetSpacingSystem().GetVu() * doc->GetDrawingUnit(100);
            }
        }
    }

    return spacingSystem.GetValue() * doc->GetDrawingUnit(100);
}

int System::GetDrawingLabelsWidth() const
{
    return (m_drawingScoreDef) ? m_drawingScoreDef->GetDrawingLabelsWidth() : 0;
}

void System::SetDrawingLabelsWidth(int width)
{
    assert(m_drawingScoreDef);
    m_drawingScoreDef->SetDrawingLabelsWidth(width);
}

void System::SetDrawingAbbrLabelsWidth(int width)
{
    if (m_drawingAbbrLabelsWidth < width) {
        m_drawingAbbrLabelsWidth = width;
    }
}

bool System::SetCurrentFloatingPositioner(
    int staffN, FloatingObject *object, Object *objectX, Object *objectY, char spanningType)
{
    assert(object);

    // If we have only the bottom alignment, then nothing to do (yet)
    if (m_systemAligner.GetChildCount() == 1) return false;
    StaffAlignment *alignment = m_systemAligner.GetStaffAlignmentForStaffN(staffN);
    if (!alignment) {
        LogError("Staff @n='%d' for rendering control event %s %s not found", staffN, object->GetClassName().c_str(),
            object->GetID().c_str());
        return false;
    }
    alignment->SetCurrentFloatingPositioner(object, objectX, objectY, spanningType);
    return true;
}

void System::SetDrawingScoreDef(ScoreDef *drawingScoreDef)
{
    assert(!m_drawingScoreDef); // We should always call ResetDrawingScoreDef before

    m_drawingScoreDef = new ScoreDef();
    m_drawingScoreDef->ReplaceWithCopyOf(drawingScoreDef);
    m_drawingScoreDef->SetParent(this);
}

void System::ResetDrawingScoreDef()
{
    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }
}

bool System::HasMixedDrawingStemDir(const LayerElement *start, const LayerElement *end) const
{
    assert(start);
    assert(end);

    // It is too inefficient to look for chord and notes over the entire system
    // We need first to get a list of measures
    const Object *measureStart = start->GetFirstAncestor(MEASURE);
    assert(measureStart);
    const Object *measureEnd = end->GetFirstAncestor(MEASURE);
    assert(measureEnd);
    ListOfConstObjects measures;

    // start and end are in the same measure, this is the only one we need
    if (measureStart == measureEnd) {
        measures.push_back(measureStart);
    }
    // otherwise look for a measures in between
    else {
        ClassIdComparison isMeasure(MEASURE);
        FindAllBetweenFunctor findAllBetween(&isMeasure, &measures, measureStart, measureEnd);
        this->Process(findAllBetween, 1);
    }

    // Now we can look for chords and note
    ClassIdsComparison matchType({ CHORD, NOTE });
    ListOfConstObjects children;
    for (const Object *measure : measures) {
        const Object *curStart = (measure == measureStart) ? start : measure->GetFirst();
        const Object *curEnd = (measure == measureEnd) ? end : measure->GetLast();
        measure->FindAllDescendantsBetween(&children, &matchType, curStart, curEnd, false);
    }

    const Layer *layerStart = vrv_cast<const Layer *>(start->GetFirstAncestor(LAYER));
    assert(layerStart);
    const Staff *staffStart = vrv_cast<const Staff *>(layerStart->GetFirstAncestor(STAFF));
    assert(staffStart);

    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    for (const Object *child : children) {
        const Layer *layer = vrv_cast<const Layer *>(child->GetFirstAncestor(LAYER));
        assert(layer);
        const Staff *staff = vrv_cast<const Staff *>(child->GetFirstAncestor(STAFF));
        assert(staff);

        // If the slur is spanning over several measures, the children list will include notes and chords
        // from other staves and layers, so we need to skip them.
        // Alternatively we could process by staff / layer, but the current solution might be better
        // if we want to look for slurs starting / ending on different staff / layer
        if ((staff->GetN() != staffStart->GetN()) || (layer->GetN() != layerStart->GetN())) {
            continue;
        }

        const StemmedDrawingInterface *interface = child->GetStemmedDrawingInterface();
        assert(interface);

        // First pass
        if (stemDir == STEMDIRECTION_NONE) {
            stemDir = interface->GetDrawingStemDir();
        }
        else if (stemDir != interface->GetDrawingStemDir()) {
            return true;
        }
    }

    return false;
}

curvature_CURVEDIR System::GetPreferredCurveDirection(
    const LayerElement *start, const LayerElement *end, const Slur *slur) const
{
    FindSpannedLayerElementsFunctor findSpannedLayerElements(slur);
    findSpannedLayerElements.SetMinMaxPos(start->GetDrawingX(), end->GetDrawingX());
    findSpannedLayerElements.SetClassIds({ CHORD, NOTE });

    const Layer *layerStart = vrv_cast<const Layer *>(start->GetFirstAncestor(LAYER));
    assert(layerStart);

    this->Process(findSpannedLayerElements);

    curvature_CURVEDIR preferredDirection = curvature_CURVEDIR_NONE;
    for (auto element : findSpannedLayerElements.GetElements()) {
        const Layer *layer = vrv_cast<const Layer *>((element)->GetFirstAncestor(LAYER));
        assert(layer);
        if (layer == layerStart) continue;

        if (curvature_CURVEDIR_NONE == preferredDirection) {
            if (layer->GetN() > layerStart->GetN()) {
                preferredDirection = curvature_CURVEDIR_above;
            }
            else {
                preferredDirection = curvature_CURVEDIR_below;
            }
        }
        // if there are layers both above and below - discard previous location and return - we'll use default direction
        else if (((curvature_CURVEDIR_above == preferredDirection) && (layer->GetN() < layerStart->GetN()))
            || ((curvature_CURVEDIR_below == preferredDirection) && (layer->GetN() > layerStart->GetN()))) {
            preferredDirection = curvature_CURVEDIR_NONE;
            break;
        }
    }

    return preferredDirection;
}

void System::AddToDrawingListIfNecessary(Object *object)
{
    assert(object);

    if (!object->HasInterface(INTERFACE_TIME_SPANNING)) return;

    if (object->Is({ ANNOTSCORE, BEAMSPAN, BRACKETSPAN, FIGURE, GLISS, HAIRPIN, LV, OCTAVE, PHRASE, PITCHINFLECTION,
            SLUR, SYL, TIE })) {
        this->AddToDrawingList(object);
    }
    else if (object->Is(DIR)) {
        Dir *dir = vrv_cast<Dir *>(object);
        assert(dir);
        if (dir->GetEnd() || (dir->GetNextLink() && (dir->GetExtender() == BOOLEAN_true))) {
            this->AddToDrawingList(dir);
        }
    }
    else if (object->Is(DYNAM)) {
        Dynam *dynam = vrv_cast<Dynam *>(object);
        assert(dynam);
        if ((dynam->GetEnd() || dynam->GetNextLink()) && (dynam->GetExtender() == BOOLEAN_true)) {
            this->AddToDrawingList(dynam);
        }
    }
    else if (object->Is(PEDAL)) {
        Pedal *pedal = vrv_cast<Pedal *>(object);
        assert(pedal);
        if (pedal->GetEnd()) {
            this->AddToDrawingList(pedal);
        }
    }
    else if (object->Is(TEMPO)) {
        Tempo *tempo = vrv_cast<Tempo *>(object);
        assert(tempo);
        if (tempo->GetEnd() && (tempo->GetExtender() == BOOLEAN_true)) {
            this->AddToDrawingList(tempo);
        }
    }
    else if (object->Is(TRILL)) {
        Trill *trill = vrv_cast<Trill *>(object);
        assert(trill);
        if (trill->GetEnd() && (trill->GetExtender() != BOOLEAN_false)) {
            this->AddToDrawingList(trill);
        }
    }
}

bool System::IsFirstInPage() const
{
    assert(this->GetParent());
    return (this->GetParent()->GetFirst(SYSTEM) == this);
}

bool System::IsLastInPage() const
{
    assert(this->GetParent());
    return (this->GetParent()->GetLast(SYSTEM) == this);
}

bool System::IsFirstOfMdiv() const
{
    assert(this->GetParent());
    const Object *nextSibling = this->GetParent()->GetPrevious(this);
    return (nextSibling && nextSibling->IsPageElement());
}

bool System::IsLastOfMdiv() const
{
    assert(this->GetParent());
    const Object *nextSibling = this->GetParent()->GetNext(this);
    return (nextSibling && nextSibling->IsPageElement());
}

bool System::IsFirstOfSelection() const
{
    const Page *page = vrv_cast<const Page *>(this->GetFirstAncestor(PAGE));
    assert(page);
    return (page->IsFirstOfSelection() && this->IsFirstInPage());
}

bool System::IsLastOfSelection() const
{
    const Page *page = vrv_cast<const Page *>(this->GetFirstAncestor(PAGE));
    assert(page);
    return (page->IsLastOfSelection() && this->IsLastInPage());
}

double System::EstimateJustificationRatio(const Doc *doc) const
{
    assert(doc);

    // We can only estimate if cast off system widths are available
    if ((m_castOffTotalWidth == 0) || (m_castOffJustifiableWidth == 0)) {
        return 1.0;
    }

    const double nonJustifiableWidth
        = m_systemLeftMar + m_systemRightMar + m_castOffTotalWidth - m_castOffJustifiableWidth;
    double estimatedRatio
        = (double)(doc->m_drawingPageContentWidth - nonJustifiableWidth) / ((double)m_castOffJustifiableWidth);

    // Apply dampening and bound compression
    estimatedRatio *= 0.95;
    estimatedRatio = std::max(estimatedRatio, 0.8);

    return estimatedRatio;
}

void System::ConvertToUnCastOffMensuralSystem()
{
    // We need to populate processing lists for processing the document by Layer
    InitProcessingListsFunctor initProcessingLists;
    this->Process(initProcessingLists);
    const IntTree &layerTree = initProcessingLists.GetLayerTree();

    // Checking just in case
    if (layerTree.child.empty()) return;

    Filters filters;
    ConvertToUnCastOffMensuralFunctor convertToUnCastOffMensural;
    convertToUnCastOffMensural.SetFilters(&filters);

    // Now we can process by layer and move their content to (measure) segments
    for (const auto &staves : layerTree.child) {
        for (const auto &layers : staves.second.child) {
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters = { &matchStaff, &matchLayer };

            convertToUnCastOffMensural.ResetContent();
            this->Process(convertToUnCastOffMensural);
            convertToUnCastOffMensural.TrackSegmentsToDelete(false);
        }
    }

    // Detach the contentPage
    for (Object *measure : convertToUnCastOffMensural.GetSegmentsToDelete()) {
        this->DeleteChild(measure);
    }
}

//----------------------------------------------------------------------------
// System functor methods
//----------------------------------------------------------------------------

FunctorCode System::Accept(Functor &functor)
{
    return functor.VisitSystem(this);
}

FunctorCode System::Accept(ConstFunctor &functor) const
{
    return functor.VisitSystem(this);
}

FunctorCode System::AcceptEnd(Functor &functor)
{
    return functor.VisitSystemEnd(this);
}

FunctorCode System::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSystemEnd(this);
}

} // namespace vrv
