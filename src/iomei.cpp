/////////////////////////////////////////////////////////////////////////////
// Name:        iomei.cpp
// Author:      Laurent Pugin
// Created:     2008
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iomei.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <ctime>
#include <sstream>

//----------------------------------------------------------------------------

#include "vrv.h"
#include "app.h"
#include "barline.h"
#include "beam.h"
#include "clef.h"
#include "keysig.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "mensur.h"
#include "multirest.h"
#include "note.h"
#include "page.h"
#include "rest.h"
#include "scoredef.h"
#include "staff.h"
#include "symbol.h"
#include "system.h"
#include "tuplet.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeiOutput
//----------------------------------------------------------------------------

MeiOutput::MeiOutput( Doc *doc, std::string filename ) :
	FileOutputStream( doc )
{
    m_filename = filename;
    m_mei = NULL;
    m_pages = NULL;
    m_page = NULL;
    m_scoreDef = NULL;
    m_system = NULL;
    m_staffGrp = NULL;
    m_staffDef = NULL;
    m_measure = NULL;
    m_staff = NULL;
    m_layer = NULL;
    m_rdgLayer = NULL;
    m_beam = NULL;
}

MeiOutput::~MeiOutput()
{
}

bool MeiOutput::ExportFile( )
{
    try {
        TiXmlDocument *meiDoc = new TiXmlDocument();
        
        m_mei = new TiXmlElement("mei");
        m_mei->SetAttribute( "xmlns", "http://www.music-encoding.org/ns/mei" );
        m_mei->SetAttribute( "meiversion", "2013" );

        // element to place the pages
        m_pages = new TiXmlElement("pages");
        m_pages->SetAttribute( "type",  DocTypeToStr( m_doc->GetType() ).c_str() );
        
        
        // this starts the call of all the functors
        m_doc->Save( this );
        
        // after the functor has run, we have the header in m_mei and the score in m_score
        TiXmlElement *mdiv = new TiXmlElement("mdiv");
        mdiv->LinkEndChild( m_pages );
        TiXmlElement *body = new TiXmlElement("body");
        body->LinkEndChild( mdiv );
        TiXmlElement *music = new TiXmlElement("music");
        music->LinkEndChild( body );
        m_mei->LinkEndChild( music );
        
        TiXmlUnknown *schema = new TiXmlUnknown();
        schema->SetValue("?xml-model href=\"http://www.aruspix.net/mei-page-based-2013-08-29.rng\" type=\"application/xml\" schematypens=\"http://relaxng.org/ns/structure/1.0\"?");
        
        meiDoc->LinkEndChild( new TiXmlDeclaration( "1.0", "UTF-8", "" ) );
        meiDoc->LinkEndChild(schema);
        meiDoc->LinkEndChild(m_mei);
        meiDoc->SaveFile( m_filename.c_str() );
    }
    catch( char * str ) {
        LogError("%s", str );
        return false;
    }
	return true;    
}

std::string MeiOutput::UuidToMeiStr( Object *element )
{
    std::string out = element->GetUuid();
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    LogDebug("uuid: %s\n", out.c_str());
    return out;
}

bool MeiOutput::WriteDoc( Doc *doc )
{
    assert( m_mei );
    
    // ---- header ----
    TiXmlElement *meiHead = new TiXmlElement("meiHead");
    
    TiXmlElement *fileDesc = new TiXmlElement("fileDesc");
    TiXmlElement *titleStmt = new TiXmlElement("titleStmt");
    fileDesc->LinkEndChild(titleStmt);
    TiXmlElement *title = new TiXmlElement("title");
    titleStmt->LinkEndChild(title);
    TiXmlElement *pubStmt = new TiXmlElement("pubStmt");
    fileDesc->LinkEndChild(pubStmt);
    TiXmlElement *date = new TiXmlElement("date");
    pubStmt->LinkEndChild(date);
    
    TiXmlElement *encodingDesc = new TiXmlElement("encodingDesc");
    TiXmlElement *projectDesc = new TiXmlElement("projectDesc");
    encodingDesc->LinkEndChild(projectDesc);
    TiXmlElement *p1 = new TiXmlElement("p");
    projectDesc->LinkEndChild(p1);
    
    p1->LinkEndChild( new TiXmlText( StringFormat( "Encoded with Verovio version %s",  GetVersion().c_str() ).c_str() ) );
    
    // date
    time_t now = time(0);
    date->LinkEndChild( new TiXmlText( ctime( &now ) ) );
    
    meiHead->LinkEndChild(fileDesc);
    meiHead->LinkEndChild(encodingDesc);
    m_mei->LinkEndChild( meiHead );
    
    return true;
}


bool MeiOutput::WritePage( Page *page )
{
    assert( m_pages );
    m_page = new TiXmlElement("page");
    m_page->SetAttribute( "xml:id",  UuidToMeiStr( page ).c_str() );
    // size and margins but only if any - we rely on page.height only to check this
    if ( page->m_pageHeight != -1 ) {
        m_page->SetAttribute( "page.width", StringFormat( "%d", page->m_pageWidth ).c_str() );
        m_page->SetAttribute( "page.height", StringFormat( "%d", page->m_pageHeight ).c_str() );
        m_page->SetAttribute( "page.leftmar", StringFormat( "%d", page->m_pageLeftMar ).c_str() );
        m_page->SetAttribute( "page.rightmar", StringFormat( "%d", page->m_pageRightMar ).c_str() );
        m_page->SetAttribute( "page.rightmar", StringFormat( "%d", page->m_pageRightMar ).c_str() );
    }
    if ( !page->m_surface.empty() ) {
        m_page->SetAttribute( "surface", page->m_surface.c_str() );
    }
    //
    TiXmlComment *comment = new TiXmlComment();
    comment->SetValue( "Coordinates in MEI axis direction" );
    m_pages->LinkEndChild( comment );
    m_pages->LinkEndChild( m_page );
    return true;
}

bool MeiOutput::WriteSystem( System *system )
{
    assert( m_page );
    m_system = new TiXmlElement("system");
    m_system->SetAttribute( "xml:id",  UuidToMeiStr( system ).c_str() );
    // margins
    m_system->SetAttribute( "system.leftmar", StringFormat( "%d", system->m_systemLeftMar ).c_str() );
    m_system->SetAttribute( "system.rightmar", StringFormat( "%d", system->m_systemRightMar ).c_str() );
    // y positions
    m_system->SetAttribute( "uly", StringFormat( "%d", system->m_yAbs ).c_str() );
    m_page->LinkEndChild( m_system );
    return true;
}

bool MeiOutput::WriteScoreDef( ScoreDef *scoreDef )
{
    assert( m_system );
    m_scoreDef = new TiXmlElement("scoreDef");
    m_scoreDef->SetAttribute( "xml:id",  UuidToMeiStr( scoreDef ).c_str() );
    if (scoreDef->GetClefAttr()) {
        m_scoreDef->SetAttribute( "clef.line", ClefLineToStr( scoreDef->GetClefAttr()->m_clefId ).c_str() );
        m_scoreDef->SetAttribute( "clef.shape", ClefShapeToStr( scoreDef->GetClefAttr()->m_clefId ).c_str() );
        // we should add 8va attr
    }
    if (scoreDef->GetKeySigAttr()) {
        m_scoreDef->SetAttribute( "key.sig", KeySigToStr( scoreDef->GetKeySigAttr()->m_num_alter,
                                                         scoreDef->GetKeySigAttr()->m_alteration ).c_str() );
    }
    // this needs to be fixed
    m_page->LinkEndChild( m_system );
    return true;
    
}

