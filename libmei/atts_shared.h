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
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttAccidLog
//----------------------------------------------------------------------------

class AttAccidLog : public Att {
public:
    AttAccidLog();
    virtual ~AttAccidLog();

    /** Reset the default values for the attribute class **/
    void ResetAccidLog();

    /** Read the values for the attribute class **/
    bool ReadAccidLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAccidLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFunc(accidLog_FUNC func_) { m_func = func_; }
    accidLog_FUNC GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Records the function of an accidental. **/
    accidLog_FUNC m_func;

    /* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttAccidental
//----------------------------------------------------------------------------

class AttAccidental : public Att {
public:
    AttAccidental();
    virtual ~AttAccidental();

    /** Reset the default values for the attribute class **/
    void ResetAccidental();

    /** Read the values for the attribute class **/
    bool ReadAccidental(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAccidental(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAccid(data_ACCIDENTAL_WRITTEN accid_) { m_accid = accid_; }
    data_ACCIDENTAL_WRITTEN GetAccid() const { return m_accid; }
    bool HasAccid() const;
    ///@}

private:
    /** Captures a written accidental. **/
    data_ACCIDENTAL_WRITTEN m_accid;

    /* include <attaccid> */
};

//----------------------------------------------------------------------------
// AttArticulation
//----------------------------------------------------------------------------

class AttArticulation : public Att {
public:
    AttArticulation();
    virtual ~AttArticulation();

    /** Reset the default values for the attribute class **/
    void ResetArticulation();

    /** Read the values for the attribute class **/
    bool ReadArticulation(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteArticulation(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetArtic(data_ARTICULATION_List artic_) { m_artic = artic_; }
    data_ARTICULATION_List GetArtic() const { return m_artic; }
    bool HasArtic() const;
    ///@}

private:
    /**
     * Encodes the written articulation(s).
     * Articulations are normally encoded in order from the note head outward; that is,
     * away from the stem. See additional notes at att.vis.note. Only articulations
     * should be encoded in the artic attribute; for example, fingerings should be
     * encoded using the
     **/
    data_ARTICULATION_List m_artic;

    /* include <attartic> */
};

//----------------------------------------------------------------------------
// AttAudience
//----------------------------------------------------------------------------

class AttAudience : public Att {
public:
    AttAudience();
    virtual ~AttAudience();

    /** Reset the default values for the attribute class **/
    void ResetAudience();

    /** Read the values for the attribute class **/
    bool ReadAudience(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAudience(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAudience(audience_AUDIENCE audience_) { m_audience = audience_; }
    audience_AUDIENCE GetAudience() const { return m_audience; }
    bool HasAudience() const;
    ///@}

private:
    /** The intended audience. **/
    audience_AUDIENCE m_audience;

    /* include <attaudience> */
};

//----------------------------------------------------------------------------
// AttAugmentDots
//----------------------------------------------------------------------------

class AttAugmentDots : public Att {
public:
    AttAugmentDots();
    virtual ~AttAugmentDots();

    /** Reset the default values for the attribute class **/
    void ResetAugmentDots();

    /** Read the values for the attribute class **/
    bool ReadAugmentDots(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAugmentDots(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDots(int dots_) { m_dots = dots_; }
    int GetDots() const { return m_dots; }
    bool HasDots() const;
    ///@}

private:
    /** Records the number of augmentation dots required by a dotted duration. **/
    int m_dots;

    /* include <attdots> */
};

//----------------------------------------------------------------------------
// AttAuthorized
//----------------------------------------------------------------------------

class AttAuthorized : public Att {
public:
    AttAuthorized();
    virtual ~AttAuthorized();

    /** Reset the default values for the attribute class **/
    void ResetAuthorized();

    /** Read the values for the attribute class **/
    bool ReadAuthorized(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAuthorized(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAuth(std::string auth_) { m_auth = auth_; }
    std::string GetAuth() const { return m_auth; }
    bool HasAuth() const;
    //
    void SetAuthUri(std::string authUri_) { m_authUri = authUri_; }
    std::string GetAuthUri() const { return m_authUri; }
    bool HasAuthUri() const;
    ///@}

private:
    /**
     * A name or label associated with the controlled vocabulary from which the value
     * is taken.
     **/
    std::string m_auth;
    /**
     * The web-accessible location of the controlled vocabulary from which the value is
     * taken.
     **/
    std::string m_authUri;

    /* include <attauth.uri> */
};

//----------------------------------------------------------------------------
// AttBarLineLog
//----------------------------------------------------------------------------

class AttBarLineLog : public Att {
public:
    AttBarLineLog();
    virtual ~AttBarLineLog();

    /** Reset the default values for the attribute class **/
    void ResetBarLineLog();

    /** Read the values for the attribute class **/
    bool ReadBarLineLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBarLineLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(data_BARRENDITION form_) { m_form = form_; }
    data_BARRENDITION GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    data_BARRENDITION m_form;

    /* include <attform> */
};

//----------------------------------------------------------------------------
// AttBasic
//----------------------------------------------------------------------------

class AttBasic : public Att {
public:
    AttBasic();
    virtual ~AttBasic();

    /** Reset the default values for the attribute class **/
    void ResetBasic();

    /** Read the values for the attribute class **/
    bool ReadBasic(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBasic(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBase(std::string base_) { m_base = base_; }
    std::string GetBase() const { return m_base; }
    bool HasBase() const;
    ///@}

private:
    /** --- **/
    std::string m_base;

    /* include <attbase> */
};

//----------------------------------------------------------------------------
// AttBibl
//----------------------------------------------------------------------------

class AttBibl : public Att {
public:
    AttBibl();
    virtual ~AttBibl();

    /** Reset the default values for the attribute class **/
    void ResetBibl();

    /** Read the values for the attribute class **/
    bool ReadBibl(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBibl(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAnalog(std::string analog_) { m_analog = analog_; }
    std::string GetAnalog() const { return m_analog; }
    bool HasAnalog() const;
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

class AttCalendared : public Att {
public:
    AttCalendared();
    virtual ~AttCalendared();

    /** Reset the default values for the attribute class **/
    void ResetCalendared();

    /** Read the values for the attribute class **/
    bool ReadCalendared(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCalendared(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCalendar(std::string calendar_) { m_calendar = calendar_; }
    std::string GetCalendar() const { return m_calendar; }
    bool HasCalendar() const;
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

class AttCanonical : public Att {
public:
    AttCanonical();
    virtual ~AttCanonical();

    /** Reset the default values for the attribute class **/
    void ResetCanonical();

    /** Read the values for the attribute class **/
    bool ReadCanonical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCanonical(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCodedval(std::string codedval_) { m_codedval = codedval_; }
    std::string GetCodedval() const { return m_codedval; }
    bool HasCodedval() const;
    ///@}

private:
    /**
     * A value that represents or identifies the element content.
     * May serve as a primary key in a web-accessible database identified by the
     **/
    std::string m_codedval;

    /* include <attcodedval> */
};

//----------------------------------------------------------------------------
// AttClassed
//----------------------------------------------------------------------------

class AttClassed : public Att {
public:
    AttClassed();
    virtual ~AttClassed();

    /** Reset the default values for the attribute class **/
    void ResetClassed();

    /** Read the values for the attribute class **/
    bool ReadClassed(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteClassed(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetClass(std::string class_) { m_class = class_; }
    std::string GetClass() const { return m_class; }
    bool HasClass() const;
    ///@}

private:
    /**
     * Contains one or more URIs which denote classification terms that apply to the
     * entity bearing this attribute.
     **/
    std::string m_class;

    /* include <attclass> */
};

//----------------------------------------------------------------------------
// AttClefLog
//----------------------------------------------------------------------------

class AttClefLog : public Att {
public:
    AttClefLog();
    virtual ~AttClefLog();

    /** Reset the default values for the attribute class **/
    void ResetClefLog();

    /** Read the values for the attribute class **/
    bool ReadClefLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteClefLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCautionary(data_BOOLEAN cautionary_) { m_cautionary = cautionary_; }
    data_BOOLEAN GetCautionary() const { return m_cautionary; }
    bool HasCautionary() const;
    ///@}

private:
    /**
     * Records the function of the clef.
     * A "cautionary" clef does not change the following pitches.
     **/
    data_BOOLEAN m_cautionary;

    /* include <attcautionary> */
};

//----------------------------------------------------------------------------
// AttClefShape
//----------------------------------------------------------------------------

class AttClefShape : public Att {
public:
    AttClefShape();
    virtual ~AttClefShape();

    /** Reset the default values for the attribute class **/
    void ResetClefShape();

    /** Read the values for the attribute class **/
    bool ReadClefShape(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteClefShape(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetShape(data_CLEFSHAPE shape_) { m_shape = shape_; }
    data_CLEFSHAPE GetShape() const { return m_shape; }
    bool HasShape() const;
    ///@}

private:
    /** Describes a clef's shape. **/
    data_CLEFSHAPE m_shape;

    /* include <attshape> */
};

//----------------------------------------------------------------------------
// AttCleffingLog
//----------------------------------------------------------------------------

class AttCleffingLog : public Att {
public:
    AttCleffingLog();
    virtual ~AttCleffingLog();

    /** Reset the default values for the attribute class **/
    void ResetCleffingLog();

    /** Read the values for the attribute class **/
    bool ReadCleffingLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCleffingLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetClefShape(data_CLEFSHAPE clefShape_) { m_clefShape = clefShape_; }
    data_CLEFSHAPE GetClefShape() const { return m_clefShape; }
    bool HasClefShape() const;
    //
    void SetClefLine(char clefLine_) { m_clefLine = clefLine_; }
    char GetClefLine() const { return m_clefLine; }
    bool HasClefLine() const;
    //
    void SetClefDis(data_OCTAVE_DIS clefDis_) { m_clefDis = clefDis_; }
    data_OCTAVE_DIS GetClefDis() const { return m_clefDis; }
    bool HasClefDis() const;
    //
    void SetClefDisPlace(data_STAFFREL_basic clefDisPlace_) { m_clefDisPlace = clefDisPlace_; }
    data_STAFFREL_basic GetClefDisPlace() const { return m_clefDisPlace; }
    bool HasClefDisPlace() const;
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
    data_STAFFREL_basic m_clefDisPlace;

    /* include <attclef.dis.place> */
};

//----------------------------------------------------------------------------
// AttColor
//----------------------------------------------------------------------------

class AttColor : public Att {
public:
    AttColor();
    virtual ~AttColor();

    /** Reset the default values for the attribute class **/
    void ResetColor();

    /** Read the values for the attribute class **/
    bool ReadColor(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteColor(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetColor(std::string color_) { m_color = color_; }
    std::string GetColor() const { return m_color; }
    bool HasColor() const;
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

class AttColoration : public Att {
public:
    AttColoration();
    virtual ~AttColoration();

    /** Reset the default values for the attribute class **/
    void ResetColoration();

    /** Read the values for the attribute class **/
    bool ReadColoration(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteColoration(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetColored(data_BOOLEAN colored_) { m_colored = colored_; }
    data_BOOLEAN GetColored() const { return m_colored; }
    bool HasColored() const;
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
// AttCoordinated
//----------------------------------------------------------------------------

class AttCoordinated : public Att {
public:
    AttCoordinated();
    virtual ~AttCoordinated();

    /** Reset the default values for the attribute class **/
    void ResetCoordinated();

    /** Read the values for the attribute class **/
    bool ReadCoordinated(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCoordinated(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetUlx(int ulx_) { m_ulx = ulx_; }
    int GetUlx() const { return m_ulx; }
    bool HasUlx() const;
    //
    void SetUly(int uly_) { m_uly = uly_; }
    int GetUly() const { return m_uly; }
    bool HasUly() const;
    //
    void SetLrx(int lrx_) { m_lrx = lrx_; }
    int GetLrx() const { return m_lrx; }
    bool HasLrx() const;
    //
    void SetLry(int lry_) { m_lry = lry_; }
    int GetLry() const { return m_lry; }
    bool HasLry() const;
    ///@}

private:
    /** Indicates the upper-left corner x coordinate. **/
    int m_ulx;
    /** Indicates the upper-left corner y coordinate. **/
    int m_uly;
    /** Indicates the lower-right corner x coordinate. **/
    int m_lrx;
    /** Indicates the lower-left corner x coordinate. **/
    int m_lry;

    /* include <attlry> */
};

//----------------------------------------------------------------------------
// AttCurvature
//----------------------------------------------------------------------------

class AttCurvature : public Att {
public:
    AttCurvature();
    virtual ~AttCurvature();

    /** Reset the default values for the attribute class **/
    void ResetCurvature();

    /** Read the values for the attribute class **/
    bool ReadCurvature(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCurvature(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBezier(std::string bezier_) { m_bezier = bezier_; }
    std::string GetBezier() const { return m_bezier; }
    bool HasBezier() const;
    //
    void SetBulge(double bulge_) { m_bulge = bulge_; }
    double GetBulge() const { return m_bulge; }
    bool HasBulge() const;
    //
    void SetCurvedir(curvature_CURVEDIR curvedir_) { m_curvedir = curvedir_; }
    curvature_CURVEDIR GetCurvedir() const { return m_curvedir; }
    bool HasCurvedir() const;
    ///@}

private:
    /**
     * Records the placement of Bezier control points as a series of pairs of space-
     * separated values; e.g., 19 45 -32 118.
     **/
    std::string m_bezier;
    /**
     * Describes a curve as one or more pairs of values with respect to an imaginary
     * line connecting the starting and ending points of the curve.
     * The first value captures a distance to the left (positive value) or right
     * (negative value) of the line, expressed in virtual units. The second value of
     * each pair represents a point along the line, expressed as a percentage of the
     * line's length. N.B. An MEI virtual unit (VU) is half the distance between
     * adjacent staff lines.
     **/
    double m_bulge;
    /** Describes a curve with a generic term indicating the direction of curvature. **/
    curvature_CURVEDIR m_curvedir;

    /* include <attcurvedir> */
};

//----------------------------------------------------------------------------
// AttCurveRend
//----------------------------------------------------------------------------

class AttCurveRend : public Att {
public:
    AttCurveRend();
    virtual ~AttCurveRend();

    /** Reset the default values for the attribute class **/
    void ResetCurveRend();

    /** Read the values for the attribute class **/
    bool ReadCurveRend(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCurveRend(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLform(data_LINEFORM lform_) { m_lform = lform_; }
    data_LINEFORM GetLform() const { return m_lform; }
    bool HasLform() const;
    //
    void SetLwidth(std::string lwidth_) { m_lwidth = lwidth_; }
    std::string GetLwidth() const { return m_lwidth; }
    bool HasLwidth() const;
    ///@}

private:
    /** Describes the line style of a curve. **/
    data_LINEFORM m_lform;
    /** Width of a curved line. **/
    std::string m_lwidth;

    /* include <attlwidth> */
};

//----------------------------------------------------------------------------
// AttCustosLog
//----------------------------------------------------------------------------

class AttCustosLog : public Att {
public:
    AttCustosLog();
    virtual ~AttCustosLog();

    /** Reset the default values for the attribute class **/
    void ResetCustosLog();

    /** Read the values for the attribute class **/
    bool ReadCustosLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCustosLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTarget(std::string target_) { m_target = target_; }
    std::string GetTarget() const { return m_target; }
    bool HasTarget() const;
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
// AttDataPointing
//----------------------------------------------------------------------------

class AttDataPointing : public Att {
public:
    AttDataPointing();
    virtual ~AttDataPointing();

    /** Reset the default values for the attribute class **/
    void ResetDataPointing();

    /** Read the values for the attribute class **/
    bool ReadDataPointing(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDataPointing(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetData(std::string data_) { m_data = data_; }
    std::string GetData() const { return m_data; }
    bool HasData() const;
    ///@}

private:
    /** Used to link metadata elements to one or more data-containing elements. **/
    std::string m_data;

    /* include <attdata> */
};

//----------------------------------------------------------------------------
// AttDatable
//----------------------------------------------------------------------------

class AttDatable : public Att {
public:
    AttDatable();
    virtual ~AttDatable();

    /** Reset the default values for the attribute class **/
    void ResetDatable();

    /** Read the values for the attribute class **/
    bool ReadDatable(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDatable(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetEnddate(std::string enddate_) { m_enddate = enddate_; }
    std::string GetEnddate() const { return m_enddate; }
    bool HasEnddate() const;
    //
    void SetIsodate(std::string isodate_) { m_isodate = isodate_; }
    std::string GetIsodate() const { return m_isodate; }
    bool HasIsodate() const;
    //
    void SetNotafter(std::string notafter_) { m_notafter = notafter_; }
    std::string GetNotafter() const { return m_notafter; }
    bool HasNotafter() const;
    //
    void SetNotbefore(std::string notbefore_) { m_notbefore = notbefore_; }
    std::string GetNotbefore() const { return m_notbefore; }
    bool HasNotbefore() const;
    //
    void SetStartdate(std::string startdate_) { m_startdate = startdate_; }
    std::string GetStartdate() const { return m_startdate; }
    bool HasStartdate() const;
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
// AttDotLog
//----------------------------------------------------------------------------

class AttDotLog : public Att {
public:
    AttDotLog();
    virtual ~AttDotLog();

    /** Reset the default values for the attribute class **/
    void ResetDotLog();

    /** Read the values for the attribute class **/
    bool ReadDotLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDotLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(dotLog_FORM form_) { m_form = form_; }
    dotLog_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    dotLog_FORM m_form;

    /* include <attform> */
};

//----------------------------------------------------------------------------
// AttDurationAdditive
//----------------------------------------------------------------------------

class AttDurationAdditive : public Att {
public:
    AttDurationAdditive();
    virtual ~AttDurationAdditive();

    /** Reset the default values for the attribute class **/
    void ResetDurationAdditive();

    /** Read the values for the attribute class **/
    bool ReadDurationAdditive(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDurationAdditive(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDur(data_DURATION dur_) { m_dur = dur_; }
    data_DURATION GetDur() const { return m_dur; }
    bool HasDur() const;
    ///@}

private:
    /**
     * When a duration cannot be represented as a single power-of-two value, multiple
     * space-separated values that add up to the total duration may be used.
     **/
    data_DURATION m_dur;

    /* include <attdur> */
};

//----------------------------------------------------------------------------
// AttDurationDefault
//----------------------------------------------------------------------------

class AttDurationDefault : public Att {
public:
    AttDurationDefault();
    virtual ~AttDurationDefault();

    /** Reset the default values for the attribute class **/
    void ResetDurationDefault();

    /** Read the values for the attribute class **/
    bool ReadDurationDefault(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDurationDefault(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDurDefault(data_DURATION durDefault_) { m_durDefault = durDefault_; }
    data_DURATION GetDurDefault() const { return m_durDefault; }
    bool HasDurDefault() const;
    //
    void SetNumDefault(int numDefault_) { m_numDefault = numDefault_; }
    int GetNumDefault() const { return m_numDefault; }
    bool HasNumDefault() const;
    //
    void SetNumbaseDefault(int numbaseDefault_) { m_numbaseDefault = numbaseDefault_; }
    int GetNumbaseDefault() const { return m_numbaseDefault; }
    bool HasNumbaseDefault() const;
    ///@}

private:
    /**
     * Contains a default duration in those situations when the first note, rest,
     * chord, etc.
     * in a measure does not have a duration specified.
     **/
    data_DURATION m_durDefault;
    /**
     * Along with numbase.default, describes the default duration as a ratio.
     * num.default is the first value in the ratio.
     **/
    int m_numDefault;
    /**
     * Along with num.default, describes the default duration as a ratio.
     * numbase.default is the second value in the ratio.
     **/
    int m_numbaseDefault;

    /* include <attnumbase.default> */
};

//----------------------------------------------------------------------------
// AttDurationLogical
//----------------------------------------------------------------------------

class AttDurationLogical : public Att {
public:
    AttDurationLogical();
    virtual ~AttDurationLogical();

    /** Reset the default values for the attribute class **/
    void ResetDurationLogical();

    /** Read the values for the attribute class **/
    bool ReadDurationLogical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDurationLogical(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDur(data_DURATION dur_) { m_dur = dur_; }
    data_DURATION GetDur() const { return m_dur; }
    bool HasDur() const;
    ///@}

private:
    /**
     * When a duration cannot be represented as a single power-of-two value, multiple
     * space-separated values that add up to the total duration may be used.
     **/
    data_DURATION m_dur;

    /* include <attdur> */
};

//----------------------------------------------------------------------------
// AttDurationRatio
//----------------------------------------------------------------------------

class AttDurationRatio : public Att {
public:
    AttDurationRatio();
    virtual ~AttDurationRatio();

    /** Reset the default values for the attribute class **/
    void ResetDurationRatio();

    /** Read the values for the attribute class **/
    bool ReadDurationRatio(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDurationRatio(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetNum(int num_) { m_num = num_; }
    int GetNum() const { return m_num; }
    bool HasNum() const;
    //
    void SetNumbase(int numbase_) { m_numbase = numbase_; }
    int GetNumbase() const { return m_numbase; }
    bool HasNumbase() const;
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
// AttEvidence
//----------------------------------------------------------------------------

class AttEvidence : public Att {
public:
    AttEvidence();
    virtual ~AttEvidence();

    /** Reset the default values for the attribute class **/
    void ResetEvidence();

    /** Read the values for the attribute class **/
    bool ReadEvidence(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteEvidence(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCert(data_CERTAINTY cert_) { m_cert = cert_; }
    data_CERTAINTY GetCert() const { return m_cert; }
    bool HasCert() const;
    //
    void SetEvidence(std::string evidence_) { m_evidence = evidence_; }
    std::string GetEvidence() const { return m_evidence; }
    bool HasEvidence() const;
    ///@}

private:
    /** Signifies the degree of certainty or precision associated with a feature. **/
    data_CERTAINTY m_cert;
    /**
     * Indicates the nature of the evidence supporting the reliability or accuracy of
     * the intervention or interpretation.
     * Suggested values include: 'internal', 'external', 'conjecture'.
     **/
    std::string m_evidence;

    /* include <attevidence> */
};

//----------------------------------------------------------------------------
// AttExtent
//----------------------------------------------------------------------------

class AttExtent : public Att {
public:
    AttExtent();
    virtual ~AttExtent();

    /** Reset the default values for the attribute class **/
    void ResetExtent();

    /** Read the values for the attribute class **/
    bool ReadExtent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteExtent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetExtent(std::string extent_) { m_extent = extent_; }
    std::string GetExtent() const { return m_extent; }
    bool HasExtent() const;
    ///@}

private:
    /**
     * Captures a measurement, count, or description.
     * When extent contains a numeric value, use the unit attribute to indicate the
     * measurement unit.
     **/
    std::string m_extent;

    /* include <attextent> */
};

//----------------------------------------------------------------------------
// AttFiling
//----------------------------------------------------------------------------

class AttFiling : public Att {
public:
    AttFiling();
    virtual ~AttFiling();

    /** Reset the default values for the attribute class **/
    void ResetFiling();

    /** Read the values for the attribute class **/
    bool ReadFiling(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteFiling(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetNonfiling(int nonfiling_) { m_nonfiling = nonfiling_; }
    int GetNonfiling() const { return m_nonfiling; }
    bool HasNonfiling() const;
    ///@}

private:
    /**
     * Holds the number of initial characters (such as those constituing an article or
     * preposition) that should not be used for sorting a title or name.
     **/
    int m_nonfiling;

    /* include <attnonfiling> */
};

//----------------------------------------------------------------------------
// AttGrpSymLog
//----------------------------------------------------------------------------

class AttGrpSymLog : public Att {
public:
    AttGrpSymLog();
    virtual ~AttGrpSymLog();

    /** Reset the default values for the attribute class **/
    void ResetGrpSymLog();

    /** Read the values for the attribute class **/
    bool ReadGrpSymLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteGrpSymLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLevel(int level_) { m_level = level_; }
    int GetLevel() const { return m_level; }
    bool HasLevel() const;
    ///@}

private:
    /** Indicates the nesting level of staff grouping symbols. **/
    int m_level;

    /* include <attlevel> */
};

//----------------------------------------------------------------------------
// AttHandIdent
//----------------------------------------------------------------------------

class AttHandIdent : public Att {
public:
    AttHandIdent();
    virtual ~AttHandIdent();

    /** Reset the default values for the attribute class **/
    void ResetHandIdent();

    /** Read the values for the attribute class **/
    bool ReadHandIdent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHandIdent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetHand(std::string hand_) { m_hand = hand_; }
    std::string GetHand() const { return m_hand; }
    bool HasHand() const;
    ///@}

private:
    /**
     * Signifies the hand responsible for an action.
     * The value must be the ID of a
     **/
    std::string m_hand;

    /* include <atthand> */
};

//----------------------------------------------------------------------------
// AttHeight
//----------------------------------------------------------------------------

class AttHeight : public Att {
public:
    AttHeight();
    virtual ~AttHeight();

    /** Reset the default values for the attribute class **/
    void ResetHeight();

    /** Read the values for the attribute class **/
    bool ReadHeight(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHeight(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetHeight(std::string height_) { m_height = height_; }
    std::string GetHeight() const { return m_height; }
    bool HasHeight() const;
    ///@}

private:
    /** Measurement of the vertical dimension of an entity. **/
    std::string m_height;

    /* include <attheight> */
};

//----------------------------------------------------------------------------
// AttHorizontalAlign
//----------------------------------------------------------------------------

class AttHorizontalAlign : public Att {
public:
    AttHorizontalAlign();
    virtual ~AttHorizontalAlign();

    /** Reset the default values for the attribute class **/
    void ResetHorizontalAlign();

    /** Read the values for the attribute class **/
    bool ReadHorizontalAlign(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHorizontalAlign(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetHalign(data_HORIZONTALALIGNMENT halign_) { m_halign = halign_; }
    data_HORIZONTALALIGNMENT GetHalign() const { return m_halign; }
    bool HasHalign() const;
    ///@}

private:
    /** Records horizontal alignment. **/
    data_HORIZONTALALIGNMENT m_halign;

    /* include <atthalign> */
};

//----------------------------------------------------------------------------
// AttInternetMedia
//----------------------------------------------------------------------------

class AttInternetMedia : public Att {
public:
    AttInternetMedia();
    virtual ~AttInternetMedia();

    /** Reset the default values for the attribute class **/
    void ResetInternetMedia();

    /** Read the values for the attribute class **/
    bool ReadInternetMedia(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteInternetMedia(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMimetype(std::string mimetype_) { m_mimetype = mimetype_; }
    std::string GetMimetype() const { return m_mimetype; }
    bool HasMimetype() const;
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
// AttKeySigLog
//----------------------------------------------------------------------------

class AttKeySigLog : public Att {
public:
    AttKeySigLog();
    virtual ~AttKeySigLog();

    /** Reset the default values for the attribute class **/
    void ResetKeySigLog();

    /** Read the values for the attribute class **/
    bool ReadKeySigLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteKeySigLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFifths(std::string fifths_) { m_fifths = fifths_; }
    std::string GetFifths() const { return m_fifths; }
    bool HasFifths() const;
    //
    void SetSig(std::string sig_) { m_sig = sig_; }
    std::string GetSig() const { return m_sig; }
    bool HasSig() const;
    ///@}

private:
    /** Indicates where the key lies in the circle of fifths. **/
    std::string m_fifths;
    /** Written key signature. **/
    std::string m_sig;

    /* include <attsig> */
};

//----------------------------------------------------------------------------
// AttKeySigDefaultLog
//----------------------------------------------------------------------------

class AttKeySigDefaultLog : public Att {
public:
    AttKeySigDefaultLog();
    virtual ~AttKeySigDefaultLog();

    /** Reset the default values for the attribute class **/
    void ResetKeySigDefaultLog();

    /** Read the values for the attribute class **/
    bool ReadKeySigDefaultLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteKeySigDefaultLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetKeyFifths(std::string keyFifths_) { m_keyFifths = keyFifths_; }
    std::string GetKeyFifths() const { return m_keyFifths; }
    bool HasKeyFifths() const;
    //
    void SetKeySig(std::string keySig_) { m_keySig = keySig_; }
    std::string GetKeySig() const { return m_keySig; }
    bool HasKeySig() const;
    ///@}

private:
    /** Indicates where the key lies in the circle of fifths. **/
    std::string m_keyFifths;
    /** Written key signature. **/
    std::string m_keySig;

    /* include <attkey.sig> */
};

//----------------------------------------------------------------------------
// AttLabelled
//----------------------------------------------------------------------------

class AttLabelled : public Att {
public:
    AttLabelled();
    virtual ~AttLabelled();

    /** Reset the default values for the attribute class **/
    void ResetLabelled();

    /** Read the values for the attribute class **/
    bool ReadLabelled(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLabelled(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLabel(std::string label_) { m_label = label_; }
    std::string GetLabel() const { return m_label; }
    bool HasLabel() const;
    ///@}

private:
    /**
     * Provides a name or label for an element.
     * Should not be used to capture document content.
     **/
    std::string m_label;

    /* include <attlabel> */
};

//----------------------------------------------------------------------------
// AttLang
//----------------------------------------------------------------------------

class AttLang : public Att {
public:
    AttLang();
    virtual ~AttLang();

    /** Reset the default values for the attribute class **/
    void ResetLang();

    /** Read the values for the attribute class **/
    bool ReadLang(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLang(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLang(std::string lang_) { m_lang = lang_; }
    std::string GetLang() const { return m_lang; }
    bool HasLang() const;
    //
    void SetTranslit(std::string translit_) { m_translit = translit_; }
    std::string GetTranslit() const { return m_translit; }
    bool HasTranslit() const;
    ///@}

private:
    /** --- **/
    std::string m_lang;
    /** Specifies the transliteration technique used. **/
    std::string m_translit;

    /* include <atttranslit> */
};

//----------------------------------------------------------------------------
// AttLayerLog
//----------------------------------------------------------------------------

class AttLayerLog : public Att {
public:
    AttLayerLog();
    virtual ~AttLayerLog();

    /** Reset the default values for the attribute class **/
    void ResetLayerLog();

    /** Read the values for the attribute class **/
    bool ReadLayerLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLayerLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDef(std::string def_) { m_def = def_; }
    std::string GetDef() const { return m_def; }
    bool HasDef() const;
    ///@}

private:
    /** Provides a mechanism for linking the layer to a layerDef element. **/
    std::string m_def;

    /* include <attdef> */
};

//----------------------------------------------------------------------------
// AttLayerIdent
//----------------------------------------------------------------------------

class AttLayerIdent : public Att {
public:
    AttLayerIdent();
    virtual ~AttLayerIdent();

    /** Reset the default values for the attribute class **/
    void ResetLayerIdent();

    /** Read the values for the attribute class **/
    bool ReadLayerIdent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLayerIdent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLayer(int layer_) { m_layer = layer_; }
    int GetLayer() const { return m_layer; }
    bool HasLayer() const;
    ///@}

private:
    /** Identifies the layer to which a feature applies. **/
    int m_layer;

    /* include <attlayer> */
};

//----------------------------------------------------------------------------
// AttLineLoc
//----------------------------------------------------------------------------

class AttLineLoc : public Att {
public:
    AttLineLoc();
    virtual ~AttLineLoc();

    /** Reset the default values for the attribute class **/
    void ResetLineLoc();

    /** Read the values for the attribute class **/
    bool ReadLineLoc(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLineLoc(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLine(char line_) { m_line = line_; }
    char GetLine() const { return m_line; }
    bool HasLine() const;
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
// AttLinking
//----------------------------------------------------------------------------

class AttLinking : public Att {
public:
    AttLinking();
    virtual ~AttLinking();

    /** Reset the default values for the attribute class **/
    void ResetLinking();

    /** Read the values for the attribute class **/
    bool ReadLinking(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLinking(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCopyof(std::string copyof_) { m_copyof = copyof_; }
    std::string GetCopyof() const { return m_copyof; }
    bool HasCopyof() const;
    //
    void SetCorresp(std::string corresp_) { m_corresp = corresp_; }
    std::string GetCorresp() const { return m_corresp; }
    bool HasCorresp() const;
    //
    void SetNext(std::string next_) { m_next = next_; }
    std::string GetNext() const { return m_next; }
    bool HasNext() const;
    //
    void SetPrev(std::string prev_) { m_prev = prev_; }
    std::string GetPrev() const { return m_prev; }
    bool HasPrev() const;
    //
    void SetSameas(std::string sameas_) { m_sameas = sameas_; }
    std::string GetSameas() const { return m_sameas; }
    bool HasSameas() const;
    //
    void SetSynch(std::string synch_) { m_synch = synch_; }
    std::string GetSynch() const { return m_synch; }
    bool HasSynch() const;
    ///@}

private:
    /** Points to an element of which the current element is a copy. **/
    std::string m_copyof;
    /**
     * Used to point to other elements that correspond to this one in a generic
     * fashion.
     **/
    std::string m_corresp;
    /** Used to point to the next event(s) in a user-defined collection. **/
    std::string m_next;
    /** Points to the previous event(s) in a user-defined collection. **/
    std::string m_prev;
    /**
     * Points to an element that is the same as the current element but is not a
     * literal copy of the current element.
     **/
    std::string m_sameas;
    /** Points to elements that are synchronous with the current element. **/
    std::string m_synch;

    /* include <attsynch> */
};

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

class AttMeasureLog : public Att {
public:
    AttMeasureLog();
    virtual ~AttMeasureLog();

    /** Reset the default values for the attribute class **/
    void ResetMeasureLog();

    /** Read the values for the attribute class **/
    bool ReadMeasureLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeasureLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLeft(data_BARRENDITION left_) { m_left = left_; }
    data_BARRENDITION GetLeft() const { return m_left; }
    bool HasLeft() const;
    //
    void SetRight(data_BARRENDITION right_) { m_right = right_; }
    data_BARRENDITION GetRight() const { return m_right; }
    bool HasRight() const;
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

class AttMeasurement : public Att {
public:
    AttMeasurement();
    virtual ~AttMeasurement();

    /** Reset the default values for the attribute class **/
    void ResetMeasurement();

    /** Read the values for the attribute class **/
    bool ReadMeasurement(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeasurement(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetUnit(std::string unit_) { m_unit = unit_; }
    std::string GetUnit() const { return m_unit; }
    bool HasUnit() const;
    ///@}

private:
    /** Indicates the unit of measurement. **/
    std::string m_unit;

    /* include <attunit> */
};

//----------------------------------------------------------------------------
// AttMediaBounds
//----------------------------------------------------------------------------

class AttMediaBounds : public Att {
public:
    AttMediaBounds();
    virtual ~AttMediaBounds();

    /** Reset the default values for the attribute class **/
    void ResetMediaBounds();

    /** Read the values for the attribute class **/
    bool ReadMediaBounds(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMediaBounds(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBegin(std::string begin_) { m_begin = begin_; }
    std::string GetBegin() const { return m_begin; }
    bool HasBegin() const;
    //
    void SetEnd(std::string end_) { m_end = end_; }
    std::string GetEnd() const { return m_end; }
    bool HasEnd() const;
    //
    void SetBetype(data_BETYPE betype_) { m_betype = betype_; }
    data_BETYPE GetBetype() const { return m_betype; }
    bool HasBetype() const;
    ///@}

private:
    /**
     * Specifies a point where the relevant content begins.
     * A numerical value must be less and a time value must be earlier than that given
     * by the end attribute.
     **/
    std::string m_begin;
    /**
     * Specifies a point where the relevant content ends.
     * If not specified, the end of the content is assumed to be the end point. A
     * numerical value must be greater and a time value must be later than that given
     * by the begin attribute.
     **/
    std::string m_end;
    /**
     * Type of values used in the begin/end attributes.
     * The begin and end attributes can only be interpreted meaningfully in conjunction
     * with this attribute.
     **/
    data_BETYPE m_betype;

    /* include <attbetype> */
};

//----------------------------------------------------------------------------
// AttMedium
//----------------------------------------------------------------------------

class AttMedium : public Att {
public:
    AttMedium();
    virtual ~AttMedium();

    /** Reset the default values for the attribute class **/
    void ResetMedium();

    /** Read the values for the attribute class **/
    bool ReadMedium(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMedium(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMedium(std::string medium_) { m_medium = medium_; }
    std::string GetMedium() const { return m_medium; }
    bool HasMedium() const;
    ///@}

private:
    /** Describes the writing medium. **/
    std::string m_medium;

    /* include <attmedium> */
};

//----------------------------------------------------------------------------
// AttMeiVersion
//----------------------------------------------------------------------------

class AttMeiVersion : public Att {
public:
    AttMeiVersion();
    virtual ~AttMeiVersion();

    /** Reset the default values for the attribute class **/
    void ResetMeiVersion();

    /** Read the values for the attribute class **/
    bool ReadMeiVersion(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeiVersion(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMeiversion(meiVersion_MEIVERSION meiversion_) { m_meiversion = meiversion_; }
    meiVersion_MEIVERSION GetMeiversion() const { return m_meiversion; }
    bool HasMeiversion() const;
    ///@}

private:
    /** Specifies a generic MEI version label. **/
    meiVersion_MEIVERSION m_meiversion;

    /* include <attmeiversion> */
};

//----------------------------------------------------------------------------
// AttMensurLog
//----------------------------------------------------------------------------

class AttMensurLog : public Att {
public:
    AttMensurLog();
    virtual ~AttMensurLog();

    /** Reset the default values for the attribute class **/
    void ResetMensurLog();

    /** Read the values for the attribute class **/
    bool ReadMensurLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMensurLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDot(data_BOOLEAN dot_) { m_dot = dot_; }
    data_BOOLEAN GetDot() const { return m_dot; }
    bool HasDot() const;
    //
    void SetSign(data_MENSURATIONSIGN sign_) { m_sign = sign_; }
    data_MENSURATIONSIGN GetSign() const { return m_sign; }
    bool HasSign() const;
    ///@}

private:
    /** Specifies whether a dot is to be added to the base symbol. **/
    data_BOOLEAN m_dot;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    data_MENSURATIONSIGN m_sign;

    /* include <attsign> */
};

//----------------------------------------------------------------------------
// AttMetadataPointing
//----------------------------------------------------------------------------

class AttMetadataPointing : public Att {
public:
    AttMetadataPointing();
    virtual ~AttMetadataPointing();

    /** Reset the default values for the attribute class **/
    void ResetMetadataPointing();

    /** Read the values for the attribute class **/
    bool ReadMetadataPointing(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMetadataPointing(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDecls(std::string decls_) { m_decls = decls_; }
    std::string GetDecls() const { return m_decls; }
    bool HasDecls() const;
    ///@}

private:
    /**
     * Identifies one or more metadata elements (other than classification terms)
     * within the header, which are understood to apply to the element bearing this
     * attribute and its content.
     **/
    std::string m_decls;

    /* include <attdecls> */
};

//----------------------------------------------------------------------------
// AttMeterConformance
//----------------------------------------------------------------------------

class AttMeterConformance : public Att {
public:
    AttMeterConformance();
    virtual ~AttMeterConformance();

    /** Reset the default values for the attribute class **/
    void ResetMeterConformance();

    /** Read the values for the attribute class **/
    bool ReadMeterConformance(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeterConformance(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMetcon(meterConformance_METCON metcon_) { m_metcon = metcon_; }
    meterConformance_METCON GetMetcon() const { return m_metcon; }
    bool HasMetcon() const;
    ///@}

private:
    /**
     * Indicates the relationship between the content of a staff or layer and the
     * prevailing meter.
     **/
    meterConformance_METCON m_metcon;

    /* include <attmetcon> */
};

//----------------------------------------------------------------------------
// AttMeterConformanceBar
//----------------------------------------------------------------------------

class AttMeterConformanceBar : public Att {
public:
    AttMeterConformanceBar();
    virtual ~AttMeterConformanceBar();

    /** Reset the default values for the attribute class **/
    void ResetMeterConformanceBar();

    /** Read the values for the attribute class **/
    bool ReadMeterConformanceBar(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeterConformanceBar(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMetcon(data_BOOLEAN metcon_) { m_metcon = metcon_; }
    data_BOOLEAN GetMetcon() const { return m_metcon; }
    bool HasMetcon() const;
    //
    void SetControl(data_BOOLEAN control_) { m_control = control_; }
    data_BOOLEAN GetControl() const { return m_control; }
    bool HasControl() const;
    ///@}

private:
    /**
     * Indicates the relationship between the content of a staff or layer and the
     * prevailing meter.
     **/
    data_BOOLEAN m_metcon;
    /**
     * Indicates whether or not a bar line is "controlling"; that is, if it indicates a
     * point of alignment across all the parts.
     * Bar lines within a score are usually controlling; that is, they "line up". Bar
     * lines within parts may or may not be controlling. When applied to
     **/
    data_BOOLEAN m_control;

    /* include <attcontrol> */
};

//----------------------------------------------------------------------------
// AttMeterSigLog
//----------------------------------------------------------------------------

class AttMeterSigLog : public Att {
public:
    AttMeterSigLog();
    virtual ~AttMeterSigLog();

    /** Reset the default values for the attribute class **/
    void ResetMeterSigLog();

    /** Read the values for the attribute class **/
    bool ReadMeterSigLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeterSigLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCount(int count_) { m_count = count_; }
    int GetCount() const { return m_count; }
    bool HasCount() const;
    //
    void SetSym(data_METERSIGN sym_) { m_sym = sym_; }
    data_METERSIGN GetSym() const { return m_sym; }
    bool HasSym() const;
    //
    void SetUnit(int unit_) { m_unit = unit_; }
    int GetUnit() const { return m_unit; }
    bool HasUnit() const;
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
    /** Indicates the unit of measurement. **/
    int m_unit;

    /* include <attunit> */
};

//----------------------------------------------------------------------------
// AttMeterSigDefaultLog
//----------------------------------------------------------------------------

class AttMeterSigDefaultLog : public Att {
public:
    AttMeterSigDefaultLog();
    virtual ~AttMeterSigDefaultLog();

    /** Reset the default values for the attribute class **/
    void ResetMeterSigDefaultLog();

    /** Read the values for the attribute class **/
    bool ReadMeterSigDefaultLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeterSigDefaultLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMeterCount(int meterCount_) { m_meterCount = meterCount_; }
    int GetMeterCount() const { return m_meterCount; }
    bool HasMeterCount() const;
    //
    void SetMeterUnit(int meterUnit_) { m_meterUnit = meterUnit_; }
    int GetMeterUnit() const { return m_meterUnit; }
    bool HasMeterUnit() const;
    //
    void SetMeterSym(data_METERSIGN meterSym_) { m_meterSym = meterSym_; }
    data_METERSIGN GetMeterSym() const { return m_meterSym; }
    bool HasMeterSym() const;
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
    /**
     * Indicates the use of a meter symbol instead of a numeric meter signature, that
     * is, 'C' for common time or 'C' with a slash for cut time.
     **/
    data_METERSIGN m_meterSym;

    /* include <attmeter.sym> */
};

//----------------------------------------------------------------------------
// AttMmTempo
//----------------------------------------------------------------------------

class AttMmTempo : public Att {
public:
    AttMmTempo();
    virtual ~AttMmTempo();

    /** Reset the default values for the attribute class **/
    void ResetMmTempo();

    /** Read the values for the attribute class **/
    bool ReadMmTempo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMmTempo(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMm(std::string mm_) { m_mm = mm_; }
    std::string GetMm() const { return m_mm; }
    bool HasMm() const;
    //
    void SetMmUnit(data_DURATION mmUnit_) { m_mmUnit = mmUnit_; }
    data_DURATION GetMmUnit() const { return m_mmUnit; }
    bool HasMmUnit() const;
    //
    void SetMmDots(int mmDots_) { m_mmDots = mmDots_; }
    int GetMmDots() const { return m_mmDots; }
    bool HasMmDots() const;
    ///@}

private:
    /**
     * Used to describe tempo in terms of beats (often the meter signature denominator)
     * per minute, ala M.M.
     * (Maezel's Metronome). Do not confuse this attribute with midi.bpm or midi.mspb.
     * In MIDI, a beat is always defined as a quarter note, *not the numerator of the
     * time signature or the metronomic indication*.
     **/
    std::string m_mm;
    /** Captures the metronomic unit. **/
    data_DURATION m_mmUnit;
    /** Records the number of augmentation dots required by a dotted metronome unit. **/
    int m_mmDots;

    /* include <attmm.dots> */
};

//----------------------------------------------------------------------------
// AttNInteger
//----------------------------------------------------------------------------

class AttNInteger : public Att {
public:
    AttNInteger();
    virtual ~AttNInteger();

    /** Reset the default values for the attribute class **/
    void ResetNInteger();

    /** Read the values for the attribute class **/
    bool ReadNInteger(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNInteger(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetN(int n_) { m_n = n_; }
    int GetN() const { return m_n; }
    bool HasN() const;
    ///@}

private:
    /**
     * Provides a numeric designation that indicates an element's position in a
     * sequence of similar elements.
     * Its value must be a non-negative integer.
     **/
    int m_n;

    /* include <attn> */
};

//----------------------------------------------------------------------------
// AttNNumberLike
//----------------------------------------------------------------------------

class AttNNumberLike : public Att {
public:
    AttNNumberLike();
    virtual ~AttNNumberLike();

    /** Reset the default values for the attribute class **/
    void ResetNNumberLike();

    /** Read the values for the attribute class **/
    bool ReadNNumberLike(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNNumberLike(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetN(int n_) { m_n = n_; }
    int GetN() const { return m_n; }
    bool HasN() const;
    ///@}

private:
    /**
     * Provides a numeric designation that indicates an element's position in a
     * sequence of similar elements.
     * Its value must be a non-negative integer.
     **/
    int m_n;

    /* include <attn> */
};

//----------------------------------------------------------------------------
// AttName
//----------------------------------------------------------------------------

class AttName : public Att {
public:
    AttName();
    virtual ~AttName();

    /** Reset the default values for the attribute class **/
    void ResetName();

    /** Read the values for the attribute class **/
    bool ReadName(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteName(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetNymref(std::string nymref_) { m_nymref = nymref_; }
    std::string GetNymref() const { return m_nymref; }
    bool HasNymref() const;
    //
    void SetRole(std::string role_) { m_role = role_; }
    std::string GetRole() const { return m_role; }
    bool HasRole() const;
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
// AttOctave
//----------------------------------------------------------------------------

class AttOctave : public Att {
public:
    AttOctave();
    virtual ~AttOctave();

    /** Reset the default values for the attribute class **/
    void ResetOctave();

    /** Read the values for the attribute class **/
    bool ReadOctave(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOctave(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOct(char oct_) { m_oct = oct_; }
    char GetOct() const { return m_oct; }
    bool HasOct() const;
    ///@}

private:
    /** Captures written octave information. **/
    char m_oct;

    /* include <attoct> */
};

//----------------------------------------------------------------------------
// AttOctaveDefault
//----------------------------------------------------------------------------

class AttOctaveDefault : public Att {
public:
    AttOctaveDefault();
    virtual ~AttOctaveDefault();

    /** Reset the default values for the attribute class **/
    void ResetOctaveDefault();

    /** Read the values for the attribute class **/
    bool ReadOctaveDefault(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOctaveDefault(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOctDefault(char octDefault_) { m_octDefault = octDefault_; }
    char GetOctDefault() const { return m_octDefault; }
    bool HasOctDefault() const;
    ///@}

private:
    /**
     * Contains a default octave specification for use when the first note, rest,
     * chord, etc.
     * in a measure does not have an octave value specified.
     **/
    char m_octDefault;

    /* include <attoct.default> */
};

//----------------------------------------------------------------------------
// AttOctaveDisplacement
//----------------------------------------------------------------------------

class AttOctaveDisplacement : public Att {
public:
    AttOctaveDisplacement();
    virtual ~AttOctaveDisplacement();

    /** Reset the default values for the attribute class **/
    void ResetOctaveDisplacement();

    /** Read the values for the attribute class **/
    bool ReadOctaveDisplacement(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOctaveDisplacement(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDis(data_OCTAVE_DIS dis_) { m_dis = dis_; }
    data_OCTAVE_DIS GetDis() const { return m_dis; }
    bool HasDis() const;
    //
    void SetDisPlace(data_STAFFREL_basic disPlace_) { m_disPlace = disPlace_; }
    data_STAFFREL_basic GetDisPlace() const { return m_disPlace; }
    bool HasDisPlace() const;
    ///@}

private:
    /** Records the amount of octave displacement. **/
    data_OCTAVE_DIS m_dis;
    /** Records the direction of octave displacement. **/
    data_STAFFREL_basic m_disPlace;

    /* include <attdis.place> */
};

//----------------------------------------------------------------------------
// AttOriginLayerIdent
//----------------------------------------------------------------------------

class AttOriginLayerIdent : public Att {
public:
    AttOriginLayerIdent();
    virtual ~AttOriginLayerIdent();

    /** Reset the default values for the attribute class **/
    void ResetOriginLayerIdent();

    /** Read the values for the attribute class **/
    bool ReadOriginLayerIdent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOriginLayerIdent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOriginLayer(std::string originLayer_) { m_originLayer = originLayer_; }
    std::string GetOriginLayer() const { return m_originLayer; }
    bool HasOriginLayer() const;
    ///@}

private:
    /** Identifies the layer on which referenced notation occurs. **/
    std::string m_originLayer;

    /* include <attorigin.layer> */
};

//----------------------------------------------------------------------------
// AttOriginStaffIdent
//----------------------------------------------------------------------------

class AttOriginStaffIdent : public Att {
public:
    AttOriginStaffIdent();
    virtual ~AttOriginStaffIdent();

    /** Reset the default values for the attribute class **/
    void ResetOriginStaffIdent();

    /** Read the values for the attribute class **/
    bool ReadOriginStaffIdent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOriginStaffIdent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOriginStaff(std::string originStaff_) { m_originStaff = originStaff_; }
    std::string GetOriginStaff() const { return m_originStaff; }
    bool HasOriginStaff() const;
    ///@}

private:
    /**
     * Signifies the staff on which referenced notation occurs.
     * Defaults to the same value as the local staff. Mandatory when applicable.
     **/
    std::string m_originStaff;

    /* include <attorigin.staff> */
};

//----------------------------------------------------------------------------
// AttOriginStartEndId
//----------------------------------------------------------------------------

class AttOriginStartEndId : public Att {
public:
    AttOriginStartEndId();
    virtual ~AttOriginStartEndId();

    /** Reset the default values for the attribute class **/
    void ResetOriginStartEndId();

    /** Read the values for the attribute class **/
    bool ReadOriginStartEndId(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOriginStartEndId(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOriginStartid(std::string originStartid_) { m_originStartid = originStartid_; }
    std::string GetOriginStartid() const { return m_originStartid; }
    bool HasOriginStartid() const;
    //
    void SetOriginEndid(std::string originEndid_) { m_originEndid = originEndid_; }
    std::string GetOriginEndid() const { return m_originEndid; }
    bool HasOriginEndid() const;
    ///@}

private:
    /** Indicates the first element in a sequence of events. **/
    std::string m_originStartid;
    /** Indicates the final element in a sequence of events. **/
    std::string m_originEndid;

    /* include <attorigin.endid> */
};

//----------------------------------------------------------------------------
// AttOriginTimestampLogical
//----------------------------------------------------------------------------

class AttOriginTimestampLogical : public Att {
public:
    AttOriginTimestampLogical();
    virtual ~AttOriginTimestampLogical();

    /** Reset the default values for the attribute class **/
    void ResetOriginTimestampLogical();

    /** Read the values for the attribute class **/
    bool ReadOriginTimestampLogical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOriginTimestampLogical(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOriginTstamp(data_MEASUREBEAT originTstamp_) { m_originTstamp = originTstamp_; }
    data_MEASUREBEAT GetOriginTstamp() const { return m_originTstamp; }
    bool HasOriginTstamp() const;
    //
    void SetOriginTstamp2(data_MEASUREBEAT originTstamp2_) { m_originTstamp2 = originTstamp2_; }
    data_MEASUREBEAT GetOriginTstamp2() const { return m_originTstamp2; }
    bool HasOriginTstamp2() const;
    ///@}

private:
    /**
     * Encodes the starting point of musical material in terms of musical time, i.e., a
     * (potentially negative) count of measures plus a beat location.
     **/
    data_MEASUREBEAT m_originTstamp;
    /**
     * Encodes the ending point of musical material in terms of musical time, i.e., a
     * count of measures plus a beat location.
     * The values are relative to the measure identified by
     **/
    data_MEASUREBEAT m_originTstamp2;

    /* include <attorigin.tstamp2> */
};

//----------------------------------------------------------------------------
// AttPitch
//----------------------------------------------------------------------------

class AttPitch : public Att {
public:
    AttPitch();
    virtual ~AttPitch();

    /** Reset the default values for the attribute class **/
    void ResetPitch();

    /** Read the values for the attribute class **/
    bool ReadPitch(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePitch(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPname(data_PITCHNAME pname_) { m_pname = pname_; }
    data_PITCHNAME GetPname() const { return m_pname; }
    bool HasPname() const;
    ///@}

private:
    /** Contains a written pitch name. **/
    data_PITCHNAME m_pname;

    /* include <attpname> */
};

//----------------------------------------------------------------------------
// AttPlacement
//----------------------------------------------------------------------------

class AttPlacement : public Att {
public:
    AttPlacement();
    virtual ~AttPlacement();

    /** Reset the default values for the attribute class **/
    void ResetPlacement();

    /** Read the values for the attribute class **/
    bool ReadPlacement(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePlacement(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPlace(data_STAFFREL_extended place_) { m_place = place_; }
    data_STAFFREL_extended GetPlace() const { return m_place; }
    bool HasPlace() const;
    ///@}

private:
    /**
     * Captures the placement of the item with respect to the staff with which it is
     * associated.
     **/
    data_STAFFREL_extended m_place;

    /* include <attplace> */
};

//----------------------------------------------------------------------------
// AttPlist
//----------------------------------------------------------------------------

class AttPlist : public Att {
public:
    AttPlist();
    virtual ~AttPlist();

    /** Reset the default values for the attribute class **/
    void ResetPlist();

    /** Read the values for the attribute class **/
    bool ReadPlist(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePlist(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPlist(std::string plist_) { m_plist = plist_; }
    std::string GetPlist() const { return m_plist; }
    bool HasPlist() const;
    ///@}

private:
    /**
     * When the target attribute is present, plist identifies the active participants;
     * that is, those entities pointed "from", in a relationship with the specified
     * target(s).
     * When the target attribute is not present, it identifies participants in a mutual
     * relationship.
     **/
    std::string m_plist;

    /* include <attplist> */
};

//----------------------------------------------------------------------------
// AttPointing
//----------------------------------------------------------------------------

class AttPointing : public Att {
public:
    AttPointing();
    virtual ~AttPointing();

    /** Reset the default values for the attribute class **/
    void ResetPointing();

    /** Read the values for the attribute class **/
    bool ReadPointing(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePointing(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetActuate(std::string actuate_) { m_actuate = actuate_; }
    std::string GetActuate() const { return m_actuate; }
    bool HasActuate() const;
    //
    void SetRole(std::string role_) { m_role = role_; }
    std::string GetRole() const { return m_role; }
    bool HasRole() const;
    //
    void SetShow(std::string show_) { m_show = show_; }
    std::string GetShow() const { return m_show; }
    bool HasShow() const;
    //
    void SetTarget(std::string target_) { m_target = target_; }
    std::string GetTarget() const { return m_target; }
    bool HasTarget() const;
    //
    void SetTargettype(std::string targettype_) { m_targettype = targettype_; }
    std::string GetTargettype() const { return m_targettype; }
    bool HasTargettype() const;
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
     * Characterization of target resource(s) using any convenient classification
     * scheme or typology.
     **/
    std::string m_targettype;

    /* include <atttargettype> */
};

//----------------------------------------------------------------------------
// AttQuantity
//----------------------------------------------------------------------------

class AttQuantity : public Att {
public:
    AttQuantity();
    virtual ~AttQuantity();

    /** Reset the default values for the attribute class **/
    void ResetQuantity();

    /** Read the values for the attribute class **/
    bool ReadQuantity(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteQuantity(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetQuantity(double quantity_) { m_quantity = quantity_; }
    double GetQuantity() const { return m_quantity; }
    bool HasQuantity() const;
    ///@}

private:
    /**
     * Numeric value capturing a measurement or count.
     * Can only be interpreted in combination with the unit or currency attribute.
     **/
    double m_quantity;

    /* include <attquantity> */
};

//----------------------------------------------------------------------------
// AttResponsibility
//----------------------------------------------------------------------------

class AttResponsibility : public Att {
public:
    AttResponsibility();
    virtual ~AttResponsibility();

    /** Reset the default values for the attribute class **/
    void ResetResponsibility();

    /** Read the values for the attribute class **/
    bool ReadResponsibility(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteResponsibility(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetResp(std::string resp_) { m_resp = resp_; }
    std::string GetResp() const { return m_resp; }
    bool HasResp() const;
    ///@}

private:
    /**
     * Indicates the agent(s) responsible for some aspect of the text's creation,
     * transcription, editing, or encoding.
     * Its value must point to one or more identifiers declared in the document header.
     **/
    std::string m_resp;

    /* include <attresp> */
};

//----------------------------------------------------------------------------
// AttScalable
//----------------------------------------------------------------------------

class AttScalable : public Att {
public:
    AttScalable();
    virtual ~AttScalable();

    /** Reset the default values for the attribute class **/
    void ResetScalable();

    /** Read the values for the attribute class **/
    bool ReadScalable(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteScalable(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetScale(data_PERCENT scale_) { m_scale = scale_; }
    data_PERCENT GetScale() const { return m_scale; }
    bool HasScale() const;
    ///@}

private:
    /** Scale factor to be applied to the feature to make it the desired display size. **/
    data_PERCENT m_scale;

    /* include <attscale> */
};

//----------------------------------------------------------------------------
// AttSequence
//----------------------------------------------------------------------------

class AttSequence : public Att {
public:
    AttSequence();
    virtual ~AttSequence();

    /** Reset the default values for the attribute class **/
    void ResetSequence();

    /** Read the values for the attribute class **/
    bool ReadSequence(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSequence(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSeq(int seq_) { m_seq = seq_; }
    int GetSeq() const { return m_seq; }
    bool HasSeq() const;
    ///@}

private:
    /**
     * Used to assign a sequence number related to the order in which the encoded
     * features carrying this attribute are believed to have occurred.
     **/
    int m_seq;

    /* include <attseq> */
};

//----------------------------------------------------------------------------
// AttSlashCount
//----------------------------------------------------------------------------

class AttSlashCount : public Att {
public:
    AttSlashCount();
    virtual ~AttSlashCount();

    /** Reset the default values for the attribute class **/
    void ResetSlashCount();

    /** Read the values for the attribute class **/
    bool ReadSlashCount(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSlashCount(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSlash(char slash_) { m_slash = slash_; }
    char GetSlash() const { return m_slash; }
    bool HasSlash() const;
    ///@}

private:
    /** Indicates the number of slashes present. **/
    char m_slash;

    /* include <attslash> */
};

//----------------------------------------------------------------------------
// AttStaffLog
//----------------------------------------------------------------------------

class AttStaffLog : public Att {
public:
    AttStaffLog();
    virtual ~AttStaffLog();

    /** Reset the default values for the attribute class **/
    void ResetStaffLog();

    /** Read the values for the attribute class **/
    bool ReadStaffLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDef(std::string def_) { m_def = def_; }
    std::string GetDef() const { return m_def; }
    bool HasDef() const;
    ///@}

private:
    /** Provides a mechanism for linking the layer to a layerDef element. **/
    std::string m_def;

    /* include <attdef> */
};

//----------------------------------------------------------------------------
// AttStaffDefLog
//----------------------------------------------------------------------------

class AttStaffDefLog : public Att {
public:
    AttStaffDefLog();
    virtual ~AttStaffDefLog();

    /** Reset the default values for the attribute class **/
    void ResetStaffDefLog();

    /** Read the values for the attribute class **/
    bool ReadStaffDefLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffDefLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLines(int lines_) { m_lines = lines_; }
    int GetLines() const { return m_lines; }
    bool HasLines() const;
    ///@}

private:
    /** Indicates the number of staff lines. **/
    int m_lines;

    /* include <attlines> */
};

//----------------------------------------------------------------------------
// AttStaffGroupingSym
//----------------------------------------------------------------------------

class AttStaffGroupingSym : public Att {
public:
    AttStaffGroupingSym();
    virtual ~AttStaffGroupingSym();

    /** Reset the default values for the attribute class **/
    void ResetStaffGroupingSym();

    /** Read the values for the attribute class **/
    bool ReadStaffGroupingSym(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffGroupingSym(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSymbol(staffGroupingSym_SYMBOL symbol_) { m_symbol = symbol_; }
    staffGroupingSym_SYMBOL GetSymbol() const { return m_symbol; }
    bool HasSymbol() const;
    ///@}

private:
    /** Specifies the symbol used to group a set of staves. **/
    staffGroupingSym_SYMBOL m_symbol;

    /* include <attsymbol> */
};

//----------------------------------------------------------------------------
// AttStaffIdent
//----------------------------------------------------------------------------

class AttStaffIdent : public Att {
public:
    AttStaffIdent();
    virtual ~AttStaffIdent();

    /** Reset the default values for the attribute class **/
    void ResetStaffIdent();

    /** Read the values for the attribute class **/
    bool ReadStaffIdent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffIdent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStaff(int staff_) { m_staff = staff_; }
    int GetStaff() const { return m_staff; }
    bool HasStaff() const;
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
// AttStartEndId
//----------------------------------------------------------------------------

class AttStartEndId : public Att {
public:
    AttStartEndId();
    virtual ~AttStartEndId();

    /** Reset the default values for the attribute class **/
    void ResetStartEndId();

    /** Read the values for the attribute class **/
    bool ReadStartEndId(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStartEndId(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetEndid(std::string endid_) { m_endid = endid_; }
    std::string GetEndid() const { return m_endid; }
    bool HasEndid() const;
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
// AttStartId
//----------------------------------------------------------------------------

class AttStartId : public Att {
public:
    AttStartId();
    virtual ~AttStartId();

    /** Reset the default values for the attribute class **/
    void ResetStartId();

    /** Read the values for the attribute class **/
    bool ReadStartId(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStartId(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStartid(std::string startid_) { m_startid = startid_; }
    std::string GetStartid() const { return m_startid; }
    bool HasStartid() const;
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
// AttSylLog
//----------------------------------------------------------------------------

class AttSylLog : public Att {
public:
    AttSylLog();
    virtual ~AttSylLog();

    /** Reset the default values for the attribute class **/
    void ResetSylLog();

    /** Read the values for the attribute class **/
    bool ReadSylLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSylLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCon(sylLog_CON con_) { m_con = con_; }
    sylLog_CON GetCon() const { return m_con; }
    bool HasCon() const;
    //
    void SetWordpos(sylLog_WORDPOS wordpos_) { m_wordpos = wordpos_; }
    sylLog_WORDPOS GetWordpos() const { return m_wordpos; }
    bool HasWordpos() const;
    ///@}

private:
    /**
     * Describes the symbols typically used to indicate breaks between syllables and
     * their functions.
     **/
    sylLog_CON m_con;
    /** Records the position of a syllable within a word. **/
    sylLog_WORDPOS m_wordpos;

    /* include <attwordpos> */
};

//----------------------------------------------------------------------------
// AttSylText
//----------------------------------------------------------------------------

class AttSylText : public Att {
public:
    AttSylText();
    virtual ~AttSylText();

    /** Reset the default values for the attribute class **/
    void ResetSylText();

    /** Read the values for the attribute class **/
    bool ReadSylText(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSylText(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSyl(std::string syl_) { m_syl = syl_; }
    std::string GetSyl() const { return m_syl; }
    bool HasSyl() const;
    ///@}

private:
    /** Holds an associated sung text syllable. **/
    std::string m_syl;

    /* include <attsyl> */
};

//----------------------------------------------------------------------------
// AttTargetEval
//----------------------------------------------------------------------------

class AttTargetEval : public Att {
public:
    AttTargetEval();
    virtual ~AttTargetEval();

    /** Reset the default values for the attribute class **/
    void ResetTargetEval();

    /** Read the values for the attribute class **/
    bool ReadTargetEval(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTargetEval(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetEvaluate(targetEval_EVALUATE evaluate_) { m_evaluate = evaluate_; }
    targetEval_EVALUATE GetEvaluate() const { return m_evaluate; }
    bool HasEvaluate() const;
    ///@}

private:
    /**
     * Specifies the intended meaning when a participant in a relationship is itself a
     * pointer.
     **/
    targetEval_EVALUATE m_evaluate;

    /* include <attevaluate> */
};

//----------------------------------------------------------------------------
// AttTempoLog
//----------------------------------------------------------------------------

class AttTempoLog : public Att {
public:
    AttTempoLog();
    virtual ~AttTempoLog();

    /** Reset the default values for the attribute class **/
    void ResetTempoLog();

    /** Read the values for the attribute class **/
    bool ReadTempoLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTempoLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFunc(tempoLog_FUNC func_) { m_func = func_; }
    tempoLog_FUNC GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Records the function of an accidental. **/
    tempoLog_FUNC m_func;

    /* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttTextRendition
//----------------------------------------------------------------------------

class AttTextRendition : public Att {
public:
    AttTextRendition();
    virtual ~AttTextRendition();

    /** Reset the default values for the attribute class **/
    void ResetTextRendition();

    /** Read the values for the attribute class **/
    bool ReadTextRendition(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTextRendition(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAltrend(std::string altrend_) { m_altrend = altrend_; }
    std::string GetAltrend() const { return m_altrend; }
    bool HasAltrend() const;
    //
    void SetRend(std::string rend_) { m_rend = rend_; }
    std::string GetRend() const { return m_rend; }
    bool HasRend() const;
    ///@}

private:
    /** Used to extend the values of the rend attribute. **/
    std::string m_altrend;
    /** Captures the appearance of the element's contents using MEI-defined descriptors. **/
    std::string m_rend;

    /* include <attrend> */
};

//----------------------------------------------------------------------------
// AttTimestampLogical
//----------------------------------------------------------------------------

class AttTimestampLogical : public Att {
public:
    AttTimestampLogical();
    virtual ~AttTimestampLogical();

    /** Reset the default values for the attribute class **/
    void ResetTimestampLogical();

    /** Read the values for the attribute class **/
    bool ReadTimestampLogical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTimestampLogical(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTstamp(double tstamp_) { m_tstamp = tstamp_; }
    double GetTstamp() const { return m_tstamp; }
    bool HasTstamp() const;
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
// AttTimestamp2Logical
//----------------------------------------------------------------------------

class AttTimestamp2Logical : public Att {
public:
    AttTimestamp2Logical();
    virtual ~AttTimestamp2Logical();

    /** Reset the default values for the attribute class **/
    void ResetTimestamp2Logical();

    /** Read the values for the attribute class **/
    bool ReadTimestamp2Logical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTimestamp2Logical(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTstamp2(data_MEASUREBEAT tstamp2_) { m_tstamp2 = tstamp2_; }
    data_MEASUREBEAT GetTstamp2() const { return m_tstamp2; }
    bool HasTstamp2() const;
    ///@}

private:
    /**
     * Encodes the ending point of an event in terms of musical time, i.e., a count of
     * measures plus a beat location.
     **/
    data_MEASUREBEAT m_tstamp2;

    /* include <atttstamp2> */
};

//----------------------------------------------------------------------------
// AttTransposition
//----------------------------------------------------------------------------

class AttTransposition : public Att {
public:
    AttTransposition();
    virtual ~AttTransposition();

    /** Reset the default values for the attribute class **/
    void ResetTransposition();

    /** Read the values for the attribute class **/
    bool ReadTransposition(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTransposition(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTransDiat(double transDiat_) { m_transDiat = transDiat_; }
    double GetTransDiat() const { return m_transDiat; }
    bool HasTransDiat() const;
    //
    void SetTransSemi(double transSemi_) { m_transSemi = transSemi_; }
    double GetTransSemi() const { return m_transSemi; }
    bool HasTransSemi() const;
    ///@}

private:
    /**
     * Records the amount of diatonic pitch shift, e.g., C to C♯ = 0, C to D♭ = 1,
     * necessary to calculate the sounded pitch from the written one.
     **/
    double m_transDiat;
    /**
     * Records the amount of pitch shift in semitones, e.g., C to C♯ = 1, C to D♭ = 1,
     * necessary to calculate the sounded pitch from the written one.
     **/
    double m_transSemi;

    /* include <atttrans.semi> */
};

//----------------------------------------------------------------------------
// AttTyped
//----------------------------------------------------------------------------

class AttTyped : public Att {
public:
    AttTyped();
    virtual ~AttTyped();

    /** Reset the default values for the attribute class **/
    void ResetTyped();

    /** Read the values for the attribute class **/
    bool ReadTyped(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTyped(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetType(std::string type_) { m_type = type_; }
    std::string GetType() const { return m_type; }
    bool HasType() const;
    ///@}

private:
    /**
     * Designation which characterizes the element in some sense, using any convenient
     * classification scheme or typology that employs single-token labels.
     **/
    std::string m_type;

    /* include <atttype> */
};

//----------------------------------------------------------------------------
// AttTypography
//----------------------------------------------------------------------------

class AttTypography : public Att {
public:
    AttTypography();
    virtual ~AttTypography();

    /** Reset the default values for the attribute class **/
    void ResetTypography();

    /** Read the values for the attribute class **/
    bool ReadTypography(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTypography(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFontfam(std::string fontfam_) { m_fontfam = fontfam_; }
    std::string GetFontfam() const { return m_fontfam; }
    bool HasFontfam() const;
    //
    void SetFontname(std::string fontname_) { m_fontname = fontname_; }
    std::string GetFontname() const { return m_fontname; }
    bool HasFontname() const;
    //
    void SetFontsize(int fontsize_) { m_fontsize = fontsize_; }
    int GetFontsize() const { return m_fontsize; }
    bool HasFontsize() const;
    //
    void SetFontstyle(data_FONTSTYLE fontstyle_) { m_fontstyle = fontstyle_; }
    data_FONTSTYLE GetFontstyle() const { return m_fontstyle; }
    bool HasFontstyle() const;
    //
    void SetFontweight(data_FONTWEIGHT fontweight_) { m_fontweight = fontweight_; }
    data_FONTWEIGHT GetFontweight() const { return m_fontweight; }
    bool HasFontweight() const;
    ///@}

private:
    /** Contains the name of a font-family. **/
    std::string m_fontfam;
    /** Holds the name of a font. **/
    std::string m_fontname;
    /**
     * Indicates the size of a font expressed in printers' points, i.e., 1/72nd of an
     * inch, relative terms, e.g., "small", "larger", etc., or percentage values
     * relative to "normal" size, e.g., "125%".
     **/
    int m_fontsize;
    /** Records the style of a font, i.e, italic, oblique, or normal. **/
    data_FONTSTYLE m_fontstyle;
    /** Used to indicate bold type. **/
    data_FONTWEIGHT m_fontweight;

    /* include <attfontweight> */
};

//----------------------------------------------------------------------------
// AttVisibility
//----------------------------------------------------------------------------

class AttVisibility : public Att {
public:
    AttVisibility();
    virtual ~AttVisibility();

    /** Reset the default values for the attribute class **/
    void ResetVisibility();

    /** Read the values for the attribute class **/
    bool ReadVisibility(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisibility(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetVisible(data_BOOLEAN visible_) { m_visible = visible_; }
    data_BOOLEAN GetVisible() const { return m_visible; }
    bool HasVisible() const;
    ///@}

private:
    /**
     * Indicates if a feature should be rendered when the notation is presented
     * graphically or sounded when it is presented in an aural form.
     **/
    data_BOOLEAN m_visible;

    /* include <attvisible> */
};

//----------------------------------------------------------------------------
// AttVisualOffsetHo
//----------------------------------------------------------------------------

class AttVisualOffsetHo : public Att {
public:
    AttVisualOffsetHo();
    virtual ~AttVisualOffsetHo();

    /** Reset the default values for the attribute class **/
    void ResetVisualOffsetHo();

    /** Read the values for the attribute class **/
    bool ReadVisualOffsetHo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualOffsetHo(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetHo(std::string ho_) { m_ho = ho_; }
    std::string GetHo() const { return m_ho; }
    bool HasHo() const;
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
// AttVisualOffsetTo
//----------------------------------------------------------------------------

class AttVisualOffsetTo : public Att {
public:
    AttVisualOffsetTo();
    virtual ~AttVisualOffsetTo();

    /** Reset the default values for the attribute class **/
    void ResetVisualOffsetTo();

    /** Read the values for the attribute class **/
    bool ReadVisualOffsetTo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualOffsetTo(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTo(double to_) { m_to = to_; }
    double GetTo() const { return m_to; }
    bool HasTo() const;
    ///@}

private:
    /**
     * Records a timestamp adjustment of a feature's programmatically-determined
     * location in terms of musical time; that is, beats.
     **/
    double m_to;

    /* include <attto> */
};

//----------------------------------------------------------------------------
// AttVisualOffsetVo
//----------------------------------------------------------------------------

class AttVisualOffsetVo : public Att {
public:
    AttVisualOffsetVo();
    virtual ~AttVisualOffsetVo();

    /** Reset the default values for the attribute class **/
    void ResetVisualOffsetVo();

    /** Read the values for the attribute class **/
    bool ReadVisualOffsetVo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualOffsetVo(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetVo(std::string vo_) { m_vo = vo_; }
    std::string GetVo() const { return m_vo; }
    bool HasVo() const;
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
// AttVisualOffset2Ho
//----------------------------------------------------------------------------

class AttVisualOffset2Ho : public Att {
public:
    AttVisualOffset2Ho();
    virtual ~AttVisualOffset2Ho();

    /** Reset the default values for the attribute class **/
    void ResetVisualOffset2Ho();

    /** Read the values for the attribute class **/
    bool ReadVisualOffset2Ho(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualOffset2Ho(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStartho(std::string startho_) { m_startho = startho_; }
    std::string GetStartho() const { return m_startho; }
    bool HasStartho() const;
    //
    void SetEndho(std::string endho_) { m_endho = endho_; }
    std::string GetEndho() const { return m_endho; }
    bool HasEndho() const;
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
// AttVisualOffset2To
//----------------------------------------------------------------------------

class AttVisualOffset2To : public Att {
public:
    AttVisualOffset2To();
    virtual ~AttVisualOffset2To();

    /** Reset the default values for the attribute class **/
    void ResetVisualOffset2To();

    /** Read the values for the attribute class **/
    bool ReadVisualOffset2To(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualOffset2To(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStartto(double startto_) { m_startto = startto_; }
    double GetStartto() const { return m_startto; }
    bool HasStartto() const;
    //
    void SetEndto(double endto_) { m_endto = endto_; }
    double GetEndto() const { return m_endto; }
    bool HasEndto() const;
    ///@}

private:
    /**
     * Records a timestamp adjustment of a feature's programmatically-determined start
     * point.
     **/
    double m_startto;
    /**
     * Records a timestamp adjustment of a feature's programmatically-determined end
     * point.
     **/
    double m_endto;

    /* include <attendto> */
};

//----------------------------------------------------------------------------
// AttVisualOffset2Vo
//----------------------------------------------------------------------------

class AttVisualOffset2Vo : public Att {
public:
    AttVisualOffset2Vo();
    virtual ~AttVisualOffset2Vo();

    /** Reset the default values for the attribute class **/
    void ResetVisualOffset2Vo();

    /** Read the values for the attribute class **/
    bool ReadVisualOffset2Vo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualOffset2Vo(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStartvo(std::string startvo_) { m_startvo = startvo_; }
    std::string GetStartvo() const { return m_startvo; }
    bool HasStartvo() const;
    //
    void SetEndvo(std::string endvo_) { m_endvo = endvo_; }
    std::string GetEndvo() const { return m_endvo; }
    bool HasEndvo() const;
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

class AttWhitespace : public Att {
public:
    AttWhitespace();
    virtual ~AttWhitespace();

    /** Reset the default values for the attribute class **/
    void ResetWhitespace();

    /** Read the values for the attribute class **/
    bool ReadWhitespace(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteWhitespace(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSpace(std::string space_) { m_space = space_; }
    std::string GetSpace() const { return m_space; }
    bool HasSpace() const;
    ///@}

private:
    /** --- **/
    std::string m_space;

    /* include <attspace> */
};

//----------------------------------------------------------------------------
// AttWidth
//----------------------------------------------------------------------------

class AttWidth : public Att {
public:
    AttWidth();
    virtual ~AttWidth();

    /** Reset the default values for the attribute class **/
    void ResetWidth();

    /** Read the values for the attribute class **/
    bool ReadWidth(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteWidth(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetWidth(std::string width_) { m_width = width_; }
    std::string GetWidth() const { return m_width; }
    bool HasWidth() const;
    ///@}

private:
    /** Measurement of the horizontal dimension of an entity. **/
    std::string m_width;

    /* include <attwidth> */
};

//----------------------------------------------------------------------------
// AttXy
//----------------------------------------------------------------------------

class AttXy : public Att {
public:
    AttXy();
    virtual ~AttXy();

    /** Reset the default values for the attribute class **/
    void ResetXy();

    /** Read the values for the attribute class **/
    bool ReadXy(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteXy(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetX(double x_) { m_x = x_; }
    double GetX() const { return m_x; }
    bool HasX() const;
    //
    void SetY(double y_) { m_y = y_; }
    double GetY() const { return m_y; }
    bool HasY() const;
    ///@}

private:
    /**
     * Encodes an x coordinate for a feature in an output coordinate system.
     * When it is necessary to record the placement of a feature in a facsimile image,
     * use the facs attribute.
     **/
    double m_x;
    /**
     * Encodes a y coordinate for a feature in an output coordinate system.
     * When it is necessary to record the placement of a feature in a facsimile image,
     * use the facs attribute.
     **/
    double m_y;

    /* include <atty> */
};

//----------------------------------------------------------------------------
// AttXy2
//----------------------------------------------------------------------------

class AttXy2 : public Att {
public:
    AttXy2();
    virtual ~AttXy2();

    /** Reset the default values for the attribute class **/
    void ResetXy2();

    /** Read the values for the attribute class **/
    bool ReadXy2(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteXy2(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetX2(double x2_) { m_x2 = x2_; }
    double GetX2() const { return m_x2; }
    bool HasX2() const;
    //
    void SetY2(double y2_) { m_y2 = y2_; }
    double GetY2() const { return m_y2; }
    bool HasY2() const;
    ///@}

private:
    /** Encodes the optional 2nd x coordinate. **/
    double m_x2;
    /** Encodes the optional 2nd y coordinate. **/
    double m_y2;

    /* include <atty2> */
};

} // vrv namespace

#endif // __VRV_ATTS_SHARED_H__
