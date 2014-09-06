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

#ifndef __VRV_ATTS_MENSURAL_H__
#define __VRV_ATTS_MENSURAL_H__

#include "att.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttLigatureLog
//----------------------------------------------------------------------------

class AttLigatureLog: public Att
{
public:
    AttLigatureLog();
    virtual ~AttLigatureLog();
    
    /** Reset the default values for the attribute class **/
    void ResetLigatureLog();
    
    /** Read the values for the attribute class **/
    bool ReadLigatureLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteLigatureLog( pugi::xml_node element );
    
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

protected:
    /** Records the function of the dot. **/
    std::string m_form;

/* include <attform> */
};

//----------------------------------------------------------------------------
// AttMensurVis
//----------------------------------------------------------------------------

class AttMensurVis: public Att
{
public:
    AttMensurVis();
    virtual ~AttMensurVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMensurVis();
    
    /** Read the values for the attribute class **/
    bool ReadMensurVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMensurVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };    
    bool HasForm( );
    
    //
    void SetOrient(data_ORIENTATION orient_) { m_orient = orient_; };
    data_ORIENTATION GetOrient() const { return m_orient; };    
    bool HasOrient( );
    
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;
    /** Describes the rotation or reflection of the base symbol. **/
    data_ORIENTATION m_orient;

/* include <attorient> */
};

//----------------------------------------------------------------------------
// AttMensuralLog
//----------------------------------------------------------------------------

class AttMensuralLog: public Att
{
public:
    AttMensuralLog();
    virtual ~AttMensuralLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMensuralLog();
    
    /** Read the values for the attribute class **/
    bool ReadMensuralLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMensuralLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMensurDot(data_BOOLEAN mensurDot_) { m_mensurDot = mensurDot_; };
    data_BOOLEAN GetMensurDot() const { return m_mensurDot; };    
    bool HasMensurDot( );
    
    //
    void SetMensurSign(data_MENSURATIONSIGN mensurSign_) { m_mensurSign = mensurSign_; };
    data_MENSURATIONSIGN GetMensurSign() const { return m_mensurSign; };    
    bool HasMensurSign( );
    
    //
    void SetMensurSlash(char mensurSlash_) { m_mensurSlash = mensurSlash_; };
    char GetMensurSlash() const { return m_mensurSlash; };    
    bool HasMensurSlash( );
    
    //
    void SetProportNum(int proportNumInt_) { m_proportNumInt = proportNumInt_; };
    int GetProportNum() const { return m_proportNumInt; };    
    bool HasProportNum( );
    
    //
    void SetProportNumbase(int proportNumbaseInt_) { m_proportNumbaseInt = proportNumbaseInt_; };
    int GetProportNumbase() const { return m_proportNumbaseInt; };    
    bool HasProportNumbase( );
    
    ///@}

protected:
    /** Determines if a dot is to be added to the base symbol. **/
    data_BOOLEAN m_mensurDot;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    data_MENSURATIONSIGN m_mensurSign;
    /**
     * Indicates the number lines added to the mensuration sign.
     * For example, one slash is added for what we now call 'alla breve'.
     **/
    char m_mensurSlash;
    /**
     * Together, proport.num and proport.numbase specify a proportional change as a
     * ratio, e.g., 1:3.
     * Proport.num is for the first value in the ratio.
     **/
    int m_proportNumInt;
    /**
     * Together, proport.num and proport.numbase specify a proportional change as a
     * ratio, e.g., 1:3.
     * Proport.numbase is for the second value in the ratio.
     **/
    int m_proportNumbaseInt;

/* include <attproport.numbase> */
};

//----------------------------------------------------------------------------
// AttMensuralShared
//----------------------------------------------------------------------------

class AttMensuralShared: public Att
{
public:
    AttMensuralShared();
    virtual ~AttMensuralShared();
    
    /** Reset the default values for the attribute class **/
    void ResetMensuralShared();
    