bool MeiOutput::WriteStaffGrp( StaffGrp *staffGrp )
{
    // for now only as part of a system - this needs to be fixed
    assert( m_system );
    m_staffGrp = new TiXmlElement("staffGrp");
    m_staffGrp->SetAttribute( "xml:id",  UuidToMeiStr( staffGrp ).c_str() );
    if ( staffGrp->GetSymbol() != STAFFGRP_NONE ) {
        m_staffGrp->SetAttribute( "symbol", StaffGrpSymbolToStr( staffGrp->GetSymbol() ).c_str() );
    }
    if ( staffGrp->GetBarthru() ) {
        m_staffGrp->SetAttribute( "barthru", BoolToStr( staffGrp->GetBarthru() ).c_str() );
    }
    
    m_system->LinkEndChild( m_staffGrp );
    return true;
}

bool MeiOutput::WriteStaffDef( StaffDef *staffDef )
{
    assert( m_staffGrp );
    m_staffDef = new TiXmlElement("staffDef");
    m_staffDef->SetAttribute( "xml:id",  UuidToMeiStr( staffDef ).c_str() );
    m_staffDef->SetAttribute( "n", StringFormat( "%d", staffDef->GetStaffNo() ).c_str() );
    if (staffDef->GetClefAttr()) {
        m_staffDef->SetAttribute( "clef.line", ClefLineToStr( staffDef->GetClefAttr()->m_clefId ).c_str() );
        m_staffDef->SetAttribute( "clef.shape", ClefShapeToStr( staffDef->GetClefAttr()->m_clefId ).c_str() );
        // we should add 8va attr
    }
    if (staffDef->GetKeySigAttr()) {
        m_staffDef->SetAttribute( "key.sig", KeySigToStr( staffDef->GetKeySigAttr()->m_num_alter,
                                                         staffDef->GetKeySigAttr()->m_alteration ).c_str() );
    }

    m_staffGrp->LinkEndChild( m_staffDef );
    return true;
}

bool MeiOutput::WriteStaff( Staff *staff )
{
    assert( m_system );
    m_staff = new TiXmlElement("staff");
    m_staff->SetAttribute( "xml:id",  UuidToMeiStr( staff ).c_str() );
    // y position
    if ( staff->notAnc ) {
        m_staff->SetAttribute( "label", "mensural" );
    }
    m_staff->SetAttribute( "uly", StringFormat( "%d", staff->m_yAbs ).c_str() );
    m_staff->SetAttribute( "n", StringFormat( "%d", staff->GetStaffNo() ).c_str() );

    m_system->LinkEndChild( m_staff );
    return true;
}

bool MeiOutput::WriteMeasure( Measure *measure )
{
    assert( m_staff );
    m_measure = new TiXmlElement("measure");
    m_measure->SetAttribute( "xml:id",  UuidToMeiStr( measure ).c_str() );
    m_measure->SetAttribute( "n", StringFormat( "%d", measure->m_logMeasureNb ).c_str() );
    m_staff->LinkEndChild( m_measure );
    return true;
}

bool MeiOutput::WriteLayer( Layer *layer )
{
    assert( m_staff );
    m_layer = new TiXmlElement("layer");
    m_layer->SetAttribute( "xml:id",  UuidToMeiStr( layer ).c_str() );
    m_layer->SetAttribute( "n", StringFormat( "%d", layer->GetLayerNo() ).c_str() );
    if ( m_measure ) {
        m_measure->LinkEndChild( m_layer );
    }
    else {
        m_staff->LinkEndChild( m_layer );
    }
    return true;
}

bool MeiOutput::WriteLayerElement( LayerElement *element )
{
    assert( m_layer );
    
    // Here we look at what is the parent.
    // For example, if we are in a beam, we vrvT attach it to the beam xml element (m_beam)
    // By default, we attach it to m_layer
    TiXmlElement *currentParent = m_layer;
    if ( dynamic_cast<LayerRdg*>(element->m_parent) ) {
        assert( m_rdgLayer );
        currentParent = m_rdgLayer;
    }
    else if ( dynamic_cast<Beam*>(element->m_parent) ) {
        assert( m_beam );
        currentParent = m_beam;
    }
    else if ( dynamic_cast<Tuplet*>(element->m_parent) ) {
        assert( m_tuplet );
        currentParent = m_tuplet;
    }
    // we should do the same for any LayerElement container (slur, tuplet, etc. )
    
    TiXmlElement *xmlElement = NULL;
    if (dynamic_cast<Barline*>(element)) {
        xmlElement = new TiXmlElement( "barline" );
        WriteMeiBarline( xmlElement, dynamic_cast<Barline*>(element) );
    }
    else if (dynamic_cast<Beam*>(element)) {
        xmlElement = new TiXmlElement("beam");
        m_beam = xmlElement;
        WriteMeiBeam( xmlElement, dynamic_cast<Beam*>(element) );
    }
    else if (dynamic_cast<Clef*>(element)) {
        xmlElement = new TiXmlElement("clef");
        WriteMeiClef( xmlElement, dynamic_cast<Clef*>(element) );
    }
    else if (dynamic_cast<Mensur*>(element)) {
        xmlElement = new TiXmlElement("mensur");
        WriteMeiMensur( xmlElement, dynamic_cast<Mensur*>(element) );
    }
    else if (dynamic_cast<MultiRest*>(element)) {
        xmlElement = new TiXmlElement("multiRest");
        WriteMeiMultiRest( xmlElement, dynamic_cast<MultiRest*>(element) );
    }
    else if (dynamic_cast<Note*>(element)) {
        xmlElement = new TiXmlElement("note");
        WriteMeiNote( xmlElement, dynamic_cast<Note*>(element) );
    }
    else if (dynamic_cast<Rest*>(element)) {
        xmlElement = new TiXmlElement("rest");
        WriteMeiRest( xmlElement, dynamic_cast<Rest*>(element) );
    }
    else if (dynamic_cast<Tuplet*>(element)) {
        xmlElement = new TiXmlElement("tuplet");
        m_tuplet = xmlElement;
        WriteMeiTuplet( xmlElement, dynamic_cast<Tuplet*>(element) );
    }
    else if (dynamic_cast<Symbol*>(element)) {        
        xmlElement = WriteMeiSymbol( dynamic_cast<Symbol*>(element) );
    }
    
    // we have it, set the uuid we read
    if ( xmlElement ) {
        this->WriteSameAsAttr( xmlElement, element );
        xmlElement->SetAttribute( "xml:id",  UuidToMeiStr( element ).c_str() );
        if ( element->m_xAbs != VRV_UNSET) {
            xmlElement->SetAttribute( "ulx", StringFormat( "%d", element->m_xAbs ).c_str() );
        }
        currentParent->LinkEndChild( xmlElement );
        return true;
    }
    else {
        LogWarning( "Element class %s could not be saved", element->GetClassName().c_str() );
        return false;
    }    
}

void MeiOutput::WriteMeiBarline( TiXmlElement *meiBarline, Barline *barline )
{
    return;
}


void MeiOutput::WriteMeiBeam( TiXmlElement *meiBeam, Beam *beam )
{
    return;
}


void MeiOutput::WriteMeiClef( TiXmlElement *meiClef, Clef *clef )
{
    meiClef->SetAttribute( "line", ClefLineToStr( clef->m_clefId ).c_str() );
    meiClef->SetAttribute( "shape", ClefShapeToStr( clef->m_clefId ).c_str() );
    // we should add 8va attr
    return;
}


void MeiOutput::WriteMeiMensur( TiXmlElement *meiMensur, Mensur *mensur )
{
    if ( mensur->m_sign ) {
        meiMensur->SetAttribute( "sign", MensurSignToStr( mensur->m_sign ).c_str() );
    }
    if ( mensur->m_dot ) {
        meiMensur->SetAttribute( "dot", "true" );
    }
    if ( mensur->m_slash ) {
        meiMensur->SetAttribute( "slash", "1" ); // only one slash for now
    }
    if ( mensur->m_reversed ) {
        meiMensur->SetAttribute( "orient", "reversed" ); // only orientation
    }
    if ( mensur->m_num ) {
        meiMensur->SetAttribute( "num", StringFormat("%d", mensur->m_num ).c_str() );
    }
    if ( mensur->m_numBase ) {
        meiMensur->SetAttribute( "numbase", StringFormat("%d", mensur->m_numBase ).c_str() );
    }
    // missing m_meterSymb
    
    return;
}

