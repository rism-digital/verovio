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

#ifndef __VRV_ATTS_CMN_H__
#define __VRV_ATTS_CMN_H__

#include "att.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttArpegLog
//----------------------------------------------------------------------------

class AttArpegLog: public Att
{
public:
    AttArpegLog();
    virtual ~AttArpegLog();
    
    /** Reset the default values for the attribute class **/
    void ResetArpegLog();
    
    /** Read the values for the attribute class **/
    bool ReadArpegLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteArpegLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetOrder(std::string order_) { m_order = order_; };
    std::string GetOrder() const { return m_order; };
    ///@}

protected:
    /** Describes the direction in which an arpeggio is to be performed. **/
    std::string m_order;

/* include <attorder> */
};

//----------------------------------------------------------------------------
// AttArpegVis
//----------------------------------------------------------------------------

class AttArpegVis: public Att
{
public:
    AttArpegVis();
    virtual ~AttArpegVis();
    
    /** Reset the default values for the attribute class **/
    void ResetArpegVis();
    
    /** Read the values for the attribute class **/
    bool ReadArpegVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteArpegVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetArrow(std::string arrow_) { m_arrow = arrow_; };
    std::string GetArrow() const { return m_arrow; };
    ///@}

protected:
    /** Indicates if an arrowhead is to be drawn as part of the arpeggiation symbol. **/
    std::string m_arrow;

/* include <attarrow> */
};

//----------------------------------------------------------------------------
// AttBTremLog
//----------------------------------------------------------------------------

class AttBTremLog: public Att
{
public:
    AttBTremLog();
    virtual ~AttBTremLog();
    
    /** Reset the default values for the attribute class **/
    void ResetBTremLog();
    
    /** Read the values for the attribute class **/
    bool ReadBTremLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBTremLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;

/* include <attform> */
};

//----------------------------------------------------------------------------
// AttBeamed
//----------------------------------------------------------------------------

class AttBeamed: public Att
{
public:
    AttBeamed();
    virtual ~AttBeamed();
    
    /** Reset the default values for the attribute class **/
    void ResetBeamed();
    
