/////////////////////////////////////////////////////////////////////////////
// Name:        musiomei.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __MUS_IOMEI_H__
#define __MUS_IOMEI_H__

#include "doc.h"

// TINYXML
#if defined (__WXMSW__)
    #include "tinyxml.h"
#else
    #include "tinyxml.h"
#endif

namespace vrv {

class Barline;
class Beam;
class Clef;
class Layer;
class Mensur;
class MultiRest;
class Note;
class Rest;
class Symbol;
class Tuplet;


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
     * Write a Barline. 
     * Callded from WriteLayerElement.
     */
    void WriteMeiBarline( TiXmlElement *meiBarline, Barline *barline );
    
    /**
     * Write a Beam. 
     * Callded from WriteLayerElement.
     */
    void WriteMeiBeam( TiXmlElement *meiBeam, Beam *beam );
    
    /**
     * Write a Clef.  
     * Callded from WriteLayerElement.
     */
    void WriteMeiClef( TiXmlElement *meiClef, Clef *clef );
    
    /**
     * Write a Mensur. 
     * Callded from WriteLayerElement.
     */
    void WriteMeiMensur( TiXmlElement *meiMensur, Mensur *mensur );
    
    /**
     * Write a MultiRest. 
     * Callded from WriteLayerElement.
     */
    void WriteMeiMultiRest( TiXmlElement *meiMultiRest, MultiRest *multiRest );
    
    /**
     * Write a Note. 
     * Callded from WriteLayerElement.
     */
    void WriteMeiNote( TiXmlElement *meiNote, Note *note );
    
    /**
     * Write a Rest. 
     * Callded from WriteLayerElement.
     */
    void WriteMeiRest( TiXmlElement *meiRest, Rest *rest );
    
    /**
     * Write a Tuplet. 
     * Callded from WriteLayerElement.
     */
    void WriteMeiTuplet( TiXmlElement *meiTuplet, Tuplet *tuplet );
    
    /**
     * Write a Symbol. 
     * The appropriate MeiElement is created by the method and returned.
     * Callded from WriteLayerElement.
     */
    TiXmlElement *WriteMeiSymbol( Symbol *symbol ); 
    
    /**
     * Write a sameAs attribute
     * The method has to be called by classed that support it (e.g., LayerElement)
     */
    void WriteSameAsAttr( TiXmlElement *meiElement, Object *element );
	
    /** @name Methods for converting members into MEI attributes. */
    ///@{
    std::string UuidToMeiStr( Object *element );
    std::string BoolToStr(bool value );
	std::string DurToStr(int dur);
	std::string OctToStr(int oct);
	std::string PitchToStr(int pitch);
    std::string AccidToStr(unsigned char accid);
    std::string ClefLineToStr(ClefId clefId);
    std::string ClefShapeToStr(ClefId clefId);
    std::string MensurSignToStr(MensurSign sign);
    std::string DocTypeToStr(DocType type);
    std::string KeySigToStr(int num, char alter_type );
    std::string BarlineTypeToStr(BarlineType type);
    std::string StaffGrpSymbolToStr(StaffGrpSymbol symbol);
    ///@}

    
public:


private:
    std::string m_filename;
    TiXmlElement *m_mei;
    /** @name Members for pointers to the current element */
    ///@{
    TiXmlElement *m_pages;
    TiXmlElement *m_page;
    TiXmlElement *m_scoreDef;
    TiXmlElement *m_system;
    TiXmlElement *m_staffGrp;
    TiXmlElement *m_staffDef;
    TiXmlElement *m_measure;
    TiXmlElement *m_staff;
    /** The pointer for the layer within a staff */
    TiXmlElement *m_layer;
    /** The pointer for the rdg within an app (LayerRdg) */
    TiXmlElement *m_rdgLayer;
    /** The pointer for a beam */
    TiXmlElement *m_beam;
    /** The pointer for a tuplet */
    TiXmlElement *m_tuplet;
    // app
    TiXmlElement *m_app;
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
    bool ReadMei( TiXmlElement *root );
    bool ReadMeiHeader( TiXmlElement *meihead );
    /** Reads the content of a <layer> or of a <rdg> for <app> within <layer> */
    bool ReadMeiPage( TiXmlElement *page );
    bool ReadMeiSystem( TiXmlElement *system );
    bool ReadMeiScoreDef( TiXmlElement *scoreDef );
    bool ReadMeiStaffGrp( TiXmlElement *system );
    bool ReadMeiStaffDef( TiXmlElement *system );
    bool ReadMeiMeasure( TiXmlElement *measure );
    bool ReadMeiStaff( TiXmlElement *staff );
    bool ReadMeiLayer( TiXmlElement *layer );
    bool ReadMeiLayerElement( TiXmlElement *XmlElement );
    LayerElement *ReadMeiBarline( TiXmlElement *barline );
    LayerElement *ReadMeiBeam( TiXmlElement *beam );
    LayerElement *ReadMeiClef( TiXmlElement *clef );
    LayerElement *ReadMeiMensur( TiXmlElement *mensur );
    LayerElement *ReadMeiMultiRest( TiXmlElement *multiRest );
    LayerElement *ReadMeiNote( TiXmlElement *note );
    LayerElement *ReadMeiRest( TiXmlElement *rest );
    LayerElement *ReadMeiTuplet( TiXmlElement *tuplet );
    LayerElement *ReadMeiAccid( TiXmlElement *accid );
    LayerElement *ReadMeiCustos( TiXmlElement *custos );
    LayerElement *ReadMeiDot( TiXmlElement *dot );
    /** Reads <app> elements. For now, only <app> within <layer> are taken into account */
    LayerElement *ReadMeiApp( TiXmlElement *app );
    bool ReadMeiRdg( TiXmlElement *rdg );
    
    /**
     * Read a sameAs attribute
     * The method has to be called by classed that support it (e.g., LayerElement)
     */
    void ReadSameAsAttr( TiXmlElement *element, Object *object );
    
    /**
     * Add the LayerElement to the appropriate parent (e.g., Layer, LayerRdg)
     */
    void AddLayerElement( LayerElement *element );
    
    /**
     * Read unsupported element and try to convert them
     */
    bool ReadUnsupported( TiXmlElement *element );
    
    /**
     * Look through the list of notes with open tie stored in MeiInput::m_openTies.
     * The note has to be on the same staff (@n) and the same layer (@n) and
     * have the same pitch. If found, the terminal attribute is the and the note
     * is removed from the list
     */
    bool FindOpenTie( Note *terminalNote );
    
	//
    void SetMeiUuid( TiXmlElement *element, Object *object );
    bool StrToBool(std::string value);
	int StrToDur(std::string dur);
	int StrToOct(std::string oct);
	int StrToPitch(std::string pitch ); 
    unsigned char StrToAccid(std::string accid);
    ClefId StrToClef(std::string shape, std::string line);
    MensurSign StrToMensurSign(std::string sign);
    DocType StrToDocType(std::string type);
    unsigned char StrToKeySigType(std::string accid);
    int StrToKeySigNum(std::string accid);
    BarlineType StrToBarlineType(std::string type);
    StaffGrpSymbol StrToStaffGrpSymbol(std::string sign);
    
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
