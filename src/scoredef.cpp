/////////////////////////////////////////////////////////////////////////////
// Name:        scoredef.cpp
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "scoredef.h"

//----------------------------------------------------------------------------

#include <cassert>

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

//----------------------------------------------------------------------------

#include "MidiEvent.h"
#include "MidiFile.h"

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
    , AttScoreDefGes()
    , AttTimeBase()
{
    this->RegisterAttClass(ATT_DISTANCES);
    this->RegisterAttClass(ATT_ENDINGS);
    this->RegisterAttClass(ATT_OPTIMIZATION);
    this->RegisterAttClass(ATT_SCOREDEFGES);
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
    this->ResetScoreDefGes();

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

void ScoreDef::ReplaceDrawingValues(const ScoreDef *newScoreDef)
{
    assert(newScoreDef);

    m_insertScoreDef = false;
    m_setAsDrawing = true;

    int redrawFlags = 0;
    const Clef *clef = NULL;
    const KeySig *keySig = NULL;
    Mensur *mensur = NULL;
    MeterSig *meterSig = NULL;
    const MeterSigGrp *meterSigGrp = NULL;

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

void ScoreDef::ReplaceDrawingValues(const StaffDef *newStaffDef)
{
    assert(newStaffDef);

    // first find the staffDef with the same @n
    StaffDef *staffDef = this->GetStaffDef(newStaffDef->GetN());

    // if found, replace attributes
    if (staffDef) {
        if (newStaffDef->HasClefInfo()) {
            staffDef->SetDrawClef(true);
            const Clef *clef = newStaffDef->GetClef();
            staffDef->SetCurrentClef(clef);
        }
        if (newStaffDef->HasKeySigInfo()) {
            staffDef->SetDrawKeySig(true);
            const KeySig *keySig = newStaffDef->GetKeySig();
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
            staffDef->SetCurrentMeterSigGrp(meterSigGrp);
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
        LogWarning("StaffDef with xml:id '%s' could not be found", newStaffDef->GetID().c_str());
    }
}

void ScoreDef::ReplaceDrawingLabels(const StaffGrp *newStaffGrp)
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

void ScoreDef::FilterList(ListOfConstObjects &childList) const
{
    // We want to keep only staffDef
    ListOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        if (!(*iter)->Is(STAFFDEF)) {
            iter = childList.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

void ScoreDef::ResetFromDrawingValues()
{
    const ListOfObjects &childList = this->GetList(this);

    StaffDef *staffDef = NULL;
    for (auto item : childList) {
        if (!item->Is(STAFFDEF)) continue;
        staffDef = vrv_cast<StaffDef *>(item);
        assert(staffDef);

        Clef *clef = vrv_cast<Clef *>(staffDef->FindDescendantByType(CLEF));
        if (clef) *clef = *staffDef->GetCurrentClef();

        KeySig *keySig = vrv_cast<KeySig *>(staffDef->FindDescendantByType(KEYSIG));
        if (keySig) *keySig = *staffDef->GetCurrentKeySig();

        Mensur *mensur = vrv_cast<Mensur *>(staffDef->FindDescendantByType(MENSUR));
        if (mensur) *mensur = *staffDef->GetCurrentMensur();

        MeterSigGrp *meterSigGrp = vrv_cast<MeterSigGrp *>(staffDef->FindDescendantByType(METERSIGGRP));
        MeterSig *meterSig = vrv_cast<MeterSig *>(staffDef->FindDescendantByType(METERSIG));
        if (meterSigGrp) {
            *meterSigGrp = *staffDef->GetCurrentMeterSigGrp();
        }
        else if (meterSig) {
            *meterSig = *staffDef->GetCurrentMeterSig();
        }
    }
}

StaffDef *ScoreDef::GetStaffDef(int n)
{
    return const_cast<StaffDef *>(std::as_const(*this).GetStaffDef(n));
}

const StaffDef *ScoreDef::GetStaffDef(int n) const
{
    const ListOfConstObjects &childList = this->GetList(this);
    ListOfConstObjects::const_iterator iter;

    const StaffDef *staffDef = NULL;
    for (iter = childList.begin(); iter != childList.end(); ++iter) {
        if (!(*iter)->Is(STAFFDEF)) continue;
        staffDef = vrv_cast<const StaffDef *>(*iter);
        assert(staffDef);
        if (staffDef->GetN() == n) {
            return staffDef;
        }
    }

    return staffDef;
}

StaffGrp *ScoreDef::GetStaffGrp(const std::string &n)
{
    return const_cast<StaffGrp *>(std::as_const(*this).GetStaffGrp(n));
}

const StaffGrp *ScoreDef::GetStaffGrp(const std::string &n) const
{
    // First get all the staffGrps
    ListOfConstObjects staffGrps = this->FindAllDescendantsByType(STAFFGRP);

    // Then the @n of each first staffDef
    for (auto &item : staffGrps) {
        const StaffGrp *staffGrp = vrv_cast<const StaffGrp *>(item);
        assert(staffGrp);
        if (staffGrp->GetN() == n) return staffGrp;
    }
    return NULL;
}

std::vector<int> ScoreDef::GetStaffNs() const
{
    const ListOfConstObjects &childList = this->GetList(this);
    ListOfConstObjects::const_iterator iter;

    std::vector<int> ns;
    const StaffDef *staffDef = NULL;
    for (iter = childList.begin(); iter != childList.end(); ++iter) {
        // It should be staffDef only, but double check.
        if (!(*iter)->Is(STAFFDEF)) continue;
        staffDef = vrv_cast<const StaffDef *>(*iter);
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

int ScoreDef::GetMaxStaffSize() const
{
    const StaffGrp *staffGrp = vrv_cast<const StaffGrp *>(this->FindDescendantByType(STAFFGRP));
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

bool ScoreDef::HasSystemStartLine() const
{
    const StaffGrp *staffGrp = vrv_cast<const StaffGrp *>(this->FindDescendantByType(STAFFGRP));
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

int ScoreDefElement::ConvertMarkupScoreDef(FunctorParams *functorParams)
{
    ConvertMarkupScoreDefParams *params = vrv_params_cast<ConvertMarkupScoreDefParams *>(functorParams);
    assert(params);

    if (this->Is(SCOREDEF)) {
        params->m_currentScoreDef = this;
        return FUNCTOR_CONTINUE;
    }

    // This should never be the case
    if (!this->Is(STAFFDEF) || !params->m_currentScoreDef) return FUNCTOR_CONTINUE;

    ScoreDefElement *scoreDef = params->m_currentScoreDef;

    // Copy score definition elements to the staffDef but only if they are not given at the staffDef
    // This might require more refined merging because we can lose data if some staffDef values are defined
    // but do not contain all the ones given in the scoreDef (e.g. @key.mode in scoreDef but not in a staffDef with
    // @key.sig)
    if (scoreDef->HasClefInfo() && !this->HasClefInfo()) {
        this->AddChild(scoreDef->GetClefCopy());
    }
    if (scoreDef->HasKeySigInfo() && !this->HasKeySigInfo()) {
        this->AddChild(scoreDef->GetKeySigCopy());
    }
    if (scoreDef->HasMeterSigGrpInfo() && !this->HasMeterSigGrpInfo()) {
        this->AddChild(scoreDef->GetMeterSigGrpCopy());
    }
    if (scoreDef->HasMeterSigInfo() && !this->HasMeterSigInfo()) {
        this->AddChild(scoreDef->GetMeterSigCopy());
    }
    if (scoreDef->HasMensurInfo() && !this->HasMensurInfo()) {
        this->AddChild(scoreDef->GetMensurCopy());
    }

    return FUNCTOR_CONTINUE;
}

int ScoreDefElement::ConvertMarkupScoreDefEnd(FunctorParams *functorParams)
{
    ConvertMarkupScoreDefParams *params = vrv_params_cast<ConvertMarkupScoreDefParams *>(functorParams);
    assert(params);

    if (!this->Is(SCOREDEF)) return FUNCTOR_CONTINUE;

    // At the end of the scoreDef, remove all score definition elements
    if (this->HasClefInfo()) {
        Object *clef = this->FindDescendantByType(CLEF, 1);
        if (clef) this->DeleteChild(clef);
    }
    if (this->HasKeySigInfo()) {
        Object *keySig = this->FindDescendantByType(KEYSIG, 1);
        if (keySig) this->DeleteChild(keySig);
    }
    if (this->HasMeterSigGrpInfo()) {
        Object *meterSigGrp = this->FindDescendantByType(METERSIGGRP, 1);
        if (meterSigGrp) this->DeleteChild(meterSigGrp);
    }
    if (this->HasMeterSigInfo()) {
        Object *meterSig = this->FindDescendantByType(METERSIG, 1);
        if (meterSig) this->DeleteChild(meterSig);
    }
    if (this->HasMensurInfo()) {
        Object *mensur = this->FindDescendantByType(MENSUR, 1);
        if (mensur) this->DeleteChild(mensur);
    }

    params->m_currentScoreDef = NULL;

    return FUNCTOR_CONTINUE;
}

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

int ScoreDef::CastOffToSelection(FunctorParams *functorParams)
{
    CastOffToSelectionParams *params = vrv_params_cast<CastOffToSelectionParams *>(functorParams);
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

int ScoreDef::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    double totalTime = params->m_totalTime;
    // check next measure for the time offset
    Object *parent = this->GetParent();
    if (parent && (parent->GetLast() != this)) {
        Object *next = parent->GetNext(this);
        if (next && next->Is(MEASURE)) {
            Measure *nextMeasure = vrv_cast<Measure *>(next);
            totalTime = nextMeasure->GetLastTimeOffset();
        }
    }
    const double currentTick = totalTime * params->m_midiFile->getTPQ();

    smf::MidiEvent midiEvent;
    midiEvent.tick = currentTick;
    // calculate reference pitch class based on @tune.pname
    int referencePitchClass = 0;
    if (this->HasTunePname()) {
        referencePitchClass = Note::PnameToPclass(this->GetTunePname());
    }
    // set temperament event if corresponding attribute present
    if (this->HasTuneTemper()) {
        switch (this->GetTuneTemper()) {
            case TEMPERAMENT_equal: midiEvent.makeTemperamentEqual(referencePitchClass); break;
            case TEMPERAMENT_just: midiEvent.makeTemperamentBad(100.0, referencePitchClass); break;
            case TEMPERAMENT_mean: midiEvent.makeTemperamentMeantone(referencePitchClass); break;
            case TEMPERAMENT_pythagorean: midiEvent.makeTemperamentPythagorean(referencePitchClass); break;
            default: break;
        }
        params->m_midiFile->addEvent(params->m_midiTrack, midiEvent);
    }
    // set tuning
    if (this->HasTuneHz()) {
        const double tuneHz = this->GetTuneHz();
        // Add tuning for all keys from 0 to 127
        std::vector<std::pair<int, double>> tuneFrequencies;
        for (int i = 0; i < 127; i++) {
            double freq = pow(2.0, (i - 69.0) / 12.0) * tuneHz;
            tuneFrequencies.push_back(std::make_pair(i, freq));
        }
        midiEvent.makeMts2_KeyTuningsByFrequency(tuneFrequencies);
        params->m_midiFile->addEvent(params->m_midiTrack, midiEvent);
    }
    // set MIDI key signature
    if (this->HasKeySigInfo()) {
        KeySig *keySig = vrv_cast<KeySig *>(this->GetKeySig());
        if (keySig && keySig->HasSig()) {
            params->m_midiFile->addKeySignature(
                params->m_midiTrack, currentTick, keySig->GetFifthsInt(), (keySig->GetMode() == MODE_minor));
        }
    }
    // set MIDI time signature
    if (this->HasMeterSigInfo()) {
        MeterSig *meterSig = vrv_cast<MeterSig *>(this->GetMeterSig());
        if (meterSig && meterSig->HasCount()) {
            params->m_midiFile->addTimeSignature(
                params->m_midiTrack, currentTick, meterSig->GetTotalCount(), meterSig->GetUnit());
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

int ScoreDef::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    if (params->m_transposeToSoundingPitch) {
        // Set the transposition in order to transpose common key signatures
        // (i.e. encoded as ScoreDef attributes or direct KeySig children)
        const std::vector<int> staffNs = this->GetStaffNs();
        if (staffNs.empty()) {
            int transposeInterval = 0;
            if (!params->m_transposeIntervalForStaffN.empty()) {
                transposeInterval = params->m_transposeIntervalForStaffN.begin()->second;
            }
            params->m_transposer->SetTransposition(transposeInterval);
        }
        else {
            this->GetStaffDef(staffNs.front())->Transpose(functorParams);
        }
    }

    return FUNCTOR_CONTINUE;
}

int ScoreDef::TransposeEnd(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    const bool hasScoreDefKeySig = (params->m_keySigForStaffN.count(-1) > 0);
    if (params->m_transposeToSoundingPitch && hasScoreDefKeySig) {
        bool showWarning = false;
        // Check if some staves are untransposed
        const int mapEntryCount = static_cast<int>(params->m_transposeIntervalForStaffN.size());
        if ((mapEntryCount > 0) && (mapEntryCount < (int)this->GetStaffNs().size())) {
            showWarning = true;
        }
        // Check if there are different transpositions
        auto iter = std::adjacent_find(params->m_transposeIntervalForStaffN.begin(),
            params->m_transposeIntervalForStaffN.end(),
            [](const auto &mapEntry1, const auto &mapEntry2) { return (mapEntry1.second != mapEntry2.second); });
        if (iter != params->m_transposeIntervalForStaffN.end()) {
            showWarning = true;
        }
        // Display warning
        if (showWarning) {
            LogWarning("Transpose to sounding pitch cannot handle different transpositions for ScoreDef key "
                       "signatures. Please encode KeySig as StaffDef attribute or child.");
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
