/////////////////////////////////////////////////////////////////////////////
// Name:        iomei.cpp
// Author:      Laurent Pugin
// Created:     2008
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iomei.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <ctime>
#include <sstream>

//----------------------------------------------------------------------------

#include "accid.h"
#include "barline.h"
#include "beam.h"
#include "clef.h"
#include "custos.h"
#include "dot.h"
#include "keysig.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "object.h"
#include "page.h"
#include "rest.h"
#include "scoredef.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tuplet.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeiOutput
//----------------------------------------------------------------------------

MeiOutput::MeiOutput( Doc *doc, std::string filename ) :
	FileOutputStream( doc )
{
    m_filename = filename;
}

MeiOutput::~MeiOutput()
{
}

bool MeiOutput::ExportFile( )
{
    try {
        pugi::xml_document meiDoc;
        
        m_mei = meiDoc.append_child("mei");
        m_mei.append_attribute( "xmlns" ) = "http://www.music-encoding.org/ns/mei";
        m_mei.append_attribute( "meiversion" ) = "2013";
        
        // this starts the call of all the functors
        m_doc->Save( this );
        
        /*
        TiXmlUnknown *schema = new TiXmlUnknown();
        schema->SetValue("?xml-model href=\"http://www.aruspix.net/mei-page-based-2013-08-29.rng\" type=\"application/xml\" schematypens=\"http://relaxng.org/ns/structure/1.0\"?");
        
        meiDoc->LinkEndChild( new TiXmlDeclaration( "1.0", "UTF-8", "" ) );
        meiDoc->LinkEndChild(schema);
        */
        meiDoc.save_file( m_filename.c_str() );
    }
    catch( char * str ) {
        LogError("%s", str );
        return false;
    }
	return true;    
}
    
bool MeiOutput::WriteObject( Object *object )
{    
    // Containers and scoreDef related
    if (dynamic_cast<Doc*>(object)) {
        WriteMeiDoc( dynamic_cast<Doc*>(object) );        
        m_nodeStack.push_back(m_currentNode);
        return true;
    }
    
    if (dynamic_cast<Page*>(object)) {
        m_currentNode = m_currentNode.append_child("page");
        WriteMeiPage( m_currentNode, dynamic_cast<Page*>(object) );
    }
    else if (dynamic_cast<System*>(object)) {
        m_currentNode = m_currentNode.append_child("system");
        WriteMeiSystem( m_currentNode, dynamic_cast<System*>(object) );
    }
    else if (dynamic_cast<ScoreDef*>(object)) {
        m_currentNode = m_currentNode.append_child("scoreDef");
        WriteMeiScoreDef( m_currentNode, dynamic_cast<ScoreDef*>(object) );
    }
    else if (dynamic_cast<StaffGrp*>(object)) {
        m_currentNode = m_currentNode.append_child("staffGrp");
        WriteMeiStaffGrp( m_currentNode, dynamic_cast<StaffGrp*>(object) );
    }
    else if (dynamic_cast<StaffDef*>(object)) {
        m_currentNode = m_currentNode.append_child("staffDef");
        WriteMeiStaffDef( m_currentNode, dynamic_cast<StaffDef*>(object) );
    }
    else if (dynamic_cast<Measure*>(object)) {
        m_currentNode = m_currentNode.append_child("measure");
        WriteMeiMeasure( m_currentNode, dynamic_cast<Measure*>(object) );
    }
    else if (dynamic_cast<Staff*>(object)) {
        m_currentNode = m_currentNode.append_child("staff");
        WriteMeiStaff( m_currentNode, dynamic_cast<Staff*>(object) );
    }
    else if (dynamic_cast<Layer*>(object)) {
        m_currentNode = m_currentNode.append_child("layer");
        WriteMeiLayer( m_currentNode, dynamic_cast<Layer*>(object) );
    }
    
    // Layer elements
    else if (dynamic_cast<Accid*>(object)) {
        m_currentNode = m_currentNode.append_child("accid");
        WriteMeiAccid( m_currentNode, dynamic_cast<Accid*>(object) );
    }
    else if (dynamic_cast<Barline*>(object)) {
        m_currentNode = m_currentNode.append_child( "barLine" );
        WriteMeiBarline( m_currentNode, dynamic_cast<Barline*>(object) );
    }
    else if (dynamic_cast<Beam*>(object)) {
        m_currentNode = m_currentNode.append_child("beam");
        WriteMeiBeam( m_currentNode, dynamic_cast<Beam*>(object) );
    }
    else if (dynamic_cast<Clef*>(object)) {
        m_currentNode = m_currentNode.append_child("clef");
        WriteMeiClef( m_currentNode, dynamic_cast<Clef*>(object) );
    }
    else if (dynamic_cast<Custos*>(object)) {
        m_currentNode = m_currentNode.append_child( "custos" );
        WriteMeiCustos( m_currentNode, dynamic_cast<Custos*>(object) );
    }
    else if (dynamic_cast<Dot*>(object)) {
        m_currentNode = m_currentNode.append_child( "dot" );
        WriteMeiDot( m_currentNode, dynamic_cast<Dot*>(object) );
    }
    else if (dynamic_cast<Mensur*>(object)) {
        m_currentNode = m_currentNode.append_child("mensur");
        WriteMeiMensur( m_currentNode, dynamic_cast<Mensur*>(object) );
    }
    else if (dynamic_cast<MRest*>(object)) {
        m_currentNode = m_currentNode.append_child("mRest");
        WriteMeiMRest( m_currentNode, dynamic_cast<MRest*>(object) );
    }
    else if (dynamic_cast<MultiRest*>(object)) {
        m_currentNode = m_currentNode.append_child("multiRest");
        WriteMeiMultiRest( m_currentNode, dynamic_cast<MultiRest*>(object) );
    }
    else if (dynamic_cast<Note*>(object)) {
        m_currentNode = m_currentNode.append_child("note");
        WriteMeiNote( m_currentNode, dynamic_cast<Note*>(object) );
    }
    else if (dynamic_cast<Rest*>(object)) {
        m_currentNode = m_currentNode.append_child("rest");
        WriteMeiRest( m_currentNode, dynamic_cast<Rest*>(object) );
    }
    else if (dynamic_cast<Tuplet*>(object)) {
        m_currentNode = m_currentNode.append_child("tuplet");
        WriteMeiTuplet( m_currentNode, dynamic_cast<Tuplet*>(object) );
    }
    
    // others
    else if (dynamic_cast<Verse*>(object)) {
        m_currentNode = m_currentNode.append_child("verse");
        WriteMeiVerse( m_currentNode, dynamic_cast<Verse*>(object) );
    }
    else if (dynamic_cast<Syl*>(object)) {
        m_currentNode = m_currentNode.append_child("syl");
        WriteMeiSyl( m_currentNode, dynamic_cast<Syl*>(object) );
    }
    
    else {
        // Missing output method for the class
        assert( false ); // let's make it stop because this should not happen
    }
    
    //LogDebug( "Current: %s ", m_currentNode.value() );
    m_nodeStack.push_back(m_currentNode);
    
    return true;
}
    
