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
    /** Describes the function of the bracketed event sequence. **/
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
// AttAttaccaLog
//----------------------------------------------------------------------------

class AttAttaccaLog : public Att {
public:
    AttAttaccaLog();
    virtual ~AttAttaccaLog();

    /** Reset the default values for the attribute class **/
    void ResetAttaccaLog();

    /** Read the values for the attribute class **/
    bool ReadAttaccaLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAttaccaLog(pugi::xml_node element);

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
    /** Indicates the next section or movement to be performed. **/
    std::string m_target;

    /* include <atttarget> */
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
    /** Records the number of augmentation dots required by a written dotted duration. **/
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
     * A name or label associated with a controlled vocabulary or other authoritative
     * source for this element or its content.
     **/
    std::string m_auth;
    /**
     * A web-accessible location of the controlled vocabulary or other authoritative
     * source of identification or definition for this element or its content.
     * This attribute may contain a complete URI or a partial URI which is completed by
     * the value of the codedval attribute.
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
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    data_BARRENDITION m_form;

    /* include <attform> */
};

//----------------------------------------------------------------------------
// AttBarring
//----------------------------------------------------------------------------

class AttBarring : public Att {
public:
    AttBarring();
    virtual ~AttBarring();

    /** Reset the default values for the attribute class **/
    void ResetBarring();

