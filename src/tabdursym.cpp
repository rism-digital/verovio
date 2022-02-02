/////////////////////////////////////////////////////////////////////////////
// Name:        tabdursym.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabdursym.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "elementpart.h"
#include "functorparams.h"
#include "layer.h"
#include "staff.h"

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

    ArrayOfObjects *children = this->GetChildrenForModification();

    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(STEM)) {
        children->insert(children->begin(), child);
    }
    else {
        children->push_back(child);
    }
    Modify();
}

void TabDurSym::AdjustDrawingYRel(Staff *staff, Doc *doc)
{
    assert(staff);
    assert(doc);

    int yRel = (staff->m_drawingLines - 1) * doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    double spacingRatio = (staff->IsTabLuteFrench()) ? 2.0 : 1.0;
    yRel += doc->GetDrawingUnit(staff->m_drawingStaffSize) * spacingRatio;

    this->SetDrawingYRel(-yRel);
}

Point TabDurSym::GetStemUpSE(Doc *doc, int staffSize, bool isCueSize)
{
    Point p(0, 0);
    return p;
}

Point TabDurSym::GetStemDownNW(Doc *doc, int staffSize, bool isCueSize)
{
    Point p(0, 0);
    return p;
}

int TabDurSym::CalcStemLenInThirdUnits(Staff *staff, data_STEMDIRECTION stemDir)
{
    assert(staff);

    if ((stemDir != STEMDIRECTION_down) && (stemDir != STEMDIRECTION_up)) {
        return 0;
    }

    int baseStem = STANDARD_STEMLENGTH_TAB * 3;

    if (!staff->IsTabWithBeamOutside()) baseStem += 6;

    return baseStem;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int TabDurSym::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Stem *currentStem = dynamic_cast<Stem *>(this->FindDescendantByType(STEM, 1));

    if (!currentStem) {
        currentStem = new Stem();
        this->AddChild(currentStem);
    }
    this->SetDrawingStem(currentStem);

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
    params->m_stemSameas = false;

    int staffSize = params->m_staff->m_drawingStaffSize;

    /************ Set the direction ************/

    data_STEMDIRECTION layerStemDir;
    // Up by default with tablature
    data_STEMDIRECTION stemDir = STEMDIRECTION_up;

    if (stem->HasStemDir()) {
        stemDir = stem->GetStemDir();
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

    // Make sure the relative position of the stem is the same
    stem->SetDrawingYRel(0);
    int stemSize = this->CalcStemLenInThirdUnits(params->m_staff, stemDir) * params->m_doc->GetDrawingUnit(staffSize);
    stemSize /= (3 * stemDirFactor);

    if (params->m_dur == DUR_2) {
        stemSize /= 2;
    }

    stem->SetDrawingStemLen(stemSize);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