bool MeiOutput::WriteObjectEnd( Object *object )
{
    m_nodeStack.pop_back();
    m_currentNode = m_nodeStack.back();
    return true;
}

std::string MeiOutput::UuidToMeiStr( Object *element )
{
    std::string out = element->GetUuid();
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    //LogDebug("uuid: %s", out.c_str());
    return out;
}

bool MeiOutput::WriteMeiDoc( Doc *doc )
{
    assert( !m_mei.empty() );
    
    // ---- header ----
    
    pugi::xml_node meiHead = m_mei.append_child("meiHead");
    
    pugi::xml_node fileDesc = meiHead.append_child("fileDesc");
    pugi::xml_node titleStmt = fileDesc.append_child("titleStmt");
    titleStmt.append_child("title");
    pugi::xml_node pubStmt = fileDesc.append_child("pubStmt");
    pugi::xml_node date = pubStmt.append_child("date");
    
    pugi::xml_node encodingDesc = meiHead.append_child("encodingDesc");
    pugi::xml_node projectDesc = encodingDesc.append_child("projectDesc");
    pugi::xml_node p1 = projectDesc.append_child("p");
    p1.append_child(pugi::node_pcdata).set_value( StringFormat( "Encoded with Verovio version %s",  GetVersion().c_str() ).c_str() );
    
    // date
    time_t now = time(0);
    date.append_child(pugi::node_pcdata).set_value( ctime( &now ) );
    
    // ---- music ----
   
    pugi::xml_node music = m_mei.append_child("music");
    pugi::xml_node body = music.append_child("body");
    pugi::xml_node mdiv = body.append_child("mdiv");
    
    // element to place the pages
    m_currentNode = mdiv.append_child("pages");
    m_currentNode.append_attribute( "type" ) = DocTypeToStr( m_doc->GetType() ).c_str();
    m_currentNode.append_child(pugi::node_comment).set_value( "Coordinates in MEI axis direction" );
    
    return true;
}


bool MeiOutput::WriteMeiPage( pugi::xml_node currentNode, Page *page )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( page ).c_str();
    // size and margins but only if any - we rely on page.height only to check this
    if ( page->m_pageHeight != -1 ) {
        currentNode.append_attribute( "page.width" ) = StringFormat( "%d", page->m_pageWidth ).c_str();
        currentNode.append_attribute( "page.height" ) = StringFormat( "%d", page->m_pageHeight ).c_str();
        currentNode.append_attribute( "page.leftmar" ) = StringFormat( "%d", page->m_pageLeftMar ).c_str();
        currentNode.append_attribute( "page.rightmar" ) = StringFormat( "%d", page->m_pageRightMar ).c_str();
        currentNode.append_attribute( "page.rightmar" ) = StringFormat( "%d", page->m_pageRightMar ).c_str();
    }
    if ( !page->m_surface.empty() ) {
        currentNode.append_attribute( "surface" ) = page->m_surface.c_str();
    }
    return true;
}

bool MeiOutput::WriteMeiSystem( pugi::xml_node currentNode, System *system )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( system ).c_str();
    // margins
    currentNode.append_attribute( "system.leftmar" ) = StringFormat( "%d", system->m_systemLeftMar ).c_str();
    currentNode.append_attribute( "system.rightmar") = StringFormat( "%d", system->m_systemRightMar ).c_str();
    // y positions
    currentNode.append_attribute( "uly" ) = StringFormat( "%d", system->m_yAbs ).c_str();
    
    return true;
}

bool MeiOutput::WriteMeiScoreDef( pugi::xml_node currentNode, ScoreDef *scoreDef )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( scoreDef ).c_str();
    if (scoreDef->GetClefAttr()) {
        scoreDef->GetClefAttr()->WriteCleffingLog(currentNode);
    }
    if (scoreDef->GetKeySigAttr()) {
        scoreDef->GetKeySigAttr()->WriteKeySigDefaultLog(currentNode);
    }
    if ( scoreDef->GetMensurAttr() ) {
        dynamic_cast<AttMensuralLog*>(scoreDef)->WriteMensuralLog(currentNode);
    }
    if ( scoreDef->GetMeterSigAttr() ) {
        scoreDef->GetMeterSigAttr()->WriteMeterSigDefaultLog(currentNode);
        scoreDef->GetMeterSigAttr()->WriteMeterSigDefaultVis(currentNode);
    }
    
    // this needs to be fixed
    return true;
}

bool MeiOutput::WriteMeiStaffGrp( pugi::xml_node currentNode, StaffGrp *staffGrp )
{
    currentNode.append_attribute( "xml:id" ) = UuidToMeiStr( staffGrp ).c_str();
    if ( staffGrp->GetSymbol() != STAFFGRP_NONE ) {
        currentNode.append_attribute( "symbol" ) = StaffGrpSymbolToStr( staffGrp->GetSymbol() ).c_str();
    }
    if ( staffGrp->GetBarthru() ) {
        currentNode.append_attribute( "barthru" ) = BoolToStr( staffGrp->GetBarthru() ).c_str();
    }
    
    return true;
}

bool MeiOutput::WriteMeiStaffDef( pugi::xml_node currentNode, StaffDef *staffDef )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( staffDef ).c_str();
    currentNode.append_attribute( "n" ) = StringFormat( "%d", staffDef->GetN() ).c_str();
    if (staffDef->GetClefAttr()) {
        staffDef->GetClefAttr()->WriteCleffingLog(currentNode);
    }
    if (staffDef->GetKeySigAttr()) {
        staffDef->GetKeySigAttr()->WriteKeySigDefaultLog(currentNode);
    }
    if ( staffDef->GetMensurAttr() ) {
        staffDef->GetMensurAttr()->WriteMensuralLog(currentNode);
    }
    if ( staffDef->GetMeterSigAttr() ) {
        staffDef->GetMeterSigAttr()->WriteMeterSigDefaultLog(currentNode);
        staffDef->GetMeterSigAttr()->WriteMeterSigDefaultVis(currentNode);
    }
    
    return true;
}
      
