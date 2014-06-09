/////////////////////////////////////////////////////////////////////////////
// Name:        attributes.h
// Author:      Laurent Pugin
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

#ifndef __VRV_SHAREDMIXIN_H__
#define __VRV_SHAREDMIXIN_H__

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAccidLog
//----------------------------------------------------------------------------

class AttAccidLog 
{
public:
    AttAccidLog();
    virtual ~AttAccidLog();
    
    /** Reset the default values for the attribute class **/
    void ResetAccidLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetFunc(std::string func_) { m_func = func_; };
    std::string GetFunc() { return m_func; };
    ///@}

protected:
    /** Records the function of an accidental. **/
    std::string m_func;

/* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttAccidental
//----------------------------------------------------------------------------

class AttAccidental 
{
public:
    AttAccidental();
    virtual ~AttAccidental();
    
    /** Reset the default values for the attribute class **/
    void ResetAccidental();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetAccid(std::string accid_) { m_accid = accid_; };
    std::string GetAccid() { return m_accid; };
    ///@}

protected:
    /** Captures a written accidental. **/
    std::string m_accid;

/* include <attaccid> */
};

//----------------------------------------------------------------------------
// AttAccidentalPerformed
//----------------------------------------------------------------------------

class AttAccidentalPerformed 
{
public:
    AttAccidentalPerformed();
    virtual ~AttAccidentalPerformed();
    
    /** Reset the default values for the attribute class **/
    void ResetAccidentalPerformed();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetAccidGes(std::string accidGes_) { m_accidGes = accidGes_; };
    std::string GetAccidGes() { return m_accidGes; };
    ///@}

protected:
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

class AttAltsym 
{
public:
    AttAltsym();
    virtual ~AttAltsym();
    
    /** Reset the default values for the attribute class **/
    void ResetAltsym();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetAltsym(std::string altsym_) { m_altsym = altsym_; };
    std::string GetAltsym() { return m_altsym; };
    ///@}

protected:
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

class AttArticulation 
{
public:
    AttArticulation();
    virtual ~AttArticulation();
    
    /** Reset the default values for the attribute class **/
    void ResetArticulation();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetArtic(std::string artic_) { m_artic = artic_; };
    std::string GetArtic() { return m_artic; };
    ///@}

protected:
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

class AttArticulationPerformed 
{
public:
    AttArticulationPerformed();
    virtual ~AttArticulationPerformed();
    
    /** Reset the default values for the attribute class **/
    void ResetArticulationPerformed();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetArticGes(std::string articGes_) { m_articGes = articGes_; };
    std::string GetArticGes() { return m_articGes; };
    ///@}

protected:
    /** Records performed articulation that differs from the written value. **/
    std::string m_articGes;

/* include <attartic.ges> */
};

//----------------------------------------------------------------------------
// AttAugmentdots
//----------------------------------------------------------------------------

class AttAugmentdots 
{
public:
    AttAugmentdots();
    virtual ~AttAugmentdots();
    
    /** Reset the default values for the attribute class **/
    void ResetAugmentdots();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDots(std::string dots_) { m_dots = dots_; };
    std::string GetDots() { return m_dots; };
    ///@}

protected:
    /** Records the number of augmentation dots required by a dotted duration. **/
    std::string m_dots;

/* include <attdots> */
};

//----------------------------------------------------------------------------
// AttAuthorized
//----------------------------------------------------------------------------

class AttAuthorized 
{
public:
    AttAuthorized();
    virtual ~AttAuthorized();
    
    /** Reset the default values for the attribute class **/
    void ResetAuthorized();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetAuthority(std::string authority_) { m_authority = authority_; };
    std::string GetAuthority() { return m_authority; };
    //
    void SetAuthURI(std::string authURI_) { m_authURI = authURI_; };
    std::string GetAuthURI() { return m_authURI; };
    ///@}

protected:
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

class AttBarLineLog 
{
public:
    AttBarLineLog();
    virtual ~AttBarLineLog();
    
    /** Reset the default values for the attribute class **/
    void ResetBarLineLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRend(std::string rend_) { m_rend = rend_; };
    std::string GetRend() { return m_rend; };
    ///@}

protected:
    /** Describes the line style of the curve. **/
    std::string m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttBarplacement
//----------------------------------------------------------------------------

class AttBarplacement 
{
public:
    AttBarplacement();
    virtual ~AttBarplacement();
    
    /** Reset the default values for the attribute class **/
    void ResetBarplacement();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBarplace(std::string barplace_) { m_barplace = barplace_; };
    std::string GetBarplace() { return m_barplace; };
    //
    void SetTaktplace(std::string taktplace_) { m_taktplace = taktplace_; };
    std::string GetTaktplace() { return m_taktplace; };
    ///@}

protected:
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

class AttBeamingVis 
{
public:
    AttBeamingVis();
    virtual ~AttBeamingVis();
    
    /** Reset the default values for the attribute class **/
    void ResetBeamingVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBeamRend(std::string beamRend_) { m_beamRend = beamRend_; };
    std::string GetBeamRend() { return m_beamRend; };
    //
    void SetBeamSlopeDbl(double beamSlopeDbl_) { m_beamSlopeDbl = beamSlopeDbl_; };
    double GetBeamSlopeDbl() { return m_beamSlopeDbl; };
    ///@}

protected:
    /** Encodes whether a beam is "feathered" and in which direction. **/
    std::string m_beamRend;
    /** Captures beam slope. **/
    double m_beamSlopeDbl;

/* include <attbeam.slope> */
};

//----------------------------------------------------------------------------
// AttBibl
//----------------------------------------------------------------------------

class AttBibl 
{
public:
    AttBibl();
    virtual ~AttBibl();
    
    /** Reset the default values for the attribute class **/
    void ResetBibl();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetAnalog(std::string analog_) { m_analog = analog_; };
    std::string GetAnalog() { return m_analog; };
    ///@}

protected:
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

class AttCalendared 
{
public:
    AttCalendared();
    virtual ~AttCalendared();
    
    /** Reset the default values for the attribute class **/
    void ResetCalendared();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCalendar(std::string calendar_) { m_calendar = calendar_; };
    std::string GetCalendar() { return m_calendar; };
    ///@}

protected:
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

class AttCanonical 
{
public:
    AttCanonical();
    virtual ~AttCanonical();
    
    /** Reset the default values for the attribute class **/
    void ResetCanonical();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDbkey(std::string dbkey_) { m_dbkey = dbkey_; };
    std::string GetDbkey() { return m_dbkey; };
    ///@}

protected:
    /** Used to record a value which serves as a primary key in an external database. **/
    std::string m_dbkey;

/* include <attdbkey> */
};

//----------------------------------------------------------------------------
// AttChordVis
//----------------------------------------------------------------------------

class AttChordVis 
{
public:
    AttChordVis();
    virtual ~AttChordVis();
    
    /** Reset the default values for the attribute class **/
    void ResetChordVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCluster(std::string cluster_) { m_cluster = cluster_; };
    std::string GetCluster() { return m_cluster; };
    ///@}

protected:
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

class AttClefLog 
{
public:
    AttClefLog();
    virtual ~AttClefLog();
    
    /** Reset the default values for the attribute class **/
    void ResetClefLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCautionary(std::string cautionary_) { m_cautionary = cautionary_; };
    std::string GetCautionary() { return m_cautionary; };
    ///@}

protected:
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

class AttCleffingLog 
{
public:
    AttCleffingLog();
    virtual ~AttCleffingLog();
    
    /** Reset the default values for the attribute class **/
    void ResetCleffingLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetClefShape(std::string clefShape_) { m_clefShape = clefShape_; };
    std::string GetClefShape() { return m_clefShape; };
    //
    void SetClefLine(std::string clefLine_) { m_clefLine = clefLine_; };
    std::string GetClefLine() { return m_clefLine; };
    //
    void SetClefDis(std::string clefDis_) { m_clefDis = clefDis_; };
    std::string GetClefDis() { return m_clefDis; };
    //
    void SetClefDisPlace(std::string clefDisPlace_) { m_clefDisPlace = clefDisPlace_; };
    std::string GetClefDisPlace() { return m_clefDisPlace; };
    ///@}

protected:
    /** Encodes a value for the clef symbol. **/
    std::string m_clefShape;
    /**
     * Contains a default value for the position of the clef.
     * The value must be in the range between 1 and the number of lines on the staff.
     * The numbering of lines starts with the lowest line of the staff.
     **/
    std::string m_clefLine;
    /** Records the amount of octave displacement to be applied to the clef. **/
    std::string m_clefDis;
    /** Records the direction of octave displacement to be applied to the clef. **/
    std::string m_clefDisPlace;

/* include <attclef.dis.place> */
};

//----------------------------------------------------------------------------
// AttCleffingVis
//----------------------------------------------------------------------------

class AttCleffingVis 
{
public:
    AttCleffingVis();
    virtual ~AttCleffingVis();
    
    /** Reset the default values for the attribute class **/
    void ResetCleffingVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetClefColor(std::string clefColor_) { m_clefColor = clefColor_; };
    std::string GetClefColor() { return m_clefColor; };
    //
    void SetClefVisible(std::string clefVisible_) { m_clefVisible = clefVisible_; };
    std::string GetClefVisible() { return m_clefVisible; };
    ///@}

protected:
    /** Describes the color of the clef. **/
    std::string m_clefColor;
    /** Determines whether the clef is to be displayed. **/
    std::string m_clefVisible;

/* include <attclef.visible> */
};

//----------------------------------------------------------------------------
// AttClefshape
//----------------------------------------------------------------------------

class AttClefshape 
{
public:
    AttClefshape();
    virtual ~AttClefshape();
    