    /** Read the values for the attribute class **/
    bool ReadMensuralShared( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMensuralShared( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetModusmaior(std::string modusmaior_) { m_modusmaior = modusmaior_; };
    std::string GetModusmaior() const { return m_modusmaior; };    
    bool HasModusmaior( );
    
    //
    void SetModusminor(std::string modusminor_) { m_modusminor = modusminor_; };
    std::string GetModusminor() const { return m_modusminor; };    
    bool HasModusminor( );
    
    //
    void SetProlatio(std::string prolatio_) { m_prolatio = prolatio_; };
    std::string GetProlatio() const { return m_prolatio; };    
    bool HasProlatio( );
    
    //
    void SetTempus(std::string tempus_) { m_tempus = tempus_; };
    std::string GetTempus() const { return m_tempus; };    
    bool HasTempus( );
    
    ///@}

protected:
    /** Describes the maxima-long relationship. **/
    std::string m_modusmaior;
    /** Describes the long-breve relationship. **/
    std::string m_modusminor;
    /** Describes the semibreve-minim relationship. **/
    std::string m_prolatio;
    /** Describes the breve-semibreve relationship. **/
    std::string m_tempus;

/* include <atttempus> */
};

//----------------------------------------------------------------------------
// AttMensuralVis
//----------------------------------------------------------------------------

class AttMensuralVis: public Att
{
public:
    AttMensuralVis();
    virtual ~AttMensuralVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMensuralVis();
    
    /** Read the values for the attribute class **/
    bool ReadMensuralVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMensuralVis( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMensurColor(std::string mensurColor_) { m_mensurColor = mensurColor_; };
    std::string GetMensurColor() const { return m_mensurColor; };    
    bool HasMensurColor( );
    
    //
    void SetMensurForm(std::string mensurForm_) { m_mensurForm = mensurForm_; };
    std::string GetMensurForm() const { return m_mensurForm; };    
    bool HasMensurForm( );
    
    //
    void SetMensurLoc(std::string mensurLoc_) { m_mensurLoc = mensurLoc_; };
    std::string GetMensurLoc() const { return m_mensurLoc; };    
    bool HasMensurLoc( );
    
    //
    void SetMensurOrient(std::string mensurOrient_) { m_mensurOrient = mensurOrient_; };
    std::string GetMensurOrient() const { return m_mensurOrient; };    
    bool HasMensurOrient( );
    
    //
    void SetMensurSize(std::string mensurSize_) { m_mensurSize = mensurSize_; };
    std::string GetMensurSize() const { return m_mensurSize; };    
    bool HasMensurSize( );
    
    ///@}

protected:
    /**
     * Records the color of the mensuration sign.
     * Do not confuse this with the musical term 'color' as used in pre-CMN notation.
     **/
    std::string m_mensurColor;
    /** Indicates whether the base symbol is written vertically or horizontally. **/
    std::string m_mensurForm;
    /** Holds the staff location of the mensuration sign. **/
    std::string m_mensurLoc;
    /** Describes the rotation or reflection of the base symbol. **/
    std::string m_mensurOrient;
    /** Describes the relative size of the mensuration sign. **/
    std::string m_mensurSize;

/* include <attmensur.size> */
};

//----------------------------------------------------------------------------
// AttNoteLogMensural
//----------------------------------------------------------------------------

class AttNoteLogMensural: public Att
{
public:
    AttNoteLogMensural();
    virtual ~AttNoteLogMensural();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteLogMensural();
    
    /** Read the values for the attribute class **/
    bool ReadNoteLogMensural( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteNoteLogMensural( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetLig(data_LIGATURE lig_) { m_lig = lig_; };
    data_LIGATURE GetLig() const { return m_lig; };    
    bool HasLig( );
    
    ///@}

protected:
    /** Indicates this element's participation in a ligature. **/
    data_LIGATURE m_lig;

/* include <attlig> */
};

//----------------------------------------------------------------------------
// AttRestVisMensural
//----------------------------------------------------------------------------

class AttRestVisMensural: public Att
{
public:
    AttRestVisMensural();
    virtual ~AttRestVisMensural();
    
    /** Reset the default values for the attribute class **/
    void ResetRestVisMensural();
    
    /** Read the values for the attribute class **/
    bool ReadRestVisMensural( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteRestVisMensural( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetSpaces(int spacesInt_) { m_spacesInt = spacesInt_; };
    int GetSpaces() const { return m_spacesInt; };    
    bool HasSpaces( );
    
    ///@}

protected:
    /** States how many spaces are covered by the rest. **/
    int m_spacesInt;

/* include <attspaces> */
};

} // vrv namespace

#endif  // __VRV_ATTS_MENSURAL_H__

