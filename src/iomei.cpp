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
#include "app.h"
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

        pugi::xml_node music = m_mei.append_child("music");
        pugi::xml_node body = music.append_child("body");
        pugi::xml_node mdiv = body.append_child("mdiv");
        
        
        // element to place the pages
        m_pages = mdiv.append_child("pages");
        m_pages.append_attribute( "type" ) = DocTypeToStr( m_doc->GetType() ).c_str();
        
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

std::string MeiOutput::UuidToMeiStr( Object *element )
{
    std::string out = element->GetUuid();
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    LogDebug("uuid: %s\n", out.c_str());
    return out;
}

bool MeiOutput::WriteDoc( Doc *doc )
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
    
    return true;
}


bool MeiOutput::WritePage( Page *page )
{
    assert( !m_pages.empty() );
    
    m_page = m_pages.append_child("page");
    m_page.append_attribute( "xml:id" ) =  UuidToMeiStr( page ).c_str();
    // size and margins but only if any - we rely on page.height only to check this
    if ( page->m_pageHeight != -1 ) {
        m_page.append_attribute( "page.width" ) = StringFormat( "%d", page->m_pageWidth ).c_str();
        m_page.append_attribute( "page.height" ) = StringFormat( "%d", page->m_pageHeight ).c_str();
        m_page.append_attribute( "page.leftmar" ) = StringFormat( "%d", page->m_pageLeftMar ).c_str();
        m_page.append_attribute( "page.rightmar" ) = StringFormat( "%d", page->m_pageRightMar ).c_str();
        m_page.append_attribute( "page.rightmar" ) = StringFormat( "%d", page->m_pageRightMar ).c_str();
    }
    if ( !page->m_surface.empty() ) {
        m_page.append_attribute( "surface" ) = page->m_surface.c_str();
    }
    //
    m_pages.append_child(pugi::node_comment).set_value( "Coordinates in MEI axis direction" );
    
    return true;
}

bool MeiOutput::WriteSystem( System *system )
{
    assert( !m_page.empty() );
    m_system = m_page.append_child("system");
    m_system.append_attribute( "xml:id" ) =  UuidToMeiStr( system ).c_str();
    // margins
    m_system.append_attribute( "system.leftmar" ) = StringFormat( "%d", system->m_systemLeftMar ).c_str();
    m_system.append_attribute( "system.rightmar") = StringFormat( "%d", system->m_systemRightMar ).c_str();
    // y positions
    m_system.append_attribute( "uly" ) = StringFormat( "%d", system->m_yAbs ).c_str();
    
    return true;
}

bool MeiOutput::WriteScoreDef( ScoreDef *scoreDef )
{
    assert( !m_system.empty() );
    m_scoreDef = m_system.append_child("scoreDef");
    m_scoreDef.append_attribute( "xml:id" ) =  UuidToMeiStr( scoreDef ).c_str();
    if (scoreDef->GetClefAttr()) {
        scoreDef->GetClefAttr()->WriteCleffingLog(m_scoreDef);
    }
    if (scoreDef->GetKeySigAttr()) {
        scoreDef->GetKeySigAttr()->WriteKeySigDefaultLog(m_scoreDef);
    }
    if ( scoreDef->GetMensurAttr() ) {
        dynamic_cast<AttMensuralLog*>(scoreDef)->WriteMensuralLog(m_scoreDef);
    }
    if ( scoreDef->GetMeterSigAttr() ) {
        scoreDef->GetMeterSigAttr()->WriteMeterSigDefaultLog(m_scoreDef);
        scoreDef->GetMeterSigAttr()->WriteMeterSigDefaultVis(m_scoreDef);
    }
    
    // this needs to be fixed
    return true;
    
}

bool MeiOutput::WriteStaffGrp( StaffGrp *staffGrp )
{
    // for now only as part of a system - this needs to be fixed
    assert( !m_system.empty() );
    
    m_staffGrp = m_system.append_child("staffGrp");
    m_staffGrp.append_attribute( "xml:id" ) = UuidToMeiStr( staffGrp ).c_str();
    if ( staffGrp->GetSymbol() != STAFFGRP_NONE ) {
        m_staffGrp.append_attribute( "symbol" ) = StaffGrpSymbolToStr( staffGrp->GetSymbol() ).c_str();
    }
    if ( staffGrp->GetBarthru() ) {
        m_staffGrp.append_attribute( "barthru" ) = BoolToStr( staffGrp->GetBarthru() ).c_str();
    }
    
    return true;
}