void MeiOutput::WriteMeiMultiRest( TiXmlElement *meiMultiRest, MultiRest *multiRest )
{
    meiMultiRest->SetAttribute( "num", StringFormat("%d", multiRest->GetNumber()).c_str() );

    return;
}

void MeiOutput::WriteMeiNote( TiXmlElement *meiNote, Note *note )
{
    meiNote->SetAttribute( "pname", PitchToStr( note->m_pname ).c_str() );
    meiNote->SetAttribute( "oct", OctToStr( note->m_oct ).c_str() );
    meiNote->SetAttribute( "dur", DurToStr( note->m_dur ).c_str() );
    if ( note->m_dots ) {
        meiNote->SetAttribute( "dots", StringFormat("%d", note->m_dots).c_str() );
    }
    if ( note->m_accid ) {
        meiNote->SetAttribute( "accid", AccidToStr( note->m_accid ).c_str() );
    }
    if ( note->m_lig ) {
        if ( note->m_ligObliqua ) {
            meiNote->SetAttribute( "lig", "obliqua" );
        }
        else {
            meiNote->SetAttribute( "lig", "recta" );
        }
    }
    if ( note->m_stemDir ) {
        // this is not really correct because Note::m_stemDir indicates that it is opposite the normal position
        meiNote->SetAttribute( "stem.dir", "up" );
    }
    if ( note->m_colored ) {
        meiNote->SetAttribute( "colored", "true" );
    }
    // missing m_artic, m_chord, m_headShape, m_slur, m_stemLen
    return;
}

void MeiOutput::WriteMeiRest( TiXmlElement *meiRest, Rest *rest )
{    
    meiRest->SetAttribute( "dur", DurToStr( rest->m_dur ).c_str() );
    if ( rest->m_dots ) {
        meiRest->SetAttribute( "dots", StringFormat("%d", rest->m_dots).c_str() );
    }
    // missing position
    meiRest->SetAttribute( "ploc", PitchToStr( rest->m_pname ).c_str() );
    meiRest->SetAttribute( "oloc", OctToStr( rest->m_oct ).c_str() );
    return;
}

TiXmlElement *MeiOutput::WriteMeiSymbol( Symbol *symbol )
{
    TiXmlElement *xmlElement = NULL;
    if (symbol->m_type==SYMBOL_ACCID) {
        TiXmlElement *accid = new TiXmlElement("accid");
        accid->SetAttribute( "accid", AccidToStr( symbol->m_accid ).c_str() );
        // position
        accid->SetAttribute( "ploc", PitchToStr( symbol->m_pname ).c_str() );
        accid->SetAttribute( "oloc", OctToStr( symbol->m_oct ).c_str() );
        xmlElement = accid;
    }
    else if (symbol->m_type==SYMBOL_CUSTOS) {
        TiXmlElement *custos = new TiXmlElement("custos");
        custos->SetAttribute( "pname", PitchToStr( symbol->m_pname ).c_str() );
        custos->SetAttribute( "oct", OctToStr( symbol->m_oct ).c_str() );
        xmlElement = custos;
    }
    else if (symbol->m_type==SYMBOL_DOT) {
        TiXmlElement *dot = new TiXmlElement("dot");
        // missing m_dots
        // position
        dot->SetAttribute( "ploc", PitchToStr( symbol->m_pname ).c_str() );
        dot->SetAttribute( "oloc", OctToStr( symbol->m_oct ).c_str() );
        xmlElement = dot;
    }
    return xmlElement;
}


void MeiOutput::WriteMeiTuplet( TiXmlElement *meiTuplet, Tuplet *tuplet )
{
    return;
}

bool MeiOutput::WriteLayerApp( LayerApp *app )
{    
    assert( m_layer );
    m_app = new TiXmlElement("app");
    m_layer->LinkEndChild( m_app ); 
    return true;
}

bool MeiOutput::WriteLayerRdg( LayerRdg *rdg )
{   
    assert( m_app );
    m_rdgLayer = new TiXmlElement("rdg");
    m_rdgLayer->SetAttribute( "source", rdg->m_source.c_str() );
    m_app->LinkEndChild( m_rdgLayer ); 
    return true;
}


void MeiOutput::WriteSameAsAttr( TiXmlElement *meiElement, Object *element )
{
    if ( !element->m_sameAs.empty() ) {
        meiElement->SetAttribute( "sameas", element->m_sameAs.c_str() );
    }
}

std::string MeiOutput::BoolToStr(bool value)
{
    if (value) return "true";
    return "false";
}

std::string MeiOutput::OctToStr(int oct)
{
	char buf[3];
	snprintf(buf, 2, "%d", oct);
	return std::string(buf);
	
	// For some reason, #include <sstream> does not work with xcode 3.2
	//std::ostringstream oss;
	//oss << oct;
	//return oss.str();
}


std::string MeiOutput::PitchToStr(int pitch)
{
    std::string value;
    switch (pitch) {
        case 7:
        case 0: value = "b"; break;
        case 1: value = "c"; break;
        case 2: value = "d"; break;
        case 3: value = "e"; break;
        case 4: value = "f"; break;
        case 5: value = "g"; break;
        case 6: value = "a"; break;
        default: 
            LogWarning("Unknown pitch '%d'", pitch);
            value = "";
            break;
    }
	return value;
}

std::string MeiOutput::AccidToStr(unsigned char accid)
{
    std::string value;
    switch (accid) {
        case ACCID_SHARP: value = "s"; break;
        case ACCID_FLAT: value = "f"; break;
        case ACCID_NATURAL: value = "n"; break;
        case ACCID_DOUBLE_SHARP: value = "x"; break;
        case ACCID_DOUBLE_FLAT: value = "ff"; break;
        case ACCID_QUARTER_SHARP: value = "ns"; break;
        case ACCID_QUARTER_FLAT: value = "nf"; break;
        default: 
            LogWarning("Unknown accid '%d'", accid);
            value = "";
            break;
    }
	return value;
}

std::string MeiOutput::ClefLineToStr( ClefId clefId )
{	
	std::string value; 
	switch(clefId)
	{	
        case SOL2 : value = "2"; break;
		case SOL1 : value = "1"; break; 
		case SOLva : value = "2"; break;
		case FA5 : value = "5"; break;
		case FA4 : value = "4"; break;
		case FA3 : value = "3"; break;
		case UT1 : value = "1"; break;
        case UT2 : value = "2"; break;
		case UT3 : value = "3"; break;
		case UT4 : value = "4"; break;
		case UT5 : value = "5"; break;
        default: 
            LogWarning("Unknown clef '%d'", clefId);
            value = "";
            break;
	}
	return value;
}

std::string MeiOutput::ClefShapeToStr( ClefId clefId )
{	
	std::string value; 
	switch(clefId)
	{	
        case SOL2 : 
		case SOL1 : 
		case SOLva : value = "G"; break;
		case FA5 : 
		case FA4 :
		case FA3 : value = "F"; break;
        case UT1 :
		case UT2 : 
		case UT3 : 
		case UT4 : 
		case UT5 : value = "C"; break;		
        default: 
            LogWarning("Unknown clef '%d'", clefId);
            value = "";
            break;
	}
	return value;
}

std::string MeiOutput::MensurSignToStr(MensurSign sign)
{
 	std::string value; 
	switch(sign)
	{	case MENSUR_SIGN_C : value = "C"; break;
		case MENSUR_SIGN_O : value = "O"; break;		
        default: 
            LogWarning("Unknown mensur sign '%d'", sign);
            value = "";
            break;
	}
	return value;   
}