    /** Reset the default values for the attribute class **/
    void ResetClefshape();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetShape(std::string shape_) { m_shape = shape_; };
    std::string GetShape() { return m_shape; };
    ///@}

protected:
    /** Describes a clef's shape. **/
    std::string m_shape;

/* include <attshape> */
};

//----------------------------------------------------------------------------
// AttColor
//----------------------------------------------------------------------------

class AttColor 
{
public:
    AttColor();
    virtual ~AttColor();
    
    /** Reset the default values for the attribute class **/
    void ResetColor();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetColor(std::string color_) { m_color = color_; };
    std::string GetColor() { return m_color; };
    ///@}

protected:
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

class AttColoration 
{
public:
    AttColoration();
    virtual ~AttColoration();
    
    /** Reset the default values for the attribute class **/
    void ResetColoration();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetColored(std::string colored_) { m_colored = colored_; };
    std::string GetColored() { return m_colored; };
    ///@}

protected:
    /**
     * Indicates this feature is 'colored'; that is, it is a participant in a change in
     * rhythmic values.
     * In mensural notation, coloration is indicated by colored notes (red, black,
     * etc.) where void notes would otherwise occur. In CMN, coloration is indicated by
     * an inverse color; that is, the note head is void when it would otherwise be
     * filled and vice versa.
     **/
    std::string m_colored;

/* include <attcolored> */
};

//----------------------------------------------------------------------------
// AttCommon
//----------------------------------------------------------------------------

class AttCommon 
{
public:
    AttCommon();
    virtual ~AttCommon();
    
    /** Reset the default values for the attribute class **/
    void ResetCommon();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLabel(std::string label_) { m_label = label_; };
    std::string GetLabel() { return m_label; };
    //
    void SetN(int n_) { m_n = n_; };
    int GetN() { return m_n; };
    //
    void SetBase(std::string base_) { m_base = base_; };
    std::string GetBase() { return m_base; };
    ///@}

protected:
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

class AttCoordinated 
{
public:
    AttCoordinated();
    virtual ~AttCoordinated();
    
    /** Reset the default values for the attribute class **/
    void ResetCoordinated();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetUlxInt(int ulxInt_) { m_ulxInt = ulxInt_; };
    int GetUlxInt() { return m_ulxInt; };
    //
    void SetUlyInt(int ulyInt_) { m_ulyInt = ulyInt_; };
    int GetUlyInt() { return m_ulyInt; };
    //
    void SetLrxInt(int lrxInt_) { m_lrxInt = lrxInt_; };
    int GetLrxInt() { return m_lrxInt; };
    //
    void SetLryInt(int lryInt_) { m_lryInt = lryInt_; };
    int GetLryInt() { return m_lryInt; };
    ///@}

protected:
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

class AttCurvature 
{
public:
    AttCurvature();
    virtual ~AttCurvature();
    
    /** Reset the default values for the attribute class **/
    void ResetCurvature();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBezier(std::string bezier_) { m_bezier = bezier_; };
    std::string GetBezier() { return m_bezier; };
    //
    void SetBulge(std::string bulge_) { m_bulge = bulge_; };
    std::string GetBulge() { return m_bulge; };
    //
    void SetCurvedir(std::string curvedir_) { m_curvedir = curvedir_; };
    std::string GetCurvedir() { return m_curvedir; };
    ///@}

protected:
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

class AttCurverend 
{
public:
    AttCurverend();
    virtual ~AttCurverend();
    
    /** Reset the default values for the attribute class **/
    void ResetCurverend();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRend(std::string rend_) { m_rend = rend_; };
    std::string GetRend() { return m_rend; };
    ///@}

protected:
    /** Describes the line style of the curve. **/
    std::string m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttCustosLog
//----------------------------------------------------------------------------

class AttCustosLog 
{
public:
    AttCustosLog();
    virtual ~AttCustosLog();
    
    /** Reset the default values for the attribute class **/
    void ResetCustosLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTarget(std::string target_) { m_target = target_; };
    std::string GetTarget() { return m_target; };
    ///@}

protected:
    /**
     * Allows the use of one or more previously-undeclared URIs to identify an external
     * electronic object.
     **/
    std::string m_target;

/* include <atttarget> */
};

//----------------------------------------------------------------------------
// AttDatable
//----------------------------------------------------------------------------

class AttDatable 
{
public:
    AttDatable();
    virtual ~AttDatable();
    
    /** Reset the default values for the attribute class **/
    void ResetDatable();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetEnddate(std::string enddate_) { m_enddate = enddate_; };
    std::string GetEnddate() { return m_enddate; };
    //
    void SetIsodate(std::string isodate_) { m_isodate = isodate_; };
    std::string GetIsodate() { return m_isodate; };
    //
    void SetNotafter(std::string notafter_) { m_notafter = notafter_; };
    std::string GetNotafter() { return m_notafter; };
    //
    void SetNotbefore(std::string notbefore_) { m_notbefore = notbefore_; };
    std::string GetNotbefore() { return m_notbefore; };
    //
    void SetStartdate(std::string startdate_) { m_startdate = startdate_; };
    std::string GetStartdate() { return m_startdate; };
    ///@}

protected:
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

class AttDatapointing 
{
public:
    AttDatapointing();
    virtual ~AttDatapointing();
    
    /** Reset the default values for the attribute class **/
    void ResetDatapointing();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetData(std::string data_) { m_data = data_; };
    std::string GetData() { return m_data; };
    ///@}

protected:
    /** Used to link metadata elements to one or more data-containing elements. **/
    std::string m_data;

/* include <attdata> */
};

//----------------------------------------------------------------------------
// AttDeclaring
//----------------------------------------------------------------------------

class AttDeclaring 
{
public:
    AttDeclaring();
    virtual ~AttDeclaring();
    
    /** Reset the default values for the attribute class **/
    void ResetDeclaring();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDecls(std::string decls_) { m_decls = decls_; };
    std::string GetDecls() { return m_decls; };
    ///@}

protected:
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

class AttDistances 
{
public:
    AttDistances();
    virtual ~AttDistances();
    
    /** Reset the default values for the attribute class **/
    void ResetDistances();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDynamDistDbl(double dynamDistDbl_) { m_dynamDistDbl = dynamDistDbl_; };
    double GetDynamDistDbl() { return m_dynamDistDbl; };
    //
    void SetHarmDistDbl(double harmDistDbl_) { m_harmDistDbl = harmDistDbl_; };
    double GetHarmDistDbl() { return m_harmDistDbl; };
    //
    void SetTextDistDbl(double textDistDbl_) { m_textDistDbl = textDistDbl_; };
    double GetTextDistDbl() { return m_textDistDbl; };
    ///@}

protected:
    /** Records the distance from the staff for dynamic marks in 1/2 inter-line units. **/
    double m_dynamDistDbl;
    /**
     * Records the default distance from the staff of harmonic indications, such as
     * guitar chord grids or functional labels.
     **/
    double m_harmDistDbl;
    /** Determines how far from the staff to render text elements. **/
    double m_textDistDbl;

/* include <atttext.dist> */
};

//----------------------------------------------------------------------------
// AttDotLog
//----------------------------------------------------------------------------

class AttDotLog 
{
public:
    AttDotLog();
    virtual ~AttDotLog();
    
    /** Reset the default values for the attribute class **/
    void ResetDotLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() { return m_form; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;

/* include <attform> */
};

//----------------------------------------------------------------------------
// AttDurationDefault
//----------------------------------------------------------------------------

class AttDurationDefault 
{
public:
    AttDurationDefault();
    virtual ~AttDurationDefault();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationDefault();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDurDefault(std::string durDefault_) { m_durDefault = durDefault_; };
    std::string GetDurDefault() { return m_durDefault; };
    ///@}

protected:
    /**
     * Contains a default duration in those situations when the first note, rest,
     * chord, etc.
     * in a measure does not have a duration specified.
     **/
    std::string m_durDefault;

/* include <attdur.default> */
};

//----------------------------------------------------------------------------
// AttDurationMusical
//----------------------------------------------------------------------------

class AttDurationMusical 
{
public:
    AttDurationMusical();
    virtual ~AttDurationMusical();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationMusical();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDur(std::string dur_) { m_dur = dur_; };
    std::string GetDur() { return m_dur; };
    ///@}

protected:
    /**
     * Records the duration of a feature using the relative durational values provided
     * by the data.DURATION datatype.
     **/
    std::string m_dur;

/* include <attdur> */
};

//----------------------------------------------------------------------------
// AttDurationPerformed
//----------------------------------------------------------------------------

class AttDurationPerformed 
{
public:
    AttDurationPerformed();
    virtual ~AttDurationPerformed();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationPerformed();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDurGes(std::string durGes_) { m_durGes = durGes_; };
    std::string GetDurGes() { return m_durGes; };
    ///@}

protected:
    /**
     * Records performed duration information that differs from written duration.
     * Its value may be expressed in any convenient form, such as measures[s]+
     * beat[s].beatpart, ppq (MIDI clicks), Humdrum **recip values, or MusicXML
     * 'divisions', etc.
     **/
    std::string m_durGes;

/* include <attdur.ges> */
};

//----------------------------------------------------------------------------
// AttDurationRatio
//----------------------------------------------------------------------------

class AttDurationRatio 
{
public:
    AttDurationRatio();
    virtual ~AttDurationRatio();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationRatio();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetNumInt(int numInt_) { m_numInt = numInt_; };
    int GetNumInt() { return m_numInt; };
    //
    void SetNumbaseInt(int numbaseInt_) { m_numbaseInt = numbaseInt_; };
    int GetNumbaseInt() { return m_numbaseInt; };
    ///@}

protected:
    /**
     * Along with numbase, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_numInt;
    /**
     * Along with num, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_numbaseInt;

/* include <attnumbase> */
};

//----------------------------------------------------------------------------
// AttDurationTimestamp
//----------------------------------------------------------------------------

class AttDurationTimestamp 
{
public:
    AttDurationTimestamp();
    virtual ~AttDurationTimestamp();
    