bool MeiOutput::WriteStaffDef( StaffDef *staffDef )
{
    assert( !m_staffGrp.empty() );
    
    m_staffDef = m_staffGrp.append_child("staffDef");
    m_staffDef.append_attribute( "xml:id" ) =  UuidToMeiStr( staffDef ).c_str();
    m_staffDef.append_attribute( "n" ) = StringFormat( "%d", staffDef->GetN() ).c_str();
    if (staffDef->GetClefAttr()) {
        staffDef->GetClefAttr()->WriteCleffingLog(m_staffDef);
    }
    if (staffDef->GetKeySigAttr()) {
        staffDef->GetKeySigAttr()->WriteKeySigDefaultLog(m_staffDef);
    }
    if ( staffDef->GetMensurAttr() ) {
        staffDef->GetMensurAttr()->WriteMensuralLog(m_staffDef);
    }
    if ( staffDef->GetMeterSigAttr() ) {
        staffDef->GetMeterSigAttr()->WriteMeterSigDefaultLog(m_staffDef);
        staffDef->GetMeterSigAttr()->WriteMeterSigDefaultVis(m_staffDef);
    }
    
    return true;
}
      
bool MeiOutput::WriteMeasure( Measure *measure )
{
    assert( !m_system.empty() );
    
    m_measure = m_system.append_child("measure");
    m_measure.append_attribute( "xml:id" ) =  UuidToMeiStr( measure ).c_str();

    measure->WriteCommon(m_measure);

    // here we transfer the barLine object values to @left and @right
    measure->SetLeft( measure->GetLeftBarlineType() );
    measure->SetRight( measure->GetRightBarlineType() );
    measure->WriteMeasureLog(m_measure);
    
    return true;
}

bool MeiOutput::WriteStaff( Staff *staff )
{
    assert( !m_measure.empty() );
    
    m_staff = m_measure.append_child("staff");
    m_staff.append_attribute( "xml:id" ) =  UuidToMeiStr( staff ).c_str();
    
    staff->WriteCommon(m_staff);
    
    // y position
    if ( staff->notAnc ) {
        m_staff.append_attribute( "label" ) = "mensural";
    }
    m_staff.append_attribute( "uly" ) = StringFormat( "%d", staff->m_yAbs ).c_str();

    return true;
}


bool MeiOutput::WriteLayer( Layer *layer )
{
    assert( !m_staff.empty() );
    m_layer = m_staff.append_child("layer");
    
    layer->WriteCommon(m_layer);
    
    m_layer.append_attribute( "xml:id" ) =  UuidToMeiStr( layer ).c_str();
    
    return true;
}

