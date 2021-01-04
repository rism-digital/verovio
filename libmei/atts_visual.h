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

#ifndef __VRV_ATTS_VISUAL_H__
#define __VRV_ATTS_VISUAL_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttAnnotVis
//----------------------------------------------------------------------------

class AttAnnotVis : public Att {
public:
    AttAnnotVis();
    virtual ~AttAnnotVis();

    /** Reset the default values for the attribute class **/
    void ResetAnnotVis();

    /** Read the values for the attribute class **/
    bool ReadAnnotVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAnnotVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPlace(data_PLACEMENT place_) { m_place = place_; }
    data_PLACEMENT GetPlace() const { return m_place; }
    bool HasPlace() const;
    /** Getter for reference (for alternate type only) */
    data_PLACEMENT *GetPlaceAlternate() { return &m_place; }
    ///@}

private:
    /** Records the placement of the beam relative to the events it affects. **/
    data_PLACEMENT m_place;

    /* include <attplace> */
};

//----------------------------------------------------------------------------
// AttArpegVis
//----------------------------------------------------------------------------

class AttArpegVis : public Att {
public:
    AttArpegVis();
    virtual ~AttArpegVis();

    /** Reset the default values for the attribute class **/
    void ResetArpegVis();

    /** Read the values for the attribute class **/
    bool ReadArpegVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteArpegVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetArrow(data_BOOLEAN arrow_) { m_arrow = arrow_; }
    data_BOOLEAN GetArrow() const { return m_arrow; }
    bool HasArrow() const;
    //
    void SetArrowShape(data_LINESTARTENDSYMBOL arrowShape_) { m_arrowShape = arrowShape_; }
    data_LINESTARTENDSYMBOL GetArrowShape() const { return m_arrowShape; }
    bool HasArrowShape() const;
    //
    void SetArrowSize(int arrowSize_) { m_arrowSize = arrowSize_; }
    int GetArrowSize() const { return m_arrowSize; }
    bool HasArrowSize() const;
    //
    void SetArrowColor(std::string arrowColor_) { m_arrowColor = arrowColor_; }
    std::string GetArrowColor() const { return m_arrowColor; }
    bool HasArrowColor() const;
    //
    void SetArrowFillcolor(std::string arrowFillcolor_) { m_arrowFillcolor = arrowFillcolor_; }
    std::string GetArrowFillcolor() const { return m_arrowFillcolor; }
    bool HasArrowFillcolor() const;
    //
    void SetLineForm(data_LINEFORM lineForm_) { m_lineForm = lineForm_; }
    data_LINEFORM GetLineForm() const { return m_lineForm; }
    bool HasLineForm() const;
    //
    void SetLineWidth(data_LINEWIDTH lineWidth_) { m_lineWidth = lineWidth_; }
    data_LINEWIDTH GetLineWidth() const { return m_lineWidth; }
    bool HasLineWidth() const;
    /** Getter for reference (for alternate type only) */
    data_LINEWIDTH *GetLineWidthAlternate() { return &m_lineWidth; }
    ///@}

private:
    /** Indicates if an arrowhead is to be drawn as part of the arpeggiation symbol. **/
    data_BOOLEAN m_arrow;
    /** Symbol rendered at end of the line. **/
    data_LINESTARTENDSYMBOL m_arrowShape;
    /** Holds the relative size of the arrow symbol. **/
    int m_arrowSize;
    /** Captures the overall color of the arrow. **/
    std::string m_arrowColor;
    /** Captures the fill color of the arrow if different from the line color. **/
    std::string m_arrowFillcolor;
    /** Visual form of the line. **/
    data_LINEFORM m_lineForm;
    /** Width of the line. **/
    data_LINEWIDTH m_lineWidth;

    /* include <attline.width> */
};

//----------------------------------------------------------------------------
// AttBarLineVis
//----------------------------------------------------------------------------

class AttBarLineVis : public Att {
public:
    AttBarLineVis();
    virtual ~AttBarLineVis();

