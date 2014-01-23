/////////////////////////////////////////////////////////////////////////////
// Name:        scoredef.cpp
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "scoredef.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <typeinfo>

//----------------------------------------------------------------------------

#include "vrv.h"
#include "clef.h"
#include "io.h"
#include "keysig.h"
#include "mensur.h"

namespace vrv {

/**
 * Define the maximum levels of staffGrp within a scoreDef
 */
#define MAX_STAFFGRP_DEPTH 5

//----------------------------------------------------------------------------
// ScoreOrStaffDefAttrInterface
//----------------------------------------------------------------------------

ScoreOrStaffDefAttrInterface::ScoreOrStaffDefAttrInterface()
{
    m_clef = NULL;
    m_keySig = NULL;
    m_mensur = NULL;
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
}

ScoreOrStaffDefAttrInterface::ScoreOrStaffDefAttrInterface( const ScoreOrStaffDefAttrInterface& interface )
{
    m_clef = NULL;
    m_keySig = NULL;
    m_mensur = NULL;
    this->ReplaceClef( interface.m_clef );
    this->ReplaceKeySig( interface.m_keySig );
    this->ReplaceMensur( interface.m_mensur );
}

ScoreOrStaffDefAttrInterface& ScoreOrStaffDefAttrInterface::operator=( const ScoreOrStaffDefAttrInterface& interface )
{
	if ( this != &interface ) // not self assignement
	{
        m_clef = NULL;
        m_keySig = NULL;
        m_mensur = NULL;
        this->ReplaceClef( interface.m_clef );
        this->ReplaceKeySig( interface.m_keySig );
        this->ReplaceMensur( interface.m_mensur );
	}
	return *this;
}

void ScoreOrStaffDefAttrInterface::ReplaceClef( Clef *newClef )
{
    if ( newClef ) {
        if (m_clef) {
            delete m_clef;
        }
        m_clef = new Clef( *newClef );
    }
}

void ScoreOrStaffDefAttrInterface::ReplaceKeySig( KeySignature *newKeySig )
{
    if ( newKeySig ) {
        if (m_keySig) {
            delete m_keySig;
        }
        m_keySig = new KeySignature( *newKeySig );
    }
}

void ScoreOrStaffDefAttrInterface::ReplaceMensur( Mensur *newMensur )
{
    if ( newMensur ) {
        if (m_mensur) {
            delete m_mensur;
        }
        m_mensur = new Mensur( *newMensur );
    }
}

//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

ScoreDef::ScoreDef() :
	Object("scoredef-"), ScoreOrStaffDefAttrInterface(), ObjectListInterface()
{
}

ScoreDef::~ScoreDef()
{
    
}

void ScoreDef::Clear()
{
    ReplaceClef(NULL);
    ReplaceKeySig(NULL);
    ReplaceMensur(NULL);
    ClearChildren();
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
    
    ArrayPtrVoid params;
	params.push_back( this );
    MusFunctor replaceStaffDefsInScoreDef( &Object::ReplaceStaffDefsInScoreDef );
    newScoreDef->Process( &replaceStaffDefsInScoreDef, params );
}

void ScoreDef::Replace( StaffDef *newStaffDef )
{
    // first find the staffDef with the same @n
    StaffDef *staffDef = this->GetStaffDef( newStaffDef->GetStaffNo() );
    
    // if found, replace attributes
    if (staffDef) {
        staffDef->ReplaceClef( newStaffDef->GetClefAttr() );
        staffDef->ReplaceKeySig( newStaffDef->GetKeySigAttr() );
        staffDef->ReplaceMensur( newStaffDef->GetMensurAttr() );
    }
}

void ScoreDef::FilterList()
{
    // We want to keep only staffDef
    ListOfObjects::iterator iter = m_list.begin();
    
    while ( iter != m_list.end()) {
        StaffDef *currentStaffDef = dynamic_cast<StaffDef*>(*iter);
        if ( !currentStaffDef )
        {
            iter = m_list.erase( iter );
        } else {
            iter++;
        }
    }
}


StaffDef *ScoreDef::GetStaffDef( int n )
{
    StaffDef *staffDef = NULL;
    
    this->ResetList( this );
    ListOfObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, i++)
    {
        staffDef = dynamic_cast<StaffDef*>(*iter);
        if (staffDef && (staffDef->GetStaffNo() == n) ) {
            return staffDef;
        }
    }
    
    return staffDef;
}


void ScoreDef::SetRedrawFlags( bool clef, bool keysig, bool mensur )
{
    ArrayPtrVoid params;
	params.push_back( &clef );
    params.push_back( &keysig );
	params.push_back( &mensur );
    MusFunctor setStaffDefDraw( &Object::SetStaffDefRedrawFlags );
    this->Process( &setStaffDefDraw, params );
}

//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

StaffGrp::StaffGrp() :
    Object(), ObjectListInterface()
{
    m_symbol = STAFFGRP_NONE;
    m_barthru = false;
}

StaffGrp::~StaffGrp()
{
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

int StaffGrp::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);  
    if (!output->WriteStaffGrp( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
    
}


void StaffGrp::FilterList()
{
    // We want to keep only staffDef
    ListOfObjects::iterator iter = m_list.begin();
    
    while ( iter != m_list.end()) {
        StaffDef *currentStaffDef = dynamic_cast<StaffDef*>(*iter);
        if ( !currentStaffDef )
        {
            iter = m_list.erase( iter );
        } else {
            iter++;
        }
    }
}

//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

StaffDef::StaffDef() :
    Object(), ScoreOrStaffDefAttrInterface()
{
    m_drawClef = false;
    m_drawKeySig = false;
    m_drawMensur = false;
}

StaffDef::~StaffDef()
{
}

int StaffDef::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);  
    if (!output->WriteStaffDef( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
    
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
    // param 2: bool the mensur flag
    bool *clef = static_cast<bool*>(params[0]);
    bool *keysig = static_cast<bool*>(params[1]);
    bool *mensur = static_cast<bool*>(params[2]);
    
    if ( (*clef) ) {
        this->SetDrawClef( true );
    }
    if ( (*keysig) ) {
        this->SetDrawKeySig( true );
    }
    if ( (*mensur) ) {
        this->SetDrawMensur( true );
    }
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