std::string MeiOutput::DurToStr( int dur )
{
    std::string value;
    if (dur == DUR_LG) value = "longa";
    else if (dur == DUR_BR) value = "brevis";
    else if (dur == DUR_1) value = "semibrevis";
    else if (dur == DUR_2) value = "minima";
    else if (dur == DUR_4) value = "semiminima";
    else if (dur == DUR_8) value = "fusa";
    else if (dur == DUR_16) value = "semifusa";
    else if (dur == DUR_LG) value = "long";
    else if (dur == DUR_BR) value = "breve";
    else if (dur == DUR_1) value = "1";
    else if (dur == DUR_2) value = "2";
    else if (dur == DUR_4) value = "4";
    else if (dur == DUR_8) value = "8";
    else if (dur == DUR_16) value = "16";
    else if (dur == DUR_32) value = "32";
    else if (dur == DUR_64) value = "64";
    else if (dur == DUR_128) value = "128";
	else {
		LogWarning("Unknown duration '%d'", dur);
        value = "4";
	}
    return value;
}

std::string MeiOutput::DocTypeToStr(DocType type)
{
 	std::string value; 
	switch(type)
	{	
        case Raw : value = "raw"; break;
        case Rendering : value = "rendering"; break;
		case Transcription : value = "transcription"; break;		
        default: 
            LogWarning("Unknown layout type '%d'", type);
            value = "";
            break;
	}
	return value;   
}


std::string MeiOutput::KeySigToStr(int num, char alter_type )
{
 	std::string value;
    if (num == 0) {
        return "0";
    }
	switch(alter_type)
	{	case ACCID_FLAT : value = StringFormat("%df", num); break;
		case ACCID_SHARP : value = StringFormat("%ds", num); break;
        default:
            LogWarning("Unknown key signature values '%d' and '%d", num, alter_type);
            value = "0";
            break;
	}
	return value;
}


std::string MeiOutput::StaffGrpSymbolToStr(StaffGrpSymbol symbol)
{
 	std::string value;
	switch(symbol)
	{	case STAFFGRP_LINE : value = "line"; break;
		case STAFFGRP_BRACE : value = "brace"; break;
        case STAFFGRP_BRACKET : value = "bracket"; break;
        default:
            LogWarning("Unknown staffGrp @symbol  '%d'", symbol);
            value = "line";
            break;
	}
	return value;
}


//----------------------------------------------------------------------------
// MeiInput
//----------------------------------------------------------------------------

MeiInput::MeiInput( Doc *doc, std::string filename ) :
	FileInputStream( doc )
{
    m_filename = filename;
    m_doc->m_fname = GetFilename( filename );
    m_page = NULL;
    m_scoreDef = NULL;
    m_staffDef = NULL;
    m_system = NULL;
	m_staff = NULL;
    m_measure = NULL;
	m_layer = NULL;
    m_layerApp = NULL;
    m_layerRdg = NULL;
    m_beam = NULL;
    m_tuplet = NULL;
    //
    m_currentLayer = NULL;
    //
    m_hasScoreDef = false;
}

MeiInput::~MeiInput()
{
}

bool MeiInput::ImportFile( )
{
    try {
        m_doc->Reset( Raw );
        TiXmlDocument doc( m_filename.c_str() );
        bool loadOkay = doc.LoadFile();
        if (!loadOkay)
        {
            return false;
        }
        TiXmlElement *root = doc.RootElement();
        return ReadMei( root );
        }
    catch( char * str ) {
        LogError("%s", str );
        return false;
    }
}

bool MeiInput::ImportString( const std::string mei )
{
    try {
        m_doc->Reset( Raw );
        TiXmlDocument doc;
        doc.Parse( mei.c_str() );
        TiXmlElement *root = doc.RootElement();
        return ReadMei( root );
    }
    catch( char * str ) {
        LogError("%s", str );
        return false;
    }
}



bool MeiInput::ReadMei( TiXmlElement *root )
{
    TiXmlElement *current;
    
    if ( root && (current = root->FirstChildElement( "meiHead" ) ) )
    {
        ReadMeiHeader( current );
    }
    // music
    TiXmlElement *music = NULL;
    TiXmlElement *body = NULL;
    TiXmlElement *mdiv = NULL;
    TiXmlElement *pages = NULL;
    if ( root ) {
        music = root->FirstChildElement("music");
    }
    if ( music) {
        body = music->FirstChildElement("body");
    }
    if ( body ) {
        mdiv = body->FirstChildElement("mdiv");
    }
    if ( mdiv ) {
        pages = mdiv->FirstChildElement("pages");
    }
    if ( pages ) {
        
        // check if there is a type attribute for the score
        DocType type;
        if ( pages->Attribute( "type" ) ) {
            type = StrToDocType( pages->Attribute( "type" ) );
            m_doc->Reset( type );
        }
        
        // this is a page-based MEI file, we just loop trough the pages
        if ( pages->FirstChildElement( "page" ) ) {
            // because we are in a page-based MEI
            this->m_hasLayoutInformation = true;
            for( current = pages->FirstChildElement( "page" ); current; current = current->NextSiblingElement( "page" ) ) {
                m_page = new Page( );
                SetMeiUuid( current, m_page );
                if (ReadMeiPage( current )) {
                    m_doc->AddPage( m_page );
                }
                else {
                    delete m_page;
                }
                m_page = NULL;
            }
        }
    }
    else {
        m_page = new Page( );
        m_system = new System( );
        m_page->AddSystem( m_system );
        m_doc->AddPage( m_page );
        TiXmlElement *current = NULL;
        for( current = mdiv->FirstChildElement( ); current; current = current->NextSiblingElement( ) ) {
            ReadUnsupported( current );
        }
    }
    
    if ( !m_openTies.empty()) {
        std::vector<Note*>::iterator iter;
        for (iter = m_openTies.begin(); iter != m_openTies.end(); ++iter)
        {
            LogWarning("Terminal tie for note '%s' could not be matched", (*iter)->GetUuid().c_str() );
        }
    }
    
    return true;
}

bool MeiInput::ReadMeiHeader( TiXmlElement *meiHead )
{
    return true;
}


bool MeiInput::ReadMeiPage( TiXmlElement *page )
{
    assert( m_page );
    
    if ( page->Attribute( "page.height" ) ) {
        m_page->m_pageHeight = atoi ( page->Attribute( "page.height" ) );
    }
    if ( page->Attribute( "page.width" ) ) {
        m_page->m_pageWidth = atoi ( page->Attribute( "page.width" ) );
    }
    if ( page->Attribute( "page.leftmar" ) ) {
        m_page->m_pageLeftMar = atoi ( page->Attribute( "page.leftmar" ) );
    }
    if ( page->Attribute( "page.rightmar" ) ) {
        m_page->m_pageRightMar = atoi ( page->Attribute( "page.rightmar" ) );
    }
    if ( page->Attribute( "page.topmar" ) ) {
        m_page->m_pageTopMar = atoi ( page->Attribute( "page.topmar" ) );
    }
    if ( page->Attribute( "surface" ) ) {
        m_page->m_surface = page->Attribute( "surface" );
    }
    
    TiXmlElement *current = NULL;
    for( current = page->FirstChildElement( "system" ); current; current = current->NextSiblingElement( "system" ) ) {
        m_system = new System( );
        SetMeiUuid( current, m_system );
        if (ReadMeiSystem( current )) {
            m_page->AddSystem( m_system );
        }
        else {
            delete m_system;
        }
        m_system = NULL;
    }
    // success only if at least one system was added to the page
    return (m_page->GetSystemCount() > 0);
}