    /** Reset the default values for the attribute class **/
    void ResetBarLineVis();

    /** Read the values for the attribute class **/
    bool ReadBarLineVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBarLineVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLen(double len_) { m_len = len_; }
    double GetLen() const { return m_len; }
    bool HasLen() const;
    //
    void SetMethod(data_BARMETHOD method_) { m_method = method_; }
    data_BARMETHOD GetMethod() const { return m_method; }
    bool HasMethod() const;
    //
    void SetPlace(int place_) { m_place = place_; }
    int GetPlace() const { return m_place; }
    bool HasPlace() const;
    ///@}

private:
    /** Encodes the stem length. **/
    double m_len;
    /** Indicates the method employed to mark corrections and normalizations. **/
    data_BARMETHOD m_method;
    /** Records the placement of the beam relative to the events it affects. **/
    int m_place;

    /* include <attplace> */
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
// AttBeatRptVis
//----------------------------------------------------------------------------

class AttBeatRptVis : public Att {
public:
    AttBeatRptVis();
    virtual ~AttBeatRptVis();

    /** Reset the default values for the attribute class **/
    void ResetBeatRptVis();

    /** Read the values for the attribute class **/
    bool ReadBeatRptVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBeatRptVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSlash(data_BEATRPT_REND slash_) { m_slash = slash_; }
    data_BEATRPT_REND GetSlash() const { return m_slash; }
    bool HasSlash() const;
    ///@}

private:
    /** Indicates presence of slash through the beam. **/
    data_BEATRPT_REND m_slash;

    /* include <attslash> */
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
// AttEpisemaVis
//----------------------------------------------------------------------------

class AttEpisemaVis : public Att {
public:
    AttEpisemaVis();
    virtual ~AttEpisemaVis();

    /** Reset the default values for the attribute class **/
    void ResetEpisemaVis();

    /** Read the values for the attribute class **/
    bool ReadEpisemaVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteEpisemaVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(episemaVis_FORM form_) { m_form = form_; }
    episemaVis_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetPlace(data_EVENTREL place_) { m_place = place_; }
    data_EVENTREL GetPlace() const { return m_place; }
    bool HasPlace() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    episemaVis_FORM m_form;
    /** Records the placement of the beam relative to the events it affects. **/
    data_EVENTREL m_place;

    /* include <attplace> */
};

//----------------------------------------------------------------------------
// AttFTremVis
//----------------------------------------------------------------------------

class AttFTremVis : public Att {
public:
    AttFTremVis();
    virtual ~AttFTremVis();

    /** Reset the default values for the attribute class **/
    void ResetFTremVis();

    /** Read the values for the attribute class **/
    bool ReadFTremVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteFTremVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBeams(int beams_) { m_beams = beams_; }
    int GetBeams() const { return m_beams; }
    bool HasBeams() const;
    //
    void SetBeamsFloat(int beamsFloat_) { m_beamsFloat = beamsFloat_; }
    int GetBeamsFloat() const { return m_beamsFloat; }
    bool HasBeamsFloat() const;
    //
    void SetFloatGap(data_MEASUREMENTABS floatGap_) { m_floatGap = floatGap_; }
    data_MEASUREMENTABS GetFloatGap() const { return m_floatGap; }
    bool HasFloatGap() const;
    ///@}

private:
    /** Indicates the number of beams present. **/
    int m_beams;
    /** Captures the number of "floating" beams, i.e., those not attached to stems. **/
    int m_beamsFloat;
    /** Records the amount of separation between floating beams and stems. **/
    data_MEASUREMENTABS m_floatGap;

    /* include <attfloat.gap> */
};

//----------------------------------------------------------------------------
// AttFermataVis
//----------------------------------------------------------------------------

class AttFermataVis : public Att {
public:
    AttFermataVis();
    virtual ~AttFermataVis();

    /** Reset the default values for the attribute class **/
    void ResetFermataVis();