    /** Reset the default values for the attribute class **/
    void ResetDurationTimestamp();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDur(std::string dur_) { m_dur = dur_; };
    std::string GetDur() { return m_dur; };
    ///@}

protected:
    /**
     * Records the duration of a feature using the relative durational values provided
     * by the data.DURATION datatype.
     **/
    std::string m_dur;

/* include <attdur> */
};

//----------------------------------------------------------------------------
// AttEnclosingchars
//----------------------------------------------------------------------------

class AttEnclosingchars 
{
public:
    AttEnclosingchars();
    virtual ~AttEnclosingchars();
    
    /** Reset the default values for the attribute class **/
    void ResetEnclosingchars();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetEnclose(std::string enclose_) { m_enclose = enclose_; };
    std::string GetEnclose() { return m_enclose; };
    ///@}

protected:
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

class AttFermatapresent 
{
public:
    AttFermatapresent();
    virtual ~AttFermatapresent();
    
    /** Reset the default values for the attribute class **/
    void ResetFermatapresent();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetFermata(std::string fermata_) { m_fermata = fermata_; };
    std::string GetFermata() { return m_fermata; };
    ///@}

protected:
    /**
     * Indicates the attachment of a fermata to this element.
     * If visual information about the fermata needs to be recorded, then a <fermata>
     * element should be employed instead.
     **/
    std::string m_fermata;

/* include <attfermata> */
};

//----------------------------------------------------------------------------
// AttHandident
//----------------------------------------------------------------------------

class AttHandident 
{
public:
    AttHandident();
    virtual ~AttHandident();
    
    /** Reset the default values for the attribute class **/
    void ResetHandident();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetHand(std::string hand_) { m_hand = hand_; };
    std::string GetHand() { return m_hand; };
    ///@}

protected:
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

class AttHorizontalalign 
{
public:
    AttHorizontalalign();
    virtual ~AttHorizontalalign();
    
    /** Reset the default values for the attribute class **/
    void ResetHorizontalalign();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetHalign(std::string halign_) { m_halign = halign_; };
    std::string GetHalign() { return m_halign; };
    ///@}

protected:
    /** Records horizontal alignment. **/
    std::string m_halign;

/* include <atthalign> */
};

//----------------------------------------------------------------------------
// AttInstrumentident
//----------------------------------------------------------------------------

class AttInstrumentident 
{
public:
    AttInstrumentident();
    virtual ~AttInstrumentident();
    
    /** Reset the default values for the attribute class **/
    void ResetInstrumentident();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetInstr(std::string instr_) { m_instr = instr_; };
    std::string GetInstr() { return m_instr; };
    ///@}

protected:
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

class AttInternetmedia 
{
public:
    AttInternetmedia();
    virtual ~AttInternetmedia();
    
    /** Reset the default values for the attribute class **/
    void ResetInternetmedia();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMimetype(std::string mimetype_) { m_mimetype = mimetype_; };
    std::string GetMimetype() { return m_mimetype; };
    ///@}

protected:
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

class AttJoined 
{
public:
    AttJoined();
    virtual ~AttJoined();
    
    /** Reset the default values for the attribute class **/
    void ResetJoined();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetJoin(std::string join_) { m_join = join_; };
    std::string GetJoin() { return m_join; };
    ///@}

protected:
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

class AttKeySigLog 
{
public:
    AttKeySigLog();
    virtual ~AttKeySigLog();
    
    /** Reset the default values for the attribute class **/
    void ResetKeySigLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMode(std::string mode_) { m_mode = mode_; };
    std::string GetMode() { return m_mode; };
    ///@}

protected:
    /** Indicates major, minor, or other tonality. **/
    std::string m_mode;

/* include <attmode> */
};

//----------------------------------------------------------------------------
// AttKeySigDefaultLog
//----------------------------------------------------------------------------

class AttKeySigDefaultLog 
{
public:
    AttKeySigDefaultLog();
    virtual ~AttKeySigDefaultLog();
    
    /** Reset the default values for the attribute class **/
    void ResetKeySigDefaultLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetKeyAccid(std::string keyAccid_) { m_keyAccid = keyAccid_; };
    std::string GetKeyAccid() { return m_keyAccid; };
    //
    void SetKeyMode(std::string keyMode_) { m_keyMode = keyMode_; };
    std::string GetKeyMode() { return m_keyMode; };
    //
    void SetKeyPname(std::string keyPname_) { m_keyPname = keyPname_; };
    std::string GetKeyPname() { return m_keyPname; };
    //
    void SetKeySig(std::string keySig_) { m_keySig = keySig_; };
    std::string GetKeySig() { return m_keySig; };
    //
    void SetKeySigMixed(std::string keySigMixed_) { m_keySigMixed = keySigMixed_; };
    std::string GetKeySigMixed() { return m_keySigMixed; };
    ///@}

protected:
    /**
     * Contains an accidental for the tonic key, if one is required, e.g., if key.pname
     * equals 'c' and key.accid equals 's', then a tonic of C# is indicated.
     **/
    std::string m_keyAccid;
    /** Indicates major, minor, or other tonality. **/
    std::string m_keyMode;
    /**
     * Holds the pitch name of the tonic key, e.g.
     * 'c' for the key of C.
     **/
    std::string m_keyPname;
    /** Indicates where the key lies in the circle of fifths. **/
    std::string m_keySig;
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

class AttKeySigDefaultVis 
{
public:
    AttKeySigDefaultVis();
    virtual ~AttKeySigDefaultVis();
    
    /** Reset the default values for the attribute class **/
    void ResetKeySigDefaultVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetKeySigShow(std::string keySigShow_) { m_keySigShow = keySigShow_; };
    std::string GetKeySigShow() { return m_keySigShow; };
    //
    void SetKeySigShowchange(std::string keySigShowchange_) { m_keySigShowchange = keySigShowchange_; };
    std::string GetKeySigShowchange() { return m_keySigShowchange; };
    ///@}

protected:
    /** Indicates whether the key signature should be displayed. **/
    std::string m_keySigShow;
    /** Determines whether cautionary accidentals should be displayed at a key change. **/
    std::string m_keySigShowchange;

/* include <attkey.sig.showchange> */
};

//----------------------------------------------------------------------------
// AttLabelsAddl
//----------------------------------------------------------------------------

class AttLabelsAddl 
{
public:
    AttLabelsAddl();
    virtual ~AttLabelsAddl();
    
    /** Reset the default values for the attribute class **/
    void ResetLabelsAddl();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLabelAbbr(std::string labelAbbr_) { m_labelAbbr = labelAbbr_; };
    std::string GetLabelAbbr() { return m_labelAbbr; };
    ///@}

protected:
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

class AttLang 
{
public:
    AttLang();
    virtual ~AttLang();
    
    /** Reset the default values for the attribute class **/
    void ResetLang();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLang(std::string lang_) { m_lang = lang_; };
    std::string GetLang() { return m_lang; };
    ///@}

protected:
    /** --- **/
    std::string m_lang;

/* include <attlang> */
};

//----------------------------------------------------------------------------
// AttLayerLog
//----------------------------------------------------------------------------

class AttLayerLog 
{
public:
    AttLayerLog();
    virtual ~AttLayerLog();
    
    /** Reset the default values for the attribute class **/
    void ResetLayerLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDef(std::string def_) { m_def = def_; };
    std::string GetDef() { return m_def; };
    ///@}

protected:
    /** Provides a mechanism for linking the layer to a layerDef element. **/
    std::string m_def;

/* include <attdef> */
};

//----------------------------------------------------------------------------
// AttLayerident
//----------------------------------------------------------------------------

class AttLayerident 
{
public:
    AttLayerident();
    virtual ~AttLayerident();
    
    /** Reset the default values for the attribute class **/
    void ResetLayerident();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLayer(std::string layer_) { m_layer = layer_; };
    std::string GetLayer() { return m_layer; };
    ///@}

protected:
    /** Identifies the layer to which a feature applies. **/
    std::string m_layer;

/* include <attlayer> */
};

//----------------------------------------------------------------------------
// AttLineloc
//----------------------------------------------------------------------------

class AttLineloc 
{
public:
    AttLineloc();
    virtual ~AttLineloc();
    
