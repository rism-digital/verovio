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
#include "keysig.h"
#include "mensur.h"
#include "metersig.h"
#include "system.h"

namespace vrv {

//----------------------------------------------------------------------------
// ScoreOrStaffDefAttrInterface
//----------------------------------------------------------------------------

ScoreOrStaffDefAttrInterface::ScoreOrStaffDefAttrInterface()
{
    // owned pointers need to be set to NULL;
    m_clef = NULL;
    m_keySig = NULL;
    m_mensur = NULL;
    m_meterSig = NULL;
    Reset();
}

ScoreOrStaffDefAttrInterface::~ScoreOrStaffDefAttrInterface()
{
    if (m_clef) {
        delete m_clef;
    }
    if (m_keySig) {
        delete m_keySig;
    }
    if (m_mensur) {
        delete m_mensur;
    }
    if (m_meterSig) {
        delete m_meterSig;
    }
}

void ScoreOrStaffDefAttrInterface::Reset()
{
    if (m_clef) {
        delete m_clef;
        m_clef = NULL;
    }
    if (m_keySig) {
        delete m_keySig;
        m_keySig = NULL;
    }
    if (m_mensur) {
        delete m_mensur;
        m_mensur = NULL;
    }
    if (m_meterSig) {
        delete m_meterSig;
        m_meterSig = NULL;
    }
}
    
ScoreOrStaffDefAttrInterface::ScoreOrStaffDefAttrInterface( const ScoreOrStaffDefAttrInterface& interface )
{
    m_clef = NULL;
    m_keySig = NULL;
    m_mensur = NULL;
    m_meterSig = NULL;
    this->ReplaceClef( interface.m_clef );
    this->ReplaceKeySig( interface.m_keySig );
    this->ReplaceMensur( interface.m_mensur );
    this->ReplaceMeterSig( interface.m_meterSig );
}

ScoreOrStaffDefAttrInterface& ScoreOrStaffDefAttrInterface::operator=( const ScoreOrStaffDefAttrInterface& interface )
{
	if ( this != &interface ) // not self assignement
	{
        if (m_clef) {
            delete m_clef;
            m_clef = NULL;
        }
        if (m_keySig) {
            delete m_keySig;
            m_keySig = NULL;
        }
        if (m_mensur) {
            delete m_mensur;
            m_mensur = NULL;
        }
        if (m_meterSig) {
            delete m_meterSig;
            m_meterSig = NULL;
        }        
        this->ReplaceClef( interface.m_clef );
        this->ReplaceKeySig( interface.m_keySig );
        this->ReplaceMensur( interface.m_mensur );
        this->ReplaceMeterSig( interface.m_meterSig );
	}
	return *this;
}

void ScoreOrStaffDefAttrInterface::ReplaceClef( Object *newClef )
{
    if ( newClef ) {
        assert( dynamic_cast<ClefAttr*>(newClef) || dynamic_cast<Clef*>(newClef) );
        if (m_clef) {
            delete m_clef;
        }
        m_clef = newClef->Clone();
    }
}

void ScoreOrStaffDefAttrInterface::ReplaceKeySig( Object *newKeySig )
{
    if ( newKeySig ) {
        assert( dynamic_cast<KeySigAttr*>(newKeySig) || dynamic_cast<KeySig*>(newKeySig) );
        if (m_keySig) {
            delete m_keySig;
        }
        m_keySig = newKeySig->Clone();
    }
}

void ScoreOrStaffDefAttrInterface::ReplaceMensur( Object *newMensur )
{
    if ( newMensur ) {
        assert( dynamic_cast<MensurAttr*>(newMensur) || dynamic_cast<Mensur*>(newMensur) );
        if (m_mensur) {
            delete m_mensur;
        }
        m_mensur = newMensur->Clone();
    }
}
    
void ScoreOrStaffDefAttrInterface::ReplaceMeterSig( Object *newMeterSig )
{
    if ( newMeterSig ) {
        assert( dynamic_cast<MeterSigAttr*>(newMeterSig) || dynamic_cast<MeterSig*>(newMeterSig) );
        if (m_meterSig) {
            delete m_meterSig;
        }
        m_meterSig = newMeterSig->Clone();
    }
}
    
Clef *ScoreOrStaffDefAttrInterface::GetClefCopy() const
{
    // we should not call it without having checked if a clef is set
    if (!m_clef) return NULL;
    Clef *copy = NULL;
    Clef *current_clef = dynamic_cast<Clef*>(m_clef);
    if (current_clef) {
        copy = new Clef(*current_clef);
    }
    else {
        ClefAttr *current_attr = dynamic_cast<ClefAttr*>(m_clef);
        copy = new Clef(current_attr);
    }
    assert(copy);
    copy->SetScoreOrStaffDefAttr(true);
    return copy;
}
    
KeySig *ScoreOrStaffDefAttrInterface::GetKeySigCopy() const
{
    // we should not call it without having checked if a keysig is set
    if (!m_keySig) return NULL;
    KeySig *copy = NULL;
    KeySig *current_keySig = dynamic_cast<KeySig*>(m_keySig);
    if (current_keySig) {
        copy = new KeySig(*current_keySig);
    }
    else {
        KeySigAttr *current_attr = dynamic_cast<KeySigAttr*>(m_keySig);
        copy = new KeySig(current_attr);
    }
    assert(copy);
    copy->SetScoreOrStaffDefAttr(true);
    return copy;
}
    
Mensur *ScoreOrStaffDefAttrInterface::GetMensurCopy() const
{
    // we should not call it without having checked if a mensur is set
    if (!m_mensur) return NULL;
    Mensur *copy = NULL;
    Mensur *current_mensur = dynamic_cast<Mensur*>(m_mensur);
    if (current_mensur) {
        copy = new Mensur(*current_mensur);
    }
    else {
        MensurAttr *current_attr = dynamic_cast<MensurAttr*>(m_mensur);
        copy = new Mensur(current_attr);
    }
    assert(copy);
    copy->SetScoreOrStaffDefAttr(true);
    return copy;
}
    
MeterSig *ScoreOrStaffDefAttrInterface::GetMeterSigCopy() const
{
    // we should not call it without having checked if a meterSig is set
    if (!m_meterSig) return NULL;
    MeterSig *copy = NULL;
    MeterSig *current_meterSig = dynamic_cast<MeterSig*>(m_meterSig);
    if (current_meterSig) {
        copy = new MeterSig(*current_meterSig);
    }
    else {
        MeterSigAttr *current_attr = dynamic_cast<MeterSigAttr*>(m_meterSig);
        copy = new MeterSig(current_attr);
    }
    assert(copy);
    copy->SetScoreOrStaffDefAttr(true);
    return copy;
}
    
Clef *ScoreOrStaffDefAttrInterface::GetClefElement() const
{
    return dynamic_cast<Clef*>(m_clef);
}
    
KeySig *ScoreOrStaffDefAttrInterface::GetKeySigElement() const
{
    return dynamic_cast<KeySig*>(m_keySig);
}
    
Mensur *ScoreOrStaffDefAttrInterface::GetMensurElement() const
{
    return dynamic_cast<Mensur*>(m_mensur);
}
    
MeterSig *ScoreOrStaffDefAttrInterface::GetMeterSigElement() const
{
    return dynamic_cast<MeterSig*>(m_meterSig);
}

ClefAttr *ScoreOrStaffDefAttrInterface::GetClefAttr() const
{
    return dynamic_cast<ClefAttr*>(m_clef);
}
    
KeySigAttr *ScoreOrStaffDefAttrInterface::GetKeySigAttr() const
{
    return dynamic_cast<KeySigAttr*>(m_keySig);
}
    
MensurAttr *ScoreOrStaffDefAttrInterface::GetMensurAttr() const
{
    return dynamic_cast<MensurAttr*>(m_mensur);
}
    
MeterSigAttr *ScoreOrStaffDefAttrInterface::GetMeterSigAttr() const
{
    return dynamic_cast<MeterSigAttr*>(m_meterSig);
}


//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

ScoreDef::ScoreDef() :
	Object("scoredef-"), ScoreOrStaffDefAttrInterface(), ObjectListInterface()
{
    Reset();
}

ScoreDef::~ScoreDef()
{
}

void ScoreDef::Reset()
{
    Object::Reset();
    ScoreOrStaffDefAttrInterface::Reset();
    m_drawLabels = false;
}

void ScoreDef::AddStaffGrp( StaffGrp *staffGrp )
{
    assert( m_children.empty() );
	staffGrp->SetParent( this );
	m_children.push_back( staffGrp );
    Modify();
}

void ScoreDef::Replace( ScoreDef *newScoreDef )
{
    ReplaceClef( newScoreDef->m_clef );
    ReplaceKeySig( newScoreDef->m_keySig );
    ReplaceMensur( newScoreDef->m_mensur );
    ReplaceMeterSig( newScoreDef->m_meterSig );
    
    ArrayPtrVoid params;
	params.push_back( this );
    Functor replaceStaffDefsInScoreDef( &Object::ReplaceStaffDefsInScoreDef );
    newScoreDef->Process( &replaceStaffDefsInScoreDef, params );
}

void ScoreDef::Replace( StaffDef *newStaffDef )
{
    // first find the staffDef with the same @n
    StaffDef *staffDef = this->GetStaffDef( newStaffDef->GetN() );
    
    // if found, replace attributes
    if (staffDef) {
        staffDef->ReplaceClef( newStaffDef->GetClef() );
        staffDef->ReplaceKeySig( newStaffDef->GetKeySig() );
        staffDef->ReplaceMensur( newStaffDef->GetMensur() );
        staffDef->ReplaceMeterSig( newStaffDef->GetMeterSig() );
        // copy other attributes if present
        if ( newStaffDef->HasLabel() ) staffDef->SetLabel( newStaffDef->GetLabel() );
        if ( newStaffDef->HasLabelAbbr() ) staffDef->SetLabelAbbr( newStaffDef->GetLabelAbbr() );
    }
}

void ScoreDef::FilterList( ListOfObjects *childList )
{
    // We want to keep only staffDef
    ListOfObjects::iterator iter = childList->begin();
    
    while ( iter != childList->end()) {
        StaffDef *currentStaffDef = dynamic_cast<StaffDef*>(*iter);
        if ( !currentStaffDef )
        {
            iter = childList->erase( iter );
        } else {
            iter++;
        }
    }
}


StaffDef *ScoreDef::GetStaffDef( int n )
{
    StaffDef *staffDef = NULL;
    
    this->ResetList( this );
    ListOfObjects* childList = this->GetList(this);
    ListOfObjects::iterator iter;

    for (iter = childList->begin(); iter != childList->end(); ++iter)
    {
        staffDef = dynamic_cast<StaffDef*>(*iter);
        if (staffDef && (staffDef->GetN() == n) ) {
            return staffDef;
        }
    }
    
    return staffDef;
}


void ScoreDef::SetRedrawFlags( bool clef, bool keysig, bool mensur, bool meterSig )
{
    ArrayPtrVoid params;
	params.push_back( &clef );
    params.push_back( &keysig );
	params.push_back( &mensur );
    params.push_back( &meterSig );
    Functor setStaffDefDraw( &Object::SetStaffDefRedrawFlags );
    this->Process( &setStaffDefDraw, params );
}

//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

StaffGrp::StaffGrp() :
    Object(), ObjectListInterface()
{
    Reset();
}

StaffGrp::~StaffGrp()
{
}

void StaffGrp::Reset()
{
    Object::Reset();
    m_symbol = STAFFGRP_NONE;
    m_barthru = false;
}

void StaffGrp::AddStaffDef( StaffDef *staffDef )
{
	staffDef->SetParent( this );
	m_children.push_back( staffDef );
    Modify();
}

void StaffGrp::AddStaffGrp( StaffGrp *staffGrp )
{
	staffGrp->SetParent( this );
	m_children.push_back( staffGrp );
    Modify();
}

void StaffGrp::FilterList( ListOfObjects *childList )
{
    // We want to keep only staffDef
    ListOfObjects::iterator iter = childList->begin();
    
    while ( iter != childList->end()) {
        StaffDef *currentStaffDef = dynamic_cast<StaffDef*>(*iter);
        if ( !currentStaffDef )
        {
            iter = childList->erase( iter );
        } else {
            iter++;
        }
    }
}

//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

StaffDef::StaffDef() :
    Object(), ScoreOrStaffDefAttrInterface(),
    AttCommon(),
    AttLabelsAddl(),
    AttStaffDefVis()
{
    Reset();
}

StaffDef::~StaffDef()
{
}
    
void StaffDef::Reset()
{
    Object::Reset();
    ScoreOrStaffDefAttrInterface::Reset();
    ResetCommon();
    ResetLabelsAddl();
    ResetStaffDefVis();
    m_drawClef = false;
    m_drawKeySig = false;
    m_drawMensur = false;
    m_drawMeterSig = false;
}
    
    
//----------------------------------------------------------------------------
// ScoreDef functor methods
//----------------------------------------------------------------------------

int ScoreDef::CastOffSystems( ArrayPtrVoid params )
{
    // param 0: a pointer to the system we are taking the content from
    // param 1: a pointer the page we are adding system to (unused)
    // param 2: a pointer to the current system
    // param 3: the cummulated shift (m_drawingXRel of the first measure of the current system) (unused)
    // param 4: the system width (unused)
    System *contentSystem = static_cast<System*>(params[0]);
    System **currentSystem = static_cast<System**>(params[2]);
    
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert( dynamic_cast<System*>(this->m_parent));
    
    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    ScoreDef *scoreDef = dynamic_cast<ScoreDef*>( contentSystem->Relinquish( this->GetIdx()) );
    (*currentSystem)->AddScoreDef( scoreDef );
    
    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// StaffDef functor methods
//----------------------------------------------------------------------------

int StaffDef::ReplaceStaffDefsInScoreDef( ArrayPtrVoid params )
{
    // param 0: the scoreDef
    ScoreDef *scoreDef = static_cast<ScoreDef*>(params[0]);
    
    scoreDef->Replace( this );
    
    return FUNCTOR_CONTINUE;
}

int StaffDef::SetStaffDefRedrawFlags( ArrayPtrVoid params )
{
    // param 0: bool clef flag
    // param 1: bool keysig flag
    // param 2: bool mensur flag
    // param 3: bool meterSig flag
    bool *clef = static_cast<bool*>(params[0]);
    bool *keysig = static_cast<bool*>(params[1]);
    bool *mensur = static_cast<bool*>(params[2]);
    bool *meterSig = static_cast<bool*>(params[3]);
    
    if ( (*clef) ) {
        this->SetDrawClef( true );
    }
    if ( (*keysig) ) {
        this->SetDrawKeySig( true );
    }
    if ( (*mensur) ) {
        this->SetDrawMensur( true );
    }
    if ( (*meterSig) ) {
        this->SetDrawMeterSig( true );
    }
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
