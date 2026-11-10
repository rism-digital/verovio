/////////////////////////////////////////////////////////////////////////////
// Name:        adjustsylspacingfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustsylspacingfunctor.h"

//----------------------------------------------------------------------------

#include <algorithm>

//----------------------------------------------------------------------------

#include "doc.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "lyricelement.h"
#include "refrain.h"
#include "staff.h"
#include "syl.h"
#include "verse.h"
#include "volta.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustSylSpacingFunctor
//----------------------------------------------------------------------------

static int AdjustLyricPosition(LayerElement *container, int &overlap, int freeSpace, const Doc *doc)
{
    if (container->IsLyricElement()) {
        LyricElement *lyricElement = vrv_cast<LyricElement *>(container);
        return lyricElement->AdjustPosition(overlap, freeSpace, doc);
    }

    int nextFreeSpace = 0;
    if (overlap > 0) {
        if (freeSpace > overlap) {
            container->SetDrawingXRel(container->GetDrawingXRel() - overlap);
            overlap = 0;
        }
        else if (freeSpace > 0) {
            container->SetDrawingXRel(container->GetDrawingXRel() - freeSpace);
            overlap -= freeSpace;
        }
    }
    else {
        nextFreeSpace = std::min(-overlap, 3 * doc->GetDrawingUnit(100));
    }
    return nextFreeSpace;
}

static int GetVoltaGroupingSymbolWidth(const LyricElement *lyricElement, int unit)
{
    if ((lyricElement->GetVoltaCount() < 2) || !lyricElement->HasVoltasym()) return 0;

    switch (lyricElement->GetVoltasym()) {
        case voltaGroupingSym_VOLTASYM_brace: return 3 * unit;
        case voltaGroupingSym_VOLTASYM_bracket:
        case voltaGroupingSym_VOLTASYM_bracketsq:
        case voltaGroupingSym_VOLTASYM_line: return 2 * unit;
        default: return 0;
    }
}

AdjustSylSpacingFunctor::AdjustSylSpacingFunctor(
    Doc *doc, int voltaTrack, const std::pair<int, data_STAFFREL> &lyricLine, const std::map<int, int> &previousStaffNs)
    : DocFunctor(doc), m_lyricLine(lyricLine), m_previousStaffNs(previousStaffNs)
{
    m_previousContainer = NULL;
    m_lastSyl = NULL;
    m_previousMeasure = NULL;
    m_freeSpace = 0;
    m_voltaTrack = voltaTrack;
}

std::pair<int, data_STAFFREL> AdjustSylSpacingFunctor::GetLyricLine(
    int staffN, data_STAFFREL place, const std::map<int, int> &previousStaffNs)
{
    if (place != STAFFREL_above) return { staffN, STAFFREL_below };

    const std::map<int, int>::const_iterator iter = previousStaffNs.find(staffN);
    if (iter != previousStaffNs.end()) return { iter->second, STAFFREL_below };

    return { staffN, STAFFREL_above };
}

int AdjustSylSpacingFunctor::AdjustPreviousLyricPosition(int &overlap)
{
    const int previousX = m_previousContainer->GetDrawingXRel();
    const int nextFreeSpace = AdjustLyricPosition(m_previousContainer, overlap, m_freeSpace, m_doc);
    const int shift = m_previousContainer->GetDrawingXRel() - previousX;
    for (LayerElement *container : m_simultaneousContainers) {
        container->SetDrawingXRel(container->GetDrawingXRel() + shift);
    }
    return nextFreeSpace;
}