    /** Reset the default values for the attribute class **/
    void ResetLineloc();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLine(std::string line_) { m_line = line_; };
    std::string GetLine() { return m_line; };
    ///@}

protected:
    /**
     * Indicates the line upon which a feature stands.
     * The value must be in the range between 1 and the number of lines on the staff.
     * The numbering of lines starts with the lowest line of the staff.
     **/
    std::string m_line;

/* include <attline> */
};

//----------------------------------------------------------------------------
// AttLinerend
//----------------------------------------------------------------------------

class AttLinerend 
{
public:
    AttLinerend();
    virtual ~AttLinerend();
    
    /** Reset the default values for the attribute class **/
    void ResetLinerend();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRend(std::string rend_) { m_rend = rend_; };
    std::string GetRend() { return m_rend; };
    ///@}

protected:
    /** Describes the line style of the curve. **/
    std::string m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttLyricstyle
//----------------------------------------------------------------------------

class AttLyricstyle 
{
public:
    AttLyricstyle();
    virtual ~AttLyricstyle();
    
    /** Reset the default values for the attribute class **/
    void ResetLyricstyle();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLyricAlignDbl(double lyricAlignDbl_) { m_lyricAlignDbl = lyricAlignDbl_; };
    double GetLyricAlignDbl() { return m_lyricAlignDbl; };
    //
    void SetLyricFam(std::string lyricFam_) { m_lyricFam = lyricFam_; };
    std::string GetLyricFam() { return m_lyricFam; };
    //
    void SetLyricName(std::string lyricName_) { m_lyricName = lyricName_; };
    std::string GetLyricName() { return m_lyricName; };
    //
    void SetLyricSizeDbl(double lyricSizeDbl_) { m_lyricSizeDbl = lyricSizeDbl_; };
    double GetLyricSizeDbl() { return m_lyricSizeDbl; };
    //
    void SetLyricStyle(std::string lyricStyle_) { m_lyricStyle = lyricStyle_; };
    std::string GetLyricStyle() { return m_lyricStyle; };
    //
    void SetLyricWeight(std::string lyricWeight_) { m_lyricWeight = lyricWeight_; };
    std::string GetLyricWeight() { return m_lyricWeight; };
    ///@}

protected:
    /** Describes the alignment of lyric syllables associated with a note or chord. **/
    double m_lyricAlignDbl;
    /** Sets the font family default value for lyrics. **/
    std::string m_lyricFam;
    /** Sets the font name default value for lyrics. **/
    std::string m_lyricName;
    /** Sets the default font size value for lyrics. **/
    double m_lyricSizeDbl;
    /** Sets the default font style value for lyrics. **/
    std::string m_lyricStyle;
    /** Sets the default font weight value for lyrics. **/
    std::string m_lyricWeight;

/* include <attlyric.weight> */
};

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

class AttMeasureLog 
{
public:
    AttMeasureLog();
    virtual ~AttMeasureLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMeasureLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLeft(std::string left_) { m_left = left_; };
    std::string GetLeft() { return m_left; };
    //
    void SetRight(std::string right_) { m_right = right_; };
    std::string GetRight() { return m_right; };
    ///@}

protected:
    /**
     * Indicates the visual rendition of the left bar line.
     * It is present here only for facilitation of translation from legacy encodings
     * which use it. Usually, it can be safely ignored.
     **/
    std::string m_left;
    /** Indicates the function of the right bar line and is structurally important. **/
    std::string m_right;

/* include <attright> */
};

//----------------------------------------------------------------------------
// AttMeasurement
//----------------------------------------------------------------------------

class AttMeasurement 
{
public:
    AttMeasurement();
    virtual ~AttMeasurement();
    
    /** Reset the default values for the attribute class **/
    void ResetMeasurement();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetUnitDbl(double unitDbl_) { m_unitDbl = unitDbl_; };
    double GetUnitDbl() { return m_unitDbl; };
    ///@}

protected:
    /** Indicates the unit used for a measurement of size. **/
    double m_unitDbl;

/* include <attunit> */
};

//----------------------------------------------------------------------------
// AttMediabounds
//----------------------------------------------------------------------------

class AttMediabounds 
{
public:
    AttMediabounds();
    virtual ~AttMediabounds();
    
    /** Reset the default values for the attribute class **/
    void ResetMediabounds();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBegin(std::string begin_) { m_begin = begin_; };
    std::string GetBegin() { return m_begin; };
    //
    void SetEnd(std::string end_) { m_end = end_; };
    std::string GetEnd() { return m_end; };
    //
    void SetBetype(std::string betype_) { m_betype = betype_; };
    std::string GetBetype() { return m_betype; };
    ///@}

protected:
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

class AttMedium 
{
public:
    AttMedium();
    virtual ~AttMedium();
    
    /** Reset the default values for the attribute class **/
    void ResetMedium();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMedium(std::string medium_) { m_medium = medium_; };
    std::string GetMedium() { return m_medium; };
    ///@}

protected:
    /** Describes the writing medium. **/
    std::string m_medium;

/* include <attmedium> */
};

//----------------------------------------------------------------------------
// AttMeiversion
//----------------------------------------------------------------------------

class AttMeiversion 
{
public:
    AttMeiversion();
    virtual ~AttMeiversion();
    
    /** Reset the default values for the attribute class **/
    void ResetMeiversion();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMeiversion(std::string meiversion_) { m_meiversion = meiversion_; };
    std::string GetMeiversion() { return m_meiversion; };
    ///@}

protected:
    /** Specifies the version number of the MEI Guidelines in use. **/
    std::string m_meiversion;

/* include <attmeiversion> */
};

//----------------------------------------------------------------------------
// AttMensurLog
//----------------------------------------------------------------------------

class AttMensurLog 
{
public:
    AttMensurLog();
    virtual ~AttMensurLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMensurLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDot(std::string dot_) { m_dot = dot_; };
    std::string GetDot() { return m_dot; };
    //
    void SetModusmaior(std::string modusmaior_) { m_modusmaior = modusmaior_; };
    std::string GetModusmaior() { return m_modusmaior; };
    //
    void SetModusminor(std::string modusminor_) { m_modusminor = modusminor_; };
    std::string GetModusminor() { return m_modusminor; };
    //
    void SetProlatio(std::string prolatio_) { m_prolatio = prolatio_; };
    std::string GetProlatio() { return m_prolatio; };
    //
    void SetSign(std::string sign_) { m_sign = sign_; };
    std::string GetSign() { return m_sign; };
    //
    void SetTempus(std::string tempus_) { m_tempus = tempus_; };
    std::string GetTempus() { return m_tempus; };
    ///@}

protected:
    /** Specifies whether a dot is to be added to the base symbol. **/
    std::string m_dot;
    /** Describes the maxima-long relationship. **/
    std::string m_modusmaior;
    /** Describes the long-breve relationship. **/
    std::string m_modusminor;
    /** Describes the semibreve-minim relationship. **/
    std::string m_prolatio;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    std::string m_sign;
    /** Describes the breve-semibreve relationship. **/
    std::string m_tempus;

/* include <atttempus> */
};

//----------------------------------------------------------------------------
// AttMeterSigLog
//----------------------------------------------------------------------------

class AttMeterSigLog 
{
public:
    AttMeterSigLog();
    virtual ~AttMeterSigLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterSigLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCountDbl(double countDbl_) { m_countDbl = countDbl_; };
    double GetCountDbl() { return m_countDbl; };
    //
    void SetSym(std::string sym_) { m_sym = sym_; };
    std::string GetSym() { return m_sym; };
    //
    void SetUnitDbl(double unitDbl_) { m_unitDbl = unitDbl_; };
    double GetUnitDbl() { return m_unitDbl; };
    ///@}

protected:
    /**
     * Captures the number of beats in a measure, that is, the top number of the meter
     * signature.
     **/
    double m_countDbl;
    /**
     * Indicates the use of a meter symbol instead of a numeric meter signature, that
     * is, 'C' for common time or 'C' with a slash for cut time.
     **/
    std::string m_sym;
    /** Indicates the unit used for a measurement of size. **/
    double m_unitDbl;

/* include <attunit> */
};

//----------------------------------------------------------------------------
// AttMeterSigVis
//----------------------------------------------------------------------------

class AttMeterSigVis 
{
public:
    AttMeterSigVis();
    virtual ~AttMeterSigVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterSigVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRend(std::string rend_) { m_rend = rend_; };
    std::string GetRend() { return m_rend; };
    ///@}

protected:
    /** Describes the line style of the curve. **/
    std::string m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttMeterSigDefaultLog
//----------------------------------------------------------------------------

class AttMeterSigDefaultLog 
{
public:
    AttMeterSigDefaultLog();
    virtual ~AttMeterSigDefaultLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterSigDefaultLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMeterCountDbl(double meterCountDbl_) { m_meterCountDbl = meterCountDbl_; };
    double GetMeterCountDbl() { return m_meterCountDbl; };
    //
    void SetMeterUnitDbl(double meterUnitDbl_) { m_meterUnitDbl = meterUnitDbl_; };
    double GetMeterUnitDbl() { return m_meterUnitDbl; };
    ///@}

protected:
    /**
     * Captures the number of beats in a measure, that is, the top number of the meter
     * signature.
     **/
    double m_meterCountDbl;
    /**
     * Contains the number indicating the beat unit, that is, the bottom number of the
     * meter signature.
     **/
    double m_meterUnitDbl;

/* include <attmeter.unit> */
};

//----------------------------------------------------------------------------
// AttMeterSigDefaultVis
//----------------------------------------------------------------------------

class AttMeterSigDefaultVis 
{
public:
    AttMeterSigDefaultVis();
    virtual ~AttMeterSigDefaultVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterSigDefaultVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMeterRend(std::string meterRend_) { m_meterRend = meterRend_; };
    std::string GetMeterRend() { return m_meterRend; };
    //
    void SetMeterShowchange(std::string meterShowchange_) { m_meterShowchange = meterShowchange_; };
    std::string GetMeterShowchange() { return m_meterShowchange; };
    //
    void SetMeterSym(std::string meterSym_) { m_meterSym = meterSym_; };
    std::string GetMeterSym() { return m_meterSym; };
    ///@}

protected:
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
    std::string m_meterSym;

/* include <attmeter.sym> */
};

//----------------------------------------------------------------------------
// AttMeterconformance
//----------------------------------------------------------------------------

class AttMeterconformance 
{
public:
    AttMeterconformance();
    virtual ~AttMeterconformance();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterconformance();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMetcon(std::string metcon_) { m_metcon = metcon_; };
    std::string GetMetcon() { return m_metcon; };
    ///@}

protected:
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

class AttMeterconformanceBar 
{
public:
    AttMeterconformanceBar();
    virtual ~AttMeterconformanceBar();
    