bool MeiOutput::WriteMeiMeasure( pugi::xml_node currentNode, Measure *measure )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( measure ).c_str();

    measure->WriteCommon(currentNode);

    // here we transfer the barLine object values to @left and @right
    measure->SetLeft( measure->GetLeftBarlineType() );
    measure->SetRight( measure->GetRightBarlineType() );
    measure->WriteMeasureLog(currentNode);
    
    return true;
}

bool MeiOutput::WriteMeiStaff( pugi::xml_node currentNode, Staff *staff )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( staff ).c_str();
    
    staff->WriteCommon(currentNode);
    
    // y position
    if ( staff->notAnc ) {
        currentNode.append_attribute( "label" ) = "mensural";
    }
    currentNode.append_attribute( "uly" ) = StringFormat( "%d", staff->m_yAbs ).c_str();

    return true;
}


bool MeiOutput::WriteMeiLayer( pugi::xml_node currentNode, Layer *layer )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( layer ).c_str();
    
    layer->WriteCommon(currentNode);
    
    return true;
}

void MeiOutput::WriteLayerElement( pugi::xml_node currentNode, LayerElement *element )
{
    this->WriteSameAsAttr( currentNode, element );
    
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( element ).c_str();
    
    if ( element->m_xAbs != VRV_UNSET) {
        currentNode.append_attribute( "ulx" ) = StringFormat( "%d", element->m_xAbs ).c_str();
    }
}
    
void MeiOutput::WriteMeiAccid( pugi::xml_node currentNode, Accid *accid )
{
    WriteLayerElement( currentNode, accid );
    WritePositionInterface(currentNode, accid);
    accid->WriteAccidental(currentNode);
}

void MeiOutput::WriteMeiBarline( pugi::xml_node currentNode, Barline *barLine )
{
    WriteLayerElement( currentNode, barLine );
    barLine->WriteBarLineLog(currentNode);
    return;
}


void MeiOutput::WriteMeiBeam( pugi::xml_node currentNode, Beam *beam )
{
    WriteLayerElement( currentNode, beam );
    return;
}


void MeiOutput::WriteMeiClef( pugi::xml_node currentNode, Clef *clef )
{
    WriteLayerElement( currentNode, clef );
    clef->WriteClefshape(currentNode);
    clef->WriteLineloc(currentNode);
    clef->WriteOctavedisplacement(currentNode);
    return;
}

void MeiOutput::WriteMeiCustos( pugi::xml_node currentNode, Custos *custos )
{
    WriteLayerElement( currentNode, custos );
    WritePositionInterface(currentNode, custos);
}

void MeiOutput::WriteMeiDot( pugi::xml_node currentNode, Dot *dot )
{
    WriteLayerElement( currentNode, dot );
    WritePositionInterface(currentNode, dot);
}

void MeiOutput::WriteMeiMensur( pugi::xml_node currentNode, Mensur *mensur )
{
    WriteLayerElement( currentNode, mensur );
    mensur->WriteDurationRatio(currentNode);
    mensur->WriteMensurLog(currentNode);
    mensur->WriteMensurVis(currentNode);
    mensur->WriteSlashcount(currentNode);
    return;
}

void MeiOutput::WriteMeiMeterSig( pugi::xml_node currentNode, MeterSig *meterSig )
{
    WriteLayerElement( currentNode, meterSig );
    meterSig->WriteMeterSigLog(currentNode);
    return;
}
    
    
void MeiOutput::WriteMeiMRest( pugi::xml_node currentNode, MRest *mRest )
{
     return;
}

void MeiOutput::WriteMeiMultiRest( pugi::xml_node currentNode, MultiRest *multiRest )
{
    WriteLayerElement( currentNode, multiRest );
    multiRest->WriteNumbered(currentNode);
    return;
}

void MeiOutput::WriteMeiNote( pugi::xml_node currentNode, Note *note )
{
    WriteLayerElement( currentNode, note );
    WriteDurationInterface(currentNode, note);
    WritePitchInterface(currentNode, note);
    note->WriteColoration(currentNode);
    note->WriteNoteLogMensural(currentNode);
    note->WriteStemmed(currentNode);
    
    if ( note->m_cueSize ) {
        currentNode.append_attribute( "grace" ) = "unknown";
    }
    return;
}

void MeiOutput::WriteMeiRest( pugi::xml_node currentNode, Rest *rest )
{
    WriteLayerElement( currentNode, rest );
    WriteDurationInterface(currentNode, rest);
    WritePositionInterface(currentNode, rest);
    return;
}

void MeiOutput::WriteMeiTuplet( pugi::xml_node currentNode, Tuplet *tuplet )
{
    WriteLayerElement( currentNode, tuplet  );
    tuplet->WriteDurationRatio( currentNode );
    return;
}
    

void MeiOutput::WriteMeiVerse( pugi::xml_node currentNode, Verse *verse )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( verse ).c_str();
    verse->WriteCommon( currentNode );    
    return;
}

void MeiOutput::WriteMeiSyl( pugi::xml_node currentNode, Syl *syl )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( syl ).c_str();
    WriteText( currentNode, syl );
    return;
}
    
void MeiOutput::WriteDurationInterface(pugi::xml_node element, vrv::DurationInterface *interface)
{
    interface->WriteAugmentdots(element);
    interface->WriteBeamsecondary(element);
    interface->WriteDurationMusical(element);
    interface->WriteDurationPerformed(element);
    interface->WriteDurationRatio(element);
    interface->WriteFermatapresent(element);
}
    
void MeiOutput::WritePitchInterface(pugi::xml_node element, vrv::PitchInterface *interface)
{
    interface->WriteAccidental(element);
    interface->WriteOctave(element);
    interface->WritePitch(element);
}
    
void MeiOutput::WritePositionInterface(pugi::xml_node element, vrv::PositionInterface *interface)
{

}

void MeiOutput::WriteSameAsAttr(pugi::xml_node element, Object *object)
{
    if ( !object->m_sameAs.empty() ) {
        element.append_attribute( "sameas" ) = object->m_sameAs.c_str();
    }
}
    
    
void MeiOutput::WriteText( pugi::xml_node element, Object *object )
{
    if ( !object->GetText().empty() ) {
        element.text() =  UTF16to8(object->GetText().c_str() ).c_str();
    }
}
    
std::string MeiOutput::BoolToStr(bool value)
{
    if (value) return "true";
    return "false";
}

/*
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
*/

