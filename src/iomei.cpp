/////////////////////////////////////////////////////////////////////////////
// Name:        iomei.cpp
// Author:      Laurent Pugin
// Created:     2008
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iomei.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "accid.h"
#include "beam.h"
#include "chord.h"
#include "custos.h"
#include "dot.h"
#include "editorial.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "page.h"
#include "rest.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tie.h"
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
    m_writeToStreamString = false;
    m_page = -1;
    m_scoreBasedMEI = false;
}

MeiOutput::~MeiOutput()
{
}

bool MeiOutput::ExportFile( )
{
    try {
        pugi::xml_document meiDoc;
        
        if ( m_page < 0 ) {
            m_mei = meiDoc.append_child("mei");
            m_mei.append_attribute( "xmlns" ) = "http://www.music-encoding.org/ns/mei";
            m_mei.append_attribute( "meiversion" ) = "2013";
            
            // this starts the call of all the functors
            m_doc->Save( this );
        }
        else {
            if (m_page >= m_doc->GetPageCount()) {
                LogError("Page %d does not exist", m_page );
                return false;
            }
            Page *page = dynamic_cast<Page*>(m_doc->m_children[m_page]);
            assert( page );
            if (m_scoreBasedMEI) {
                m_currentNode = meiDoc.append_child("score");
                m_currentNode = m_currentNode.append_child("section");
                m_nodeStack.push_back(m_currentNode);
                // First save the main scoreDef
                m_doc->m_scoreDef.Save( this );
            } else {
                m_currentNode = meiDoc.append_child("pages");
            }
            
            page->Save( this );
        }
        if ( m_writeToStreamString ) {
            meiDoc.save(m_streamStringOutput, "    ");
        }
        else {
            meiDoc.save_file( m_filename.c_str(), "    " );
        }
    }
    catch( char * str ) {
        LogError("%s", str );
        return false;
    }
	return true;    
}
    