    /** Read the values for the attribute class **/
    bool ReadFermataVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteFermataVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(fermataVis_FORM form_) { m_form = form_; }
    fermataVis_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetShape(fermataVis_SHAPE shape_) { m_shape = shape_; }
    fermataVis_SHAPE GetShape() const { return m_shape; }
    bool HasShape() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    fermataVis_FORM m_form;
    /** Describes a clef's shape. **/
    fermataVis_SHAPE m_shape;

    /* include <attshape> */
};

//----------------------------------------------------------------------------
// AttFingGrpVis
//----------------------------------------------------------------------------

class AttFingGrpVis : public Att {
public:
    AttFingGrpVis();
    virtual ~AttFingGrpVis();

    /** Reset the default values for the attribute class **/
    void ResetFingGrpVis();

    /** Read the values for the attribute class **/
    bool ReadFingGrpVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteFingGrpVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOrient(fingGrpVis_ORIENT orient_) { m_orient = orient_; }
    fingGrpVis_ORIENT GetOrient() const { return m_orient; }
    bool HasOrient() const;
    ///@}

private:
    /** Describes the rotation or reflection of the base symbol. **/
    fingGrpVis_ORIENT m_orient;

    /* include <attorient> */
};

//----------------------------------------------------------------------------
// AttHairpinVis
//----------------------------------------------------------------------------

class AttHairpinVis : public Att {
public:
    AttHairpinVis();
    virtual ~AttHairpinVis();

    /** Reset the default values for the attribute class **/
    void ResetHairpinVis();

    /** Read the values for the attribute class **/
    bool ReadHairpinVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHairpinVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOpening(data_MEASUREMENTABS opening_) { m_opening = opening_; }
    data_MEASUREMENTABS GetOpening() const { return m_opening; }
    bool HasOpening() const;
    ///@}

private:
    /**
     * Specifies the distance between the lines at the open end of a hairpin dynamic
     * mark.
     **/
    data_MEASUREMENTABS m_opening;

    /* include <attopening> */
};

//----------------------------------------------------------------------------
// AttHarmVis
//----------------------------------------------------------------------------

class AttHarmVis : public Att {
public:
    AttHarmVis();
    virtual ~AttHarmVis();

    /** Reset the default values for the attribute class **/
    void ResetHarmVis();

    /** Read the values for the attribute class **/
    bool ReadHarmVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHarmVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetRendgrid(harmVis_RENDGRID rendgrid_) { m_rendgrid = rendgrid_; }
    harmVis_RENDGRID GetRendgrid() const { return m_rendgrid; }
    bool HasRendgrid() const;
    ///@}

private:
    /** Describes how the harmonic indication should be rendered. **/
    harmVis_RENDGRID m_rendgrid;

    /* include <attrendgrid> */
};

//----------------------------------------------------------------------------
// AttHispanTickVis
//----------------------------------------------------------------------------

class AttHispanTickVis : public Att {
public:
    AttHispanTickVis();
    virtual ~AttHispanTickVis();

    /** Reset the default values for the attribute class **/
    void ResetHispanTickVis();

    /** Read the values for the attribute class **/
    bool ReadHispanTickVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHispanTickVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPlace(data_EVENTREL place_) { m_place = place_; }
    data_EVENTREL GetPlace() const { return m_place; }
    bool HasPlace() const;
    //
    void SetTilt(data_COMPASSDIRECTION tilt_) { m_tilt = tilt_; }
    data_COMPASSDIRECTION GetTilt() const { return m_tilt; }
    bool HasTilt() const;
    ///@}

private:
    /** Records the placement of the beam relative to the events it affects. **/
    data_EVENTREL m_place;
    /** Direction of the pen stroke. **/
    data_COMPASSDIRECTION m_tilt;

