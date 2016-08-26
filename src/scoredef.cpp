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
#include "keysig.h"
#include "mensur.h"
#include "metersig.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// ScoreDefElement
//----------------------------------------------------------------------------

ScoreDefElement::ScoreDefElement(std::string classid) : Object(classid), ScoreDefInterface()
{
    RegisterInterface(ScoreDefInterface::GetAttClasses(), ScoreDefInterface::IsInterface());

    Reset();
}

ScoreDefElement::~ScoreDefElement()
{
}

void ScoreDefElement::Reset()
{
    Object::Reset();
    ScoreDefInterface::Reset();
}

bool ScoreDefElement::HasClefInfo() const
{
    if (this->HasClefAttrInfo()) return true;
    return (this->HasClefElementInfo());
}

bool ScoreDefElement::HasKeySigInfo() const
{
    if (this->HasKeySigAttrInfo()) return true;
    return (this->HasKeySigElementInfo());
}

bool ScoreDefElement::HasMensurInfo() const
{
    if (this->HasMensurAttrInfo()) return true;
    return (this->HasMensurElementInfo());
}

bool ScoreDefElement::HasMeterSigInfo() const
{
    if (this->HasMeterSigAttrInfo()) return true;
    return (this->HasMeterSigElementInfo());
}

bool ScoreDefElement::HasClefAttrInfo() const
{
    // We need at least a @clef.shape and a @clef.line ?
    return (this->HasClefShape() && this->HasClefLine());

    // Eventually we can look for a child clef element
    // We would probably need to take into account app and rdg?
    return false;
}

bool ScoreDefElement::HasKeySigAttrInfo() const
{
    return (this->HasKeySig());
}

bool ScoreDefElement::HasMensurAttrInfo() const
{
    // What is the minimum we need? Checking only some for now. Need clarification
    return (this->HasProlatio() || this->HasTempus() || this->HasProportNum() || this->HasProportNumbase());
}

bool ScoreDefElement::HasMeterSigAttrInfo() const
{
    return (this->HasMeterCount() || this->HasMeterSym() || this->HasMeterUnit());
}

bool ScoreDefElement::HasClefElementInfo() const
{
    // Eventually we can look for a child clef element
    // We would probably need to take into account app and rdg?
    return false;
}

bool ScoreDefElement::HasKeySigElementInfo() const
{
    return false;
}

bool ScoreDefElement::HasMensurElementInfo() const
{
    return false;
}

bool ScoreDefElement::HasMeterSigElementInfo() const
{
    return false;
}

Clef *ScoreDefElement::GetClefCopy() const
{
    Clef *copy = NULL;
    if (this->HasClefAttrInfo()) {
        copy = new Clef(this);
    }
    else if (this->HasClefElementInfo()) {
        // Eventually return a copy of the child element;
    }
    // Always check if HasClefInfo() is true before asking for a copy
    assert(copy);
    copy->SetScoreOrStaffDefAttr(true);
    return copy;
}

KeySig *ScoreDefElement::GetKeySigCopy() const
{
    KeySig *copy = NULL;
    if (this->HasKeySigAttrInfo()) {
        copy = new KeySig(this);
    }
    else {
        // Eventually return a copy of the child element;
    }
    // Always check if HasKeySigInfo() is true before asking for a copy
    assert(copy);
    copy->SetScoreOrStaffDefAttr(true);
    return copy;
}

Mensur *ScoreDefElement::GetMensurCopy() const
{
    Mensur *copy = NULL;
    if (this->HasMensurAttrInfo()) {
        copy = new Mensur(this);
    }
    else {
        // Eventually return a copy of the child element;
    }
    // Always check if HasMensurInfo() is true before asking for a copy
    assert(copy);
    copy->SetScoreOrStaffDefAttr(true);
    return copy;
}

MeterSig *ScoreDefElement::GetMeterSigCopy() const
{
    MeterSig *copy = NULL;
    if (this->HasMeterSigAttrInfo()) {
        copy = new MeterSig(this);
    }
    else {
        // Eventually return a copy of the child element;
    }
    // Always check if HasMeterSigInfo() is true before asking for a copy
    assert(copy);
    copy->SetScoreOrStaffDefAttr(true);
    return copy;
}

//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

ScoreDef::ScoreDef() : ScoreDefElement("scoredef-"), ObjectListInterface(), AttEndings()
{
    RegisterAttClass(ATT_ENDINGS);

    Reset();
}

ScoreDef::~ScoreDef()
{
}

