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
#include "editorial.h"
#include "functorparams.h"
#include "grpsym.h"
#include "keysig.h"
#include "label.h"
#include "mensur.h"
#include "metersig.h"
#include "pgfoot.h"
#include "pgfoot2.h"
#include "pghead.h"
#include "pghead2.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// ScoreDefElement
//----------------------------------------------------------------------------

ScoreDefElement::ScoreDefElement(const std::string &classid) : Object(classid), ScoreDefInterface(), AttTyped()
{
    RegisterInterface(ScoreDefInterface::GetAttClasses(), ScoreDefInterface::IsInterface());
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ScoreDefElement::~ScoreDefElement() {}

void ScoreDefElement::Reset()
{
    Object::Reset();
    ScoreDefInterface::Reset();
    ResetTyped();
}

bool ScoreDefElement::HasClefInfo()
{
    return (this->FindDescendantByType(CLEF, 1));
}

bool ScoreDefElement::HasKeySigInfo()
{
    return (this->FindDescendantByType(KEYSIG, 1));
}

bool ScoreDefElement::HasMensurInfo()
{
    return (this->FindDescendantByType(MENSUR, 1));
}

bool ScoreDefElement::HasMeterSigInfo()
{
    return (this->FindDescendantByType(METERSIG, 1));
}

Clef *ScoreDefElement::GetClef()
{
    // Always check if HasClefInfo() is true before asking for it
    Clef *clef = vrv_cast<Clef *>(this->FindDescendantByType(CLEF, 1));
    assert(clef);
    return clef;
}

Clef *ScoreDefElement::GetClefCopy()
{
    // Always check if HasClefInfo() is true before asking for a clone
    Clef *clone = dynamic_cast<Clef *>(this->GetClef()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

KeySig *ScoreDefElement::GetKeySig()
{
    // Always check if HasKeySigInfo() is true before asking for it
    KeySig *keySig = vrv_cast<KeySig *>(this->FindDescendantByType(KEYSIG, 1));
    assert(keySig);
    return keySig;
}

KeySig *ScoreDefElement::GetKeySigCopy()
{
    // Always check if HasKeySigInfo() is true before asking for a clone
    KeySig *clone = dynamic_cast<KeySig *>(this->GetKeySig()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

Mensur *ScoreDefElement::GetMensur()
{
    // Always check if HasMensurInfo() is true before asking for it
    Mensur *mensur = vrv_cast<Mensur *>(this->FindDescendantByType(MENSUR, 1));
    assert(mensur);
    return mensur;
}

Mensur *ScoreDefElement::GetMensurCopy()
{
    // Always check if HasMensurInfo() is true before asking for a clone
    Mensur *clone = dynamic_cast<Mensur *>(this->GetMensur()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

MeterSig *ScoreDefElement::GetMeterSig()
{
    // Always check if HasMeterSigInfo() is true before asking for it
    MeterSig *meterSig = vrv_cast<MeterSig *>(this->FindDescendantByType(METERSIG, 1));
    assert(meterSig);
    return meterSig;
}

MeterSig *ScoreDefElement::GetMeterSigCopy()
{
    // Always check if HasMeterSigInfo() is true before asking for a clone
    MeterSig *clone = dynamic_cast<MeterSig *>(this->GetMeterSig()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

ScoreDef::ScoreDef()
    : ScoreDefElement("scoredef-"), ObjectListInterface(), AttDistances(), AttEndings(), AttOptimization()
{
    RegisterAttClass(ATT_DISTANCES);
    RegisterAttClass(ATT_ENDINGS);
    RegisterAttClass(ATT_OPTIMIZATION);

    Reset();
}

ScoreDef::~ScoreDef() {}

void ScoreDef::Reset()
{
    ScoreDefElement::Reset();
    ResetDistances();
    ResetEndings();
    ResetOptimization();

    m_drawLabels = false;
    m_drawingWidth = 0;
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

    m_setAsDrawing = true;

    bool drawClef = false;
    bool drawKeySig = false;
    bool drawMensur = false;
    bool drawMeterSig = false;
    Clef const *clef = NULL;
    KeySig const *keySig = NULL;
    Mensur *mensur = NULL;
    MeterSig *meterSig = NULL;

    if (newScoreDef->HasClefInfo()) {
        drawClef = true;
        clef = newScoreDef->GetClef();
    }
    if (newScoreDef->HasKeySigInfo()) {
        drawKeySig = true;
        keySig = newScoreDef->GetKeySig();
    }
    if (newScoreDef->HasMensurInfo()) {
        drawMensur = true;
        mensur = newScoreDef->GetMensurCopy();
    }
    if (newScoreDef->HasMeterSigInfo()) {
        drawMeterSig = true;
        meterSig = newScoreDef->GetMeterSigCopy();
    }

    ReplaceDrawingValuesInStaffDefParams replaceDrawingValuesInStaffDefParams(clef, keySig, mensur, meterSig);
    Functor replaceDrawingValuesInScoreDef(&Object::ReplaceDrawingValuesInStaffDef);
    this->Process(&replaceDrawingValuesInScoreDef, &replaceDrawingValuesInStaffDefParams);

    if (mensur) delete mensur;
    if (meterSig) delete meterSig;

    this->SetRedrawFlags(drawClef, drawKeySig, drawMensur, drawMeterSig, false);
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
        if (newStaffDef->HasMeterSigInfo()) {
            staffDef->SetDrawMeterSig(true);
            // Never draw a meterSig AND a mensur
            staffDef->SetDrawMensur(false);
            MeterSig *meterSig = newStaffDef->GetMeterSigCopy();
            staffDef->SetCurrentMeterSig(meterSig);
            delete meterSig;
        }
        // copy other attributes if present
        if (newStaffDef->HasLabel()) staffDef->SetLabel(newStaffDef->GetLabel());
    }
    else {
        LogWarning("StaffDef with xml:id '%s' could not be found", newStaffDef->GetUuid().c_str());
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

void ScoreDef::SetRedrawFlags(bool clef, bool keySig, bool mensur, bool meterSig, bool applyToAll)
{
    m_setAsDrawing = true;

    SetStaffDefRedrawFlagsParams setStaffDefRedrawFlagsParams;
    setStaffDefRedrawFlagsParams.m_clef = clef;
    setStaffDefRedrawFlagsParams.m_keySig = keySig;
    setStaffDefRedrawFlagsParams.m_mensur = mensur;
    setStaffDefRedrawFlagsParams.m_meterSig = meterSig;
    setStaffDefRedrawFlagsParams.m_applyToAll = applyToAll;
    Functor setStaffDefDraw(&Object::SetStaffDefRedrawFlags);
    this->Process(&setStaffDefDraw, &setStaffDefRedrawFlagsParams);
}

void ScoreDef::SetDrawingWidth(int drawingWidth)
{
    m_drawingWidth = drawingWidth;
}

PgFoot *ScoreDef::GetPgFoot()
{
    return dynamic_cast<PgFoot *>(this->FindDescendantByType(PGFOOT));
}

PgFoot2 *ScoreDef::GetPgFoot2()
{
    return dynamic_cast<PgFoot2 *>(this->FindDescendantByType(PGFOOT2));
}

PgHead *ScoreDef::GetPgHead()
{
    return dynamic_cast<PgHead *>(this->FindDescendantByType(PGHEAD));
}

PgHead2 *ScoreDef::GetPgHead2()
{
    return dynamic_cast<PgHead2 *>(this->FindDescendantByType(PGHEAD2));
}

int ScoreDef::GetMaxStaffSize()
{
    StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(this->FindDescendantByType(STAFFGRP));
    return (staffGrp) ? staffGrp->GetMaxStaffSize() : 100;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int ScoreDef::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    // Move itself to the pageBasedSystem - do not process children
    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_SIBLINGS;
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
    params->m_pendingObjects.push_back(scoreDef);
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

} // namespace vrv