std::string MeiOutput::GetOutput( int page )
{
    m_writeToStreamString = true;
    m_page = page;
    this->ExportFile();
    m_writeToStreamString = false;
    m_page = -1;
    return m_streamStringOutput.str();
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
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("page");
            WriteMeiPage( m_currentNode, dynamic_cast<Page*>(object) );
        }
        // Here we could add a <pb> element
    }
    else if (dynamic_cast<System*>(object)) {
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("system");
            WriteMeiSystem( m_currentNode, dynamic_cast<System*>(object) );
        }
        // Here we could add a <sb> element (but not for the first system of the page?)
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
    
    // Measure elements
    else if (dynamic_cast<Slur*>(object)) {
        m_currentNode = m_currentNode.append_child("slur");
        WriteMeiSlur( m_currentNode, dynamic_cast<Slur*>(object) );
    }
    else if (dynamic_cast<Tie*>(object)) {
        m_currentNode = m_currentNode.append_child("tie");
        WriteMeiTie( m_currentNode, dynamic_cast<Tie*>(object) );
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
    else if (dynamic_cast<Chord*>(object)) {
        m_currentNode = m_currentNode.append_child( "chord" );
        WriteMeiChord( m_currentNode, dynamic_cast<Chord*>(object) );
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
    else if (dynamic_cast<KeySig*>(object)) {
        m_currentNode = m_currentNode.append_child("keySig");
        WriteMeiKeySig( m_currentNode, dynamic_cast<KeySig*>(object) );
    }
    else if (dynamic_cast<Mensur*>(object)) {
        m_currentNode = m_currentNode.append_child("mensur");
        WriteMeiMensur( m_currentNode, dynamic_cast<Mensur*>(object) );
    }
    else if (dynamic_cast<MeterSig*>(object)) {
        m_currentNode = m_currentNode.append_child("meterSig");
        WriteMeiMeterSig( m_currentNode, dynamic_cast<MeterSig*>(object) );
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
    else if (dynamic_cast<Space*>(object)) {
        m_currentNode = m_currentNode.append_child("space");
        WriteMeiSpace( m_currentNode, dynamic_cast<Space*>(object) );
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
    
    // Editorial markup
    else if (dynamic_cast<App*>(object)) {
        m_currentNode = m_currentNode.append_child("app");
        WriteMeiApp( m_currentNode, dynamic_cast<App*>(object) );
    }
    else if (dynamic_cast<Lem*>(object)) {
        m_currentNode = m_currentNode.append_child("lem");
        WriteMeiLem( m_currentNode, dynamic_cast<Lem*>(object) );
    }
    else if (dynamic_cast<Rdg*>(object)) {
        m_currentNode = m_currentNode.append_child("rdg");
        WriteMeiRdg( m_currentNode, dynamic_cast<Rdg*>(object) );
    }
    else if (dynamic_cast<Annot*>(object)) {
        m_currentNode = m_currentNode.append_child("annot");
        WriteMeiAnnot( m_currentNode, dynamic_cast<Annot*>(object) );
    }
    
    else {
        // Missing output method for the class
        LogError("Output method missing for '%s'", object->GetClassName().c_str() );
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
    
    for (pugi::xml_node child = m_doc->m_header.first_child(); child; child = child.next_sibling())
    {
        meiHead.append_copy(child);
    }
    
    /*
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
    */
    
    // ---- music ----
   
    pugi::xml_node music = m_mei.append_child("music");
    pugi::xml_node body = music.append_child("body");
    pugi::xml_node mdiv = body.append_child("mdiv");
    
    if (m_scoreBasedMEI) {
        m_currentNode = mdiv.append_child("score");
        m_currentNode = m_currentNode.append_child("section");
        m_nodeStack.push_back(m_currentNode);
        // First save the main scoreDef
        m_doc->m_scoreDef.Save( this );
    } else {
        // element to place the pages
        m_currentNode = mdiv.append_child("pages");
        m_currentNode.append_attribute( "type" ) = DocTypeToStr( m_doc->GetType() ).c_str();
        m_currentNode.append_child(pugi::node_comment).set_value( "Coordinates in MEI axis direction" );
    }
    
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
    if ( system->m_yAbs != VRV_UNSET) {
        currentNode.append_attribute( "uly" ) = StringFormat( "%d", system->m_yAbs ).c_str();
    }
    
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
        scoreDef->GetMensurAttr()->WriteMensuralLog(currentNode);
        scoreDef->GetMensurAttr()->WriteMensuralShared(currentNode);
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
    
    staffDef->WriteCommon(currentNode);
    staffDef->WriteLabelsAddl(currentNode);
    staffDef->WriteStaffDefVis(currentNode);
    
    if (staffDef->GetClefAttr()) {
        staffDef->GetClefAttr()->WriteCleffingLog(currentNode);
    }
    if (staffDef->GetKeySigAttr()) {
        staffDef->GetKeySigAttr()->WriteKeySigDefaultLog(currentNode);
    }
    if ( staffDef->GetMensurAttr() ) {
        staffDef->GetMensurAttr()->WriteMensuralLog(currentNode);
        staffDef->GetMensurAttr()->WriteMensuralShared(currentNode);
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

void MeiOutput::WriteMeiTie( pugi::xml_node currentNode, Tie *tie )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( tie ).c_str();
    
    WriteTimeSpanningInterface(currentNode, tie);
    
    return;
};

void MeiOutput::WriteMeiSlur( pugi::xml_node currentNode, Slur *slur )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( slur ).c_str();
    
    WriteTimeSpanningInterface(currentNode, slur);
    
    return;
};

bool MeiOutput::WriteMeiStaff( pugi::xml_node currentNode, Staff *staff )
{
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( staff ).c_str();
    
    staff->WriteCommon(currentNode);
    
    // y position
    if ( staff->notAnc ) {
        currentNode.append_attribute( "label" ) = "mensural";
    }
    if ( staff->m_yAbs != VRV_UNSET) {
        currentNode.append_attribute( "uly" ) = StringFormat( "%d", staff->m_yAbs ).c_str();
    }

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

void MeiOutput::WriteMeiChord( pugi::xml_node currentNode, Chord *chord )
{
    WriteLayerElement( currentNode, chord );
    WriteDurationInterface( currentNode, chord);
    chord->WriteCommon(currentNode);
    chord->WriteStemmed(currentNode);
    chord->WriteTiepresent(currentNode);
    
    if ( chord->m_cueSize ) {
        currentNode.append_attribute( "grace" ) = "unknown";
    }
    
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
    
void MeiOutput::WriteMeiKeySig( pugi::xml_node currentNode, KeySig *keySig )
{
    WriteLayerElement( currentNode, keySig );
    keySig->WriteAccidental(currentNode);
    keySig->WritePitch(currentNode);
    return;
}

void MeiOutput::WriteMeiMensur( pugi::xml_node currentNode, Mensur *mensur )
{
    WriteLayerElement( currentNode, mensur );
    mensur->WriteDurationRatio(currentNode);
    mensur->WriteMensuralShared(currentNode);
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
    WriteLayerElement( currentNode, mRest );
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
    note->WriteTiepresent(currentNode);
    
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
    
void MeiOutput::WriteMeiSpace( pugi::xml_node currentNode, Space *space )
{
    WriteLayerElement( currentNode, space );
    WriteDurationInterface(currentNode, space);
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
    syl->WriteTypography( currentNode );
    syl->WriteSylLog( currentNode );
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
    interface->WriteStaffident(element);
}
    
void MeiOutput::WritePitchInterface(pugi::xml_node element, vrv::PitchInterface *interface)
{
    interface->WriteAccidental(element);
    interface->WriteOctave(element);
    interface->WritePitch(element);
}
    
void MeiOutput::WritePositionInterface(pugi::xml_node element, vrv::PositionInterface *interface)
{
    interface->WriteStafflocPitched(element);
}
    
void MeiOutput::WriteTimeSpanningInterface(pugi::xml_node element, vrv::TimeSpanningInterface *interface)
{
    interface->WriteStartendid(element);
    interface->WriteStartid(element);
}

void MeiOutput::WriteSameAsAttr(pugi::xml_node element, Object *object)
{
    if ( !object->m_sameAs.empty() ) {
        element.append_attribute( "sameas" ) = object->m_sameAs.c_str();
    }
}

void MeiOutput::WriteUnsupportedAttr(pugi::xml_node element, vrv::Object *object)
{
    ArrayOfStrAttr::iterator iter;
    for (iter = object->m_unsupported.begin(); iter != object->m_unsupported.end(); iter++) {
        element.append_attribute( (*iter).first.c_str() ) = (*iter).second.c_str();
    }
}
    
    
void MeiOutput::WriteText( pugi::xml_node element, Object *object )
{
    if ( !object->GetText().empty() ) {
        element.text() =  UTF16to8(object->GetText().c_str() ).c_str();
    }
}
    
void MeiOutput::WriteEditorialElement( pugi::xml_node currentNode, EditorialElement *element )
{
    this->WriteSameAsAttr( currentNode, element );    
    currentNode.append_attribute( "xml:id" ) =  UuidToMeiStr( element ).c_str();
    element->WriteCommon( currentNode );
}

bool MeiOutput::WriteMeiApp( pugi::xml_node currentNode, App *app )
{
    WriteEditorialElement(currentNode, app);
    return true;
};

bool MeiOutput::WriteMeiLem( pugi::xml_node currentNode, Lem *lem )
{
    WriteEditorialElement(currentNode, lem);
    return true;
};

bool MeiOutput::WriteMeiRdg( pugi::xml_node currentNode, Rdg *rdg )
{
    WriteEditorialElement(currentNode, rdg);
    return true;
};

bool MeiOutput::WriteMeiAnnot( pugi::xml_node currentNode, Annot *annot )
{
    WriteEditorialElement(currentNode, annot);
    annot->WritePlist(currentNode);
    annot->WriteSource(currentNode);
    
    for (pugi::xml_node child = annot->m_content.first_child(); child; child = child.next_sibling())
    {
        currentNode.append_copy(child);
    }
    
    return true;
};
    
std::string MeiOutput::BoolToStr(bool value)
{
    if (value) return "true";
    return "false";
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
    
bool MeiInput::IsAllowed(std::string element, vrv::Object *filterParent)
{
    if (!filterParent) {
        return true;
    }
    
    const std::type_info *elementType = &typeid(*filterParent);
    if ( element == "app" ) {
        return true;
    }
    if ( element == "annot" ) {
        return true;
    }
    else if (*elementType  == typeid(Note))
    {
        //if ( element == "accid" ) return true;
        //else 
        if ( element == "syl" ) return true;
        else if ( element == "verse" ) return true;
        else return false;
    }
    else if (*elementType  == typeid(Verse))
    {
        if ( element == "syl" ) return true;
        else return false;
    }
    else
    {
        LogDebug("Unknow filter for '%s'", filterParent->GetClassName().c_str());
        return true;
    }
}



bool MeiInput::ReadMei( pugi::xml_node root )
{
    pugi::xml_node current;
    bool success = true;
    
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
                if (!success) break;
                success = ReadMeiPage( current );
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
            if (!success) break;
            success = ReadScoreBasedMei( current );
        }
    }
    
    return success;
}

bool MeiInput::ReadMeiHeader( pugi::xml_node meiHead )
{
    m_doc->m_header.reset();
    //copy all the nodes inside into the master document
    for (pugi::xml_node child = meiHead.first_child(); child; child = child.next_sibling())
    {
        m_doc->m_header.append_copy(child);
    }
    
    return true;
}


bool MeiInput::ReadMeiPage( pugi::xml_node page )
{
    Page *vrvPage = new Page();
    SetMeiUuid( page, vrvPage );
    
    if ( page.attribute( "page.height" ) ) {
        vrvPage->m_pageHeight = atoi ( page.attribute( "page.height" ).value() );
    }
    if ( page.attribute( "page.width" ) ) {
        vrvPage->m_pageWidth = atoi ( page.attribute( "page.width" ).value() );
    }
    if ( page.attribute( "page.leftmar" ) ) {
        vrvPage->m_pageLeftMar = atoi ( page.attribute( "page.leftmar" ).value() );
    }
    if ( page.attribute( "page.rightmar" ) ) {
        vrvPage->m_pageRightMar = atoi ( page.attribute( "page.rightmar" ).value() );
    }
    if ( page.attribute( "page.topmar" ) ) {
        vrvPage->m_pageTopMar = atoi ( page.attribute( "page.topmar" ).value() );
    }
    if ( page.attribute( "surface" ) ) {
        vrvPage->m_surface = page.attribute( "surface" ).value();
    }

    m_doc->AddPage( vrvPage );
    
    return ReadMeiPageChildren(vrvPage, page);
}
    
bool MeiInput::ReadMeiPageChildren( Object *parent, pugi::xml_node parentNode )
{
    // If we allow <app> between <page> elements
    //assert( dynamic_cast<Page*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    assert( dynamic_cast<Page*>( parent ) );
    
    pugi::xml_node current;
    for( current = parentNode.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "system" ) {
            ReadMeiSystem( parent, current );
        }
        // can we have scoreDef between system in the current page-based cusotmization? To be checked
        else if ( std::string( current.name() ) == "scoreDef" ) {
            ReadMeiScoreDef( parent, current );
        }
        // can we have scoreDef between system in the current page-based cusotmization?
        // To be checked or defined - we would need to add an EDITORIAL_PAGE EditorialLevel
        /*
         else if ( std::string( current.name() ) == "app" ) {
         ReadMeiApp( vrvPage, current, EDITORIAL_PAGE );
         }
         */
        else {
            LogWarning("Unsupported '<%s>' within <page>", current.name() );
        }
    }
    
    return true;
}

bool MeiInput::ReadMeiSystem( Object *parent, pugi::xml_node system )
{
    // If we allow <app> between <page> elements
    //assert( dynamic_cast<Page*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    assert( dynamic_cast<Page*>( parent ) );
    
    System *vrvSystem = new System();
    SetMeiUuid(system, vrvSystem);
    
    if ( system.attribute( "system.leftmar") ) {
        vrvSystem->m_systemLeftMar = atoi ( system.attribute( "system.leftmar" ).value() );
    }
    if ( system.attribute( "system.rightmar" ) ) {
        vrvSystem->m_systemRightMar = atoi ( system.attribute( "system.rightmar" ).value() );
    }
    if ( system.attribute( "uly" ) ) {
        vrvSystem->m_yAbs = atoi ( system.attribute( "uly" ).value() ) * DEFINITON_FACTOR;
    }
    
    // This could me moved to an AddSystem method for consistency with AddLayerElement
    if ( dynamic_cast<Page*>( parent ) ) {
        dynamic_cast<Page*>( parent )->AddSystem( vrvSystem );
    }
    
    ReadMeiSystemChildren(vrvSystem, system);
    
    return true;
}
    
bool MeiInput::ReadMeiSystemChildren( Object *parent, pugi::xml_node parentNode )
{
    assert( dynamic_cast<System*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    
    bool success = true;
    pugi::xml_node current;
    Measure *unmeasured = NULL;
    for( current = parentNode.first_child( ); current; current = current.next_sibling( ) ) {
        if (!success) break;
        if ( std::string( current.name() ) == "scoreDef" ) {
            // we should not have scoredef with unmeasured within a system... (?)
            assert(!unmeasured);
            ReadMeiScoreDef( parent, current );
        }
        // unmeasured music
        else if ( parentNode.child( "staff" ) ) {
            if (!unmeasured) {
                if ( dynamic_cast<System*>( parent ) ) {
                    unmeasured = new Measure( false );
                    dynamic_cast<System*>( parent )->AddMeasure(unmeasured);
                }
                else {
                    LogError( "Unmeasured music within editorial markup is currently not supported" );
                    return false;
                }
            }
            success = ReadMeiStaff( unmeasured, current );
        }
        else if ( parentNode.child( "measure" ) ) {
            // we should not mix measured and unmeasured within a system...
            assert(!unmeasured);
            if ( dynamic_cast<EditorialElement*>( parent ) ) {
                if ( !m_ignoreLayoutInformation ) {
                    LogError( "Cannot have <measure> within editorial markup unless layout information \
                             is ignored and continous layout is chosen (try with --no-layout)" );
                    return false;
                }
                else {
                    m_hasMeasureWithinEditMarkup = true;
                }
                
            }
            success = ReadMeiMeasure(parent, current);
        }
        else if ( std::string( current.name() ) == "app" ) {
            success = ReadMeiApp( parent, current, EDITORIAL_SYSTEM );
        }
        else if ( std::string( current.name() ) == "annot" ) {
            success = ReadMeiAnnot( parent, current );
        }
        else {
            LogWarning("Unsupported '<%s>' within <system>", current.name() );
        }
    }
    
    return success;
}

bool MeiInput::ReadMeiScoreDef( Object *parent, pugi::xml_node scoreDef )
{
    assert( dynamic_cast<System*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    
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
        meterSig.ReadMeterSigDefaultVis( scoreDef );  // not great, but we need to do it in case we have both and the first one succeeded
        vrvScoreDef->ReplaceMeterSig( &meterSig );
    }
    MensurAttr mensur;
    if ( mensur.ReadMensuralLog( scoreDef ) || mensur.ReadMensuralShared( scoreDef ) ) {
        mensur.ReadMensuralShared( scoreDef ); // not great, but we need to do it in case we have both and the first one succeeded
        vrvScoreDef->ReplaceMensur( &mensur );
    }
    
    AddScoreDef(parent, vrvScoreDef);
    
    return ReadMeiScoreDefChildren( vrvScoreDef, scoreDef );
}
    
bool MeiInput::ReadMeiScoreDefChildren( Object *parent, pugi::xml_node parentNode )
{
    assert( dynamic_cast<ScoreDef*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    
    bool success = true;
    pugi::xml_node current;
    for( current = parentNode.first_child( ); current; current = current.next_sibling( ) ) {
        if (!success) break;
        if ( std::string( current.name() ) == "app" ) {
            success = ReadMeiApp( parent, current, EDITORIAL_SCOREDEF);
        }
        if ( std::string( current.name() ) == "annot" ) {
            success = ReadMeiAnnot( parent, current );
        }
        else if ( std::string( current.name() ) == "staffGrp" ) {
            success = ReadMeiStaffGrp( parent, current );
        }
        else {
            LogWarning("Unsupported '<%s>' within <scoreDef>", current.name() );
        }
    }
    
    return success;
}

bool MeiInput::ReadMeiStaffGrp( Object *parent, pugi::xml_node staffGrp )
{
    assert( dynamic_cast<ScoreDef*>( parent ) || dynamic_cast<StaffGrp*>( parent ) );
    
    StaffGrp *vrvStaffGrp = new StaffGrp( );
    SetMeiUuid( staffGrp, vrvStaffGrp );
    
    if ( staffGrp.attribute( "symbol" ) ) {
        vrvStaffGrp->SetSymbol( StrToStaffGrpSymbol( staffGrp.attribute( "symbol" ).value() ) );
    }
    if ( staffGrp.attribute( "barthru" ) ) {
        vrvStaffGrp->SetBarthru( StrToBool( staffGrp.attribute( "barthru" ).value() ) );
    }
    
    AddStaffGrp(parent, vrvStaffGrp);
    
    return ReadMeiStaffGrpChildren( vrvStaffGrp, staffGrp );
}    

bool MeiInput::ReadMeiStaffGrpChildren( Object *parent, pugi::xml_node parentNode )
{
    assert( dynamic_cast<StaffGrp*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    
    bool success = true;
    pugi::xml_node current;
    for( current = parentNode.first_child( ); current; current = current.next_sibling( ) ) {
        if (!success) break;
        if ( std::string( current.name() ) == "app" ) {
            success = ReadMeiApp( parent, current, EDITORIAL_STAFFGRP );
        }
        if ( std::string( current.name() ) == "annot" ) {
            success = ReadMeiAnnot( parent, current );
        }
        else if ( std::string( current.name() ) == "staffGrp" ) {
            success = ReadMeiStaffGrp( parent, current);
        }
        else if ( std::string( current.name() ) == "staffDef" ) {
            success = ReadMeiStaffDef( parent, current );
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffGrp>", current.name() );
        }
    }
    
    return success;
}

bool MeiInput::ReadMeiStaffDef( Object *parent, pugi::xml_node staffDef )
{
    assert( dynamic_cast<StaffGrp*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    
    StaffDef *vrvStaffDef = new StaffDef( );
    SetMeiUuid( staffDef, vrvStaffDef );
    
    vrvStaffDef->ReadCommon(staffDef);
    vrvStaffDef->ReadLabelsAddl(staffDef);
    vrvStaffDef->ReadStaffDefVis(staffDef);
    
    if ( !vrvStaffDef->HasN() ) {
        LogWarning("No @n on <staffDef> might yield unpredictable results");
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
        meterSig.ReadMeterSigDefaultVis( staffDef );  // not great, but we need to do it in case we have both and the first one succeeded
        vrvStaffDef->ReplaceMeterSig( &meterSig );
    }
    MensurAttr mensur;
    if ( mensur.ReadMensuralLog( staffDef ) || mensur.ReadMensuralShared( staffDef ) ) {
        mensur.ReadMensuralShared( staffDef ); // not great, but we need to do it in case we have both and the first one succeeded
        vrvStaffDef->ReplaceMensur( &mensur );
    }
    
    // This could me moved to an AddMeasure method for consistency with AddLayerElement
    if ( dynamic_cast<StaffGrp*>( parent ) ) {
        dynamic_cast<StaffGrp*>( parent )->AddStaffDef(vrvStaffDef);
    }
    else if ( dynamic_cast<EditorialElement*>( parent ) ) {
        dynamic_cast<EditorialElement*>( parent )->AddStaffDef(vrvStaffDef);
    }
    
    return true;
}

bool MeiInput::ReadMeiMeasure( Object *parent, pugi::xml_node measure )
{
    Measure *vrvMeasure = new Measure( );
    SetMeiUuid( measure, vrvMeasure );
    
    vrvMeasure->ReadCommon(measure);
    vrvMeasure->ReadMeasureLog(measure);
    
    // here we transfer the @left and @right values to the barLine objects
    vrvMeasure->SetLeftBarlineType( vrvMeasure->GetLeft() );
    vrvMeasure->SetRightBarlineType( vrvMeasure->GetRight() );
    
    // This could me moved to an AddMeasure method for consistency with AddLayerElement
    if ( dynamic_cast<System*>( parent ) ) {
        dynamic_cast<System*>( parent )->AddMeasure(vrvMeasure);
    }
    else if ( dynamic_cast<EditorialElement*>( parent ) ) {
        dynamic_cast<EditorialElement*>( parent )->AddMeasure(vrvMeasure);
    }
    
    return ReadMeiMeasureChildren(vrvMeasure, measure);
}

bool MeiInput::ReadMeiMeasureChildren( Object *parent, pugi::xml_node parentNode )
{
    assert( dynamic_cast<Measure*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    
    bool success = true;
    pugi::xml_node current;
    for( current = parentNode.first_child( ); current; current = current.next_sibling( ) ) {
        if (!success) break;
        if ( std::string( current.name() ) == "app" ) {
            success = ReadMeiApp( parent, current, EDITORIAL_MEASURE);
        }
        if ( std::string( current.name() ) == "annot" ) {
            success = ReadMeiAnnot( parent, current );
        }
        else if ( std::string( current.name() ) == "staff" ) {
            success = ReadMeiStaff( parent, current );
        }
        else if ( std::string( current.name() ) == "tupletSpan" ) {
            if (!ReadTupletSpanAsTuplet( dynamic_cast<Measure*>( parent ), current )) {
                LogWarning( "<tupletSpan> not readable as <tuplet> and ignored" );
            }
        }
        else if ( std::string( current.name() ) == "tie" ) {
            success = ReadMeiTie( parent, current );
        }
        else if ( std::string( current.name() ) == "slur" ) {
            success = ReadMeiSlur( parent, current );
        }
        else {
            LogWarning("Unsupported '<%s>' within <measure>", current.name() );
        }
    }
    
    return success;
}
    
bool MeiInput::ReadMeiTie( Object *parent, pugi::xml_node tie )
{
    Tie *vrvTie = new Tie();
    SetMeiUuid(tie, vrvTie);
    
    ReadTimeSpanningInterface(tie, vrvTie);
    
    AddMeasureElement(parent, vrvTie);
    
    return true;
}

bool MeiInput::ReadMeiSlur( Object *parent, pugi::xml_node slur )
{
    Slur *vrvSlur = new Slur();
    SetMeiUuid(slur, vrvSlur);
    
    ReadTimeSpanningInterface(slur, vrvSlur);
    
    AddMeasureElement(parent, vrvSlur);
    
    return true;
}

bool MeiInput::ReadMeiStaff( Object *parent, pugi::xml_node staff )
{
    Staff *vrvStaff = new Staff();
    SetMeiUuid(staff, vrvStaff);
    
    vrvStaff->ReadCommon(staff);
    
    if ( staff.attribute( "uly" ) ) {
        vrvStaff->m_yAbs = atoi ( staff.attribute( "uly" ).value() ) * DEFINITON_FACTOR;
    }
    if ( staff.attribute( "label" ) ) {
        // we use type only for typing mensural notation
        vrvStaff->notAnc = true;
    }
    
    if ( !vrvStaff->HasN() ) {
        LogWarning("No @n on <staff> might yield unpredictable results");
    }
    
    AddMeasureElement(parent, vrvStaff);

    return ReadMeiStaffChildren( vrvStaff, staff );
}
    
    
bool MeiInput::ReadMeiStaffChildren( Object *parent, pugi::xml_node parentNode )
{
    assert( dynamic_cast<Staff*>( parent ) || dynamic_cast<EditorialElement*>( parent ) );
    
    bool success = true;
    pugi::xml_node current;
    for( current = parentNode.first_child( ); current; current = current.next_sibling( ) ) {
        if (!success) break;
        if ( std::string( current.name() ) == "app" ) {
            success = ReadMeiApp( parent, current, EDITORIAL_STAFF);
        }
        if ( std::string( current.name() ) == "annot" ) {
            success = ReadMeiAnnot( parent, current );
        }
        else if ( std::string( current.name() ) == "layer" ) {
            success = ReadMeiLayer( parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staff>", current.name() );
        }
    }
    
    return success;
}
    
bool MeiInput::ReadMeiLayer( Object *parent, pugi::xml_node layer )
{
    Layer *vrvLayer = new Layer();
    SetMeiUuid(layer, vrvLayer);
    
    vrvLayer->ReadCommon(layer);
    
    if ( !vrvLayer->HasN() ) {
        LogWarning("No @n on <layer> might yield unpredictable results");
    }
    
    // This could me moved to an AddLayer method for consistency with AddLayerElement
    if ( dynamic_cast<Staff*>( parent ) ) {
        dynamic_cast<Staff*>( parent )->AddLayer( vrvLayer );
    }
    else if ( dynamic_cast<EditorialElement*>( parent ) ) {
        dynamic_cast<EditorialElement*>( parent )->AddLayer( vrvLayer );
    }
    
    return ReadMeiLayerChildren( vrvLayer, layer );
}

bool MeiInput::ReadMeiLayerChildren( Object *parent, pugi::xml_node parentNode, Object *filter )
{
    bool success = true;
    pugi::xml_node xmlElement;
    std::string elementName;
    for( xmlElement = parentNode.first_child( ); xmlElement; xmlElement = xmlElement.next_sibling( ) ) {
        if (!success) {
            break;
        }
        elementName = std::string( xmlElement.name() );
        if ( !IsAllowed( elementName, filter ) ) {
            LogDebug("Element <%s> within %s ignored", xmlElement.name(), filter->GetClassName().c_str() );
            continue;
        }
        else if ( elementName == "accid" ) {
            success = ReadMeiAccid( parent, xmlElement );
        }
        else if ( elementName == "app" ) {
            success = ReadMeiApp( parent, xmlElement, EDITORIAL_LAYER, filter);
        }
        else if ( elementName == "annot" ) {
            success = ReadMeiAnnot( parent, xmlElement);
        }
        else if ( elementName  == "barLine" ) {
            success = ReadMeiBarline( parent, xmlElement );
        }
        else if ( elementName == "beam" ) {
            success = ReadMeiBeam( parent, xmlElement);
        }
        else if ( elementName == "chord" ) {
            success = ReadMeiChord( parent, xmlElement);
        }
        else if ( elementName == "clef" ) {
            success = ReadMeiClef( parent, xmlElement);
        }
        else if ( elementName == "custos" ) {
            success = ReadMeiCustos( parent, xmlElement );
        }
        else if ( elementName == "dot" ) {
            success = ReadMeiDot( parent, xmlElement );
        }
        else if ( elementName == "keySig" ) {
            success = ReadMeiKeySig( parent, xmlElement );
        }
        else if ( elementName == "mensur" ) {
            success = ReadMeiMensur( parent, xmlElement );
        }
        else if ( elementName == "meterSig" ) {
            success = ReadMeiMeterSig( parent, xmlElement );
        }
        else if ( elementName == "note" ) {
            success = ReadMeiNote( parent, xmlElement );
        }
        else if ( elementName == "rest" ) {
            success = ReadMeiRest( parent, xmlElement );
        }
        else if ( elementName == "mRest" ) {
            success = ReadMeiMRest( parent, xmlElement );
        }
        else if ( elementName == "multiRest" ) {
            success = ReadMeiMultiRest( parent, xmlElement );
        }
        else if ( elementName == "space" ) {
            success = ReadMeiSpace( parent, xmlElement );
        }
        else if ( elementName == "syl" ) {
            success = ReadMeiSyl( parent, xmlElement );
        }
        else if ( elementName == "tuplet" ) {
            success = ReadMeiTuplet( parent, xmlElement );
        }
        else if ( elementName == "verse" ) {
            success = ReadMeiVerse( parent, xmlElement );
        }
        // unknown
        else {
            LogDebug("Element %s ignored", xmlElement.name() );
        }
    }
    
    return success;
}
    
bool MeiInput::ReadLayerElement( pugi::xml_node element, LayerElement *object )
{
    if ( element.attribute( "ulx" ) ) {
        object->m_xAbs = atoi ( element.attribute( "ulx" ).value() ) * DEFINITON_FACTOR;
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
    vrvAccid->ReadAccidental(accid);
    
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
    
    ReadMeiLayerChildren(vrvBeam, beam);
    
    if ( vrvBeam->GetNoteCount() == 1 ) {
        LogWarning("<beam> with only one note");
    }
    
    return true;
}
    
bool MeiInput::ReadMeiChord( Object *parent, pugi::xml_node chord)
{
    Chord *vrvChord = new Chord();
    SetMeiUuid(chord, vrvChord);
    
    ReadDurationInterface(chord, vrvChord);
    vrvChord->ReadCommon(chord);
    vrvChord->ReadStemmed(chord);
    vrvChord->ReadTiepresent(chord);
    
    if ( chord.attribute( "grace" ) ) {
        vrvChord->m_cueSize = true;
    }
    
    AddLayerElement(parent, vrvChord);
    return ReadMeiLayerChildren(vrvChord, chord);
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
    
bool MeiInput::ReadMeiKeySig( Object *parent, pugi::xml_node keySig )
{
    KeySig *vrvKeySig = new KeySig();
    ReadLayerElement(keySig, vrvKeySig);
    
    vrvKeySig->ReadAccidental( keySig );
    vrvKeySig->ReadPitch( keySig );
    
    vrvKeySig->ConvertToInternal();
    
    AddLayerElement(parent, vrvKeySig);
    return true;
}

bool MeiInput::ReadMeiMensur( Object *parent, pugi::xml_node mensur )
{
    Mensur *vrvMensur = new Mensur();
    ReadLayerElement(mensur, vrvMensur);
    
    vrvMensur->ReadDurationRatio( mensur );
    vrvMensur->ReadMensuralShared( mensur );
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
    vrvNote->ReadTiepresent(note);
    
    // grace
    if ( note.attribute( "grace" ) ) {
		vrvNote->m_cueSize = true; //
	}
    
    AddLayerElement(parent, vrvNote);
    
    // We can drop this once we allow <accid> and <note> child
    pugi::xml_node current;
    for( current = note.first_child( ); current; current = current.next_sibling( ) ) {
        if ( std::string( current.name() ) == "accid" ) {
            ReadAccidAsAttr(  vrvNote, current );
        }
    }
    
	return ReadMeiLayerChildren(vrvNote, note, vrvNote);
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

bool MeiInput::ReadMeiSpace( Object *parent, pugi::xml_node space )
{
    Space *vrvSpace = new Space();
    ReadLayerElement(space, vrvSpace);
    
    ReadDurationInterface(space, vrvSpace);
    
    AddLayerElement(parent, vrvSpace);
    return true;
}

bool MeiInput::ReadMeiSyl( Object *parent, pugi::xml_node syl)
{
    Syl *vrvSyl = new Syl();
    ReadLayerElement(syl, vrvSyl);
    
    vrvSyl->ReadTypography( syl );
    vrvSyl->ReadSylLog( syl );
    ReadText( syl, vrvSyl );
    
    AddLayerElement(parent, vrvSyl);
    return true;
}

bool MeiInput::ReadMeiTuplet( Object *parent, pugi::xml_node tuplet )
{
    Tuplet *vrvTuplet = new Tuplet();
    ReadLayerElement(tuplet, vrvTuplet);
    
    // Read in the numerator and denominator properties
    vrvTuplet->ReadDurationRatio(tuplet);
    
    AddLayerElement(parent, vrvTuplet);
    
    bool success = ReadMeiLayerChildren(vrvTuplet, tuplet);
    
    if ( vrvTuplet->GetNoteCount() == 1 ) {
        LogWarning("<tuplet> with only one note");
    }
    
    return success;
}

bool MeiInput::ReadMeiVerse(Object *parent, pugi::xml_node verse)
{
    Verse *vrvVerse = new Verse();
    ReadLayerElement(verse, vrvVerse);
    
    vrvVerse->ReadCommon(verse);
    
    AddLayerElement(parent, vrvVerse);
    
    return ReadMeiLayerChildren(vrvVerse, verse, vrvVerse);
}

bool MeiInput::ReadDurationInterface(pugi::xml_node element, DurationInterface *interface)
{
    interface->ReadAugmentdots(element);
    interface->ReadBeamsecondary(element);
    interface->ReadDurationMusical(element);
    interface->ReadDurationPerformed(element);
    interface->ReadDurationRatio(element);
    interface->ReadFermatapresent(element);
    interface->ReadStaffident(element);
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
    
bool MeiInput::ReadTimeSpanningInterface(pugi::xml_node element, TimeSpanningInterface *interface)
{
    interface->ReadStartendid(element);
    interface->ReadStartid(element);
    return true;
}

bool MeiInput::ReadAccidAsAttr( Note *note, pugi::xml_node accid )
{
    Accid vrvAccid;
    vrvAccid.ReadAccidental( accid );
    note->SetAccid( vrvAccid.GetAccid() );
    return true;
}
    
void MeiInput::ReadSameAsAttr( pugi::xml_node element, Object *object )
{
    if ( !element.attribute( "sameas" ) ) {
        return;
    }
    
    object->m_sameAs = element.attribute( "sameas" ).value();
}

    
void MeiInput::ReadUnsupportedAttr( pugi::xml_node element, Object *object )
{
    for (pugi::xml_attribute_iterator ait = element.attributes_begin(); ait != element.attributes_end(); ++ait)
    {
        object->m_unsupported.push_back(std::make_pair(ait->name(), ait->value()));
    }
}
    
void MeiInput::ReadText( pugi::xml_node element, Object *object )
{
    if (element.text()) {
        object->SetText(UTF8to16( element.text().as_string() ));
    }
}
    
bool MeiInput::ReadEditorialElement( pugi::xml_node element, EditorialElement *object )
{

    object->ReadCommon( element );
    ReadSameAsAttr( element, object );
    SetMeiUuid( element, object );
    
    return true;
}

bool MeiInput::ReadMeiApp( Object *parent, pugi::xml_node app, EditorialLevel level, Object *filter )
{
    if (!m_hasScoreDef) {
        LogError("<app> before any <scoreDef> is not supported");
        return false;
    }
    
    App *vrvApp = new App( level );
    ReadEditorialElement( app, vrvApp );
    parent->AddEditorialElement(vrvApp);
    
    return ReadMeiAppChildren( vrvApp, app, level, filter );
}
    
    
bool MeiInput::ReadMeiAppChildren( Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter )
{
    assert( dynamic_cast<App*>( parent ) );
    
    // Check if one child node matches the m_rdgXPathQuery
    pugi::xml_node selectedLemOrRdg;
    if ( m_rdgXPathQuery.length() > 0 ) {
        pugi::xpath_node selection = parentNode.select_single_node( m_rdgXPathQuery.c_str() );
        if ( selection ) selectedLemOrRdg = selection.node();
    }
    
    bool success = true;
    bool hasXPathSelected = false;
    pugi::xml_node current;
    for( current = parentNode.first_child( ); current; current = current.next_sibling( ) ) {
        if (!success) break;
        if ( std::string( current.name() ) == "lem" ) {
            success = ReadMeiLemOrRdg( parent, current, level, filter);
        }
        else if ( std::string( current.name() ) == "rdg" ) {
            success = ReadMeiLemOrRdg( parent, current, level, filter);
        }
        else {
            LogWarning("Unsupported '<%s>' within <app>", current.name() );
        }
        // Now we check if the xpath selection (if any) matches the current node.
        // If yes, make it visible
        if ( selectedLemOrRdg == current ) {
            EditorialElement *last = dynamic_cast<EditorialElement*>(parent->m_children.back());
            if (last) {
                last->m_visibility = Visible;
                hasXPathSelected = true;
            }
        }
    }
    
    // If no child was made visible through the xpath selection, make the first one visible
    if (!hasXPathSelected) {
        EditorialElement *first = dynamic_cast<EditorialElement*>(parent->m_children.front());
        if (first) {
            first->m_visibility = Visible;
        }
        else {
            LogWarning("Could not make one <rdg> or <lem> visible" );
        }
    }

    return success;
}
    
bool MeiInput::ReadMeiLemOrRdg( Object *parent, pugi::xml_node lemOrRdg, EditorialLevel level, Object *filter )

{
    assert( dynamic_cast<App*>( parent ) );
    
    EditorialElement *vrvLemOrRdg;
    if ( std::string( lemOrRdg.name() ) == "lem" ) {
        vrvLemOrRdg = new Lem();
    }
    else {
        vrvLemOrRdg = new Rdg();
    }
    // By default make them all hidden. MeiInput::ReadMeiAppChildren will make one visible.
    vrvLemOrRdg->m_visibility = Hidden;
    
    ReadEditorialElement( lemOrRdg, vrvLemOrRdg );
    dynamic_cast<App*>( parent )->AddLemOrRdg(vrvLemOrRdg);
    
    if (level == EDITORIAL_SYSTEM) {
        return ReadMeiSystemChildren(vrvLemOrRdg, lemOrRdg);
    }
    else if (level == EDITORIAL_SCOREDEF) {
        return ReadMeiScoreDefChildren(vrvLemOrRdg, lemOrRdg);
    }
    else if (level == EDITORIAL_STAFFGRP) {
        return ReadMeiStaffGrpChildren(vrvLemOrRdg, lemOrRdg);
    }
    else if (level == EDITORIAL_MEASURE) {
        return ReadMeiMeasureChildren(vrvLemOrRdg, lemOrRdg);
    }
    else if (level == EDITORIAL_STAFF) {
        return ReadMeiStaffChildren(vrvLemOrRdg, lemOrRdg);
    }
    else if (level == EDITORIAL_LAYER) {
        return ReadMeiLayerChildren(vrvLemOrRdg, lemOrRdg, filter);
    }
    else {
        return false;
    }
}
    
bool MeiInput::ReadMeiAnnot( Object *parent, pugi::xml_node annot )
{
    
    Annot *vrvAnnot = new Annot( );
    ReadEditorialElement( annot, vrvAnnot );
    vrvAnnot->ReadPlist(annot);
    vrvAnnot->ReadSource(annot);
    
    vrvAnnot->m_content.reset();
    //copy all the nodes inside into the document
    for (pugi::xml_node child = annot.first_child(); child; child = child.next_sibling())
    {
        vrvAnnot->m_content.append_copy(child);
    }
    
    parent->AddEditorialElement(vrvAnnot);
    
    return true;
}
    
void MeiInput::AddScoreDef(Object *parent, ScoreDef *scoreDef)
{
    if (!m_hasScoreDef) {
        m_hasScoreDef = true;
    }
    else if ( dynamic_cast<EditorialElement*>( parent ) ) {
        dynamic_cast<EditorialElement*>( parent )->AddScoreDef( scoreDef );
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
    if ( dynamic_cast<EditorialElement*>( parent ) ) {
        dynamic_cast<EditorialElement*>( parent )->AddStaffGrp( staffGrp );
    }
    else if ( dynamic_cast<ScoreDef*>( parent ) ) {
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
    if ( dynamic_cast<EditorialElement*>( parent ) ) {
        dynamic_cast<EditorialElement*>( parent )->AddLayerElement( element );
    }
    else if ( dynamic_cast<Layer*>( parent ) ) {
        dynamic_cast<Layer*>( parent )->AddLayerElement( element );
    }
    else if ( dynamic_cast<Chord*>( parent ) ) {
        dynamic_cast<Chord*>( parent )->AddLayerElement( element );
    }
    else if ( dynamic_cast<Note*>( parent ) ) {
        dynamic_cast<Note*>( parent )->AddLayerElement( element );
    }
    else if ( dynamic_cast<Beam*>( parent ) ) {
        dynamic_cast<Beam*>( parent )->AddLayerElement( element );
    }
    else if ( dynamic_cast<Tuplet*>( parent ) ) {
        dynamic_cast<Tuplet*>( parent )->AddLayerElement( element );
    }
    else if ( dynamic_cast<Verse*>( parent ) ) {
        dynamic_cast<Verse*>( parent )->AddLayerElement( element );
    }
    else {
        LogWarning("'%s' not supported within '%s'", element->GetClassName().c_str(), parent->GetClassName().c_str() );
        delete element;
    }
}
    
void MeiInput::AddMeasureElement(Object *parent, MeasureElement *element)
{
    if ( dynamic_cast<EditorialElement*>( parent ) ) {
        dynamic_cast<EditorialElement*>( parent )->AddMeasureElement( element );
    }
    else if ( dynamic_cast<Measure*>( parent ) ) {
        dynamic_cast<Measure*>( parent )->AddMeasureElement( element );
    }
    else {
        LogWarning("'%s' not supported within '%s'", element->GetClassName().c_str(), parent->GetClassName().c_str() );
        delete element;
    }
}

bool MeiInput::ReadScoreBasedMei( pugi::xml_node element )
{
    bool success = true;
    if ( (std::string( element.name() ) == "app") ) {
        success = ReadMeiApp( m_system, element, EDITORIAL_SYSTEM );
    }
    if ( (std::string( element.name() ) == "annot") ) {
        success = ReadMeiAnnot( m_system, element );
    }
    else if ( std::string( element.name() ) == "measure" ) {
        // This is the call that will put us back on the page-based reading loop
        success = ReadMeiMeasure( m_system, element );
    }
    else if (std::string( element.name() ) == "pb") {
        if ( (m_system->GetChildCount() > 0) && !m_ignoreLayoutInformation) {
            //LogDebug( "pb" );
            this->m_hasLayoutInformation = true;
            m_page = new Page( );
            m_system = new System( );
            m_page->AddSystem( m_system );
            m_doc->AddPage( m_page );
        }        
    }
    else if (std::string( element.name() ) == "sb") {
        if ( (m_page->GetSystemCount() > 0)  && !m_ignoreLayoutInformation) {
            //LogDebug( "pb" );
            this->m_hasLayoutInformation = true;
            m_system = new System( );
            m_page->AddSystem( m_system );
        }
    }
    else if ( std::string( element.name() ) == "score" ) {
        pugi::xml_node current;
        for( current = element.first_child( ); current; current = current.next_sibling( ) ) {
            if (!success) break;
            success = ReadScoreBasedMei( current );
        }
    }
    else if ( (std::string( element.name() ) == "scoreDef") ) {
        success = ReadMeiScoreDef( m_system, element );
    }
    else if ( std::string( element.name() ) == "section" ) {
        pugi::xml_node current;
        for( current = element.first_child( ); current; current = current.next_sibling( ) ) {
            if (!success) break;
            // This will happen with unmeasured music - ReadMeiSystemChildren will read take over the loop
            // This means that <pb> and <sb> (for example) will not be read
            else if ( (std::string( current.name() ) == "staff") ) {
                success = ReadMeiSystemChildren( m_system, element );
                break;
            }
            success = ReadScoreBasedMei( current );
        }
    }
    else {
        LogWarning( "Elements <%s> ignored", element.name() );
    }
    return success;
}
    
bool MeiInput::ReadTupletSpanAsTuplet( Measure *measure, pugi::xml_node tupletSpan)
{
    if (!measure) {
        LogWarning( "Cannot read <tupleSpan> within editorial markup" );
        return false;
    }
    
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
        tuplet->AddLayerElement( dynamic_cast<LayerElement*>( parentLayer->DetachChild(i) ) );
    }
    tuplet->SetParent( parentLayer );
    parentLayer->InsertChild( tuplet, startIdx );

    return true;    
}

void MeiInput::SetMeiUuid( pugi::xml_node element, Object *object )
{
    if ( !element.attribute( "xml:id" ) ) {
        return;
    }
    
    object->SetUuid( element.attribute( "xml:id" ).value() );
    element.remove_attribute("xml:id");
}

bool MeiInput::StrToBool(std::string value)
{
    if (value == "false") return false;
	return true;
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
    size_t pos = refUuid.find_last_of("#");
    if ( (pos != std::string::npos) && (pos < refUuid.length() - 1) ) {
        refUuid = refUuid.substr( pos + 1 );
    }
    return refUuid;
}

} // namespace vrv