    /** Reset the default values for the attribute class **/
    void ResetMeterconformanceBar();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMetcon(std::string metcon_) { m_metcon = metcon_; };
    std::string GetMetcon() { return m_metcon; };
    //
    void SetControl(std::string control_) { m_control = control_; };
    std::string GetControl() { return m_control; };
    ///@}

protected:
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

class AttMmtempo 
{
public:
    AttMmtempo();
    virtual ~AttMmtempo();
    
    /** Reset the default values for the attribute class **/
    void ResetMmtempo();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMm(std::string mm_) { m_mm = mm_; };
    std::string GetMm() { return m_mm; };
    ///@}

protected:
    /**
     * Used to describe tempo in terms of beats (meter signature denominator) per
     * minute, ala M.M.
     * (Maezel's Metronome).
     **/
    std::string m_mm;

/* include <attmm> */
};

//----------------------------------------------------------------------------
// AttMultinummeasures
//----------------------------------------------------------------------------

class AttMultinummeasures 
{
public:
    AttMultinummeasures();
    virtual ~AttMultinummeasures();
    
    /** Reset the default values for the attribute class **/
    void ResetMultinummeasures();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMultiNumber(std::string multiNumber_) { m_multiNumber = multiNumber_; };
    std::string GetMultiNumber() { return m_multiNumber; };
    ///@}

protected:
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

class AttName 
{
public:
    AttName();
    virtual ~AttName();
    
    /** Reset the default values for the attribute class **/
    void ResetName();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetNymref(std::string nymref_) { m_nymref = nymref_; };
    std::string GetNymref() { return m_nymref; };
    //
    void SetRole(std::string role_) { m_role = role_; };
    std::string GetRole() { return m_role; };
    ///@}

protected:
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

class AttNoteGes 
{
public:
    AttNoteGes();
    virtual ~AttNoteGes();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteGes();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetOctGes(std::string octGes_) { m_octGes = octGes_; };
    std::string GetOctGes() { return m_octGes; };
    //
    void SetPnameGes(std::string pnameGes_) { m_pnameGes = pnameGes_; };
    std::string GetPnameGes() { return m_pnameGes; };
    //
    void SetPnum(std::string pnum_) { m_pnum = pnum_; };
    std::string GetPnum() { return m_pnum; };
    ///@}

protected:
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

class AttNoteVis 
{
public:
    AttNoteVis();
    virtual ~AttNoteVis();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetHeadshape(std::string headshape_) { m_headshape = headshape_; };
    std::string GetHeadshape() { return m_headshape; };
    ///@}

protected:
    /** Used to override the head shape normally used for the given duration. **/
    std::string m_headshape;

/* include <attheadshape> */
};

//----------------------------------------------------------------------------
// AttOctave
//----------------------------------------------------------------------------

class AttOctave 
{
public:
    AttOctave();
    virtual ~AttOctave();
    
    /** Reset the default values for the attribute class **/
    void ResetOctave();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetOct(std::string oct_) { m_oct = oct_; };
    std::string GetOct() { return m_oct; };
    ///@}

protected:
    /** Captures written octave information. **/
    std::string m_oct;

/* include <attoct> */
};

//----------------------------------------------------------------------------
// AttOctavedefault
//----------------------------------------------------------------------------

class AttOctavedefault 
{
public:
    AttOctavedefault();
    virtual ~AttOctavedefault();
    
    /** Reset the default values for the attribute class **/
    void ResetOctavedefault();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetOctaveDefault(std::string octaveDefault_) { m_octaveDefault = octaveDefault_; };
    std::string GetOctaveDefault() { return m_octaveDefault; };
    ///@}

protected:
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

class AttOctavedisplacement 
{
public:
    AttOctavedisplacement();
    virtual ~AttOctavedisplacement();
    
    /** Reset the default values for the attribute class **/
    void ResetOctavedisplacement();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDis(std::string dis_) { m_dis = dis_; };
    std::string GetDis() { return m_dis; };
    //
    void SetDisPlace(std::string disPlace_) { m_disPlace = disPlace_; };
    std::string GetDisPlace() { return m_disPlace; };
    ///@}

protected:
    /** Records the amount of octave displacement. **/
    std::string m_dis;
    /** Records the direction of octave displacement. **/
    std::string m_disPlace;

/* include <attdis.place> */
};

//----------------------------------------------------------------------------
// AttOnelinestaff
//----------------------------------------------------------------------------

class AttOnelinestaff 
{
public:
    AttOnelinestaff();
    virtual ~AttOnelinestaff();
    
    /** Reset the default values for the attribute class **/
    void ResetOnelinestaff();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetOntheline(std::string ontheline_) { m_ontheline = ontheline_; };
    std::string GetOntheline() { return m_ontheline; };
    ///@}

protected:
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

class AttPadLog 
{
public:
    AttPadLog();
    virtual ~AttPadLog();
    
    /** Reset the default values for the attribute class **/
    void ResetPadLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetNumInt(int numInt_) { m_numInt = numInt_; };
    int GetNumInt() { return m_numInt; };
    ///@}

protected:
    /**
     * Along with numbase, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_numInt;

/* include <attnum> */
};

//----------------------------------------------------------------------------
// AttPbVis
//----------------------------------------------------------------------------

class AttPbVis 
{
public:
    AttPbVis();
    virtual ~AttPbVis();
    
    /** Reset the default values for the attribute class **/
    void ResetPbVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetFunc(std::string func_) { m_func = func_; };
    std::string GetFunc() { return m_func; };
    ///@}

protected:
    /** Records the function of an accidental. **/
    std::string m_func;

/* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttPitch
//----------------------------------------------------------------------------

class AttPitch 
{
public:
    AttPitch();
    virtual ~AttPitch();
    
    /** Reset the default values for the attribute class **/
    void ResetPitch();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetPname(std::string pname_) { m_pname = pname_; };
    std::string GetPname() { return m_pname; };
    ///@}

protected:
    /** Contains a written pitch name. **/
    std::string m_pname;

/* include <attpname> */
};

//----------------------------------------------------------------------------
// AttPlacement
//----------------------------------------------------------------------------

class AttPlacement 
{
public:
    AttPlacement();
    virtual ~AttPlacement();
    
    /** Reset the default values for the attribute class **/
    void ResetPlacement();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetPlace(std::string place_) { m_place = place_; };
    std::string GetPlace() { return m_place; };
    ///@}

protected:
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

class AttPlist 
{
public:
    AttPlist();
    virtual ~AttPlist();
    
    /** Reset the default values for the attribute class **/
    void ResetPlist();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetPlist(std::string plist_) { m_plist = plist_; };
    std::string GetPlist() { return m_plist; };
    //
    void SetEvaluate(std::string evaluate_) { m_evaluate = evaluate_; };
    std::string GetEvaluate() { return m_evaluate; };
    ///@}

protected:
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

class AttPointing 
{
public:
    AttPointing();
    virtual ~AttPointing();
    
    /** Reset the default values for the attribute class **/
    void ResetPointing();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetActuate(std::string actuate_) { m_actuate = actuate_; };
    std::string GetActuate() { return m_actuate; };
    //
    void SetRole(std::string role_) { m_role = role_; };
    std::string GetRole() { return m_role; };
    //
    void SetShow(std::string show_) { m_show = show_; };
    std::string GetShow() { return m_show; };
    //
    void SetTarget(std::string target_) { m_target = target_; };
    std::string GetTarget() { return m_target; };
    //
    void SetTargettype(std::string targettype_) { m_targettype = targettype_; };
    std::string GetTargettype() { return m_targettype; };
    //
    void SetTitle(std::string title_) { m_title = title_; };
    std::string GetTitle() { return m_title; };
    ///@}

protected:
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
     * Allows the use of one or more previously-undeclared URIs to identify an external
     * electronic object.
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

class AttRelativesize 
{
public:
    AttRelativesize();
    virtual ~AttRelativesize();
    
