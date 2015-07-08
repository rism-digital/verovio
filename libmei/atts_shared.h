/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei 
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////// 
// NOTE: this file was generated with the Verovio libmei version and 
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATTS_SHARED_H__
#define __VRV_ATTS_SHARED_H__

#include "att.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAccidLog
//----------------------------------------------------------------------------

class AttAccidLog: public Att
{
public:
    AttAccidLog();
    virtual ~AttAccidLog();
    
    /** Reset the default values for the attribute class **/
    void ResetAccidLog();
    
    /** Read the values for the attribute class **/
    bool ReadAccidLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteAccidLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetFunc(std::string func_) { m_func = func_; };
    std::string GetFunc() const { return m_func; };    
    bool HasFunc( );
    
    ///@}

private:
    /** Records the function of an accidental. **/
    std::string m_func;

/* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttAccidental
//----------------------------------------------------------------------------

class AttAccidental: public Att
{
public:
    AttAccidental();
    virtual ~AttAccidental();
    
    /** Reset the default values for the attribute class **/
    void ResetAccidental();
    
    /** Read the values for the attribute class **/
    bool ReadAccidental( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteAccidental( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetAccid(data_ACCIDENTAL_EXPLICIT accid_) { m_accid = accid_; };
    data_ACCIDENTAL_EXPLICIT GetAccid() const { return m_accid; };    
    bool HasAccid( );
    
    ///@}

private:
    /** Captures a written accidental. **/
    data_ACCIDENTAL_EXPLICIT m_accid;

/* include <attaccid> */
};

//----------------------------------------------------------------------------
// AttAccidentalPerformed
//----------------------------------------------------------------------------

class AttAccidentalPerformed: public Att
{
public:
    AttAccidentalPerformed();
    virtual ~AttAccidentalPerformed();
    
    /** Reset the default values for the attribute class **/
    void ResetAccidentalPerformed();
    
    /** Read the values for the attribute class **/
    bool ReadAccidentalPerformed( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteAccidentalPerformed( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetAccidGes(std::string accidGes_) { m_accidGes = accidGes_; };
    std::string GetAccidGes() const { return m_accidGes; };    
    bool HasAccidGes( );
    
    ///@}

private:
    /**
     * Records the performed pitch inflection when it differs from the written
     * accidental.
     **/
    std::string m_accidGes;

/* include <attaccid.ges> */
};

//----------------------------------------------------------------------------
// AttAltsym
//----------------------------------------------------------------------------

class AttAltsym: public Att
{
public:
    AttAltsym();
    virtual ~AttAltsym();
    
    /** Reset the default values for the attribute class **/
    void ResetAltsym();
    
    /** Read the values for the attribute class **/
    bool ReadAltsym( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteAltsym( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetAltsym(std::string altsym_) { m_altsym = altsym_; };
    std::string GetAltsym() const { return m_altsym; };    
    bool HasAltsym( );
    
    ///@}

private:
    /**
     * Provides a way of pointing to a user-defined symbol.
     * It must contain an ID of a <symbolDef> element elsewhere in the document.
     **/
    std::string m_altsym;

/* include <attaltsym> */
};

//----------------------------------------------------------------------------
// AttArticulation
//----------------------------------------------------------------------------

class AttArticulation: public Att
{
public:
    AttArticulation();
    virtual ~AttArticulation();
    
    /** Reset the default values for the attribute class **/
    void ResetArticulation();
    
    /** Read the values for the attribute class **/
    bool ReadArticulation( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteArticulation( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetArtic(std::string artic_) { m_artic = artic_; };
    std::string GetArtic() const { return m_artic; };    
    bool HasArtic( );
    
    ///@}

private:
    /**
     * Encodes the written articulation(s).
     * Articulations are normally encoded in order from the note head outward; that is,
     * away from the stem. See additional notes at att.vis.note. Only articulations
     * should be encoded in the artic attribute; fingerings should be encoded using the
     * <dir> element.
     **/
    std::string m_artic;

/* include <attartic> */
};

//----------------------------------------------------------------------------
// AttArticulationPerformed
//----------------------------------------------------------------------------

class AttArticulationPerformed: public Att
{
public:
    AttArticulationPerformed();
    virtual ~AttArticulationPerformed();
    
    /** Reset the default values for the attribute class **/
    void ResetArticulationPerformed();
    
    /** Read the values for the attribute class **/
    bool ReadArticulationPerformed( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteArticulationPerformed( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetArticGes(std::string articGes_) { m_articGes = articGes_; };
    std::string GetArticGes() const { return m_articGes; };    
    bool HasArticGes( );
    
    ///@}

private:
    /** Records performed articulation that differs from the written value. **/
    std::string m_articGes;

/* include <attartic.ges> */
};

//----------------------------------------------------------------------------
// AttAugmentdots
//----------------------------------------------------------------------------

class AttAugmentdots: public Att
{
public:
    AttAugmentdots();
    virtual ~AttAugmentdots();
    
    /** Reset the default values for the attribute class **/
    void ResetAugmentdots();
    
    /** Read the values for the attribute class **/
    bool ReadAugmentdots( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteAugmentdots( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDots(char dots_) { m_dots = dots_; };
    char GetDots() const { return m_dots; };    
    bool HasDots( );
    
    ///@}

private:
    /** Records the number of augmentation dots required by a dotted duration. **/
    char m_dots;

/* include <attdots> */
};

//----------------------------------------------------------------------------
// AttAuthorized
//----------------------------------------------------------------------------

class AttAuthorized: public Att
{
public:
    AttAuthorized();
    virtual ~AttAuthorized();
    
    /** Reset the default values for the attribute class **/
    void ResetAuthorized();
    
    /** Read the values for the attribute class **/
    bool ReadAuthorized( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteAuthorized( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetAuthority(std::string authority_) { m_authority = authority_; };
    std::string GetAuthority() const { return m_authority; };    
    bool HasAuthority( );
    
    //
    void SetAuthURI(std::string authURI_) { m_authURI = authURI_; };
    std::string GetAuthURI() const { return m_authURI; };    
    bool HasAuthURI( );
    
    ///@}

private:
    /**
     * A name or label associated with the controlled vocabulary from which the value
     * is taken.
     **/
    std::string m_authority;
    /**
     * The web-accessible location of the controlled vocabulary from which the value is
     * taken.
     **/
    std::string m_authURI;

/* include <attauthURI> */
};

//----------------------------------------------------------------------------
// AttBarLineLog
//----------------------------------------------------------------------------

class AttBarLineLog: public Att
{
public:
    AttBarLineLog();
    virtual ~AttBarLineLog();
    
    /** Reset the default values for the attribute class **/
    void ResetBarLineLog();
    
    /** Read the values for the attribute class **/
    bool ReadBarLineLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBarLineLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetRend(data_BARRENDITION rend_) { m_rend = rend_; };
    data_BARRENDITION GetRend() const { return m_rend; };    
    bool HasRend( );
    
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    data_BARRENDITION m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttBarplacement
//----------------------------------------------------------------------------

class AttBarplacement: public Att
{
public:
    AttBarplacement();
    virtual ~AttBarplacement();
    
    /** Reset the default values for the attribute class **/
    void ResetBarplacement();
    
    /** Read the values for the attribute class **/
    bool ReadBarplacement( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBarplacement( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetBarplace(std::string barplace_) { m_barplace = barplace_; };
    std::string GetBarplace() const { return m_barplace; };    
    bool HasBarplace( );
    
    //
    void SetTaktplace(std::string taktplace_) { m_taktplace = taktplace_; };
    std::string GetTaktplace() const { return m_taktplace; };    
    bool HasTaktplace( );
    
    ///@}

private:
    /** Records the location of a bar line. **/
    std::string m_barplace;
    /**
     * If takt bar lines are to be used, then the taktplace attribute may be used to
     * denote the staff location of the shortened bar line.
     * The location may include staff lines, spaces, and the spaces directly above and
     * below the staff. The value ranges between 0 (just below the staff) to 2 * number
     * of staff lines (directly above the staff). For example, on a 5-line staff the
     * lines would be numbered 1,3,5,7, and 9 while the spaces would be numbered
     * 0,2,4,6,8,10. For example, a value of '9' puts the bar line through the top line
     * of a 5-line staff.
     **/
    std::string m_taktplace;

/* include <atttaktplace> */
};

//----------------------------------------------------------------------------
// AttBeamingVis
//----------------------------------------------------------------------------

class AttBeamingVis: public Att
{
public:
    AttBeamingVis();
    virtual ~AttBeamingVis();
    
    /** Reset the default values for the attribute class **/
    void ResetBeamingVis();
    
    /** Read the values for the attribute class **/
    bool ReadBeamingVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBeamingVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetBeamColor(std::string beamColor_) { m_beamColor = beamColor_; };
    std::string GetBeamColor() const { return m_beamColor; };    
    bool HasBeamColor( );
    
    //
    void SetBeamRend(std::string beamRend_) { m_beamRend = beamRend_; };
    std::string GetBeamRend() const { return m_beamRend; };    
    bool HasBeamRend( );
    
    //
    void SetBeamSlope(double beamSlopeDbl_) { m_beamSlopeDbl = beamSlopeDbl_; };
    double GetBeamSlope() const { return m_beamSlopeDbl; };    
    bool HasBeamSlope( );
    
    ///@}

private:
    /** Color of beams, including those associated with tuplets. **/
    std::string m_beamColor;
    /** Encodes whether a beam is "feathered" and in which direction. **/
    std::string m_beamRend;
    /** Captures beam slope. **/
    double m_beamSlopeDbl;

/* include <attbeam.slope> */
};

//----------------------------------------------------------------------------
// AttBibl
//----------------------------------------------------------------------------

class AttBibl: public Att
{
public:
    AttBibl();
    virtual ~AttBibl();
    
    /** Reset the default values for the attribute class **/
    void ResetBibl();
    
    /** Read the values for the attribute class **/
    bool ReadBibl( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBibl( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetAnalog(std::string analog_) { m_analog = analog_; };
    std::string GetAnalog() const { return m_analog; };    
    bool HasAnalog( );
    
    ///@}

private:
    /**
     * Contains a reference to a field or element in another descriptive encoding
     * system to which this MEI element is comparable.
     **/
    std::string m_analog;

/* include <attanalog> */
};

//----------------------------------------------------------------------------
// AttCalendared
//----------------------------------------------------------------------------

class AttCalendared: public Att
{
public:
    AttCalendared();
    virtual ~AttCalendared();
    
    /** Reset the default values for the attribute class **/
    void ResetCalendared();
    
    /** Read the values for the attribute class **/
    bool ReadCalendared( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCalendared( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCalendar(std::string calendar_) { m_calendar = calendar_; };
    std::string GetCalendar() const { return m_calendar; };    
    bool HasCalendar( );
    
    ///@}

private:
    /**
     * Indicates the calendar system to which a date belongs, for example, Gregorian,
     * Julian, Roman, Mosaic, Revolutionary, Islamic, etc.
     **/
    std::string m_calendar;

/* include <attcalendar> */
};

//----------------------------------------------------------------------------
// AttCanonical
//----------------------------------------------------------------------------

class AttCanonical: public Att
{
public:
    AttCanonical();
    virtual ~AttCanonical();
    
    /** Reset the default values for the attribute class **/
    void ResetCanonical();
    
    /** Read the values for the attribute class **/
    bool ReadCanonical( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCanonical( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDbkey(std::string dbkey_) { m_dbkey = dbkey_; };
    std::string GetDbkey() const { return m_dbkey; };    
    bool HasDbkey( );
    
    ///@}

private:
    /** Used to record a value which serves as a primary key in an external database. **/
    std::string m_dbkey;

/* include <attdbkey> */
};

//----------------------------------------------------------------------------
// AttChordVis
//----------------------------------------------------------------------------

class AttChordVis: public Att
{
public:
    AttChordVis();
    virtual ~AttChordVis();
    
    /** Reset the default values for the attribute class **/
    void ResetChordVis();
    
    /** Read the values for the attribute class **/
    bool ReadChordVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteChordVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCluster(std::string cluster_) { m_cluster = cluster_; };
    std::string GetCluster() const { return m_cluster; };    
    bool HasCluster( );
    
    ///@}

private:
    /**
     * Indicates a single alternative note head should be displayed instead of
     * individual note heads.
     * See Read, p. 320-321, re: tone clusters.
     **/
    std::string m_cluster;

/* include <attcluster> */
};

//----------------------------------------------------------------------------
// AttClefLog
//----------------------------------------------------------------------------

class AttClefLog: public Att
{
public:
    AttClefLog();
    virtual ~AttClefLog();
    
    /** Reset the default values for the attribute class **/
    void ResetClefLog();
    
    /** Read the values for the attribute class **/
    bool ReadClefLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteClefLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCautionary(std::string cautionary_) { m_cautionary = cautionary_; };
    std::string GetCautionary() const { return m_cautionary; };    
    bool HasCautionary( );
    
    ///@}

private:
    /**
     * Records the function of the clef.
     * A "cautionary" clef does not change the following pitches.
     **/
    std::string m_cautionary;

/* include <attcautionary> */
};

//----------------------------------------------------------------------------
// AttCleffingLog
//----------------------------------------------------------------------------

class AttCleffingLog: public Att
{
public:
    AttCleffingLog();
    virtual ~AttCleffingLog();
    
    /** Reset the default values for the attribute class **/
    void ResetCleffingLog();
    
    /** Read the values for the attribute class **/
    bool ReadCleffingLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCleffingLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetClefShape(data_CLEFSHAPE clefShape_) { m_clefShape = clefShape_; };
    data_CLEFSHAPE GetClefShape() const { return m_clefShape; };    
    bool HasClefShape( );
    
    //
    void SetClefLine(char clefLine_) { m_clefLine = clefLine_; };
    char GetClefLine() const { return m_clefLine; };    
    bool HasClefLine( );
    
    //
    void SetClefDis(data_OCTAVE_DIS clefDis_) { m_clefDis = clefDis_; };
    data_OCTAVE_DIS GetClefDis() const { return m_clefDis; };    
    bool HasClefDis( );
    
    //
    void SetClefDisPlace(data_PLACE clefDisPlace_) { m_clefDisPlace = clefDisPlace_; };
    data_PLACE GetClefDisPlace() const { return m_clefDisPlace; };    
    bool HasClefDisPlace( );
    
    ///@}

private:
    /** Encodes a value for the clef symbol. **/
    data_CLEFSHAPE m_clefShape;
    /**
     * Contains a default value for the position of the clef.
     * The value must be in the range between 1 and the number of lines on the staff.
     * The numbering of lines starts with the lowest line of the staff.
     **/
    char m_clefLine;
    /** Records the amount of octave displacement to be applied to the clef. **/
    data_OCTAVE_DIS m_clefDis;
    /** Records the direction of octave displacement to be applied to the clef. **/
    data_PLACE m_clefDisPlace;

/* include <attclef.dis.place> */
};

//----------------------------------------------------------------------------
// AttCleffingVis
//----------------------------------------------------------------------------

class AttCleffingVis: public Att
{
public:
    AttCleffingVis();
    virtual ~AttCleffingVis();
    
    /** Reset the default values for the attribute class **/
    void ResetCleffingVis();
    
    /** Read the values for the attribute class **/
    bool ReadCleffingVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCleffingVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetClefColor(std::string clefColor_) { m_clefColor = clefColor_; };
    std::string GetClefColor() const { return m_clefColor; };    
    bool HasClefColor( );
    
    //
    void SetClefVisible(std::string clefVisible_) { m_clefVisible = clefVisible_; };
    std::string GetClefVisible() const { return m_clefVisible; };    
    bool HasClefVisible( );
    
    ///@}

private:
    /** Describes the color of the clef. **/
    std::string m_clefColor;
    /** Determines whether the clef is to be displayed. **/
    std::string m_clefVisible;

/* include <attclef.visible> */
};

//----------------------------------------------------------------------------
// AttClefshape
//----------------------------------------------------------------------------

class AttClefshape: public Att
{
public:
    AttClefshape();
    virtual ~AttClefshape();
    
    /** Reset the default values for the attribute class **/
    void ResetClefshape();
    
    /** Read the values for the attribute class **/
    bool ReadClefshape( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteClefshape( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetShape(data_CLEFSHAPE shape_) { m_shape = shape_; };
    data_CLEFSHAPE GetShape() const { return m_shape; };    
    bool HasShape( );
    
    ///@}

private:
    /** Describes a clef's shape. **/
    data_CLEFSHAPE m_shape;

/* include <attshape> */
};

//----------------------------------------------------------------------------
// AttCoded
//----------------------------------------------------------------------------

class AttCoded: public Att
{
public:
    AttCoded();
    virtual ~AttCoded();
    
    /** Reset the default values for the attribute class **/
    void ResetCoded();
    
    /** Read the values for the attribute class **/
    bool ReadCoded( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCoded( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCode(std::string code_) { m_code = code_; };
    std::string GetCode() const { return m_code; };    
    bool HasCode( );
    
    ///@}

private:
    /** Captures one or more coded values for the textual content of this element. **/
    std::string m_code;

/* include <attcode> */
};

//----------------------------------------------------------------------------
// AttColor
//----------------------------------------------------------------------------

class AttColor: public Att
{
public:
    AttColor();
    virtual ~AttColor();
    
    /** Reset the default values for the attribute class **/
    void ResetColor();
    
    /** Read the values for the attribute class **/
    bool ReadColor( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteColor( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetColor(std::string color_) { m_color = color_; };
    std::string GetColor() const { return m_color; };    
    bool HasColor( );
    
    ///@}

private:
    /**
     * Used to indicate visual appearance.
     * Do not confuse this with the musical term 'color' as used in pre-CMN notation.
     **/
    std::string m_color;

/* include <attcolor> */
};

//----------------------------------------------------------------------------
// AttColoration
//----------------------------------------------------------------------------

class AttColoration: public Att
{
public:
    AttColoration();
    virtual ~AttColoration();
    
    /** Reset the default values for the attribute class **/
    void ResetColoration();
    
    /** Read the values for the attribute class **/
    bool ReadColoration( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteColoration( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetColored(data_BOOLEAN colored_) { m_colored = colored_; };
    data_BOOLEAN GetColored() const { return m_colored; };    
    bool HasColored( );
    
    ///@}

private:
    /**
     * Indicates this feature is 'colored'; that is, it is a participant in a change in
     * rhythmic values.
     * In mensural notation, coloration is indicated by colored notes (red, black,
     * etc.) where void notes would otherwise occur. In CMN, coloration is indicated by
     * an inverse color; that is, the note head is void when it would otherwise be
     * filled and vice versa.
     **/
    data_BOOLEAN m_colored;

/* include <attcolored> */
};

//----------------------------------------------------------------------------
// AttCommon
//----------------------------------------------------------------------------

class AttCommon: public Att
{
public:
    AttCommon();
    virtual ~AttCommon();
    
    /** Reset the default values for the attribute class **/
    void ResetCommon();
    
    /** Read the values for the attribute class **/
    bool ReadCommon( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCommon( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLabel(std::string label_) { m_label = label_; };
    std::string GetLabel() const { return m_label; };    
    bool HasLabel( );
    
    //
    void SetN(int n_) { m_n = n_; };
    int GetN() const { return m_n; };    
    bool HasN( );
    
    //
    void SetBase(std::string base_) { m_base = base_; };
    std::string GetBase() const { return m_base; };    
    bool HasBase( );
    
    ///@}

private:
    /**
     * Provides a label for an element.
     * The value may be any string.
     **/
    std::string m_label;
    /**
     * Provides a name or number designation for an element.
     * While the value need not be unique, it is required to be a single token.
     **/
    int m_n;
    /** --- **/
    std::string m_base;

/* include <attbase> */
};

//----------------------------------------------------------------------------
// AttCoordinated
//----------------------------------------------------------------------------

class AttCoordinated: public Att
{
public:
    AttCoordinated();
    virtual ~AttCoordinated();
    
    /** Reset the default values for the attribute class **/
    void ResetCoordinated();
    
    /** Read the values for the attribute class **/
    bool ReadCoordinated( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCoordinated( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetUlx(int ulxInt_) { m_ulxInt = ulxInt_; };
    int GetUlx() const { return m_ulxInt; };    
    bool HasUlx( );
    
    //
    void SetUly(int ulyInt_) { m_ulyInt = ulyInt_; };
    int GetUly() const { return m_ulyInt; };    
    bool HasUly( );
    
    //
    void SetLrx(int lrxInt_) { m_lrxInt = lrxInt_; };
    int GetLrx() const { return m_lrxInt; };    
    bool HasLrx( );
    
    //
    void SetLry(int lryInt_) { m_lryInt = lryInt_; };
    int GetLry() const { return m_lryInt; };    
    bool HasLry( );
    
    ///@}

private:
    /** Indicates the upper-left corner x coordinate. **/
    int m_ulxInt;
    /** Indicates the upper-left corner y coordinate. **/
    int m_ulyInt;
    /** Indicates the lower-right corner x coordinate. **/
    int m_lrxInt;
    /** Indicates the lower-left corner x coordinate. **/
    int m_lryInt;

/* include <attlry> */
};

//----------------------------------------------------------------------------
// AttCurvature
//----------------------------------------------------------------------------

class AttCurvature: public Att
{
public:
    AttCurvature();
    virtual ~AttCurvature();
    
    /** Reset the default values for the attribute class **/
    void ResetCurvature();
    
    /** Read the values for the attribute class **/
    bool ReadCurvature( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCurvature( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetBezier(std::string bezier_) { m_bezier = bezier_; };
    std::string GetBezier() const { return m_bezier; };    
    bool HasBezier( );
    
    //
    void SetBulge(std::string bulge_) { m_bulge = bulge_; };
    std::string GetBulge() const { return m_bulge; };    
    bool HasBulge( );
    
    //
    void SetCurvedir(std::string curvedir_) { m_curvedir = curvedir_; };
    std::string GetCurvedir() const { return m_curvedir; };    
    bool HasCurvedir( );
    
    ///@}

private:
    /**
     * Records the placement of Bezier control points as a series of space-separated xy
     * coordinates, e.g., 19 45 -32 118.
     **/
    std::string m_bezier;
    /**
     * Describes a curve as a set of distance values above or below an imaginary line
     * connecting the endpoints of the curve.
     * The bulge attribute must contain one or more decimal values expressed in inter-
     * line units.
     **/
    std::string m_bulge;
    /** Describes a curve with a generic term indicating the direction of curvature. **/
    std::string m_curvedir;

/* include <attcurvedir> */
};

//----------------------------------------------------------------------------
// AttCurverend
//----------------------------------------------------------------------------

class AttCurverend: public Att
{
public:
    AttCurverend();
    virtual ~AttCurverend();
    
    /** Reset the default values for the attribute class **/
    void ResetCurverend();
    
    /** Read the values for the attribute class **/
    bool ReadCurverend( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCurverend( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetRend(data_BARRENDITION rend_) { m_rend = rend_; };
    data_BARRENDITION GetRend() const { return m_rend; };    
    bool HasRend( );
    
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    data_BARRENDITION m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttCustosLog
//----------------------------------------------------------------------------

class AttCustosLog: public Att
{
public:
    AttCustosLog();
    virtual ~AttCustosLog();
    
    /** Reset the default values for the attribute class **/
    void ResetCustosLog();
    
    /** Read the values for the attribute class **/
    bool ReadCustosLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCustosLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTarget(std::string target_) { m_target = target_; };
    std::string GetTarget() const { return m_target; };    
    bool HasTarget( );
    
    ///@}

private:
    /**
     * Encodes the target note when its pitch differs from the pitch at which the
     * custos appears.
     **/
    std::string m_target;

/* include <atttarget> */
};

//----------------------------------------------------------------------------
// AttDatable
//----------------------------------------------------------------------------

class AttDatable: public Att
{
public:
    AttDatable();
    virtual ~AttDatable();
    
    /** Reset the default values for the attribute class **/
    void ResetDatable();
    
    /** Read the values for the attribute class **/
    bool ReadDatable( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDatable( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetEnddate(std::string enddate_) { m_enddate = enddate_; };
    std::string GetEnddate() const { return m_enddate; };    
    bool HasEnddate( );
    
    //
    void SetIsodate(std::string isodate_) { m_isodate = isodate_; };
    std::string GetIsodate() const { return m_isodate; };    
    bool HasIsodate( );
    
    //
    void SetNotafter(std::string notafter_) { m_notafter = notafter_; };
    std::string GetNotafter() const { return m_notafter; };    
    bool HasNotafter( );
    
    //
    void SetNotbefore(std::string notbefore_) { m_notbefore = notbefore_; };
    std::string GetNotbefore() const { return m_notbefore; };    
    bool HasNotbefore( );
    
    //
    void SetStartdate(std::string startdate_) { m_startdate = startdate_; };
    std::string GetStartdate() const { return m_startdate; };    
    bool HasStartdate( );
    
    ///@}

private:
    /** Contains the end point of a date range in standard ISO form. **/
    std::string m_enddate;
    /** Provides the value of a textual date in standard ISO form. **/
    std::string m_isodate;
    /** Contains an upper boundary for an uncertain date in standard ISO form. **/
    std::string m_notafter;
    /** Contains a lower boundary, in standard ISO form, for an uncertain date. **/
    std::string m_notbefore;
    /** Contains the starting point of a date range in standard ISO form. **/
    std::string m_startdate;

/* include <attstartdate> */
};

//----------------------------------------------------------------------------
// AttDatapointing
//----------------------------------------------------------------------------

class AttDatapointing: public Att
{
public:
    AttDatapointing();
    virtual ~AttDatapointing();
    
    /** Reset the default values for the attribute class **/
    void ResetDatapointing();
    
    /** Read the values for the attribute class **/
    bool ReadDatapointing( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDatapointing( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetData(std::string data_) { m_data = data_; };
    std::string GetData() const { return m_data; };    
    bool HasData( );
    
    ///@}

private:
    /** Used to link metadata elements to one or more data-containing elements. **/
    std::string m_data;

/* include <attdata> */
};

//----------------------------------------------------------------------------
// AttDeclaring
//----------------------------------------------------------------------------

class AttDeclaring: public Att
{
public:
    AttDeclaring();
    virtual ~AttDeclaring();
    
    /** Reset the default values for the attribute class **/
    void ResetDeclaring();
    
    /** Read the values for the attribute class **/
    bool ReadDeclaring( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDeclaring( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDecls(std::string decls_) { m_decls = decls_; };
    std::string GetDecls() const { return m_decls; };    
    bool HasDecls( );
    
    ///@}

private:
    /**
     * Identifies one or more metadata elements within the header, which are understood
     * to apply to the element bearing this attribute and its content.
     **/
    std::string m_decls;

/* include <attdecls> */
};

//----------------------------------------------------------------------------
// AttDistances
//----------------------------------------------------------------------------

class AttDistances: public Att
{
public:
    AttDistances();
    virtual ~AttDistances();
    
    /** Reset the default values for the attribute class **/
    void ResetDistances();
    
    /** Read the values for the attribute class **/
    bool ReadDistances( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDistances( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDynamDist(std::string dynamDist_) { m_dynamDist = dynamDist_; };
    std::string GetDynamDist() const { return m_dynamDist; };    
    bool HasDynamDist( );
    
    //
    void SetHarmDist(std::string harmDist_) { m_harmDist = harmDist_; };
    std::string GetHarmDist() const { return m_harmDist; };    
    bool HasHarmDist( );
    
    //
    void SetTextDist(std::string textDist_) { m_textDist = textDist_; };
    std::string GetTextDist() const { return m_textDist; };    
    bool HasTextDist( );
    
    ///@}

private:
    /** Records the default distance from the staff for dynamic marks. **/
    std::string m_dynamDist;
    /**
     * Records the default distance from the staff of harmonic indications, such as
     * guitar chord grids or functional labels.
     **/
    std::string m_harmDist;
    /** Determines how far from the staff to render text elements. **/
    std::string m_textDist;

/* include <atttext.dist> */
};

//----------------------------------------------------------------------------
// AttDotLog
//----------------------------------------------------------------------------

class AttDotLog: public Att
{
public:
    AttDotLog();
    virtual ~AttDotLog();
    
    /** Reset the default values for the attribute class **/
    void ResetDotLog();
    
    /** Read the values for the attribute class **/
    bool ReadDotLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDotLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };    
    bool HasForm( );
    
    ///@}

private:
    /** Records the function of the dot. **/
    std::string m_form;

/* include <attform> */
};

//----------------------------------------------------------------------------
// AttDurationAdditive
//----------------------------------------------------------------------------

class AttDurationAdditive: public Att
{
public:
    AttDurationAdditive();
    virtual ~AttDurationAdditive();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationAdditive();
    
    /** Read the values for the attribute class **/
    bool ReadDurationAdditive( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDurationAdditive( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDur(data_DURATION dur_) { m_dur = dur_; };
    data_DURATION GetDur() const { return m_dur; };    
    bool HasDur( );
    
    ///@}

private:
    /**
     * Records duration using ident dotted, relative durational values provided by the
     * data.DURATION datatype.
     * When the duration is "irrational", as is sometimes the case with tuplets,
     * multiple space-separated values that add up to the total duration may be used.
     * When dotted values are present, the dots attribute must be ignored.
     **/
    data_DURATION m_dur;

/* include <attdur> */
};

//----------------------------------------------------------------------------
// AttDurationDefault
//----------------------------------------------------------------------------

class AttDurationDefault: public Att
{
public:
    AttDurationDefault();
    virtual ~AttDurationDefault();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationDefault();
    
    /** Read the values for the attribute class **/
    bool ReadDurationDefault( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDurationDefault( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDurDefault(std::string durDefault_) { m_durDefault = durDefault_; };
    std::string GetDurDefault() const { return m_durDefault; };    
    bool HasDurDefault( );
    
    //
    void SetNumDefault(int numDefaultInt_) { m_numDefaultInt = numDefaultInt_; };
    int GetNumDefault() const { return m_numDefaultInt; };    
    bool HasNumDefault( );
    
    //
    void SetNumbaseDefault(int numbaseDefaultInt_) { m_numbaseDefaultInt = numbaseDefaultInt_; };
    int GetNumbaseDefault() const { return m_numbaseDefaultInt; };    
    bool HasNumbaseDefault( );
    
    ///@}

private:
    /**
     * Contains a default duration in those situations when the first note, rest,
     * chord, etc.
     * in a measure does not have a duration specified.
     **/
    std::string m_durDefault;
    /**
     * Along with numbase.default, describes the default duration as a ratio.
     * num.default is the first value in the ratio.
     **/
    int m_numDefaultInt;
    /**
     * Along with num.default, describes the default duration as a ratio.
     * numbase.default is the second value in the ratio.
     **/
    int m_numbaseDefaultInt;

/* include <attnumbase.default> */
};

//----------------------------------------------------------------------------
// AttDurationMusical
//----------------------------------------------------------------------------

class AttDurationMusical: public Att
{
public:
    AttDurationMusical();
    virtual ~AttDurationMusical();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationMusical();
    
    /** Read the values for the attribute class **/
    bool ReadDurationMusical( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDurationMusical( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDur(data_DURATION dur_) { m_dur = dur_; };
    data_DURATION GetDur() const { return m_dur; };    
    bool HasDur( );
    
    ///@}

private:
    /**
     * Records duration using ident dotted, relative durational values provided by the
     * data.DURATION datatype.
     * When the duration is "irrational", as is sometimes the case with tuplets,
     * multiple space-separated values that add up to the total duration may be used.
     * When dotted values are present, the dots attribute must be ignored.
     **/
    data_DURATION m_dur;

/* include <attdur> */
};

//----------------------------------------------------------------------------
// AttDurationPerformed
//----------------------------------------------------------------------------

class AttDurationPerformed: public Att
{
public:
    AttDurationPerformed();
    virtual ~AttDurationPerformed();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationPerformed();
    
    /** Read the values for the attribute class **/
    bool ReadDurationPerformed( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDurationPerformed( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDurGes(data_DURATION durGes_) { m_durGes = durGes_; };
    data_DURATION GetDurGes() const { return m_durGes; };    
    bool HasDurGes( );
    
    ///@}

private:
    /**
     * Records performed duration information that differs from the written duration.
     * Its value may be expressed in several forms; that is, ppq (MIDI clicks and
     * MusicXML 'divisions'), Humdrum **recip values, beats, seconds, or mensural
     * duration values.
     **/
    data_DURATION m_durGes;

/* include <attdur.ges> */
};

//----------------------------------------------------------------------------
// AttDurationRatio
//----------------------------------------------------------------------------

class AttDurationRatio: public Att
{
public:
    AttDurationRatio();
    virtual ~AttDurationRatio();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationRatio();
    
    /** Read the values for the attribute class **/
    bool ReadDurationRatio( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteDurationRatio( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetNum(int num_) { m_num = num_; };
    int GetNum() const { return m_num; };    
    bool HasNum( );
    
    //
    void SetNumbase(int numbase_) { m_numbase = numbase_; };
    int GetNumbase() const { return m_numbase; };    
    bool HasNumbase( );
    
    ///@}

private:
    /**
     * Along with numbase, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_num;
    /**
     * Along with num, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_numbase;

/* include <attnumbase> */
};

//----------------------------------------------------------------------------
// AttEnclosingchars
//----------------------------------------------------------------------------

class AttEnclosingchars: public Att
{
public:
    AttEnclosingchars();
    virtual ~AttEnclosingchars();
    
    /** Reset the default values for the attribute class **/
    void ResetEnclosingchars();
    
    /** Read the values for the attribute class **/
    bool ReadEnclosingchars( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteEnclosingchars( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetEnclose(std::string enclose_) { m_enclose = enclose_; };
    std::string GetEnclose() const { return m_enclose; };    
    bool HasEnclose( );
    
    ///@}

private:
    /**
     * Records the characters often used to mark accidentals, articulations, and
     * sometimes notes as having a cautionary or editorial function.
     * For an example of cautionary accidentals enclosed in parentheses, see Read, p.
     * 131, ex. 9-14.
     **/
    std::string m_enclose;

/* include <attenclose> */
};

//----------------------------------------------------------------------------
// AttFermatapresent
//----------------------------------------------------------------------------

class AttFermatapresent: public Att
{
public:
    AttFermatapresent();
    virtual ~AttFermatapresent();
    
    /** Reset the default values for the attribute class **/
    void ResetFermatapresent();
    
    /** Read the values for the attribute class **/
    bool ReadFermatapresent( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteFermatapresent( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetFermata(data_PLACE fermata_) { m_fermata = fermata_; };
    data_PLACE GetFermata() const { return m_fermata; };    
    bool HasFermata( );
    
    ///@}

private:
    /**
     * Indicates the attachment of a fermata to this element.
     * If visual information about the fermata needs to be recorded, then a <fermata>
     * element should be employed instead.
     **/
    data_PLACE m_fermata;

/* include <attfermata> */
};

//----------------------------------------------------------------------------
// AttHandident
//----------------------------------------------------------------------------

class AttHandident: public Att
{
public:
    AttHandident();
    virtual ~AttHandident();
    
    /** Reset the default values for the attribute class **/
    void ResetHandident();
    
    /** Read the values for the attribute class **/
    bool ReadHandident( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteHandident( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetHand(std::string hand_) { m_hand = hand_; };
    std::string GetHand() const { return m_hand; };    
    bool HasHand( );
    
    ///@}

private:
    /**
     * Signifies the hand responsible for an action.
     * The value must be the ID of a <hand> element declared in the header.
     **/
    std::string m_hand;

/* include <atthand> */
};

//----------------------------------------------------------------------------
// AttHorizontalalign
//----------------------------------------------------------------------------

class AttHorizontalalign: public Att
{
public:
    AttHorizontalalign();
    virtual ~AttHorizontalalign();
    
    /** Reset the default values for the attribute class **/
    void ResetHorizontalalign();
    
    /** Read the values for the attribute class **/
    bool ReadHorizontalalign( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteHorizontalalign( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetHalign(std::string halign_) { m_halign = halign_; };
    std::string GetHalign() const { return m_halign; };    
    bool HasHalign( );
    
    ///@}

private:
    /** Records horizontal alignment. **/
    std::string m_halign;

/* include <atthalign> */
};

//----------------------------------------------------------------------------
// AttInstrumentident
//----------------------------------------------------------------------------

class AttInstrumentident: public Att
{
public:
    AttInstrumentident();
    virtual ~AttInstrumentident();
    
    /** Reset the default values for the attribute class **/
    void ResetInstrumentident();
    
    /** Read the values for the attribute class **/
    bool ReadInstrumentident( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteInstrumentident( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetInstr(std::string instr_) { m_instr = instr_; };
    std::string GetInstr() const { return m_instr; };    
    bool HasInstr( );
    
    ///@}

private:
    /**
     * Provides a way of pointing to a MIDI instrument definition.
     * It must contain the ID of an <instrDef> element elsewhere in the document.
     **/
    std::string m_instr;

/* include <attinstr> */
};

//----------------------------------------------------------------------------
// AttInternetmedia
//----------------------------------------------------------------------------

class AttInternetmedia: public Att
{
public:
    AttInternetmedia();
    virtual ~AttInternetmedia();
    
    /** Reset the default values for the attribute class **/
    void ResetInternetmedia();
    
    /** Read the values for the attribute class **/
    bool ReadInternetmedia( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteInternetmedia( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMimetype(std::string mimetype_) { m_mimetype = mimetype_; };
    std::string GetMimetype() const { return m_mimetype; };    
    bool HasMimetype( );
    
    ///@}

private:
    /**
     * Specifies the applicable MIME (multimedia internet mail extension) type.
     * The value should be a valid MIME media type defined by the Internet Engineering
     * Task Force in RFC 2046.
     **/
    std::string m_mimetype;

/* include <attmimetype> */
};

//----------------------------------------------------------------------------
// AttJoined
//----------------------------------------------------------------------------

class AttJoined: public Att
{
public:
    AttJoined();
    virtual ~AttJoined();
    
    /** Reset the default values for the attribute class **/
    void ResetJoined();
    
    /** Read the values for the attribute class **/
    bool ReadJoined( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteJoined( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetJoin(std::string join_) { m_join = join_; };
    std::string GetJoin() const { return m_join; };    
    bool HasJoin( );
    
    ///@}

private:
    /**
     * Used for linking visually separate entities that form a single logical entity,
     * for example, multiple slurs broken across a system break that form a single
     * musical phrase.
     * Also used to indicate a measure which metrically completes the current one.
     * Record the identifiers of the separately encoded components, excluding the one
     * carrying the attribute.
     **/
    std::string m_join;

/* include <attjoin> */
};

//----------------------------------------------------------------------------
// AttKeySigLog
//----------------------------------------------------------------------------

class AttKeySigLog: public Att
{
public:
    AttKeySigLog();
    virtual ~AttKeySigLog();
    
    /** Reset the default values for the attribute class **/
    void ResetKeySigLog();
    
    /** Read the values for the attribute class **/
    bool ReadKeySigLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteKeySigLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMode(std::string mode_) { m_mode = mode_; };
    std::string GetMode() const { return m_mode; };    
    bool HasMode( );
    
    ///@}

private:
    /** Indicates major, minor, or other tonality. **/
    std::string m_mode;

/* include <attmode> */
};

//----------------------------------------------------------------------------
// AttKeySigDefaultLog
//----------------------------------------------------------------------------

class AttKeySigDefaultLog: public Att
{
public:
    AttKeySigDefaultLog();
    virtual ~AttKeySigDefaultLog();
    
    /** Reset the default values for the attribute class **/
    void ResetKeySigDefaultLog();
    
    /** Read the values for the attribute class **/
    bool ReadKeySigDefaultLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteKeySigDefaultLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetKeyAccid(data_ACCIDENTAL_IMPLICIT keyAccid_) { m_keyAccid = keyAccid_; };
    data_ACCIDENTAL_IMPLICIT GetKeyAccid() const { return m_keyAccid; };    
    bool HasKeyAccid( );
    
    //
    void SetKeyMode(data_MODE keyMode_) { m_keyMode = keyMode_; };
    data_MODE GetKeyMode() const { return m_keyMode; };    
    bool HasKeyMode( );
    
    //
    void SetKeyPname(data_PITCHNAME keyPname_) { m_keyPname = keyPname_; };
    data_PITCHNAME GetKeyPname() const { return m_keyPname; };    
    bool HasKeyPname( );
    
    //
    void SetKeySig(data_KEYSIGNATURE keySig_) { m_keySig = keySig_; };
    data_KEYSIGNATURE GetKeySig() const { return m_keySig; };    
    bool HasKeySig( );
    
    //
    void SetKeySigMixed(std::string keySigMixed_) { m_keySigMixed = keySigMixed_; };
    std::string GetKeySigMixed() const { return m_keySigMixed; };    
    bool HasKeySigMixed( );
    
    ///@}

private:
    /**
     * Contains an accidental for the tonic key, if one is required, e.g., if key.pname
     * equals 'c' and key.accid equals 's', then a tonic of C# is indicated.
     **/
    data_ACCIDENTAL_IMPLICIT m_keyAccid;
    /** Indicates major, minor, or other tonality. **/
    data_MODE m_keyMode;
    /**
     * Holds the pitch name of the tonic key, e.g.
     * 'c' for the key of C.
     **/
    data_PITCHNAME m_keyPname;
    /** Indicates where the key lies in the circle of fifths. **/
    data_KEYSIGNATURE m_keySig;
    /**
     * Mixed key signatures, e.g.
     * those consisting of a mixture of flats and sharps (Read, p. 143, ex. 9-39), and
     * key signatures with unorthodox placement of the accidentals (Read, p. 141) must
     * be indicated by setting the key.sig attribute to 'mixed' and providing explicit
     * key signature information in the key.sig.mixed attribute or in the <keySig>
     * element. It is intended that key.sig.mixed contain a series of tokens with each
     * token containing pitch name, accidental, and octave, such as 'A4 Cs5 Ef5' that
     * indicate what key accidentals should be rendered and where they should be
     * placed.
     **/
    std::string m_keySigMixed;

/* include <attkey.sig.mixed> */
};

//----------------------------------------------------------------------------
// AttKeySigDefaultVis
//----------------------------------------------------------------------------

class AttKeySigDefaultVis: public Att
{
public:
    AttKeySigDefaultVis();
    virtual ~AttKeySigDefaultVis();
    
    /** Reset the default values for the attribute class **/
    void ResetKeySigDefaultVis();
    
    /** Read the values for the attribute class **/
    bool ReadKeySigDefaultVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteKeySigDefaultVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetKeySigShow(std::string keySigShow_) { m_keySigShow = keySigShow_; };
    std::string GetKeySigShow() const { return m_keySigShow; };    
    bool HasKeySigShow( );
    
    //
    void SetKeySigShowchange(std::string keySigShowchange_) { m_keySigShowchange = keySigShowchange_; };
    std::string GetKeySigShowchange() const { return m_keySigShowchange; };    
    bool HasKeySigShowchange( );
    
    ///@}

private:
    /** Indicates whether the key signature should be displayed. **/
    std::string m_keySigShow;
    /** Determines whether cautionary accidentals should be displayed at a key change. **/
    std::string m_keySigShowchange;

/* include <attkey.sig.showchange> */
};

//----------------------------------------------------------------------------
// AttLabelsAddl
//----------------------------------------------------------------------------

class AttLabelsAddl: public Att
{
public:
    AttLabelsAddl();
    virtual ~AttLabelsAddl();
    
    /** Reset the default values for the attribute class **/
    void ResetLabelsAddl();
    
    /** Read the values for the attribute class **/
    bool ReadLabelsAddl( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLabelsAddl( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLabelAbbr(std::string labelAbbr_) { m_labelAbbr = labelAbbr_; };
    std::string GetLabelAbbr() const { return m_labelAbbr; };    
    bool HasLabelAbbr( );
    
    ///@}

private:
    /**
     * Provides a label for a group of staves on pages after the first page.
     * Usually, this label takes an abbreviated form.
     **/
    std::string m_labelAbbr;

/* include <attlabel.abbr> */
};

//----------------------------------------------------------------------------
// AttLang
//----------------------------------------------------------------------------

class AttLang: public Att
{
public:
    AttLang();
    virtual ~AttLang();
    
    /** Reset the default values for the attribute class **/
    void ResetLang();
    
    /** Read the values for the attribute class **/
    bool ReadLang( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLang( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLang(std::string lang_) { m_lang = lang_; };
    std::string GetLang() const { return m_lang; };    
    bool HasLang( );
    
    ///@}

private:
    /** --- **/
    std::string m_lang;

/* include <attlang> */
};

//----------------------------------------------------------------------------
// AttLayerLog
//----------------------------------------------------------------------------

class AttLayerLog: public Att
{
public:
    AttLayerLog();
    virtual ~AttLayerLog();
    
    /** Reset the default values for the attribute class **/
    void ResetLayerLog();
    
    /** Read the values for the attribute class **/
    bool ReadLayerLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLayerLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDef(std::string def_) { m_def = def_; };
    std::string GetDef() const { return m_def; };    
    bool HasDef( );
    
    ///@}

private:
    /** Provides a mechanism for linking the layer to a layerDef element. **/
    std::string m_def;

/* include <attdef> */
};

//----------------------------------------------------------------------------
// AttLayerident
//----------------------------------------------------------------------------

class AttLayerident: public Att
{
public:
    AttLayerident();
    virtual ~AttLayerident();
    
    /** Reset the default values for the attribute class **/
    void ResetLayerident();
    
    /** Read the values for the attribute class **/
    bool ReadLayerident( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLayerident( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLayer(std::string layer_) { m_layer = layer_; };
    std::string GetLayer() const { return m_layer; };    
    bool HasLayer( );
    
    ///@}

private:
    /** Identifies the layer to which a feature applies. **/
    std::string m_layer;

/* include <attlayer> */
};

//----------------------------------------------------------------------------
// AttLineloc
//----------------------------------------------------------------------------

class AttLineloc: public Att
{
public:
    AttLineloc();
    virtual ~AttLineloc();
    
    /** Reset the default values for the attribute class **/
    void ResetLineloc();
    
    /** Read the values for the attribute class **/
    bool ReadLineloc( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLineloc( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLine(char line_) { m_line = line_; };
    char GetLine() const { return m_line; };    
    bool HasLine( );
    
    ///@}

private:
    /**
     * Indicates the line upon which a feature stands.
     * The value must be in the range between 1 and the number of lines on the staff.
     * The numbering of lines starts with the lowest line of the staff.
     **/
    char m_line;

/* include <attline> */
};

//----------------------------------------------------------------------------
// AttLinerend
//----------------------------------------------------------------------------

class AttLinerend: public Att
{
public:
    AttLinerend();
    virtual ~AttLinerend();
    
    /** Reset the default values for the attribute class **/
    void ResetLinerend();
    
    /** Read the values for the attribute class **/
    bool ReadLinerend( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLinerend( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetRend(data_BARRENDITION rend_) { m_rend = rend_; };
    data_BARRENDITION GetRend() const { return m_rend; };    
    bool HasRend( );
    
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    data_BARRENDITION m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttLyricstyle
//----------------------------------------------------------------------------

class AttLyricstyle: public Att
{
public:
    AttLyricstyle();
    virtual ~AttLyricstyle();
    
    /** Reset the default values for the attribute class **/
    void ResetLyricstyle();
    
    /** Read the values for the attribute class **/
    bool ReadLyricstyle( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLyricstyle( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLyricAlign(std::string lyricAlign_) { m_lyricAlign = lyricAlign_; };
    std::string GetLyricAlign() const { return m_lyricAlign; };    
    bool HasLyricAlign( );
    
    //
    void SetLyricFam(std::string lyricFam_) { m_lyricFam = lyricFam_; };
    std::string GetLyricFam() const { return m_lyricFam; };    
    bool HasLyricFam( );
    
    //
    void SetLyricName(std::string lyricName_) { m_lyricName = lyricName_; };
    std::string GetLyricName() const { return m_lyricName; };    
    bool HasLyricName( );
    
    //
    void SetLyricSize(std::string lyricSize_) { m_lyricSize = lyricSize_; };
    std::string GetLyricSize() const { return m_lyricSize; };    
    bool HasLyricSize( );
    
    //
    void SetLyricStyle(std::string lyricStyle_) { m_lyricStyle = lyricStyle_; };
    std::string GetLyricStyle() const { return m_lyricStyle; };    
    bool HasLyricStyle( );
    
    //
    void SetLyricWeight(std::string lyricWeight_) { m_lyricWeight = lyricWeight_; };
    std::string GetLyricWeight() const { return m_lyricWeight; };    
    bool HasLyricWeight( );
    
    ///@}

private:
    /** Describes the alignment of lyric syllables associated with a note or chord. **/
    std::string m_lyricAlign;
    /** Sets the font family default value for lyrics. **/
    std::string m_lyricFam;
    /** Sets the font name default value for lyrics. **/
    std::string m_lyricName;
    /** Sets the default font size value for lyrics. **/
    std::string m_lyricSize;
    /** Sets the default font style value for lyrics. **/
    std::string m_lyricStyle;
    /** Sets the default font weight value for lyrics. **/
    std::string m_lyricWeight;

/* include <attlyric.weight> */
};

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

class AttMeasureLog: public Att
{
public:
    AttMeasureLog();
    virtual ~AttMeasureLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMeasureLog();
    
    /** Read the values for the attribute class **/
    bool ReadMeasureLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeasureLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLeft(data_BARRENDITION left_) { m_left = left_; };
    data_BARRENDITION GetLeft() const { return m_left; };    
    bool HasLeft( );
    
    //
    void SetRight(data_BARRENDITION right_) { m_right = right_; };
    data_BARRENDITION GetRight() const { return m_right; };    
    bool HasRight( );
    
    ///@}

private:
    /**
     * Indicates the visual rendition of the left bar line.
     * It is present here only for facilitation of translation from legacy encodings
     * which use it. Usually, it can be safely ignored.
     **/
    data_BARRENDITION m_left;
    /** Indicates the function of the right bar line and is structurally important. **/
    data_BARRENDITION m_right;

/* include <attright> */
};

//----------------------------------------------------------------------------
// AttMeasurement
//----------------------------------------------------------------------------

class AttMeasurement: public Att
{
public:
    AttMeasurement();
    virtual ~AttMeasurement();
    
    /** Reset the default values for the attribute class **/
    void ResetMeasurement();
    
    /** Read the values for the attribute class **/
    bool ReadMeasurement( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeasurement( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetUnit(int unit_) { m_unit = unit_; };
    int GetUnit() const { return m_unit; };    
    bool HasUnit( );
    
    ///@}

private:
    /** Indicates the unit used for a measurement of size. **/
    int m_unit;

/* include <attunit> */
};

//----------------------------------------------------------------------------
// AttMediabounds
//----------------------------------------------------------------------------

class AttMediabounds: public Att
{
public:
    AttMediabounds();
    virtual ~AttMediabounds();
    
    /** Reset the default values for the attribute class **/
    void ResetMediabounds();
    
    /** Read the values for the attribute class **/
    bool ReadMediabounds( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMediabounds( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetBegin(std::string begin_) { m_begin = begin_; };
    std::string GetBegin() const { return m_begin; };    
    bool HasBegin( );
    
    //
    void SetEnd(std::string end_) { m_end = end_; };
    std::string GetEnd() const { return m_end; };    
    bool HasEnd( );
    
    //
    void SetBetype(std::string betype_) { m_betype = betype_; };
    std::string GetBetype() const { return m_betype; };    
    bool HasBetype( );
    
    ///@}

private:
    /**
     * Specifies a point where the relevant content begins.
     * A numerical value must be less and a time value must be earlier than that in the
     * end attribute.
     **/
    std::string m_begin;
    /**
     * Specifies a point where the relevant content ends.
     * If not specified, the end of the content is assumed to be the end point. A
     * numerical value must be greater and a time value must be later than that in the
     * begin attribute.
     **/
    std::string m_end;
    /**
     * Type of values used in the begin/end attributes.
     * The begin and end attributes can only be interpreted meaningfully in conjunction
     * with this attribute.
     **/
    std::string m_betype;

/* include <attbetype> */
};

//----------------------------------------------------------------------------
// AttMedium
//----------------------------------------------------------------------------

class AttMedium: public Att
{
public:
    AttMedium();
    virtual ~AttMedium();
    
    /** Reset the default values for the attribute class **/
    void ResetMedium();
    
    /** Read the values for the attribute class **/
    bool ReadMedium( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMedium( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMedium(std::string medium_) { m_medium = medium_; };
    std::string GetMedium() const { return m_medium; };    
    bool HasMedium( );
    
    ///@}

private:
    /** Describes the writing medium. **/
    std::string m_medium;

/* include <attmedium> */
};

//----------------------------------------------------------------------------
// AttMeiversion
//----------------------------------------------------------------------------

class AttMeiversion: public Att
{
public:
    AttMeiversion();
    virtual ~AttMeiversion();
    
    /** Reset the default values for the attribute class **/
    void ResetMeiversion();
    
    /** Read the values for the attribute class **/
    bool ReadMeiversion( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeiversion( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMeiversion(std::string meiversion_) { m_meiversion = meiversion_; };
    std::string GetMeiversion() const { return m_meiversion; };    
    bool HasMeiversion( );
    
    //
    void SetMeiversionNum(std::string meiversionNum_) { m_meiversionNum = meiversionNum_; };
    std::string GetMeiversionNum() const { return m_meiversionNum; };    
    bool HasMeiversionNum( );
    
    ///@}

private:
    /** Specifies a generic MEI version label. **/
    std::string m_meiversion;
    /** Records a detailed MEI version number. **/
    std::string m_meiversionNum;

/* include <attmeiversion.num> */
};

//----------------------------------------------------------------------------
// AttMensurLog
//----------------------------------------------------------------------------

class AttMensurLog: public Att
{
public:
    AttMensurLog();
    virtual ~AttMensurLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMensurLog();
    
    /** Read the values for the attribute class **/
    bool ReadMensurLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMensurLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDot(data_BOOLEAN dot_) { m_dot = dot_; };
    data_BOOLEAN GetDot() const { return m_dot; };    
    bool HasDot( );
    
    //
    void SetSign(data_MENSURATIONSIGN sign_) { m_sign = sign_; };
    data_MENSURATIONSIGN GetSign() const { return m_sign; };    
    bool HasSign( );
    
    ///@}

private:
    /** Specifies whether a dot is to be added to the base symbol. **/
    data_BOOLEAN m_dot;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    data_MENSURATIONSIGN m_sign;

/* include <attsign> */
};

//----------------------------------------------------------------------------
// AttMeterSigLog
//----------------------------------------------------------------------------

class AttMeterSigLog: public Att
{
public:
    AttMeterSigLog();
    virtual ~AttMeterSigLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterSigLog();
    
    /** Read the values for the attribute class **/
    bool ReadMeterSigLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeterSigLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCount(int count_) { m_count = count_; };
    int GetCount() const { return m_count; };    
    bool HasCount( );
    
    //
    void SetSym(data_METERSIGN sym_) { m_sym = sym_; };
    data_METERSIGN GetSym() const { return m_sym; };    
    bool HasSym( );
    
    //
    void SetUnit(int unit_) { m_unit = unit_; };
    int GetUnit() const { return m_unit; };    
    bool HasUnit( );
    
    ///@}

private:
    /**
     * Captures the number of beats in a measure, that is, the top number of the meter
     * signature.
     * It must contain a decimal number or an additive expression that evaluates to a
     * decimal number, such as 2+3.
     **/
    int m_count;
    /**
     * Indicates the use of a meter symbol instead of a numeric meter signature, that
     * is, 'C' for common time or 'C' with a slash for cut time.
     **/
    data_METERSIGN m_sym;
    /** Indicates the unit used for a measurement of size. **/
    int m_unit;

/* include <attunit> */
};

//----------------------------------------------------------------------------
// AttMeterSigVis
//----------------------------------------------------------------------------

class AttMeterSigVis: public Att
{
public:
    AttMeterSigVis();
    virtual ~AttMeterSigVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterSigVis();
    
    /** Read the values for the attribute class **/
    bool ReadMeterSigVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeterSigVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetRend(data_BARRENDITION rend_) { m_rend = rend_; };
    data_BARRENDITION GetRend() const { return m_rend; };    
    bool HasRend( );
    
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    data_BARRENDITION m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttMeterSigDefaultLog
//----------------------------------------------------------------------------

class AttMeterSigDefaultLog: public Att
{
public:
    AttMeterSigDefaultLog();
    virtual ~AttMeterSigDefaultLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterSigDefaultLog();
    
    /** Read the values for the attribute class **/
    bool ReadMeterSigDefaultLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeterSigDefaultLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMeterCount(int meterCount_) { m_meterCount = meterCount_; };
    int GetMeterCount() const { return m_meterCount; };    
    bool HasMeterCount( );
    
    //
    void SetMeterUnit(int meterUnit_) { m_meterUnit = meterUnit_; };
    int GetMeterUnit() const { return m_meterUnit; };    
    bool HasMeterUnit( );
    
    ///@}

private:
    /**
     * Captures the number of beats in a measure, that is, the top number of the meter
     * signature.
     * It must contain a decimal number or an additive expression that evaluates to a
     * decimal number, such as 2+3.
     **/
    int m_meterCount;
    /**
     * Contains the number indicating the beat unit, that is, the bottom number of the
     * meter signature.
     **/
    int m_meterUnit;

/* include <attmeter.unit> */
};

//----------------------------------------------------------------------------
// AttMeterSigDefaultVis
//----------------------------------------------------------------------------

class AttMeterSigDefaultVis: public Att
{
public:
    AttMeterSigDefaultVis();
    virtual ~AttMeterSigDefaultVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterSigDefaultVis();
    
    /** Read the values for the attribute class **/
    bool ReadMeterSigDefaultVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeterSigDefaultVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMeterRend(std::string meterRend_) { m_meterRend = meterRend_; };
    std::string GetMeterRend() const { return m_meterRend; };    
    bool HasMeterRend( );
    
    //
    void SetMeterShowchange(std::string meterShowchange_) { m_meterShowchange = meterShowchange_; };
    std::string GetMeterShowchange() const { return m_meterShowchange; };    
    bool HasMeterShowchange( );
    
    //
    void SetMeterSym(data_METERSIGN meterSym_) { m_meterSym = meterSym_; };
    data_METERSIGN GetMeterSym() const { return m_meterSym; };    
    bool HasMeterSym( );
    
    ///@}

private:
    /** Contains an indication of how the meter signature should be rendered. **/
    std::string m_meterRend;
    /**
     * Determines whether a new meter signature should be displayed when the meter
     * signature changes.
     **/
    std::string m_meterShowchange;
    /**
     * Indicates the use of a meter symbol instead of a numeric meter signature, that
     * is, 'C' for common time or 'C' with a slash for cut time.
     **/
    data_METERSIGN m_meterSym;

/* include <attmeter.sym> */
};

//----------------------------------------------------------------------------
// AttMeterconformance
//----------------------------------------------------------------------------

class AttMeterconformance: public Att
{
public:
    AttMeterconformance();
    virtual ~AttMeterconformance();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterconformance();
    
    /** Read the values for the attribute class **/
    bool ReadMeterconformance( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeterconformance( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMetcon(std::string metcon_) { m_metcon = metcon_; };
    std::string GetMetcon() const { return m_metcon; };    
    bool HasMetcon( );
    
    ///@}

private:
    /**
     * Indicates the relationship between the content of a staff or layer and the
     * prevailing meter.
     **/
    std::string m_metcon;

/* include <attmetcon> */
};

//----------------------------------------------------------------------------
// AttMeterconformanceBar
//----------------------------------------------------------------------------

class AttMeterconformanceBar: public Att
{
public:
    AttMeterconformanceBar();
    virtual ~AttMeterconformanceBar();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterconformanceBar();
    
    /** Read the values for the attribute class **/
    bool ReadMeterconformanceBar( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMeterconformanceBar( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMetcon(std::string metcon_) { m_metcon = metcon_; };
    std::string GetMetcon() const { return m_metcon; };    
    bool HasMetcon( );
    
    //
    void SetControl(std::string control_) { m_control = control_; };
    std::string GetControl() const { return m_control; };    
    bool HasControl( );
    
    ///@}

private:
    /**
     * Indicates the relationship between the content of a staff or layer and the
     * prevailing meter.
     **/
    std::string m_metcon;
    /**
     * Indicates whether or not a bar line is "controlling"; that is, if it indicates a
     * point of alignment across all the parts.
     * Bar lines within a score are usually controlling; that is, they "line up". Bar
     * lines within parts may or may not be controlling. When applied to <measure>,
     * this attribute indicates the nature of the right barline but not the left.
     **/
    std::string m_control;

/* include <attcontrol> */
};

//----------------------------------------------------------------------------
// AttMmtempo
//----------------------------------------------------------------------------

class AttMmtempo: public Att
{
public:
    AttMmtempo();
    virtual ~AttMmtempo();
    
    /** Reset the default values for the attribute class **/
    void ResetMmtempo();
    
    /** Read the values for the attribute class **/
    bool ReadMmtempo( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMmtempo( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMm(std::string mm_) { m_mm = mm_; };
    std::string GetMm() const { return m_mm; };    
    bool HasMm( );
    
    //
    void SetMmUnit(std::string mmUnit_) { m_mmUnit = mmUnit_; };
    std::string GetMmUnit() const { return m_mmUnit; };    
    bool HasMmUnit( );
    
    //
    void SetMmDots(std::string mmDots_) { m_mmDots = mmDots_; };
    std::string GetMmDots() const { return m_mmDots; };    
    bool HasMmDots( );
    
    ///@}

private:
    /**
     * Used to describe tempo in terms of beats (often the meter signature denominator)
     * per minute, ala M.M.
     * (Maezel's Metronome).
     **/
    std::string m_mm;
    /** Captures the metronomic unit. **/
    std::string m_mmUnit;
    /** Records the number of augmentation dots required by a dotted metronome unit. **/
    std::string m_mmDots;

/* include <attmm.dots> */
};

//----------------------------------------------------------------------------
// AttMultinummeasures
//----------------------------------------------------------------------------

class AttMultinummeasures: public Att
{
public:
    AttMultinummeasures();
    virtual ~AttMultinummeasures();
    
    /** Reset the default values for the attribute class **/
    void ResetMultinummeasures();
    
    /** Read the values for the attribute class **/
    bool ReadMultinummeasures( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMultinummeasures( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMultiNumber(std::string multiNumber_) { m_multiNumber = multiNumber_; };
    std::string GetMultiNumber() const { return m_multiNumber; };    
    bool HasMultiNumber( );
    
    ///@}

private:
    /**
     * Indicates whether programmatically calculated counts of multiple measures of
     * rest (mRest) and whole measure repeats (mRpt) in parts should be rendered.
     **/
    std::string m_multiNumber;

/* include <attmulti.number> */
};

//----------------------------------------------------------------------------
// AttName
//----------------------------------------------------------------------------

class AttName: public Att
{
public:
    AttName();
    virtual ~AttName();
    
    /** Reset the default values for the attribute class **/
    void ResetName();
    
    /** Read the values for the attribute class **/
    bool ReadName( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteName( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetNymref(std::string nymref_) { m_nymref = nymref_; };
    std::string GetNymref() const { return m_nymref; };    
    bool HasNymref( );
    
    //
    void SetRole(std::string role_) { m_role = role_; };
    std::string GetRole() const { return m_role; };    
    bool HasRole( );
    
    ///@}

private:
    /**
     * Used to record a pointer to the regularized form of the name elsewhere in the
     * document.
     **/
    std::string m_nymref;
    /**
     * Used to specify further information about the entity referenced by this name,
     * for example, the occupation of a person or the status of a place.
     **/
    std::string m_role;

/* include <attrole> */
};

//----------------------------------------------------------------------------
// AttNoteGes
//----------------------------------------------------------------------------

class AttNoteGes: public Att
{
public:
    AttNoteGes();
    virtual ~AttNoteGes();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteGes();
    
    /** Read the values for the attribute class **/
    bool ReadNoteGes( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteNoteGes( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetOctGes(std::string octGes_) { m_octGes = octGes_; };
    std::string GetOctGes() const { return m_octGes; };    
    bool HasOctGes( );
    
    //
    void SetPnameGes(std::string pnameGes_) { m_pnameGes = pnameGes_; };
    std::string GetPnameGes() const { return m_pnameGes; };    
    bool HasPnameGes( );
    
    //
    void SetPnum(std::string pnum_) { m_pnum = pnum_; };
    std::string GetPnum() const { return m_pnum; };    
    bool HasPnum( );
    
    ///@}

private:
    /** Records performed octave information that differs from the written value. **/
    std::string m_octGes;
    /** Contains a performed pitch name that differs from the written value. **/
    std::string m_pnameGes;
    /** Holds a pitch-to-number mapping, a base-40 or MIDI note number, for example. **/
    std::string m_pnum;

/* include <attpnum> */
};

//----------------------------------------------------------------------------
// AttNoteVis
//----------------------------------------------------------------------------

class AttNoteVis: public Att
{
public:
    AttNoteVis();
    virtual ~AttNoteVis();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteVis();
    
    /** Read the values for the attribute class **/
    bool ReadNoteVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteNoteVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetHeadshape(std::string headshape_) { m_headshape = headshape_; };
    std::string GetHeadshape() const { return m_headshape; };    
    bool HasHeadshape( );
    
    ///@}

private:
    /** Used to override the head shape normally used for the given duration. **/
    std::string m_headshape;

/* include <attheadshape> */
};

//----------------------------------------------------------------------------
// AttOctave
//----------------------------------------------------------------------------

class AttOctave: public Att
{
public:
    AttOctave();
    virtual ~AttOctave();
    
    /** Reset the default values for the attribute class **/
    void ResetOctave();
    
    /** Read the values for the attribute class **/
    bool ReadOctave( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteOctave( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetOct(char oct_) { m_oct = oct_; };
    char GetOct() const { return m_oct; };    
    bool HasOct( );
    
    ///@}

private:
    /** Captures written octave information. **/
    char m_oct;

/* include <attoct> */
};

//----------------------------------------------------------------------------
// AttOctavedefault
//----------------------------------------------------------------------------

class AttOctavedefault: public Att
{
public:
    AttOctavedefault();
    virtual ~AttOctavedefault();
    
    /** Reset the default values for the attribute class **/
    void ResetOctavedefault();
    
    /** Read the values for the attribute class **/
    bool ReadOctavedefault( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteOctavedefault( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetOctaveDefault(std::string octaveDefault_) { m_octaveDefault = octaveDefault_; };
    std::string GetOctaveDefault() const { return m_octaveDefault; };    
    bool HasOctaveDefault( );
    
    ///@}

private:
    /**
     * Contains a default octave specification for use when the first note, rest,
     * chord, etc.
     * in a measure does not have an octave value specified.
     **/
    std::string m_octaveDefault;

/* include <attoctave.default> */
};

//----------------------------------------------------------------------------
// AttOctavedisplacement
//----------------------------------------------------------------------------

class AttOctavedisplacement: public Att
{
public:
    AttOctavedisplacement();
    virtual ~AttOctavedisplacement();
    
    /** Reset the default values for the attribute class **/
    void ResetOctavedisplacement();
    
    /** Read the values for the attribute class **/
    bool ReadOctavedisplacement( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteOctavedisplacement( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDis(data_OCTAVE_DIS dis_) { m_dis = dis_; };
    data_OCTAVE_DIS GetDis() const { return m_dis; };    
    bool HasDis( );
    
    //
    void SetDisPlace(data_PLACE disPlace_) { m_disPlace = disPlace_; };
    data_PLACE GetDisPlace() const { return m_disPlace; };    
    bool HasDisPlace( );
    
    ///@}

private:
    /** Records the amount of octave displacement. **/
    data_OCTAVE_DIS m_dis;
    /** Records the direction of octave displacement. **/
    data_PLACE m_disPlace;

/* include <attdis.place> */
};

//----------------------------------------------------------------------------
// AttOnelinestaff
//----------------------------------------------------------------------------

class AttOnelinestaff: public Att
{
public:
    AttOnelinestaff();
    virtual ~AttOnelinestaff();
    
    /** Reset the default values for the attribute class **/
    void ResetOnelinestaff();
    
    /** Read the values for the attribute class **/
    bool ReadOnelinestaff( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteOnelinestaff( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetOntheline(std::string ontheline_) { m_ontheline = ontheline_; };
    std::string GetOntheline() const { return m_ontheline; };    
    bool HasOntheline( );
    
    ///@}

private:
    /**
     * Determines the placement of notes on a 1-line staff.
     * A value of 'true' places all notes on the line, while a value of 'false' places
     * stems-up notes above the line and stems-down notes below the line.
     **/
    std::string m_ontheline;

/* include <attontheline> */
};

//----------------------------------------------------------------------------
// AttPadLog
//----------------------------------------------------------------------------

class AttPadLog: public Att
{
public:
    AttPadLog();
    virtual ~AttPadLog();
    
    /** Reset the default values for the attribute class **/
    void ResetPadLog();
    
    /** Read the values for the attribute class **/
    bool ReadPadLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePadLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetNum(int num_) { m_num = num_; };
    int GetNum() const { return m_num; };    
    bool HasNum( );
    
    ///@}

private:
    /**
     * Along with numbase, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_num;

/* include <attnum> */
};

//----------------------------------------------------------------------------
// AttPbVis
//----------------------------------------------------------------------------

class AttPbVis: public Att
{
public:
    AttPbVis();
    virtual ~AttPbVis();
    
    /** Reset the default values for the attribute class **/
    void ResetPbVis();
    
    /** Read the values for the attribute class **/
    bool ReadPbVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePbVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetFunc(std::string func_) { m_func = func_; };
    std::string GetFunc() const { return m_func; };    
    bool HasFunc( );
    
    ///@}

private:
    /** Records the function of an accidental. **/
    std::string m_func;

/* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttPitch
//----------------------------------------------------------------------------

class AttPitch: public Att
{
public:
    AttPitch();
    virtual ~AttPitch();
    
    /** Reset the default values for the attribute class **/
    void ResetPitch();
    
    /** Read the values for the attribute class **/
    bool ReadPitch( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePitch( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetPname(data_PITCHNAME pname_) { m_pname = pname_; };
    data_PITCHNAME GetPname() const { return m_pname; };    
    bool HasPname( );
    
    ///@}

private:
    /** Contains a written pitch name. **/
    data_PITCHNAME m_pname;

/* include <attpname> */
};

//----------------------------------------------------------------------------
// AttPlacement
//----------------------------------------------------------------------------

class AttPlacement: public Att
{
public:
    AttPlacement();
    virtual ~AttPlacement();
    
    /** Reset the default values for the attribute class **/
    void ResetPlacement();
    
    /** Read the values for the attribute class **/
    bool ReadPlacement( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePlacement( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetPlace(std::string place_) { m_place = place_; };
    std::string GetPlace() const { return m_place; };    
    bool HasPlace( );
    
    ///@}

private:
    /**
     * Captures the placement of the item with respect to the staff with which it is
     * associated.
     **/
    std::string m_place;

/* include <attplace> */
};

//----------------------------------------------------------------------------
// AttPlist
//----------------------------------------------------------------------------

class AttPlist: public Att
{
public:
    AttPlist();
    virtual ~AttPlist();
    
    /** Reset the default values for the attribute class **/
    void ResetPlist();
    
    /** Read the values for the attribute class **/
    bool ReadPlist( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePlist( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetPlist(std::string plist_) { m_plist = plist_; };
    std::string GetPlist() const { return m_plist; };    
    bool HasPlist( );
    
    //
    void SetEvaluate(std::string evaluate_) { m_evaluate = evaluate_; };
    std::string GetEvaluate() const { return m_evaluate; };    
    bool HasEvaluate( );
    
    ///@}

private:
    /**
     * Contains a space separated list of references that identify logical events that
     * participate in a collection, such as notes under a phrase mark.
     **/
    std::string m_plist;
    /** Specifies the intended meaning when the target of a pointer is itself a pointer. **/
    std::string m_evaluate;

/* include <attevaluate> */
};

//----------------------------------------------------------------------------
// AttPointing
//----------------------------------------------------------------------------

class AttPointing: public Att
{
public:
    AttPointing();
    virtual ~AttPointing();
    
    /** Reset the default values for the attribute class **/
    void ResetPointing();
    
    /** Read the values for the attribute class **/
    bool ReadPointing( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePointing( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetActuate(std::string actuate_) { m_actuate = actuate_; };
    std::string GetActuate() const { return m_actuate; };    
    bool HasActuate( );
    
    //
    void SetRole(std::string role_) { m_role = role_; };
    std::string GetRole() const { return m_role; };    
    bool HasRole( );
    
    //
    void SetShow(std::string show_) { m_show = show_; };
    std::string GetShow() const { return m_show; };    
    bool HasShow( );
    
    //
    void SetTarget(std::string target_) { m_target = target_; };
    std::string GetTarget() const { return m_target; };    
    bool HasTarget( );
    
    //
    void SetTargettype(std::string targettype_) { m_targettype = targettype_; };
    std::string GetTargettype() const { return m_targettype; };    
    bool HasTargettype( );
    
    //
    void SetTitle(std::string title_) { m_title = title_; };
    std::string GetTitle() const { return m_title; };    
    bool HasTitle( );
    
    ///@}

private:
    /** --- **/
    std::string m_actuate;
    /**
     * Used to specify further information about the entity referenced by this name,
     * for example, the occupation of a person or the status of a place.
     **/
    std::string m_role;
    /** --- **/
    std::string m_show;
    /**
     * Encodes the target note when its pitch differs from the pitch at which the
     * custos appears.
     **/
    std::string m_target;
    /**
     * In contrast with the role attribute, allows the target resource to be
     * characterized using any convenient classification scheme or typology.
     **/
    std::string m_targettype;
    /** --- **/
    std::string m_title;

/* include <atttitle> */
};

//----------------------------------------------------------------------------
// AttRelativesize
//----------------------------------------------------------------------------

class AttRelativesize: public Att
{
public:
    AttRelativesize();
    virtual ~AttRelativesize();
    
    /** Reset the default values for the attribute class **/
    void ResetRelativesize();
    
    /** Read the values for the attribute class **/
    bool ReadRelativesize( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteRelativesize( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetSize(std::string size_) { m_size = size_; };
    std::string GetSize() const { return m_size; };    
    bool HasSize( );
    
    ///@}

private:
    /** Describes the relative size of a feature. **/
    std::string m_size;

/* include <attsize> */
};

//----------------------------------------------------------------------------
// AttResponsibility
//----------------------------------------------------------------------------

class AttResponsibility: public Att
{
public:
    AttResponsibility();
    virtual ~AttResponsibility();
    
    /** Reset the default values for the attribute class **/
    void ResetResponsibility();
    
    /** Read the values for the attribute class **/
    bool ReadResponsibility( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteResponsibility( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetResp(std::string resp_) { m_resp = resp_; };
    std::string GetResp() const { return m_resp; };    
    bool HasResp( );
    
    ///@}

private:
    /**
     * Captures information regarding responsibility for some aspect of the text's
     * creation, transcription, editing, or encoding.
     * Its value must point to one or more identifiers declared in the document header.
     **/
    std::string m_resp;

/* include <attresp> */
};

//----------------------------------------------------------------------------
// AttSbVis
//----------------------------------------------------------------------------

class AttSbVis: public Att
{
public:
    AttSbVis();
    virtual ~AttSbVis();
    
    /** Reset the default values for the attribute class **/
    void ResetSbVis();
    
    /** Read the values for the attribute class **/
    bool ReadSbVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSbVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetRend(data_BARRENDITION rend_) { m_rend = rend_; };
    data_BARRENDITION GetRend() const { return m_rend; };    
    bool HasRend( );
    
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    data_BARRENDITION m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttScalable
//----------------------------------------------------------------------------

class AttScalable: public Att
{
public:
    AttScalable();
    virtual ~AttScalable();
    
    /** Reset the default values for the attribute class **/
    void ResetScalable();
    
    /** Read the values for the attribute class **/
    bool ReadScalable( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteScalable( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetScale(std::string scale_) { m_scale = scale_; };
    std::string GetScale() const { return m_scale; };    
    bool HasScale( );
    
    ///@}

private:
    /** Scale factor to be applied to the feature to make it the desired display size. **/
    std::string m_scale;

/* include <attscale> */
};

//----------------------------------------------------------------------------
// AttScoreDefGes
//----------------------------------------------------------------------------

class AttScoreDefGes: public Att
{
public:
    AttScoreDefGes();
    virtual ~AttScoreDefGes();
    
    /** Reset the default values for the attribute class **/
    void ResetScoreDefGes();
    
    /** Read the values for the attribute class **/
    bool ReadScoreDefGes( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteScoreDefGes( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTunePname(std::string tunePname_) { m_tunePname = tunePname_; };
    std::string GetTunePname() const { return m_tunePname; };    
    bool HasTunePname( );
    
    //
    void SetTuneHz(double tuneHzDbl_) { m_tuneHzDbl = tuneHzDbl_; };
    double GetTuneHz() const { return m_tuneHzDbl; };    
    bool HasTuneHz( );
    
    //
    void SetTuneTemper(std::string tuneTemper_) { m_tuneTemper = tuneTemper_; };
    std::string GetTuneTemper() const { return m_tuneTemper; };    
    bool HasTuneTemper( );
    
    ///@}

private:
    /** Holds the pitch name of a tuning reference pitch. **/
    std::string m_tunePname;
    /** Holds a value for cycles per second, i.e., Hertz, for a tuning reference pitch. **/
    double m_tuneHzDbl;
    /** Provides an indication of the tuning system, 'just', for example. **/
    std::string m_tuneTemper;

/* include <atttune.temper> */
};

//----------------------------------------------------------------------------
// AttScoreDefVis
//----------------------------------------------------------------------------

class AttScoreDefVis: public Att
{
public:
    AttScoreDefVis();
    virtual ~AttScoreDefVis();
    
    /** Reset the default values for the attribute class **/
    void ResetScoreDefVis();
    
    /** Read the values for the attribute class **/
    bool ReadScoreDefVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteScoreDefVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetEndingRend(std::string endingRend_) { m_endingRend = endingRend_; };
    std::string GetEndingRend() const { return m_endingRend; };    
    bool HasEndingRend( );
    
    //
    void SetMnumVisible(std::string mnumVisible_) { m_mnumVisible = mnumVisible_; };
    std::string GetMnumVisible() const { return m_mnumVisible; };    
    bool HasMnumVisible( );
    
    //
    void SetMusicName(std::string musicName_) { m_musicName = musicName_; };
    std::string GetMusicName() const { return m_musicName; };    
    bool HasMusicName( );
    
    //
    void SetMusicSize(std::string musicSize_) { m_musicSize = musicSize_; };
    std::string GetMusicSize() const { return m_musicSize; };    
    bool HasMusicSize( );
    
    //
    void SetOptimize(std::string optimize_) { m_optimize = optimize_; };
    std::string GetOptimize() const { return m_optimize; };    
    bool HasOptimize( );
    
    //
    void SetPageHeight(std::string pageHeight_) { m_pageHeight = pageHeight_; };
    std::string GetPageHeight() const { return m_pageHeight; };    
    bool HasPageHeight( );
    
    //
    void SetPageWidth(std::string pageWidth_) { m_pageWidth = pageWidth_; };
    std::string GetPageWidth() const { return m_pageWidth; };    
    bool HasPageWidth( );
    
    //
    void SetPageTopmar(std::string pageTopmar_) { m_pageTopmar = pageTopmar_; };
    std::string GetPageTopmar() const { return m_pageTopmar; };    
    bool HasPageTopmar( );
    
    //
    void SetPageBotmar(std::string pageBotmar_) { m_pageBotmar = pageBotmar_; };
    std::string GetPageBotmar() const { return m_pageBotmar; };    
    bool HasPageBotmar( );
    
    //
    void SetPageLeftmar(std::string pageLeftmar_) { m_pageLeftmar = pageLeftmar_; };
    std::string GetPageLeftmar() const { return m_pageLeftmar; };    
    bool HasPageLeftmar( );
    
    //
    void SetPageRightmar(std::string pageRightmar_) { m_pageRightmar = pageRightmar_; };
    std::string GetPageRightmar() const { return m_pageRightmar; };    
    bool HasPageRightmar( );
    
    //
    void SetPagePanels(std::string pagePanels_) { m_pagePanels = pagePanels_; };
    std::string GetPagePanels() const { return m_pagePanels; };    
    bool HasPagePanels( );
    
    //
    void SetPageScale(std::string pageScale_) { m_pageScale = pageScale_; };
    std::string GetPageScale() const { return m_pageScale; };    
    bool HasPageScale( );
    
    //
    void SetSpacingPackexp(double spacingPackexpDbl_) { m_spacingPackexpDbl = spacingPackexpDbl_; };
    double GetSpacingPackexp() const { return m_spacingPackexpDbl; };    
    bool HasSpacingPackexp( );
    
    //
    void SetSpacingPackfact(double spacingPackfactDbl_) { m_spacingPackfactDbl = spacingPackfactDbl_; };
    double GetSpacingPackfact() const { return m_spacingPackfactDbl; };    
    bool HasSpacingPackfact( );
    
    //
    void SetSpacingStaff(std::string spacingStaff_) { m_spacingStaff = spacingStaff_; };
    std::string GetSpacingStaff() const { return m_spacingStaff; };    
    bool HasSpacingStaff( );
    
    //
    void SetSpacingSystem(std::string spacingSystem_) { m_spacingSystem = spacingSystem_; };
    std::string GetSpacingSystem() const { return m_spacingSystem; };    
    bool HasSpacingSystem( );
    
    //
    void SetSystemLeftmar(std::string systemLeftmar_) { m_systemLeftmar = systemLeftmar_; };
    std::string GetSystemLeftmar() const { return m_systemLeftmar; };    
    bool HasSystemLeftmar( );
    
    //
    void SetSystemRightmar(std::string systemRightmar_) { m_systemRightmar = systemRightmar_; };
    std::string GetSystemRightmar() const { return m_systemRightmar; };    
    bool HasSystemRightmar( );
    
    //
    void SetSystemTopmar(std::string systemTopmar_) { m_systemTopmar = systemTopmar_; };
    std::string GetSystemTopmar() const { return m_systemTopmar; };    
    bool HasSystemTopmar( );
    
    //
    void SetVuHeight(std::string vuHeight_) { m_vuHeight = vuHeight_; };
    std::string GetVuHeight() const { return m_vuHeight; };    
    bool HasVuHeight( );
    
    ///@}

private:
    /** Describes where ending marks should be displayed. **/
    std::string m_endingRend;
    /** Indicates whether measure numbers should be displayed. **/
    std::string m_mnumVisible;
    /** Sets the default music font name. **/
    std::string m_musicName;
    /** Sets the default music font size. **/
    std::string m_musicSize;
    /**
     * Indicates whether staves without notes, rests, etc.
     * should be displayed. When the value is 'true', empty staves are displayed.
     **/
    std::string m_optimize;
    /**
     * Specifies the height of the page; may be expressed in real-world units or staff
     * steps.
     **/
    std::string m_pageHeight;
    /**
     * Describes the width of the page; may be expressed in real-world units or staff
     * steps.
     **/
    std::string m_pageWidth;
    /** Indicates the amount of whitespace at the top of a page. **/
    std::string m_pageTopmar;
    /** Indicates the amount of whitespace at the bottom of a page. **/
    std::string m_pageBotmar;
    /** Indicates the amount of whitespace at the left side of a page. **/
    std::string m_pageLeftmar;
    /** Indicates the amount of whitespace at the right side of a page. **/
    std::string m_pageRightmar;
    /** Indicates the number of logical pages to be rendered on a single physical page. **/
    std::string m_pagePanels;
    /** Indicates how the page should be scaled when rendered. **/
    std::string m_pageScale;
    /** Describes a note's spacing relative to its time value. **/
    double m_spacingPackexpDbl;
    /** Describes the note spacing of output. **/
    double m_spacingPackfactDbl;
    /**
     * Specifies the minimum amount of space between adjacent staves in the same
     * system; measured from the bottom line of the staff above to the top line of the
     * staff below.
     **/
    std::string m_spacingStaff;
    /**
     * Describes the space between adjacent systems; a pair of space-separated values
     * (minimum and maximum, respectively) provides a range between which a rendering
     * system-supplied value may fall, while a single value indicates a fixed amount of
     * space; that is, the minimum and maximum values are equal.
     **/
    std::string m_spacingSystem;
    /**
     * Describes the amount of whitespace at the left system margin relative to
     * page.leftmar.
     **/
    std::string m_systemLeftmar;
    /**
     * Describes the amount of whitespace at the right system margin relative to
     * page.rightmar.
     **/
    std::string m_systemRightmar;
    /**
     * Describes the distance from page's top edge to the first system; used for first
     * page only.
     **/
    std::string m_systemTopmar;
    /**
     * Defines the height of a "virtual unit" (vu) in terms of real-world units.
     * A single vu is half the distance between the vertical center point of a staff
     * line and that of an adjacent staff line.
     **/
    std::string m_vuHeight;

/* include <attvu.height> */
};

//----------------------------------------------------------------------------
// AttSectionVis
//----------------------------------------------------------------------------

class AttSectionVis: public Att
{
public:
    AttSectionVis();
    virtual ~AttSectionVis();
    
    /** Reset the default values for the attribute class **/
    void ResetSectionVis();
    
    /** Read the values for the attribute class **/
    bool ReadSectionVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSectionVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetRestart(std::string restart_) { m_restart = restart_; };
    std::string GetRestart() const { return m_restart; };    
    bool HasRestart( );
    
    ///@}

private:
    /** Indicates that staves begin again with this section. **/
    std::string m_restart;

/* include <attrestart> */
};

//----------------------------------------------------------------------------
// AttSequence
//----------------------------------------------------------------------------

class AttSequence: public Att
{
public:
    AttSequence();
    virtual ~AttSequence();
    
    /** Reset the default values for the attribute class **/
    void ResetSequence();
    
    /** Read the values for the attribute class **/
    bool ReadSequence( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSequence( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetSeq(int seqInt_) { m_seqInt = seqInt_; };
    int GetSeq() const { return m_seqInt; };    
    bool HasSeq( );
    
    ///@}

private:
    /**
     * Used to assign a sequence number related to the order in which the encoded
     * features carrying this attribute are believed to have occurred.
     **/
    int m_seqInt;

/* include <attseq> */
};

//----------------------------------------------------------------------------
// AttSlashcount
//----------------------------------------------------------------------------

class AttSlashcount: public Att
{
public:
    AttSlashcount();
    virtual ~AttSlashcount();
    
    /** Reset the default values for the attribute class **/
    void ResetSlashcount();
    
    /** Read the values for the attribute class **/
    bool ReadSlashcount( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSlashcount( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetSlash(char slash_) { m_slash = slash_; };
    char GetSlash() const { return m_slash; };    
    bool HasSlash( );
    
    ///@}

private:
    /** Indicates the number of slashes present. **/
    char m_slash;

/* include <attslash> */
};

//----------------------------------------------------------------------------
// AttSlurpresent
//----------------------------------------------------------------------------

class AttSlurpresent: public Att
{
public:
    AttSlurpresent();
    virtual ~AttSlurpresent();
    
    /** Reset the default values for the attribute class **/
    void ResetSlurpresent();
    
    /** Read the values for the attribute class **/
    bool ReadSlurpresent( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSlurpresent( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetSlur(std::string slur_) { m_slur = slur_; };
    std::string GetSlur() const { return m_slur; };    
    bool HasSlur( );
    
    ///@}

private:
    /**
     * Indicates that this element participates in a slur.
     * If visual information about the slur needs to be recorded, then a <slur> element
     * should be employed.
     **/
    std::string m_slur;

/* include <attslur> */
};

//----------------------------------------------------------------------------
// AttSpaceVis
//----------------------------------------------------------------------------

class AttSpaceVis: public Att
{
public:
    AttSpaceVis();
    virtual ~AttSpaceVis();
    
    /** Reset the default values for the attribute class **/
    void ResetSpaceVis();
    
    /** Read the values for the attribute class **/
    bool ReadSpaceVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSpaceVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCompressable(std::string compressable_) { m_compressable = compressable_; };
    std::string GetCompressable() const { return m_compressable; };    
    bool HasCompressable( );
    
    ///@}

private:
    /**
     * Indicates whether a space is 'compressible', i.e., if it may be removed at the
     * discretion of processing software.
     **/
    std::string m_compressable;

/* include <attcompressable> */
};

//----------------------------------------------------------------------------
// AttStaffLog
//----------------------------------------------------------------------------

class AttStaffLog: public Att
{
public:
    AttStaffLog();
    virtual ~AttStaffLog();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffLog();
    
    /** Read the values for the attribute class **/
    bool ReadStaffLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStaffLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDef(std::string def_) { m_def = def_; };
    std::string GetDef() const { return m_def; };    
    bool HasDef( );
    
    ///@}

private:
    /** Provides a mechanism for linking the layer to a layerDef element. **/
    std::string m_def;

/* include <attdef> */
};

//----------------------------------------------------------------------------
// AttStaffDefVis
//----------------------------------------------------------------------------

class AttStaffDefVis: public Att
{
public:
    AttStaffDefVis();
    virtual ~AttStaffDefVis();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffDefVis();
    
    /** Read the values for the attribute class **/
    bool ReadStaffDefVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStaffDefVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetGridShow(data_BOOLEAN gridShow_) { m_gridShow = gridShow_; };
    data_BOOLEAN GetGridShow() const { return m_gridShow; };    
    bool HasGridShow( );
    
    //
    void SetLayerscheme(data_LAYERSCHEME layerscheme_) { m_layerscheme = layerscheme_; };
    data_LAYERSCHEME GetLayerscheme() const { return m_layerscheme; };    
    bool HasLayerscheme( );
    
    //
    void SetLines(int linesInt_) { m_linesInt = linesInt_; };
    int GetLines() const { return m_linesInt; };    
    bool HasLines( );
    
    //
    void SetLinesColor(std::string linesColor_) { m_linesColor = linesColor_; };
    std::string GetLinesColor() const { return m_linesColor; };    
    bool HasLinesColor( );
    
    //
    void SetLinesVisible(data_BOOLEAN linesVisible_) { m_linesVisible = linesVisible_; };
    data_BOOLEAN GetLinesVisible() const { return m_linesVisible; };    
    bool HasLinesVisible( );
    
    //
    void SetSpacing(std::string spacing_) { m_spacing = spacing_; };
    std::string GetSpacing() const { return m_spacing; };    
    bool HasSpacing( );
    
    ///@}

private:
    /** Determines whether to display guitar chord grids. **/
    data_BOOLEAN m_gridShow;
    /** Indicates the number of layers and their stem directions. **/
    data_LAYERSCHEME m_layerscheme;
    /** Indicates the number of staff lines. **/
    int m_linesInt;
    /**
     * Captures the colors of the staff lines.
     * The value is structured; that is, it should have the same number of space-
     * separated RGB values as the number of lines indicated by the lines attribute. A
     * line can be made invisible by assigning it the same RGB value as the background,
     * usually white.
     **/
    std::string m_linesColor;
    /** Records whether all staff lines are visible. **/
    data_BOOLEAN m_linesVisible;
    /**
     * Records the absolute distance (as opposed to the relative distances recorded in
     * <scoreDef> elements) between this staff and the preceding one in the same
     * system.
     * This value is meaningless for the first staff in a system since the
     * spacing.system attribute indicates the spacing between systems.
     **/
    std::string m_spacing;

/* include <attspacing> */
};

//----------------------------------------------------------------------------
// AttStaffGrpVis
//----------------------------------------------------------------------------

class AttStaffGrpVis: public Att
{
public:
    AttStaffGrpVis();
    virtual ~AttStaffGrpVis();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffGrpVis();
    
    /** Read the values for the attribute class **/
    bool ReadStaffGrpVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStaffGrpVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetBarthru(std::string barthru_) { m_barthru = barthru_; };
    std::string GetBarthru() const { return m_barthru; };    
    bool HasBarthru( );
    
    ///@}

private:
    /**
     * Indicates whether bar lines go across the space between staves (true) or are
     * only drawn across the lines of each staff (false).
     **/
    std::string m_barthru;

/* include <attbarthru> */
};

//----------------------------------------------------------------------------
// AttStaffgroupingsym
//----------------------------------------------------------------------------

class AttStaffgroupingsym: public Att
{
public:
    AttStaffgroupingsym();
    virtual ~AttStaffgroupingsym();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffgroupingsym();
    
    /** Read the values for the attribute class **/
    bool ReadStaffgroupingsym( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStaffgroupingsym( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetSymbol(std::string symbol_) { m_symbol = symbol_; };
    std::string GetSymbol() const { return m_symbol; };    
    bool HasSymbol( );
    
    ///@}

private:
    /** Specifies the symbol used to group a set of staves. **/
    std::string m_symbol;

/* include <attsymbol> */
};

//----------------------------------------------------------------------------
// AttStaffident
//----------------------------------------------------------------------------

class AttStaffident: public Att
{
public:
    AttStaffident();
    virtual ~AttStaffident();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffident();
    
    /** Read the values for the attribute class **/
    bool ReadStaffident( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStaffident( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetStaff(int staff_) { m_staff = staff_; };
    int GetStaff() const { return m_staff; };    
    bool HasStaff( );
    
    ///@}

private:
    /**
     * Signifies the staff on which a notated event occurs or to which a control event
     * applies.
     * Mandatory when applicable.
     **/
    int m_staff;

/* include <attstaff> */
};

//----------------------------------------------------------------------------
// AttStaffloc
//----------------------------------------------------------------------------

class AttStaffloc: public Att
{
public:
    AttStaffloc();
    virtual ~AttStaffloc();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffloc();
    
    /** Read the values for the attribute class **/
    bool ReadStaffloc( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStaffloc( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLoc(std::string loc_) { m_loc = loc_; };
    std::string GetLoc() const { return m_loc; };    
    bool HasLoc( );
    
    ///@}

private:
    /** Holds the staff location of the feature. **/
    std::string m_loc;

/* include <attloc> */
};

//----------------------------------------------------------------------------
// AttStafflocPitched
//----------------------------------------------------------------------------

class AttStafflocPitched: public Att
{
public:
    AttStafflocPitched();
    virtual ~AttStafflocPitched();
    
    /** Reset the default values for the attribute class **/
    void ResetStafflocPitched();
    
    /** Read the values for the attribute class **/
    bool ReadStafflocPitched( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStafflocPitched( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetPloc(data_PITCHNAME ploc_) { m_ploc = ploc_; };
    data_PITCHNAME GetPloc() const { return m_ploc; };    
    bool HasPloc( );
    
    //
    void SetOloc(char oloc_) { m_oloc = oloc_; };
    char GetOloc() const { return m_oloc; };    
    bool HasOloc( );
    
    ///@}

private:
    /** Captures staff location in terms of written pitch name. **/
    data_PITCHNAME m_ploc;
    /** Records staff location in terms of written octave. **/
    char m_oloc;

/* include <attoloc> */
};

//----------------------------------------------------------------------------
// AttStartendid
//----------------------------------------------------------------------------

class AttStartendid: public Att
{
public:
    AttStartendid();
    virtual ~AttStartendid();
    
    /** Reset the default values for the attribute class **/
    void ResetStartendid();
    
    /** Read the values for the attribute class **/
    bool ReadStartendid( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStartendid( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetEndid(std::string endid_) { m_endid = endid_; };
    std::string GetEndid() const { return m_endid; };    
    bool HasEndid( );
    
    ///@}

private:
    /**
     * Indicates the final element in a sequence of events to which the feature
     * applies.
     **/
    std::string m_endid;

/* include <attendid> */
};

//----------------------------------------------------------------------------
// AttStartid
//----------------------------------------------------------------------------

class AttStartid: public Att
{
public:
    AttStartid();
    virtual ~AttStartid();
    
    /** Reset the default values for the attribute class **/
    void ResetStartid();
    
    /** Read the values for the attribute class **/
    bool ReadStartid( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStartid( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetStartid(std::string startid_) { m_startid = startid_; };
    std::string GetStartid() const { return m_startid; };    
    bool HasStartid( );
    
    ///@}

private:
    /**
     * Holds a reference to the first element in a sequence of events to which the
     * feature applies.
     **/
    std::string m_startid;

/* include <attstartid> */
};

//----------------------------------------------------------------------------
// AttStemmed
//----------------------------------------------------------------------------

class AttStemmed: public Att
{
public:
    AttStemmed();
    virtual ~AttStemmed();
    
    /** Reset the default values for the attribute class **/
    void ResetStemmed();
    
    /** Read the values for the attribute class **/
    bool ReadStemmed( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStemmed( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetStemDir(data_STEMDIRECTION stemDir_) { m_stemDir = stemDir_; };
    data_STEMDIRECTION GetStemDir() const { return m_stemDir; };    
    bool HasStemDir( );
    
    //
    void SetStemLen(int stemLen_) { m_stemLen = stemLen_; };
    int GetStemLen() const { return m_stemLen; };    
    bool HasStemLen( );
    
    //
    void SetStemPos(data_STEMPOSITION stemPos_) { m_stemPos = stemPos_; };
    data_STEMPOSITION GetStemPos() const { return m_stemPos; };    
    bool HasStemPos( );
    
    //
    void SetStemX(int stemX_) { m_stemX = stemX_; };
    int GetStemX() const { return m_stemX; };    
    bool HasStemX( );
    
    //
    void SetStemY(int stemY_) { m_stemY = stemY_; };
    int GetStemY() const { return m_stemY; };    
    bool HasStemY( );
    
    ///@}

private:
    /** Describes the direction of a stem. **/
    data_STEMDIRECTION m_stemDir;
    /** Encodes the stem length. **/
    int m_stemLen;
    /** Records the position of the stem in relation to the note head(s). **/
    data_STEMPOSITION m_stemPos;
    /** Records the output x coordinate of the stem's attachment point. **/
    int m_stemX;
    /** Records the output y coordinate of the stem's attachment point. **/
    int m_stemY;

/* include <attstem.y> */
};

//----------------------------------------------------------------------------
// AttSylLog
//----------------------------------------------------------------------------

class AttSylLog: public Att
{
public:
    AttSylLog();
    virtual ~AttSylLog();
    
    /** Reset the default values for the attribute class **/
    void ResetSylLog();
    
    /** Read the values for the attribute class **/
    bool ReadSylLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSylLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCon(data_CON con_) { m_con = con_; };
    data_CON GetCon() const { return m_con; };    
    bool HasCon( );
    
    //
    void SetWordpos(data_WORDPOS wordpos_) { m_wordpos = wordpos_; };
    data_WORDPOS GetWordpos() const { return m_wordpos; };    
    bool HasWordpos( );
    
    ///@}

private:
    /**
     * Describes the symbols typically used to indicate breaks between syllables and
     * their functions.
     **/
    data_CON m_con;
    /** Records the position of a syllable within a word. **/
    data_WORDPOS m_wordpos;

/* include <attwordpos> */
};

//----------------------------------------------------------------------------
// AttSyltext
//----------------------------------------------------------------------------

class AttSyltext: public Att
{
public:
    AttSyltext();
    virtual ~AttSyltext();
    
    /** Reset the default values for the attribute class **/
    void ResetSyltext();
    
    /** Read the values for the attribute class **/
    bool ReadSyltext( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSyltext( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetSyl(std::string syl_) { m_syl = syl_; };
    std::string GetSyl() const { return m_syl; };    
    bool HasSyl( );
    
    ///@}

private:
    /** Holds an associated sung text syllable. **/
    std::string m_syl;

/* include <attsyl> */
};

//----------------------------------------------------------------------------
// AttTextstyle
//----------------------------------------------------------------------------

class AttTextstyle: public Att
{
public:
    AttTextstyle();
    virtual ~AttTextstyle();
    
    /** Reset the default values for the attribute class **/
    void ResetTextstyle();
    
    /** Read the values for the attribute class **/
    bool ReadTextstyle( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTextstyle( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTextFam(std::string textFam_) { m_textFam = textFam_; };
    std::string GetTextFam() const { return m_textFam; };    
    bool HasTextFam( );
    
    //
    void SetTextName(std::string textName_) { m_textName = textName_; };
    std::string GetTextName() const { return m_textName; };    
    bool HasTextName( );
    
    //
    void SetTextSize(std::string textSize_) { m_textSize = textSize_; };
    std::string GetTextSize() const { return m_textSize; };    
    bool HasTextSize( );
    
    //
    void SetTextStyle(std::string textStyle_) { m_textStyle = textStyle_; };
    std::string GetTextStyle() const { return m_textStyle; };    
    bool HasTextStyle( );
    
    //
    void SetTextWeight(std::string textWeight_) { m_textWeight = textWeight_; };
    std::string GetTextWeight() const { return m_textWeight; };    
    bool HasTextWeight( );
    
    ///@}

private:
    /**
     * Provides a default value for the font family name of text (other than lyrics)
     * when this information is not provided on the individual elements.
     **/
    std::string m_textFam;
    /**
     * Provides a default value for the font name of text (other than lyrics) when this
     * information is not provided on the individual elements.
     **/
    std::string m_textName;
    /**
     * Provides a default value for the font size of text (other than lyrics) when this
     * information is not provided on the individual elements.
     **/
    std::string m_textSize;
    /**
     * Provides a default value for the font style of text (other than lyrics) when
     * this information is not provided on the individual elements.
     **/
    std::string m_textStyle;
    /**
     * Provides a default value for the font weight for text (other than lyrics) when
     * this information is not provided on the individual elements.
     **/
    std::string m_textWeight;

/* include <atttext.weight> */
};

//----------------------------------------------------------------------------
// AttTiepresent
//----------------------------------------------------------------------------

class AttTiepresent: public Att
{
public:
    AttTiepresent();
    virtual ~AttTiepresent();
    
    /** Reset the default values for the attribute class **/
    void ResetTiepresent();
    
    /** Read the values for the attribute class **/
    bool ReadTiepresent( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTiepresent( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTie(data_TIE tie_) { m_tie = tie_; };
    data_TIE GetTie() const { return m_tie; };    
    bool HasTie( );
    
    ///@}

private:
    /**
     * Indicates that this element participates in a tie.
     * If visual information about the tie needs to be recorded, then a <tie> element
     * should be employed.
     **/
    data_TIE m_tie;

/* include <atttie> */
};

//----------------------------------------------------------------------------
// AttTimestampMusical
//----------------------------------------------------------------------------

class AttTimestampMusical: public Att
{
public:
    AttTimestampMusical();
    virtual ~AttTimestampMusical();
    
    /** Reset the default values for the attribute class **/
    void ResetTimestampMusical();
    
    /** Read the values for the attribute class **/
    bool ReadTimestampMusical( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTimestampMusical( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTstamp(double tstamp_) { m_tstamp = tstamp_; };
    double GetTstamp() const { return m_tstamp; };    
    bool HasTstamp( );
    
    ///@}

private:
    /**
     * Encodes the onset time in terms of musical time, i.e.,
     * beats[.fractional_beat_part].
     **/
    double m_tstamp;

/* include <atttstamp> */
};

//----------------------------------------------------------------------------
// AttTimestampPerformed
//----------------------------------------------------------------------------

class AttTimestampPerformed: public Att
{
public:
    AttTimestampPerformed();
    virtual ~AttTimestampPerformed();
    
    /** Reset the default values for the attribute class **/
    void ResetTimestampPerformed();
    
    /** Read the values for the attribute class **/
    bool ReadTimestampPerformed( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTimestampPerformed( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTstampGes(int tstampGesInt_) { m_tstampGesInt = tstampGesInt_; };
    int GetTstampGes() const { return m_tstampGesInt; };    
    bool HasTstampGes( );
    
    //
    void SetTstampReal(std::string tstampReal_) { m_tstampReal = tstampReal_; };
    std::string GetTstampReal() const { return m_tstampReal; };    
    bool HasTstampReal( );
    
    ///@}

private:
    /**
     * Used to record the onset time in pulses per quarter note (ppq, MusicXML
     * divisions, or MIDI clicks) since the start of the file.
     **/
    int m_tstampGesInt;
    /** Used to record the onset time in terms of ISO time since the start of the file. **/
    std::string m_tstampReal;

/* include <atttstamp.real> */
};

//----------------------------------------------------------------------------
// AttTimestamp2Musical
//----------------------------------------------------------------------------

class AttTimestamp2Musical: public Att
{
public:
    AttTimestamp2Musical();
    virtual ~AttTimestamp2Musical();
    
    /** Reset the default values for the attribute class **/
    void ResetTimestamp2Musical();
    
    /** Read the values for the attribute class **/
    bool ReadTimestamp2Musical( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTimestamp2Musical( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTstamp2(typedef_TSTAMP2 tstamp2_) { m_tstamp2 = tstamp2_; };
    typedef_TSTAMP2 GetTstamp2() const { return m_tstamp2; };    
    bool HasTstamp2( );
    
    ///@}

private:
    /**
     * Encodes the ending point of an event in terms of musical time, i.e., a count of
     * measures plus a beat location.
     **/
    typedef_TSTAMP2 m_tstamp2;

/* include <atttstamp2> */
};

//----------------------------------------------------------------------------
// AttTransposition
//----------------------------------------------------------------------------

class AttTransposition: public Att
{
public:
    AttTransposition();
    virtual ~AttTransposition();
    
    /** Reset the default values for the attribute class **/
    void ResetTransposition();
    
    /** Read the values for the attribute class **/
    bool ReadTransposition( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTransposition( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTransDiat(double transDiatDbl_) { m_transDiatDbl = transDiatDbl_; };
    double GetTransDiat() const { return m_transDiatDbl; };    
    bool HasTransDiat( );
    
    //
    void SetTransSemi(double transSemiDbl_) { m_transSemiDbl = transSemiDbl_; };
    double GetTransSemi() const { return m_transSemiDbl; };    
    bool HasTransSemi( );
    
    ///@}

private:
    /**
     * Records the amount of diatonic pitch shift, e.g., C to C♯ = 0, C to D♭ = 1,
     * necessary to calculate the sounded pitch from the written one.
     **/
    double m_transDiatDbl;
    /**
     * Records the amount of pitch shift in semitones, e.g., C to C♯ = 1, C to D♭ =
     * 1, necessary to calculate the sounded pitch from the written one.
     **/
    double m_transSemiDbl;

/* include <atttrans.semi> */
};

//----------------------------------------------------------------------------
// AttTupletpresent
//----------------------------------------------------------------------------

class AttTupletpresent: public Att
{
public:
    AttTupletpresent();
    virtual ~AttTupletpresent();
    
    /** Reset the default values for the attribute class **/
    void ResetTupletpresent();
    
    /** Read the values for the attribute class **/
    bool ReadTupletpresent( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTupletpresent( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTuplet(std::string tuplet_) { m_tuplet = tuplet_; };
    std::string GetTuplet() const { return m_tuplet; };    
    bool HasTuplet( );
    
    ///@}

private:
    /**
     * Indicates that this feature participates in a tuplet.
     * If visual information about the tuplet needs to be recorded, then a <tuplet>
     * element should be employed.
     **/
    std::string m_tuplet;

/* include <atttuplet> */
};

//----------------------------------------------------------------------------
// AttTyped
//----------------------------------------------------------------------------

class AttTyped: public Att
{
public:
    AttTyped();
    virtual ~AttTyped();
    
    /** Reset the default values for the attribute class **/
    void ResetTyped();
    
    /** Read the values for the attribute class **/
    bool ReadTyped( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTyped( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetType(std::string type_) { m_type = type_; };
    std::string GetType() const { return m_type; };    
    bool HasType( );
    
    //
    void SetSubtype(std::string subtype_) { m_subtype = subtype_; };
    std::string GetSubtype() const { return m_subtype; };    
    bool HasSubtype( );
    
    ///@}

private:
    /**
     * Characterizes the element in some sense, using any convenient classification
     * scheme or typology.
     **/
    std::string m_type;
    /**
     * Provide any sub-classification for the element, additional to that given by its
     * type attribute.
     **/
    std::string m_subtype;

/* include <attsubtype> */
};

//----------------------------------------------------------------------------
// AttTypography
//----------------------------------------------------------------------------

class AttTypography: public Att
{
public:
    AttTypography();
    virtual ~AttTypography();
    
    /** Reset the default values for the attribute class **/
    void ResetTypography();
    
    /** Read the values for the attribute class **/
    bool ReadTypography( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTypography( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetFontfam(std::string fontfam_) { m_fontfam = fontfam_; };
    std::string GetFontfam() const { return m_fontfam; };    
    bool HasFontfam( );
    
    //
    void SetFontname(std::string fontname_) { m_fontname = fontname_; };
    std::string GetFontname() const { return m_fontname; };    
    bool HasFontname( );
    
    //
    void SetFontsize(double fontsizeDbl_) { m_fontsizeDbl = fontsizeDbl_; };
    double GetFontsize() const { return m_fontsizeDbl; };    
    bool HasFontsize( );
    
    //
    void SetFontstyle(data_FONTSTYLE fontstyle_) { m_fontstyle = fontstyle_; };
    data_FONTSTYLE GetFontstyle() const { return m_fontstyle; };    
    bool HasFontstyle( );
    
    //
    void SetFontweight(data_FONTWEIGHT fontweight_) { m_fontweight = fontweight_; };
    data_FONTWEIGHT GetFontweight() const { return m_fontweight; };    
    bool HasFontweight( );
    
    ///@}

private:
    /** Contains the name of a font-family. **/
    std::string m_fontfam;
    /** Holds the name of a font. **/
    std::string m_fontname;
    /** Indicates the size of a font in printers' points, i.e., 1/72nd of an inch. **/
    double m_fontsizeDbl;
    /** Records the style of a font, i.e, italic, oblique, or normal. **/
    data_FONTSTYLE m_fontstyle;
    /** Used to indicate bold type. **/
    data_FONTWEIGHT m_fontweight;

/* include <attfontweight> */
};

//----------------------------------------------------------------------------
// AttVisibility
//----------------------------------------------------------------------------

class AttVisibility: public Att
{
public:
    AttVisibility();
    virtual ~AttVisibility();
    
    /** Reset the default values for the attribute class **/
    void ResetVisibility();
    
    /** Read the values for the attribute class **/
    bool ReadVisibility( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteVisibility( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetVisible(std::string visible_) { m_visible = visible_; };
    std::string GetVisible() const { return m_visible; };    
    bool HasVisible( );
    
    ///@}

private:
    /**
     * Indicates if a feature should be rendered when the notation is presented
     * graphically or sounded when it is presented in an aural form.
     **/
    std::string m_visible;

/* include <attvisible> */
};

//----------------------------------------------------------------------------
// AttVisualoffsetHo
//----------------------------------------------------------------------------

class AttVisualoffsetHo: public Att
{
public:
    AttVisualoffsetHo();
    virtual ~AttVisualoffsetHo();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetHo();
    
    /** Read the values for the attribute class **/
    bool ReadVisualoffsetHo( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteVisualoffsetHo( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetHo(std::string ho_) { m_ho = ho_; };
    std::string GetHo() const { return m_ho; };    
    bool HasHo( );
    
    ///@}

private:
    /**
     * Records a horizontal adjustment to a feature's programmatically-determined
     * location in terms of staff interline distance; that is, in units of 1/2 the
     * distance between adjacent staff lines.
     **/
    std::string m_ho;

/* include <attho> */
};

//----------------------------------------------------------------------------
// AttVisualoffsetTo
//----------------------------------------------------------------------------

class AttVisualoffsetTo: public Att
{
public:
    AttVisualoffsetTo();
    virtual ~AttVisualoffsetTo();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetTo();
    
    /** Read the values for the attribute class **/
    bool ReadVisualoffsetTo( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteVisualoffsetTo( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTo(std::string to_) { m_to = to_; };
    std::string GetTo() const { return m_to; };    
    bool HasTo( );
    
    ///@}

private:
    /**
     * Records a timestamp adjustment of a feature's programmatically-determined
     * location in terms of musical time; that is, beats.
     **/
    std::string m_to;

/* include <attto> */
};

//----------------------------------------------------------------------------
// AttVisualoffsetVo
//----------------------------------------------------------------------------

class AttVisualoffsetVo: public Att
{
public:
    AttVisualoffsetVo();
    virtual ~AttVisualoffsetVo();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetVo();
    
    /** Read the values for the attribute class **/
    bool ReadVisualoffsetVo( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteVisualoffsetVo( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetVo(std::string vo_) { m_vo = vo_; };
    std::string GetVo() const { return m_vo; };    
    bool HasVo( );
    
    ///@}

private:
    /**
     * Records the vertical adjustment of a feature's programmatically-determined
     * location in terms of staff interline distance; that is, in units of 1/2 the
     * distance between adjacent staff lines.
     **/
    std::string m_vo;

/* include <attvo> */
};

//----------------------------------------------------------------------------
// AttVisualoffset2Ho
//----------------------------------------------------------------------------

class AttVisualoffset2Ho: public Att
{
public:
    AttVisualoffset2Ho();
    virtual ~AttVisualoffset2Ho();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2Ho();
    
    /** Read the values for the attribute class **/
    bool ReadVisualoffset2Ho( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteVisualoffset2Ho( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetStartho(std::string startho_) { m_startho = startho_; };
    std::string GetStartho() const { return m_startho; };    
    bool HasStartho( );
    
    //
    void SetEndho(std::string endho_) { m_endho = endho_; };
    std::string GetEndho() const { return m_endho; };    
    bool HasEndho( );
    
    ///@}

private:
    /**
     * Records the horizontal adjustment of a feature's programmatically-determined
     * start point.
     **/
    std::string m_startho;
    /**
     * Records the horizontal adjustment of a feature's programmatically-determined end
     * point.
     **/
    std::string m_endho;

/* include <attendho> */
};

//----------------------------------------------------------------------------
// AttVisualoffset2To
//----------------------------------------------------------------------------

class AttVisualoffset2To: public Att
{
public:
    AttVisualoffset2To();
    virtual ~AttVisualoffset2To();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2To();
    
    /** Read the values for the attribute class **/
    bool ReadVisualoffset2To( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteVisualoffset2To( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetStartto(std::string startto_) { m_startto = startto_; };
    std::string GetStartto() const { return m_startto; };    
    bool HasStartto( );
    
    //
    void SetEndto(std::string endto_) { m_endto = endto_; };
    std::string GetEndto() const { return m_endto; };    
    bool HasEndto( );
    
    ///@}

private:
    /**
     * Records a timestamp adjustment of a feature's programmatically-determined start
     * point.
     **/
    std::string m_startto;
    /**
     * Records a timestamp adjustment of a feature's programmatically-determined end
     * point.
     **/
    std::string m_endto;

/* include <attendto> */
};

//----------------------------------------------------------------------------
// AttVisualoffset2Vo
//----------------------------------------------------------------------------

class AttVisualoffset2Vo: public Att
{
public:
    AttVisualoffset2Vo();
    virtual ~AttVisualoffset2Vo();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2Vo();
    
    /** Read the values for the attribute class **/
    bool ReadVisualoffset2Vo( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteVisualoffset2Vo( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetStartvo(std::string startvo_) { m_startvo = startvo_; };
    std::string GetStartvo() const { return m_startvo; };    
    bool HasStartvo( );
    
    //
    void SetEndvo(std::string endvo_) { m_endvo = endvo_; };
    std::string GetEndvo() const { return m_endvo; };    
    bool HasEndvo( );
    
    ///@}

private:
    /**
     * Records a vertical adjustment of a feature's programmatically-determined start
     * point.
     **/
    std::string m_startvo;
    /**
     * Records a vertical adjustment of a feature's programmatically-determined end
     * point.
     **/
    std::string m_endvo;

/* include <attendvo> */
};

//----------------------------------------------------------------------------
// AttWhitespace
//----------------------------------------------------------------------------

class AttWhitespace: public Att
{
public:
    AttWhitespace();
    virtual ~AttWhitespace();
    
    /** Reset the default values for the attribute class **/
    void ResetWhitespace();
    
    /** Read the values for the attribute class **/
    bool ReadWhitespace( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteWhitespace( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetSpace(std::string space_) { m_space = space_; };
    std::string GetSpace() const { return m_space; };    
    bool HasSpace( );
    
    ///@}

private:
    /** --- **/
    std::string m_space;

/* include <attspace> */
};

//----------------------------------------------------------------------------
// AttWidth
//----------------------------------------------------------------------------

class AttWidth: public Att
{
public:
    AttWidth();
    virtual ~AttWidth();
    
    /** Reset the default values for the attribute class **/
    void ResetWidth();
    
    /** Read the values for the attribute class **/
    bool ReadWidth( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteWidth( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetWidth(std::string width_) { m_width = width_; };
    std::string GetWidth() const { return m_width; };    
    bool HasWidth( );
    
    ///@}

private:
    /**
     * Measurement of the horizontal dimension of an entity.
     * The width attribute may be used to capture measure width data for interchange
     * with music printing systems that utilize this information for printing. On
     * <barLine> the width attribute captures the width of the preceding measure.
     **/
    std::string m_width;

/* include <attwidth> */
};

//----------------------------------------------------------------------------
// AttXy
//----------------------------------------------------------------------------

class AttXy: public Att
{
public:
    AttXy();
    virtual ~AttXy();
    
    /** Reset the default values for the attribute class **/
    void ResetXy();
    
    /** Read the values for the attribute class **/
    bool ReadXy( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteXy( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetX(double xDbl_) { m_xDbl = xDbl_; };
    double GetX() const { return m_xDbl; };    
    bool HasX( );
    
    //
    void SetY(double yDbl_) { m_yDbl = yDbl_; };
    double GetY() const { return m_yDbl; };    
    bool HasY( );
    
    ///@}

private:
    /**
     * Encodes an x coordinate for a feature in an output coordinate system.
     * When it is necessary to record the placement of a feature in a facsimile image,
     * use the facs attribute.
     **/
    double m_xDbl;
    /**
     * Encodes an y coordinate for a feature in an output coordinate system.
     * When it is necessary to record the placement of a feature in a facsimile image,
     * use the facs attribute.
     **/
    double m_yDbl;

/* include <atty> */
};

//----------------------------------------------------------------------------
// AttXy2
//----------------------------------------------------------------------------

class AttXy2: public Att
{
public:
    AttXy2();
    virtual ~AttXy2();
    
    /** Reset the default values for the attribute class **/
    void ResetXy2();
    
    /** Read the values for the attribute class **/
    bool ReadXy2( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteXy2( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetX2(double x2Dbl_) { m_x2Dbl = x2Dbl_; };
    double GetX2() const { return m_x2Dbl; };    
    bool HasX2( );
    
    //
    void SetY2(double y2Dbl_) { m_y2Dbl = y2Dbl_; };
    double GetY2() const { return m_y2Dbl; };    
    bool HasY2( );
    
    ///@}

private:
    /** Encodes the optional 2nd x coordinate. **/
    double m_x2Dbl;
    /** Encodes the optional 2nd y coordinate. **/
    double m_y2Dbl;

/* include <atty2> */
};

} // vrv namespace

#endif  // __VRV_ATTS_SHARED_H__