std::string MeiOutput::DocTypeToStr(DocType type)
{
 	std::string value; 
	switch(type)
	{	
        case Raw : value = "raw"; break;
        case Rendering : value = "rendering"; break;
		case Transcription : value = "transcription"; break;		
        default: 
            LogWarning("Unknown document type '%d'", type);
            value = "";
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
    m_system = NULL;
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
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file( m_filename.c_str() );
        if (!result)
        { 
            return false;
        }
        pugi::xml_node root = doc.first_child();
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
        pugi::xml_document doc;
        doc.load( mei.c_str() );
        pugi::xml_node root = doc.first_child();
        return ReadMei( root );
    }
    catch( char * str ) {
        LogError("%s", str );
        return false;
    }
}



bool MeiInput::ReadMei( pugi::xml_node root )
{
    pugi::xml_node current;
    
    if ( !root.empty() && (current = root.child("meiHead" ) ) )
    {
        ReadMeiHeader( current );
    }
    // music
    pugi::xml_node music;
    pugi::xml_node body;
    pugi::xml_node mdiv;
    pugi::xml_node pages;
    if ( !root.empty() ) {
        music = root.child("music");
    }
    if ( !music.empty() ) {
        body = music.child("body");
    }
    if ( !body.empty() ) {
        mdiv = body.child("mdiv");
    }
    if ( !mdiv.empty() ) {
        pages = mdiv.child("pages");
    }
    if ( !pages.empty() ) {
        
        // check if there is a type attribute for the score
        DocType type;
        if ( pages.attribute( "type" ) ) {
            type = StrToDocType( pages.attribute( "type" ).value() );
            m_doc->Reset( type );
        }
        
        // this is a page-based MEI file, we just loop trough the pages
        if ( pages.child( "page" ) ) {
            // because we are in a page-based MEI
            this->m_hasLayoutInformation = true;
            for( current = pages.child( "page" ); current; current = current.next_sibling( "page" ) ) {
                ReadMeiPage( current );
            }
        }
    }
    else {
        m_page = new Page( );
        m_system  = new System( );
        m_page->AddSystem( m_system );
        m_doc->AddPage( m_page );
        pugi::xml_node current;
        for( current = mdiv.first_child( ); current; current = current.next_sibling( ) ) {
            ReadScoreBasedMei( current );
        }
    }
    
    if ( !m_openTies.empty()) {
        std::vector<Note*>::iterator iter;
        for (iter = m_openTies.begin(); iter != m_openTies.end(); ++iter)
        {
            LogWarning("Terminal @tie for <note> '%s' could not be matched", (*iter)->GetUuid().c_str() );
        }
    }
    
    return true;
}

bool MeiInput::ReadMeiHeader( pugi::xml_node meiHead )
{
    return true;
}


bool MeiInput::ReadMeiPage( pugi::xml_node page )
{
    Page *vrvPage = new Page();
    SetMeiUuid( page, vrvPage );
    
    if ( page.attribute( "page.height" ) ) {
        m_page->m_pageHeight = atoi ( page.attribute( "page.height" ).value() );
    }
    if ( page.attribute( "page.width" ) ) {
        m_page->m_pageWidth = atoi ( page.attribute( "page.width" ).value() );
    }
    if ( page.attribute( "page.leftmar" ) ) {
        m_page->m_pageLeftMar = atoi ( page.attribute( "page.leftmar" ).value() );
    }
    if ( page.attribute( "page.rightmar" ) ) {
        m_page->m_pageRightMar = atoi ( page.attribute( "page.rightmar" ).value() );
    }
    if ( page.attribute( "page.topmar" ) ) {
        m_page->m_pageTopMar = atoi ( page.attribute( "page.topmar" ).value() );
    }
    if ( page.attribute( "surface" ) ) {
        m_page->m_surface = page.attribute( "surface" ).value();
    }
    
    m_doc->AddPage( vrvPage );
    
    pugi::xml_node current;
    for( current = page.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "system" ) {
            ReadMeiSystem( vrvPage, current );
        }
        // can we have scoreDef between system in the current page-based cusotmization? To be checked
        else if ( std::string( current.name() ) == "scoreDef" ) {
            ReadMeiScoreDef( vrvPage, current );
        }
    }
    
    return true;
}

bool MeiInput::ReadMeiSystem( Page *page, pugi::xml_node system )
{
    System *vrvSystem = new System();
    SetMeiUuid(system, vrvSystem);
    
    if ( system.attribute( "system.leftmar") ) {
        vrvSystem->m_systemLeftMar = atoi ( system.attribute( "system.leftmar" ).value() );
    }
    if ( system.attribute( "system.rightmar" ) ) {
        vrvSystem->m_systemRightMar = atoi ( system.attribute( "system.rightmar" ).value() );
    }
    if ( system.attribute( "uly" ) ) {
        vrvSystem->m_yAbs = atoi ( system.attribute( "uly" ).value() );
    }
    
        page->AddSystem(vrvSystem);
    
    pugi::xml_node current;
    Measure *unmeasured = NULL;
    for( current = system.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "scoreDef" ) {
            // we should not have scoredef with unmeasured within a system... (?)
            assert(!unmeasured);
            ReadMeiScoreDef( vrvSystem, current );
        }
        // unmeasured music
        else if ( system.child( "staff" ) ) {
            if (!unmeasured) {
                unmeasured = new Measure( false );
                vrvSystem->AddMeasure(unmeasured);
            }
            ReadMeiStaff( unmeasured, current );
        }
        else {
            // we should not mix measure and unmeasured within a system...
            assert(!unmeasured);
            ReadMeiMeasure(vrvSystem, current);
        }
    }
    
    return true;
}

bool MeiInput::ReadMeiScoreDef( Object *parent, pugi::xml_node scoreDef )
{
    ScoreDef *vrvScoreDef;
    if (!m_hasScoreDef) {
        vrvScoreDef = &m_doc->m_scoreDef;
    }
    else {
        vrvScoreDef = new ScoreDef();
    }
    SetMeiUuid(scoreDef, vrvScoreDef);
    
    ClefAttr clefAttr;
    if ( clefAttr.ReadCleffingLog( scoreDef ) ) {
        vrvScoreDef->ReplaceClef( &clefAttr );
    }
    KeySigAttr keySigAttr;
    if ( keySigAttr.ReadKeySigDefaultLog( scoreDef ) ) {
        vrvScoreDef->ReplaceKeySig( &keySigAttr );
    }
    MeterSigAttr meterSig;
    if ( meterSig.ReadMeterSigDefaultLog( scoreDef ) || meterSig.ReadMeterSigDefaultVis( scoreDef ) ) {
        vrvScoreDef->ReplaceMeterSig( &meterSig );
    }
    MensurAttr mensur;
    if ( mensur.ReadMensuralLog( scoreDef ) ) {
        vrvScoreDef->ReplaceMensur( &mensur );
    }
    
    AddScoreDef(parent, vrvScoreDef);
    
    pugi::xml_node current;
    for( current = scoreDef.child( "staffGrp" ); current; current = current.next_sibling( "staffGrp" ) ) {
        ReadMeiStaffGrp(vrvScoreDef, current);
    }
    
    return true;
}