void ScoreDef::Reset()
{
    ScoreDefElement::Reset();
    ResetEndings();

    m_drawLabels = false;
    m_drawingWidth = 0;
    m_setAsDrawing = false;
}

void ScoreDef::AddChild(Object *child)
{
    if (child->Is() == STAFFGRP) {
        assert(dynamic_cast<StaffGrp *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void ScoreDef::ReplaceDrawingValues(ScoreDef *newScoreDef)
{
    assert(newScoreDef);

    m_setAsDrawing = true;

    bool drawClef = false;
    bool drawKeySig = false;
    bool drawMensur = false;
    bool drawMeterSig = false;
    Clef *clef = NULL;
    KeySig *keySig = NULL;
    Mensur *mensur = NULL;
    MeterSig *meterSig = NULL;

    if (newScoreDef->HasClefInfo()) {
        drawClef = true;
        clef = newScoreDef->GetClefCopy();
    }
    if (newScoreDef->HasKeySigInfo()) {
        drawKeySig = true;
        keySig = newScoreDef->GetKeySigCopy();
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

    if (clef) delete clef;
    if (keySig) delete keySig;
    if (mensur) delete mensur;
    if (meterSig) delete meterSig;

    // The keySig cancellation flag is the same as keySig because we draw cancellation with new key sig
    this->SetRedrawFlags(drawClef, drawKeySig, drawMensur, drawMeterSig, drawKeySig, false);
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
            Clef *clef = newStaffDef->GetClefCopy();
            staffDef->SetCurrentClef(clef);
            delete clef;
        }
        if (newStaffDef->HasKeySigInfo()) {
            staffDef->SetDrawKeySig(true);
            staffDef->SetDrawKeySigCancellation(true);
            KeySig *keySig = newStaffDef->GetKeySigCopy();
            staffDef->SetCurrentKeySig(keySig);
            delete keySig;
        }
        if (newStaffDef->HasMensurInfo()) {
            staffDef->SetDrawMensur(true);
            Mensur *mensur = newStaffDef->GetMensurCopy();
            staffDef->SetCurrentMensur(mensur);
            delete mensur;
        }
        if (newStaffDef->HasMeterSigInfo()) {
            staffDef->SetDrawMeterSig(true);
            MeterSig *meterSig = newStaffDef->GetMeterSigCopy();
            staffDef->SetCurrentMeterSig(meterSig);
            delete meterSig;
        }
        // copy other attributes if present
        if (newStaffDef->HasLabel()) staffDef->SetLabel(newStaffDef->GetLabel());
        if (newStaffDef->HasLabelAbbr()) staffDef->SetLabelAbbr(newStaffDef->GetLabelAbbr());
    }
    else {
        LogWarning("StaffDef with xml:id '%s' could not be found", newStaffDef->GetUuid().c_str());
    }
}

void ScoreDef::FilterList(ListOfObjects *childList)
{
    // We want to keep only staffDef
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if ((*iter)->Is() != STAFFDEF) {
            iter = childList->erase(iter);
        }
        else {
            iter++;
        }
    }
}

StaffDef *ScoreDef::GetStaffDef(int n)
{
    StaffDef *staffDef = NULL;

    this->ResetList(this);
    ListOfObjects *childList = this->GetList(this);
    ListOfObjects::iterator iter;

    for (iter = childList->begin(); iter != childList->end(); ++iter) {
        if ((*iter)->Is() != STAFFDEF) continue;
        staffDef = dynamic_cast<StaffDef *>(*iter);
        assert(staffDef);
        if (staffDef->GetN() == n) {
            return staffDef;
        }
    }

    return staffDef;
}

void ScoreDef::SetRedrawFlags(
    bool clef, bool keySig, bool mensur, bool meterSig, bool keySigCancellation, bool applyToAll)
{
    m_setAsDrawing = true;

    SetStaffDefRedrawFlagsParams setStaffDefRedrawFlagsParams;
    setStaffDefRedrawFlagsParams.m_clef = clef;
    setStaffDefRedrawFlagsParams.m_keySig = keySig;
    setStaffDefRedrawFlagsParams.m_mensur = mensur;
    setStaffDefRedrawFlagsParams.m_meterSig = meterSig;
    setStaffDefRedrawFlagsParams.m_keySigCancellation = keySigCancellation;
    setStaffDefRedrawFlagsParams.m_applyToAll = applyToAll;
    Functor setStaffDefDraw(&Object::SetStaffDefRedrawFlags);
    this->Process(&setStaffDefDraw, &setStaffDefRedrawFlagsParams);
}

void ScoreDef::SetDrawingWidth(int drawingWidth)
{
    m_drawingWidth = drawingWidth;
}

//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

StaffGrp::StaffGrp()
    : Object()
    , ObjectListInterface()
    , AttCommon()
    , AttCommonPart()
    , AttLabelsAddl()
    , AttStaffgroupingsym()
    , AttStaffGrpVis()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_COMMONPART);
    RegisterAttClass(ATT_LABELSADDL);
    RegisterAttClass(ATT_STAFFGROUPINGSYM);
    RegisterAttClass(ATT_STAFFGRPVIS);

    Reset();
}