    /** Read the values for the attribute class **/
    bool ReadBarring(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBarring(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBarLen(double barLen_) { m_barLen = barLen_; }
    double GetBarLen() const { return m_barLen; }
    bool HasBarLen() const;
    //
    void SetBarMethod(data_BARMETHOD barMethod_) { m_barMethod = barMethod_; }
    data_BARMETHOD GetBarMethod() const { return m_barMethod; }
    bool HasBarMethod() const;
    //
    void SetBarPlace(int barPlace_) { m_barPlace = barPlace_; }
    int GetBarPlace() const { return m_barPlace; }
    bool HasBarPlace() const;
    ///@}

private:
    /**
     * States the length of barlines in virtual units.
     * The value must be greater than 0 and is typically equal to 2 times (the number
     * of staff lines - 1); e.g., a value of '8' for a 5-line staff.
     **/
    double m_barLen;
    /** Records the method of barring. **/
    data_BARMETHOD m_barMethod;
    /**
     * Denotes the staff location of bar lines, if the length is non-standard; that is,
     * not equal to 2 times (the number of staff lines - 1).
     **/
    int m_barPlace;

    /* include <attbar.place> */
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
     * A value that represents or identifies other data.
     * Often, it is a primary key in the database or a unique value in the coded list
     * identified by the
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
    //
    void SetRotate(double rotate_) { m_rotate = rotate_; }
    double GetRotate() const { return m_rotate; }
    bool HasRotate() const;
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
    /**
     * Indicates the amount by which the contents of this element have been rotated
     * clockwise or, if applicable, how the orientation of the element self should be
     * interpreted, with respect to the normal orientation of the parent surface.
     * The orientation is expressed in arc degrees.
     **/
    double m_rotate;

    /* include <attrotate> */
};

//----------------------------------------------------------------------------
// AttCue
//----------------------------------------------------------------------------

class AttCue : public Att {
public:
    AttCue();
    virtual ~AttCue();

    /** Reset the default values for the attribute class **/
    void ResetCue();

    /** Read the values for the attribute class **/
    bool ReadCue(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCue(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCue(data_BOOLEAN cue_) { m_cue = cue_; }
    data_BOOLEAN GetCue() const { return m_cue; }
    bool HasCue() const;
    ///@}

private:
    /** --- **/
    data_BOOLEAN m_cue;

    /* include <attcue> */
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
    void SetBulge(std::string bulge_) { m_bulge = bulge_; }
    std::string GetBulge() const { return m_bulge; }
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
    std::string m_bulge;
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
    void SetLwidth(data_LINEWIDTH lwidth_) { m_lwidth = lwidth_; }
    data_LINEWIDTH GetLwidth() const { return m_lwidth; }
    bool HasLwidth() const;
    /** Getter for reference (for alternate type only) */
    data_LINEWIDTH *GetLwidthAlternate() { return &m_lwidth; }
    ///@}

private:
    /** Describes the line style of a curve. **/
    data_LINEFORM m_lform;
    /** Width of a curved line. **/
    data_LINEWIDTH m_lwidth;

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
    /** Indicates the next section or movement to be performed. **/
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
// AttDistances
//----------------------------------------------------------------------------

class AttDistances : public Att {
public:
    AttDistances();
    virtual ~AttDistances();

    /** Reset the default values for the attribute class **/
    void ResetDistances();

    /** Read the values for the attribute class **/
    bool ReadDistances(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDistances(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDynamDist(data_MEASUREMENTREL dynamDist_) { m_dynamDist = dynamDist_; }
    data_MEASUREMENTREL GetDynamDist() const { return m_dynamDist; }
    bool HasDynamDist() const;
    //
    void SetHarmDist(data_MEASUREMENTREL harmDist_) { m_harmDist = harmDist_; }
    data_MEASUREMENTREL GetHarmDist() const { return m_harmDist; }
    bool HasHarmDist() const;
    //
    void SetTextDist(data_MEASUREMENTREL textDist_) { m_textDist = textDist_; }
    data_MEASUREMENTREL GetTextDist() const { return m_textDist; }
    bool HasTextDist() const;
    ///@}

private:
    /** Records the default distance from the staff for dynamic marks. **/
    data_MEASUREMENTREL m_dynamDist;
    /**
     * Records the default distance from the staff of harmonic indications, such as
     * guitar chord grids or functional labels.
     **/
    data_MEASUREMENTREL m_harmDist;
    /** Determines how far from the staff to render text elements. **/
    data_MEASUREMENTREL m_textDist;

    /* include <atttext.dist> */
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
    /** Indicates to what degree the harmonic label is supported by the notation. **/
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
    /** Records a number or count accompanying a notational feature. **/
    int m_num;
    /**
     * Along with num, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_numbase;

    /* include <attnumbase> */
};

//----------------------------------------------------------------------------
// AttEnclosingChars
//----------------------------------------------------------------------------

class AttEnclosingChars : public Att {
public:
    AttEnclosingChars();
    virtual ~AttEnclosingChars();

    /** Reset the default values for the attribute class **/
    void ResetEnclosingChars();

    /** Read the values for the attribute class **/
    bool ReadEnclosingChars(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteEnclosingChars(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetEnclose(data_ENCLOSURE enclose_) { m_enclose = enclose_; }
    data_ENCLOSURE GetEnclose() const { return m_enclose; }
    bool HasEnclose() const;
    ///@}

private:
    /**
     * Records the characters often used to mark accidentals, articulations, and
     * sometimes notes as having a cautionary or editorial function.
     * For an example of cautionary accidentals enclosed in parentheses, see Read, p.
     * 131, ex. 9-14.
     **/
    data_ENCLOSURE m_enclose;

    /* include <attenclose> */
};

//----------------------------------------------------------------------------
// AttEndings
//----------------------------------------------------------------------------

class AttEndings : public Att {
public:
    AttEndings();
    virtual ~AttEndings();

    /** Reset the default values for the attribute class **/
    void ResetEndings();

    /** Read the values for the attribute class **/
    bool ReadEndings(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteEndings(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetEndingRend(endings_ENDINGREND endingRend_) { m_endingRend = endingRend_; }
    endings_ENDINGREND GetEndingRend() const { return m_endingRend; }
    bool HasEndingRend() const;
    ///@}

private:
    /** Describes where ending marks should be displayed. **/
    endings_ENDINGREND m_endingRend;

    /* include <attending.rend> */
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
     **/
    std::string m_evidence;

    /* include <attevidence> */
};

//----------------------------------------------------------------------------
// AttExtender
//----------------------------------------------------------------------------

class AttExtender : public Att {
public:
    AttExtender();
    virtual ~AttExtender();

    /** Reset the default values for the attribute class **/
    void ResetExtender();

    /** Read the values for the attribute class **/
    bool ReadExtender(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteExtender(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetExtender(data_BOOLEAN extender_) { m_extender = extender_; }
    data_BOOLEAN GetExtender() const { return m_extender; }
    bool HasExtender() const;
    ///@}

private:
    /** Indicates the presence of an extension symbol, typically a line. **/
    data_BOOLEAN m_extender;

    /* include <attextender> */
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
// AttFermataPresent
//----------------------------------------------------------------------------

class AttFermataPresent : public Att {
public:
    AttFermataPresent();
    virtual ~AttFermataPresent();

    /** Reset the default values for the attribute class **/
    void ResetFermataPresent();

    /** Read the values for the attribute class **/
    bool ReadFermataPresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteFermataPresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFermata(data_STAFFREL_basic fermata_) { m_fermata = fermata_; }
    data_STAFFREL_basic GetFermata() const { return m_fermata; }
    bool HasFermata() const;
    ///@}

private:
    /**
     * Indicates the attachment of a fermata to this element.
     * If visual information about the fermata needs to be recorded, then a
     **/
    data_STAFFREL_basic m_fermata;

    /* include <attfermata> */
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
     * Holds the number of initial characters (such as those constituting an article or
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
    void SetHeight(data_MEASUREMENTABS height_) { m_height = height_; }
    data_MEASUREMENTABS GetHeight() const { return m_height; }
    bool HasHeight() const;
    ///@}

private:
    /** Measurement of the vertical dimension of an entity. **/
    data_MEASUREMENTABS m_height;

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
// AttJoined
//----------------------------------------------------------------------------

class AttJoined : public Att {
public:
    AttJoined();
    virtual ~AttJoined();

    /** Reset the default values for the attribute class **/
    void ResetJoined();

    /** Read the values for the attribute class **/
    bool ReadJoined(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteJoined(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetJoin(std::string join_) { m_join = join_; }
    std::string GetJoin() const { return m_join; }
    bool HasJoin() const;
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
    void SetSig(data_KEYSIGNATURE sig_) { m_sig = sig_; }
    data_KEYSIGNATURE GetSig() const { return m_sig; }
    bool HasSig() const;
    ///@}

private:
    /** Written key signature. **/
    data_KEYSIGNATURE m_sig;

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
    void SetKeySig(data_KEYSIGNATURE keySig_) { m_keySig = keySig_; }
    data_KEYSIGNATURE GetKeySig() const { return m_keySig; }
    bool HasKeySig() const;
    ///@}

private:
    /** Written key signature. **/
    data_KEYSIGNATURE m_keySig;

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
     * Captures text to be used to generate a label for the element to which it's
     * attached, a "tool tip" or prefatory text, for example.
     * Should not be used to record document content.
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
// AttLineRend
//----------------------------------------------------------------------------

class AttLineRend : public Att {
public:
    AttLineRend();
    virtual ~AttLineRend();

    /** Reset the default values for the attribute class **/
    void ResetLineRend();

    /** Read the values for the attribute class **/
    bool ReadLineRend(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLineRend(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLendsym(data_LINESTARTENDSYMBOL lendsym_) { m_lendsym = lendsym_; }
    data_LINESTARTENDSYMBOL GetLendsym() const { return m_lendsym; }
    bool HasLendsym() const;
    //
    void SetLendsymSize(int lendsymSize_) { m_lendsymSize = lendsymSize_; }
    int GetLendsymSize() const { return m_lendsymSize; }
    bool HasLendsymSize() const;
    //
    void SetLstartsym(data_LINESTARTENDSYMBOL lstartsym_) { m_lstartsym = lstartsym_; }
    data_LINESTARTENDSYMBOL GetLstartsym() const { return m_lstartsym; }
    bool HasLstartsym() const;
    //
    void SetLstartsymSize(int lstartsymSize_) { m_lstartsymSize = lstartsymSize_; }
    int GetLstartsymSize() const { return m_lstartsymSize; }
    bool HasLstartsymSize() const;
    ///@}

private:
    /** Symbol rendered at end of line. **/
    data_LINESTARTENDSYMBOL m_lendsym;
    /** Holds the relative size of the line-end symbol. **/
    int m_lendsymSize;
    /** Symbol rendered at start of line. **/
    data_LINESTARTENDSYMBOL m_lstartsym;
    /** Holds the relative size of the line-start symbol. **/
    int m_lstartsymSize;

    /* include <attlstartsym.size> */
};

//----------------------------------------------------------------------------
// AttLineRendBase
//----------------------------------------------------------------------------

class AttLineRendBase : public Att {
public:
    AttLineRendBase();
    virtual ~AttLineRendBase();

    /** Reset the default values for the attribute class **/
    void ResetLineRendBase();

    /** Read the values for the attribute class **/
    bool ReadLineRendBase(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLineRendBase(pugi::xml_node element);

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
    void SetLsegs(int lsegs_) { m_lsegs = lsegs_; }
    int GetLsegs() const { return m_lsegs; }
    bool HasLsegs() const;
    //
    void SetLwidth(data_LINEWIDTH lwidth_) { m_lwidth = lwidth_; }
    data_LINEWIDTH GetLwidth() const { return m_lwidth; }
    bool HasLwidth() const;
    /** Getter for reference (for alternate type only) */
    data_LINEWIDTH *GetLwidthAlternate() { return &m_lwidth; }
    ///@}

private:
    /** Describes the line style of a curve. **/
    data_LINEFORM m_lform;
    /**
     * Describes the number of segments into which a dashed or dotted line may be
     * divided, or the number of "peaks" of a wavy line; a pair of space-separated
     * values (minimum and maximum, respectively) provides a range between which a
     * rendering system-supplied value may fall, while a single value indicates a fixed
     * amount of space; that is, the minimum and maximum values are equal.
     **/
    int m_lsegs;
    /** Width of a curved line. **/
    data_LINEWIDTH m_lwidth;

    /* include <attlwidth> */
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
    void SetFollows(std::string follows_) { m_follows = follows_; }
    std::string GetFollows() const { return m_follows; }
    bool HasFollows() const;
    //
    void SetNext(std::string next_) { m_next = next_; }
    std::string GetNext() const { return m_next; }
    bool HasNext() const;
    //
    void SetPrecedes(std::string precedes_) { m_precedes = precedes_; }
    std::string GetPrecedes() const { return m_precedes; }
    bool HasPrecedes() const;
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
    /**
     * Points to one or more events in a user-defined collection that are known to be
     * predecessors of the current element.
     **/
    std::string m_follows;
    /** Used to point to the next event(s) in a user-defined collection. **/
    std::string m_next;
    /**
     * Points to one or more events in a user-defined collection that are known to be
     * successors of the current element.
     **/
    std::string m_precedes;
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
// AttLyricStyle
//----------------------------------------------------------------------------

class AttLyricStyle : public Att {
public:
    AttLyricStyle();
    virtual ~AttLyricStyle();

    /** Reset the default values for the attribute class **/
    void ResetLyricStyle();

    /** Read the values for the attribute class **/
    bool ReadLyricStyle(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLyricStyle(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLyricAlign(data_MEASUREMENTREL lyricAlign_) { m_lyricAlign = lyricAlign_; }
    data_MEASUREMENTREL GetLyricAlign() const { return m_lyricAlign; }
    bool HasLyricAlign() const;
    //
    void SetLyricFam(std::string lyricFam_) { m_lyricFam = lyricFam_; }
    std::string GetLyricFam() const { return m_lyricFam; }
    bool HasLyricFam() const;
    //
    void SetLyricName(std::string lyricName_) { m_lyricName = lyricName_; }
    std::string GetLyricName() const { return m_lyricName; }
    bool HasLyricName() const;
    //
    void SetLyricSize(data_FONTSIZE lyricSize_) { m_lyricSize = lyricSize_; }
    data_FONTSIZE GetLyricSize() const { return m_lyricSize; }
    bool HasLyricSize() const;
    /** Getter for reference (for alternate type only) */
    data_FONTSIZE *GetLyricSizeAlternate() { return &m_lyricSize; }
    //
    void SetLyricStyle(data_FONTSTYLE lyricStyle_) { m_lyricStyle = lyricStyle_; }
    data_FONTSTYLE GetLyricStyle() const { return m_lyricStyle; }
    bool HasLyricStyle() const;
    //
    void SetLyricWeight(data_FONTWEIGHT lyricWeight_) { m_lyricWeight = lyricWeight_; }
    data_FONTWEIGHT GetLyricWeight() const { return m_lyricWeight; }
    bool HasLyricWeight() const;
    ///@}

private:
    /** Describes the alignment of lyric syllables associated with a note or chord. **/
    data_MEASUREMENTREL m_lyricAlign;
    /** Sets the font family default value for lyrics. **/
    std::string m_lyricFam;
    /** Sets the font name default value for lyrics. **/
    std::string m_lyricName;
    /** Sets the default font size value for lyrics. **/
    data_FONTSIZE m_lyricSize;
    /** Sets the default font style value for lyrics. **/
    data_FONTSTYLE m_lyricStyle;
    /** Sets the default font weight value for lyrics. **/
    data_FONTWEIGHT m_lyricWeight;

    /* include <attlyric.weight> */
};

//----------------------------------------------------------------------------
// AttMeasureNumbers
//----------------------------------------------------------------------------

class AttMeasureNumbers : public Att {
public:
    AttMeasureNumbers();
    virtual ~AttMeasureNumbers();

    /** Reset the default values for the attribute class **/
    void ResetMeasureNumbers();

    /** Read the values for the attribute class **/
    bool ReadMeasureNumbers(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeasureNumbers(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMnumVisible(data_BOOLEAN mnumVisible_) { m_mnumVisible = mnumVisible_; }
    data_BOOLEAN GetMnumVisible() const { return m_mnumVisible; }
    bool HasMnumVisible() const;
    ///@}

private:
    /** Indicates whether measure numbers should be displayed. **/
    data_BOOLEAN m_mnumVisible;

    /* include <attmnum.visible> */
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
    /** Indicates the number of performers. **/
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
    void SetMm(int mm_) { m_mm = mm_; }
    int GetMm() const { return m_mm; }
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
     * (Maelzel's Metronome). Do not confuse this attribute with midi.bpm or midi.mspb.
     * In MIDI, a beat is always defined as a quarter note, *not the numerator of the
     * time signature or the metronomic indication*.
     **/
    int m_mm;
    /** Captures the metronomic unit. **/
    data_DURATION m_mmUnit;
    /** Records the number of augmentation dots required by a dotted metronome unit. **/
    int m_mmDots;

    /* include <attmm.dots> */
};

//----------------------------------------------------------------------------
// AttMultinumMeasures
//----------------------------------------------------------------------------

class AttMultinumMeasures : public Att {
public:
    AttMultinumMeasures();
    virtual ~AttMultinumMeasures();

    /** Reset the default values for the attribute class **/
    void ResetMultinumMeasures();

    /** Read the values for the attribute class **/
    bool ReadMultinumMeasures(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMultinumMeasures(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMultiNumber(data_BOOLEAN multiNumber_) { m_multiNumber = multiNumber_; }
    data_BOOLEAN GetMultiNumber() const { return m_multiNumber; }
    bool HasMultiNumber() const;
    ///@}

private:
    /**
     * Indicates whether programmatically calculated counts of multiple measures of
     * rest (mRest) and whole measure repeats (mRpt) in parts should be rendered.
     **/
    data_BOOLEAN m_multiNumber;

    /* include <attmulti.number> */
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
    void SetN(std::string n_) { m_n = n_; }
    std::string GetN() const { return m_n; }
    bool HasN() const;
    ///@}

private:
    /**
     * Provides a numeric designation that indicates an element's position in a
     * sequence of similar elements.
     * Its value must be a non-negative integer.
     **/
    std::string m_n;

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
// AttNotationStyle
//----------------------------------------------------------------------------

class AttNotationStyle : public Att {
public:
    AttNotationStyle();
    virtual ~AttNotationStyle();

    /** Reset the default values for the attribute class **/
    void ResetNotationStyle();

    /** Read the values for the attribute class **/
    bool ReadNotationStyle(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNotationStyle(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMusicName(std::string musicName_) { m_musicName = musicName_; }
    std::string GetMusicName() const { return m_musicName; }
    bool HasMusicName() const;
    //
    void SetMusicSize(data_FONTSIZE musicSize_) { m_musicSize = musicSize_; }
    data_FONTSIZE GetMusicSize() const { return m_musicSize; }
    bool HasMusicSize() const;
    /** Getter for reference (for alternate type only) */
    data_FONTSIZE *GetMusicSizeAlternate() { return &m_musicSize; }
    ///@}

private:
    /** Sets the default music font name. **/
    std::string m_musicName;
    /** Sets the default music font size. **/
    data_FONTSIZE m_musicSize;

    /* include <attmusic.size> */
};

//----------------------------------------------------------------------------
// AttNoteHeads
//----------------------------------------------------------------------------

class AttNoteHeads : public Att {
public:
    AttNoteHeads();
    virtual ~AttNoteHeads();

    /** Reset the default values for the attribute class **/
    void ResetNoteHeads();

    /** Read the values for the attribute class **/
    bool ReadNoteHeads(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNoteHeads(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetHeadAltsym(std::string headAltsym_) { m_headAltsym = headAltsym_; }
    std::string GetHeadAltsym() const { return m_headAltsym; }
    bool HasHeadAltsym() const;
    //
    void SetHeadAuth(std::string headAuth_) { m_headAuth = headAuth_; }
    std::string GetHeadAuth() const { return m_headAuth; }
    bool HasHeadAuth() const;
    //
    void SetHeadColor(std::string headColor_) { m_headColor = headColor_; }
    std::string GetHeadColor() const { return m_headColor; }
    bool HasHeadColor() const;
    //
    void SetHeadFill(data_FILL headFill_) { m_headFill = headFill_; }
    data_FILL GetHeadFill() const { return m_headFill; }
    bool HasHeadFill() const;
    //
    void SetHeadFillcolor(std::string headFillcolor_) { m_headFillcolor = headFillcolor_; }
    std::string GetHeadFillcolor() const { return m_headFillcolor; }
    bool HasHeadFillcolor() const;
    //
    void SetHeadMod(data_NOTEHEADMODIFIER headMod_) { m_headMod = headMod_; }
    data_NOTEHEADMODIFIER GetHeadMod() const { return m_headMod; }
    bool HasHeadMod() const;
    //
    void SetHeadRotation(data_ROTATION headRotation_) { m_headRotation = headRotation_; }
    data_ROTATION GetHeadRotation() const { return m_headRotation; }
    bool HasHeadRotation() const;
    //
    void SetHeadShape(data_HEADSHAPE headShape_) { m_headShape = headShape_; }
    data_HEADSHAPE GetHeadShape() const { return m_headShape; }
    bool HasHeadShape() const;
    //
    void SetHeadVisible(data_BOOLEAN headVisible_) { m_headVisible = headVisible_; }
    data_BOOLEAN GetHeadVisible() const { return m_headVisible; }
    bool HasHeadVisible() const;
    ///@}

private:
    /**
     * Provides a way of pointing to a user-defined symbol.
     * It must contain a reference to an ID of a
     **/
    std::string m_headAltsym;
    /**
     * A name or label associated with the controlled vocabulary from which a numerical
     * value of
     **/
    std::string m_headAuth;
    /** Captures the overall color of a notehead. **/
    std::string m_headColor;
    /** Describes how/if the notehead is filled. **/
    data_FILL m_headFill;
    /** Captures the fill color of a notehead if different from the overall note color. **/
    std::string m_headFillcolor;
    /** Records any additional symbols applied to the notehead. **/
    data_NOTEHEADMODIFIER m_headMod;
    /**
     * Describes rotation applied to the basic notehead shape.
     * A positive value rotates the notehead in a counter-clockwise fashion, while
     * negative values produce clockwise rotation.
     **/
    data_ROTATION m_headRotation;
    /** Used to override the head shape normally used for the given duration. **/
    data_HEADSHAPE m_headShape;
    /**
     * Indicates if a feature should be rendered when the notation is presented
     * graphically or sounded when it is presented in an aural form.
     **/
    data_BOOLEAN m_headVisible;

    /* include <atthead.visible> */
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
    void SetOct(data_OCTAVE oct_) { m_oct = oct_; }
    data_OCTAVE GetOct() const { return m_oct; }
    bool HasOct() const;
    ///@}

private:
    /** Captures written octave information. **/
    data_OCTAVE m_oct;

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
    void SetOctDefault(data_OCTAVE octDefault_) { m_octDefault = octDefault_; }
    data_OCTAVE GetOctDefault() const { return m_octDefault; }
    bool HasOctDefault() const;
    ///@}

private:
    /**
     * Contains a default octave specification for use when the first note, rest,
     * chord, etc.
     * in a measure does not have an octave value specified.
     **/
    data_OCTAVE m_octDefault;

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
// AttOneLineStaff
//----------------------------------------------------------------------------

class AttOneLineStaff : public Att {
public:
    AttOneLineStaff();
    virtual ~AttOneLineStaff();

    /** Reset the default values for the attribute class **/
    void ResetOneLineStaff();

    /** Read the values for the attribute class **/
    bool ReadOneLineStaff(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOneLineStaff(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOntheline(data_BOOLEAN ontheline_) { m_ontheline = ontheline_; }
    data_BOOLEAN GetOntheline() const { return m_ontheline; }
    bool HasOntheline() const;
    ///@}

private:
    /**
     * Determines the placement of notes on a 1-line staff.
     * A value of 'true' places all notes on the line, while a value of 'false' places
     * stems-up notes above the line and stems-down notes below the line.
     **/
    data_BOOLEAN m_ontheline;

    /* include <attontheline> */
};

//----------------------------------------------------------------------------
// AttOptimization
//----------------------------------------------------------------------------

class AttOptimization : public Att {
public:
    AttOptimization();
    virtual ~AttOptimization();

    /** Reset the default values for the attribute class **/
    void ResetOptimization();

    /** Read the values for the attribute class **/
    bool ReadOptimization(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOptimization(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOptimize(data_BOOLEAN optimize_) { m_optimize = optimize_; }
    data_BOOLEAN GetOptimize() const { return m_optimize; }
    bool HasOptimize() const;
    ///@}

private:
    /**
     * Indicates whether staves without notes, rests, etc.
     * should be displayed. When the value is 'true', empty staves are displayed.
     **/
    data_BOOLEAN m_optimize;

    /* include <attoptimize> */
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
// AttPages
//----------------------------------------------------------------------------

class AttPages : public Att {
public:
    AttPages();
    virtual ~AttPages();

    /** Reset the default values for the attribute class **/
    void ResetPages();

    /** Read the values for the attribute class **/
    bool ReadPages(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePages(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPageHeight(data_MEASUREMENTABS pageHeight_) { m_pageHeight = pageHeight_; }
    data_MEASUREMENTABS GetPageHeight() const { return m_pageHeight; }
    bool HasPageHeight() const;
    //
    void SetPageWidth(data_MEASUREMENTABS pageWidth_) { m_pageWidth = pageWidth_; }
    data_MEASUREMENTABS GetPageWidth() const { return m_pageWidth; }
    bool HasPageWidth() const;
    //
    void SetPageTopmar(data_MEASUREMENTABS pageTopmar_) { m_pageTopmar = pageTopmar_; }
    data_MEASUREMENTABS GetPageTopmar() const { return m_pageTopmar; }
    bool HasPageTopmar() const;
    //
    void SetPageBotmar(data_MEASUREMENTABS pageBotmar_) { m_pageBotmar = pageBotmar_; }
    data_MEASUREMENTABS GetPageBotmar() const { return m_pageBotmar; }
    bool HasPageBotmar() const;
    //
    void SetPageLeftmar(data_MEASUREMENTABS pageLeftmar_) { m_pageLeftmar = pageLeftmar_; }
    data_MEASUREMENTABS GetPageLeftmar() const { return m_pageLeftmar; }
    bool HasPageLeftmar() const;
    //
    void SetPageRightmar(data_MEASUREMENTABS pageRightmar_) { m_pageRightmar = pageRightmar_; }
    data_MEASUREMENTABS GetPageRightmar() const { return m_pageRightmar; }
    bool HasPageRightmar() const;
    //
    void SetPagePanels(std::string pagePanels_) { m_pagePanels = pagePanels_; }
    std::string GetPagePanels() const { return m_pagePanels; }
    bool HasPagePanels() const;
    //
    void SetPageScale(std::string pageScale_) { m_pageScale = pageScale_; }
    std::string GetPageScale() const { return m_pageScale; }
    bool HasPageScale() const;
    ///@}

private:
    /**
     * Specifies the height of the page; may be expressed in real-world units or staff
     * steps.
     **/
    data_MEASUREMENTABS m_pageHeight;
    /**
     * Describes the width of the page; may be expressed in real-world units or staff
     * steps.
     **/
    data_MEASUREMENTABS m_pageWidth;
    /** Indicates the amount of whitespace at the top of a page. **/
    data_MEASUREMENTABS m_pageTopmar;
    /** Indicates the amount of whitespace at the bottom of a page. **/
    data_MEASUREMENTABS m_pageBotmar;
    /** Indicates the amount of whitespace at the left side of a page. **/
    data_MEASUREMENTABS m_pageLeftmar;
    /** Indicates the amount of whitespace at the right side of a page. **/
    data_MEASUREMENTABS m_pageRightmar;
    /** Indicates the number of logical pages to be rendered on a single physical page. **/
    std::string m_pagePanels;
    /** Indicates how the page should be scaled when rendered. **/
    std::string m_pageScale;

    /* include <attpage.scale> */
};

//----------------------------------------------------------------------------
// AttPartIdent
//----------------------------------------------------------------------------

class AttPartIdent : public Att {
public:
    AttPartIdent();
    virtual ~AttPartIdent();

    /** Reset the default values for the attribute class **/
    void ResetPartIdent();

    /** Read the values for the attribute class **/
    bool ReadPartIdent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePartIdent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPart(std::string part_) { m_part = part_; }
    std::string GetPart() const { return m_part; }
    bool HasPart() const;
    //
    void SetPartstaff(std::string partstaff_) { m_partstaff = partstaff_; }
    std::string GetPartstaff() const { return m_partstaff; }
    bool HasPartstaff() const;
    ///@}

private:
    /**
     * Indicates the part in which the current feature should appear.
     * Use '%all' when the feature should occur in every part.
     **/
    std::string m_part;
    /**
     * Signifies the part staff on which a notated feature occurs.
     * Use '%all' when the feature should occur on every staff.
     **/
    std::string m_partstaff;

    /* include <attpartstaff> */
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
    void SetPlace(data_STAFFREL place_) { m_place = place_; }
    data_STAFFREL GetPlace() const { return m_place; }
    bool HasPlace() const;
    ///@}

private:
    /** Records the placement of the beam relative to the events it affects. **/
    data_STAFFREL m_place;

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
    void SetPlist(xsdAnyURI_List plist_) { m_plist = plist_; }
    xsdAnyURI_List GetPlist() const { return m_plist; }
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
    xsdAnyURI_List m_plist;

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
    /** Indicates the next section or movement to be performed. **/
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
     * Can only be interpreted in combination with the unit attribute.
     **/
    double m_quantity;

    /* include <attquantity> */
};

//----------------------------------------------------------------------------
// AttRanging
//----------------------------------------------------------------------------

class AttRanging : public Att {
public:
    AttRanging();
    virtual ~AttRanging();

    /** Reset the default values for the attribute class **/
    void ResetRanging();

    /** Read the values for the attribute class **/
    bool ReadRanging(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteRanging(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAtleast(double atleast_) { m_atleast = atleast_; }
    double GetAtleast() const { return m_atleast; }
    bool HasAtleast() const;
    //
    void SetAtmost(double atmost_) { m_atmost = atmost_; }
    double GetAtmost() const { return m_atmost; }
    bool HasAtmost() const;
    //
    void SetMin(double min_) { m_min = min_; }
    double GetMin() const { return m_min; }
    bool HasMin() const;
    //
    void SetMax(double max_) { m_max = max_; }
    double GetMax() const { return m_max; }
    bool HasMax() const;
    //
    void SetConfidence(double confidence_) { m_confidence = confidence_; }
    double GetConfidence() const { return m_confidence; }
    bool HasConfidence() const;
    ///@}

private:
    /** Gives a minimum estimated value for an approximate measurement. **/
    double m_atleast;
    /** Gives a maximum estimated value for an approximate measurement. **/
    double m_atmost;
    /**
     * Where the measurement summarizes more than one observation or a range of values,
     * supplies the minimum value observed.
     **/
    double m_min;
    /**
     * Where the measurement summarizes more than one observation or a range of values,
     * supplies the maximum value observed.
     **/
    double m_max;
    /**
     * Specifies the degree of statistical confidence (between zero and one) that a
     * value falls within the range specified by min and max, or the proportion of
     * observed values that fall within that range.
     **/
    double m_confidence;

    /* include <attconfidence> */
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
     * Indicates the agent(s) responsible for some aspect of the text's transcription,
     * editing, or encoding.
     * Its value must point to one or more identifiers declared in the document header.
     **/
    std::string m_resp;

    /* include <attresp> */
};

//----------------------------------------------------------------------------
// AttRestdurationLogical
//----------------------------------------------------------------------------

class AttRestdurationLogical : public Att {
public:
    AttRestdurationLogical();
    virtual ~AttRestdurationLogical();

    /** Reset the default values for the attribute class **/
    void ResetRestdurationLogical();

    /** Read the values for the attribute class **/
    bool ReadRestdurationLogical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteRestdurationLogical(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDur(data_DURATIONRESTS dur_) { m_dur = dur_; }
    data_DURATIONRESTS GetDur() const { return m_dur; }
    bool HasDur() const;
    ///@}

private:
    /**
     * When a duration cannot be represented as a single power-of-two value, multiple
     * space-separated values that add up to the total duration may be used.
     **/
    data_DURATIONRESTS m_dur;

    /* include <attdur> */
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
    /** Indicates presence of slash through the beam. **/
    char m_slash;

    /* include <attslash> */
};

//----------------------------------------------------------------------------
// AttSlurPresent
//----------------------------------------------------------------------------

class AttSlurPresent : public Att {
public:
    AttSlurPresent();
    virtual ~AttSlurPresent();

    /** Reset the default values for the attribute class **/
    void ResetSlurPresent();

    /** Read the values for the attribute class **/
    bool ReadSlurPresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSlurPresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSlur(std::string slur_) { m_slur = slur_; }
    std::string GetSlur() const { return m_slur; }
    bool HasSlur() const;
    ///@}

private:
    /**
     * Indicates that this element participates in a slur.
     * If visual information about the slur needs to be recorded, then a
     **/
    std::string m_slur;

    /* include <attslur> */
};

//----------------------------------------------------------------------------
// AttSource
//----------------------------------------------------------------------------

class AttSource : public Att {
public:
    AttSource();
    virtual ~AttSource();

    /** Reset the default values for the attribute class **/
    void ResetSource();

    /** Read the values for the attribute class **/
    bool ReadSource(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSource(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSource(std::string source_) { m_source = source_; }
    std::string GetSource() const { return m_source; }
    bool HasSource() const;
    ///@}

private:
    /**
     * Contains a list of one or more pointers indicating the sources which attest to a
     * given reading.
     * Each value should correspond to the ID of a
     **/
    std::string m_source;

    /* include <attsource> */
};

//----------------------------------------------------------------------------
// AttSpacing
//----------------------------------------------------------------------------

class AttSpacing : public Att {
public:
    AttSpacing();
    virtual ~AttSpacing();

    /** Reset the default values for the attribute class **/
    void ResetSpacing();

    /** Read the values for the attribute class **/
    bool ReadSpacing(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSpacing(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSpacingPackexp(double spacingPackexp_) { m_spacingPackexp = spacingPackexp_; }
    double GetSpacingPackexp() const { return m_spacingPackexp; }
    bool HasSpacingPackexp() const;
    //
    void SetSpacingPackfact(double spacingPackfact_) { m_spacingPackfact = spacingPackfact_; }
    double GetSpacingPackfact() const { return m_spacingPackfact; }
    bool HasSpacingPackfact() const;
    //
    void SetSpacingStaff(data_MEASUREMENTREL spacingStaff_) { m_spacingStaff = spacingStaff_; }
    data_MEASUREMENTREL GetSpacingStaff() const { return m_spacingStaff; }
    bool HasSpacingStaff() const;
    //
    void SetSpacingSystem(data_MEASUREMENTREL spacingSystem_) { m_spacingSystem = spacingSystem_; }
    data_MEASUREMENTREL GetSpacingSystem() const { return m_spacingSystem; }
    bool HasSpacingSystem() const;
    ///@}

private:
    /** Describes a note's spacing relative to its time value. **/
    double m_spacingPackexp;
    /** Describes the note spacing of output. **/
    double m_spacingPackfact;
    /**
     * Specifies the minimum amount of space between adjacent staves in the same
     * system; measured from the bottom line of the staff above to the top line of the
     * staff below.
     **/
    data_MEASUREMENTREL m_spacingStaff;
    /**
     * Describes the space between adjacent systems; a pair of space-separated values
     * (minimum and maximum, respectively) provides a range between which a rendering
     * system-supplied value may fall, while a single value indicates a fixed amount of
     * space; that is, the minimum and maximum values are equal.
     **/
    data_MEASUREMENTREL m_spacingSystem;

    /* include <attspacing.system> */
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
    void SetStaff(xsdPositiveInteger_List staff_) { m_staff = staff_; }
    xsdPositiveInteger_List GetStaff() const { return m_staff; }
    bool HasStaff() const;
    ///@}

private:
    /**
     * Signifies the staff on which a notated event occurs or to which a control event
     * applies.
     * Mandatory when applicable.
     **/
    xsdPositiveInteger_List m_staff;

    /* include <attstaff> */
};

//----------------------------------------------------------------------------
// AttStaffItems
//----------------------------------------------------------------------------

class AttStaffItems : public Att {
public:
    AttStaffItems();
    virtual ~AttStaffItems();

    /** Reset the default values for the attribute class **/
    void ResetStaffItems();

    /** Read the values for the attribute class **/
    bool ReadStaffItems(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffItems(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAboveorder(data_STAFFITEM aboveorder_) { m_aboveorder = aboveorder_; }
    data_STAFFITEM GetAboveorder() const { return m_aboveorder; }
    bool HasAboveorder() const;
    //
    void SetBeloworder(data_STAFFITEM beloworder_) { m_beloworder = beloworder_; }
    data_STAFFITEM GetBeloworder() const { return m_beloworder; }
    bool HasBeloworder() const;
    //
    void SetBetweenorder(data_STAFFITEM betweenorder_) { m_betweenorder = betweenorder_; }
    data_STAFFITEM GetBetweenorder() const { return m_betweenorder; }
    bool HasBetweenorder() const;
    ///@}

private:
    /**
     * Describes vertical order of items printed above a staff, from closest to
     * farthest away from the staff.
     **/
    data_STAFFITEM m_aboveorder;
    /**
     * Describes vertical order of items printed below a staff, from closest to
     * farthest away from the staff.
     **/
    data_STAFFITEM m_beloworder;
    /** Describes vertical order of items printed between staves, from top to bottom. **/
    data_STAFFITEM m_betweenorder;

    /* include <attbetweenorder> */
};

//----------------------------------------------------------------------------
// AttStaffLoc
//----------------------------------------------------------------------------

class AttStaffLoc : public Att {
public:
    AttStaffLoc();
    virtual ~AttStaffLoc();

    /** Reset the default values for the attribute class **/
    void ResetStaffLoc();

    /** Read the values for the attribute class **/
    bool ReadStaffLoc(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffLoc(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLoc(int loc_) { m_loc = loc_; }
    int GetLoc() const { return m_loc; }
    bool HasLoc() const;
    ///@}

private:
    /** Holds the staff location of the feature. **/
    int m_loc;

    /* include <attloc> */
};

//----------------------------------------------------------------------------
// AttStaffLocPitched
//----------------------------------------------------------------------------

class AttStaffLocPitched : public Att {
public:
    AttStaffLocPitched();
    virtual ~AttStaffLocPitched();

    /** Reset the default values for the attribute class **/
    void ResetStaffLocPitched();

    /** Read the values for the attribute class **/
    bool ReadStaffLocPitched(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffLocPitched(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPloc(data_PITCHNAME ploc_) { m_ploc = ploc_; }
    data_PITCHNAME GetPloc() const { return m_ploc; }
    bool HasPloc() const;
    //
    void SetOloc(data_OCTAVE oloc_) { m_oloc = oloc_; }
    data_OCTAVE GetOloc() const { return m_oloc; }
    bool HasOloc() const;
    ///@}

private:
    /** Captures staff location in terms of written pitch name. **/
    data_PITCHNAME m_ploc;
    /** Records staff location in terms of written octave. **/
    data_OCTAVE m_oloc;

    /* include <attoloc> */
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
// AttStems
//----------------------------------------------------------------------------

class AttStems : public Att {
public:
    AttStems();
    virtual ~AttStems();

    /** Reset the default values for the attribute class **/
    void ResetStems();

    /** Read the values for the attribute class **/
    bool ReadStems(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStems(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStemDir(data_STEMDIRECTION stemDir_) { m_stemDir = stemDir_; }
    data_STEMDIRECTION GetStemDir() const { return m_stemDir; }
    bool HasStemDir() const;
    //
    void SetStemLen(double stemLen_) { m_stemLen = stemLen_; }
    double GetStemLen() const { return m_stemLen; }
    bool HasStemLen() const;
    //
    void SetStemMod(data_STEMMODIFIER stemMod_) { m_stemMod = stemMod_; }
    data_STEMMODIFIER GetStemMod() const { return m_stemMod; }
    bool HasStemMod() const;
    //
    void SetStemPos(data_STEMPOSITION stemPos_) { m_stemPos = stemPos_; }
    data_STEMPOSITION GetStemPos() const { return m_stemPos; }
    bool HasStemPos() const;
    //
    void SetStemSameas(std::string stemSameas_) { m_stemSameas = stemSameas_; }
    std::string GetStemSameas() const { return m_stemSameas; }
    bool HasStemSameas() const;
    //
    void SetStemVisible(data_BOOLEAN stemVisible_) { m_stemVisible = stemVisible_; }
    data_BOOLEAN GetStemVisible() const { return m_stemVisible; }
    bool HasStemVisible() const;
    //
    void SetStemX(double stemX_) { m_stemX = stemX_; }
    double GetStemX() const { return m_stemX; }
    bool HasStemX() const;
    //
    void SetStemY(double stemY_) { m_stemY = stemY_; }
    double GetStemY() const { return m_stemY; }
    bool HasStemY() const;
    ///@}

private:
    /** Describes the direction of a stem. **/
    data_STEMDIRECTION m_stemDir;
    /** Encodes the stem length. **/
    double m_stemLen;
    /**
     * Encodes any stem "modifiers"; that is, symbols rendered on the stem, such as
     * tremolo or Sprechstimme indicators.
     **/
    data_STEMMODIFIER m_stemMod;
    /** Records the position of the stem in relation to the note head(s). **/
    data_STEMPOSITION m_stemPos;
    /**
     * Points to a note element in a different layer whose stem is shared.
     * The linked notes should be rendered like a chord though they are part of
     * different layers.
     **/
    std::string m_stemSameas;
    /** Determines whether a stem should be displayed. **/
    data_BOOLEAN m_stemVisible;
    /** Records the output x coordinate of the stem's attachment point. **/
    double m_stemX;
    /** Records the output y coordinate of the stem's attachment point. **/
    double m_stemY;

    /* include <attstem.y> */
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
     * Connection to the previous component within the same neume; this attribute
     * should not be used for the first component of a neume.
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
// AttSystems
//----------------------------------------------------------------------------

class AttSystems : public Att {
public:
    AttSystems();
    virtual ~AttSystems();

    /** Reset the default values for the attribute class **/
    void ResetSystems();

    /** Read the values for the attribute class **/
    bool ReadSystems(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSystems(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSystemLeftline(data_BOOLEAN systemLeftline_) { m_systemLeftline = systemLeftline_; }
    data_BOOLEAN GetSystemLeftline() const { return m_systemLeftline; }
    bool HasSystemLeftline() const;
    //
    void SetSystemLeftmar(data_MEASUREMENTABS systemLeftmar_) { m_systemLeftmar = systemLeftmar_; }
    data_MEASUREMENTABS GetSystemLeftmar() const { return m_systemLeftmar; }
    bool HasSystemLeftmar() const;
    //
    void SetSystemRightmar(data_MEASUREMENTABS systemRightmar_) { m_systemRightmar = systemRightmar_; }
    data_MEASUREMENTABS GetSystemRightmar() const { return m_systemRightmar; }
    bool HasSystemRightmar() const;
    //
    void SetSystemTopmar(data_MEASUREMENTABS systemTopmar_) { m_systemTopmar = systemTopmar_; }
    data_MEASUREMENTABS GetSystemTopmar() const { return m_systemTopmar; }
    bool HasSystemTopmar() const;
    ///@}

private:
    /**
     * Indicates whether the system starts with a continuous line connecting all
     * staves, including single-staff systems.
     * Do not confuse this with the heavy vertical line used as a grouping symbol.
     **/
    data_BOOLEAN m_systemLeftline;
    /**
     * Describes the amount of whitespace at the left system margin relative to
     * page.leftmar.
     **/
    data_MEASUREMENTABS m_systemLeftmar;
    /**
     * Describes the amount of whitespace at the right system margin relative to
     * page.rightmar.
     **/
    data_MEASUREMENTABS m_systemRightmar;
    /**
     * Describes the distance from page's top edge to the first system; used for first
     * page only.
     **/
    data_MEASUREMENTABS m_systemTopmar;

    /* include <attsystem.topmar> */
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
    /** Describes the function of the bracketed event sequence. **/
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
    void SetRend(data_TEXTRENDITION rend_) { m_rend = rend_; }
    data_TEXTRENDITION GetRend() const { return m_rend; }
    bool HasRend() const;
    ///@}

private:
    /** Used to extend the values of the rend attribute. **/
    std::string m_altrend;
    /** Captures the appearance of the element's contents using MEI-defined descriptors. **/
    data_TEXTRENDITION m_rend;

    /* include <attrend> */
};

//----------------------------------------------------------------------------
// AttTextStyle
//----------------------------------------------------------------------------

class AttTextStyle : public Att {
public:
    AttTextStyle();
    virtual ~AttTextStyle();

    /** Reset the default values for the attribute class **/
    void ResetTextStyle();

    /** Read the values for the attribute class **/
    bool ReadTextStyle(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTextStyle(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTextFam(std::string textFam_) { m_textFam = textFam_; }
    std::string GetTextFam() const { return m_textFam; }
    bool HasTextFam() const;
    //
    void SetTextName(std::string textName_) { m_textName = textName_; }
    std::string GetTextName() const { return m_textName; }
    bool HasTextName() const;
    //
    void SetTextSize(data_FONTSIZE textSize_) { m_textSize = textSize_; }
    data_FONTSIZE GetTextSize() const { return m_textSize; }
    bool HasTextSize() const;
    /** Getter for reference (for alternate type only) */
    data_FONTSIZE *GetTextSizeAlternate() { return &m_textSize; }
    //
    void SetTextStyle(data_FONTSTYLE textStyle_) { m_textStyle = textStyle_; }
    data_FONTSTYLE GetTextStyle() const { return m_textStyle; }
    bool HasTextStyle() const;
    //
    void SetTextWeight(data_FONTWEIGHT textWeight_) { m_textWeight = textWeight_; }
    data_FONTWEIGHT GetTextWeight() const { return m_textWeight; }
    bool HasTextWeight() const;
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
    data_FONTSIZE m_textSize;
    /**
     * Provides a default value for the font style of text (other than lyrics) when
     * this information is not provided on the individual elements.
     **/
    data_FONTSTYLE m_textStyle;
    /**
     * Provides a default value for the font weight for text (other than lyrics) when
     * this information is not provided on the individual elements.
     **/
    data_FONTWEIGHT m_textWeight;

    /* include <atttext.weight> */
};

//----------------------------------------------------------------------------
// AttTiePresent
//----------------------------------------------------------------------------

class AttTiePresent : public Att {
public:
    AttTiePresent();
    virtual ~AttTiePresent();

    /** Reset the default values for the attribute class **/
    void ResetTiePresent();

    /** Read the values for the attribute class **/
    bool ReadTiePresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTiePresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTie(data_TIE tie_) { m_tie = tie_; }
    data_TIE GetTie() const { return m_tie; }
    bool HasTie() const;
    ///@}

private:
    /**
     * Indicates that this element participates in a tie.
     * If visual information about the tie needs to be recorded, then a
     **/
    data_TIE m_tie;

    /* include <atttie> */
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
     * Encodes the onset time in terms of musical time, i.e., beats[.fractional beat
     * part], as expressed in the written time signature.
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
     * Encodes the ending point of an event, i.e., a count of measures plus a beat
     * location in the ending measure.
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
// AttTupletPresent
//----------------------------------------------------------------------------

class AttTupletPresent : public Att {
public:
    AttTupletPresent();
    virtual ~AttTupletPresent();

    /** Reset the default values for the attribute class **/
    void ResetTupletPresent();

    /** Read the values for the attribute class **/
    bool ReadTupletPresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTupletPresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTuplet(std::string tuplet_) { m_tuplet = tuplet_; }
    std::string GetTuplet() const { return m_tuplet; }
    bool HasTuplet() const;
    ///@}

private:
    /**
     * Indicates that this feature participates in a tuplet.
     * If visual information about the tuplet needs to be recorded, then a
     **/
    std::string m_tuplet;

    /* include <atttuplet> */
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
     * Provides a description of the relationship between the current and the target
     * categories.
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
    void SetFontsize(data_FONTSIZE fontsize_) { m_fontsize = fontsize_; }
    data_FONTSIZE GetFontsize() const { return m_fontsize; }
    bool HasFontsize() const;
    /** Getter for reference (for alternate type only) */
    data_FONTSIZE *GetFontsizeAlternate() { return &m_fontsize; }
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
    data_FONTSIZE m_fontsize;
    /** Records the style of a font, i.e, italic, oblique, or normal. **/
    data_FONTSTYLE m_fontstyle;
    /** Used to indicate bold type. **/
    data_FONTWEIGHT m_fontweight;

    /* include <attfontweight> */
};

//----------------------------------------------------------------------------
// AttVerticalAlign
//----------------------------------------------------------------------------

class AttVerticalAlign : public Att {
public:
    AttVerticalAlign();
    virtual ~AttVerticalAlign();

    /** Reset the default values for the attribute class **/
    void ResetVerticalAlign();

    /** Read the values for the attribute class **/
    bool ReadVerticalAlign(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVerticalAlign(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetValign(data_VERTICALALIGNMENT valign_) { m_valign = valign_; }
    data_VERTICALALIGNMENT GetValign() const { return m_valign; }
    bool HasValign() const;
    ///@}

private:
    /** Records vertical alignment. **/
    data_VERTICALALIGNMENT m_valign;

    /* include <attvalign> */
};

//----------------------------------------------------------------------------
// AttVerticalGroup
//----------------------------------------------------------------------------

class AttVerticalGroup : public Att {
public:
    AttVerticalGroup();
    virtual ~AttVerticalGroup();

    /** Reset the default values for the attribute class **/
    void ResetVerticalGroup();

    /** Read the values for the attribute class **/
    bool ReadVerticalGroup(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVerticalGroup(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetVgrp(int vgrp_) { m_vgrp = vgrp_; }
    int GetVgrp() const { return m_vgrp; }
    bool HasVgrp() const;
    ///@}

private:
    /** Provides a label for members of a vertically aligned group. **/
    int m_vgrp;

    /* include <attvgrp> */
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
    void SetHo(data_MEASUREMENTREL ho_) { m_ho = ho_; }
    data_MEASUREMENTREL GetHo() const { return m_ho; }
    bool HasHo() const;
    ///@}

private:
    /**
     * Records a horizontal adjustment to a feature's programmatically-determined
     * location in terms of staff interline distance; that is, in units of 1/2 the
     * distance between adjacent staff lines.
     **/
    data_MEASUREMENTREL m_ho;

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
    void SetVo(data_MEASUREMENTREL vo_) { m_vo = vo_; }
    data_MEASUREMENTREL GetVo() const { return m_vo; }
    bool HasVo() const;
    ///@}

private:
    /**
     * Records the vertical adjustment of a feature's programmatically-determined
     * location in terms of staff interline distance; that is, in units of 1/2 the
     * distance between adjacent staff lines.
     **/
    data_MEASUREMENTREL m_vo;

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
    void SetStartho(data_MEASUREMENTREL startho_) { m_startho = startho_; }
    data_MEASUREMENTREL GetStartho() const { return m_startho; }
    bool HasStartho() const;
    //
    void SetEndho(data_MEASUREMENTREL endho_) { m_endho = endho_; }
    data_MEASUREMENTREL GetEndho() const { return m_endho; }
    bool HasEndho() const;
    ///@}

private:
    /**
     * Records the horizontal adjustment of a feature's programmatically-determined
     * start point.
     **/
    data_MEASUREMENTREL m_startho;
    /**
     * Records the horizontal adjustment of a feature's programmatically-determined end
     * point.
     **/
    data_MEASUREMENTREL m_endho;

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
    void SetStartvo(data_MEASUREMENTREL startvo_) { m_startvo = startvo_; }
    data_MEASUREMENTREL GetStartvo() const { return m_startvo; }
    bool HasStartvo() const;
    //
    void SetEndvo(data_MEASUREMENTREL endvo_) { m_endvo = endvo_; }
    data_MEASUREMENTREL GetEndvo() const { return m_endvo; }
    bool HasEndvo() const;
    ///@}

private:
    /**
     * Records a vertical adjustment of a feature's programmatically-determined start
     * point.
     **/
    data_MEASUREMENTREL m_startvo;
    /**
     * Records a vertical adjustment of a feature's programmatically-determined end
     * point.
     **/
    data_MEASUREMENTREL m_endvo;

    /* include <attendvo> */
};

//----------------------------------------------------------------------------
// AttVoltaGroupingSym
//----------------------------------------------------------------------------

class AttVoltaGroupingSym : public Att {
public:
    AttVoltaGroupingSym();
    virtual ~AttVoltaGroupingSym();

    /** Reset the default values for the attribute class **/
    void ResetVoltaGroupingSym();

    /** Read the values for the attribute class **/
    bool ReadVoltaGroupingSym(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVoltaGroupingSym(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetVoltasym(voltaGroupingSym_VOLTASYM voltasym_) { m_voltasym = voltasym_; }
    voltaGroupingSym_VOLTASYM GetVoltasym() const { return m_voltasym; }
    bool HasVoltasym() const;
    ///@}

private:
    /** Specifies the symbol used to group lyrics. **/
    voltaGroupingSym_VOLTASYM m_voltasym;

    /* include <attvoltasym> */
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
    void SetWidth(data_MEASUREMENTABS width_) { m_width = width_; }
    data_MEASUREMENTABS GetWidth() const { return m_width; }
    bool HasWidth() const;
    ///@}

private:
    /** Measurement of the horizontal dimension of an entity. **/
    data_MEASUREMENTABS m_width;

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
