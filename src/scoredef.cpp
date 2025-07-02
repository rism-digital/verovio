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
#include "functor.h"
#include "grpsym.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "mensur.h"
#include "metersig.h"
#include "metersiggrp.h"
#include "pgfoot.h"
#include "pghead.h"
#include "section.h"
#include "setscoredeffunctor.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "symboltable.h"
#include "system.h"
#include "tempo.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiEvent.h"
#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// ScoreDefElement
//----------------------------------------------------------------------------

ScoreDefElement::ScoreDefElement() : Object(SCOREDEF_ELEMENT), ScoreDefInterface(), AttTyped()
{
    this->RegisterInterface(ScoreDefInterface::GetAttClasses(), ScoreDefInterface::IsInterface());
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

ScoreDefElement::ScoreDefElement(ClassId classId) : Object(classId), ScoreDefInterface(), AttTyped()
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
    : ScoreDefElement(SCOREDEF)
    , ObjectListInterface()
    , AttDistances()
    , AttEndings()
    , AttOptimization()
    , AttTimeBase()
    , AttTuning()
{
    this->RegisterAttClass(ATT_DISTANCES);
    this->RegisterAttClass(ATT_ENDINGS);
    this->RegisterAttClass(ATT_OPTIMIZATION);
    this->RegisterAttClass(ATT_TIMEBASE);
    this->RegisterAttClass(ATT_TUNING);

    this->Reset();
}

ScoreDef::~ScoreDef() {}

void ScoreDef::Reset()
{
    ScoreDefElement::Reset();
    this->ResetDistances();
    this->ResetEndings();
    this->ResetOptimization();
    this->ResetTimeBase();
    this->ResetTuning();

    m_drawLabels = false;
    m_drawingWidth = 0;
    m_drawingLabelsWidth = 0;
    m_setAsDrawing = false;
}

bool ScoreDef::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ CLEF, GRPSYM, KEYSIG, MENSUR, METERSIG, METERSIGGRP, STAFFGRP,
        SYMBOLTABLE };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsRunningElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

bool ScoreDef::AddChildAdditionalCheck(Object *child)
{
    // Clef and mensur are actually not allowed as children of scoreDef in MEI.
    // Left as a warning for now.
    if (child->Is(CLEF) && !child->IsAttribute()) {
        LogWarning("Having <clef> as child of <scoreDef> is not valid MEI");
    }
    else if (child->Is(MENSUR) && !child->IsAttribute()) {
        LogWarning("Having <mensur> as child of <scoreDef> is not valid MEI");
    }
    return (ScoreDefElement::AddChildAdditionalCheck(child));
}