StaffGrp::~StaffGrp()
{
}

void StaffGrp::Reset()
{
    Object::Reset();
    ResetCommon();
    ResetCommonPart();
    ResetLabelsAddl();
    ResetStaffgroupingsym();
    ResetStaffGrpVis();
}

void StaffGrp::AddChild(Object *child)
{
    if (child->Is() == STAFFDEF) {
        assert(dynamic_cast<StaffDef *>(child));
    }
    else if (child->Is() == STAFFGRP) {
        assert(dynamic_cast<StaffGrp *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void StaffGrp::FilterList(ListOfObjects *childList)
{
    // We want to keep only staffDef
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if ((*iter)->Is() != STAFFDEF) {
            iter = childList->erase(iter);
        }
        else {
            iter++;
        }
    }
}

//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

StaffDef::StaffDef()
    : ScoreDefElement("staffdef-")
    , AttCommon()
    , AttCommonPart()
    , AttLabelsAddl()
    , AttNotationtype()
    , AttScalable()
    , AttStaffDefVis()
    , AttTransposition()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_COMMONPART);
    RegisterAttClass(ATT_LABELSADDL);
    RegisterAttClass(ATT_NOTATIONTYPE);
    RegisterAttClass(ATT_SCALABLE);
    RegisterAttClass(ATT_STAFFDEFVIS);
    RegisterAttClass(ATT_TRANSPOSITION);

    Reset();
}

StaffDef::~StaffDef()
{
}

void StaffDef::Reset()
{
    ScoreDefElement::Reset();
    StaffDefDrawingInterface::Reset();
    ResetCommon();
    ResetCommonPart();
    ResetLabelsAddl();
    ResetNotationtype();
    ResetScalable();
    ResetStaffDefVis();
    ResetTransposition();
}

//----------------------------------------------------------------------------
// ScoreDef functor methods
//----------------------------------------------------------------------------

int ScoreDef::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = dynamic_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    // Move itself to the pageBasedSystem - do not process children
    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_SIBLINGS;
}

int ScoreDef::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

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
    CastOffEncodingParams *params = dynamic_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// StaffDef functor methods
//----------------------------------------------------------------------------

int StaffDef::ReplaceDrawingValuesInStaffDef(FunctorParams *functorParams)
{
    ReplaceDrawingValuesInStaffDefParams *params = dynamic_cast<ReplaceDrawingValuesInStaffDefParams *>(functorParams);
    assert(params);

    if (params->m_clef) {
        this->SetCurrentClef(params->m_clef);
    }
    if (params->m_keySig) {
        this->SetCurrentKeySig(params->m_keySig);
    }
    if (params->m_mensur) {
        this->SetCurrentMensur(params->m_mensur);
    }
    if (params->m_meterSig) {
        this->SetCurrentMeterSig(params->m_meterSig);
    }

    return FUNCTOR_CONTINUE;
}

int StaffDef::SetStaffDefRedrawFlags(FunctorParams *functorParams)
{
    SetStaffDefRedrawFlagsParams *params = dynamic_cast<SetStaffDefRedrawFlagsParams *>(functorParams);
    assert(params);

    if (params->m_clef || params->m_applyToAll) {
        this->SetDrawClef(params->m_clef);
    }
    if (params->m_keySig || params->m_applyToAll) {
        this->SetDrawKeySig(params->m_keySig);
    }
    if (params->m_mensur || params->m_applyToAll) {
        this->SetDrawMensur(params->m_mensur);
    }
    if (params->m_meterSig || params->m_applyToAll) {
        this->SetDrawMeterSig(params->m_meterSig);
    }
    if (params->m_keySigCancellation || params->m_applyToAll) {
        this->SetDrawKeySigCancellation(params->m_keySigCancellation);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