bool MeiOutput::WriteLayerElement( LayerElement *element )
{
    assert( !m_layer.empty() );
    
    // Here we look at what is the parent.
    // For example, if we are in a beam, we vrvT attach it to the beam xml element (m_beam)
    // By default, we attach it to m_layer
    pugi::xml_node currentParent = m_layer;
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
    
    pugi::xml_node xmlElement;
    if (dynamic_cast<Accid*>(element)) {
        xmlElement = currentParent.append_child("accid");
        WriteMeiAccid( xmlElement, dynamic_cast<Accid*>(element) );
    }
    else if (dynamic_cast<Barline*>(element)) {
        xmlElement = currentParent.append_child( "barLine" );
        WriteMeiBarline( xmlElement, dynamic_cast<Barline*>(element) );
    }
    else if (dynamic_cast<Beam*>(element)) {
        xmlElement = currentParent.append_child("beam");
        m_beam = xmlElement;
        WriteMeiBeam( xmlElement, dynamic_cast<Beam*>(element) );
    }
    else if (dynamic_cast<Clef*>(element)) {
        xmlElement = currentParent.append_child("clef");
        WriteMeiClef( xmlElement, dynamic_cast<Clef*>(element) );
    }
    else if (dynamic_cast<Custos*>(element)) {
        xmlElement = currentParent.append_child( "custos" );
        WriteMeiCustos( xmlElement, dynamic_cast<Custos*>(element) );
    }
    else if (dynamic_cast<Dot*>(element)) {
        xmlElement = currentParent.append_child( "dot" );
        WriteMeiDot( xmlElement, dynamic_cast<Dot*>(element) );
    }
    else if (dynamic_cast<Mensur*>(element)) {
        xmlElement = currentParent.append_child("mensur");
        WriteMeiMensur( xmlElement, dynamic_cast<Mensur*>(element) );
    }
    else if (dynamic_cast<MRest*>(element)) {
        xmlElement = currentParent.append_child("mRest");
        WriteMeiMRest( xmlElement, dynamic_cast<MRest*>(element) );
    }
    else if (dynamic_cast<MultiRest*>(element)) {
        xmlElement = currentParent.append_child("multiRest");
        WriteMeiMultiRest( xmlElement, dynamic_cast<MultiRest*>(element) );
    }
    else if (dynamic_cast<Note*>(element)) {
        xmlElement = currentParent.append_child("note");
        WriteMeiNote( xmlElement, dynamic_cast<Note*>(element) );
    }
    else if (dynamic_cast<Rest*>(element)) {
        xmlElement = currentParent.append_child("rest");
        WriteMeiRest( xmlElement, dynamic_cast<Rest*>(element) );
    }
    else if (dynamic_cast<Tuplet*>(element)) {
        xmlElement = currentParent.append_child("tuplet");
        m_tuplet = xmlElement;
        WriteMeiTuplet( xmlElement, dynamic_cast<Tuplet*>(element) );
    }
    
    // we have it, set the uuid we read
    if ( !xmlElement.empty() ) {
        this->WriteSameAsAttr( xmlElement, element );
        xmlElement.append_attribute( "xml:id" ) =  UuidToMeiStr( element ).c_str();
        if ( element->m_xAbs != VRV_UNSET) {
            xmlElement.append_attribute( "ulx" ) = StringFormat( "%d", element->m_xAbs ).c_str();
        }
        return true;
    }
    else {
        LogWarning( "Element class %s could not be saved", element->GetClassName().c_str() );
        return false;
    }    
}
    
void MeiOutput::WriteMeiAccid( pugi::xml_node meiAccid, Accid *accid )
{
    WritePositionInterface(meiAccid, accid);
    accid->WriteAccidental(meiAccid);
}

void MeiOutput::WriteMeiBarline( pugi::xml_node meiBarline, Barline *barLine )
{
    barLine->WriteBarLineLog(meiBarline);
    return;
}


void MeiOutput::WriteMeiBeam( pugi::xml_node meiBeam, Beam *beam )
{
    return;
}


void MeiOutput::WriteMeiClef( pugi::xml_node meiClef, Clef *clef )
{
    clef->WriteClefshape(meiClef);
    clef->WriteLineloc(meiClef);
    clef->WriteOctavedisplacement(meiClef);
    return;
}

void MeiOutput::WriteMeiCustos( pugi::xml_node meiCustos, Custos *custos )
{
    WritePositionInterface(meiCustos, custos);
}

void MeiOutput::WriteMeiDot( pugi::xml_node meiDot, Dot *dot )
{
    WritePositionInterface(meiDot, dot);
}

void MeiOutput::WriteMeiMensur( pugi::xml_node meiMensur, Mensur *mensur )
{
    mensur->WriteDurationRatio(meiMensur);
    mensur->WriteMensurLog(meiMensur);
    mensur->WriteMensurVis(meiMensur);
    mensur->WriteSlashcount(meiMensur);
    return;
}

void MeiOutput::WriteMeiMeterSig( pugi::xml_node meiMeterSig, MeterSig *meterSig )
{
    meterSig->WriteMeterSigLog(meiMeterSig);
    return;
}
    
    
void MeiOutput::WriteMeiMRest( pugi::xml_node meiMRest, MRest *mRest )
{
     return;
}

void MeiOutput::WriteMeiMultiRest( pugi::xml_node meiMultiRest, MultiRest *multiRest )
{
    multiRest->WriteNumbered(meiMultiRest);
   return;
}

void MeiOutput::WriteMeiNote( pugi::xml_node meiNote, Note *note )
{
    WriteDurationInterface(meiNote, note);
    WritePitchInterface(meiNote, note);
    note->WriteColoration(meiNote);
    note->WriteNoteLogMensural(meiNote);
    note->WriteStemmed(meiNote);
    
    if ( note->m_cueSize ) {
        meiNote.append_attribute( "grace" ) = "unknown";
    }
    return;
}