    /** Reset the default values for the attribute class **/
    void ResetRelativesize();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSize(std::string size_) { m_size = size_; };
    std::string GetSize() { return m_size; };
    ///@}

protected:
    /** Describes the relative size of a feature. **/
    std::string m_size;

/* include <attsize> */
};

//----------------------------------------------------------------------------
// AttResponsibility
//----------------------------------------------------------------------------

class AttResponsibility 
{
public:
    AttResponsibility();
    virtual ~AttResponsibility();
    
    /** Reset the default values for the attribute class **/
    void ResetResponsibility();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetResp(std::string resp_) { m_resp = resp_; };
    std::string GetResp() { return m_resp; };
    ///@}

protected:
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

class AttSbVis 
{
public:
    AttSbVis();
    virtual ~AttSbVis();
    
    /** Reset the default values for the attribute class **/
    void ResetSbVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRend(std::string rend_) { m_rend = rend_; };
    std::string GetRend() { return m_rend; };
    ///@}

protected:
    /** Describes the line style of the curve. **/
    std::string m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttScalable
//----------------------------------------------------------------------------

class AttScalable 
{
public:
    AttScalable();
    virtual ~AttScalable();
    
    /** Reset the default values for the attribute class **/
    void ResetScalable();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetScale(std::string scale_) { m_scale = scale_; };
    std::string GetScale() { return m_scale; };
    ///@}

protected:
    /** Scale factor to be applied to the feature to make it the desired display size. **/
    std::string m_scale;

/* include <attscale> */
};

//----------------------------------------------------------------------------
// AttScoreDefGes
//----------------------------------------------------------------------------

class AttScoreDefGes 
{
public:
    AttScoreDefGes();
    virtual ~AttScoreDefGes();
    
    /** Reset the default values for the attribute class **/
    void ResetScoreDefGes();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTunePname(std::string tunePname_) { m_tunePname = tunePname_; };
    std::string GetTunePname() { return m_tunePname; };
    //
    void SetTuneHzDbl(double tuneHzDbl_) { m_tuneHzDbl = tuneHzDbl_; };
    double GetTuneHzDbl() { return m_tuneHzDbl; };
    //
    void SetTuneTemper(std::string tuneTemper_) { m_tuneTemper = tuneTemper_; };
    std::string GetTuneTemper() { return m_tuneTemper; };
    ///@}

protected:
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

class AttScoreDefVis 
{
public:
    AttScoreDefVis();
    virtual ~AttScoreDefVis();
    
    /** Reset the default values for the attribute class **/
    void ResetScoreDefVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetEndingRend(std::string endingRend_) { m_endingRend = endingRend_; };
    std::string GetEndingRend() { return m_endingRend; };
    //
    void SetMnumVisible(std::string mnumVisible_) { m_mnumVisible = mnumVisible_; };
    std::string GetMnumVisible() { return m_mnumVisible; };
    //
    void SetMusicName(std::string musicName_) { m_musicName = musicName_; };
    std::string GetMusicName() { return m_musicName; };
    //
    void SetMusicSizeDbl(double musicSizeDbl_) { m_musicSizeDbl = musicSizeDbl_; };
    double GetMusicSizeDbl() { return m_musicSizeDbl; };
    //
    void SetOptimize(std::string optimize_) { m_optimize = optimize_; };
    std::string GetOptimize() { return m_optimize; };
    //
    void SetPageHeightDbl(double pageHeightDbl_) { m_pageHeightDbl = pageHeightDbl_; };
    double GetPageHeightDbl() { return m_pageHeightDbl; };
    //
    void SetPageWidthDbl(double pageWidthDbl_) { m_pageWidthDbl = pageWidthDbl_; };
    double GetPageWidthDbl() { return m_pageWidthDbl; };
    //
    void SetPageUnits(std::string pageUnits_) { m_pageUnits = pageUnits_; };
    std::string GetPageUnits() { return m_pageUnits; };
    //
    void SetPageTopmarDbl(double pageTopmarDbl_) { m_pageTopmarDbl = pageTopmarDbl_; };
    double GetPageTopmarDbl() { return m_pageTopmarDbl; };
    //
    void SetPageBotmarDbl(double pageBotmarDbl_) { m_pageBotmarDbl = pageBotmarDbl_; };
    double GetPageBotmarDbl() { return m_pageBotmarDbl; };
    //
    void SetPageLeftmarDbl(double pageLeftmarDbl_) { m_pageLeftmarDbl = pageLeftmarDbl_; };
    double GetPageLeftmarDbl() { return m_pageLeftmarDbl; };
    //
    void SetPageRightmarDbl(double pageRightmarDbl_) { m_pageRightmarDbl = pageRightmarDbl_; };
    double GetPageRightmarDbl() { return m_pageRightmarDbl; };
    //
    void SetPagePanels(std::string pagePanels_) { m_pagePanels = pagePanels_; };
    std::string GetPagePanels() { return m_pagePanels; };
    //
    void SetPageScale(std::string pageScale_) { m_pageScale = pageScale_; };
    std::string GetPageScale() { return m_pageScale; };
    //
    void SetSpacingPackexpDbl(double spacingPackexpDbl_) { m_spacingPackexpDbl = spacingPackexpDbl_; };
    double GetSpacingPackexpDbl() { return m_spacingPackexpDbl; };
    //
    void SetSpacingPackfactDbl(double spacingPackfactDbl_) { m_spacingPackfactDbl = spacingPackfactDbl_; };
    double GetSpacingPackfactDbl() { return m_spacingPackfactDbl; };
    //
    void SetSpacingStaffDbl(double spacingStaffDbl_) { m_spacingStaffDbl = spacingStaffDbl_; };
    double GetSpacingStaffDbl() { return m_spacingStaffDbl; };
    //
    void SetSpacingSystem(std::string spacingSystem_) { m_spacingSystem = spacingSystem_; };
    std::string GetSpacingSystem() { return m_spacingSystem; };
    //
    void SetSystemLeftmarDbl(double systemLeftmarDbl_) { m_systemLeftmarDbl = systemLeftmarDbl_; };
    double GetSystemLeftmarDbl() { return m_systemLeftmarDbl; };
    //
    void SetSystemRightmarDbl(double systemRightmarDbl_) { m_systemRightmarDbl = systemRightmarDbl_; };
    double GetSystemRightmarDbl() { return m_systemRightmarDbl; };
    //
    void SetSystemTopmarDbl(double systemTopmarDbl_) { m_systemTopmarDbl = systemTopmarDbl_; };
    double GetSystemTopmarDbl() { return m_systemTopmarDbl; };
    ///@}

protected:
    /** Describes where ending marks should be displayed. **/
    std::string m_endingRend;
    /** Indicates whether measure numbers should be displayed. **/
    std::string m_mnumVisible;
    /** Sets the default music font name. **/
    std::string m_musicName;
    /** Sets the default music font size. **/
    double m_musicSizeDbl;
    /**
     * Indicates whether staves without notes, rests, etc.
     * should be displayed. When the value is 'true', empty staves are displayed.
     **/
    std::string m_optimize;
    /** Describes the physical height of the rendered output page. **/
    double m_pageHeightDbl;
    /** Describes the physical width of the rendered output page. **/
    double m_pageWidthDbl;
    /**
     * Contains the real-world measurement units (inches, centimeters, millimeters)
     * used to describe the rendered page height, width, and margins.
     **/
    std::string m_pageUnits;
    /** Indicates the amount of whitespace at the top of a rendered score page. **/
    double m_pageTopmarDbl;
    /** Indicates the amount of whitespace at the bottom of a rendered score page. **/
    double m_pageBotmarDbl;
    /** Indicates the amount of whitespace at the left side of a rendered score page. **/
    double m_pageLeftmarDbl;
    /** Indicates the amount of whitespace at the right side of a rendered score page. **/
    double m_pageRightmarDbl;
    /** Indicates the number of logical pages to be rendered on a single physical page. **/
    std::string m_pagePanels;
    /** Indicates how the page should be scaled when rendered. **/
    std::string m_pageScale;
    /** Describes a note's spacing relative to its time value. **/
    double m_spacingPackexpDbl;
    /** Describes the note spacing of output. **/
    double m_spacingPackfactDbl;
    /** Sets the minimum amount of space between staves in the same system. **/
    double m_spacingStaffDbl;
    /**
     * Contains a space-separated pair of numbers describing the minimum and maximum
     * amount of space between systems.
     **/
    std::string m_spacingSystem;
    /**
     * Describes the amount of whitespace at the left system margin relative to
     * page.leftmar.
     **/
    double m_systemLeftmarDbl;
    /**
     * Describes the amount of whitespace at the right system margin relative to
     * page.rightmar.
     **/
    double m_systemRightmarDbl;
    /**
     * Describes the distance from page's top edge to the first system; used for first
     * page only.
     **/
    double m_systemTopmarDbl;

/* include <attsystem.topmar> */
};

//----------------------------------------------------------------------------
// AttSectionVis
//----------------------------------------------------------------------------

class AttSectionVis 
{
public:
    AttSectionVis();
    virtual ~AttSectionVis();
    
    /** Reset the default values for the attribute class **/
    void ResetSectionVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRestart(std::string restart_) { m_restart = restart_; };
    std::string GetRestart() { return m_restart; };
    ///@}

protected:
    /** Indicates that staves begin again with this section. **/
    std::string m_restart;

/* include <attrestart> */
};

//----------------------------------------------------------------------------
// AttSequence
//----------------------------------------------------------------------------

class AttSequence 
{
public:
    AttSequence();
    virtual ~AttSequence();
    
