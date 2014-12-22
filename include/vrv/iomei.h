/////////////////////////////////////////////////////////////////////////////
// Name:        iomei.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOMEI_H__
#define __VRV_IOMEI_H__

#include <sstream>

//----------------------------------------------------------------------------

#include "doc.h"
#include "pugixml.hpp"

namespace vrv {

class Accid;
class Barline;
class Beam;
class Clef;
class Custos;
class Dot;
class DurationInterface;
class Layer;
class LayerElement;
class Measure;
class Mensur;
class MeterSig;
class MRest;
class MultiRest;
class Note;
class PitchInterface;
class PositionInterface;
class Rest;
class ScoreDef;
class Staff;
class Syl;
class System;
class Tuplet;
class Verse;


//----------------------------------------------------------------------------
// MeiOutput
//----------------------------------------------------------------------------

/**
 * This class is a file output stream for writing MEI files.
 * It uses the libmei C++ library.
 * Not implemented.
*/
class MeiOutput: public FileOutputStream
{
public:
    /** @name Constructors and destructor */
    ///@{
    MeiOutput( Doc *doc, std::string filename );
    virtual ~MeiOutput();
    ///@}
    
    /**
     * The main method for exporting the file to MEI.
     */ 
    virtual bool ExportFile( );

    /**
     * The main method for write objects.
     */
    virtual bool WriteObject( Object *object );
    
    /**
     * Writing object method that must be overriden in child class.
     */
    virtual bool WriteObjectEnd( Object *object );
    
    /**
     * Return the output as a string by writing it to the stringstream member.
     */
    std::string GetOutput();

private:
    
    bool WriteMeiDoc( Doc *doc );
    
    /**
     * @name Methods for reading  MEI containers (measures, staff, etc) scoreDef and related.
     */
    ///@{
    bool WriteMeiPage( pugi::xml_node currentNode, Page *page );
    bool WriteMeiSystem( pugi::xml_node currentNode, System *system );
    bool WriteMeiScoreDef( pugi::xml_node currentNode, ScoreDef *scoreDef );
    bool WriteMeiStaffGrp( pugi::xml_node currentNode, StaffGrp *staffGrp );
    bool WriteMeiStaffDef( pugi::xml_node currentNode, StaffDef *staffDef );
    bool WriteMeiMeasure( pugi::xml_node currentNode, Measure *measure );
    bool WriteMeiStaff( pugi::xml_node currentNode, Staff *staff );
    bool WriteMeiLayer( pugi::xml_node currentNode, Layer *layer );
    ///@}
    
    /**
     * Write an LayerElement child. 
     * Called from WriteLayerElement.
     */
    ///@{
    void WriteMeiAccid( pugi::xml_node currentNode, Accid *accid );
    void WriteMeiBarline( pugi::xml_node currentNode, Barline *barLine );
    void WriteMeiBeam( pugi::xml_node currentNode, Beam *beam );
    void WriteMeiClef( pugi::xml_node currentNode, Clef *clef );
    void WriteMeiCustos( pugi::xml_node currentNode, Custos *custos );
    void WriteMeiDot( pugi::xml_node currentNode, Dot *dot );
    void WriteMeiMensur( pugi::xml_node currentNode, Mensur *mensur );
    void WriteMeiMeterSig( pugi::xml_node currentNode, MeterSig *meterSig );
    void WriteMeiMRest( pugi::xml_node currentNode, MRest *mRest );
    void WriteMeiMultiRest( pugi::xml_node currentNode, MultiRest *multiRest );
    void WriteMeiNote( pugi::xml_node currentNode, Note *note );
    void WriteMeiRest( pugi::xml_node currentNode, Rest *rest );
    void WriteMeiTuplet( pugi::xml_node currentNode, Tuplet *tuplet );
    ///@}
    
    /**
     * Write other mei elements
     */
    ///@{
    void WriteMeiVerse( pugi::xml_node currentNode, Verse *verse );
    void WriteMeiSyl( pugi::xml_node currentNode, Syl *syl );
    ///@}
    
