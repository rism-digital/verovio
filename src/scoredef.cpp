/////////////////////////////////////////////////////////////////////////////
// Name:        scoredef.cpp
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "scoredef.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "clef.h"
#include "comparison.h"
#include "editorial.h"
#include "functorparams.h"
#include "grpsym.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "mensur.h"
#include "metersig.h"
#include "metersiggrp.h"
#include "pgfoot.h"
#include "pgfoot2.h"
#include "pghead.h"
#include "pghead2.h"
#include "section.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// ScoreDefElement
//----------------------------------------------------------------------------

ScoreDefElement::ScoreDefElement() : Object(SCOREDEF_ELEMENT, "scoredefelement-"), ScoreDefInterface(), AttTyped()
{
    this->RegisterInterface(ScoreDefInterface::GetAttClasses(), ScoreDefInterface::IsInterface());
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

ScoreDefElement::ScoreDefElement(ClassId classId) : Object(classId, "scoredefelement-"), ScoreDefInterface(), AttTyped()
{
    this->RegisterInterface(ScoreDefInterface::GetAttClasses(), ScoreDefInterface::IsInterface());
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

ScoreDefElement::ScoreDefElement(ClassId classId, const std::string &classIdStr)
    : Object(classId, classIdStr), ScoreDefInterface(), AttTyped()
{
    this->RegisterInterface(ScoreDefInterface::GetAttClasses(), ScoreDefInterface::IsInterface());
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

ScoreDefElement::~ScoreDefElement() {}

void ScoreDefElement::Reset()
{
    Object::Reset();
    ScoreDefInterface::Reset();
    this->ResetTyped();
}

bool ScoreDefElement::HasClefInfo(int depth) const
{
    return (this->FindDescendantByType(CLEF, depth));
}

bool ScoreDefElement::HasKeySigInfo(int depth) const
{
    return (this->FindDescendantByType(KEYSIG, depth));
}

bool ScoreDefElement::HasMensurInfo(int depth) const
{
    return (this->FindDescendantByType(MENSUR, depth));
}

bool ScoreDefElement::HasMeterSigInfo(int depth) const
{
    return (this->FindDescendantByType(METERSIG, depth));
}

bool ScoreDefElement::HasMeterSigGrpInfo(int depth) const
{
    return (this->FindDescendantByType(METERSIGGRP, depth));
}

Clef *ScoreDefElement::GetClef()
{
    return const_cast<Clef *>(std::as_const(*this).GetClef());
}

const Clef *ScoreDefElement::GetClef() const
{
    // Always check if HasClefInfo() is true before asking for it
    const Clef *clef = vrv_cast<const Clef *>(this->FindDescendantByType(CLEF, 1));
    assert(clef);
    return clef;
}

Clef *ScoreDefElement::GetClefCopy() const
{
    // Always check if HasClefInfo() is true before asking for a clone
    Clef *clone = dynamic_cast<Clef *>(this->GetClef()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

KeySig *ScoreDefElement::GetKeySig()
{
    return const_cast<KeySig *>(std::as_const(*this).GetKeySig());
}

const KeySig *ScoreDefElement::GetKeySig() const
{
    // Always check if HasKeySigInfo() is true before asking for it
    const KeySig *keySig = vrv_cast<const KeySig *>(this->FindDescendantByType(KEYSIG, 1));
    assert(keySig);
    return keySig;
}

KeySig *ScoreDefElement::GetKeySigCopy() const
{
    // Always check if HasKeySigInfo() is true before asking for a clone
    KeySig *clone = dynamic_cast<KeySig *>(this->GetKeySig()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

Mensur *ScoreDefElement::GetMensur()
{
    return const_cast<Mensur *>(std::as_const(*this).GetMensur());
}

const Mensur *ScoreDefElement::GetMensur() const
{
    // Always check if HasMensurInfo() is true before asking for it
    const Mensur *mensur = vrv_cast<const Mensur *>(this->FindDescendantByType(MENSUR, 1));
    assert(mensur);
    return mensur;
}

Mensur *ScoreDefElement::GetMensurCopy() const
{
    // Always check if HasMensurInfo() is true before asking for a clone
    Mensur *clone = dynamic_cast<Mensur *>(this->GetMensur()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

MeterSig *ScoreDefElement::GetMeterSig()
{
    return const_cast<MeterSig *>(std::as_const(*this).GetMeterSig());
}

const MeterSig *ScoreDefElement::GetMeterSig() const
{
    // Always check if HasMeterSigInfo() is true before asking for it
    const MeterSig *meterSig = vrv_cast<const MeterSig *>(this->FindDescendantByType(METERSIG, 1));
    assert(meterSig);
    return meterSig;
}

MeterSig *ScoreDefElement::GetMeterSigCopy() const
{
    // Always check if HasMeterSigInfo() is true before asking for a clone
    MeterSig *clone = dynamic_cast<MeterSig *>(this->GetMeterSig()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

MeterSigGrp *ScoreDefElement::GetMeterSigGrp()
{
    return const_cast<MeterSigGrp *>(std::as_const(*this).GetMeterSigGrp());
}

const MeterSigGrp *ScoreDefElement::GetMeterSigGrp() const
{
    // Always check if HasMeterSigGrpInfo() is true before asking for it
    const MeterSigGrp *meterSigGrp = vrv_cast<const MeterSigGrp *>(this->FindDescendantByType(METERSIGGRP, 1));
    assert(meterSigGrp);
    return meterSigGrp;
}

MeterSigGrp *ScoreDefElement::GetMeterSigGrpCopy() const
{
    // Always check if HasMeterSigGrpInfo() is true before asking for a clone
    MeterSigGrp *clone = dynamic_cast<MeterSigGrp *>(this->GetMeterSigGrp()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

static const ClassRegistrar<ScoreDef> s_factory("scoreDef", SCOREDEF);

ScoreDef::ScoreDef()
    : ScoreDefElement(SCOREDEF, "scoredef-")
    , ObjectListInterface()
    , AttDistances()
    , AttEndings()
    , AttOptimization()
    , AttTimeBase()
{
    this->RegisterAttClass(ATT_DISTANCES);
    this->RegisterAttClass(ATT_ENDINGS);
    this->RegisterAttClass(ATT_OPTIMIZATION);
    this->RegisterAttClass(ATT_TIMEBASE);

    this->Reset();
}

ScoreDef::~ScoreDef() {}

void ScoreDef::Reset()
{
    ScoreDefElement::Reset();
    this->ResetDistances();
    this->ResetEndings();
    this->ResetOptimization();

    m_drawLabels = false;
    m_drawingWidth = 0;
    m_drawingLabelsWidth = 0;
    m_setAsDrawing = false;
}

bool ScoreDef::IsSupportedChild(Object *child)
{
    if (child->Is(CLEF)) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is(GRPSYM)) {
        assert(dynamic_cast<GrpSym *>(child));
    }
    else if (child->Is(KEYSIG)) {
        assert(dynamic_cast<KeySig *>(child));
    }
    else if (child->Is(STAFFGRP)) {
        assert(dynamic_cast<StaffGrp *>(child));
    }
    else if (child->Is(MENSUR)) {
        assert(dynamic_cast<Mensur *>(child));
    }
    else if (child->Is(METERSIG)) {
        assert(dynamic_cast<MeterSig *>(child));
    }
    else if (child->Is(METERSIGGRP)) {
        assert(dynamic_cast<MeterSigGrp *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else if (child->IsRunningElement()) {
        assert(dynamic_cast<RunningElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void ScoreDef::ReplaceDrawingValues(ScoreDef *newScoreDef)
{
    assert(newScoreDef);

    m_insertScoreDef = false;
    m_setAsDrawing = true;

    int redrawFlags = 0;
    Clef const *clef = NULL;
    KeySig const *keySig = NULL;
    Mensur *mensur = NULL;
    MeterSig *meterSig = NULL;
    MeterSigGrp *meterSigGrp = NULL;

    if (newScoreDef->HasClefInfo()) {
        redrawFlags |= StaffDefRedrawFlags::REDRAW_CLEF;
        clef = newScoreDef->GetClef();
    }
    if (newScoreDef->HasKeySigInfo()) {
        redrawFlags |= StaffDefRedrawFlags::REDRAW_KEYSIG;
        keySig = newScoreDef->GetKeySig();
    }
    if (newScoreDef->HasMensurInfo()) {
        redrawFlags |= StaffDefRedrawFlags::REDRAW_MENSUR;
        mensur = newScoreDef->GetMensurCopy();
    }
    if (newScoreDef->HasMeterSigGrpInfo()) {
        redrawFlags &= ~(StaffDefRedrawFlags::REDRAW_MENSUR);
        redrawFlags |= StaffDefRedrawFlags::REDRAW_METERSIGGRP;
        meterSigGrp = newScoreDef->GetMeterSigGrp();
        meterSig = meterSigGrp->GetSimplifiedMeterSig();
    }
    else if (newScoreDef->HasMeterSigInfo()) {
        redrawFlags |= StaffDefRedrawFlags::REDRAW_METERSIG;
        meterSig = newScoreDef->GetMeterSigCopy();
    }

    ReplaceDrawingValuesInStaffDefParams replaceDrawingValuesInStaffDefParams(
        clef, keySig, mensur, meterSig, meterSigGrp);
    Functor replaceDrawingValuesInScoreDef(&Object::ReplaceDrawingValuesInStaffDef);
    this->Process(&replaceDrawingValuesInScoreDef, &replaceDrawingValuesInStaffDefParams);

    if (mensur) delete mensur;
    if (meterSig) delete meterSig;

    this->SetRedrawFlags(redrawFlags);
}

void ScoreDef::ReplaceDrawingValues(StaffDef *newStaffDef)
{
    assert(newStaffDef);

    // first find the staffDef with the same @n
    StaffDef *staffDef = this->GetStaffDef(newStaffDef->GetN());

    // if found, replace attributes
    if (staffDef) {
        if (newStaffDef->HasClefInfo()) {
            staffDef->SetDrawClef(true);
            Clef const *clef = newStaffDef->GetClef();
            staffDef->SetCurrentClef(clef);
        }
        if (newStaffDef->HasKeySigInfo()) {
            staffDef->SetDrawKeySig(true);
            KeySig const *keySig = newStaffDef->GetKeySig();
            staffDef->SetCurrentKeySig(keySig);
        }
        if (newStaffDef->HasMensurInfo()) {
            staffDef->SetDrawMensur(true);
            // Never draw a mensur AND a meterSig
            staffDef->SetDrawMeterSig(false);
            Mensur *mensur = newStaffDef->GetMensurCopy();
            staffDef->SetCurrentMensur(mensur);
            delete mensur;
        }
        if (newStaffDef->HasMeterSigGrpInfo()) {
            staffDef->SetDrawMeterSigGrp(true);
            // Never draw a meterSig AND a mensur
            staffDef->SetDrawMeterSig(false);
            staffDef->SetDrawMensur(false);
            MeterSigGrp *meterSigGrp = newStaffDef->GetMeterSigGrpCopy();
            MeterSig *meterSig = meterSigGrp->GetSimplifiedMeterSig();
            delete meterSigGrp;
            staffDef->SetCurrentMeterSig(meterSig);
            delete meterSig;
        }
        else if (newStaffDef->HasMeterSigInfo()) {
            MeterSig *meterSig = newStaffDef->GetMeterSigCopy();
            if (newStaffDef->HasMensurInfo()) {
                // If there is a mensur and the meterSig
                // is invisible, then print mensur instead
                data_METERFORM meterForm = meterSig->GetForm();
                if (meterForm == METERFORM_invis) {
                    staffDef->SetDrawMeterSig(false);
                    staffDef->SetDrawMensur(true);
                    Mensur *mensur = newStaffDef->GetMensurCopy();
                    staffDef->SetCurrentMensur(mensur);
                    // Invisible meterSig is still needed for mRest:
                    staffDef->SetCurrentMeterSig(meterSig);
                    delete mensur;
                }
                else {
                    staffDef->SetDrawMeterSig(true);
                    staffDef->SetDrawMensur(false);
                    staffDef->SetCurrentMeterSig(meterSig);
                }
            }
            else {
                staffDef->SetDrawMeterSig(true);
                staffDef->SetDrawMensur(false);
                staffDef->SetCurrentMeterSig(meterSig);
            }
            delete meterSig;
        }
        // copy other attributes if present
        if (newStaffDef->HasLabel()) staffDef->SetLabel(newStaffDef->GetLabel());
    }
    else {
        LogWarning("StaffDef with xml:id '%s' could not be found", newStaffDef->GetUuid().c_str());
    }
}

void ScoreDef::ReplaceDrawingLabels(StaffGrp *newStaffGrp)
{
    assert(newStaffGrp);

    // first find the staffGrp with the same @n
    StaffGrp *staffGrp = this->GetStaffGrp(newStaffGrp->GetN());
    if (staffGrp) {
        // Replace with thew new label only if we have one
        if (newStaffGrp->HasLabelInfo()) {
            Label *label = newStaffGrp->GetLabelCopy();
            // Check if we previoulsy had one, and replace it if yes
            if (staffGrp->HasLabelInfo()) {
                Label *oldLabel = staffGrp->GetLabel();
                staffGrp->ReplaceChild(oldLabel, label);
                delete oldLabel;
            }
            // Otherwise simply add it
            else {
                staffGrp->AddChild(label);
            }
        }
        if (newStaffGrp->HasLabelAbbrInfo()) {
            LabelAbbr *labelAbbr = newStaffGrp->GetLabelAbbrCopy();
            // Check if we previoulsy had one, and replace it if yes
            if (staffGrp->HasLabelAbbrInfo()) {
                LabelAbbr *oldLabelAbbr = staffGrp->GetLabelAbbr();
                staffGrp->ReplaceChild(oldLabelAbbr, labelAbbr);
                delete oldLabelAbbr;
            }
            // Otherwise simply add it
            else {
                staffGrp->AddChild(labelAbbr);
            }
        }
    }
}

void ScoreDef::FilterList(ArrayOfObjects *childList)
{
    // We want to keep only staffDef
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->Is(STAFFDEF)) {
            iter = childList->erase(iter);
        }
        else {
            ++iter;
        }
    }
}

StaffDef *ScoreDef::GetStaffDef(int n)
{
    this->ResetList(this);
    const ArrayOfObjects *childList = this->GetList(this);
    ArrayOfObjects::const_iterator iter;

    StaffDef *staffDef = NULL;
    for (iter = childList->begin(); iter != childList->end(); ++iter) {
        if (!(*iter)->Is(STAFFDEF)) continue;
        staffDef = vrv_cast<StaffDef *>(*iter);
        assert(staffDef);
        if (staffDef->GetN() == n) {
            return staffDef;
        }
    }

    return staffDef;
}

StaffGrp *ScoreDef::GetStaffGrp(const std::string &n)
{
    // First get all the staffGrps
    ListOfObjects staffGrps = this->FindAllDescendantsByType(STAFFGRP);

    // Then the @n of each first staffDef
    for (auto &item : staffGrps) {
        StaffGrp *staffGrp = vrv_cast<StaffGrp *>(item);
        assert(staffGrp);
        if (staffGrp->GetN() == n) return staffGrp;
    }
    return NULL;
}

std::vector<int> ScoreDef::GetStaffNs()
{
    this->ResetList(this);
    const ArrayOfObjects *childList = this->GetList(this);
    ArrayOfObjects::const_iterator iter;

    std::vector<int> ns;
    StaffDef *staffDef = NULL;
    for (iter = childList->begin(); iter != childList->end(); ++iter) {
        // It should be staffDef only, but double check.
        if (!(*iter)->Is(STAFFDEF)) continue;
        staffDef = vrv_cast<StaffDef *>(*iter);
        assert(staffDef);
        ns.push_back(staffDef->GetN());
    }
    return ns;
}

void ScoreDef::SetRedrawFlags(int redrawFlags)
{
    m_setAsDrawing = true;
    SetStaffDefRedrawFlagsParams setStaffDefRedrawFlagsParams;
    setStaffDefRedrawFlagsParams.m_redrawFlags = redrawFlags;
    Functor setStaffDefDraw(&Object::SetStaffDefRedrawFlags);
    this->Process(&setStaffDefDraw, &setStaffDefRedrawFlagsParams);
}

void ScoreDef::SetDrawingWidth(int drawingWidth)
{
    m_drawingWidth = drawingWidth;
}

void ScoreDef::SetDrawingLabelsWidth(int width)
{
    if (m_drawingLabelsWidth < width) {
        m_drawingLabelsWidth = width;
    }
}

PgFoot *ScoreDef::GetPgFoot()
{
    return dynamic_cast<PgFoot *>(this->FindDescendantByType(PGFOOT));
}

const PgFoot *ScoreDef::GetPgFoot() const
{
    return dynamic_cast<const PgFoot *>(this->FindDescendantByType(PGFOOT));
}

PgFoot2 *ScoreDef::GetPgFoot2()
{
    return dynamic_cast<PgFoot2 *>(this->FindDescendantByType(PGFOOT2));
}

const PgFoot2 *ScoreDef::GetPgFoot2() const
{
    return dynamic_cast<const PgFoot2 *>(this->FindDescendantByType(PGFOOT2));
}

PgHead *ScoreDef::GetPgHead()
{
    return dynamic_cast<PgHead *>(this->FindDescendantByType(PGHEAD));
}

const PgHead *ScoreDef::GetPgHead() const
{
    return dynamic_cast<const PgHead *>(this->FindDescendantByType(PGHEAD));
}

PgHead2 *ScoreDef::GetPgHead2()
{
    return dynamic_cast<PgHead2 *>(this->FindDescendantByType(PGHEAD2));
}

const PgHead2 *ScoreDef::GetPgHead2() const
{
    return dynamic_cast<const PgHead2 *>(this->FindDescendantByType(PGHEAD2));
}

int ScoreDef::GetMaxStaffSize()
{
    StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(this->FindDescendantByType(STAFFGRP));
    return (staffGrp) ? staffGrp->GetMaxStaffSize() : 100;
}

bool ScoreDef::IsSectionRestart() const
{
    if (!this->GetParent()) return false;
    // In page-based structure, Section is a sibling to scoreDef
    // This has limitations: will not work with editorial markup, additional nested sections, and
    // if the section milestone is in the previous system.
    const Section *section = dynamic_cast<const Section *>(this->GetParent()->GetPrevious(this, SECTION));
    return (section && (section->GetRestart() == BOOLEAN_true));
}

bool ScoreDef::HasSystemStartLine()
{
    StaffGrp *staffGrp = vrv_cast<StaffGrp *>(this->FindDescendantByType(STAFFGRP));
    if (staffGrp) {
        auto [firstDef, lastDef] = staffGrp->GetFirstLastStaffDef();
        if ((firstDef && lastDef && (firstDef != lastDef)) || staffGrp->GetFirst(GRPSYM)) {
            return (this->GetSystemLeftline() != BOOLEAN_false);
        }
        return (this->GetSystemLeftline() == BOOLEAN_true);
    }
    return false;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int ScoreDef::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    m_drawingLabelsWidth = 0;

    return FUNCTOR_CONTINUE;
}

int ScoreDef::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    // Move itself to the pageBasedSystem - do not process children
    assert(params->m_currentSystem);
    this->MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int ScoreDef::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = vrv_params_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    assert(params->m_targetSystem);
    this->MoveItselfTo(params->m_targetSystem);

    return FUNCTOR_CONTINUE;
}

int ScoreDef::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    ScoreDef *scoreDef = dynamic_cast<ScoreDef *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingElements.push_back(scoreDef);
    // This is not perfect since now the scoreDefWith is the one of the intermediate scoreDefs (and not
    // the initial one - for this to be corrected, we would need two parameters, one for the current initial
    // scoreDef and one for the current that will be the initial one at the next system
    // Also, the abbr label (width) changes would not be taken into account
    params->m_currentScoreDefWidth = this->GetDrawingWidth() + params->m_contentSystem->GetDrawingAbbrLabelsWidth();

    return FUNCTOR_SIBLINGS;
}

int ScoreDef::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int ScoreDef::InitSelection(FunctorParams *functorParams)
{
    InitSelectionParams *params = vrv_params_cast<InitSelectionParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int ScoreDef::AlignMeasures(FunctorParams *functorParams)
{
    AlignMeasuresParams *params = vrv_params_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    params->m_shift += m_drawingLabelsWidth;

    if (params->m_applySectionRestartShift) {
        ClassIdsComparison comparison({ LABEL, LABELABBR });
        if (this->FindDescendantByComparison(&comparison)) {
            params->m_applySectionRestartShift = false;
        }
    }

    return FUNCTOR_CONTINUE;
}

int ScoreDef::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = vrv_params_cast<JustifyXParams *>(functorParams);
    assert(params);

    if (m_drawingLabelsWidth > 0) {
        params->m_measureXRel += m_drawingLabelsWidth;
        params->m_applySectionRestartShift = false;
    }

    return FUNCTOR_SIBLINGS;
}

int ScoreDef::PrepareDuration(FunctorParams *functorParams)
{
    PrepareDurationParams *params = vrv_params_cast<PrepareDurationParams *>(functorParams);
    assert(params);

    params->m_durDefaultForStaffN.clear();
    params->m_durDefault = this->GetDurDefault();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