FunctorCode AdjustSylSpacingFunctor::VisitMeasureEnd(Measure *measure)
{
    // Process the lyric elements from left to right across staves and layers
    std::stable_sort(m_measureLyricElements.begin(), m_measureLyricElements.end(),
        [](const LyricElement *element1, const LyricElement *element2) {
            const int x1 = element1->GetAlignment() ? element1->GetAlignment()->GetXRel() : 0;
            const int x2 = element2->GetAlignment() ? element2->GetAlignment()->GetXRel() : 0;
            return (x1 < x2);
        });
    for (LyricElement *lyricElement : m_measureLyricElements) {
        this->AdjustLyricElement(lyricElement);
    }
    m_measureLyricElements.clear();

    // At the end of the measure - pass it along for overlapping verses
    m_previousMeasure = measure;

    // Adjust the position of the alignment according to what we have collected for this verse
    measure->m_measureAligner.AdjustProportionally(m_overlappingSyl);
    m_overlappingSyl.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitSystem(System *system)
{
    // reset it
    m_overlappingSyl.clear();
    m_previousContainer = NULL;
    m_simultaneousContainers.clear();
    m_previousMeasure = NULL;
    m_freeSpace = 0;
    m_systemLyricSources.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitSystemEnd(System *system)
{
    if (!m_previousMeasure) {
        return FUNCTOR_CONTINUE;
    }

    // Here we also need to handle the last syl of the measure - we check the alignment with the right barline
    if (m_previousContainer && m_lastSyl) {
        int overlap = m_lastSyl->GetContentRight() - m_previousMeasure->GetRightBarLine()->GetAlignment()->GetXRel();
        this->AdjustPreviousLyricPosition(overlap);

        // If the previous verse was not in the previous measure (but before), ignore the overlap because it is
        // not likely to go over the whole following measure
        if ((m_previousMeasure == m_previousContainer->GetFirstAncestor(MEASURE)) && (overlap > 0)) {
            m_overlappingSyl.push_back(std::make_tuple(
                m_previousContainer->GetAlignment(), m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
        }
    }

    // Adjust the position of the alignment according to what we have collected for this verse
    m_previousMeasure->m_measureAligner.AdjustProportionally(m_overlappingSyl);
    m_overlappingSyl.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitVerse(Verse *verse)
{
    return this->VisitLyricElement(verse);
}

FunctorCode AdjustSylSpacingFunctor::VisitRefrain(Refrain *refrain)
{
    return this->VisitLyricElement(refrain);
}

FunctorCode AdjustSylSpacingFunctor::VisitLyricElement(LyricElement *lyricElement)
{
    const int staffN = lyricElement->GetAncestorStaff()->GetN();
    if (GetLyricLine(staffN, lyricElement->GetPlace(), m_previousStaffNs) != m_lyricLine) return FUNCTOR_CONTINUE;

    // The lyric elements are processed at the end of the measure
    m_measureLyricElements.push_back(lyricElement);

    return FUNCTOR_CONTINUE;
}

void AdjustSylSpacingFunctor::AdjustLyricElement(LyricElement *lyricElement)
{
    const Staff *staff = lyricElement->GetAncestorStaff();
    const int staffSize = staff->m_drawingStaffSize;
    const Layer *layer = vrv_cast<const Layer *>(lyricElement->GetFirstAncestor(LAYER));
    assert(layer);

    Verse *verse = lyricElement->Is(VERSE) ? vrv_cast<Verse *>(lyricElement) : NULL;
    /****** find label / labelAbbr */

    // Label abbreviations are tracked separately for each staff and layer
    const std::pair<int, int> lyricSource{ staff->GetN(), layer->GetN() };
    LabelAbbr *&currentLabelAbbr = m_currentLabelAbbrs[lyricSource];

    // If we have a <label>, reset the previous abbreviation
    if (verse && verse->FindDescendantByType(LABEL)) {
        currentLabelAbbr = NULL;
    }

    bool newLabelAbbr = false;
    if (verse) verse->SetDrawingLabelAbbr(NULL);
    // Find the labelAbbr (if none previously given)
    if (verse && (currentLabelAbbr == NULL)) {
        currentLabelAbbr = vrv_cast<LabelAbbr *>(verse->FindDescendantByType(LABELABBR));
        // Keep indication that this is a new abbreviation and that it should not be displayed on this verse
        newLabelAbbr = true;
    }

    /*******/

    LayerElement *container = lyricElement;
    ListOfObjects syls;
    if (m_voltaTrack == 0) {
        syls = lyricElement->FindAllDescendantsByType(SYL);
        syls.remove_if([](Object *syl) { return (syl->GetFirstAncestor(VOLTA) != NULL); });
    }
    else {
        for (Object *object : lyricElement->FindAllDescendantsByType(VOLTA)) {
            Volta *volta = vrv_cast<Volta *>(object);
            assert(volta);
            if (volta->GetDrawingVoltaN() == m_voltaTrack) {
                container = volta;
                syls = volta->FindAllDescendantsByType(SYL);
                break;
            }
        }
        if (container == lyricElement) return;
    }

    int shift = m_doc->GetDrawingUnit(staffSize);
    Syl::AdjustToLyricSize(m_doc, shift);

    int previousSylShift = 0;

    const int groupingSymbolWidth = (m_voltaTrack > 0) ? GetVoltaGroupingSymbolWidth(lyricElement, shift) : 0;
    container->SetDrawingXRel(-shift);

    ListOfObjects::iterator iter = syls.begin();
    while (iter != syls.end()) {
        if ((*iter)->HasContentHorizontalBB()) {
            Syl *syl = vrv_cast<Syl *>(*iter);
            assert(syl);
            syl->SetDrawingXRel(previousSylShift);
            previousSylShift += syl->GetContentX2() + syl->CalcConnectorSpacing(m_doc, staffSize);
            ++iter;
        }
        else {
            iter = syls.erase(iter);
        }
    }

    if (syls.empty()) return;

    Syl *firstSyl = vrv_cast<Syl *>(syls.front());
    assert(firstSyl);
    // We keep a pointer to the last syl because we move it (when more than one) and the verse content bounding box is
    // not updated
    Syl *lastSyl = vrv_cast<Syl *>(syls.back());
    assert(lastSyl);

    // The abbreviation is displayed on the first syllable of each staff and layer in the system
    if (m_systemLyricSources.insert(lyricSource).second && verse && !newLabelAbbr && currentLabelAbbr) {
        verse->SetDrawingLabelAbbr(currentLabelAbbr);
    }

    // Not much to do when we hit the first syllable of the system
    if (m_previousContainer == NULL) {
        m_previousContainer = container;
        m_simultaneousContainers.clear();
        m_lastSyl = lastSyl;

        // No free space because we never move the first one back
        m_freeSpace = 0;
        m_previousMeasure = NULL;
        return;
    }

    // Lyric elements at the same position in another layer or staff are not spaced against each other: keep the one
    // extending furthest to the right. Grace notes in a layer share an alignment, so the layer is checked too.
    if (!m_previousMeasure && (container->GetAlignment() == m_previousContainer->GetAlignment())
        && (layer != m_previousContainer->GetFirstAncestor(LAYER))) {
        if (lastSyl->GetContentRight() > m_lastSyl->GetContentRight()) {
            m_simultaneousContainers.push_back(m_previousContainer);
            m_previousContainer = container;
            m_lastSyl = lastSyl;
        }
        else {
            m_simultaneousContainers.push_back(container);
        }
        return;
    }

    int xShift = 0;

    // We have a previous syllable from the previous measure - we need to add the measure width because the measures are
    // not aligned yet
    if (m_previousMeasure) {
        xShift = m_previousMeasure->GetWidth();
    }

    // Use the syl because the content bounding box of the verse might be invalid at this stage
    int overlap = m_lastSyl->GetContentRight() - (firstSyl->GetContentLeft() + xShift);
    overlap += m_lastSyl->CalcConnectorSpacing(m_doc, staffSize);
    overlap += groupingSymbolWidth;

    // Check that we also include the space for the label if the verse has a new label
    Label *label = verse ? vrv_cast<Label *>(verse->FindDescendantByType(LABEL)) : NULL;
    if (label) {
        overlap += (label->GetContentX2() - label->GetContentX1()) + m_doc->GetDrawingDoubleUnit(staffSize);
    }

    int nextFreeSpace = this->AdjustPreviousLyricPosition(overlap);

    if (overlap > 0) {
        // We are adjusting syl in two different measures - move only the right barline of the first measure
        if (m_previousMeasure) {
            m_overlappingSyl.push_back(std::make_tuple(
                m_previousContainer->GetAlignment(), m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
            // Do it now
            m_previousMeasure->m_measureAligner.AdjustProportionally(m_overlappingSyl);
            m_overlappingSyl.clear();
        }
        else {
            // Normal case, both in the same measure
            m_overlappingSyl.push_back(
                std::make_tuple(m_previousContainer->GetAlignment(), container->GetAlignment(), overlap));
        }
    }

    m_previousContainer = container;
    m_simultaneousContainers.clear();
    m_lastSyl = lastSyl;
    m_freeSpace = nextFreeSpace;
    m_previousMeasure = NULL;
}

} // namespace vrv
