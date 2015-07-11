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
#include "io.h"
#include "pugixml.hpp"

namespace vrv {

class App;
class Accid;
class Annot;
class Barline;
class Beam;
class Chord;
class Clef;
class Custos;
class Dot;
class DurationInterface;
class Layer;
class LayerElement;
class Lem;
class Measure;
class MeasureElement;
class Mensur;
class MeterSig;
class MRest;
class MultiRest;
class Note;
class PitchInterface;
class PositionInterface;
class Rdg;
class Rest;
class ScoreDef;
class Slur;
class Space;
class Staff;
class Syl;
class System;
class Tie;
class TimeSpanningInterface;
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
    std::string GetOutput( int page = -1 );
    
    /**
     * Setter for score-based MEI output (non implemented)
     */
    void SetScoreBasedMEI( bool scoreBasedMEI ) { m_scoreBasedMEI = scoreBasedMEI; };

private:
    
    bool WriteMeiDoc( Doc *doc );
    
    /**
     * @name Methods for wrinting MEI containers (measures, staff, etc) scoreDef and related.
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
     * @name Methods for wrinting LayerElement children.
     * Called from WriteLayerElement.
     */
    ///@{
    void WriteMeiAccid( pugi::xml_node currentNode, Accid *accid );
    void WriteMeiBarline( pugi::xml_node currentNode, Barline *barLine );
    void WriteMeiBeam( pugi::xml_node currentNode, Beam *beam );
    void WriteMeiChord( pugi::xml_node currentNode, Chord *chord );
    void WriteMeiClef( pugi::xml_node currentNode, Clef *clef );
    void WriteMeiCustos( pugi::xml_node currentNode, Custos *custos );
    void WriteMeiDot( pugi::xml_node currentNode, Dot *dot );
    void WriteMeiKeySig( pugi::xml_node currentNode, KeySig *keySig );
    void WriteMeiMensur( pugi::xml_node currentNode, Mensur *mensur );
    void WriteMeiMeterSig( pugi::xml_node currentNode, MeterSig *meterSig );
    void WriteMeiMRest( pugi::xml_node currentNode, MRest *mRest );
    void WriteMeiMultiRest( pugi::xml_node currentNode, MultiRest *multiRest );
    void WriteMeiNote( pugi::xml_node currentNode, Note *note );
    void WriteMeiRest( pugi::xml_node currentNode, Rest *rest );
    void WriteMeiSpace( pugi::xml_node currentNode, Space *space );
    void WriteMeiTuplet( pugi::xml_node currentNode, Tuplet *tuplet );
    ///@}
    
    /**
     * @name Methods for wrinting MeasureElement children (other the staff).
     */
    ///@{
    void WriteMeiTie( pugi::xml_node currentNode, Tie *tie );
    void WriteMeiSlur( pugi::xml_node currentNode, Slur *slur );
    ///@}
    
    /**
     * @name Methods for writing editorial markup
     */
    ///@{
    bool WriteMeiApp( pugi::xml_node currentNode, App *app );
    bool WriteMeiLem( pugi::xml_node currentNode, Lem *lem );
    bool WriteMeiRdg( pugi::xml_node currentNode, Rdg *rdg );
    bool WriteMeiAnnot( pugi::xml_node currentNode, Annot *annot );
    ///@}
    
    /**
     * @name Methods for wrinting other mei elements
     */
    ///@{
    void WriteMeiVerse( pugi::xml_node currentNode, Verse *verse );
    void WriteMeiSyl( pugi::xml_node currentNode, Syl *syl );
    ///@}
    
    /**
     * @name Methods for wrinting a sameAs attribute
     * The method has to be called by classed that support it (e.g., LayerElement)
     * To be changed to Att
     */
    void WriteSameAsAttr( pugi::xml_node currentNode, Object *object );
    
    /**
     * Write unsupported attributes stored in Object::m_unsupported (not tested)
     */
    void WriteUnsupportedAttr( pugi::xml_node currentNode, Object *object );
    
    /**
     * @name Methods for wrinting LayerElement, EditorialElement and interfaces.
     * Call WriteDurationInferface from WriteNote, for example.
     */
    ///@{
    void WriteLayerElement( pugi::xml_node currentNode, LayerElement *element );
    void WriteEditorialElement( pugi::xml_node currentNode, EditorialElement *element );
    void WriteDurationInterface( pugi::xml_node currentNode, DurationInterface *interface );
    void WritePitchInterface( pugi::xml_node currentNode, PitchInterface *interface );
    void WritePositionInterface( pugi::xml_node currentNode, PositionInterface *interface );
    void WriteTimeSpanningInterface( pugi::xml_node currentNode, TimeSpanningInterface *interface );
    ///@}
    