    /* include <atttilt> */
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
    void SetKeysigShow(data_BOOLEAN keysigShow_) { m_keysigShow = keysigShow_; }
    data_BOOLEAN GetKeysigShow() const { return m_keysigShow; }
    bool HasKeysigShow() const;
    //
    void SetKeysigShowchange(data_BOOLEAN keysigShowchange_) { m_keysigShowchange = keysigShowchange_; }
    data_BOOLEAN GetKeysigShowchange() const { return m_keysigShowchange; }
    bool HasKeysigShowchange() const;
    ///@}

private:
    /** Indicates whether the key signature should be displayed. **/
    data_BOOLEAN m_keysigShow;
    /** Determines whether cautionary accidentals should be displayed at a key change. **/
    data_BOOLEAN m_keysigShowchange;

    /* include <attkeysig.showchange> */
};

//----------------------------------------------------------------------------
// AttLigatureVis
//----------------------------------------------------------------------------

class AttLigatureVis : public Att {
public:
    AttLigatureVis();
    virtual ~AttLigatureVis();

    /** Reset the default values for the attribute class **/
    void ResetLigatureVis();

    /** Read the values for the attribute class **/
    bool ReadLigatureVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLigatureVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(data_LIGATUREFORM form_) { m_form = form_; }
    data_LIGATUREFORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    data_LIGATUREFORM m_form;

    /* include <attform> */
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
    void SetWidth(data_LINEWIDTH width_) { m_width = width_; }
    data_LINEWIDTH GetWidth() const { return m_width; }
    bool HasWidth() const;
    /** Getter for reference (for alternate type only) */
    data_LINEWIDTH *GetWidthAlternate() { return &m_width; }
    //
    void SetEndsym(data_LINESTARTENDSYMBOL endsym_) { m_endsym = endsym_; }
    data_LINESTARTENDSYMBOL GetEndsym() const { return m_endsym; }
    bool HasEndsym() const;
    //
    void SetEndsymSize(int endsymSize_) { m_endsymSize = endsymSize_; }
    int GetEndsymSize() const { return m_endsymSize; }
    bool HasEndsymSize() const;
    //
    void SetStartsym(data_LINESTARTENDSYMBOL startsym_) { m_startsym = startsym_; }
    data_LINESTARTENDSYMBOL GetStartsym() const { return m_startsym; }
    bool HasStartsym() const;
    //
    void SetStartsymSize(int startsymSize_) { m_startsymSize = startsymSize_; }
    int GetStartsymSize() const { return m_startsymSize; }
    bool HasStartsymSize() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    data_LINEFORM m_form;
    /** Measurement of the horizontal dimension of an entity. **/
    data_LINEWIDTH m_width;
    /** Symbol rendered at end of line. **/
    data_LINESTARTENDSYMBOL m_endsym;
    /** Holds the relative size of the line-end symbol. **/
    int m_endsymSize;
    /** Symbol rendered at start of line. **/
    data_LINESTARTENDSYMBOL m_startsym;
    /** Holds the relative size of the line-start symbol. **/
    int m_startsymSize;

    /* include <attstartsym.size> */
};

//----------------------------------------------------------------------------
// AttLiquescentVis
//----------------------------------------------------------------------------

class AttLiquescentVis : public Att {
public:
    AttLiquescentVis();
    virtual ~AttLiquescentVis();

    /** Reset the default values for the attribute class **/
    void ResetLiquescentVis();

    /** Read the values for the attribute class **/
    bool ReadLiquescentVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLiquescentVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCurve(liquescentVis_CURVE curve_) { m_curve = curve_; }
    liquescentVis_CURVE GetCurve() const { return m_curve; }
    bool HasCurve() const;
    //
    void SetLooped(data_BOOLEAN looped_) { m_looped = looped_; }
    data_BOOLEAN GetLooped() const { return m_looped; }
    bool HasLooped() const;
    ///@}

private:
    /** Records direction of curvature. **/
    liquescentVis_CURVE m_curve;
    /** Indicates whether curve is closed. **/
    data_BOOLEAN m_looped;