    /**
     * Write a sameAs attribute
     * The method has to be called by classed that support it (e.g., LayerElement)
     * To be changed to Att
     */
    void WriteSameAsAttr( pugi::xml_node currentNode, Object *object );
    
    /**
     * Write a LayerElement and interfaces.
     * Call WriteDurationInferface from WriteNote, for example.
     */
    ///@{
    void WriteLayerElement( pugi::xml_node currentNode, LayerElement *element );
    void WriteDurationInterface( pugi::xml_node currentNode, DurationInterface *interface );
    void WritePitchInterface( pugi::xml_node currentNode, PitchInterface *interface );
    void WritePositionInterface( pugi::xml_node currentNode, PositionInterface *interface );
    ///@}
    
    /**
     * Write the XML text content
     */
    void WriteText( pugi::xml_node currentNode, Object *object );
	
    /** @name Methods for converting members into MEI attributes. */
    ///@{
    std::string UuidToMeiStr( Object *element );
    std::string BoolToStr(bool value );
    std::string DocTypeToStr(DocType type);
    std::string StaffGrpSymbolToStr(StaffGrpSymbol symbol);
    ///@}

    
public:


private:
    std::string m_filename;
    std::stringstream m_streamStringOutput;
    bool m_writeToStreamString;
    pugi::xml_node m_mei;
    /** @name Current element */
    pugi::xml_node m_currentNode;
    std::list<pugi::xml_node> m_nodeStack;
};


//----------------------------------------------------------------------------
// MeiInput
//----------------------------------------------------------------------------

/**
 * This class is a file input stream for reading MEI files.
 * It uses the libmei C++ library.
 * Under development.
*/
class MeiInput: public FileInputStream
{
public:
    // constructors and destructors
    MeiInput( Doc *doc, std::string filename );
    virtual ~MeiInput();
    
    virtual bool ImportFile( );
    virtual bool ImportString(std::string mei);
    
    /**
     * Set an xPath query for selecting specific <rdg>.
     * By default, the first <lem> or <rdg> is loaded.
     * If a query is provided, the element retieved by the specified xPath
     * query will be selected (if any, otherwise the first one will be used).
     */
    virtual void SetRdgXPathQuery( std::string rdgXPathQuery ) { m_rdgXPathQuery = rdgXPathQuery; };
    
private:
    bool ReadMei( pugi::xml_node root );
    bool ReadMeiHeader( pugi::xml_node meihead );
    
    /**
     * @name Methods for reading  MEI containers (measures, staff, etc) scoreDef and related. 
     */
    ///@{
    bool ReadMeiPage( pugi::xml_node page );
    bool ReadMeiSystem( Page* page, pugi::xml_node system );
    bool ReadMeiScoreDef( Object *parent, pugi::xml_node scoreDef );
    bool ReadMeiStaffGrp( Object *parent, pugi::xml_node system );
    bool ReadMeiStaffDef( StaffGrp *staffGrp, pugi::xml_node system );
    bool ReadMeiMeasure( System *system, pugi::xml_node measure );
    bool ReadMeiStaff( Measure *measure, pugi::xml_node staff );
    bool ReadMeiLayer( Staff *staff, pugi::xml_node layer );
    ///@}

    /**
     * @name Methods for reading MEI layer elements
     */
    ///@{
    bool ReadMeiLayerElement( Object *parent, pugi::xml_node xmlElement );
    bool ReadMeiAccid( Object *parent, pugi::xml_node accid );
    bool ReadMeiBarline( Object *parent, pugi::xml_node barLine );
    bool ReadMeiBeam( Object *parent, pugi::xml_node beam );
    bool ReadMeiClef( Object *parent, pugi::xml_node clef );
    bool ReadMeiCustos( Object *parent, pugi::xml_node custos );
    bool ReadMeiDot( Object *parent, pugi::xml_node dot );
    bool ReadMeiMensur( Object *parent, pugi::xml_node mensur );
    bool ReadMeiMeterSig( Object *parent, pugi::xml_node meterSig );
    bool ReadMeiMRest( Object *parent, pugi::xml_node mRest );
    bool ReadMeiMultiRest( Object *parent, pugi::xml_node multiRest );
    bool ReadMeiNote( Object *parent, pugi::xml_node note );
    bool ReadMeiRest( Object *parent, pugi::xml_node rest );
    bool ReadMeiTuplet( Object *parent, pugi::xml_node tuplet );
    ///@}
  