bool MeiInput::ReadMeiSystem( TiXmlElement *system )
{
    assert( m_system );
    assert( !m_measure );
    assert( !m_staff );
    
    if ( system->Attribute( "system.leftmar") ) {
        m_system->m_systemLeftMar = atoi ( system->Attribute( "system.leftmar" ) );
    }
    if ( system->Attribute( "system.rightmar" ) ) {
        m_system->m_systemRightMar = atoi ( system->Attribute( "system.rightmar" ) );
    }
    if ( system->Attribute( "uly" ) ) {
        m_system->m_yAbs = atoi ( system->Attribute( "uly" ) );
    }
    
    TiXmlElement *current = NULL;
    // unmeasured music
    if ( system->FirstChildElement( "staff" ) ) {
        // this is the trick for un-measured music: we add one measure ( false )
        if ( !m_measure ) {
            m_measure = new Measure( false );
        }
        for( current = system->FirstChildElement( "staff" ); current; current = current->NextSiblingElement( "staff" ) ) {
            m_staff = new Staff( );
            SetMeiUuid( current , m_staff );
            if ( ReadMeiStaff( current )) {
                m_measure->AddStaff( m_staff );
            }
            else {
                delete m_staff;
            }
            m_staff = NULL;
        }
        if ( m_measure->GetStaffCount() > 0) {
            m_system->AddMeasure( m_measure );
        }
        else {
            delete m_measure;
        }
        m_measure = NULL;
    }
    else {
        // measured
        for( current = system->FirstChildElement( "measure" ); current; current = current->NextSiblingElement( "measure" ) ) {
            m_measure = new Measure( );
            SetMeiUuid( current, m_measure );
            if (ReadMeiMeasure( current )) {
                m_system->AddMeasure( m_measure );
            }
            else {
                delete m_measure;
            }
            m_measure = NULL;
        }
    }
    
    // success only if at least one measure was added to the system
    return (m_system->GetMeasureCount() > 0);
}

bool MeiInput::ReadMeiScoreDef( TiXmlElement *scoreDef )
{
    assert( m_scoreDef );
    assert( m_staffGrps.empty() );
    
    if ( scoreDef->Attribute( "key.sig" ) ) {
        KeySignature keysig(
                StrToKeySigNum( scoreDef->Attribute( "key.sig" ) ),
                StrToKeySigType( scoreDef->Attribute( "key.sig" ) ) );
        m_scoreDef->ReplaceKeySig( &keysig );
    }
    if ( scoreDef->Attribute( "clef.line" ) && scoreDef->Attribute( "clef.shape" ) ) {
        Clef clef;
        clef.m_clefId = StrToClef( scoreDef->Attribute( "clef.shape" ) , scoreDef->Attribute( "clef.line" ) );
        m_scoreDef->ReplaceClef( &clef );
        // add other attributes for SOLva
    }
    
    TiXmlElement *current = NULL;
    for( current = scoreDef->FirstChildElement( "staffGrp" ); current; current = current->NextSiblingElement( "staffGrp" ) ) {
        StaffGrp *staffGrp = new StaffGrp( );
        m_staffGrps.push_back( staffGrp );
        SetMeiUuid( current , staffGrp );
        if (ReadMeiStaffGrp( current )) {
            m_scoreDef->AddStaffGrp( staffGrp );
        }
        else {
            delete staffGrp;
        }
        m_staffGrps.pop_back();
    }
    
    return true;
}

bool MeiInput::ReadMeiStaffGrp( TiXmlElement *staffGrp )
{
    assert( !m_staffGrps.empty() );
    assert( !m_staffDef );
    
    StaffGrp *currentStaffGrp = m_staffGrps.back();
    
    if ( staffGrp->Attribute( "symbol" ) ) {
        currentStaffGrp->SetSymbol( StrToStaffGrpSymbol( staffGrp->Attribute( "symbol" ) ) );
    }
    if ( staffGrp->Attribute( "barthru" ) ) {
        currentStaffGrp->SetBarthru( StrToBool( staffGrp->Attribute( "barthru" ) ) );
    }
    
    TiXmlElement *current = NULL;
    for( current = staffGrp->FirstChildElement( ); current; current = current->NextSiblingElement( ) ) {
        if ( std::string( current->Value() ) == "staffGrp" ) {
            StaffGrp *staffGrp = new StaffGrp( );
            m_staffGrps.push_back( staffGrp );
            SetMeiUuid( current , staffGrp );
            if (ReadMeiStaffGrp( current )) {
                currentStaffGrp->AddStaffGrp( staffGrp );
            }
            else {
                delete staffGrp;
            }
            m_staffGrps.pop_back();            
        }
        else if ( std::string( current->Value() ) == "staffDef" ) {
            m_staffDef = new StaffDef( );
            SetMeiUuid( current , m_staffDef );
            if (ReadMeiStaffDef( current )) {
                currentStaffGrp->AddStaffDef( m_staffDef );
            }
            else {
                delete m_staffDef;
            }
            m_staffDef = NULL;
        }        
    }
    
    return true;
}

bool MeiInput::ReadMeiStaffDef( TiXmlElement *staffDef )
{
    assert( m_staffDef );
    
    if ( staffDef->Attribute( "n" ) ) {
        m_staffDef->SetStaffNo( atoi ( staffDef->Attribute( "n" ) ) );
    }
    else {
        LogWarning("No @n on staffDef");
    }
    if ( staffDef->Attribute( "key.sig" ) ) {
        KeySignature keysig(
                         StrToKeySigNum( staffDef->Attribute( "key.sig" ) ),
                         StrToKeySigType( staffDef->Attribute( "key.sig" ) ) );
        m_staffDef->ReplaceKeySig( &keysig );
    }
    if ( staffDef->Attribute( "clef.line" ) && staffDef->Attribute( "clef.shape" ) ) {
        Clef clef;
        clef.m_clefId = StrToClef( staffDef->Attribute( "clef.shape" ) , staffDef->Attribute( "clef.line" ) );
        // this is obviously a short cut - assuming @clef.dis being SOLva
        if ( staffDef->Attribute( "clef.dis" ) ) {
            clef.m_clefId = SOLva;
        }
        m_staffDef->ReplaceClef( &clef );
    }
    
    return true;
}

bool MeiInput::ReadMeiMeasure( TiXmlElement *measure )
{
    assert( m_measure );
    assert( !m_staff );
    
    if ( measure->Attribute( "right" ) ) {
        m_measure->GetRightBarline()->m_barlineType = StrToBarlineType( measure->Attribute( "right" ) );
    }
    
    TiXmlElement *current = NULL;
    for( current = measure->FirstChildElement( "staff" ); current; current = current->NextSiblingElement( "staff" ) ) {
        m_staff = new Staff( );
        SetMeiUuid( current , m_staff );
        if ( ReadMeiStaff( current )) {
            m_measure->AddStaff( m_staff );
        }
        else {
            delete m_staff;
        }
        m_staff = NULL;
    }
    // success only if at least one staff was added to the measure
    return (m_measure->GetStaffCount() > 0);
}

bool MeiInput::ReadMeiStaff( TiXmlElement *staff )
{
    assert( m_staff );
    assert( !m_layer );
    
    if ( staff->Attribute( "n" ) ) {
        m_staff->SetStaffNo( atoi ( staff->Attribute( "n" ) ) );
    }
    else {
        LogWarning("No @n on staff");
    }
    if ( staff->Attribute( "uly" ) ) {
        m_staff->m_yAbs = atoi ( staff->Attribute( "uly" ) );
    }
    if ( staff->Attribute( "label" ) ) {
        // we use type only for typing mensural notation
        m_staff->notAnc = true;
    }
    
    TiXmlElement *current = NULL;
    for( current = staff->FirstChildElement( "layer" ); current; current = current->NextSiblingElement( "layer" ) ) {
        m_layer = new Layer( 1 );
        m_currentLayer = m_layer;
        SetMeiUuid( current , m_layer );
        if (ReadMeiLayer( current )) {
            m_staff->AddLayer( m_layer );
        }
        else {
            delete m_layer;
        }
        m_layer = NULL;
    }
    
    // success only if at least one measure was added to the staff
    return (m_staff->GetLayerCount() > 0);
}

