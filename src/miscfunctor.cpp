/////////////////////////////////////////////////////////////////////////////
// Name:        miscfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "miscfunctor.h"

//----------------------------------------------------------------------------

#include "layer.h"
#include "lyricelement.h"
#include "page.h"
#include "refrain.h"
#include "staff.h"
#include "surface.h"
#include "system.h"
#include "verse.h"
#include "volta.h"
#include "zone.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ApplyPPUFactorFunctor
//----------------------------------------------------------------------------

ApplyPPUFactorFunctor::ApplyPPUFactorFunctor(Page *page) : Functor()
{
    m_page = page;
}

FunctorCode ApplyPPUFactorFunctor::VisitLayerElement(LayerElement *layerElement)
{
    assert(m_page);

    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (layerElement->m_drawingFacsX != VRV_UNSET) layerElement->m_drawingFacsX /= m_page->GetPPUFactor();
    if (layerElement->m_drawingFacsY != VRV_UNSET) layerElement->m_drawingFacsY /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitMeasure(Measure *measure)
{
    assert(m_page);

    if (measure->m_drawingFacsX1 != VRV_UNSET) measure->m_drawingFacsX1 /= m_page->GetPPUFactor();
    if (measure->m_drawingFacsX2 != VRV_UNSET) measure->m_drawingFacsX2 /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitPage(Page *page)
{
    m_page = page;
    m_page->m_pageWidth /= m_page->GetPPUFactor();
    m_page->m_pageHeight /= m_page->GetPPUFactor();
    m_page->m_pageMarginBottom /= m_page->GetPPUFactor();
    m_page->m_pageMarginLeft /= m_page->GetPPUFactor();
    m_page->m_pageMarginRight /= m_page->GetPPUFactor();
    m_page->m_pageMarginTop /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitStaff(Staff *staff)
{
    assert(m_page);

    if (staff->m_drawingFacsY != VRV_UNSET) staff->m_drawingFacsY /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitSurface(Surface *surface)
{
    assert(m_page);

    if (surface->HasUlx()) surface->SetUlx(surface->GetUlx() * m_page->GetPPUFactor());
    if (surface->HasUly()) surface->SetUly(surface->GetUly() * m_page->GetPPUFactor());
    if (surface->HasLrx()) surface->SetLrx(surface->GetLrx() * m_page->GetPPUFactor());
    if (surface->HasLry()) surface->SetLry(surface->GetLry() * m_page->GetPPUFactor());

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitSystem(System *system)
{
    assert(m_page);

    if (system->m_drawingFacsX != VRV_UNSET) system->m_drawingFacsX /= m_page->GetPPUFactor();
    if (system->m_drawingFacsY != VRV_UNSET) system->m_drawingFacsY /= m_page->GetPPUFactor();
    system->m_systemLeftMar *= m_page->GetPPUFactor();
    system->m_systemRightMar *= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitZone(Zone *zone)
{
    assert(m_page);

    if (zone->HasUlx()) zone->SetUlx(zone->GetUlx() * m_page->GetPPUFactor());
    if (zone->HasUly()) zone->SetUly(zone->GetUly() * m_page->GetPPUFactor());
    if (zone->HasLrx()) zone->SetLrx(zone->GetLrx() * m_page->GetPPUFactor());
    if (zone->HasLry()) zone->SetLry(zone->GetLry() * m_page->GetPPUFactor());

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// GetAlignmentLeftRightFunctor
//----------------------------------------------------------------------------

GetAlignmentLeftRightFunctor::GetAlignmentLeftRightFunctor() : ConstFunctor()
{
    m_minLeft = -VRV_UNSET;
    m_maxRight = VRV_UNSET;
}

FunctorCode GetAlignmentLeftRightFunctor::VisitObject(const Object *object)
{
    if (!object->IsLayerElement()) return FUNCTOR_CONTINUE;

    if (!object->HasSelfBB() || object->HasEmptyBB()) return FUNCTOR_CONTINUE;

    if (object->IsAnyOf(m_excludeClasses)) return FUNCTOR_CONTINUE;

    m_minLeft = std::min(m_minLeft, object->GetSelfLeft());
    m_maxRight = std::max(m_maxRight, object->GetSelfRight());

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// InitProcessingListsFunctor
//----------------------------------------------------------------------------

InitProcessingListsFunctor::InitProcessingListsFunctor() : ConstFunctor() {}

const IntTree &InitProcessingListsFunctor::GetVerseTree()
{
    this->PrepareLyricElementTracks();
    return m_verseTree;
}

FunctorCode InitProcessingListsFunctor::VisitLayer(const Layer *layer)
{
    const Staff *staff = vrv_cast<const Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);
    m_layerTree.child[staff->GetN()].child[layer->GetN()];

    return FUNCTOR_CONTINUE;
}

FunctorCode InitProcessingListsFunctor::VisitVerse(const Verse *verse)
{
    this->CollectLyricElement(verse);
    return FUNCTOR_SIBLINGS;
}

FunctorCode InitProcessingListsFunctor::VisitRefrain(const Refrain *refrain)
{
    this->CollectLyricElement(refrain);
    return FUNCTOR_SIBLINGS;
}

void InitProcessingListsFunctor::CollectLyricElement(const LyricElement *lyricElement)
{
    m_lyricElements.push_back(lyricElement);
}

static int GetRefrainPosition(const LyricElement *lyricElement)
{
    assert(lyricElement->Is(REFRAIN));
    int position = 1;
    const Object *parent = lyricElement->GetParent();
    assert(parent);
    for (const Object *child : parent->GetChildren()) {
        if (child == lyricElement) break;
        if (child->Is(REFRAIN)) ++position;
    }
    return position;
}

static data_STAFFREL GetLyricPlace(const LyricElement *lyricElement)
{
    return (lyricElement->GetPlace() == STAFFREL_above) ? STAFFREL_above : STAFFREL_below;
}

void InitProcessingListsFunctor::PrepareLyricElementTracks()
{
    if (m_lyricElementTracksPrepared) return;
    m_lyricElementTracksPrepared = true;

    std::map<std::pair<int, int>, int> maxVerseN;
    std::map<std::tuple<int, int, data_STAFFREL>, int> maxVerseNByPlace;
    for (const LyricElement *lyricElement : m_lyricElements) {
        if (!lyricElement->Is(VERSE)) continue;
        const Verse *verse = vrv_cast<const Verse *>(lyricElement);
        assert(verse);
        const Staff *staff = lyricElement->GetAncestorStaff();
        const Layer *layer = vrv_cast<const Layer *>(lyricElement->GetFirstAncestor(LAYER));
        assert(staff && layer);
        const int verseN = std::max(verse->GetN(), 1);
        const std::pair<int, int> staffLayer{ staff->GetN(), layer->GetN() };
        maxVerseN[staffLayer] = std::max(maxVerseN[staffLayer], verseN);
        const auto placeKey = std::make_tuple(staff->GetN(), layer->GetN(), GetLyricPlace(lyricElement));
        maxVerseNByPlace[placeKey] = std::max(maxVerseNByPlace[placeKey], verseN);
    }

    for (const LyricElement *lyricElement : m_lyricElements) {
        const Staff *staff = lyricElement->GetAncestorStaff();
        const Layer *layer = vrv_cast<const Layer *>(lyricElement->GetFirstAncestor(LAYER));
        assert(staff && layer);

        const int staffN = staff->GetN();
        const int layerN = layer->GetN();
        int drawingVerseN = 1;
        int lyricGroupN = 1;
        if (lyricElement->Is(VERSE)) {
            const Verse *verse = vrv_cast<const Verse *>(lyricElement);
            assert(verse);
            drawingVerseN = std::max(verse->GetN(), 1);
            lyricGroupN = drawingVerseN;
        }
        else {
            const int refrainPosition = GetRefrainPosition(lyricElement);
            drawingVerseN
                = maxVerseNByPlace[std::make_tuple(staffN, layerN, GetLyricPlace(lyricElement))] + refrainPosition;
            lyricGroupN = maxVerseN[{ staffN, layerN }] + refrainPosition;
        }
        lyricElement->SetDrawingVerseN(drawingVerseN);
        lyricElement->SetDrawingLyricGroupN(lyricGroupN);

        IntTree &verseTree = m_verseTree.child[staffN].child[layerN].child[lyricGroupN];
        const std::tuple<int, int, int> lyricKey{ staffN, layerN, lyricGroupN };
        const bool hasDirectSyl = lyricElement->HasDirectSyl();
        if (hasDirectSyl) verseTree.child[0];

        std::vector<const LyricElement *> &lyricElementGroup = m_lyricElementGroups[lyricKey];
        lyricElementGroup.push_back(lyricElement);
        if (hasDirectSyl) m_directSylTrackGroups.insert(lyricKey);
        if (m_directSylTrackGroups.count(lyricKey)) {
            for (const LyricElement *groupMember : lyricElementGroup) groupMember->SetDrawingDirectSylTrack();
        }

        int position = 0;
        for (const Object *object : lyricElement->FindAllDescendantsByType(VOLTA)) {
            const Volta *volta = vrv_cast<const Volta *>(object);
            assert(volta);
            ++position;

            if (volta->HasDrawingVoltaN()) {
                verseTree.child[volta->GetDrawingVoltaN()];
                continue;
            }

            const std::string identity = volta->HasN() ? "n:" + volta->GetN() : "position:" + std::to_string(position);
            const auto trackKey = std::make_tuple(staffN, layerN, lyricGroupN, identity);
            auto [track, inserted] = m_voltaTracks.emplace(trackKey, 0);
            if (inserted) track->second = ++m_nextVoltaTrack[lyricKey];

            volta->SetDrawingVoltaN(track->second);
            verseTree.child[track->second];
        }
    }

    // Refrain alternatives occupy consecutive outer lyric slots after all numbered verses. This keeps every refrain
    // sub-line after the verse block with the existing above/below positioning rules.
    std::map<std::tuple<int, int, int>, int> refrainLineCounts;
    for (const LyricElement *lyricElement : m_lyricElements) {
        if (!lyricElement->Is(REFRAIN)) continue;
        const Staff *staff = lyricElement->GetAncestorStaff();
        const Layer *layer = vrv_cast<const Layer *>(lyricElement->GetFirstAncestor(LAYER));
        assert(staff && layer);
        const auto key = std::make_tuple(staff->GetN(), layer->GetN(), GetRefrainPosition(lyricElement));
        refrainLineCounts[key] = std::max(refrainLineCounts[key], lyricElement->GetLyricLineCount());
    }
    for (const LyricElement *lyricElement : m_lyricElements) {
        if (!lyricElement->Is(REFRAIN)) continue;
        const Staff *staff = lyricElement->GetAncestorStaff();
        const Layer *layer = vrv_cast<const Layer *>(lyricElement->GetFirstAncestor(LAYER));
        assert(staff && layer);
        const int position = GetRefrainPosition(lyricElement);
        int precedingLineCount = 0;
        for (int preceding = 1; preceding < position; ++preceding) {
            precedingLineCount += refrainLineCounts[{ staff->GetN(), layer->GetN(), preceding }];
        }
        const int maxVerse
            = maxVerseNByPlace[std::make_tuple(staff->GetN(), layer->GetN(), GetLyricPlace(lyricElement))];
        lyricElement->SetDrawingVerseN(maxVerse + precedingLineCount + 1);
    }
}

//----------------------------------------------------------------------------
// ReorderByXPosFunctor
//----------------------------------------------------------------------------

ReorderByXPosFunctor::ReorderByXPosFunctor() : Functor() {}

FunctorCode ReorderByXPosFunctor::VisitObject(Object *object)
{
    if (object->GetFacsimileInterface()) {
        if (object->GetFacsimileInterface()->HasFacs()) {
            return FUNCTOR_SIBLINGS; // This would have already been reordered.
        }
    }

    ArrayOfObjects &children = object->GetChildrenForModification();
    std::stable_sort(children.begin(), children.end(), Object::sortByUlx);

    object->Modify();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