int ScoreDef::GetInsertOrderFor(ClassId classId) const
{

    static const std::vector s_order(
        { SYMBOLTABLE, CLEF, KEYSIG, METERSIGGRP, METERSIG, MENSUR, PGHEAD, PGFOOT, STAFFGRP, GRPSYM });
    return this->GetInsertOrderForIn(classId, s_order);
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

    ReplaceDrawingValuesInStaffDefFunctor replaceDrawingValuesInStaffDef(clef, keySig, mensur, meterSig, meterSigGrp);
    this->Process(replaceDrawingValuesInStaffDef);

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
                if (meterSig->GetVisible() == BOOLEAN_false) {
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
    const ListOfObjects &childList = this->GetList();

    StaffDef *staffDef = NULL;
    for (Object *object : childList) {
        if (!object->Is(STAFFDEF)) continue;
        staffDef = vrv_cast<StaffDef *>(object);
        assert(staffDef);

        Clef *clef = vrv_cast<Clef *>(staffDef->FindDescendantByType(CLEF));
        if (clef) clef->ReplaceWithCopyOf(staffDef->GetCurrentClef());

        KeySig *keySig = vrv_cast<KeySig *>(staffDef->FindDescendantByType(KEYSIG));
        if (keySig) keySig->ReplaceWithCopyOf(staffDef->GetCurrentKeySig());

        Mensur *mensur = vrv_cast<Mensur *>(staffDef->FindDescendantByType(MENSUR));
        if (mensur) mensur->ReplaceWithCopyOf(staffDef->GetCurrentMensur());

        MeterSigGrp *meterSigGrp = vrv_cast<MeterSigGrp *>(staffDef->FindDescendantByType(METERSIGGRP));
        MeterSig *meterSig = vrv_cast<MeterSig *>(staffDef->FindDescendantByType(METERSIG));
        if (meterSigGrp) {
            meterSigGrp->ReplaceWithCopyOf(staffDef->GetCurrentMeterSigGrp());
        }
        else if (meterSig) {
            meterSig->ReplaceWithCopyOf(staffDef->GetCurrentMeterSig());
        }
    }
}

StaffDef *ScoreDef::GetStaffDef(int n)
{
    return const_cast<StaffDef *>(std::as_const(*this).GetStaffDef(n));
}

const StaffDef *ScoreDef::GetStaffDef(int n) const
{
    const ListOfConstObjects &childList = this->GetList();

    const StaffDef *staffDef = NULL;
    for (const Object *child : childList) {
        if (!child->Is(STAFFDEF)) continue;
        staffDef = vrv_cast<const StaffDef *>(child);
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
    for (const Object *object : staffGrps) {
        const StaffGrp *staffGrp = vrv_cast<const StaffGrp *>(object);
        assert(staffGrp);
        if (staffGrp->GetN() == n) return staffGrp;
    }
    return NULL;
}

std::vector<int> ScoreDef::GetStaffNs() const
{
    const ListOfConstObjects &childList = this->GetList();

    std::vector<int> ns;
    const StaffDef *staffDef = NULL;
    for (const Object *child : childList) {
        // It should be staffDef only, but double check.
        if (!child->Is(STAFFDEF)) continue;
        staffDef = vrv_cast<const StaffDef *>(child);
        assert(staffDef);
        ns.push_back(staffDef->GetN());
    }
    return ns;
}

void ScoreDef::SetRedrawFlags(int redrawFlags)
{
    m_setAsDrawing = true;
    SetStaffDefRedrawFlagsFunctor setStaffDefRedrawFlags(redrawFlags);
    this->Process(setStaffDefRedrawFlags);
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

PgFoot *ScoreDef::GetPgFoot(data_PGFUNC func)
{
    AttFormeworkComparison comparison(PGFOOT, func);
    return vrv_cast<PgFoot *>(this->FindDescendantByComparison(&comparison));
}

const PgFoot *ScoreDef::GetPgFoot(data_PGFUNC func) const
{
    AttFormeworkComparison comparison(PGFOOT, func);
    return vrv_cast<const PgFoot *>(this->FindDescendantByComparison(&comparison));
}

PgHead *ScoreDef::GetPgHead(data_PGFUNC func)
{
    AttFormeworkComparison comparison(PGHEAD, func);
    return vrv_cast<PgHead *>(this->FindDescendantByComparison(&comparison));
}

const PgHead *ScoreDef::GetPgHead(data_PGFUNC func) const
{
    AttFormeworkComparison comparison(PGHEAD, func);
    return vrv_cast<const PgHead *>(this->FindDescendantByComparison(&comparison));
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
    const Section *section = vrv_cast<const Section *>(this->GetParent()->GetPrevious(this, SECTION));
    return (section && (section->GetRestart() == BOOLEAN_true));
}

bool ScoreDef::HasSystemStartLine() const
{
    const StaffGrp *staffGrp = vrv_cast<const StaffGrp *>(this->FindDescendantByType(STAFFGRP));
    if (staffGrp) {
        auto [firstDef, lastDef] = staffGrp->GetFirstLastStaffDef();
        ListOfConstObjects allDefs = staffGrp->FindAllDescendantsByType(STAFFDEF);
        if ((firstDef && lastDef && allDefs.size() > 1) || staffGrp->GetFirst(GRPSYM)) {
            return (this->GetSystemLeftline() != BOOLEAN_false);
        }
        return (this->GetSystemLeftline() == BOOLEAN_true);
    }
    return false;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode ScoreDefElement::Accept(Functor &functor)
{
    return functor.VisitScoreDefElement(this);
}

FunctorCode ScoreDefElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitScoreDefElement(this);
}

FunctorCode ScoreDefElement::AcceptEnd(Functor &functor)
{
    return functor.VisitScoreDefElementEnd(this);
}

FunctorCode ScoreDefElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitScoreDefElementEnd(this);
}

FunctorCode ScoreDef::Accept(Functor &functor)
{
    return functor.VisitScoreDef(this);
}

FunctorCode ScoreDef::Accept(ConstFunctor &functor) const
{
    return functor.VisitScoreDef(this);
}

FunctorCode ScoreDef::AcceptEnd(Functor &functor)
{
    return functor.VisitScoreDefEnd(this);
}

FunctorCode ScoreDef::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitScoreDefEnd(this);
}

} // namespace vrv