    /** Read the values for the attribute class **/
    bool ReadBeamed( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBeamed( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBeam(std::string beam_) { m_beam = beam_; };
    std::string GetBeam() const { return m_beam; };
    ///@}

protected:
    /** Indicates that this event is "under a beam". **/
    std::string m_beam;

/* include <attbeam> */
};

//----------------------------------------------------------------------------
// AttBeamedwith
//----------------------------------------------------------------------------

class AttBeamedwith: public Att
{
public:
    AttBeamedwith();
    virtual ~AttBeamedwith();
    
    /** Reset the default values for the attribute class **/
    void ResetBeamedwith();
    
    /** Read the values for the attribute class **/
    bool ReadBeamedwith( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBeamedwith( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBeamWith(std::string beamWith_) { m_beamWith = beamWith_; };
    std::string GetBeamWith() const { return m_beamWith; };
    ///@}

protected:
    /**
     * In the case of cross-staff beams, the beam.with attribute is used to indicate
     * which staff the beam is connected to; that is, the staff above or the staff
     * below.
     **/
    std::string m_beamWith;

/* include <attbeam.with> */
};

//----------------------------------------------------------------------------
// AttBeamingLog
//----------------------------------------------------------------------------

class AttBeamingLog: public Att
{
public:
    AttBeamingLog();
    virtual ~AttBeamingLog();
    
    /** Reset the default values for the attribute class **/
    void ResetBeamingLog();
    
    /** Read the values for the attribute class **/
    bool ReadBeamingLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBeamingLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBeamGroup(std::string beamGroup_) { m_beamGroup = beamGroup_; };
    std::string GetBeamGroup() const { return m_beamGroup; };
    //
    void SetBeamRests(std::string beamRests_) { m_beamRests = beamRests_; };
    std::string GetBeamRests() const { return m_beamRests; };
    ///@}

protected:
    /**
     * Provides an example of how automated beaming (including secondary beams) is to
     * be performed.
     **/
    std::string m_beamGroup;
    /**
     * Indicates whether automatically-drawn beams should include rests shorter than a
     * quarter note duration.
     **/
    std::string m_beamRests;

/* include <attbeam.rests> */
};

//----------------------------------------------------------------------------
// AttBeamrend
//----------------------------------------------------------------------------

class AttBeamrend: public Att
{
public:
    AttBeamrend();
    virtual ~AttBeamrend();
    
    /** Reset the default values for the attribute class **/
    void ResetBeamrend();
    
    /** Read the values for the attribute class **/
    bool ReadBeamrend( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBeamrend( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRend(std::string rend_) { m_rend = rend_; };
    std::string GetRend() const { return m_rend; };
    //
    void SetSlopeDbl(double slopeDbl_) { m_slopeDbl = slopeDbl_; };
    double GetSlopeDbl() const { return m_slopeDbl; };
    ///@}

protected:
    /** Describes the line style of the curve. **/
    std::string m_rend;
    /** Records the slope of the beam. **/
    double m_slopeDbl;

/* include <attslope> */
};

//----------------------------------------------------------------------------
// AttBeamsecondary
//----------------------------------------------------------------------------

class AttBeamsecondary: public Att
{
public:
    AttBeamsecondary();
    virtual ~AttBeamsecondary();
    
    /** Reset the default values for the attribute class **/
    void ResetBeamsecondary();
    
    /** Read the values for the attribute class **/
    bool ReadBeamsecondary( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBeamsecondary( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBreaksecInt(int breaksecInt_) { m_breaksecInt = breaksecInt_; };
    int GetBreaksecInt() const { return m_breaksecInt; };
    ///@}

protected:
    /**
     * Presence of this attribute indicates that the secondary beam should be broken
     * following this note/chord.
     * The value of the attribute records the number of beams which should remain
     * unbroken.
     **/
    int m_breaksecInt;

/* include <attbreaksec> */
};

//----------------------------------------------------------------------------
// AttBeatRptVis
//----------------------------------------------------------------------------

class AttBeatRptVis: public Att
{
public:
    AttBeatRptVis();
    virtual ~AttBeatRptVis();
    
    /** Reset the default values for the attribute class **/
    void ResetBeatRptVis();
    
    /** Read the values for the attribute class **/
    bool ReadBeatRptVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBeatRptVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRend(std::string rend_) { m_rend = rend_; };
    std::string GetRend() const { return m_rend; };
    ///@}

protected:
    /** Describes the line style of the curve. **/
    std::string m_rend;

/* include <attrend> */
};

//----------------------------------------------------------------------------
// AttBendGes
//----------------------------------------------------------------------------

class AttBendGes: public Att
{
public:
    AttBendGes();
    virtual ~AttBendGes();
    
    /** Reset the default values for the attribute class **/
    void ResetBendGes();
    
    /** Read the values for the attribute class **/
    bool ReadBendGes( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteBendGes( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetAmount(std::string amount_) { m_amount = amount_; };
    std::string GetAmount() const { return m_amount; };
    ///@}

protected:
    /**
     * Records the amount of detuning.
     * The decimal values should be rendered as a fraction (or an integer plus a
     * fraction) along with the bend symbol.
     **/
    std::string m_amount;

/* include <attamount> */
};

//----------------------------------------------------------------------------
// AttCutout
//----------------------------------------------------------------------------

class AttCutout: public Att
{
public:
    AttCutout();
    virtual ~AttCutout();
    
    /** Reset the default values for the attribute class **/
    void ResetCutout();
    
    /** Read the values for the attribute class **/
    bool ReadCutout( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCutout( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCutout(std::string cutout_) { m_cutout = cutout_; };
    std::string GetCutout() const { return m_cutout; };
    ///@}

protected:
    /** "Cut-out" style indicated for this measure. **/
    std::string m_cutout;

/* include <attcutout> */
};

//----------------------------------------------------------------------------
// AttExpandable
//----------------------------------------------------------------------------

class AttExpandable: public Att
{
public:
    AttExpandable();
    virtual ~AttExpandable();
    
    /** Reset the default values for the attribute class **/
    void ResetExpandable();
    
    /** Read the values for the attribute class **/
    bool ReadExpandable( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteExpandable( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetExpand(std::string expand_) { m_expand = expand_; };
    std::string GetExpand() const { return m_expand; };
    ///@}

protected:
    /**
     * Indicates whether to render a repeat symbol or the source material to which it
     * refers.
     * A value of 'true' renders the source material, while 'false' displays the repeat
     * symbol.
     **/
    std::string m_expand;

/* include <attexpand> */
};

//----------------------------------------------------------------------------
// AttFTremLog
//----------------------------------------------------------------------------

class AttFTremLog: public Att
{
public:
    AttFTremLog();
    virtual ~AttFTremLog();
    
    /** Reset the default values for the attribute class **/
    void ResetFTremLog();
    
    /** Read the values for the attribute class **/
    bool ReadFTremLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteFTremLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;

/* include <attform> */
};

//----------------------------------------------------------------------------
// AttFermataVis
//----------------------------------------------------------------------------

class AttFermataVis: public Att
{
public:
    AttFermataVis();
    virtual ~AttFermataVis();
    
    /** Reset the default values for the attribute class **/
    void ResetFermataVis();
    
    /** Read the values for the attribute class **/
    bool ReadFermataVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteFermataVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };
    //
    void SetShape(std::string shape_) { m_shape = shape_; };
    std::string GetShape() const { return m_shape; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;
    /** Describes a clef's shape. **/
    std::string m_shape;

/* include <attshape> */
};

//----------------------------------------------------------------------------
// AttGlissVis
//----------------------------------------------------------------------------

class AttGlissVis: public Att
{
public:
    AttGlissVis();
    virtual ~AttGlissVis();
    
    /** Reset the default values for the attribute class **/
    void ResetGlissVis();
    
    /** Read the values for the attribute class **/
    bool ReadGlissVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteGlissVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetText(std::string text_) { m_text = text_; };
    std::string GetText() const { return m_text; };
    ///@}

protected:
    /** Records a text string, such as 'gliss', that accompanies the glissando mark. **/
    std::string m_text;

/* include <atttext> */
};

//----------------------------------------------------------------------------
// AttGraced
//----------------------------------------------------------------------------

class AttGraced: public Att
{
public:
    AttGraced();
    virtual ~AttGraced();
    
    /** Reset the default values for the attribute class **/
    void ResetGraced();
    
    /** Read the values for the attribute class **/
    bool ReadGraced( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteGraced( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetGrace(std::string grace_) { m_grace = grace_; };
    std::string GetGrace() const { return m_grace; };
    //
    void SetGraceTime(std::string graceTime_) { m_graceTime = graceTime_; };
    std::string GetGraceTime() const { return m_graceTime; };
    ///@}

protected:
    /**
     * Marks a note or chord as a "grace" (without a definitive written duration) and
     * records from which other note/chord it should "steal" time.
     **/
    std::string m_grace;
    /** Records the amount of time to be "stolen" from a non-grace note/chord. **/
    std::string m_graceTime;

/* include <attgrace.time> */
};

//----------------------------------------------------------------------------
// AttHairpinLog
//----------------------------------------------------------------------------

class AttHairpinLog: public Att
{
public:
    AttHairpinLog();
    virtual ~AttHairpinLog();
    
    /** Reset the default values for the attribute class **/
    void ResetHairpinLog();
    
    /** Read the values for the attribute class **/
    bool ReadHairpinLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteHairpinLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;

/* include <attform> */
};

//----------------------------------------------------------------------------
// AttHairpinVis
//----------------------------------------------------------------------------

class AttHairpinVis: public Att
{
public:
    AttHairpinVis();
    virtual ~AttHairpinVis();
    
    /** Reset the default values for the attribute class **/
    void ResetHairpinVis();
    
    /** Read the values for the attribute class **/
    bool ReadHairpinVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteHairpinVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetOpening(std::string opening_) { m_opening = opening_; };
    std::string GetOpening() const { return m_opening; };
    ///@}

protected:
    /**
     * Specifies the distance between the points of the open end of a hairpin dynamic
     * mark.
     **/
    std::string m_opening;

/* include <attopening> */
};

//----------------------------------------------------------------------------
// AttHarpPedalLog
//----------------------------------------------------------------------------

class AttHarpPedalLog: public Att
{
public:
    AttHarpPedalLog();
    virtual ~AttHarpPedalLog();
    
    /** Reset the default values for the attribute class **/
    void ResetHarpPedalLog();
    
    /** Read the values for the attribute class **/
    bool ReadHarpPedalLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteHarpPedalLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetC(std::string c_) { m_c = c_; };
    std::string GetC() const { return m_c; };
    //
    void SetD(std::string d_) { m_d = d_; };
    std::string GetD() const { return m_d; };
    //
    void SetE(std::string e_) { m_e = e_; };
    std::string GetE() const { return m_e; };
    //
    void SetF(std::string f_) { m_f = f_; };
    std::string GetF() const { return m_f; };
    //
    void SetG(std::string g_) { m_g = g_; };
    std::string GetG() const { return m_g; };
    //
    void SetA(std::string a_) { m_a = a_; };
    std::string GetA() const { return m_a; };
    //
    void SetB(std::string b_) { m_b = b_; };
    std::string GetB() const { return m_b; };
    ///@}

protected:
    /** Indicates the pedal setting for the harp's C strings. **/
    std::string m_c;
    /** Indicates the pedal setting for the harp's D strings. **/
    std::string m_d;
    /** Indicates the pedal setting for the harp's E strings. **/
    std::string m_e;
    /** Indicates the pedal setting for the harp's F strings. **/
    std::string m_f;
    /** Indicates the pedal setting for the harp's G strings. **/
    std::string m_g;
    /** Indicates the pedal setting for the harp's A strings. **/
    std::string m_a;
    /** Indicates the pedal setting for the harp's B strings. **/
    std::string m_b;

/* include <attb> */
};

//----------------------------------------------------------------------------
// AttLvpresent
//----------------------------------------------------------------------------

class AttLvpresent: public Att
{
public:
    AttLvpresent();
    virtual ~AttLvpresent();
    
    /** Reset the default values for the attribute class **/
    void ResetLvpresent();
    
    /** Read the values for the attribute class **/
    bool ReadLvpresent( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLvpresent( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLv(std::string lv_) { m_lv = lv_; };
    std::string GetLv() const { return m_lv; };
    ///@}

protected:
    /**
     * Indicates the attachment of an l.v.
     * (laissez vibrer) sign to this element.
     **/
    std::string m_lv;

/* include <attlv> */
};

//----------------------------------------------------------------------------
// AttMultiRestVis
//----------------------------------------------------------------------------

class AttMultiRestVis: public Att
{
public:
    AttMultiRestVis();
    virtual ~AttMultiRestVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMultiRestVis();
    
    /** Read the values for the attribute class **/
    bool ReadMultiRestVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMultiRestVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBlock(std::string block_) { m_block = block_; };
    std::string GetBlock() const { return m_block; };
    ///@}

protected:
    /**
     * When the block attribute is used, combinations of the 1, 2, and 4 measure rest
     * forms (Read, p.
     * 104) should be rendered instead of the modern form or an alternative symbol.
     **/
    std::string m_block;

/* include <attblock> */
};

//----------------------------------------------------------------------------
// AttNoteGesCmn
//----------------------------------------------------------------------------

class AttNoteGesCmn: public Att
{
public:
    AttNoteGesCmn();
    virtual ~AttNoteGesCmn();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteGesCmn();
    
    /** Read the values for the attribute class **/
    bool ReadNoteGesCmn( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteNoteGesCmn( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetGliss(std::string gliss_) { m_gliss = gliss_; };
    std::string GetGliss() const { return m_gliss; };
    ///@}

protected:
    /** Indicates that this element participates in a glissando. **/
    std::string m_gliss;

/* include <attgliss> */
};

//----------------------------------------------------------------------------
// AttNumbered
//----------------------------------------------------------------------------

class AttNumbered: public Att
{
public:
    AttNumbered();
    virtual ~AttNumbered();
    
    /** Reset the default values for the attribute class **/
    void ResetNumbered();
    
    /** Read the values for the attribute class **/
    bool ReadNumbered( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteNumbered( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetNumInt(int numInt_) { m_numInt = numInt_; };
    int GetNumInt() const { return m_numInt; };
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
// AttNumberplacement
//----------------------------------------------------------------------------

class AttNumberplacement: public Att
{
public:
    AttNumberplacement();
    virtual ~AttNumberplacement();
    
    /** Reset the default values for the attribute class **/
    void ResetNumberplacement();
    
    /** Read the values for the attribute class **/
    bool ReadNumberplacement( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteNumberplacement( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetNumPlace(std::string numPlace_) { m_numPlace = numPlace_; };
    std::string GetNumPlace() const { return m_numPlace; };
    //
    void SetNumVisible(std::string numVisible_) { m_numVisible = numVisible_; };
    std::string GetNumVisible() const { return m_numVisible; };
    ///@}

protected:
    /** States where the tuplet number will be placed in relation to the note heads. **/
    std::string m_numPlace;
    /** Determines if the tuplet number is visible. **/
    std::string m_numVisible;

/* include <attnum.visible> */
};

//----------------------------------------------------------------------------
// AttOctaveLog
//----------------------------------------------------------------------------

class AttOctaveLog: public Att
{
public:
    AttOctaveLog();
    virtual ~AttOctaveLog();
    
    /** Reset the default values for the attribute class **/
    void ResetOctaveLog();
    
    /** Read the values for the attribute class **/
    bool ReadOctaveLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteOctaveLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetColl(std::string coll_) { m_coll = coll_; };
    std::string GetColl() const { return m_coll; };
    ///@}

protected:
    /**
     * Indicates whether the octave displacement should be performed simultaneously
     * with the written notes, i.e., "coll' ottava".
     * Unlike other octave signs which are indicated by broken lines, coll' ottava
     * typically uses an unbroken line or a series of longer broken lines, ending with
     * a short vertical stroke. See Read, p. 47-48.
     **/
    std::string m_coll;

/* include <attcoll> */
};

//----------------------------------------------------------------------------
// AttPedalLog
//----------------------------------------------------------------------------

class AttPedalLog: public Att
{
public:
    AttPedalLog();
    virtual ~AttPedalLog();
    
    /** Reset the default values for the attribute class **/
    void ResetPedalLog();
    
    /** Read the values for the attribute class **/
    bool ReadPedalLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePedalLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDir(std::string dir_) { m_dir = dir_; };
    std::string GetDir() const { return m_dir; };
    ///@}

protected:
    /** Records the position of the piano damper pedal. **/
    std::string m_dir;

/* include <attdir> */
};

//----------------------------------------------------------------------------
// AttPedalVis
//----------------------------------------------------------------------------

class AttPedalVis: public Att
{
public:
    AttPedalVis();
    virtual ~AttPedalVis();
    
    /** Reset the default values for the attribute class **/
    void ResetPedalVis();
    
    /** Read the values for the attribute class **/
    bool ReadPedalVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePedalVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetStyle(std::string style_) { m_style = style_; };
    std::string GetStyle() const { return m_style; };
    ///@}

protected:
    /** Determines whether piano pedal marks should be rendered as lines or as terms. **/
    std::string m_style;

/* include <attstyle> */
};

//----------------------------------------------------------------------------
// AttPianopedals
//----------------------------------------------------------------------------

class AttPianopedals: public Att
{
public:
    AttPianopedals();
    virtual ~AttPianopedals();
    
    /** Reset the default values for the attribute class **/
    void ResetPianopedals();
    
    /** Read the values for the attribute class **/
    bool ReadPianopedals( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePianopedals( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetPedalStyle(std::string pedalStyle_) { m_pedalStyle = pedalStyle_; };
    std::string GetPedalStyle() const { return m_pedalStyle; };
    ///@}

protected:
    /** Determines whether piano pedal marks should be rendered as lines or as terms. **/
    std::string m_pedalStyle;

/* include <attpedal.style> */
};

//----------------------------------------------------------------------------
// AttRehearsal
//----------------------------------------------------------------------------

class AttRehearsal: public Att
{
public:
    AttRehearsal();
    virtual ~AttRehearsal();
    
    /** Reset the default values for the attribute class **/
    void ResetRehearsal();
    
    /** Read the values for the attribute class **/
    bool ReadRehearsal( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteRehearsal( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRehEnclose(std::string rehEnclose_) { m_rehEnclose = rehEnclose_; };
    std::string GetRehEnclose() const { return m_rehEnclose; };
    ///@}

protected:
    /** Describes the enclosing shape for rehearsal marks. **/
    std::string m_rehEnclose;

/* include <attreh.enclose> */
};

//----------------------------------------------------------------------------
// AttScoreDefVisCmn
//----------------------------------------------------------------------------

class AttScoreDefVisCmn: public Att
{
public:
    AttScoreDefVisCmn();
    virtual ~AttScoreDefVisCmn();
    
    /** Reset the default values for the attribute class **/
    void ResetScoreDefVisCmn();
    
    /** Read the values for the attribute class **/
    bool ReadScoreDefVisCmn( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteScoreDefVisCmn( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetGridShow(std::string gridShow_) { m_gridShow = gridShow_; };
    std::string GetGridShow() const { return m_gridShow; };
    ///@}

protected:
    /** Determines whether to display guitar chord grids. **/
    std::string m_gridShow;

/* include <attgrid.show> */
};

//----------------------------------------------------------------------------
// AttSlurrend
//----------------------------------------------------------------------------

class AttSlurrend: public Att
{
public:
    AttSlurrend();
    virtual ~AttSlurrend();
    
    /** Reset the default values for the attribute class **/
    void ResetSlurrend();
    
    /** Read the values for the attribute class **/
    bool ReadSlurrend( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSlurrend( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSlurRend(std::string slurRend_) { m_slurRend = slurRend_; };
    std::string GetSlurRend() const { return m_slurRend; };
    ///@}

protected:
    /** Describes the line style of the slur. **/
    std::string m_slurRend;

/* include <attslur.rend> */
};

//----------------------------------------------------------------------------
// AttStemmedCmn
//----------------------------------------------------------------------------

class AttStemmedCmn: public Att
{
public:
    AttStemmedCmn();
    virtual ~AttStemmedCmn();
    
    /** Reset the default values for the attribute class **/
    void ResetStemmedCmn();
    
    /** Read the values for the attribute class **/
    bool ReadStemmedCmn( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStemmedCmn( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetStemMod(std::string stemMod_) { m_stemMod = stemMod_; };
    std::string GetStemMod() const { return m_stemMod; };
    //
    void SetStemWith(std::string stemWith_) { m_stemWith = stemWith_; };
    std::string GetStemWith() const { return m_stemWith; };
    ///@}

protected:
    /**
     * Encodes any stem "modifiers"; that is, symbols rendered on the stem, such as
     * tremolo or Sprechstimme indicators.
     **/
    std::string m_stemMod;
    /**
     * Contains an indication of which staff a note or chord that logically belongs to
     * the current staff should be visually placed on; that is, the one above or the
     * one below.
     **/
    std::string m_stemWith;

/* include <attstem.with> */
};

//----------------------------------------------------------------------------
// AttTierend
//----------------------------------------------------------------------------

class AttTierend: public Att
{
public:
    AttTierend();
    virtual ~AttTierend();
    
    /** Reset the default values for the attribute class **/
    void ResetTierend();
    
    /** Read the values for the attribute class **/
    bool ReadTierend( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTierend( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTieRend(std::string tieRend_) { m_tieRend = tieRend_; };
    std::string GetTieRend() const { return m_tieRend; };
    ///@}

protected:
    /** Describes the line style of the tie. **/
    std::string m_tieRend;

/* include <atttie.rend> */
};

//----------------------------------------------------------------------------
// AttTremmeasured
//----------------------------------------------------------------------------

class AttTremmeasured: public Att
{
public:
    AttTremmeasured();
    virtual ~AttTremmeasured();
    
    /** Reset the default values for the attribute class **/
    void ResetTremmeasured();
    
    /** Read the values for the attribute class **/
    bool ReadTremmeasured( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTremmeasured( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMeasperf(std::string measperf_) { m_measperf = measperf_; };
    std::string GetMeasperf() const { return m_measperf; };
    ///@}

protected:
    /** The performed duration of an individual note in a measured tremolo. **/
    std::string m_measperf;

/* include <attmeasperf> */
};

//----------------------------------------------------------------------------
// AttTupletLog
//----------------------------------------------------------------------------

class AttTupletLog: public Att
{
public:
    AttTupletLog();
    virtual ~AttTupletLog();
    
    /** Reset the default values for the attribute class **/
    void ResetTupletLog();
    
    /** Read the values for the attribute class **/
    bool ReadTupletLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTupletLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDur(std::string dur_) { m_dur = dur_; };
    std::string GetDur() const { return m_dur; };
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
// AttTupletVis
//----------------------------------------------------------------------------

class AttTupletVis: public Att
{
public:
    AttTupletVis();
    virtual ~AttTupletVis();
    
    /** Reset the default values for the attribute class **/
    void ResetTupletVis();
    
    /** Read the values for the attribute class **/
    bool ReadTupletVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTupletVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBracketPlace(std::string bracketPlace_) { m_bracketPlace = bracketPlace_; };
    std::string GetBracketPlace() const { return m_bracketPlace; };
    //
    void SetBracketVisible(std::string bracketVisible_) { m_bracketVisible = bracketVisible_; };
    std::string GetBracketVisible() const { return m_bracketVisible; };
    //
    void SetDurVisible(std::string durVisible_) { m_durVisible = durVisible_; };
    std::string GetDurVisible() const { return m_durVisible; };
    //
    void SetNumFormat(std::string numFormat_) { m_numFormat = numFormat_; };
    std::string GetNumFormat() const { return m_numFormat; };
    ///@}

protected:
    /**
     * Used to state where a tuplet bracket will be placed in relation to the note
     * heads.
     **/
    std::string m_bracketPlace;
    /** States whether a bracket should be rendered with a tuplet. **/
    std::string m_bracketVisible;
    /** Determines if the tuplet duration is visible. **/
    std::string m_durVisible;
    /** Controls how the num:numbase ratio is to be displayed. **/
    std::string m_numFormat;

/* include <attnum.format> */
};

//----------------------------------------------------------------------------
// AttTupletSpanLog
//----------------------------------------------------------------------------

class AttTupletSpanLog: public Att
{
public:
    AttTupletSpanLog();
    virtual ~AttTupletSpanLog();
    
    /** Reset the default values for the attribute class **/
    void ResetTupletSpanLog();
    
    /** Read the values for the attribute class **/
    bool ReadTupletSpanLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTupletSpanLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDur(std::string dur_) { m_dur = dur_; };
    std::string GetDur() const { return m_dur; };
    ///@}

protected:
    /**
     * Records the duration of a feature using the relative durational values provided
     * by the data.DURATION datatype.
     **/
    std::string m_dur;

/* include <attdur> */
};

} // vrv namespace

#endif  // __VRV_ATTS_CMN_H__