bool MeiInput::ReadMeiStaffGrp( Object *parent, pugi::xml_node staffGrp )
{
    StaffGrp *vrvStaffGrp = new StaffGrp( );
    SetMeiUuid( staffGrp, vrvStaffGrp );
    
    if ( staffGrp.attribute( "symbol" ) ) {
        vrvStaffGrp->SetSymbol( StrToStaffGrpSymbol( staffGrp.attribute( "symbol" ).value() ) );
    }
    if ( staffGrp.attribute( "barthru" ) ) {
        vrvStaffGrp->SetBarthru( StrToBool( staffGrp.attribute( "barthru" ).value() ) );
    }
    
    AddStaffGrp(parent, vrvStaffGrp);
    
    pugi::xml_node current;
    for( current = staffGrp.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "staffGrp" ) {
            ReadMeiStaffGrp( vrvStaffGrp, current);           
        }
        else if ( std::string( current.name() ) == "staffDef" ) {
            ReadMeiStaffDef( vrvStaffGrp, current );
        }        
    }
    
    return true;
}

bool MeiInput::ReadMeiStaffDef( StaffGrp *staffGrp, pugi::xml_node staffDef )
{
    StaffDef *vrvStaffDef = new StaffDef( );
    SetMeiUuid( staffDef, vrvStaffDef );
    
    if ( staffDef.attribute( "n" ) ) {
        vrvStaffDef->SetStaffNo( atoi ( staffDef.attribute( "n" ).value() ) );
    }
    else {
        LogWarning("No @n on <staffDef>");
    }
    
    ClefAttr clefAttr;
    if ( clefAttr.ReadCleffingLog( staffDef ) ) {
        vrvStaffDef->ReplaceClef( &clefAttr );
    }
    KeySigAttr keySigAttr;
    if ( keySigAttr.ReadKeySigDefaultLog( staffDef ) ) {
        vrvStaffDef->ReplaceKeySig( &keySigAttr );
    }
    MeterSigAttr meterSig;
    if ( meterSig.ReadMeterSigDefaultLog( staffDef ) || meterSig.ReadMeterSigDefaultVis( staffDef ) ) {
        vrvStaffDef->ReplaceMeterSig( &meterSig );
    }

    staffGrp->AddStaffDef(vrvStaffDef);
    
    return true;
}

bool MeiInput::ReadMeiMeasure( System *system, pugi::xml_node measure )
{
    Measure *vrvMeasure = new Measure( );
    SetMeiUuid( measure, vrvMeasure );
    
    vrvMeasure->ReadCommon(measure);
    vrvMeasure->ReadMeasureLog(measure);
    
    // here we transfer the @left and @right values to the barLine objects
    vrvMeasure->SetLeftBarlineType( vrvMeasure->GetLeft() );
    vrvMeasure->SetRightBarlineType( vrvMeasure->GetRight() );
    
    system->AddMeasure(vrvMeasure);
    
    pugi::xml_node current;
    for( current = measure.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "staff" ) {
            ReadMeiStaff( vrvMeasure, current);
        }
        else if ( std::string( current.name() ) == "tupletSpan" ) {
            if (!ReadTupletSpanAsTuplet( vrvMeasure, current )) {
                LogWarning( "<tupletSpan> not readable as <tuplet> and ignored" );
            }
        }
        else if ( std::string( current.name() ) == "slur" ) {
            if (!ReadSlurAsSlurAttr( vrvMeasure, current )) {
                LogWarning( "<slur> not readable as @slur and ignored" );
            }
        }
        else {
            LogWarning("Unsupported '<%s>' within <measure>", current.name() );
        }
    }

    return true;
}

bool MeiInput::ReadMeiStaff( Measure *measure, pugi::xml_node staff )
{
    Staff *vrvStaff = new Staff();
    SetMeiUuid(staff, vrvStaff);
    
    vrvStaff->ReadCommon(staff);
    
    if ( staff.attribute( "uly" ) ) {
        vrvStaff->m_yAbs = atoi ( staff.attribute( "uly" ).value() );
    }
    if ( staff.attribute( "label" ) ) {
        // we use type only for typing mensural notation
        vrvStaff->notAnc = true;
    }
    
    measure->AddStaff(vrvStaff);
    
    pugi::xml_node current;
    for( current = staff.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "layer" ) {
            ReadMeiLayer( vrvStaff, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staff>", current.name() );
        }
    }

    return true;
}

bool MeiInput::ReadMeiLayer( Staff *staff, pugi::xml_node layer )
{
    Layer *vrvLayer = new Layer();
    SetMeiUuid(layer, vrvLayer);
    
    vrvLayer->ReadCommon(layer);
    
    staff->AddLayer(vrvLayer);
    
    pugi::xml_node current;
    for( current = layer.first_child( ); current; current = current.next_sibling( ) ) {
        ReadMeiLayerElement( vrvLayer, current );
    }
    
    return true;
}

