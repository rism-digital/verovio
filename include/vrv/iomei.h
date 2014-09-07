/////////////////////////////////////////////////////////////////////////////
// Name:        iomei.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOMEI_H__
#define __VRV_IOMEI_H__

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
class Mensur;
class MeterSig;
class MRest;
class MultiRest;
class Note;
class PitchInterface;
class PositionInterface;
class Rest;
class Syl;
class Symbol;
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
    
    /** @name Writing element methods
     * Overriding methods for writing the MEI file.
     */
    ///@{
    virtual bool WriteDoc( Doc *doc );
    virtual bool WritePage( Page *page );
    virtual bool WriteSystem( System *system );
    virtual bool WriteScoreDef( ScoreDef *scoreDef );
    virtual bool WriteStaffGrp( StaffGrp *staffGrp );
    virtual bool WriteStaffDef( StaffDef *staffDef );
    virtual bool WriteMeasure( Measure *measure );
    virtual bool WriteStaff( Staff *staff );
    virtual bool WriteLayer( Layer *layer );
    virtual bool WriteLayerElement( LayerElement *element );
    // app
    virtual bool WriteLayerApp( LayerApp *app );
    virtual bool WriteLayerRdg( LayerRdg *rdg );
    ///@}

private:
    
    /**
     * Write an LayerElement child. 
     * Called from WriteLayerElement.
     */
    ///@{
    void WriteMeiAccid( pugi::xml_node meiAccid, Accid *accid );
    void WriteMeiBarline( pugi::xml_node meiBarline, Barline *barLine );
    void WriteMeiBeam( pugi::xml_node meiBeam, Beam *beam );
    void WriteMeiClef( pugi::xml_node meiClef, Clef *clef );
    void WriteMeiCustos( pugi::xml_node meiCustos, Custos *custos );
    void WriteMeiDot( pugi::xml_node meiDot, Dot *dot );
    void WriteMeiMensur( pugi::xml_node meiMensur, Mensur *mensur );
    void WriteMeiMeterSig( pugi::xml_node meiMeterSig, MeterSig *meterSig );
    void WriteMeiMRest( pugi::xml_node meiMRest, MRest *mRest );
    void WriteMeiMultiRest( pugi::xml_node meiMultiRest, MultiRest *multiRest );
    void WriteMeiNote( pugi::xml_node meiNote, Note *note );
    void WriteMeiRest( pugi::xml_node meiRest, Rest *rest );
    void WriteMeiTuplet( pugi::xml_node meiTuplet, Tuplet *tuplet );
    
    /**
     * Write a Verse and syl
     */
    //void WriteVerse( Verse *verse, pugi::xml_node currentParent );
    //void WriteSyl( Syl *syl, pugi::xml_node currentParent );
    
    /**
     * Write a sameAs attribute
     * The method has to be called by classed that support it (e.g., LayerElement)
     * To be changed to Att
     */
    void WriteSameAsAttr( pugi::xml_node element, Object *object );
    
    /**
     * Write a interfaces.
     * Call WriteDurationInferface from WriteNote, for example.
     */
    ///@{
    void WriteDurationInterface( pugi::xml_node element, DurationInterface *interface );
    void WritePitchInterface( pugi::xml_node element, PitchInterface *interface );
    void WritePositionInterface( pugi::xml_node element, PositionInterface *interface );
    ///@}
    
    /**
     * Write the XML text content
     */
    void WriteText( pugi::xml_node element, Object *object );
	
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
    pugi::xml_node m_mei;
    /** @name Members for pointers to the current element */
    ///@{
    pugi::xml_node m_pages;
    pugi::xml_node m_page;
    pugi::xml_node m_scoreDef;
    pugi::xml_node m_system;
    pugi::xml_node m_staffGrp;
    pugi::xml_node m_staffDef;
    pugi::xml_node m_measure;
    pugi::xml_node m_staff;
    /** The pointer for the layer within a staff */
    pugi::xml_node m_layer;
    /** The pointer for the rdg within an app (LayerRdg) */
    pugi::xml_node m_rdgLayer;
    /** The pointer for a beam */
    pugi::xml_node m_beam;
    /** The pointer for a tuplet */
    pugi::xml_node m_tuplet;
    // app
    pugi::xml_node m_app;
    ///@}
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
    