    /* include <attlooped> */
};

//----------------------------------------------------------------------------
// AttMensurVis
//----------------------------------------------------------------------------

class AttMensurVis : public Att {
public:
    AttMensurVis();
    virtual ~AttMensurVis();

    /** Reset the default values for the attribute class **/
    void ResetMensurVis();

    /** Read the values for the attribute class **/
    bool ReadMensurVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMensurVis(pugi::xml_node element);

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
    void SetForm(mensurVis_FORM form_) { m_form = form_; }
    mensurVis_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetOrient(data_ORIENTATION orient_) { m_orient = orient_; }
    data_ORIENTATION GetOrient() const { return m_orient; }
    bool HasOrient() const;
    //
    void SetSign(data_MENSURATIONSIGN sign_) { m_sign = sign_; }
    data_MENSURATIONSIGN GetSign() const { return m_sign; }
    bool HasSign() const;
    ///@}

private:
    /** Specifies whether a dot is to be added to the base symbol. **/
    data_BOOLEAN m_dot;
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    mensurVis_FORM m_form;
    /** Describes the rotation or reflection of the base symbol. **/
    data_ORIENTATION m_orient;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    data_MENSURATIONSIGN m_sign;

    /* include <attsign> */
};

//----------------------------------------------------------------------------
// AttMensuralVis
//----------------------------------------------------------------------------

class AttMensuralVis : public Att {
public:
    AttMensuralVis();
    virtual ~AttMensuralVis();

    /** Reset the default values for the attribute class **/
    void ResetMensuralVis();

    /** Read the values for the attribute class **/
    bool ReadMensuralVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMensuralVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMensurColor(std::string mensurColor_) { m_mensurColor = mensurColor_; }
    std::string GetMensurColor() const { return m_mensurColor; }
    bool HasMensurColor() const;
    //
    void SetMensurDot(data_BOOLEAN mensurDot_) { m_mensurDot = mensurDot_; }
    data_BOOLEAN GetMensurDot() const { return m_mensurDot; }
    bool HasMensurDot() const;
    //
    void SetMensurForm(mensuralVis_MENSURFORM mensurForm_) { m_mensurForm = mensurForm_; }
    mensuralVis_MENSURFORM GetMensurForm() const { return m_mensurForm; }
    bool HasMensurForm() const;
    //
    void SetMensurLoc(int mensurLoc_) { m_mensurLoc = mensurLoc_; }
    int GetMensurLoc() const { return m_mensurLoc; }
    bool HasMensurLoc() const;
    //
    void SetMensurOrient(data_ORIENTATION mensurOrient_) { m_mensurOrient = mensurOrient_; }
    data_ORIENTATION GetMensurOrient() const { return m_mensurOrient; }
    bool HasMensurOrient() const;
    //
    void SetMensurSign(data_MENSURATIONSIGN mensurSign_) { m_mensurSign = mensurSign_; }
    data_MENSURATIONSIGN GetMensurSign() const { return m_mensurSign; }
    bool HasMensurSign() const;
    //
    void SetMensurSize(data_FONTSIZE mensurSize_) { m_mensurSize = mensurSize_; }
    data_FONTSIZE GetMensurSize() const { return m_mensurSize; }
    bool HasMensurSize() const;
    /** Getter for reference (for alternate type only) */
    data_FONTSIZE *GetMensurSizeAlternate() { return &m_mensurSize; }
    //
    void SetMensurSlash(int mensurSlash_) { m_mensurSlash = mensurSlash_; }
    int GetMensurSlash() const { return m_mensurSlash; }
    bool HasMensurSlash() const;
    ///@}

private:
    /**
     * Records the color of the mensuration sign.
     * Do not confuse this with the musical term 'color' as used in pre-CMN notation.
     **/
    std::string m_mensurColor;
    /** Determines if a dot is to be added to the base symbol. **/
    data_BOOLEAN m_mensurDot;
    /** Indicates whether the base symbol is written vertically or horizontally. **/
    mensuralVis_MENSURFORM m_mensurForm;
    /** Holds the staff location of the mensuration sign. **/
    int m_mensurLoc;
    /** Describes the rotation or reflection of the base symbol. **/
    data_ORIENTATION m_mensurOrient;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    data_MENSURATIONSIGN m_mensurSign;
    /** Describes the relative size of the mensuration sign. **/
    data_FONTSIZE m_mensurSize;
    /**
     * Indicates the number lines added to the mensuration sign.
     * For example, one slash is added for what we now call 'alla breve'.
     **/
    int m_mensurSlash;

