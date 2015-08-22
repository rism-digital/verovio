/////////////////////////////////////////////////////////////////////////////
// Name:        scoredef.h
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SCORE_DEF_H__
#define __VRV_SCORE_DEF_H__

#include "atts_shared.h"
#include "drawinginterface.h"
#include "object.h"
#include "scoredefinterface.h"

namespace vrv {

class Clef;
class KeySig;
class Mensur;
class MeterSig;
class StaffGrp;
class StaffDef;

//----------------------------------------------------------------------------
// ScoreOrStaffDefElement
//----------------------------------------------------------------------------

/**
 * This class is an interface for MEI scoreDef or staffDef attributes clef, keysig and mensur.
 * It can either hold element or attribute values. Element values are hold in normal objects
 * (e.g., Clef) and attribute values are hold in dedicated Object classes (e.g., ClefAttr)
 * During rendering, only Element object are used. They are obained by the GetXXXCopy methods
 * that create a copy of the Element object or a corresponding Element object if a attribute value
 * object is hold.
 */
class ScoreDefElement: public Object, public ScoreDefInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    ScoreDefElement(std::string classid);
    virtual ~ScoreDefElement();
    virtual void Reset();
    virtual ClassId Is() { return SCORE_DEF_ELEMENT; };
    ///@}
    
    /**
     * @name Get a copy of the clef, keysig, mensur and meterSig.
     * These methods creates new objects that need to be deleted.
     * The also convert attribute value object to an object. For example,
     * if m_clef holds a ClefAttr object, the copy will be a Clef object.
     * They are used when writing the MEI.
     */
    ///@{
    bool HasClefInfo();
    bool HasKeySigInfo();
    bool HasMensurInfo();
    bool HasMeterSigInfo();
    ///@}
    
    /**
     * @name Get a copy of the clef, keysig, mensur and meterSig.
     * These methods creates new objects that need to be deleted.
     * The also convert attribute value object to an object. For example,
     * if m_clef holds a ClefAttr object, the copy will be a Clef object.
     * They are used when writing the MEI.
     */
    ///@{
    Clef *GetClefCopy();
    KeySig *GetKeySigCopy();
    Mensur *GetMensurCopy();
    MeterSig *GetMeterSigCopy();
    ///@}
    
protected:
    
private:
    /**
     * @name Methods for checking if clef info is available at the attribute level
     */
    ///@{
    bool HasClefAttrInfo();
    bool HasKeySigAttrInfo();
    bool HasMensurAttrInfo();
    bool HasMeterSigAttrInfo();
    ///@}

    /**
     * @name Methods for checking if clef info is available at the element level
     */
    ///@{
    bool HasClefElementInfo();
    bool HasKeySigElementInfo();
    bool HasMensurElementInfo();
    bool HasMeterSigElementInfo();
    ///@}
    
};



//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

/**
 * This class represents a MEI scoreDef.
 * It contains StaffGrp objects.
*/
class ScoreDef: public ScoreDefElement, public ObjectListInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    ScoreDef();
    virtual ~ScoreDef();    
    virtual void Reset();
    virtual std::string GetClassName( ) { return "ScoreDef"; };
    virtual ClassId Is() { return SCORE_DEF; };
    ///@}
    
	void AddStaffGrp( StaffGrp *staffGrp );
    
    /**
     * Replace the scoreDef with the content of the newScoreDef.
     */
    void ReplaceDrawingValues( ScoreDef *newScoreDef );
    
    /**
     * Replace the corresponding staffDef with the content of the newStaffDef.
     * Looks for the staffDef with the same m_n (@n) and replace the attribute set.
     * Attribute set is provided by the ScoreOrStaffDefInterface.
     */
    void ReplaceDrawingValues( StaffDef *newStaffDef );
    
    /**
     * Get the staffDef with number n (NULL if not found).
     */
    StaffDef *GetStaffDef( int n );
    
    /**
     * Set the redraw flag to all staffDefs.
     * This is necessary at the beginning or when a scoreDef occurs.
     */
    void SetRedrawFlags( bool clef, bool keySig, bool mensur, bool meterSig, bool keySigCancellation );
    
    /**
     * @name Set and get the scoreDef drawing flags for clef, keysig and mensur.
     */
    ///@{
    bool DrawLabels() const { return m_drawLabels; };
    void SetDrawLabels( bool drawLabels ) { m_drawLabels = drawLabels; };
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Fill a page by adding systems with the appropriate length
     * For ScoreDef, this means only moving them since their width is not taken into
     * account
     */
    virtual int CastOffSystems( ArrayPtrVoid *params );
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep staffGrp for fast access.
     */
    virtual void FilterList( ListOfObjects *childList );
    
private:
    
public:

private:
    /** Flags for indicating whether lables needs to be drawn or not */
    bool m_drawLabels;
    
};


//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

/**
 * This class represents a MEI staffGrp.
 * It contains StaffDef objects.
 */
class StaffGrp: public Object, public ObjectListInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    StaffGrp();
    virtual ~StaffGrp();
    virtual Object* Clone() { return new StaffGrp(*this); };
    virtual void Reset();
    virtual std::string GetClassName( ) { return "StaffGrp"; };
    virtual ClassId Is() { return STAFF_GRP; };
    ///@}
	
    /**
     * @name Methods for adding allowed content
     */
    ///@{
	void AddStaffDef( StaffDef *staffDef );
	void AddStaffGrp( StaffGrp *staffGrp );
    ///@}
    
    /**
     * @name Set and get the staffGrp @symbol
     */
    ///@{
    StaffGrpSymbol GetSymbol() const { return m_symbol; };
    void SetSymbol( StaffGrpSymbol symbol ) { m_symbol = symbol; };
    ///@}
    
    /**
     * @name Set and get the staffGrp @barthru
     */
    ///@{
    bool GetBarthru() const { return m_barthru; };
    void SetBarthru( bool barthru ) { m_barthru = barthru; };
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep staffDef for fast access.
     */
    virtual void FilterList( ListOfObjects *childList );
    
private:
    
public:
    
private:
    StaffGrpSymbol m_symbol;
    bool m_barthru;

};


//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

/**
 * This class represents a MEI staffDef.
 */
class StaffDef: public ScoreDefElement, public StaffDefDrawingInterface,
    public AttCommon,
    public AttLabelsAddl,
    public AttScalable,
    public AttStaffDefVis
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    StaffDef();
    virtual ~StaffDef();
    virtual Object* Clone() { return new StaffDef(*this); };
    virtual void Reset();
    virtual std::string GetClassName( ) { return "StaffDef"; };
    virtual ClassId Is() { return STAFF_DEF; };
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Replace all the staffDefs in a scoreDef.
     * Calls ScoreDef::Replace.
     * param 0: a pointer to the scoreDef we are going to replace the staffDefs
     */
    virtual int ReplaceDrawingValuesInScoreDef( ArrayPtrVoid *params );
    
    /**
     * Set flags for the staff set for indicating whether clefs, keysig, etc. needs to be redrawn.
     * This is typically occurs when a new system or a new scoreDef is encountered.
     * param 0: bool clef flag.
     * param 1: bool keysig flag.
     * param 2: bool the mensur flag.
     */
    virtual int SetStaffDefRedrawFlags( ArrayPtrVoid *params );
    
private:
    
public:
    
private:
    
};
    
} // namespace vrv

#endif