    /** Reset the default values for the attribute class **/
    void ResetSequence();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSeqInt(int seqInt_) { m_seqInt = seqInt_; };
    int GetSeqInt() { return m_seqInt; };
    ///@}

protected:
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

class AttSlashcount 
{
public:
    AttSlashcount();
    virtual ~AttSlashcount();
    
    /** Reset the default values for the attribute class **/
    void ResetSlashcount();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSlash(std::string slash_) { m_slash = slash_; };
    std::string GetSlash() { return m_slash; };
    ///@}

protected:
    /** Indicates the number of slashes present. **/
    std::string m_slash;

/* include <attslash> */
};

//----------------------------------------------------------------------------
// AttSlurpresent
//----------------------------------------------------------------------------

class AttSlurpresent 
{
public:
    AttSlurpresent();
    virtual ~AttSlurpresent();
    
    /** Reset the default values for the attribute class **/
    void ResetSlurpresent();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSlur(std::string slur_) { m_slur = slur_; };
    std::string GetSlur() { return m_slur; };
    ///@}

protected:
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

class AttSpaceVis 
{
public:
    AttSpaceVis();
    virtual ~AttSpaceVis();
    
    /** Reset the default values for the attribute class **/
    void ResetSpaceVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCompressable(std::string compressable_) { m_compressable = compressable_; };
    std::string GetCompressable() { return m_compressable; };
    ///@}

protected:
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

class AttStaffLog 
{
public:
    AttStaffLog();
    virtual ~AttStaffLog();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDef(std::string def_) { m_def = def_; };
    std::string GetDef() { return m_def; };
    ///@}

protected:
    /** Provides a mechanism for linking the layer to a layerDef element. **/
    std::string m_def;

/* include <attdef> */
};

//----------------------------------------------------------------------------
// AttStaffDefVis
//----------------------------------------------------------------------------

class AttStaffDefVis 
{
public:
    AttStaffDefVis();
    virtual ~AttStaffDefVis();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffDefVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetGridShow(std::string gridShow_) { m_gridShow = gridShow_; };
    std::string GetGridShow() { return m_gridShow; };
    //
    void SetLayerscheme(std::string layerscheme_) { m_layerscheme = layerscheme_; };
    std::string GetLayerscheme() { return m_layerscheme; };
    //
    void SetLinesInt(int linesInt_) { m_linesInt = linesInt_; };
    int GetLinesInt() { return m_linesInt; };
    //
    void SetLinesColor(std::string linesColor_) { m_linesColor = linesColor_; };
    std::string GetLinesColor() { return m_linesColor; };
    //
    void SetLinesVisible(std::string linesVisible_) { m_linesVisible = linesVisible_; };
    std::string GetLinesVisible() { return m_linesVisible; };
    //
    void SetSpacingDbl(double spacingDbl_) { m_spacingDbl = spacingDbl_; };
    double GetSpacingDbl() { return m_spacingDbl; };
    ///@}

protected:
    /** Determines whether to display guitar chord grids. **/
    std::string m_gridShow;
    /** Indicates the number of layers and their stem directions. **/
    std::string m_layerscheme;
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
    std::string m_linesVisible;
    /**
     * Records the absolute distance (as opposed to the relative distances recorded in
     * <scoreDef> elements) between this staff and the preceding one in the same
     * system.
     * This value is meaningless for the first staff in a system since the
     * spacing.system attribute indicates the spacing between systems.
     **/
    double m_spacingDbl;

/* include <attspacing> */
};

//----------------------------------------------------------------------------
// AttStaffGrpVis
//----------------------------------------------------------------------------

class AttStaffGrpVis 
{
public:
    AttStaffGrpVis();
    virtual ~AttStaffGrpVis();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffGrpVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBarthru(std::string barthru_) { m_barthru = barthru_; };
    std::string GetBarthru() { return m_barthru; };
    ///@}

protected:
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

class AttStaffgroupingsym 
{
public:
    AttStaffgroupingsym();
    virtual ~AttStaffgroupingsym();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffgroupingsym();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSymbol(std::string symbol_) { m_symbol = symbol_; };
    std::string GetSymbol() { return m_symbol; };
    ///@}

protected:
    /** Specifies the symbol used to group a set of staves. **/
    std::string m_symbol;

/* include <attsymbol> */
};

//----------------------------------------------------------------------------
// AttStaffident
//----------------------------------------------------------------------------

class AttStaffident 
{
public:
    AttStaffident();
    virtual ~AttStaffident();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffident();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetStaff(std::string staff_) { m_staff = staff_; };
    std::string GetStaff() { return m_staff; };
    ///@}

protected:
    /**
     * Signifies the staff on which a notated event occurs or to which a control event
     * applies.
     * Mandatory when applicable.
     **/
    std::string m_staff;

/* include <attstaff> */
};

//----------------------------------------------------------------------------
// AttStaffloc
//----------------------------------------------------------------------------

class AttStaffloc 
{
public:
    AttStaffloc();
    virtual ~AttStaffloc();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffloc();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLoc(std::string loc_) { m_loc = loc_; };
    std::string GetLoc() { return m_loc; };
    ///@}

protected:
    /** Holds the staff location of the feature. **/
    std::string m_loc;

/* include <attloc> */
};

//----------------------------------------------------------------------------
// AttStartendid
//----------------------------------------------------------------------------

class AttStartendid 
{
public:
    AttStartendid();
    virtual ~AttStartendid();
    
    /** Reset the default values for the attribute class **/
    void ResetStartendid();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetEndid(std::string endid_) { m_endid = endid_; };
    std::string GetEndid() { return m_endid; };
    ///@}

protected:
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

class AttStartid 
{
public:
    AttStartid();
    virtual ~AttStartid();
    
    /** Reset the default values for the attribute class **/
    void ResetStartid();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetStartid(std::string startid_) { m_startid = startid_; };
    std::string GetStartid() { return m_startid; };
    ///@}

protected:
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

class AttStemmed 
{
public:
    AttStemmed();
    virtual ~AttStemmed();
    
    /** Reset the default values for the attribute class **/
    void ResetStemmed();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetStemDir(std::string stemDir_) { m_stemDir = stemDir_; };
    std::string GetStemDir() { return m_stemDir; };
    //
    void SetStemLenDbl(double stemLenDbl_) { m_stemLenDbl = stemLenDbl_; };
    double GetStemLenDbl() { return m_stemLenDbl; };
    //
    void SetStemPos(std::string stemPos_) { m_stemPos = stemPos_; };
    std::string GetStemPos() { return m_stemPos; };
    //
    void SetStemXDbl(double stemXDbl_) { m_stemXDbl = stemXDbl_; };
    double GetStemXDbl() { return m_stemXDbl; };
    //
    void SetStemYDbl(double stemYDbl_) { m_stemYDbl = stemYDbl_; };
    double GetStemYDbl() { return m_stemYDbl; };
    ///@}

protected:
    /** Describes the direction of a stem. **/
    std::string m_stemDir;
    /** Encodes the stem length. **/
    double m_stemLenDbl;
    /** Records the position of the stem in relation to the note head(s). **/
    std::string m_stemPos;
    /** Records the output x coordinate of the stem's attachment point. **/
    double m_stemXDbl;
    /** Records the output y coordinate of the stem's attachment point. **/
    double m_stemYDbl;

/* include <attstem.y> */
};

//----------------------------------------------------------------------------
// AttSylLog
//----------------------------------------------------------------------------

class AttSylLog 
{
public:
    AttSylLog();
    virtual ~AttSylLog();
    
    /** Reset the default values for the attribute class **/
    void ResetSylLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCon(std::string con_) { m_con = con_; };
    std::string GetCon() { return m_con; };
    //
    void SetWordpos(std::string wordpos_) { m_wordpos = wordpos_; };
    std::string GetWordpos() { return m_wordpos; };
    ///@}

protected:
    /**
     * Describes the symbols typically used to indicate breaks between syllables and
     * their functions.
     **/
    std::string m_con;
    /** Records the position of a syllable within a word. **/
    std::string m_wordpos;

/* include <attwordpos> */
};

//----------------------------------------------------------------------------
// AttSyltext
//----------------------------------------------------------------------------

class AttSyltext 
{
public:
    AttSyltext();
    virtual ~AttSyltext();
    
    /** Reset the default values for the attribute class **/
    void ResetSyltext();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSyl(std::string syl_) { m_syl = syl_; };
    std::string GetSyl() { return m_syl; };
    ///@}

protected:
    /** Holds an associated sung text syllable. **/
    std::string m_syl;

/* include <attsyl> */
};

//----------------------------------------------------------------------------
// AttTextstyle
//----------------------------------------------------------------------------

class AttTextstyle 
{
public:
    AttTextstyle();
    virtual ~AttTextstyle();
    
    /** Reset the default values for the attribute class **/
    void ResetTextstyle();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTextFam(std::string textFam_) { m_textFam = textFam_; };
    std::string GetTextFam() { return m_textFam; };
    //
    void SetTextName(std::string textName_) { m_textName = textName_; };
    std::string GetTextName() { return m_textName; };
    //
    void SetTextSizeDbl(double textSizeDbl_) { m_textSizeDbl = textSizeDbl_; };
    double GetTextSizeDbl() { return m_textSizeDbl; };
    //
    void SetTextStyle(std::string textStyle_) { m_textStyle = textStyle_; };
    std::string GetTextStyle() { return m_textStyle; };
    //
    void SetTextWeight(std::string textWeight_) { m_textWeight = textWeight_; };
    std::string GetTextWeight() { return m_textWeight; };
    ///@}

protected:
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
    double m_textSizeDbl;
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

class AttTiepresent 
{
public:
    AttTiepresent();
    virtual ~AttTiepresent();
    