    /* include <attmensur.slash> */
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
    void SetForm(data_METERFORM form_) { m_form = form_; }
    data_METERFORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    data_METERFORM m_form;

    /* include <attform> */
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
    void SetMeterForm(data_METERFORM meterForm_) { m_meterForm = meterForm_; }
    data_METERFORM GetMeterForm() const { return m_meterForm; }
    bool HasMeterForm() const;
    //
    void SetMeterShowchange(data_BOOLEAN meterShowchange_) { m_meterShowchange = meterShowchange_; }
    data_BOOLEAN GetMeterShowchange() const { return m_meterShowchange; }
    bool HasMeterShowchange() const;
    ///@}

private:
    /** Contains an indication of how the meter signature should be rendered. **/
    data_METERFORM m_meterForm;
    /**
     * Determines whether the old meter signature should be displayed when the meter
     * signature changes.
     **/
    data_BOOLEAN m_meterShowchange;

    /* include <attmeter.showchange> */
};

//----------------------------------------------------------------------------
// AttMultiRestVis
//----------------------------------------------------------------------------

class AttMultiRestVis : public Att {
public:
    AttMultiRestVis();
    virtual ~AttMultiRestVis();

    /** Reset the default values for the attribute class **/
    void ResetMultiRestVis();

    /** Read the values for the attribute class **/
    bool ReadMultiRestVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMultiRestVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBlock(data_BOOLEAN block_) { m_block = block_; }
    data_BOOLEAN GetBlock() const { return m_block; }
    bool HasBlock() const;
    ///@}

private:
    /**
     * The block attribute controls whether the multimeasure rest should be rendered as
     * a block rest or as church rests ("Kirchenpausen"), that are combinations of
     * longa, breve and semibreve rests.
     **/
    data_BOOLEAN m_block;

    /* include <attblock> */
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
     * the
     **/
    pbVis_FOLIUM m_folium;

    /* include <attfolium> */
};

//----------------------------------------------------------------------------
// AttPedalVis
//----------------------------------------------------------------------------

class AttPedalVis : public Att {
public:
    AttPedalVis();
    virtual ~AttPedalVis();

    /** Reset the default values for the attribute class **/
    void ResetPedalVis();

    /** Read the values for the attribute class **/
    bool ReadPedalVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePedalVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(pedalVis_FORM form_) { m_form = form_; }
    pedalVis_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    pedalVis_FORM m_form;

    /* include <attform> */
};

//----------------------------------------------------------------------------
// AttQuilismaVis
//----------------------------------------------------------------------------

class AttQuilismaVis : public Att {
public:
    AttQuilismaVis();
    virtual ~AttQuilismaVis();

    /** Reset the default values for the attribute class **/
    void ResetQuilismaVis();

    /** Read the values for the attribute class **/
    bool ReadQuilismaVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteQuilismaVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetWaves(int waves_) { m_waves = waves_; }
    int GetWaves() const { return m_waves; }
    bool HasWaves() const;
    ///@}

private:
    /** Number of "crests" of a wavy line. **/
    int m_waves;

    /* include <attwaves> */
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
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    sbVis_FORM m_form;

    /* include <attform> */
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
// AttSignifLetVis
//----------------------------------------------------------------------------

class AttSignifLetVis : public Att {
public:
    AttSignifLetVis();
    virtual ~AttSignifLetVis();