    /**
     * @name Methods for wrinting the XML text content
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
    int m_page;
    bool m_scoreBasedMEI;
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
     * For each container (page, system, measure, staff and layer) there is one method for 
     * reading the element and one method for reading it children. The method for reading
     * the children can also be called when reading EditorialElement objects (<lem> or <rdg>
     * for example. The filter object is optionnal and can be set for filtering the allowed 
     * children (see MeiInput::IsAllowed)
     */
    ///@{
    bool ReadMeiPage( pugi::xml_node page );
    bool ReadMeiPageChildren( Object* parent, pugi::xml_node parentNode );
    bool ReadMeiSystem( Object* parent, pugi::xml_node system );
    bool ReadMeiSystemChildren( Object* parent, pugi::xml_node parentNode );
    bool ReadMeiScoreDef( Object *parent, pugi::xml_node scoreDef );
    bool ReadMeiScoreDefChildren( Object *parent, pugi::xml_node parentNode );
    bool ReadMeiStaffGrp( Object *parent, pugi::xml_node staffGrp );
    bool ReadMeiStaffGrpChildren( Object *parent, pugi::xml_node parentNode );
    bool ReadMeiStaffDef( Object *parent, pugi::xml_node staffDef );
    bool ReadMeiMeasure( Object *parent, pugi::xml_node measure );
    bool ReadMeiMeasureChildren( Object *parent, pugi::xml_node parentNode );
    bool ReadMeiStaff( Object *parent, pugi::xml_node staff );
    bool ReadMeiStaffChildren( Object *parent, pugi::xml_node parentNode );
    bool ReadMeiLayer( Object *parent, pugi::xml_node layer );
    bool ReadMeiLayerChildren( Object *parent, pugi::xml_node parentNode, Object *filter = NULL );
    ///@}

    /**
     * @name Methods for reading MEI layer elements
     */
    ///@{
    bool ReadMeiAccid( Object *parent, pugi::xml_node accid );
    bool ReadMeiBarline( Object *parent, pugi::xml_node barLine );
    bool ReadMeiBeam( Object *parent, pugi::xml_node beam );
    bool ReadMeiChord( Object* parent, pugi::xml_node chord );
    bool ReadMeiClef( Object *parent, pugi::xml_node clef );
    bool ReadMeiCustos( Object *parent, pugi::xml_node custos );
    bool ReadMeiDot( Object *parent, pugi::xml_node dot );
    bool ReadMeiKeySig( Object *parent, pugi::xml_node keySig );
    bool ReadMeiMensur( Object *parent, pugi::xml_node mensur );
    bool ReadMeiMeterSig( Object *parent, pugi::xml_node meterSig );
    bool ReadMeiMRest( Object *parent, pugi::xml_node mRest );
    bool ReadMeiMultiRest( Object *parent, pugi::xml_node multiRest );
    bool ReadMeiNote( Object *parent, pugi::xml_node note );
    bool ReadMeiRest( Object *parent, pugi::xml_node rest );
    bool ReadMeiSpace( Object *parent, pugi::xml_node space );
    bool ReadMeiSyl( Object *parent, pugi::xml_node syl );
    bool ReadMeiTuplet( Object *parent, pugi::xml_node tuplet );
    bool ReadMeiVerse( Object *parent, pugi::xml_node verse );
    ///@}
    
    /**
     * @name Methods for reading MEI measure elements
     */
    ///@{
    bool ReadMeiSlur( Object *parent, pugi::xml_node slur );
    bool ReadMeiTie( Object *parent, pugi::xml_node tie );
    ///@}
  
    /**
     * @name Methods for reading critical apparatus.
     * Only one child of <app> is loaded
     * The filter is propagated (if any)
     */
    ///@{
    bool ReadMeiApp( Object *parent, pugi::xml_node app, EditorialLevel level, Object *filter = NULL );
    bool ReadMeiAppChildren( Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter = NULL );
    bool ReadMeiLemOrRdg( Object *parent, pugi::xml_node lemOrRdg, EditorialLevel level, Object *filter = NULL );
    bool ReadMeiAnnot( Object *parent, pugi::xml_node annot );
    ///@}
    ///@}
    
    /**
     * @name Methods for reading MEI LayerElement, EidtorialElement and interfaces
     */
    ///@{
    bool ReadLayerElement( pugi::xml_node element, LayerElement *object );
    bool ReadEditorialElement( pugi::xml_node element, EditorialElement *object );
    bool ReadDurationInterface( pugi::xml_node element, DurationInterface *interface );
    bool ReadPitchInterface( pugi::xml_node element, PitchInterface *interface );
    bool ReadPositionInterface( pugi::xml_node element, PositionInterface *interface );
    bool ReadTimeSpanningInterface( pugi::xml_node element, TimeSpanningInterface *interface );
    ///@}

    /**
     * @name Methods for reading other MEI elements.
     */
    ///@{
    bool ReadAccidAsAttr( Note *note, pugi::xml_node verse );
    bool ReadTupletSpanAsTuplet( Measure *measure, pugi::xml_node tupletSpan );
    ///@}
    
    /**
     * Read a sameAs attribute
     * The method has to be called by classed that support it (e.g., LayerElement)
     */
    void ReadSameAsAttr( pugi::xml_node element, Object *object );
    
    /**
     * Write unsupported attributes and store them in Object::m_unsupported (not tested)
     */
    void ReadUnsupportedAttr( pugi::xml_node element, Object *object );
    
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
    void AddMeasureElement( Object *parent, MeasureElement *element );
    void AddScoreDef( Object *parent, ScoreDef *element );
    void AddStaffGrp( Object *parent, StaffGrp *element );
    ///@}
    
    /**
     * Read score-based MEI
     */
    bool ReadScoreBasedMei( pugi::xml_node element );    
    
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
     * Check if an element is allowed within a given parent
     */
    bool IsAllowed( std::string element, Object *filterParent );
};

} // namespace vrv

#endif