private:
    bool ReadMei( pugi::xml_node root );
    bool ReadMeiHeader( pugi::xml_node meihead );
    /** Reads the content of a <layer> or of a <rdg> for <app> within <layer> */
    bool ReadMeiPage( pugi::xml_node page );
    bool ReadMeiSystem( pugi::xml_node system );
    bool ReadMeiScoreDef( pugi::xml_node scoreDef );
    bool ReadMeiStaffGrp( pugi::xml_node system );
    bool ReadMeiStaffDef( pugi::xml_node system );
    bool ReadMeiMeasure( pugi::xml_node measure );
    bool ReadMeiStaff( pugi::xml_node staff );
    bool ReadMeiLayer( pugi::xml_node layer );
    bool ReadMeiLayerElement( pugi::xml_node XmlElement );
    LayerElement *ReadMeiBarline( pugi::xml_node barLine );
    LayerElement *ReadMeiBeam( pugi::xml_node beam );
    LayerElement *ReadMeiClef( pugi::xml_node clef );
    LayerElement *ReadMeiMensur( pugi::xml_node mensur );
    LayerElement *ReadMeiMeterSig( pugi::xml_node meterSig );
    LayerElement *ReadMeiMRest( pugi::xml_node mRest );
    LayerElement *ReadMeiMultiRest( pugi::xml_node multiRest );
    LayerElement *ReadMeiNote( pugi::xml_node note );
    LayerElement *ReadMeiRest( pugi::xml_node rest );
    LayerElement *ReadMeiTuplet( pugi::xml_node tuplet );
    LayerElement *ReadMeiAccid( pugi::xml_node accid );
    LayerElement *ReadMeiCustos( pugi::xml_node custos );
    LayerElement *ReadMeiDot( pugi::xml_node dot );
    //
    bool ReadDurationInterface( pugi::xml_node element, DurationInterface *interface );
    bool ReadPitchInterface( pugi::xml_node element, PitchInterface *interface );
    bool ReadPositionInterface( pugi::xml_node element, PositionInterface *interface );
    //
    bool ReadVerse( Note *note, pugi::xml_node verse );
    bool ReadSyl( Verse *verse, pugi::xml_node syl );
    //
    bool ReadTupletSpanAsTuplet( pugi::xml_node tupletSpan );
    bool ReadSlurAsSlurAttr( pugi::xml_node slur );
    /** Reads <app> elements. For now, only <app> within <layer> are taken into account */
    LayerElement *ReadMeiApp( pugi::xml_node app );
    bool ReadMeiRdg( pugi::xml_node rdg );
    
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
     * Add the LayerElement to the appropriate parent (e.g., Layer, LayerRdg)
     */
    void AddLayerElement( LayerElement *element );
    
    /**
     * Read unsupported element and try to convert them
     */
    bool ReadUnsupported( pugi::xml_node element );
    
    /**
     * Look through the list of notes with open tie stored in MeiInput::m_openTies.
     * The note has to be on the same staff (@n) and the same layer (@n) and
     * have the same pitch. If found, the terminal attribute is the and the note
     * is removed from the list
     */
    bool FindOpenTie( Note *terminalNote );
    
	//
    void SetMeiUuid( pugi::xml_node element, Object *object );
    bool StrToBool(std::string value);
    DocType StrToDocType(std::string type);
    StaffGrpSymbol StrToStaffGrpSymbol(std::string sign);
    /** Extract the uuid for references to uuids with ..#uuid values */
    std::string ExtractUuidFragment(std::string refUuid);
     
public:
    
private:
    std::string m_filename;
    Page *m_page;
    System *m_system;
    ScoreDef *m_scoreDef;
    std::list<StaffGrp*> m_staffGrps;
    StaffDef *m_staffDef;
    Measure *m_measure;
	Staff *m_staff;
	Layer *m_layer;
    LayerRdg *m_layerRdg;
    Beam *m_beam;
    Tuplet *m_tuplet;
    Object *m_currentLayer;
    LayerApp *m_layerApp;
    /**
     * This is used when reading a standard MEI file to specify if a scoreDef has already been read or not.
     */
    bool m_hasScoreDef;
    
    /**
     * A vector of keeping the notes with open ties.
     */
    std::vector<Note*> m_openTies;
};

} // namespace vrv

#endif