    /** Reset the default values for the attribute class **/
    void ResetTiepresent();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTie(std::string tie_) { m_tie = tie_; };
    std::string GetTie() { return m_tie; };
    ///@}

protected:
    /**
     * Indicates that this element participates in a tie.
     * If visual information about the tie needs to be recorded, then a <tie> element
     * should be employed.
     **/
    std::string m_tie;

/* include <atttie> */
};

//----------------------------------------------------------------------------
// AttTimestampMusical
//----------------------------------------------------------------------------

class AttTimestampMusical 
{
public:
    AttTimestampMusical();
    virtual ~AttTimestampMusical();
    
    /** Reset the default values for the attribute class **/
    void ResetTimestampMusical();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTstamp(std::string tstamp_) { m_tstamp = tstamp_; };
    std::string GetTstamp() { return m_tstamp; };
    ///@}

protected:
    /**
     * Encodes the onset time in terms of musical time, i.e.,
     * beats[.fractional_beat_part].
     **/
    std::string m_tstamp;

/* include <atttstamp> */
};

//----------------------------------------------------------------------------
// AttTimestampPerformed
//----------------------------------------------------------------------------

class AttTimestampPerformed 
{
public:
    AttTimestampPerformed();
    virtual ~AttTimestampPerformed();
    
    /** Reset the default values for the attribute class **/
    void ResetTimestampPerformed();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTstampGesInt(int tstampGesInt_) { m_tstampGesInt = tstampGesInt_; };
    int GetTstampGesInt() { return m_tstampGesInt; };
    //
    void SetTstampReal(std::string tstampReal_) { m_tstampReal = tstampReal_; };
    std::string GetTstampReal() { return m_tstampReal; };
    ///@}

protected:
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
// AttTransposition
//----------------------------------------------------------------------------

class AttTransposition 
{
public:
    AttTransposition();
    virtual ~AttTransposition();
    
    /** Reset the default values for the attribute class **/
    void ResetTransposition();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTransDiatDbl(double transDiatDbl_) { m_transDiatDbl = transDiatDbl_; };
    double GetTransDiatDbl() { return m_transDiatDbl; };
    //
    void SetTransSemiDbl(double transSemiDbl_) { m_transSemiDbl = transSemiDbl_; };
    double GetTransSemiDbl() { return m_transSemiDbl; };
    ///@}

protected:
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

class AttTupletpresent 
{
public:
    AttTupletpresent();
    virtual ~AttTupletpresent();
    
    /** Reset the default values for the attribute class **/
    void ResetTupletpresent();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTuplet(std::string tuplet_) { m_tuplet = tuplet_; };
    std::string GetTuplet() { return m_tuplet; };
    ///@}

protected:
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

class AttTyped 
{
public:
    AttTyped();
    virtual ~AttTyped();
    
    /** Reset the default values for the attribute class **/
    void ResetTyped();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetType(std::string type_) { m_type = type_; };
    std::string GetType() { return m_type; };
    //
    void SetSubtype(std::string subtype_) { m_subtype = subtype_; };
    std::string GetSubtype() { return m_subtype; };
    ///@}

protected:
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

class AttTypography 
{
public:
    AttTypography();
    virtual ~AttTypography();
    
    /** Reset the default values for the attribute class **/
    void ResetTypography();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetFontfam(std::string fontfam_) { m_fontfam = fontfam_; };
    std::string GetFontfam() { return m_fontfam; };
    //
    void SetFontname(std::string fontname_) { m_fontname = fontname_; };
    std::string GetFontname() { return m_fontname; };
    //
    void SetFontsizeDbl(double fontsizeDbl_) { m_fontsizeDbl = fontsizeDbl_; };
    double GetFontsizeDbl() { return m_fontsizeDbl; };
    //
    void SetFontstyle(std::string fontstyle_) { m_fontstyle = fontstyle_; };
    std::string GetFontstyle() { return m_fontstyle; };
    //
    void SetFontweight(std::string fontweight_) { m_fontweight = fontweight_; };
    std::string GetFontweight() { return m_fontweight; };
    ///@}

protected:
    /** Contains the name of a font-family. **/
    std::string m_fontfam;
    /** Holds the name of a font. **/
    std::string m_fontname;
    /** Indicates the size of a font in printers' points, i.e., 1/72nd of an inch. **/
    double m_fontsizeDbl;
    /** Records the style of a font, i.e, italic, oblique, or normal. **/
    std::string m_fontstyle;
    /** Used to indicate bold type. **/
    std::string m_fontweight;

/* include <attfontweight> */
};

//----------------------------------------------------------------------------
// AttVisibility
//----------------------------------------------------------------------------

class AttVisibility 
{
public:
    AttVisibility();
    virtual ~AttVisibility();
    
    /** Reset the default values for the attribute class **/
    void ResetVisibility();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetVisible(std::string visible_) { m_visible = visible_; };
    std::string GetVisible() { return m_visible; };
    ///@}

protected:
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

class AttVisualoffsetHo 
{
public:
    AttVisualoffsetHo();
    virtual ~AttVisualoffsetHo();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetHo();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetHo(std::string ho_) { m_ho = ho_; };
    std::string GetHo() { return m_ho; };
    ///@}

protected:
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

class AttVisualoffsetTo 
{
public:
    AttVisualoffsetTo();
    virtual ~AttVisualoffsetTo();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetTo();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTo(std::string to_) { m_to = to_; };
    std::string GetTo() { return m_to; };
    ///@}

protected:
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

class AttVisualoffsetVo 
{
public:
    AttVisualoffsetVo();
    virtual ~AttVisualoffsetVo();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetVo();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetVo(std::string vo_) { m_vo = vo_; };
    std::string GetVo() { return m_vo; };
    ///@}

protected:
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

class AttVisualoffset2Ho 
{
public:
    AttVisualoffset2Ho();
    virtual ~AttVisualoffset2Ho();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2Ho();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetStartho(std::string startho_) { m_startho = startho_; };
    std::string GetStartho() { return m_startho; };
    //
    void SetEndho(std::string endho_) { m_endho = endho_; };
    std::string GetEndho() { return m_endho; };
    ///@}

protected:
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

class AttVisualoffset2To 
{
public:
    AttVisualoffset2To();
    virtual ~AttVisualoffset2To();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2To();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetStartto(std::string startto_) { m_startto = startto_; };
    std::string GetStartto() { return m_startto; };
    //
    void SetEndto(std::string endto_) { m_endto = endto_; };
    std::string GetEndto() { return m_endto; };
    ///@}

protected:
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

class AttVisualoffset2Vo 
{
public:
    AttVisualoffset2Vo();
    virtual ~AttVisualoffset2Vo();
    
    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2Vo();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetStartvo(std::string startvo_) { m_startvo = startvo_; };
    std::string GetStartvo() { return m_startvo; };
    //
    void SetEndvo(std::string endvo_) { m_endvo = endvo_; };
    std::string GetEndvo() { return m_endvo; };
    ///@}

protected:
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
// AttWidth
//----------------------------------------------------------------------------

class AttWidth 
{
public:
    AttWidth();
    virtual ~AttWidth();
    
    /** Reset the default values for the attribute class **/
    void ResetWidth();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetWidthDbl(double widthDbl_) { m_widthDbl = widthDbl_; };
    double GetWidthDbl() { return m_widthDbl; };
    ///@}

protected:
    /**
     * Measurement of the horizontal dimension of an entity.
     * This value can only be interpreted meaningfully in combination with the units
     * attribute. The width attribute may be used to capture measure width data for
     * interchange with music printing systems that utilize this information for
     * printing. On <barLine> the width attribute captures the width of the preceding
     * measure.
     **/
    double m_widthDbl;

/* include <attwidth> */
};

//----------------------------------------------------------------------------
// AttXy
//----------------------------------------------------------------------------

class AttXy 
{
public:
    AttXy();
    virtual ~AttXy();
    
    /** Reset the default values for the attribute class **/
    void ResetXy();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetXDbl(double xDbl_) { m_xDbl = xDbl_; };
    double GetXDbl() { return m_xDbl; };
    //
    void SetYDbl(double yDbl_) { m_yDbl = yDbl_; };
    double GetYDbl() { return m_yDbl; };
    ///@}

protected:
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

class AttXy2 
{
public:
    AttXy2();
    virtual ~AttXy2();
    
    /** Reset the default values for the attribute class **/
    void ResetXy2();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetX2Dbl(double x2Dbl_) { m_x2Dbl = x2Dbl_; };
    double GetX2Dbl() { return m_x2Dbl; };
    //
    void SetY2Dbl(double y2Dbl_) { m_y2Dbl = y2Dbl_; };
    double GetY2Dbl() { return m_y2Dbl; };
    ///@}

protected:
    /** Encodes the optional 2nd x coordinate. **/
    double m_x2Dbl;
    /** Encodes the optional 2nd y coordinate. **/
    double m_y2Dbl;

/* include <atty2> */
};

} // vrv namespace

#endif  // __VRV_SHAREDMIXIN_H__

