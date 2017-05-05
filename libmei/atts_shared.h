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
    void SetAccid(data_ACCIDENTAL_EXPLICIT accid_) { m_accid = accid_; }
    data_ACCIDENTAL_EXPLICIT GetAccid() const { return m_accid; }
    bool HasAccid() const;
    ///@}

private:
    /** Captures a written accidental. **/
    data_ACCIDENTAL_EXPLICIT m_accid;

    /* include <attaccid> */
};

//----------------------------------------------------------------------------
// AttAccidentalPerformed
//----------------------------------------------------------------------------

class AttAccidentalPerformed : public Att {
public:
    AttAccidentalPerformed();
    virtual ~AttAccidentalPerformed();

    /** Reset the default values for the attribute class **/
    void ResetAccidentalPerformed();

    /** Read the values for the attribute class **/
    bool ReadAccidentalPerformed(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAccidentalPerformed(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAccidGes(data_ACCIDENTAL_IMPLICIT accidGes_) { m_accidGes = accidGes_; }
    data_ACCIDENTAL_IMPLICIT GetAccidGes() const { return m_accidGes; }
    bool HasAccidGes() const;
    ///@}

private:
    /** Records the performed pitch inflection. **/
    data_ACCIDENTAL_IMPLICIT m_accidGes;

    /* include <attaccid.ges> */
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
     * encoded using the <fingering> element.
     **/
    data_ARTICULATION_List m_artic;

    /* include <attartic> */
};

//----------------------------------------------------------------------------
// AttArticulationPerformed
//----------------------------------------------------------------------------

class AttArticulationPerformed : public Att {
public:
    AttArticulationPerformed();
    virtual ~AttArticulationPerformed();

    /** Reset the default values for the attribute class **/
    void ResetArticulationPerformed();

    /** Read the values for the attribute class **/
    bool ReadArticulationPerformed(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteArticulationPerformed(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetArticGes(data_ARTICULATION articGes_) { m_articGes = articGes_; }
    data_ARTICULATION GetArticGes() const { return m_articGes; }
    bool HasArticGes() const;
    ///@}

private:
    /** Records performed articulation that differs from the written value. **/
    data_ARTICULATION m_articGes;

    /* include <attartic.ges> */
};

//----------------------------------------------------------------------------
// AttAugmentdots
//----------------------------------------------------------------------------

class AttAugmentdots : public Att {
public:
    AttAugmentdots();
    virtual ~AttAugmentdots();

    /** Reset the default values for the attribute class **/
    void ResetAugmentdots();

    /** Read the values for the attribute class **/
    bool ReadAugmentdots(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAugmentdots(pugi::xml_node element);

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
    void SetAuthority(std::string authority_) { m_authority = authority_; }
    std::string GetAuthority() const { return m_authority; }
    bool HasAuthority() const;
    //
    void SetAuthURI(std::string authURI_) { m_authURI = authURI_; }
    std::string GetAuthURI() const { return m_authURI; }
    bool HasAuthURI() const;
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
// AttBarplacement
//----------------------------------------------------------------------------

class AttBarplacement : public Att {
public:
    AttBarplacement();
    virtual ~AttBarplacement();

    /** Reset the default values for the attribute class **/
    void ResetBarplacement();

    /** Read the values for the attribute class **/
    bool ReadBarplacement(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBarplacement(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBarplace(data_BARPLACE barplace_) { m_barplace = barplace_; }
    data_BARPLACE GetBarplace() const { return m_barplace; }
    bool HasBarplace() const;
    //
    void SetTaktplace(int taktplace_) { m_taktplace = taktplace_; }
    int GetTaktplace() const { return m_taktplace; }
    bool HasTaktplace() const;
    ///@}

private:
    /** Records the location of a bar line. **/
    data_BARPLACE m_barplace;
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
    int m_taktplace;

    /* include <atttaktplace> */
};

//----------------------------------------------------------------------------
// AttBeamingVis
//----------------------------------------------------------------------------

class AttBeamingVis : public Att {
public:
    AttBeamingVis();
    virtual ~AttBeamingVis();

    /** Reset the default values for the attribute class **/
    void ResetBeamingVis();

    /** Read the values for the attribute class **/
    bool ReadBeamingVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBeamingVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBeamColor(std::string beamColor_) { m_beamColor = beamColor_; }
    std::string GetBeamColor() const { return m_beamColor; }
    bool HasBeamColor() const;
    //
    void SetBeamRend(beamingVis_BEAMREND beamRend_) { m_beamRend = beamRend_; }
    beamingVis_BEAMREND GetBeamRend() const { return m_beamRend; }
    bool HasBeamRend() const;
    //
    void SetBeamSlope(double beamSlope_) { m_beamSlope = beamSlope_; }
    double GetBeamSlope() const { return m_beamSlope; }
    bool HasBeamSlope() const;
    ///@}

private:
    /** Color of beams, including those associated with tuplets. **/
    std::string m_beamColor;
    /** Encodes whether a beam is "feathered" and in which direction. **/
    beamingVis_BEAMREND m_beamRend;
    /** Captures beam slope. **/
    double m_beamSlope;

    /* include <attbeam.slope> */
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
     * authURI attribute.
     **/
    std::string m_codedval;

    /* include <attcodedval> */
};

//----------------------------------------------------------------------------
// AttChordVis
//----------------------------------------------------------------------------

class AttChordVis : public Att {
public:
    AttChordVis();
    virtual ~AttChordVis();

    /** Reset the default values for the attribute class **/
    void ResetChordVis();

    /** Read the values for the attribute class **/
    bool ReadChordVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteChordVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCluster(data_CLUSTER cluster_) { m_cluster = cluster_; }
    data_CLUSTER GetCluster() const { return m_cluster; }
    bool HasCluster() const;
    ///@}

private:
    /**
     * Indicates a single, alternative note head should be displayed instead of
     * individual note heads.
     * The highest and lowest notes of the chord usually indicate the upper and lower
     * boundaries of the cluster note head.
     **/
    data_CLUSTER m_cluster;

    /* include <attcluster> */
};

//----------------------------------------------------------------------------
// AttClasscodeident
//----------------------------------------------------------------------------

class AttClasscodeident : public Att {
public:
    AttClasscodeident();
    virtual ~AttClasscodeident();

    /** Reset the default values for the attribute class **/
    void ResetClasscodeident();

    /** Read the values for the attribute class **/
    bool ReadClasscodeident(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteClasscodeident(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetClasscode(std::string classcode_) { m_classcode = classcode_; }
    std::string GetClasscode() const { return m_classcode; }
    bool HasClasscode() const;
    ///@}

private:
    /**
     * Contains a reference to the controlled vocabulary from which the term is drawn.
     * The value must match the value of an ID attribute on a classCode element given
     * elsewhere in the document.
     **/
    std::string m_classcode;

    /* include <attclasscode> */
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
    void SetClefDisPlace(data_PLACE clefDisPlace_) { m_clefDisPlace = clefDisPlace_; }
    data_PLACE GetClefDisPlace() const { return m_clefDisPlace; }
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
    data_PLACE m_clefDisPlace;

    /* include <attclef.dis.place> */
};

//----------------------------------------------------------------------------
// AttCleffingVis
//----------------------------------------------------------------------------

class AttCleffingVis : public Att {
public:
    AttCleffingVis();
    virtual ~AttCleffingVis();

    /** Reset the default values for the attribute class **/
    void ResetCleffingVis();

    /** Read the values for the attribute class **/
    bool ReadCleffingVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCleffingVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetClefColor(std::string clefColor_) { m_clefColor = clefColor_; }
    std::string GetClefColor() const { return m_clefColor; }
    bool HasClefColor() const;
    //
    void SetClefVisible(data_BOOLEAN clefVisible_) { m_clefVisible = clefVisible_; }
    data_BOOLEAN GetClefVisible() const { return m_clefVisible; }
    bool HasClefVisible() const;
    ///@}

private:
    /** Describes the color of the clef. **/
    std::string m_clefColor;
    /** Determines whether the clef is to be displayed. **/
    data_BOOLEAN m_clefVisible;

    /* include <attclef.visible> */
};

//----------------------------------------------------------------------------
// AttClefshape
//----------------------------------------------------------------------------

class AttClefshape : public Att {
public:
    AttClefshape();
    virtual ~AttClefshape();

    /** Reset the default values for the attribute class **/
    void ResetClefshape();

    /** Read the values for the attribute class **/
    bool ReadClefshape(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteClefshape(pugi::xml_node element);

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
// AttCommon
//----------------------------------------------------------------------------

class AttCommon : public Att {
public:
    AttCommon();
    virtual ~AttCommon();

    /** Reset the default values for the attribute class **/
    void ResetCommon();

    /** Read the values for the attribute class **/
    bool ReadCommon(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCommon(pugi::xml_node element);

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
    /** Provides a number-like designation for an element. **/
    int m_n;

    /* include <attn> */
};

//----------------------------------------------------------------------------
// AttCommonPart
//----------------------------------------------------------------------------

class AttCommonPart : public Att {
public:
    AttCommonPart();
    virtual ~AttCommonPart();

    /** Reset the default values for the attribute class **/
    void ResetCommonPart();

    /** Read the values for the attribute class **/
    bool ReadCommonPart(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCommonPart(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLabel(std::string label_) { m_label = label_; }
    std::string GetLabel() const { return m_label; }
    bool HasLabel() const;
    //
    void SetBase(std::string base_) { m_base = base_; }
    std::string GetBase() const { return m_base; }
    bool HasBase() const;
    ///@}

private:
    /**
     * Provides a name or label for an element.
     * The value may be any string.
     **/
    std::string m_label;
    /** --- **/
    std::string m_base;

    /* include <attbase> */
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
// AttCurverend
//----------------------------------------------------------------------------

class AttCurverend : public Att {
public:
    AttCurverend();
    virtual ~AttCurverend();

    /** Reset the default values for the attribute class **/
    void ResetCurverend();

    /** Read the values for the attribute class **/
    bool ReadCurverend(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCurverend(pugi::xml_node element);

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
// AttDatapointing
//----------------------------------------------------------------------------

class AttDatapointing : public Att {
public:
    AttDatapointing();
    virtual ~AttDatapointing();

    /** Reset the default values for the attribute class **/
    void ResetDatapointing();

    /** Read the values for the attribute class **/
    bool ReadDatapointing(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDatapointing(pugi::xml_node element);

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
// AttDeclaring
//----------------------------------------------------------------------------

class AttDeclaring : public Att {
public:
    AttDeclaring();
    virtual ~AttDeclaring();

    /** Reset the default values for the attribute class **/
    void ResetDeclaring();

    /** Read the values for the attribute class **/
    bool ReadDeclaring(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDeclaring(pugi::xml_node element);

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
     * Identifies one or more metadata elements within the header, which are understood
     * to apply to the element bearing this attribute and its content.
     **/
    std::string m_decls;

    /* include <attdecls> */
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
    void SetDynamDist(std::string dynamDist_) { m_dynamDist = dynamDist_; }
    std::string GetDynamDist() const { return m_dynamDist; }
    bool HasDynamDist() const;
    //
    void SetHarmDist(std::string harmDist_) { m_harmDist = harmDist_; }
    std::string GetHarmDist() const { return m_harmDist; }
    bool HasHarmDist() const;
    //
    void SetTextDist(std::string textDist_) { m_textDist = textDist_; }
    std::string GetTextDist() const { return m_textDist; }
    bool HasTextDist() const;
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
     * Records duration using optionally dotted, relative durational values provided by
     * the data.DURATION datatype.
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
// AttDurationMusical
//----------------------------------------------------------------------------

class AttDurationMusical : public Att {
public:
    AttDurationMusical();
    virtual ~AttDurationMusical();

    /** Reset the default values for the attribute class **/
    void ResetDurationMusical();

    /** Read the values for the attribute class **/
    bool ReadDurationMusical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDurationMusical(pugi::xml_node element);

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
     * Records duration using optionally dotted, relative durational values provided by
     * the data.DURATION datatype.
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

class AttDurationPerformed : public Att {
public:
    AttDurationPerformed();
    virtual ~AttDurationPerformed();

    /** Reset the default values for the attribute class **/
    void ResetDurationPerformed();

    /** Read the values for the attribute class **/
    bool ReadDurationPerformed(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDurationPerformed(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDurGes(data_DURATION durGes_) { m_durGes = durGes_; }
    data_DURATION GetDurGes() const { return m_durGes; }
    bool HasDurGes() const;
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
// AttEnclosingchars
//----------------------------------------------------------------------------

class AttEnclosingchars : public Att {
public:
    AttEnclosingchars();
    virtual ~AttEnclosingchars();

    /** Reset the default values for the attribute class **/
    void ResetEnclosingchars();

    /** Read the values for the attribute class **/
    bool ReadEnclosingchars(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteEnclosingchars(pugi::xml_node element);

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
// AttFermatapresent
//----------------------------------------------------------------------------

class AttFermatapresent : public Att {
public:
    AttFermatapresent();
    virtual ~AttFermatapresent();

    /** Reset the default values for the attribute class **/
    void ResetFermatapresent();

    /** Read the values for the attribute class **/
    bool ReadFermatapresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteFermatapresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFermata(data_PLACE fermata_) { m_fermata = fermata_; }
    data_PLACE GetFermata() const { return m_fermata; }
    bool HasFermata() const;
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
// AttHandident
//----------------------------------------------------------------------------

class AttHandident : public Att {
public:
    AttHandident();
    virtual ~AttHandident();

    /** Reset the default values for the attribute class **/
    void ResetHandident();

    /** Read the values for the attribute class **/
    bool ReadHandident(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHandident(pugi::xml_node element);

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
     * The value must be the ID of a <hand> element declared in the header.
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
// AttHorizontalalign
//----------------------------------------------------------------------------

class AttHorizontalalign : public Att {
public:
    AttHorizontalalign();
    virtual ~AttHorizontalalign();

    /** Reset the default values for the attribute class **/
    void ResetHorizontalalign();

    /** Read the values for the attribute class **/
    bool ReadHorizontalalign(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHorizontalalign(pugi::xml_node element);

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
// AttInstrumentident
//----------------------------------------------------------------------------

class AttInstrumentident : public Att {
public:
    AttInstrumentident();
    virtual ~AttInstrumentident();

    /** Reset the default values for the attribute class **/
    void ResetInstrumentident();

    /** Read the values for the attribute class **/
    bool ReadInstrumentident(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteInstrumentident(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetInstr(std::string instr_) { m_instr = instr_; }
    std::string GetInstr() const { return m_instr; }
    bool HasInstr() const;
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

class AttInternetmedia : public Att {
public:
    AttInternetmedia();
    virtual ~AttInternetmedia();

    /** Reset the default values for the attribute class **/
    void ResetInternetmedia();

    /** Read the values for the attribute class **/
    bool ReadInternetmedia(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteInternetmedia(pugi::xml_node element);

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
    //
    void SetSigMixed(std::string sigMixed_) { m_sigMixed = sigMixed_; }
    std::string GetSigMixed() const { return m_sigMixed; }
    bool HasSigMixed() const;
    //
    void SetMode(data_MODE mode_) { m_mode = mode_; }
    data_MODE GetMode() const { return m_mode; }
    bool HasMode() const;
    ///@}

private:
    /** Indicates where the key lies in the circle of fifths. **/
    data_KEYSIGNATURE m_sig;
    /**
     * Mixed key signatures, e.g.
     * those consisting of a mixture of flats and sharps (Read, p. 143, ex. 9-39), and
     * key signatures with unorthodox placement of the accidentals (Read, p. 141) must
     * be indicated by setting the key.sig attribute to 'mixed' and providing explicit
     * key signature information in the key.sig.mixed attribute or in the <keySig>
     * element. It is intended that key.sig.mixed contain a series of tokens with each
     * token containing pitch name, accidental, and octave, such as 'a4 c5s e5f' that
     * indicate what key accidentals should be rendered and where they should be
     * placed.
     **/
    std::string m_sigMixed;
    /** Indicates major, minor, or other tonality. **/
    data_MODE m_mode;

    /* include <attmode> */
};

//----------------------------------------------------------------------------
// AttKeySigVis
//----------------------------------------------------------------------------

class AttKeySigVis : public Att {
public:
    AttKeySigVis();
    virtual ~AttKeySigVis();

    /** Reset the default values for the attribute class **/
    void ResetKeySigVis();

    /** Read the values for the attribute class **/
    bool ReadKeySigVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteKeySigVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSigShowchange(data_BOOLEAN sigShowchange_) { m_sigShowchange = sigShowchange_; }
    data_BOOLEAN GetSigShowchange() const { return m_sigShowchange; }
    bool HasSigShowchange() const;
    ///@}

private:
    /** Determines whether cautionary accidentals should be displayed at a key change. **/
    data_BOOLEAN m_sigShowchange;

    /* include <attsig.showchange> */
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
    void SetKeyAccid(data_ACCIDENTAL_IMPLICIT keyAccid_) { m_keyAccid = keyAccid_; }
    data_ACCIDENTAL_IMPLICIT GetKeyAccid() const { return m_keyAccid; }
    bool HasKeyAccid() const;
    //
    void SetKeyMode(data_MODE keyMode_) { m_keyMode = keyMode_; }
    data_MODE GetKeyMode() const { return m_keyMode; }
    bool HasKeyMode() const;
    //
    void SetKeyPname(data_PITCHNAME keyPname_) { m_keyPname = keyPname_; }
    data_PITCHNAME GetKeyPname() const { return m_keyPname; }
    bool HasKeyPname() const;
    //
    void SetKeySig(data_KEYSIGNATURE keySig_) { m_keySig = keySig_; }
    data_KEYSIGNATURE GetKeySig() const { return m_keySig; }
    bool HasKeySig() const;
    //
    void SetKeySigMixed(std::string keySigMixed_) { m_keySigMixed = keySigMixed_; }
    std::string GetKeySigMixed() const { return m_keySigMixed; }
    bool HasKeySigMixed() const;
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
     * token containing pitch name, accidental, and octave, such as 'a4 c5s e5f' that
     * indicate what key accidentals should be rendered and where they should be
     * placed.
     **/
    std::string m_keySigMixed;

    /* include <attkey.sig.mixed> */
};

//----------------------------------------------------------------------------
// AttKeySigDefaultVis
//----------------------------------------------------------------------------

class AttKeySigDefaultVis : public Att {
public:
    AttKeySigDefaultVis();
    virtual ~AttKeySigDefaultVis();

    /** Reset the default values for the attribute class **/
    void ResetKeySigDefaultVis();

    /** Read the values for the attribute class **/
    bool ReadKeySigDefaultVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteKeySigDefaultVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetKeySigShow(data_BOOLEAN keySigShow_) { m_keySigShow = keySigShow_; }
    data_BOOLEAN GetKeySigShow() const { return m_keySigShow; }
    bool HasKeySigShow() const;
    //
    void SetKeySigShowchange(data_BOOLEAN keySigShowchange_) { m_keySigShowchange = keySigShowchange_; }
    data_BOOLEAN GetKeySigShowchange() const { return m_keySigShowchange; }
    bool HasKeySigShowchange() const;
    ///@}

private:
    /** Indicates whether the key signature should be displayed. **/
    data_BOOLEAN m_keySigShow;
    /** Determines whether cautionary accidentals should be displayed at a key change. **/
    data_BOOLEAN m_keySigShowchange;

    /* include <attkey.sig.showchange> */
};

//----------------------------------------------------------------------------
// AttLabelsAddl
//----------------------------------------------------------------------------

class AttLabelsAddl : public Att {
public:
    AttLabelsAddl();
    virtual ~AttLabelsAddl();

    /** Reset the default values for the attribute class **/
    void ResetLabelsAddl();

    /** Read the values for the attribute class **/
    bool ReadLabelsAddl(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLabelsAddl(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLabelAbbr(std::string labelAbbr_) { m_labelAbbr = labelAbbr_; }
    std::string GetLabelAbbr() const { return m_labelAbbr; }
    bool HasLabelAbbr() const;
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
// AttLayerident
//----------------------------------------------------------------------------

class AttLayerident : public Att {
public:
    AttLayerident();
    virtual ~AttLayerident();

    /** Reset the default values for the attribute class **/
    void ResetLayerident();

    /** Read the values for the attribute class **/
    bool ReadLayerident(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLayerident(pugi::xml_node element);

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
// AttLineVis
//----------------------------------------------------------------------------

class AttLineVis : public Att {
public:
    AttLineVis();
    virtual ~AttLineVis();

    /** Reset the default values for the attribute class **/
    void ResetLineVis();

    /** Read the values for the attribute class **/
    bool ReadLineVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLineVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(data_LINEFORM form_) { m_form = form_; }
    data_LINEFORM GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetWidth(std::string width_) { m_width = width_; }
    std::string GetWidth() const { return m_width; }
    bool HasWidth() const;
    //
    void SetEndsym(data_LINESTARTENDSYMBOL endsym_) { m_endsym = endsym_; }
    data_LINESTARTENDSYMBOL GetEndsym() const { return m_endsym; }
    bool HasEndsym() const;
    //
    void SetEndsymsize(std::string endsymsize_) { m_endsymsize = endsymsize_; }
    std::string GetEndsymsize() const { return m_endsymsize; }
    bool HasEndsymsize() const;
    //
    void SetStartsym(data_LINESTARTENDSYMBOL startsym_) { m_startsym = startsym_; }
    data_LINESTARTENDSYMBOL GetStartsym() const { return m_startsym; }
    bool HasStartsym() const;
    //
    void SetStartsymsize(std::string startsymsize_) { m_startsymsize = startsymsize_; }
    std::string GetStartsymsize() const { return m_startsymsize; }
    bool HasStartsymsize() const;
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    data_LINEFORM m_form;
    /** Width of the line. **/
    std::string m_width;
    /** Symbol rendered at end of line. **/
    data_LINESTARTENDSYMBOL m_endsym;
    /** Holds the relative size of the line-end symbol. **/
    std::string m_endsymsize;
    /** Symbol rendered at start of line. **/
    data_LINESTARTENDSYMBOL m_startsym;
    /** Holds the relative size of the line-start symbol. **/
    std::string m_startsymsize;

    /* include <attstartsymsize> */
};

//----------------------------------------------------------------------------
// AttLineloc
//----------------------------------------------------------------------------

class AttLineloc : public Att {
public:
    AttLineloc();
    virtual ~AttLineloc();

    /** Reset the default values for the attribute class **/
    void ResetLineloc();

    /** Read the values for the attribute class **/
    bool ReadLineloc(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLineloc(pugi::xml_node element);

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
// AttLinerend
//----------------------------------------------------------------------------

class AttLinerend : public Att {
public:
    AttLinerend();
    virtual ~AttLinerend();

    /** Reset the default values for the attribute class **/
    void ResetLinerend();

    /** Read the values for the attribute class **/
    bool ReadLinerend(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLinerend(pugi::xml_node element);

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
    void SetLendsymsize(std::string lendsymsize_) { m_lendsymsize = lendsymsize_; }
    std::string GetLendsymsize() const { return m_lendsymsize; }
    bool HasLendsymsize() const;
    //
    void SetLstartsym(data_LINESTARTENDSYMBOL lstartsym_) { m_lstartsym = lstartsym_; }
    data_LINESTARTENDSYMBOL GetLstartsym() const { return m_lstartsym; }
    bool HasLstartsym() const;
    //
    void SetLstartsymsize(std::string lstartsymsize_) { m_lstartsymsize = lstartsymsize_; }
    std::string GetLstartsymsize() const { return m_lstartsymsize; }
    bool HasLstartsymsize() const;
    ///@}

private:
    /** Symbol rendered at end of line. **/
    data_LINESTARTENDSYMBOL m_lendsym;
    /** Holds the relative size of the line-end symbol. **/
    std::string m_lendsymsize;
    /** Symbol rendered at start of line. **/
    data_LINESTARTENDSYMBOL m_lstartsym;
    /** Holds the relative size of the line-start symbol. **/
    std::string m_lstartsymsize;

    /* include <attlstartsymsize> */
};

//----------------------------------------------------------------------------
// AttLinerendBase
//----------------------------------------------------------------------------

class AttLinerendBase : public Att {
public:
    AttLinerendBase();
    virtual ~AttLinerendBase();

    /** Reset the default values for the attribute class **/
    void ResetLinerendBase();

    /** Read the values for the attribute class **/
    bool ReadLinerendBase(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLinerendBase(pugi::xml_node element);

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
// AttLyricstyle
//----------------------------------------------------------------------------

class AttLyricstyle : public Att {
public:
    AttLyricstyle();
    virtual ~AttLyricstyle();

    /** Reset the default values for the attribute class **/
    void ResetLyricstyle();

    /** Read the values for the attribute class **/
    bool ReadLyricstyle(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLyricstyle(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLyricAlign(std::string lyricAlign_) { m_lyricAlign = lyricAlign_; }
    std::string GetLyricAlign() const { return m_lyricAlign; }
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
    void SetLyricSize(int lyricSize_) { m_lyricSize = lyricSize_; }
    int GetLyricSize() const { return m_lyricSize; }
    bool HasLyricSize() const;
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
    std::string m_lyricAlign;
    /** Sets the font family default value for lyrics. **/
    std::string m_lyricFam;
    /** Sets the font name default value for lyrics. **/
    std::string m_lyricName;
    /** Sets the default font size value for lyrics. **/
    int m_lyricSize;
    /** Sets the default font style value for lyrics. **/
    data_FONTSTYLE m_lyricStyle;
    /** Sets the default font weight value for lyrics. **/
    data_FONTWEIGHT m_lyricWeight;

    /* include <attlyric.weight> */
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
// AttMeasurenumbers
//----------------------------------------------------------------------------

class AttMeasurenumbers : public Att {
public:
    AttMeasurenumbers();
    virtual ~AttMeasurenumbers();

    /** Reset the default values for the attribute class **/
    void ResetMeasurenumbers();

    /** Read the values for the attribute class **/
    bool ReadMeasurenumbers(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeasurenumbers(pugi::xml_node element);

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
// AttMediabounds
//----------------------------------------------------------------------------

class AttMediabounds : public Att {
public:
    AttMediabounds();
    virtual ~AttMediabounds();

    /** Reset the default values for the attribute class **/
    void ResetMediabounds();

    /** Read the values for the attribute class **/
    bool ReadMediabounds(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMediabounds(pugi::xml_node element);

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
// AttMeiversion
//----------------------------------------------------------------------------

class AttMeiversion : public Att {
public:
    AttMeiversion();
    virtual ~AttMeiversion();

    /** Reset the default values for the attribute class **/
    void ResetMeiversion();

    /** Read the values for the attribute class **/
    bool ReadMeiversion(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeiversion(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMeiversion(meiversion_MEIVERSION meiversion_) { m_meiversion = meiversion_; }
    meiversion_MEIVERSION GetMeiversion() const { return m_meiversion; }
    bool HasMeiversion() const;
    ///@}

private:
    /** Specifies a generic MEI version label. **/
    meiversion_MEIVERSION m_meiversion;

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
// AttMeterSigVis
//----------------------------------------------------------------------------

class AttMeterSigVis : public Att {
public:
    AttMeterSigVis();
    virtual ~AttMeterSigVis();

    /** Reset the default values for the attribute class **/
    void ResetMeterSigVis();

    /** Read the values for the attribute class **/
    bool ReadMeterSigVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeterSigVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(meterSigVis_FORM form_) { m_form = form_; }
    meterSigVis_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    meterSigVis_FORM m_form;

    /* include <attform> */
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

class AttMeterSigDefaultVis : public Att {
public:
    AttMeterSigDefaultVis();
    virtual ~AttMeterSigDefaultVis();

    /** Reset the default values for the attribute class **/
    void ResetMeterSigDefaultVis();

    /** Read the values for the attribute class **/
    bool ReadMeterSigDefaultVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeterSigDefaultVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMeterRend(meterSigDefaultVis_METERREND meterRend_) { m_meterRend = meterRend_; }
    meterSigDefaultVis_METERREND GetMeterRend() const { return m_meterRend; }
    bool HasMeterRend() const;
    //
    void SetMeterShowchange(data_BOOLEAN meterShowchange_) { m_meterShowchange = meterShowchange_; }
    data_BOOLEAN GetMeterShowchange() const { return m_meterShowchange; }
    bool HasMeterShowchange() const;
    //
    void SetMeterSym(data_METERSIGN meterSym_) { m_meterSym = meterSym_; }
    data_METERSIGN GetMeterSym() const { return m_meterSym; }
    bool HasMeterSym() const;
    ///@}

private:
    /** Contains an indication of how the meter signature should be rendered. **/
    meterSigDefaultVis_METERREND m_meterRend;
    /**
     * Determines whether a new meter signature should be displayed when the meter
     * signature changes.
     **/
    data_BOOLEAN m_meterShowchange;
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

class AttMeterconformance : public Att {
public:
    AttMeterconformance();
    virtual ~AttMeterconformance();

    /** Reset the default values for the attribute class **/
    void ResetMeterconformance();

    /** Read the values for the attribute class **/
    bool ReadMeterconformance(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeterconformance(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMetcon(meterconformance_METCON metcon_) { m_metcon = metcon_; }
    meterconformance_METCON GetMetcon() const { return m_metcon; }
    bool HasMetcon() const;
    ///@}

private:
    /**
     * Indicates the relationship between the content of a staff or layer and the
     * prevailing meter.
     **/
    meterconformance_METCON m_metcon;

    /* include <attmetcon> */
};

//----------------------------------------------------------------------------
// AttMeterconformanceBar
//----------------------------------------------------------------------------

class AttMeterconformanceBar : public Att {
public:
    AttMeterconformanceBar();
    virtual ~AttMeterconformanceBar();

    /** Reset the default values for the attribute class **/
    void ResetMeterconformanceBar();

    /** Read the values for the attribute class **/
    bool ReadMeterconformanceBar(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMeterconformanceBar(pugi::xml_node element);

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
     * lines within parts may or may not be controlling. When applied to <measure>,
     * this attribute indicates the nature of the right barline but not the left.
     **/
    data_BOOLEAN m_control;

    /* include <attcontrol> */
};

//----------------------------------------------------------------------------
// AttMmtempo
//----------------------------------------------------------------------------

class AttMmtempo : public Att {
public:
    AttMmtempo();
    virtual ~AttMmtempo();

    /** Reset the default values for the attribute class **/
    void ResetMmtempo();

    /** Read the values for the attribute class **/
    bool ReadMmtempo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMmtempo(pugi::xml_node element);

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
// AttMultinummeasures
//----------------------------------------------------------------------------

class AttMultinummeasures : public Att {
public:
    AttMultinummeasures();
    virtual ~AttMultinummeasures();

    /** Reset the default values for the attribute class **/
    void ResetMultinummeasures();

    /** Read the values for the attribute class **/
    bool ReadMultinummeasures(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMultinummeasures(pugi::xml_node element);

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
     * Use a standard value whenever possible.
     **/
    std::string m_role;

    /* include <attrole> */
};

//----------------------------------------------------------------------------
// AttNotationstyle
//----------------------------------------------------------------------------

class AttNotationstyle : public Att {
public:
    AttNotationstyle();
    virtual ~AttNotationstyle();

    /** Reset the default values for the attribute class **/
    void ResetNotationstyle();

    /** Read the values for the attribute class **/
    bool ReadNotationstyle(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNotationstyle(pugi::xml_node element);

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
    void SetMusicSize(int musicSize_) { m_musicSize = musicSize_; }
    int GetMusicSize() const { return m_musicSize; }
    bool HasMusicSize() const;
    ///@}

private:
    /** Sets the default music font name. **/
    std::string m_musicName;
    /** Sets the default music font size. **/
    int m_musicSize;

    /* include <attmusic.size> */
};

//----------------------------------------------------------------------------
// AttNoteGes
//----------------------------------------------------------------------------

class AttNoteGes : public Att {
public:
    AttNoteGes();
    virtual ~AttNoteGes();

    /** Reset the default values for the attribute class **/
    void ResetNoteGes();

    /** Read the values for the attribute class **/
    bool ReadNoteGes(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNoteGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOctGes(char octGes_) { m_octGes = octGes_; }
    char GetOctGes() const { return m_octGes; }
    bool HasOctGes() const;
    //
    void SetPnameGes(data_PITCHNAME pnameGes_) { m_pnameGes = pnameGes_; }
    data_PITCHNAME GetPnameGes() const { return m_pnameGes; }
    bool HasPnameGes() const;
    //
    void SetPnum(int pnum_) { m_pnum = pnum_; }
    int GetPnum() const { return m_pnum; }
    bool HasPnum() const;
    ///@}

private:
    /** Records performed octave information that differs from the written value. **/
    char m_octGes;
    /** Contains a performed pitch name that differs from the written value. **/
    data_PITCHNAME m_pnameGes;
    /** Holds a pitch-to-number mapping, a base-40 or MIDI note number, for example. **/
    int m_pnum;

    /* include <attpnum> */
};

//----------------------------------------------------------------------------
// AttNoteheads
//----------------------------------------------------------------------------

class AttNoteheads : public Att {
public:
    AttNoteheads();
    virtual ~AttNoteheads();

    /** Reset the default values for the attribute class **/
    void ResetNoteheads();

    /** Read the values for the attribute class **/
    bool ReadNoteheads(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNoteheads(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
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
    void SetHeadMod(std::string headMod_) { m_headMod = headMod_; }
    std::string GetHeadMod() const { return m_headMod; }
    bool HasHeadMod() const;
    //
    void SetHeadRotation(std::string headRotation_) { m_headRotation = headRotation_; }
    std::string GetHeadRotation() const { return m_headRotation; }
    bool HasHeadRotation() const;
    //
    void SetHeadShape(data_HEADSHAPE_list headShape_) { m_headShape = headShape_; }
    data_HEADSHAPE_list GetHeadShape() const { return m_headShape; }
    bool HasHeadShape() const;
    //
    void SetHeadVisible(data_BOOLEAN headVisible_) { m_headVisible = headVisible_; }
    data_BOOLEAN GetHeadVisible() const { return m_headVisible; }
    bool HasHeadVisible() const;
    ///@}

private:
    /** Captures the overall color of a notehead. **/
    std::string m_headColor;
    /** Describes how/if the notehead is filled. **/
    data_FILL m_headFill;
    /** Captures the fill color of a notehead if different from the overall note color. **/
    std::string m_headFillcolor;
    /** Records any additional symbols applied to the notehead. **/
    std::string m_headMod;
    /**
     * Describes rotation applied to the basic notehead shape.
     * A positive value rotates the notehead in a counter-clockwise fashion, while
     * negative values produce clockwise rotation.
     **/
    std::string m_headRotation;
    /** Used to override the head shape normally used for the given duration. **/
    data_HEADSHAPE_list m_headShape;
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
// AttOctavedefault
//----------------------------------------------------------------------------

class AttOctavedefault : public Att {
public:
    AttOctavedefault();
    virtual ~AttOctavedefault();

    /** Reset the default values for the attribute class **/
    void ResetOctavedefault();

    /** Read the values for the attribute class **/
    bool ReadOctavedefault(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOctavedefault(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOctaveDefault(char octaveDefault_) { m_octaveDefault = octaveDefault_; }
    char GetOctaveDefault() const { return m_octaveDefault; }
    bool HasOctaveDefault() const;
    ///@}

private:
    /**
     * Contains a default octave specification for use when the first note, rest,
     * chord, etc.
     * in a measure does not have an octave value specified.
     **/
    char m_octaveDefault;

    /* include <attoctave.default> */
};

//----------------------------------------------------------------------------
// AttOctavedisplacement
//----------------------------------------------------------------------------

class AttOctavedisplacement : public Att {
public:
    AttOctavedisplacement();
    virtual ~AttOctavedisplacement();

    /** Reset the default values for the attribute class **/
    void ResetOctavedisplacement();

    /** Read the values for the attribute class **/
    bool ReadOctavedisplacement(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOctavedisplacement(pugi::xml_node element);

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
    void SetDisPlace(data_PLACE disPlace_) { m_disPlace = disPlace_; }
    data_PLACE GetDisPlace() const { return m_disPlace; }
    bool HasDisPlace() const;
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

class AttOnelinestaff : public Att {
public:
    AttOnelinestaff();
    virtual ~AttOnelinestaff();

    /** Reset the default values for the attribute class **/
    void ResetOnelinestaff();

    /** Read the values for the attribute class **/
    bool ReadOnelinestaff(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOnelinestaff(pugi::xml_node element);

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
// AttOriginLayerident
//----------------------------------------------------------------------------

class AttOriginLayerident : public Att {
public:
    AttOriginLayerident();
    virtual ~AttOriginLayerident();

    /** Reset the default values for the attribute class **/
    void ResetOriginLayerident();

    /** Read the values for the attribute class **/
    bool ReadOriginLayerident(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOriginLayerident(pugi::xml_node element);

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
// AttOriginStaffident
//----------------------------------------------------------------------------

class AttOriginStaffident : public Att {
public:
    AttOriginStaffident();
    virtual ~AttOriginStaffident();

    /** Reset the default values for the attribute class **/
    void ResetOriginStaffident();

    /** Read the values for the attribute class **/
    bool ReadOriginStaffident(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOriginStaffident(pugi::xml_node element);

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
// AttOriginStartendid
//----------------------------------------------------------------------------

class AttOriginStartendid : public Att {
public:
    AttOriginStartendid();
    virtual ~AttOriginStartendid();

    /** Reset the default values for the attribute class **/
    void ResetOriginStartendid();

    /** Read the values for the attribute class **/
    bool ReadOriginStartendid(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOriginStartendid(pugi::xml_node element);

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
// AttOriginTimestampMusical
//----------------------------------------------------------------------------

class AttOriginTimestampMusical : public Att {
public:
    AttOriginTimestampMusical();
    virtual ~AttOriginTimestampMusical();

    /** Reset the default values for the attribute class **/
    void ResetOriginTimestampMusical();

    /** Read the values for the attribute class **/
    bool ReadOriginTimestampMusical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOriginTimestampMusical(pugi::xml_node element);

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
     * The values are relative to the measure identified by @origin.tstamp.
     **/
    data_MEASUREBEAT m_originTstamp2;

    /* include <attorigin.tstamp2> */
};

//----------------------------------------------------------------------------
// AttPadLog
//----------------------------------------------------------------------------

class AttPadLog : public Att {
public:
    AttPadLog();
    virtual ~AttPadLog();

    /** Reset the default values for the attribute class **/
    void ResetPadLog();

    /** Read the values for the attribute class **/
    bool ReadPadLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePadLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetNum(int num_) { m_num = num_; }
    int GetNum() const { return m_num; }
    bool HasNum() const;
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
    void SetPageHeight(std::string pageHeight_) { m_pageHeight = pageHeight_; }
    std::string GetPageHeight() const { return m_pageHeight; }
    bool HasPageHeight() const;
    //
    void SetPageWidth(std::string pageWidth_) { m_pageWidth = pageWidth_; }
    std::string GetPageWidth() const { return m_pageWidth; }
    bool HasPageWidth() const;
    //
    void SetPageTopmar(std::string pageTopmar_) { m_pageTopmar = pageTopmar_; }
    std::string GetPageTopmar() const { return m_pageTopmar; }
    bool HasPageTopmar() const;
    //
    void SetPageBotmar(std::string pageBotmar_) { m_pageBotmar = pageBotmar_; }
    std::string GetPageBotmar() const { return m_pageBotmar; }
    bool HasPageBotmar() const;
    //
    void SetPageLeftmar(std::string pageLeftmar_) { m_pageLeftmar = pageLeftmar_; }
    std::string GetPageLeftmar() const { return m_pageLeftmar; }
    bool HasPageLeftmar() const;
    //
    void SetPageRightmar(std::string pageRightmar_) { m_pageRightmar = pageRightmar_; }
    std::string GetPageRightmar() const { return m_pageRightmar; }
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

    /* include <attpage.scale> */
};

//----------------------------------------------------------------------------
// AttPbVis
//----------------------------------------------------------------------------

class AttPbVis : public Att {
public:
    AttPbVis();
    virtual ~AttPbVis();

    /** Reset the default values for the attribute class **/
    void ResetPbVis();

    /** Read the values for the attribute class **/
    bool ReadPbVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePbVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFolium(pbVis_FOLIUM folium_) { m_folium = folium_; }
    pbVis_FOLIUM GetFolium() const { return m_folium; }
    bool HasFolium() const;
    ///@}

private:
    /**
     * States the side of a leaf (as in a manuscript) on which the content following
     * the <pb> element occurs.
     **/
    pbVis_FOLIUM m_folium;

    /* include <attfolium> */
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
    /**
     * Captures the placement of the item with respect to the staff with which it is
     * associated.
     **/
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
    void SetPlist(std::string plist_) { m_plist = plist_; }
    std::string GetPlist() const { return m_plist; }
    bool HasPlist() const;
    ///@}

private:
    /**
     * Contains a space separated list of references that identify active participants
     * in a collection/relationship, such as notes under a phrase mark; that is, the
     * entities pointed "from".
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
     * Use a standard value whenever possible.
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
// AttRelativesize
//----------------------------------------------------------------------------

class AttRelativesize : public Att {
public:
    AttRelativesize();
    virtual ~AttRelativesize();

    /** Reset the default values for the attribute class **/
    void ResetRelativesize();

    /** Read the values for the attribute class **/
    bool ReadRelativesize(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteRelativesize(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSize(data_SIZE size_) { m_size = size_; }
    data_SIZE GetSize() const { return m_size; }
    bool HasSize() const;
    ///@}

private:
    /** Describes the relative size of a feature. **/
    data_SIZE m_size;

    /* include <attsize> */
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
// AttSbVis
//----------------------------------------------------------------------------

class AttSbVis : public Att {
public:
    AttSbVis();
    virtual ~AttSbVis();

    /** Reset the default values for the attribute class **/
    void ResetSbVis();

    /** Read the values for the attribute class **/
    bool ReadSbVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSbVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(sbVis_FORM form_) { m_form = form_; }
    sbVis_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    sbVis_FORM m_form;

    /* include <attform> */
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
// AttScoreDefGes
//----------------------------------------------------------------------------

class AttScoreDefGes : public Att {
public:
    AttScoreDefGes();
    virtual ~AttScoreDefGes();

    /** Reset the default values for the attribute class **/
    void ResetScoreDefGes();

    /** Read the values for the attribute class **/
    bool ReadScoreDefGes(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteScoreDefGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTunePname(data_PITCHNAME tunePname_) { m_tunePname = tunePname_; }
    data_PITCHNAME GetTunePname() const { return m_tunePname; }
    bool HasTunePname() const;
    //
    void SetTuneHz(double tuneHz_) { m_tuneHz = tuneHz_; }
    double GetTuneHz() const { return m_tuneHz; }
    bool HasTuneHz() const;
    //
    void SetTuneTemper(data_TEMPERAMENT tuneTemper_) { m_tuneTemper = tuneTemper_; }
    data_TEMPERAMENT GetTuneTemper() const { return m_tuneTemper; }
    bool HasTuneTemper() const;
    ///@}

private:
    /** Holds the pitch name of a tuning reference pitch. **/
    data_PITCHNAME m_tunePname;
    /** Holds a value for cycles per second, i.e., Hertz, for a tuning reference pitch. **/
    double m_tuneHz;
    /** Provides an indication of the tuning system, 'just', for example. **/
    data_TEMPERAMENT m_tuneTemper;

    /* include <atttune.temper> */
};

//----------------------------------------------------------------------------
// AttScoreDefVis
//----------------------------------------------------------------------------

class AttScoreDefVis : public Att {
public:
    AttScoreDefVis();
    virtual ~AttScoreDefVis();

    /** Reset the default values for the attribute class **/
    void ResetScoreDefVis();

    /** Read the values for the attribute class **/
    bool ReadScoreDefVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteScoreDefVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetVuHeight(std::string vuHeight_) { m_vuHeight = vuHeight_; }
    std::string GetVuHeight() const { return m_vuHeight; }
    bool HasVuHeight() const;
    ///@}

private:
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

class AttSectionVis : public Att {
public:
    AttSectionVis();
    virtual ~AttSectionVis();

    /** Reset the default values for the attribute class **/
    void ResetSectionVis();

    /** Read the values for the attribute class **/
    bool ReadSectionVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSectionVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetRestart(data_BOOLEAN restart_) { m_restart = restart_; }
    data_BOOLEAN GetRestart() const { return m_restart; }
    bool HasRestart() const;
    ///@}

private:
    /** Indicates that staves begin again with this section. **/
    data_BOOLEAN m_restart;

    /* include <attrestart> */
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
// AttSlashcount
//----------------------------------------------------------------------------

class AttSlashcount : public Att {
public:
    AttSlashcount();
    virtual ~AttSlashcount();

    /** Reset the default values for the attribute class **/
    void ResetSlashcount();

    /** Read the values for the attribute class **/
    bool ReadSlashcount(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSlashcount(pugi::xml_node element);

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
// AttSlurpresent
//----------------------------------------------------------------------------

class AttSlurpresent : public Att {
public:
    AttSlurpresent();
    virtual ~AttSlurpresent();

    /** Reset the default values for the attribute class **/
    void ResetSlurpresent();

    /** Read the values for the attribute class **/
    bool ReadSlurpresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSlurpresent(pugi::xml_node element);

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
     * If visual information about the slur needs to be recorded, then a <slur> element
     * should be employed.
     **/
    std::string m_slur;

    /* include <attslur> */
};

//----------------------------------------------------------------------------
// AttSpaceVis
//----------------------------------------------------------------------------

class AttSpaceVis : public Att {
public:
    AttSpaceVis();
    virtual ~AttSpaceVis();

    /** Reset the default values for the attribute class **/
    void ResetSpaceVis();

    /** Read the values for the attribute class **/
    bool ReadSpaceVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSpaceVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCompressable(data_BOOLEAN compressable_) { m_compressable = compressable_; }
    data_BOOLEAN GetCompressable() const { return m_compressable; }
    bool HasCompressable() const;
    ///@}

private:
    /**
     * Indicates whether a space is 'compressible', i.e., if it may be removed at the
     * discretion of processing software.
     **/
    data_BOOLEAN m_compressable;

    /* include <attcompressable> */
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
    void SetSpacingStaff(std::string spacingStaff_) { m_spacingStaff = spacingStaff_; }
    std::string GetSpacingStaff() const { return m_spacingStaff; }
    bool HasSpacingStaff() const;
    //
    void SetSpacingSystem(std::string spacingSystem_) { m_spacingSystem = spacingSystem_; }
    std::string GetSpacingSystem() const { return m_spacingSystem; }
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
    std::string m_spacingStaff;
    /**
     * Describes the space between adjacent systems; a pair of space-separated values
     * (minimum and maximum, respectively) provides a range between which a rendering
     * system-supplied value may fall, while a single value indicates a fixed amount of
     * space; that is, the minimum and maximum values are equal.
     **/
    std::string m_spacingSystem;

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
// AttStaffDefVis
//----------------------------------------------------------------------------

class AttStaffDefVis : public Att {
public:
    AttStaffDefVis();
    virtual ~AttStaffDefVis();

    /** Reset the default values for the attribute class **/
    void ResetStaffDefVis();

    /** Read the values for the attribute class **/
    bool ReadStaffDefVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffDefVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetGridShow(data_BOOLEAN gridShow_) { m_gridShow = gridShow_; }
    data_BOOLEAN GetGridShow() const { return m_gridShow; }
    bool HasGridShow() const;
    //
    void SetLayerscheme(data_LAYERSCHEME layerscheme_) { m_layerscheme = layerscheme_; }
    data_LAYERSCHEME GetLayerscheme() const { return m_layerscheme; }
    bool HasLayerscheme() const;
    //
    void SetLines(int lines_) { m_lines = lines_; }
    int GetLines() const { return m_lines; }
    bool HasLines() const;
    //
    void SetLinesColor(std::string linesColor_) { m_linesColor = linesColor_; }
    std::string GetLinesColor() const { return m_linesColor; }
    bool HasLinesColor() const;
    //
    void SetLinesVisible(data_BOOLEAN linesVisible_) { m_linesVisible = linesVisible_; }
    data_BOOLEAN GetLinesVisible() const { return m_linesVisible; }
    bool HasLinesVisible() const;
    //
    void SetSpacing(std::string spacing_) { m_spacing = spacing_; }
    std::string GetSpacing() const { return m_spacing; }
    bool HasSpacing() const;
    ///@}

private:
    /** Determines whether to display guitar chord grids. **/
    data_BOOLEAN m_gridShow;
    /** Indicates the number of layers and their stem directions. **/
    data_LAYERSCHEME m_layerscheme;
    /** Indicates the number of staff lines. **/
    int m_lines;
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

class AttStaffGrpVis : public Att {
public:
    AttStaffGrpVis();
    virtual ~AttStaffGrpVis();

    /** Reset the default values for the attribute class **/
    void ResetStaffGrpVis();

    /** Read the values for the attribute class **/
    bool ReadStaffGrpVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffGrpVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBarthru(data_BOOLEAN barthru_) { m_barthru = barthru_; }
    data_BOOLEAN GetBarthru() const { return m_barthru; }
    bool HasBarthru() const;
    ///@}

private:
    /**
     * Indicates whether bar lines go across the space between staves (true) or are
     * only drawn across the lines of each staff (false).
     **/
    data_BOOLEAN m_barthru;

    /* include <attbarthru> */
};

//----------------------------------------------------------------------------
// AttStaffgroupingsym
//----------------------------------------------------------------------------

class AttStaffgroupingsym : public Att {
public:
    AttStaffgroupingsym();
    virtual ~AttStaffgroupingsym();

    /** Reset the default values for the attribute class **/
    void ResetStaffgroupingsym();

    /** Read the values for the attribute class **/
    bool ReadStaffgroupingsym(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffgroupingsym(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSymbol(staffgroupingsym_SYMBOL symbol_) { m_symbol = symbol_; }
    staffgroupingsym_SYMBOL GetSymbol() const { return m_symbol; }
    bool HasSymbol() const;
    ///@}

private:
    /** Specifies the symbol used to group a set of staves. **/
    staffgroupingsym_SYMBOL m_symbol;

    /* include <attsymbol> */
};

//----------------------------------------------------------------------------
// AttStaffident
//----------------------------------------------------------------------------

class AttStaffident : public Att {
public:
    AttStaffident();
    virtual ~AttStaffident();

    /** Reset the default values for the attribute class **/
    void ResetStaffident();

    /** Read the values for the attribute class **/
    bool ReadStaffident(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffident(pugi::xml_node element);

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
// AttStaffloc
//----------------------------------------------------------------------------

class AttStaffloc : public Att {
public:
    AttStaffloc();
    virtual ~AttStaffloc();

    /** Reset the default values for the attribute class **/
    void ResetStaffloc();

    /** Read the values for the attribute class **/
    bool ReadStaffloc(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStaffloc(pugi::xml_node element);

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
// AttStafflocPitched
//----------------------------------------------------------------------------

class AttStafflocPitched : public Att {
public:
    AttStafflocPitched();
    virtual ~AttStafflocPitched();

    /** Reset the default values for the attribute class **/
    void ResetStafflocPitched();

    /** Read the values for the attribute class **/
    bool ReadStafflocPitched(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStafflocPitched(pugi::xml_node element);

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
    void SetOloc(char oloc_) { m_oloc = oloc_; }
    char GetOloc() const { return m_oloc; }
    bool HasOloc() const;
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

class AttStartendid : public Att {
public:
    AttStartendid();
    virtual ~AttStartendid();

    /** Reset the default values for the attribute class **/
    void ResetStartendid();

    /** Read the values for the attribute class **/
    bool ReadStartendid(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStartendid(pugi::xml_node element);

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
// AttStartid
//----------------------------------------------------------------------------

class AttStartid : public Att {
public:
    AttStartid();
    virtual ~AttStartid();

    /** Reset the default values for the attribute class **/
    void ResetStartid();

    /** Read the values for the attribute class **/
    bool ReadStartid(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStartid(pugi::xml_node element);

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
    void SetStemLen(int stemLen_) { m_stemLen = stemLen_; }
    int GetStemLen() const { return m_stemLen; }
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
    int m_stemLen;
    /**
     * Encodes any stem "modifiers"; that is, symbols rendered on the stem, such as
     * tremolo or Sprechstimme indicators.
     **/
    data_STEMMODIFIER m_stemMod;
    /** Records the position of the stem in relation to the note head(s). **/
    data_STEMPOSITION m_stemPos;
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
     * Describes the symbols typically used to indicate breaks between syllables and
     * their functions.
     **/
    sylLog_CON m_con;
    /** Records the position of a syllable within a word. **/
    sylLog_WORDPOS m_wordpos;

    /* include <attwordpos> */
};

//----------------------------------------------------------------------------
// AttSyltext
//----------------------------------------------------------------------------

class AttSyltext : public Att {
public:
    AttSyltext();
    virtual ~AttSyltext();

    /** Reset the default values for the attribute class **/
    void ResetSyltext();

    /** Read the values for the attribute class **/
    bool ReadSyltext(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSyltext(pugi::xml_node element);

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
    void SetSystemLeftmar(std::string systemLeftmar_) { m_systemLeftmar = systemLeftmar_; }
    std::string GetSystemLeftmar() const { return m_systemLeftmar; }
    bool HasSystemLeftmar() const;
    //
    void SetSystemRightmar(std::string systemRightmar_) { m_systemRightmar = systemRightmar_; }
    std::string GetSystemRightmar() const { return m_systemRightmar; }
    bool HasSystemRightmar() const;
    //
    void SetSystemTopmar(std::string systemTopmar_) { m_systemTopmar = systemTopmar_; }
    std::string GetSystemTopmar() const { return m_systemTopmar; }
    bool HasSystemTopmar() const;
    ///@}

private:
    /**
     * Indicates whether the staves are joined at the left by a continuous line.
     * The default value is "true". Do not confuse this with the heavy vertical line
     * used as a grouping symbol.
     **/
    data_BOOLEAN m_systemLeftline;
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

    /* include <attsystem.topmar> */
};

//----------------------------------------------------------------------------
// AttTargeteval
//----------------------------------------------------------------------------

class AttTargeteval : public Att {
public:
    AttTargeteval();
    virtual ~AttTargeteval();

    /** Reset the default values for the attribute class **/
    void ResetTargeteval();

    /** Read the values for the attribute class **/
    bool ReadTargeteval(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTargeteval(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetEvaluate(targeteval_EVALUATE evaluate_) { m_evaluate = evaluate_; }
    targeteval_EVALUATE GetEvaluate() const { return m_evaluate; }
    bool HasEvaluate() const;
    ///@}

private:
    /**
     * Specifies the intended meaning when a participant in a relationship is itself a
     * pointer.
     **/
    targeteval_EVALUATE m_evaluate;

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
// AttTextstyle
//----------------------------------------------------------------------------

class AttTextstyle : public Att {
public:
    AttTextstyle();
    virtual ~AttTextstyle();

    /** Reset the default values for the attribute class **/
    void ResetTextstyle();

    /** Read the values for the attribute class **/
    bool ReadTextstyle(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTextstyle(pugi::xml_node element);

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
    void SetTextSize(int textSize_) { m_textSize = textSize_; }
    int GetTextSize() const { return m_textSize; }
    bool HasTextSize() const;
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
    int m_textSize;
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
// AttTiepresent
//----------------------------------------------------------------------------

class AttTiepresent : public Att {
public:
    AttTiepresent();
    virtual ~AttTiepresent();

    /** Reset the default values for the attribute class **/
    void ResetTiepresent();

    /** Read the values for the attribute class **/
    bool ReadTiepresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTiepresent(pugi::xml_node element);

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
     * If visual information about the tie needs to be recorded, then a <tie> element
     * should be employed.
     **/
    data_TIE m_tie;

    /* include <atttie> */
};

//----------------------------------------------------------------------------
// AttTimestampMusical
//----------------------------------------------------------------------------

class AttTimestampMusical : public Att {
public:
    AttTimestampMusical();
    virtual ~AttTimestampMusical();

    /** Reset the default values for the attribute class **/
    void ResetTimestampMusical();

    /** Read the values for the attribute class **/
    bool ReadTimestampMusical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTimestampMusical(pugi::xml_node element);

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
// AttTimestampPerformed
//----------------------------------------------------------------------------

class AttTimestampPerformed : public Att {
public:
    AttTimestampPerformed();
    virtual ~AttTimestampPerformed();

    /** Reset the default values for the attribute class **/
    void ResetTimestampPerformed();

    /** Read the values for the attribute class **/
    bool ReadTimestampPerformed(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTimestampPerformed(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTstampGes(data_DURATION tstampGes_) { m_tstampGes = tstampGes_; }
    data_DURATION GetTstampGes() const { return m_tstampGes; }
    bool HasTstampGes() const;
    //
    void SetTstampReal(std::string tstampReal_) { m_tstampReal = tstampReal_; }
    std::string GetTstampReal() const { return m_tstampReal; }
    bool HasTstampReal() const;
    ///@}

private:
    /**
     * Captures performed onset time in several forms; that is, ppq (MIDI clicks and
     * MusicXML 'divisions'), Humdrum **recip values, beats, seconds, or mensural
     * duration values.
     **/
    data_DURATION m_tstampGes;
    /** Used to record the onset time in terms of ISO time. **/
    std::string m_tstampReal;

    /* include <atttstamp.real> */
};

//----------------------------------------------------------------------------
// AttTimestamp2Musical
//----------------------------------------------------------------------------

class AttTimestamp2Musical : public Att {
public:
    AttTimestamp2Musical();
    virtual ~AttTimestamp2Musical();

    /** Reset the default values for the attribute class **/
    void ResetTimestamp2Musical();

    /** Read the values for the attribute class **/
    bool ReadTimestamp2Musical(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTimestamp2Musical(pugi::xml_node element);

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
// AttTupletpresent
//----------------------------------------------------------------------------

class AttTupletpresent : public Att {
public:
    AttTupletpresent();
    virtual ~AttTupletpresent();

    /** Reset the default values for the attribute class **/
    void ResetTupletpresent();

    /** Read the values for the attribute class **/
    bool ReadTupletpresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTupletpresent(pugi::xml_node element);

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
     * If visual information about the tuplet needs to be recorded, then a <tuplet>
     * element should be employed.
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
    //
    void SetSubtype(std::string subtype_) { m_subtype = subtype_; }
    std::string GetSubtype() const { return m_subtype; }
    bool HasSubtype() const;
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
// AttVisualoffsetHo
//----------------------------------------------------------------------------

class AttVisualoffsetHo : public Att {
public:
    AttVisualoffsetHo();
    virtual ~AttVisualoffsetHo();

    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetHo();

    /** Read the values for the attribute class **/
    bool ReadVisualoffsetHo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualoffsetHo(pugi::xml_node element);

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
// AttVisualoffsetTo
//----------------------------------------------------------------------------

class AttVisualoffsetTo : public Att {
public:
    AttVisualoffsetTo();
    virtual ~AttVisualoffsetTo();

    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetTo();

    /** Read the values for the attribute class **/
    bool ReadVisualoffsetTo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualoffsetTo(pugi::xml_node element);

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
// AttVisualoffsetVo
//----------------------------------------------------------------------------

class AttVisualoffsetVo : public Att {
public:
    AttVisualoffsetVo();
    virtual ~AttVisualoffsetVo();

    /** Reset the default values for the attribute class **/
    void ResetVisualoffsetVo();

    /** Read the values for the attribute class **/
    bool ReadVisualoffsetVo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualoffsetVo(pugi::xml_node element);

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
// AttVisualoffset2Ho
//----------------------------------------------------------------------------

class AttVisualoffset2Ho : public Att {
public:
    AttVisualoffset2Ho();
    virtual ~AttVisualoffset2Ho();

    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2Ho();

    /** Read the values for the attribute class **/
    bool ReadVisualoffset2Ho(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualoffset2Ho(pugi::xml_node element);

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
// AttVisualoffset2To
//----------------------------------------------------------------------------

class AttVisualoffset2To : public Att {
public:
    AttVisualoffset2To();
    virtual ~AttVisualoffset2To();

    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2To();

    /** Read the values for the attribute class **/
    bool ReadVisualoffset2To(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualoffset2To(pugi::xml_node element);

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
// AttVisualoffset2Vo
//----------------------------------------------------------------------------

class AttVisualoffset2Vo : public Att {
public:
    AttVisualoffset2Vo();
    virtual ~AttVisualoffset2Vo();

    /** Reset the default values for the attribute class **/
    void ResetVisualoffset2Vo();

    /** Read the values for the attribute class **/
    bool ReadVisualoffset2Vo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVisualoffset2Vo(pugi::xml_node element);

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
    /** Width of the line. **/
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
     * Encodes an y coordinate for a feature in an output coordinate system.
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