bool MeiInput::ReadMeiLayerElement( Object *parent, pugi::xml_node xmlElement )
{
    if ( std::string( xmlElement.name() )  == "barLine" ) {
        ReadMeiBarline( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "beam" ) {
        ReadMeiBeam( parent, xmlElement);
    }
    else if ( std::string( xmlElement.name() ) == "clef" ) {
        ReadMeiClef( parent, xmlElement);
    }
    else if ( std::string( xmlElement.name() ) == "mensur" ) {
        ReadMeiMensur( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "meterSig" ) {
        ReadMeiMeterSig( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "note" ) {
        ReadMeiNote( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "rest" ) {
        ReadMeiRest( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "mRest" ) {
        ReadMeiMRest( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "multiRest" ) {
        ReadMeiMultiRest( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "tuplet" ) {
        ReadMeiTuplet( parent, xmlElement );
    }
    // symbols
    else if ( std::string( xmlElement.name() ) == "accid" ) {
        ReadMeiAccid( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "custos" ) {
        ReadMeiCustos( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "dot" ) {
        ReadMeiDot( parent, xmlElement );
    }
    else if ( std::string( xmlElement.name() ) == "chord" ) {
        // We just read the first note for now
        pugi::xml_node note = xmlElement.child("note");
        if ( note ) {
            note.append_attribute( "dur" ) =  xmlElement.attribute("dur").value();
            ReadMeiNote( parent, note );
            LogDebug("Only first note of chords is read" );
        }
    }
    // unknown
    else {
        LogDebug("Element %s ignored", xmlElement.name() );
    }
    
    return true;
}
    
bool MeiInput::ReadLayerElement( pugi::xml_node element, LayerElement *object )
{
    if ( element.attribute( "ulx" ) ) {
        object->m_xAbs = atoi ( element.attribute( "ulx" ).value() );
    }
    ReadSameAsAttr( element, object );
    SetMeiUuid( element, object );
    
    return true;
}

bool MeiInput::ReadMeiAccid( Object *parent, pugi::xml_node accid )
{
    Accid *vrvAccid = new Accid( );
    ReadLayerElement(accid, vrvAccid);
    
    ReadPositionInterface(accid, vrvAccid);
    vrvAccid->ResetAccidental();
    
    AddLayerElement(parent, vrvAccid);
    return true;
}

bool MeiInput::ReadMeiBarline( Object *parent, pugi::xml_node barLine )
{
    Barline *vrvBarline = new Barline();
    ReadLayerElement(barLine, vrvBarline);
    
    vrvBarline->ReadBarLineLog(barLine);
    
    AddLayerElement(parent, vrvBarline);
    
    return true;
    
}

bool MeiInput::ReadMeiBeam( Object *parent, pugi::xml_node beam )
{
    Beam *vrvBeam = new Beam();
    ReadLayerElement(beam, vrvBeam);
    
    AddLayerElement(parent, vrvBeam);
    
    pugi::xml_node current;
    for( current = beam.first_child( ); current; current = current.next_sibling( ) ) {
        ReadMeiLayerElement( vrvBeam, current );
    }
    
    if ( vrvBeam->GetNoteCount() == 1 ) {
        LogWarning("<beam> with only one note");
    }
    
    return true;
}

bool MeiInput::ReadMeiClef( Object *parent, pugi::xml_node clef )
{ 
    Clef *vrvClef = new Clef();
    ReadLayerElement(clef, vrvClef);
    
    vrvClef->ReadClefshape( clef );
    vrvClef->ReadLineloc( clef );
    vrvClef->ReadOctavedisplacement( clef );

    AddLayerElement(parent, vrvClef);
    
    return true;
}

bool MeiInput::ReadMeiCustos( Object *parent, pugi::xml_node custos )
{
    Custos *vrvCustos = new Custos( );
    ReadLayerElement(custos, vrvCustos);
    
    ReadPositionInterface(custos, vrvCustos);
    
    AddLayerElement(parent, vrvCustos);
    
    return true;
}


bool MeiInput::ReadMeiDot( Object *parent, pugi::xml_node dot )
{
    Dot *vrvDot = new Dot( );
    ReadLayerElement(dot, vrvDot);
    
    ReadPositionInterface(dot, vrvDot);
    
    AddLayerElement(parent, vrvDot);
    
    return true;
}

bool MeiInput::ReadMeiMensur( Object *parent, pugi::xml_node mensur )
{
    Mensur *vrvMensur = new Mensur();
    ReadLayerElement(mensur, vrvMensur);
    
    vrvMensur->ReadDurationRatio( mensur );
    vrvMensur->ReadMensurLog( mensur );
    vrvMensur->ReadMensurVis( mensur );
    vrvMensur->ReadSlashcount( mensur );

    AddLayerElement(parent, vrvMensur);
    
    return true;
}
    
bool MeiInput::ReadMeiMeterSig( Object *parent, pugi::xml_node meterSig )
{
    MeterSig *vrvMeterSig = new MeterSig();
    ReadLayerElement(meterSig, vrvMeterSig);
    
    vrvMeterSig->ReadMeterSigLog(meterSig);
    
    AddLayerElement(parent, vrvMeterSig);
    return true;
}
    
bool MeiInput::ReadMeiMRest( Object *parent, pugi::xml_node mRest )
{
    MRest *vrvMRest = new MRest();
    ReadLayerElement(mRest, vrvMRest);
    
    AddLayerElement(parent, vrvMRest);
    return true;
}

bool MeiInput::ReadMeiMultiRest( Object *parent, pugi::xml_node multiRest )
{
	MultiRest *vrvMultiRest = new MultiRest( 1 );
    ReadLayerElement(multiRest, vrvMultiRest);
    
    vrvMultiRest->ReadNumbered(multiRest);
    
    AddLayerElement(parent, vrvMultiRest);
	return true;
}

bool MeiInput::ReadMeiNote( Object *parent, pugi::xml_node note )
{
	Note *vrvNote = new Note();
    ReadLayerElement(note, vrvNote);
    
    ReadDurationInterface(note, vrvNote);
    ReadPitchInterface(note, vrvNote);
    vrvNote->ReadColoration(note);
    vrvNote->ReadNoteLogMensural(note);
    vrvNote->ReadStemmed(note);
    
    // grace
    if ( note.attribute( "grace" ) ) {
		vrvNote->m_cueSize = true; //
	}
    
    AddLayerElement(parent, vrvNote);
    
    // Ties - this should be moved to the Object::PrepareDrawing functor
    if ( note.attribute( "tie" ) ) {
        if ( (strcmp ( note.attribute( "tie" ).value(), "i" ) == 0) || (strcmp ( note.attribute( "tie" ).value(), "m" ) == 0) ) {
            vrvNote->SetTieAttrInitial();
            m_openTies.push_back( vrvNote );
        }
        if ( (strcmp ( note.attribute( "tie" ).value(), "t" ) == 0) || (strcmp ( note.attribute( "tie" ).value(), "m" ) == 0) ) {
            if (!FindOpenTie( vrvNote ) ) {
                LogWarning("Initial @tie not found" );
            }
        }
    }
    
    pugi::xml_node current;
    for( current = note.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "accid" ) {
            //ReadAccid(  vrvElement, current );
        }
        if ( std::string( current.name() ) == "verse" ) {
            ReadVerse(  vrvNote, current );
        }
    }
	
	return true;
}


bool MeiInput::ReadMeiRest( Object *parent, pugi::xml_node rest )
{
    Rest *vrvRest = new Rest();
    ReadLayerElement(rest, vrvRest);
    
    ReadDurationInterface(rest, vrvRest);
    ReadPositionInterface(rest, vrvRest);
	
    AddLayerElement(parent, vrvRest);
    return true;
}


bool MeiInput::ReadMeiTuplet( Object *parent, pugi::xml_node tuplet )
{
    Tuplet *vrvTuplet = new Tuplet();
    ReadLayerElement(tuplet, vrvTuplet);
    
    // Read in the numerator and denominator properties
    vrvTuplet->ReadDurationRatio(tuplet);
    
    AddLayerElement(parent, vrvTuplet);
    
    pugi::xml_node current;
    for( current = tuplet.first_child( ); current; current = current.next_sibling( ) ) {
        ReadMeiLayerElement( vrvTuplet, current );
    }
    
    if ( vrvTuplet->GetNoteCount() == 1 ) {
        LogWarning("<tuplet> with only one note");
    }
    
    return true;
}
    
bool MeiInput::ReadVerse( Note *vrvNote, pugi::xml_node verse)
{
    Verse *vrvVerse = new Verse();
    SetMeiUuid( verse , vrvVerse );
    
    vrvVerse->ReadCommon(verse);
    
    pugi::xml_node current;
    for( current = verse.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "syl" ) {
            ReadSyl( vrvVerse, current );
        }
    }

    vrvNote->AddVerse( vrvVerse );
    return true;
}

bool MeiInput::ReadSyl( Verse *vrvVerse, pugi::xml_node syl)
{
    Syl *vrvSyl = new Syl();
    SetMeiUuid( syl , vrvSyl );
    
    vrvSyl->ReadSylLog( syl );
    ReadText( syl, vrvSyl );
    
    vrvVerse->AddSyl( vrvSyl );
    return true;
}

bool MeiInput::ReadDurationInterface(pugi::xml_node element, DurationInterface *interface)
{
    interface->ReadAugmentdots(element);
    interface->ReadBeamsecondary(element);
    interface->ReadDurationMusical(element);
    interface->ReadDurationPerformed(element);
    interface->ReadDurationRatio(element);
    interface->ReadFermatapresent(element);
    return true;
}

bool MeiInput::ReadPitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    interface->ReadAccidental(element);
    interface->ReadOctave(element);
    interface->ReadPitch(element);
    return true;
}

bool MeiInput::ReadPositionInterface(pugi::xml_node element, PositionInterface *interface)
{
    interface->ReadStafflocPitched(element);
    return true;
}
    
void MeiInput::ReadSameAsAttr( pugi::xml_node element, Object *object )
{
    if ( !element.attribute( "sameas" ) ) {
        return;
    }
    
    object->m_sameAs = element.attribute( "sameas" ).value();
}

void MeiInput::ReadText( pugi::xml_node element, Object *object )
{
    if (element.text()) {
        object->SetText(UTF8to16( element.text().as_string() ));
    }
}
    
void MeiInput::AddScoreDef(Object *parent, ScoreDef *scoreDef)
{
    if (!m_hasScoreDef) {
        m_hasScoreDef = true;
    }
    else if ( dynamic_cast<System*>( parent ) ) {
        dynamic_cast<System*>( parent )->AddScoreDef( scoreDef );
    }
    else {
        LogWarning("'%s' not supported within '%s'", scoreDef->GetClassName().c_str(), parent->GetClassName().c_str() );
        delete scoreDef;
    }    
}
    
void MeiInput::AddStaffGrp(Object *parent, StaffGrp *staffGrp)
{
    if ( dynamic_cast<ScoreDef*>( parent ) ) {
        dynamic_cast<ScoreDef*>( parent )->AddStaffGrp( staffGrp );
    }
    else if ( dynamic_cast<StaffGrp*>( parent ) ) {
        dynamic_cast<StaffGrp*>( parent )->AddStaffGrp( staffGrp );
    }
    else {
        LogWarning("'%s' not supported within '%s'", staffGrp->GetClassName().c_str(), parent->GetClassName().c_str() );
        delete staffGrp;
    }
}

void MeiInput::AddLayerElement( Object *parent, LayerElement *element )
{
    if ( dynamic_cast<Layer*>( parent ) ) {
        dynamic_cast<Layer*>( parent )->AddElement( element );
    }
    else if ( dynamic_cast<Beam*>( parent ) ) {
        dynamic_cast<Beam*>( parent )->AddElement( element );
    }
    else if ( dynamic_cast<Tuplet*>( parent ) ) {
        dynamic_cast<Tuplet*>( parent )->AddElement( element );
    }
    else {
        LogWarning("'%s' not supported within '%s'", element->GetClassName().c_str(), parent->GetClassName().c_str() );
        delete element;
    }
}

bool MeiInput::ReadScoreBasedMei( pugi::xml_node element )
{
    if ( std::string( element.name() ) == "score" ) {
        pugi::xml_node current;
        for( current = element.first_child( ); current; current = current.next_sibling( ) ) {
            ReadScoreBasedMei( current );
        }
    }
    if ( std::string( element.name() ) == "section" ) {
        pugi::xml_node current;
        for( current = element.first_child( ); current; current = current.next_sibling( ) ) {
            ReadScoreBasedMei( current );
        }       
    }
    else if ( std::string( element.name() ) == "measure" ) {
        // This is the call that will put us back on the page-based reading loop
        ReadMeiMeasure( m_system, element );
    }
    /*
    else if ( std::string( element.name() ) == "staff" ) {
        LogDebug( "staff" );
        int n = 1;
        if ( element.attribute( "n" ) ) {
            element.attribute( "n", &n );
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
    else if (std::string( element.name() ) == "pb") {
        if ( (m_system->GetMeasureCount() > 0) && !m_ignoreLayoutInformation) {
            LogDebug( "pb" );
            this->m_hasLayoutInformation = true;
            m_page = new Page( );
            m_system = new System( );
            m_page->AddSystem( m_system );
            m_doc->AddPage( m_page );
        }        
    }
    else if (std::string( element.name() ) == "sb") {
        if ( (m_page->GetSystemCount() > 0)  && !m_ignoreLayoutInformation) {
            LogDebug( "pb" );
            this->m_hasLayoutInformation = true;
            m_system = new System( );
            m_page->AddSystem( m_system );
        }
    }
    else if ( (std::string( element.name() ) == "scoreDef") ) {
        ReadMeiScoreDef( m_system, element );
    }
    else {
        LogWarning( "Elements <%s> ignored", element.name() );
    }
    return true;
}
    
bool MeiInput::ReadSlurAsSlurAttr( Measure *measure, pugi::xml_node slur)
{
    LayerElement *start = NULL;
    LayerElement *end = NULL;
    
	// position (pitch)
	if ( slur.attribute( "startid" ) ) {
        std::string refId = ExtractUuidFragment( slur.attribute( "startid" ).value() );
        start = dynamic_cast<LayerElement*>( measure->FindChildByUuid( refId ) );

        if (!start || !start->IsNote()) {
            LogWarning( "Note with @startid '%s' not found when trying to read the <slur>", refId.c_str() );
        }    
        
	}
	if ( slur.attribute( "endid" ) ) {
        std::string refId = ExtractUuidFragment( slur.attribute( "endid" ).value() );
        end = dynamic_cast<LayerElement*>( measure->FindChildByUuid( refId ) );
        
        if (!end || !end->IsNote()) {
            LogWarning( "Note with @endid '%s' not found when trying to read the <slur>", refId.c_str() );
        }
	}
    Note *startNote = dynamic_cast<Note*>(start);
    Note *endNote = dynamic_cast<Note*>(end);
    
    if (!start || !end || !startNote || !endNote) {
        return false;
    }
    
    startNote->SetSlurAttrInitial();
    endNote->SetSlurAttrTerminal( startNote );
    
    return true;
}
    
bool MeiInput::ReadTupletSpanAsTuplet( Measure *measure, pugi::xml_node tupletSpan)
{    
    Tuplet *tuplet = new Tuplet();
    SetMeiUuid(tupletSpan, tuplet);
    
    LayerElement *start = NULL;
    LayerElement *end = NULL;
    
    // Read in the numerator and denominator properties
    if ( tupletSpan.attribute( "num" ) ) {
        tuplet->SetNum(atoi( tupletSpan.attribute( "num" ).value() ));
    }
    if ( tupletSpan.attribute( "numbase" ) ) {
        tuplet->SetNumbase(atoi( tupletSpan.attribute( "numbase" ).value() ));
    }
    
    // position (pitch)
    if ( tupletSpan.attribute( "startid" ) ) {
        std::string refId = ExtractUuidFragment( tupletSpan.attribute( "startid" ).value() );
        start = dynamic_cast<LayerElement*>( measure->FindChildByUuid( refId ) );
        
        if (!start) {
            LogWarning( "Element with @startid '%s' not found when trying to read the <tupletSpan>", refId.c_str() );
        }
        
    }
    if ( tupletSpan.attribute( "endid" ) ) {
        std::string refId = ExtractUuidFragment( tupletSpan.attribute( "endid" ).value() );
        end = dynamic_cast<LayerElement*>( measure->FindChildByUuid( refId ) );
        
        if (!end) {
            LogWarning( "Element with @endid '%s' not found when trying to read the <tupletSpan>", refId.c_str() );
        }
    }
    if (!start || !end) {
        delete tuplet;
        return false;
    }
    
    LayerElement *startChild =  dynamic_cast<LayerElement*>( start->GetLastParentNot( &typeid(Layer) ) );
    LayerElement *endChild =  dynamic_cast<LayerElement*>( end->GetLastParentNot( &typeid(Layer) ) );
    
    if ( !startChild || !endChild || (startChild->m_parent != endChild->m_parent) ) {
        LogWarning( "Start and end elements for <tupletSpan> '%s' not in the same layer", tuplet->GetUuid().c_str() );
        delete tuplet;
        return false;
    }
    
    Layer *parentLayer = dynamic_cast<Layer*>( startChild->m_parent );
    
    int startIdx = startChild->GetIdx();
    int endIdx = endChild->GetIdx();
    //LogDebug("%d %d %s!", startIdx, endIdx, start->GetUuid().c_str());
    int i;
    for (i = endIdx; i >= startIdx; i--) {
        tuplet->AddElement( dynamic_cast<LayerElement*>( parentLayer->DetachChild(i) ) );
    }
    parentLayer->InsertChild( tuplet, startIdx );

    return true;    
}

bool MeiInput::FindOpenTie( Note *terminalNote )
{
    Layer *currentLayer = dynamic_cast<Layer*>( terminalNote->GetFirstParent( &typeid(Layer) ) );
    Staff *currentStaff = dynamic_cast<Staff*>( terminalNote->GetFirstParent( &typeid(Staff) ) );
    assert( currentLayer );
    assert( currentStaff );
    
    std::vector<Note*>::iterator iter;
    for (iter = m_openTies.begin(); iter != m_openTies.end(); ++iter)
    {
        // we need to get the parent layer and the parent staff for comparing their number
        Layer *parentLayer = dynamic_cast<Layer*>( (*iter)->GetFirstParent( &typeid(Layer) ) );
        Staff *parentStaff = dynamic_cast<Staff*>( (*iter)->GetFirstParent( &typeid(Staff) ) );
        // We assume that if the note has no parent layer or no parent staff it,
        // is because we are in the same staff or layer (e.g., beam) and they have not been added
        //to their parent (staff or layer) yet.
        // If we have one, compare the number
        if ( (parentStaff) && (currentStaff->GetN() != parentStaff->GetN()) ) {
            continue;
        }
        // same layer?
        if ( (parentLayer) && (currentLayer->GetN() != parentLayer->GetN()) ) {
            continue;
        }
        // we only compare oct and pname because alteration is not relevant for ties
        if ( (terminalNote->GetOct() == (*iter)->GetOct()) && (terminalNote->GetPname() == (*iter)->GetPname()) ) {
            terminalNote->SetTieAttrTerminal( *iter );
            m_openTies.erase(iter);
            return true;
        }
        
    }

    return false;
}

void MeiInput::SetMeiUuid( pugi::xml_node element, Object *object )
{
    if ( !element.attribute( "xml:id" ) ) {
        return;
    }
    
    object->SetUuid( element.attribute( "xml:id" ).value() );
}

bool MeiInput::StrToBool(std::string value)
{
    if (value == "false") return false;
	return true;
}

    /*
int MeiInput::StrToOct(std::string oct)
{
	return atoi(oct.c_str());
}
     */
   
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
    
std::string MeiInput::ExtractUuidFragment(std::string refUuid)
{
    unsigned pos = refUuid.find_last_of("#");
    if ( (pos != std::string::npos) && (pos < refUuid.length() - 1) ) {
        refUuid = refUuid.substr( pos + 1 );
    }
    return refUuid;
}

} // namespace vrv