    /** Reset the default values for the attribute class **/
    void ResetSignifLetVis();

    /** Read the values for the attribute class **/
    bool ReadSignifLetVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSignifLetVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPlace(data_EVENTREL place_) { m_place = place_; }
    data_EVENTREL GetPlace() const { return m_place; }
    bool HasPlace() const;
    ///@}

private:
    /** Records the placement of the beam relative to the events it affects. **/
    data_EVENTREL m_place;

    /* include <attplace> */
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
    void SetLinesColor(std::string linesColor_) { m_linesColor = linesColor_; }
    std::string GetLinesColor() const { return m_linesColor; }
    bool HasLinesColor() const;
    //
    void SetLinesVisible(data_BOOLEAN linesVisible_) { m_linesVisible = linesVisible_; }
    data_BOOLEAN GetLinesVisible() const { return m_linesVisible; }
    bool HasLinesVisible() const;
    //
    void SetSpacing(data_MEASUREMENTREL spacing_) { m_spacing = spacing_; }
    data_MEASUREMENTREL GetSpacing() const { return m_spacing; }
    bool HasSpacing() const;
    ///@}

private:
    /** Determines whether to display guitar chord grids. **/
    data_BOOLEAN m_gridShow;
    /** Indicates the number of layers and their stem directions. **/
    data_LAYERSCHEME m_layerscheme;
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
    /** Records the absolute distance (as opposed to the relative distances recorded in **/
    data_MEASUREMENTREL m_spacing;

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
    void SetBarThru(data_BOOLEAN barThru_) { m_barThru = barThru_; }
    data_BOOLEAN GetBarThru() const { return m_barThru; }
    bool HasBarThru() const;
    ///@}

private:
    /**
     * Indicates whether bar lines go across the space between staves (true) or are
     * only drawn across the lines of each staff (false).
     **/
    data_BOOLEAN m_barThru;

    /* include <attbar.thru> */
};

//----------------------------------------------------------------------------
// AttTupletVis
//----------------------------------------------------------------------------

class AttTupletVis : public Att {
public:
    AttTupletVis();
    virtual ~AttTupletVis();

    /** Reset the default values for the attribute class **/
    void ResetTupletVis();

    /** Read the values for the attribute class **/
    bool ReadTupletVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTupletVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBracketPlace(data_STAFFREL_basic bracketPlace_) { m_bracketPlace = bracketPlace_; }
    data_STAFFREL_basic GetBracketPlace() const { return m_bracketPlace; }
    bool HasBracketPlace() const;
    //
    void SetBracketVisible(data_BOOLEAN bracketVisible_) { m_bracketVisible = bracketVisible_; }
    data_BOOLEAN GetBracketVisible() const { return m_bracketVisible; }
    bool HasBracketVisible() const;
    //
    void SetDurVisible(data_BOOLEAN durVisible_) { m_durVisible = durVisible_; }
    data_BOOLEAN GetDurVisible() const { return m_durVisible; }
    bool HasDurVisible() const;
    //
    void SetNumFormat(tupletVis_NUMFORMAT numFormat_) { m_numFormat = numFormat_; }
    tupletVis_NUMFORMAT GetNumFormat() const { return m_numFormat; }
    bool HasNumFormat() const;
    ///@}

private:
    /**
     * Used to state where a tuplet bracket will be placed in relation to the note
     * heads.
     **/
    data_STAFFREL_basic m_bracketPlace;
    /** States whether a bracket should be rendered with a tuplet. **/
    data_BOOLEAN m_bracketVisible;
    /** Determines if the tuplet duration is visible. **/
    data_BOOLEAN m_durVisible;
    /** Controls how the num:numbase ratio is to be displayed. **/
    tupletVis_NUMFORMAT m_numFormat;

    /* include <attnum.format> */
};

} // vrv namespace

#endif // __VRV_ATTS_VISUAL_H__