    /**
     * @name Methods for reading critical apparatus.
     * Only one child of <app> is loaded
     */
    ///@{
    bool ReadMeiApp( Object *parent, pugi::xml_node app );
    pugi::xml_node GetSelectedReading( pugi::xml_node app );
    bool ReadMeiLemOrRdg( Object *parent, pugi::xml_node lemOrRdg );
    ///@}
    
    /**
     * @name Methods for reading MEI LayerElement and interfaces
     */
    ///@{
    bool ReadLayerElement( pugi::xml_node element, LayerElement *object );
    bool ReadDurationInterface( pugi::xml_node element, DurationInterface *interface );
    bool ReadPitchInterface( pugi::xml_node element, PitchInterface *interface );
    bool ReadPositionInterface( pugi::xml_node element, PositionInterface *interface );
    ///@}

    /**
     * @name Methods for reading other MEI elements.
     */
    ///@{
    bool ReadAccidAsAttr( Note *note, pugi::xml_node verse );
    bool ReadVerse( Note *note, pugi::xml_node verse );
    bool ReadSyl( Verse *verse, pugi::xml_node syl );
    bool ReadTupletSpanAsTuplet( Measure *measure, pugi::xml_node tupletSpan );
    bool ReadSlurAsSlurAttr( Measure *measure, pugi::xml_node slur );
    ///@}
    
    /**
     * Read a sameAs attribute
     * The method has to be called by classed that support it (e.g., LayerElement)
     */
    void ReadSameAsAttr( pugi::xml_node element, Object *object );
    
    /**
     * Read the XML text content.
     * It has to be called explicitly for elements with text (e.g., <syl>).
     */
    void ReadText( pugi::xml_node element, Object *object );
    
    /**
     * Method for adding the element to the appropriate parent (e.g., Layer, Beam).
     * This used for any element that supports different types of child.
     * For example, the StaffGrp can contain StaffGrp or StaffDef.
     * These methods dynamically case the parent to the appropriate class.
     */
    ///@{
    void AddLayerElement( Object *parent, LayerElement *element );
    void AddScoreDef( Object *parent, ScoreDef *element );
    void AddStaffGrp( Object *parent, StaffGrp *element );
    ///@}
    
    /**
     * Read score-based MEI
     */
    bool ReadScoreBasedMei( pugi::xml_node element );    
    
    /**
     * Look through the list of notes with open tie stored in MeiInput::m_openTies.
     * The note has to be on the same staff (@n) and the same layer (@n) and
     * have the same pitch. If found, the terminal attribute is the and the note
     * is removed from the list
     */
    bool FindOpenTie( Note *terminalNote );
    
    /**
     * @name Various methods for reading / converting values.
     */
    ///@{
    void SetMeiUuid( pugi::xml_node element, Object *object );
    bool StrToBool(std::string value);
    DocType StrToDocType(std::string type);
    StaffGrpSymbol StrToStaffGrpSymbol(std::string sign);
    /** Extract the uuid for references to uuids with ..#uuid values */
    std::string ExtractUuidFragment(std::string refUuid);
    ///@}
     
public:
    
private:
    std::string m_filename;
    
    /** The current page when reading score-based MEI */
    Page *m_page;
    /** The current system when reading score-based MEI */
    System *m_system;
    
    /**
     *
     */
    std::string m_rdgXPathQuery;
    
    /**
     * This is used when reading a standard MEI file to specify if a scoreDef has already been read or not.
     */
    bool m_hasScoreDef;
    
    /**
     * A vector of keeping the notes with open ties.
     * This should be moved to the Object::PrepareDrawing functor
     */
    std::vector<Note*> m_openTies;
};

} // namespace vrv

#endif
