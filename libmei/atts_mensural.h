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
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };
    //
    void SetOrient(data_ORIENTATION orient_) { m_orient = orient_; };
    data_ORIENTATION GetOrient() const { return m_orient; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;
    /** Describes the rotation or reflection of the base symbol. **/
    data_ORIENTATION m_orient;

/* include <attorient> */
};

//----------------------------------------------------------------------------
// AttMensurDefaultLog
//----------------------------------------------------------------------------

class AttMensurDefaultLog: public Att
{
public:
    AttMensurDefaultLog();
    virtual ~AttMensurDefaultLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMensurDefaultLog();
    
    /** Read the values for the attribute class **/
    bool ReadMensurDefaultLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMensurDefaultLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMensurDot(data_BOOLEAN mensurDot_) { m_mensurDot = mensurDot_; };
    data_BOOLEAN GetMensurDot() const { return m_mensurDot; };
    //
    void SetMensurLocInt(int mensurLocInt_) { m_mensurLocInt = mensurLocInt_; };
    int GetMensurLocInt() const { return m_mensurLocInt; };
    //
    void SetMensurSign(data_MENSURATIONSIGN mensurSign_) { m_mensurSign = mensurSign_; };
    data_MENSURATIONSIGN GetMensurSign() const { return m_mensurSign; };
    //
    void SetMensurSlash(unsigned char mensurSlash_) { m_mensurSlash = mensurSlash_; };
    unsigned char GetMensurSlash() const { return m_mensurSlash; };
    ///@}

protected:
    /** Determines if a dot is to be added to the base symbol. **/
    data_BOOLEAN m_mensurDot;
    /** Holds the staff location of the mensuration sign. **/
    int m_mensurLocInt;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    data_MENSURATIONSIGN m_mensurSign;
    /**
     * Indicates the number lines added to the mensuration sign.
     * For example, one slash is added for what we now call 'alla breve'.
     **/
    unsigned char m_mensurSlash;

/* include <attmensur.slash> */
};

//----------------------------------------------------------------------------
// AttMensurDefaultVis
//----------------------------------------------------------------------------

class AttMensurDefaultVis: public Att
{
public:
    AttMensurDefaultVis();
    virtual ~AttMensurDefaultVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMensurDefaultVis();
    
    /** Read the values for the attribute class **/
    bool ReadMensurDefaultVis( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMensurDefaultVis( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMensurColor(std::string mensurColor_) { m_mensurColor = mensurColor_; };
    std::string GetMensurColor() const { return m_mensurColor; };
    //
    void SetMensurForm(std::string mensurForm_) { m_mensurForm = mensurForm_; };
    std::string GetMensurForm() const { return m_mensurForm; };
    //
    void SetMensurOrient(std::string mensurOrient_) { m_mensurOrient = mensurOrient_; };
    std::string GetMensurOrient() const { return m_mensurOrient; };
    //
    void SetMensurSize(std::string mensurSize_) { m_mensurSize = mensurSize_; };
    std::string GetMensurSize() const { return m_mensurSize; };
    ///@}

protected:
    /**
     * Records the color of the mensuration sign.
     * Do not confuse this with the musical term 'color' as used in pre-CMN notation.
     **/
    std::string m_mensurColor;
    /** Indicates whether the base symbol is written vertically or horizontally. **/
    std::string m_mensurForm;
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
     * @name Setters and getters for class members
     */
    ///@{
    void SetLig(data_LIGATURE lig_) { m_lig = lig_; };
    data_LIGATURE GetLig() const { return m_lig; };
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
     * @name Setters and getters for class members
     */
    ///@{
    void SetSpacesInt(int spacesInt_) { m_spacesInt = spacesInt_; };
    int GetSpacesInt() const { return m_spacesInt; };
    ///@}

protected:
    /** States how many spaces are covered by the rest. **/
    int m_spacesInt;

/* include <attspaces> */
};

//----------------------------------------------------------------------------
// AttStaffDefLogMensural
//----------------------------------------------------------------------------

class AttStaffDefLogMensural: public Att
{
public:
    AttStaffDefLogMensural();
    virtual ~AttStaffDefLogMensural();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffDefLogMensural();
    
    /** Read the values for the attribute class **/
    bool ReadStaffDefLogMensural( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStaffDefLogMensural( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetProportNumInt(int proportNumInt_) { m_proportNumInt = proportNumInt_; };
    int GetProportNumInt() const { return m_proportNumInt; };
    //
    void SetProportNumbaseInt(int proportNumbaseInt_) { m_proportNumbaseInt = proportNumbaseInt_; };
    int GetProportNumbaseInt() const { return m_proportNumbaseInt; };
    ///@}

protected:
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
     * @name Setters and getters for class members
     */
    ///@{
    void SetPloc(std::string ploc_) { m_ploc = ploc_; };
    std::string GetPloc() const { return m_ploc; };
    //
    void SetOloc(std::string oloc_) { m_oloc = oloc_; };
    std::string GetOloc() const { return m_oloc; };
    ///@}

protected:
    /** Captures staff location in terms of written pitch name. **/
    std::string m_ploc;
    /** Records staff location in terms of written octave. **/
    std::string m_oloc;

/* include <attoloc> */
};

} // vrv namespace

#endif  // __VRV_ATTS_MENSURAL_H__