bool MeiInput::ReadMeiLayer( TiXmlElement *layer )
{
    assert( m_layer );
    
    if ( layer->Attribute( "n" ) ) {
        m_layer->SetLayerNo( atoi ( layer->Attribute( "n" ) ) );
    }
    else {
        LogWarning("No @n on layer");
    }
    
    TiXmlElement *current = NULL;
    for( current = layer->FirstChildElement( ); current; current = current->NextSiblingElement( ) ) {
        ReadMeiLayerElement( current );
    }
    // success in any case
    return true;
}

bool MeiInput::ReadMeiLayerElement( TiXmlElement *xmlElement )
{
    LayerElement *vrvElement = NULL;
    if ( std::string( xmlElement->Value() )  == "barLine" ) {
        vrvElement = ReadMeiBarline( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "beam" ) {
        vrvElement = ReadMeiBeam( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "clef" ) {
        vrvElement = ReadMeiClef( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "mensur" ) {
        vrvElement = ReadMeiMensur( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "note" ) {
        vrvElement = ReadMeiNote( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "rest" ) {
        vrvElement = ReadMeiRest( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "multiRest" ) {
        vrvElement = ReadMeiMultiRest( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "tuplet" ) {
        vrvElement = ReadMeiTuplet( xmlElement );
    }
    // symbols
    else if ( std::string( xmlElement->Value() ) == "accid" ) {
        vrvElement = ReadMeiAccid( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "custos" ) {
        vrvElement = ReadMeiCustos( xmlElement );
    }
    else if ( std::string( xmlElement->Value() ) == "dot" ) {
        vrvElement = ReadMeiDot( xmlElement );
    }
    // app
    else if ( std::string( xmlElement->Value() ) == "app" ) {
        vrvElement = ReadMeiApp( xmlElement );
    }
    // unkown            
    else {
        LogDebug("Element %s ignored", xmlElement->Value() );
    }
    
    if ( !vrvElement ) {
        return false;
    }
    
    if ( xmlElement->Attribute( "ulx" ) ) {
        vrvElement->m_xAbs = atoi ( xmlElement->Attribute( "ulx" ) );
    }
    ReadSameAsAttr( xmlElement, vrvElement );
    SetMeiUuid( xmlElement, vrvElement );
    
    AddLayerElement( vrvElement );
    return true;
}

LayerElement *MeiInput::ReadMeiBarline( TiXmlElement *barline )
{
    Barline *vrvBarline = new Barline();
    
    return vrvBarline;    
}

LayerElement *MeiInput::ReadMeiBeam( TiXmlElement *beam )
{
    assert ( !m_beam );
    
    // m_beam will be used for adding elements to the beam
    m_beam = new Beam();
    
    Object *previousLayer = m_currentLayer;
    m_currentLayer = m_beam;
    
    TiXmlElement *current = NULL;
    for( current = beam->FirstChildElement( ); current; current = current->NextSiblingElement( ) ) {
        ReadMeiLayerElement( current );
    }
    
    if ( m_beam->GetNoteCount() == 1 ) {
        LogWarning("Beam element with only one note");
    }
    // switch back to the previous one
    m_currentLayer = previousLayer;
    if ( m_beam->GetNoteCount() < 1 ) {
        delete m_beam;
        m_beam = NULL;
        return NULL;
    } 
    else {
        // set the member to NULL but keep a pointer to be returned        
        Beam *vrvBeam = m_beam;
        m_beam = NULL;
        return vrvBeam;
    }
}

LayerElement *MeiInput::ReadMeiClef( TiXmlElement *clef )
{ 
    Clef *vrvClef = new Clef(); 
    if ( clef->Attribute( "shape" ) && clef->Attribute( "line" ) ) {
        vrvClef->m_clefId = StrToClef( clef->Attribute( "shape" ) , clef->Attribute( "line" ) );
    }
    
    return vrvClef;
}


LayerElement *MeiInput::ReadMeiMensur( TiXmlElement *mensur )
{
    Mensur *vrvMensur = new Mensur();
    
    if ( mensur->Attribute( "sign" ) ) {
        vrvMensur->m_sign = StrToMensurSign( mensur->Attribute( "sign" ) );
    }
    if ( mensur->Attribute( "dot" ) ) {
        vrvMensur->m_dot = ( strcmp( mensur->Attribute( "dot" ), "true" ) == 0 );
    }
    if ( mensur->Attribute( "slash" ) ) {
        vrvMensur->m_slash =  1; //atoi( mensur->Attribute( "Slash" ) );
    }
    if ( mensur->Attribute( "orient" ) ) {
        vrvMensur->m_reversed = ( strcmp ( mensur->Attribute( "orient" ), "reversed" ) == 0 );
    }
    if ( mensur->Attribute( "num" ) ) {
        vrvMensur->m_num = atoi ( mensur->Attribute( "num" ) );
    }
    if ( mensur->Attribute( "numbase" ) ) {
        vrvMensur->m_numBase = atoi ( mensur->Attribute( "numbase" ) );
    }
    // missing m_meterSymb
    
    return vrvMensur;
}

LayerElement *MeiInput::ReadMeiMultiRest( TiXmlElement *multiRest )
{
	MultiRest *vrvMultiRest = new MultiRest();
    
	// pitch
    if ( multiRest->Attribute( "num" ) ) {
        vrvMultiRest->SetNumber( atoi ( multiRest->Attribute( "num" ) ) );
    }
	
	return vrvMultiRest;
}

LayerElement *MeiInput::ReadMeiNote( TiXmlElement *note )
{
	Note *vrvNote = new Note();
    
	// pitch
	if ( note->Attribute( "pname" ) ) {
		vrvNote->m_pname = StrToPitch( note->Attribute( "pname" ) );
	}
	// oct
	if ( note->Attribute( "oct" ) ) {
		vrvNote->m_oct = StrToOct( note->Attribute( "oct" ) );
	}
	// duration
	if ( note->Attribute( "dur" ) ) {
		vrvNote->m_dur = StrToDur( note->Attribute( "dur" ) );
	}
    // dots
    if ( note->Attribute( "dots" ) ) {
		vrvNote->m_dots = atoi( note->Attribute( "dots" ) );
	}
    // accid
    if ( note->Attribute( "accid" ) ) {
		vrvNote->m_accid = StrToAccid( note->Attribute( "accid" ) );
	}
    // ligature
    if ( note->Attribute( "lig" ) ) {
        vrvNote->m_lig = true; // this has to be double checked
        if ( strcmp( note->Attribute( "lig" ), "obliqua" ) == 0 ) {
            vrvNote->m_ligObliqua = true;
        }
    }
    // stem direction
    if ( note->Attribute( "stem.dir" ) ) {
        // we use it to indicate opposite direction
        vrvNote->m_stemDir = 1;
    }
    // coloration
    if ( note->Attribute( "colored" ) ) {
        vrvNote->m_colored = ( strcmp ( note->Attribute( "colored" ), "true" ) == 0 );
    }
    // coloration
    if ( note->Attribute( "tie" ) ) {
        if ( (strcmp ( note->Attribute( "tie" ), "i" ) == 0) || (strcmp ( note->Attribute( "tie" ), "m" ) == 0) ) {
            vrvNote->SetTieAttrInitial();
            m_openTies.push_back( vrvNote );
        }
        if ( (strcmp ( note->Attribute( "tie" ), "t" ) == 0) || (strcmp ( note->Attribute( "tie" ), "m" ) == 0) ) {
            if (!FindOpenTie( vrvNote ) ) {
                LogWarning("Initial tie could not be found" );
            }
        }
    }
	
	return vrvNote;
}


LayerElement *MeiInput::ReadMeiRest( TiXmlElement *rest )
{
    Rest *vrvRest = new Rest();
    
	// duration
	if ( rest->Attribute( "dur" ) ) {
		vrvRest->m_dur = StrToDur( rest->Attribute( "dur" ) );
	}
    if ( rest->Attribute( "dots" ) ) {
		vrvRest->m_dots = atoi( rest->Attribute( "dots" ) );
	}
    // position
	if ( rest->Attribute( "ploc" ) ) {
		vrvRest->m_pname = StrToPitch( rest->Attribute( "ploc" ) );
	}
	// oct
	if ( rest->Attribute( "oloc" ) ) {
		vrvRest->m_oct = StrToOct( rest->Attribute( "oloc" ) );
	}
	
    return vrvRest;
}


LayerElement *MeiInput::ReadMeiTuplet( TiXmlElement *tuplet )
{
    assert ( !m_tuplet );
    
    // m_tuplet will be used for adding elements to the beam
    m_tuplet = new Tuplet();
    
    Object *previousLayer = m_currentLayer;
    m_currentLayer = m_tuplet;
    
    // Read in the numerator and denominator properties
    if ( tuplet->Attribute( "num" ) ) {
		m_tuplet->m_num = atoi( tuplet->Attribute( "num" ) );
	}
    if ( tuplet->Attribute( "numbase" ) ) {
		m_tuplet->m_numbase = atoi( tuplet->Attribute( "numbase" ) );
	}
    
    TiXmlElement *current = NULL;
    for( current = tuplet->FirstChildElement( ); current; current = current->NextSiblingElement( ) ) {
        ReadMeiLayerElement( current );
    }
    
    if ( m_tuplet->GetNoteCount() == 1 ) {
        LogWarning("Tuplet element with only one note");
    }
    // switch back to the previous one
    m_currentLayer = previousLayer;
    if ( m_tuplet->GetNoteCount() < 1 ) {
        delete m_tuplet;
        return NULL;
    }
    else {
        // set the member to NULL but keep a pointer to be returned
        Tuplet *vrvTuplet = m_tuplet;
        m_tuplet = NULL;
        return vrvTuplet;
    }
}


LayerElement *MeiInput::ReadMeiAccid( TiXmlElement *accid )
{
    Symbol *vrvAccid = new Symbol( SYMBOL_ACCID );
    
    if ( accid->Attribute( "accid" ) ) {
        vrvAccid->m_accid = StrToAccid( accid->Attribute( "accid" ) );
    }
    // position
	if ( accid->Attribute( "ploc" ) ) {
		vrvAccid->m_pname = StrToPitch( accid->Attribute( "ploc" ) );
	}
	// oct
	if ( accid->Attribute( "oloc" ) ) {
		vrvAccid->m_oct = StrToOct( accid->Attribute( "oloc" ) );
	}
	
	return vrvAccid;
}

LayerElement *MeiInput::ReadMeiCustos( TiXmlElement *custos )
{
    Symbol *vrvCustos = new Symbol( SYMBOL_CUSTOS );
    
	// position (pitch)
	if ( custos->Attribute( "pname" ) ) {
		vrvCustos->m_pname = StrToPitch( custos->Attribute( "pname" ) );
	}
	// oct
	if ( custos->Attribute( "oct" ) ) {
		vrvCustos->m_oct = StrToOct( custos->Attribute( "oct" ) );
	}
	
	return vrvCustos;    
}

LayerElement *MeiInput::ReadMeiDot( TiXmlElement *dot )
{
    Symbol *vrvDot = new Symbol( SYMBOL_DOT );
    
    vrvDot->m_dot = 0;
    // missing m_dots
    // position
	if ( dot->Attribute( "ploc" ) ) {
		vrvDot->m_pname = StrToPitch( dot->Attribute( "ploc" ) );
	}
	// oct
	if ( dot->Attribute( "oloc" ) ) {
		vrvDot->m_oct = StrToOct( dot->Attribute( "oloc" ) );
	}
	
	return vrvDot;
}

LayerElement *MeiInput::ReadMeiApp( TiXmlElement *app )
{
    m_layerApp = new LayerApp( );
   
    TiXmlElement *current = NULL;
    for( current = app->FirstChildElement( "rdg" ); current; current = current->NextSiblingElement( "rdg" ) ) {
        ReadMeiRdg( current );
	}
	
    // set the member to NULL but keep a pointer to be returned
    LayerApp *layerApp = m_layerApp;
    m_layerApp = NULL;
    
    return layerApp;
}

bool MeiInput::ReadMeiRdg( TiXmlElement *rdg )
{
    assert ( !m_layerRdg );
    assert( m_layerApp );
    
    m_layerRdg = new LayerRdg( );
    
    if ( rdg->Attribute( "source" ) ) {
        m_layerRdg->m_source = rdg->Attribute( "source" );
    }
    
    // switch to the rdg
    Object *previousLayer = m_currentLayer;
    m_currentLayer = m_layerRdg;
 
    TiXmlElement *current = NULL;
    for( current = rdg->FirstChildElement( ); current; current = current->NextSiblingElement( ) ) {
        ReadMeiLayerElement( current );
    }
    
    // switch back to the previous one
    m_currentLayer = previousLayer;

    // set the member to NULL but keep a pointer to be returned
    LayerRdg *layerRdg = m_layerRdg;
    m_layerRdg = NULL;
    
    return layerRdg;
}


void MeiInput::ReadSameAsAttr( TiXmlElement *element, Object *object )
{
    if ( !element->Attribute( "sameas" ) ) {
        return;
    }
    
    object->m_sameAs = element->Attribute( "sameas" );
}


void MeiInput::AddLayerElement( LayerElement *element )
{
    assert( m_currentLayer );
    if ( dynamic_cast<Layer*>( m_currentLayer ) ) {
        ((Layer*)m_currentLayer)->AddElement( element );
    }
    else if ( dynamic_cast<LayerRdg*>( m_currentLayer ) ) {
        ((LayerRdg*)m_currentLayer)->AddElement( element );
    }
    else if ( dynamic_cast<Beam*>( m_currentLayer ) ) {
        ((Beam*)m_currentLayer)->AddElement( element );
    }
    else if ( dynamic_cast<Tuplet*>( m_currentLayer ) ) {
        ((Tuplet*)m_currentLayer)->AddElement( element );
    }
    
}


bool MeiInput::ReadUnsupported( TiXmlElement *element )
{
    if ( std::string( element->Value() ) == "score" ) {
        TiXmlElement *current = NULL;
        for( current = element->FirstChildElement( ); current; current = current->NextSiblingElement( ) ) {
            ReadUnsupported( current );
        }
    }
    if ( std::string( element->Value() ) == "section" ) {
        TiXmlElement *current = NULL;
        for( current = element->FirstChildElement( ); current; current = current->NextSiblingElement( ) ) {
            ReadUnsupported( current );
        }       
    }
    else if ( std::string( element->Value() ) == "measure" ) {
        LogDebug( "measure" );
        m_measure = new Measure( );
        SetMeiUuid( element, m_measure );
        if (ReadMeiMeasure( element )) {
            m_system->AddMeasure( m_measure );
        }
        else {
            delete m_measure;
        }
        m_measure = NULL;
    }
    /*
    else if ( std::string( element->Value() ) == "staff" ) {
        LogDebug( "staff" );
        int n = 1;
        if ( element->Attribute( "n" ) ) {
            element->Attribute( "n", &n );
        }
        Staff *staff = m_system->GetStaff( n - 1 );
        if ( staff ) {
            m_staff = staff;
        }
        else
        {
            m_staff = new Staff( n );
            m_system->AddStaff( m_staff );
        }
        m_measure = new Measure( *m_contentBasedMeasure );
        ReadMeiStaff( element );
    }
    */
    else if ( (std::string( element->Value() ) == "pb") && (m_system->GetMeasureCount() > 0 ) ) {
        LogDebug( "pb" );
        this->m_hasLayoutInformation = true;
        m_page = new Page( );
        m_system = new System( );
        m_page->AddSystem( m_system );
        m_doc->AddPage( m_page );
        
    }
    else if ( (std::string( element->Value() ) == "sb") && (m_page->GetSystemCount() > 0 ) ) {
        LogDebug( "sb" );
        this->m_hasLayoutInformation = true;
        m_system = new System( );
        m_page->AddSystem( m_system );
    }
    else if ( (std::string( element->Value() ) == "scoreDef") && ( !m_hasScoreDef ) ) {
        LogDebug( "scoreDef" );
        m_scoreDef = &m_doc->m_scoreDef;
        SetMeiUuid( element, m_scoreDef );
        if (ReadMeiScoreDef( element )) {
            m_hasScoreDef = true;
        }
        else {
            m_hasScoreDef = false;
        }
    }
    else {
        LogWarning( "Element %s ignored", element->Value() );
    }
    return true;
}

bool MeiInput::FindOpenTie( Note *terminalNote )
{
    assert( m_staff );
    assert( m_layer );
    
    std::vector<Note*>::iterator iter;
    for (iter = m_openTies.begin(); iter != m_openTies.end(); ++iter)
    {
        // we need to get the parent layer for comparing their number
        Layer *parentLayer = dynamic_cast<Layer*>( (*iter)->GetFirstParent( &typeid(Layer) ) );
        if (!parentLayer) {
            continue;
        }
        Staff *parentStaff = dynamic_cast<Staff*>( parentLayer->GetFirstParent( &typeid(Staff) ) );
        // We assume the if the note has no parent staff it is because we are in the same layer and that
        // the layer has not been added to its parent staff yet.
        // If we have one, compare the number
        if ( (parentStaff) && (m_staff->GetStaffNo() != parentStaff->GetStaffNo()) ) {
            continue;
        }
        // same layer?
        if ( m_layer->GetLayerNo() != parentLayer->GetLayerNo() ) {
            continue;
        }
        // we only compare oct and pname because alteration is not relevant for ties
        if ( (terminalNote->m_oct == (*iter)->m_oct) && (terminalNote->m_pname == (*iter)->m_pname) ) {
            terminalNote->SetTieAttrTerminal( *iter );
            m_openTies.erase(iter);
            return true;
        }
        
    }
    return false;
}

void MeiInput::SetMeiUuid( TiXmlElement *element, Object *object )
{
    if ( !element->Attribute( "xml:id" ) ) {
        return;
    }
    
    object->SetUuid( element->Attribute( "xml:id" ) );
}

bool MeiInput::StrToBool(std::string value)
{
    if (value == "false") return false;
	return true;
}

int MeiInput::StrToDur(std::string dur)
{
    int value;
    if (dur == "longa") value = DUR_LG;
    else if (dur == "brevis") value = DUR_BR;
    else if (dur == "semibrevis") value = DUR_1;
    else if (dur == "minima") value = DUR_2;
    else if (dur == "semiminima") value = DUR_4;
    else if (dur == "fusa") value = DUR_8;
    else if (dur == "semifusa") value = DUR_16;
    else if (dur == "long") value = DUR_LG;
    else if (dur == "breve") value = DUR_BR;
    else if (dur == "1") value = DUR_1;
    else if (dur == "2") value = DUR_2;
    else if (dur == "4") value = DUR_4;
    else if (dur == "8") value = DUR_8;
    else if (dur == "16") value = DUR_16;
    else if (dur == "32") value = DUR_32;
    else if (dur == "64") value = DUR_64;
    else if (dur == "128") value = DUR_128;
	else {
		//LogWarning("Unknown duration '%s'", dur.c_str());
        value = DUR_4;
	}
    return value;
}

int MeiInput::StrToOct(std::string oct)
{
	return atoi(oct.c_str());
}

int MeiInput::StrToPitch(std::string pitch)
{
    int value;
    if (pitch == "c") value = PITCH_C;
    else if (pitch == "d") value = PITCH_D;
    else if (pitch == "e") value = PITCH_E;
    else if (pitch == "f") value = PITCH_F;
    else if (pitch == "g") value = PITCH_G;
    else if (pitch == "a") value = PITCH_A;
    else if (pitch == "b") value = PITCH_B;
    else {
		LogWarning("Unknow pname '%s'", pitch.c_str());
        value = PITCH_C;
    }
    return value;
}


unsigned char MeiInput::StrToAccid(std::string accid)
{
    unsigned char value;
    if ( accid == "s" ) value = ACCID_SHARP;
    else if ( accid == "f" ) value = ACCID_FLAT;
    else if ( accid == "n" ) value = ACCID_NATURAL;
    else if ( accid == "x" ) value = ACCID_DOUBLE_SHARP;
    else if ( accid == "ff" ) value = ACCID_DOUBLE_FLAT;
    else if ( accid == "ns" ) value = ACCID_QUARTER_SHARP;
    else if ( accid == "nf" ) value = ACCID_QUARTER_FLAT;
    else {
        LogWarning("Unknown accid '%s'", accid.c_str() );
        value = ACCID_NATURAL;
    }
	return value;
}


ClefId MeiInput::StrToClef( std::string shape, std::string line )
{
    ClefId clefId = SOL2;
    std::string clef = shape + line;
    if ( clef == "G2" ) clefId = SOL2;
    else if ( clef == "G1" ) clefId = SOL1; 
    else if ( clef == "F5" ) clefId = FA5;
    else if ( clef == "F4" ) clefId = FA4; 
    else if ( clef == "F3" ) clefId = FA3; 
    else if ( clef == "C1" ) clefId = UT1; 
    else if ( clef == "C2" ) clefId = UT2; 
    else if ( clef == "C3" ) clefId = UT3; 
    else if ( clef == "C4" ) clefId = UT4; 
    else if ( clef == "C5" ) clefId = UT5; 
    else 
    {
        LogWarning("Unknown clef shape '%s' line '%s'", shape.c_str(), line.c_str() );
    }
    return clefId;
}

MensurSign MeiInput::StrToMensurSign(std::string sign)
{
    if (sign == "C") return MENSUR_SIGN_C;
    else if (sign == "O") return MENSUR_SIGN_O;
    else {
        LogWarning("Unknown mensur sign '%s'", sign.c_str() );
	}
    // default
	return MENSUR_SIGN_C;
}

DocType MeiInput::StrToDocType(std::string type)
{
    if (type == "raw") return Raw;
    else if (type == "rendering") return Rendering;
    else if (type == "transcription") return Transcription;
    else {
        LogWarning("Unknown layout type '%s'", type.c_str() );
	}
    // default
	return Raw;
}

unsigned char MeiInput::StrToKeySigType(std::string accid)
{
    if ( accid == "0" ) return  ACCID_NATURAL;
    else if ( accid.at(1) == 'f' ) return ACCID_FLAT;
    else if ( accid.at(1) == 's' ) return ACCID_SHARP;
    else {
        LogWarning("Unknown keysig '%s'", accid.c_str() );
        return ACCID_NATURAL;
    }
}

int MeiInput::StrToKeySigNum(std::string accid)
{
    if ( accid == "0" ) return  0;
    else {
        // low level way, remove '0', which is 48
        return accid.at(0) - '0';
    }
}

BarlineType MeiInput::StrToBarlineType(std::string type)
{
    if (type == "sigle") return BARLINE_SINGLE;
    else if (type == "end") return BARLINE_END;
    else if (type == "dbl") return BARLINE_DBL;
    else if (type == "rptend") return BARLINE_RPTEND;
    else if (type == "rptstart") return BARLINE_RPTSTART;
    else if (type == "rptboth") return BARLINE_RPTBOTH;
    else {
        LogWarning("Unknown barline type '%s'", type.c_str() );
	}
    // default
	return BARLINE_SINGLE;
}

StaffGrpSymbol MeiInput::StrToStaffGrpSymbol(std::string symbol)
{
    if (symbol == "line") return STAFFGRP_LINE;
    else if (symbol == "brace") return STAFFGRP_BRACE;
    else if (symbol == "bracket") return STAFFGRP_BRACKET;
    else {
        LogWarning("Unknown staffGrp @symbol '%s'", symbol.c_str() );
	}
    // default
	return STAFFGRP_LINE;
}
    
} // namespace vrv