void MeiOutput::WriteMeiRest( pugi::xml_node meiRest, Rest *rest )
{
    WriteDurationInterface(meiRest, rest);
    WritePositionInterface(meiRest, rest);
    return;
}

void MeiOutput::WriteMeiTuplet( pugi::xml_node meiTuplet, Tuplet *tuplet )
{
    tuplet->WriteDurationRatio(meiTuplet);
    return;
}
    
/* To be refactored - make a single MeiOutput::Write method that call the appropriate method
void MeiOutput::WriteVerse( Verse *verse, pugi::xml_node currentParent )
{
    pugi::xml_node xmlVerse = currentParent.append_child("verse");
    m_staff.append_attribute( "xml:id" ) =  UuidToMeiStr( staff ).c_str();
    // y position
    if ( staff->notAnc ) {
        m_staff.append_attribute( "label" ) = "mensural";
    }
    m_staff.append_attribute( "uly" ) = StringFormat( "%d", staff->m_yAbs ).c_str();
    m_staff.append_attribute( "n" ) = StringFormat( "%d", staff->GetN() ).c_str();
    
    return;
}

void MeiOutput::WriteSyl( Syl *syl, pugi::xml_node currentParent )
{
    
}
*/

bool MeiOutput::WriteLayerApp( LayerApp *app )
{    
    assert( !m_layer.empty() );
    
    m_app = m_layer.append_child("app");
    
    return true;
}

