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

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttLigatureLog
//----------------------------------------------------------------------------

class AttLigatureLog 
{
public:
    AttLigatureLog();
    virtual ~AttLigatureLog();
    
    /** Reset the default values for the attribute class **/
    void ResetLigatureLog();
    
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
// AttMensurVis
//----------------------------------------------------------------------------

class AttMensurVis 
{
public:
    AttMensurVis();
    virtual ~AttMensurVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMensurVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() { return m_form; };
    //
    void SetOrient(std::string orient_) { m_orient = orient_; };
    std::string GetOrient() { return m_orient; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;
    /** Describes the rotation or reflection of the base symbol. **/
    std::string m_orient;

/* include <attorient> */
};

//----------------------------------------------------------------------------
// AttMensurDefaultLog
//----------------------------------------------------------------------------

class AttMensurDefaultLog 
{
public:
    AttMensurDefaultLog();
    virtual ~AttMensurDefaultLog();
    
    /** Reset the default values for the attribute class **/
    void ResetMensurDefaultLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMensurDot(std::string mensurDot_) { m_mensurDot = mensurDot_; };
    std::string GetMensurDot() { return m_mensurDot; };
    //
    void SetMensurLocInt(int mensurLocInt_) { m_mensurLocInt = mensurLocInt_; };
    int GetMensurLocInt() { return m_mensurLocInt; };
    //
    void SetMensurSign(std::string mensurSign_) { m_mensurSign = mensurSign_; };
    std::string GetMensurSign() { return m_mensurSign; };
    //
    void SetMensurSlashInt(int mensurSlashInt_) { m_mensurSlashInt = mensurSlashInt_; };
    int GetMensurSlashInt() { return m_mensurSlashInt; };
    ///@}

protected:
    /** Determines if a dot is to be added to the base symbol. **/
    std::string m_mensurDot;
    /** Holds the staff location of the mensuration sign. **/
    int m_mensurLocInt;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    std::string m_mensurSign;
    /**
     * Indicates the number lines added to the mensuration sign.
     * For example, one slash is added for what we now call 'alla breve'.
     **/
    int m_mensurSlashInt;

/* include <attmensur.slash> */
};

//----------------------------------------------------------------------------
// AttMensurDefaultVis
//----------------------------------------------------------------------------

class AttMensurDefaultVis 
{
public:
    AttMensurDefaultVis();
    virtual ~AttMensurDefaultVis();
    
    /** Reset the default values for the attribute class **/
    void ResetMensurDefaultVis();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMensurColor(std::string mensurColor_) { m_mensurColor = mensurColor_; };
    std::string GetMensurColor() { return m_mensurColor; };
    //
    void SetMensurForm(std::string mensurForm_) { m_mensurForm = mensurForm_; };
    std::string GetMensurForm() { return m_mensurForm; };
    //
    void SetMensurOrient(std::string mensurOrient_) { m_mensurOrient = mensurOrient_; };
    std::string GetMensurOrient() { return m_mensurOrient; };
    //
    void SetMensurSize(std::string mensurSize_) { m_mensurSize = mensurSize_; };
    std::string GetMensurSize() { return m_mensurSize; };
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

class AttNoteLogMensural 
{
public:
    AttNoteLogMensural();
    virtual ~AttNoteLogMensural();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteLogMensural();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetLig(std::string lig_) { m_lig = lig_; };
    std::string GetLig() { return m_lig; };
    ///@}

protected:
    /** Indicates this element's participation in a ligature. **/
    std::string m_lig;

/* include <attlig> */
};

//----------------------------------------------------------------------------
// AttRestVisMensural
//----------------------------------------------------------------------------

class AttRestVisMensural 
{
public:
    AttRestVisMensural();
    virtual ~AttRestVisMensural();
    
    /** Reset the default values for the attribute class **/
    void ResetRestVisMensural();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSpacesInt(int spacesInt_) { m_spacesInt = spacesInt_; };
    int GetSpacesInt() { return m_spacesInt; };
    ///@}

protected:
    /** States how many spaces are covered by the rest. **/
    int m_spacesInt;

/* include <attspaces> */
};

//----------------------------------------------------------------------------
// AttStaffDefLogMensural
//----------------------------------------------------------------------------

class AttStaffDefLogMensural 
{
public:
    AttStaffDefLogMensural();
    virtual ~AttStaffDefLogMensural();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffDefLogMensural();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetProportNumInt(int proportNumInt_) { m_proportNumInt = proportNumInt_; };
    int GetProportNumInt() { return m_proportNumInt; };
    //
    void SetProportNumbaseInt(int proportNumbaseInt_) { m_proportNumbaseInt = proportNumbaseInt_; };
    int GetProportNumbaseInt() { return m_proportNumbaseInt; };
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

class AttStafflocPitched 
{
public:
    AttStafflocPitched();
    virtual ~AttStafflocPitched();
    
    /** Reset the default values for the attribute class **/
    void ResetStafflocPitched();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetPloc(std::string ploc_) { m_ploc = ploc_; };
    std::string GetPloc() { return m_ploc; };
    //
    void SetOloc(std::string oloc_) { m_oloc = oloc_; };
    std::string GetOloc() { return m_oloc; };
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

