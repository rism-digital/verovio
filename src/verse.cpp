/////////////////////////////////////////////////////////////////////////////
// Name:        verse.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "verse.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "staff.h"
#include "syl.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

Verse::Verse() : LayerElement("verse-"), AttColor(), AttLang(), AttNInteger(), AttTypography()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_NINTEGER);
    RegisterAttClass(ATT_TYPOGRAPHY);

    Reset();
}

Verse::~Verse() {}

void Verse::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetLang();
    ResetNInteger();
    ResetTypography();

    m_drawingLabelAbbr = NULL;
}

bool Verse::IsSupportedChild(Object *child)
{
    if (child->Is(LABEL)) {
        assert(dynamic_cast<Label *>(child));
    }
    else if (child->Is(LABELABBR)) {
        assert(dynamic_cast<LabelAbbr *>(child));
    }
    else if (child->Is(SYL)) {
        assert(dynamic_cast<Syl *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

int Verse::AdjustPosition(int &overlap, int freeSpace, Doc *doc)
{
    assert(doc);

    int nextFreeSpace = 0;

    if (overlap > 0) {
        // We have enough space to absorb the overla completely
        if (freeSpace > overlap) {
            this->SetDrawingXRel(this->GetDrawingXRel() - overlap);
            // The space is set to 0. This means that consecutive overlaps will not be recursively absorbed.
            // Only the first preceeding syl will be moved.
            overlap = 0;
        }
        else if (freeSpace > 0) {
            this->SetDrawingXRel(this->GetDrawingXRel() - freeSpace);
            overlap -= freeSpace;
        }
    }
    else {
        nextFreeSpace = std::min(-overlap, 3 * doc->GetDrawingUnit(100));
    }

    return nextFreeSpace;
}

//----------------------------------------------------------------------------
// Verse functor methods
//----------------------------------------------------------------------------

int Verse::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    // this gets (or creates) the measureAligner for the measure
    StaffAlignment *alignment = params->m_systemAligner->GetStaffAlignmentForStaffN(params->m_staffN);

    if (!alignment) return FUNCTOR_CONTINUE;

    // Add the number count
    alignment->SetVerseCount(this->GetN());

    return FUNCTOR_CONTINUE;
}

int Verse::AdjustSylSpacing(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = vrv_params_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    /****** find label / labelAbbr */

    // If we have a <label>, reset the previous abbreviation
    if (this->FindDescendantByType(LABEL)) {
        params->m_currentLabelAbbr = NULL;
    }

    bool newLabelAbbr = false;
    this->m_drawingLabelAbbr = NULL;
    // Find the labelAbbr (if none previously given)
    if (params->m_currentLabelAbbr == NULL) {
        params->m_currentLabelAbbr = dynamic_cast<LabelAbbr *>(this->FindDescendantByType(LABELABBR));
        // Keep indication that this is a new abbreviation and that is should not be displayed on this verse
        newLabelAbbr = true;
    }

    /*******/

    ListOfObjects syls;
    ClassIdComparison matchTypeSyl(SYL);
    this->FindAllDescendantByComparison(&syls, &matchTypeSyl);

    int shift = params->m_doc->GetDrawingUnit(params->m_staffSize);
    // Adjust it proportionally to the lyric size
    shift
        *= params->m_doc->GetOptions()->m_lyricSize.GetValue() / params->m_doc->GetOptions()->m_lyricSize.GetDefault();

    int previousSylShift = 0;

    this->SetDrawingXRel(-1 * shift);

    ListOfObjects::iterator iter = syls.begin();
    while (iter != syls.end()) {
        if ((*iter)->HasContentHorizontalBB()) {
            Syl *syl = vrv_cast<Syl *>(*iter);
            assert(syl);
            syl->SetDrawingXRel(previousSylShift);
            previousSylShift += syl->GetContentX2() + syl->CalcConnectorSpacing(params->m_doc, params->m_staffSize);
            ++iter;
        }
        else {
            iter = syls.erase(iter);
        }
    }

    if (syls.empty()) return FUNCTOR_CONTINUE;

    Syl *firstSyl = vrv_cast<Syl *>(syls.front());
    assert(firstSyl);
    // We keep a pointer to the last syl because we move it (when more than one) and the verse content bounding box is
    // not updated
    Syl *lastSyl = vrv_cast<Syl *>(syls.back());
    assert(lastSyl);

    // Not much to do when we hit the first syllable of the system
    if (params->m_previousVerse == NULL) {
        params->m_previousVerse = this;
        params->m_lastSyl = lastSyl;

        if (!newLabelAbbr && params->m_currentLabelAbbr) {
            this->m_drawingLabelAbbr = params->m_currentLabelAbbr;
        }

        // No free space because we never move the first one back
        params->m_freeSpace = 0;
        params->m_previousMeasure = NULL;
        return FUNCTOR_CONTINUE;
    }

    int xShift = 0;

    // We have a previous syllable from the previous measure - we need to add the measure with because the measures are
    // not aligned yet
    if (params->m_previousMeasure) {
        xShift = params->m_previousMeasure->GetWidth();
    }

    // Use the syl because the content bounding box of the verse might be invalid at this stage
    int overlap = params->m_lastSyl->GetContentRight() - (firstSyl->GetContentLeft() + xShift);
    overlap += params->m_lastSyl->CalcConnectorSpacing(params->m_doc, params->m_staffSize);

    int nextFreeSpace = params->m_previousVerse->AdjustPosition(overlap, params->m_freeSpace, params->m_doc);

    if (overlap > 0) {
        // We are adjusting syl in two different measures - move only the to right barline of the first measure
        if (params->m_previousMeasure) {
            params->m_overlapingSyl.push_back(std::make_tuple(params->m_previousVerse->GetAlignment(),
                params->m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
            // Do it now
            params->m_previousMeasure->m_measureAligner.AdjustProportionally(params->m_overlapingSyl);
            params->m_overlapingSyl.clear();
        }
        else {
            // Normal case, both in the same measure
            params->m_overlapingSyl.push_back(
                std::make_tuple(params->m_previousVerse->GetAlignment(), this->GetAlignment(), overlap));
        }
    }

    params->m_previousVerse = this;
    params->m_lastSyl = lastSyl;
    params->m_freeSpace = nextFreeSpace;
    params->m_previousMeasure = NULL;

    return FUNCTOR_CONTINUE;
}

int Verse::PrepareProcessingLists(FunctorParams *functorParams)
{
    PrepareProcessingListsParams *params = vrv_params_cast<PrepareProcessingListsParams *>(functorParams);
    assert(params);
    // StaffN_LayerN_VerseN_t *tree = vrv_cast<StaffN_LayerN_VerseN_t*>((*params).at(0));

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(staff && layer);

    params->m_verseTree.child[staff->GetN()].child[layer->GetN()].child[this->GetN()];
    // Alternate solution with StaffN_LayerN_VerseN_t
    //(*tree)[ staff->GetN() ][ layer->GetN() ][ this->GetN() ] = true;

    return FUNCTOR_SIBLINGS;
}

int Verse::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_drawingLabelAbbr = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