bool MeiOutput::WriteLayerRdg( LayerRdg *rdg )
{   
    assert( !m_app.empty() );
    
    m_rdgLayer = m_app.append_child("rdg");
    m_rdgLayer.append_attribute( "source" ) = rdg->m_source.c_str();

    return true;
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

void MeiOutput::WriteSameAsAttr( pugi::xml_node element, Object *object )
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
            ReadUnsupported( m_system, current );
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
    // chord - temporary, read only the first note
    else if ( std::string( xmlElement.name() ) == "chord" ) {
        LogDebug("Only first note of chords is read" );
        //ReadUnsupported(xmlElement);
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
    // tie
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
    
    AddLayerElement(parent, vrvNote);
    
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
        /*
        if ( dynamic_cast<Layer*>( parent ) ) {
            dynamic_cast<Layer*>( parent )->AddElement( element );
        }
        else if ( dynamic_cast<Beam*>( parent ) ) {
            dynamic_cast<Beam*>( parent )->AddElement( element );
        }
        else if ( dynamic_cast<Tuplet*>( parent ) ) {
            dynamic_cast<Tuplet*>( parent )->AddElement( element );
        }
        */
        else {
            LogWarning("'%s' not supported within '%s'", scoreDef->GetClassName().c_str(), parent->GetClassName().c_str() );
            delete scoreDef;
        }
        
    }
    
    void MeiInput::AddStaffGrp(Object *parent, StaffGrp *staffGrp)
    {
        /*
         if ( dynamic_cast<Layer*>( parent ) ) {
         dynamic_cast<Layer*>( parent )->AddElement( element );
         }
         else if ( dynamic_cast<Beam*>( parent ) ) {
         dynamic_cast<Beam*>( parent )->AddElement( element );
         }
         else if ( dynamic_cast<Tuplet*>( parent ) ) {
         dynamic_cast<Tuplet*>( parent )->AddElement( element );
         }
         */
        {
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

bool MeiInput::ReadUnsupported( Object *parent, pugi::xml_node element )
{
    if ( std::string( element.name() ) == "score" ) {
        pugi::xml_node current;
        for( current = element.first_child( ); current; current = current.next_sibling( ) ) {
            ReadUnsupported( parent, current );
        }
    }
    if ( std::string( element.name() ) == "section" ) {
        pugi::xml_node current;
        for( current = element.first_child( ); current; current = current.next_sibling( ) ) {
            ReadUnsupported( parent, current );
        }       
    }
    else if ( std::string( element.name() ) == "measure" ) {
        if (parent != m_system) {
            LogDebug( "<measure> supported only as <section> child" );
        }
        else {
            ReadMeiMeasure( m_system, element );
        }
    }
    else if ( std::string( element.name() ) == "chord" ) {
        // We just read the first note for now
        pugi::xml_node note = element.child("note");
        note.append_attribute( "dur" ) =  element.attribute("dur").value();
        //if (ReadMeiLayerElement( note )) {
        //    LogMessage( "<note> in chord read!" );
        //}
    }
    else if ( std::string( element.name() ) == "tupletSpan" ) {
        if (!ReadTupletSpanAsTuplet( element )) {
            LogWarning( "<tupletSpan> not readable as <tuplet> and ignored" );
        }
    }
    else if ( std::string( element.name() ) == "slur" ) {
        if (!ReadSlurAsSlurAttr( element )) {
            LogWarning( "<slur> not readable as @slur and ignored" );
        }
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
        if (parent != m_system) {
            LogDebug( "<pb> supported only between <measure> elements" );
        }
        else if ( (m_system->GetMeasureCount() > 0) && !m_ignoreLayoutInformation) {
            LogDebug( "pb" );
            this->m_hasLayoutInformation = true;
            m_page = new Page( );
            m_system = new System( );
            m_page->AddSystem( m_system );
            m_doc->AddPage( m_page );
        }        
    }
    else if (std::string( element.name() ) == "sb") {
        if (parent != m_system) {
            LogDebug( "<sb> supported only between <measure> elements" );
        }
        if ( (m_page->GetSystemCount() > 0)  && !m_ignoreLayoutInformation) {
            LogDebug( "pb" );
            this->m_hasLayoutInformation = true;
            m_system = new System( );
            m_page->AddSystem( m_system );
        }
    }
    else if ( (std::string( element.name() ) == "scoreDef") ) {
        ReadMeiScoreDef( parent, element );
    }
    else {
        LogWarning( "Elements <%s> ignored", element.name() );
    }
    return true;
}
    
bool MeiInput::ReadSlurAsSlurAttr(pugi::xml_node slur)
{
    /*
    assert( m_measure );
    
    LayerElement *start = NULL;
    LayerElement *end = NULL;
    
	// position (pitch)
	if ( slur.attribute( "startid" ) ) {
        std::string refId = ExtractUuidFragment( slur.attribute( "startid" ).value() );
        start = dynamic_cast<LayerElement*>( m_measure->FindChildByUuid( refId ) );

        if (!start || !start->IsNote()) {
            LogWarning( "Note with @startid '%s' not found when trying to read the <slur>", refId.c_str() );
        }    
        
	}
	if ( slur.attribute( "endid" ) ) {
        std::string refId = ExtractUuidFragment( slur.attribute( "endid" ).value() );
        end = dynamic_cast<LayerElement*>( m_measure->FindChildByUuid( refId ) );
        
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
    */
    return true;
     

}
    
bool MeiInput::ReadTupletSpanAsTuplet(pugi::xml_node tupletSpan)
{
    /*
    assert( m_measure );
    
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
        start = dynamic_cast<LayerElement*>( m_measure->FindChildByUuid( refId ) );
        
        if (!start) {
            LogWarning( "Element with @startid '%s' not found when trying to read the <tupletSpan>", refId.c_str() );
        }
        
    }
    if ( tupletSpan.attribute( "endid" ) ) {
        std::string refId = ExtractUuidFragment( tupletSpan.attribute( "endid" ).value() );
        end = dynamic_cast<LayerElement*>( m_measure->FindChildByUuid( refId ) );
        
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
    LogDebug("%d %d %s!", startIdx, endIdx, start->GetUuid().c_str());
    int i;
    for (i = endIdx; i >= startIdx; i--) {
        tuplet->AddElement( dynamic_cast<LayerElement*>( parentLayer->DetachChild(i) ) );
    }
    parentLayer->InsertChild( tuplet, startIdx );
    */
    return true;
    
}

bool MeiInput::FindOpenTie( Note *terminalNote )
{
    return true;
    
    /*
    assert( m_staff );
    assert( m_layer );
    
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
        if ( (parentStaff) && (m_staff->GetN() != parentStaff->GetN()) ) {
            continue;
        }
        // same layer?
        if ( (parentLayer) && (m_layer->GetN() != parentLayer->GetN()) ) {
            continue;
        }
        // we only compare oct and pname because alteration is not relevant for ties
        if ( (terminalNote->GetOct() == (*iter)->GetOct()) && (terminalNote->GetPname() == (*iter)->GetPname()) ) {
            terminalNote->SetTieAttrTerminal( *iter );
            m_openTies.erase(iter);
            return true;
        }
        
    }
    */
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

