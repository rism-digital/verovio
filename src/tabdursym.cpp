/////////////////////////////////////////////////////////////////////////////
// Name:        tabdursym.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabdursym.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "elementpart.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "stem.h"
#include "tabgrp.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabDurSym
//----------------------------------------------------------------------------

static const ClassRegistrar<TabDurSym> s_factory("tabDurSym", TABDURSYM);

TabDurSym::TabDurSym() : LayerElement(TABDURSYM, "tabdursym-"), StemmedDrawingInterface(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

TabDurSym::~TabDurSym() {}

void TabDurSym::Reset()
{
    LayerElement::Reset();
    StemmedDrawingInterface::Reset();
    this->ResetNNumberLike();
}

bool TabDurSym::IsSupportedChild(Object *child)
{
    if (child->Is(STEM)) {
        assert(dynamic_cast<Stem *>(child));
    }
    else {
        return false;
    }
    return true;
}

void TabDurSym::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);

    ArrayOfObjects &children = this->GetChildrenForModification();

    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(STEM)) {
        children.insert(children.begin(), child);
    }
    else {
        children.push_back(child);
    }
    Modify();
}

void TabDurSym::AdjustDrawingYRel(const Staff *staff, const Doc *doc)
{
    assert(staff);
    assert(doc);

    int yRel = (staff->m_drawingLines - 1) * doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    // For stems outside add a margin to the tabDurSym - otherwise attached to the staff line
    if (staff->IsTabWithStemsOutside()) {
        double spacingRatio = (staff->IsTabLuteFrench()) ? 2.0 : 1.0;
        yRel += doc->GetDrawingUnit(staff->m_drawingStaffSize) * spacingRatio;
    }

    this->SetDrawingYRel(-yRel);
}

Point TabDurSym::GetStemUpSE(const Doc *doc, int staffSize, bool isCueSize) const
{
    return Point();
}

Point TabDurSym::GetStemDownNW(const Doc *doc, int staffSize, bool isCueSize) const
{
    return Point();
}

int TabDurSym::CalcStemLenInThirdUnits(const Staff *staff, data_STEMDIRECTION stemDir) const
{
    assert(staff);

    if ((stemDir != STEMDIRECTION_down) && (stemDir != STEMDIRECTION_up)) {
        return 0;
    }

    int baseStem = STANDARD_STEMLENGTH_TAB * 3;

    // One unit longer for guitar tablature
    if (staff->IsTabGuitar()) baseStem += 3;

    // One unit longer for stems inside the staff
    if (!staff->IsTabWithStemsOutside()) baseStem += 3;

    return baseStem;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int TabDurSym::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Stem *currentStem = vrv_cast<Stem *>(this->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    if (currentStem) currentFlag = vrv_cast<Flag *>(currentStem->GetFirst(FLAG));

    if (!currentStem) {
        currentStem = new Stem();
        currentStem->IsAttribute(true);
        this->AddChild(currentStem);
    }
    this->SetDrawingStem(currentStem);

    /************ flags ***********/

    TabGrp *tabGrp = vrv_cast<TabGrp *>(this->GetFirstAncestor(TABGRP));
    assert(tabGrp);

    // No flag within beam of for durations longer than 8th notes
    if (!this->IsInBeam() && tabGrp->GetActualDur() > DUR_4) {
        // We must have a stem at this stage
        assert(currentStem);
        if (!currentFlag) {
            currentFlag = new Flag();
            currentStem->AddChild(currentFlag);
        }
    }
    // This will happen only if the duration has changed (no flag required anymore)
    else if (currentFlag) {
        assert(currentStem);
        if (currentStem->DeleteChild(currentFlag)) currentFlag = NULL;
    }

    return FUNCTOR_SIBLINGS;
}

int TabDurSym::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    // Stems have been calculated previously in Beam
    if (this->IsInBeam()) {
        return FUNCTOR_SIBLINGS;
    }

    // This now need should be NULL and the chord stem length will be 0
    params->m_interface = NULL;
    params->m_chordStemLength = 0;

    Stem *stem = this->GetDrawingStem();
    assert(stem);

    // Do not draw virtual (e.g., whole note) stems
    if (params->m_dur < DUR_2 || params->m_tabGrpWithNoNote) {
        stem->IsVirtual(true);
        return FUNCTOR_SIBLINGS;
    }

    // Cache the in params to avoid further lookup
    params->m_staff = this->GetAncestorStaff();
    assert(params->m_staff);
    params->m_layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(params->m_layer);
    params->m_interface = this;
    // Grace an stem sameas not supported in tablature
    params->m_isGraceNote = false;
    params->m_isStemSameasSecondary = false;

    int staffSize = params->m_staff->m_drawingStaffSize;

    /************ Set the direction ************/

    data_STEMDIRECTION layerStemDir;
    // Up by default with tablature
    data_STEMDIRECTION stemDir = STEMDIRECTION_up;

    if (stem->HasDir()) {
        stemDir = stem->GetDir();
    }
    else if ((layerStemDir = params->m_layer->GetDrawingStemDir()) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
    }

    this->SetDrawingStemDir(stemDir);

    int stemDirFactor = -1;
    if (stemDir == STEMDIRECTION_down) {
        this->AdjustDrawingYRel(params->m_staff, params->m_doc);
        stemDirFactor = 1;
    }

    if (params->m_staff->IsTabWithStemsOutside()) {
        // Make sure the relative position of the stem is the same
        stem->SetDrawingYRel(0);
    }
    else {
        // Other wise attach it to the closest note
        TabGrp *tabGrp = vrv_cast<TabGrp *>(this->GetFirstAncestor(TABGRP));
        assert(tabGrp);
        Note *note = (stemDir == STEMDIRECTION_down) ? tabGrp->GetBottomNote() : tabGrp->GetTopNote();
        int yRel = (note) ? note->GetDrawingYRel() : 0;
        // Because the tabDurSym is relative to the top or bottom staff line, remove its relative value
        yRel -= this->GetDrawingYRel();
        // Remove a unit for the stem not to go to the center of the note
        yRel -= params->m_doc->GetDrawingUnit(params->m_staff->m_drawingStaffSize) * stemDirFactor;
        stem->SetDrawingYRel(yRel);
    }

    int stemSize = this->CalcStemLenInThirdUnits(params->m_staff, stemDir) * params->m_doc->GetDrawingUnit(staffSize);
    stemSize /= (3 * stemDirFactor);

    if (params->m_dur == DUR_2) {
        // Stems for half notes twice shorter
        stemSize /= 2;
    }

    stem->SetDrawingStemLen(stemSize);

    // Flag currently used only for gitar tablature because it is included in the glyphs for lute tab
    if (params->m_staff->IsTabGuitar()) {
        Flag *flag = vrv_cast<Flag *>(stem->GetFirst(FLAG));
        if (flag) {
            flag->m_drawingNbFlags = params->m_dur - DUR_4;
            flag->SetDrawingYRel(-stemSize);
        }
    }

    // Do not call Stem::CalcStem with TabDurSym because everything is done here
    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
