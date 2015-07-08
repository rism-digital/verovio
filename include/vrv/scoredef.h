/////////////////////////////////////////////////////////////////////////////
// Name:        scoredef.h
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SCOREDEF_H__
#define __VRV_SCOREDEF_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

class Clef;
class ClefAttr;
class KeySig;
class KeySigAttr;
class Mensur;
class MensurAttr;
class MeterSig;
class MeterSigAttr;
class StaffGrp;
class StaffDef;

// unused? LP
#define STAFFDEF_DRAW_NONE  0
#define STAFFDEF_DRAW_CLEF  (1<<0)
#define STAFFDEF_DRAW_KEYSIG  (1<<1)
#define STAFFDEF_DRAW_MENSUR  (1<<2)
#define STAFFDEF_DRAW_ALL (1<<3) - 1

//----------------------------------------------------------------------------
// ScoreOrStaffDefAttrInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for MEI scoreDef or staffDef attributes clef, keysig and mensur.
 * It can either hold element or attribute values. Element values are hold in normal objects
 * (e.g., Clef) and attribute values are hold in dedicated Object classes (e.g., ClefAttr)
 * During rendering, only Element object are used. They are obained by the GetXXXCopy methods
 * that create a copy of the Element object or a corresponding Element object if a attribute value
 * object is hold.
 */
class ScoreOrStaffDefAttrInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    ScoreOrStaffDefAttrInterface();
    virtual ~ScoreOrStaffDefAttrInterface();
    virtual void Reset();
    ScoreOrStaffDefAttrInterface( const ScoreOrStaffDefAttrInterface& interface ); // copy contructor
    ScoreOrStaffDefAttrInterface& operator=( const ScoreOrStaffDefAttrInterface& interface ); // copy assignement;
    ///@}
    
    /**
     * @name Replace the clef, keysig, mensur and meterSig.
     * The current value (if any) is replaced by the new value (if any).
     * Values passed and hold can be either a object or attribute value object.
     * For example, it can be either a Clef or a ClefAttr.
    ///@{
     */
    void ReplaceClef( Object *newClef );
    void ReplaceKeySig( Object *newKeySig );
    void ReplaceMensur( Object *newMensur );
    void ReplaceMeterSig( Object *newMeterSig );
    ///@}
    
    /**
     * @name Get a copy of the clef, keysig, mensur and meterSig.
     * These methods creates new objects that need to be deleted.
     * The also convert attribute value object to an object. For example,
     * if m_clef holds a ClefAttr object, the copy will be a Clef object.
     * They are used when writing the MEI.
     */
    ///@{
    Clef *GetClefCopy() const;
    KeySig *GetKeySigCopy() const;
    Mensur *GetMensurCopy() const;
    MeterSig *GetMeterSigCopy() const;
    ///@}
    
    /**
     * @name Get the clef, keysig, mensur and meterSig.
     * They will return a reference only if the value hold is
     * an element (e.g., Clef) and not a attribute value object (ClefAttr).
     * They are used when writing the MEI.
     */
    ///@{
    Clef *GetClefElement() const;
    KeySig *GetKeySigElement() const;
    Mensur *GetMensurElement() const;
    MeterSig *GetMeterSigElement() const;
    ///@}
    
    /**
     * @name Get the clef, keysig, mensur and meterSig attributes.
     * They will return a reference only if the value hold is
     * an attibute value object (e.g., ClefAtt) and not an element (Clef).
     */
    ///@{
    ClefAttr *GetClefAttr() const;
    KeySigAttr *GetKeySigAttr() const;
    MensurAttr *GetMensurAttr() const;
    MeterSigAttr *GetMeterSigAttr() const;
    ///@}
    
    /**
     * @name Get the clef, keysig, mensur and meterSig object.
     * They will return a reference to the hold object (element or attribute).
     */
    ///@{
    Object *GetClef() const { return m_clef; };
    Object *GetKeySig() const { return m_keySig; };
    Object *GetMensur() const { return m_mensur; };
    Object *GetMeterSig() const { return m_meterSig; };
    ///@}
    
protected:
    /** The clef or clef attributes */
    Object *m_clef;
    /** The key signature */
    Object *m_keySig;
    /** The mensur */
    Object *m_mensur;
    /** The meter signature (time signature) */
    Object *m_meterSig;

};



//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

/**
 * This class represents a MEI scoreDef.
 * It contains StaffGrp objects.
*/
class ScoreDef: public Object, public ScoreOrStaffDefAttrInterface, public ObjectListInterface
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
    ///@}
    
	void AddStaffGrp( StaffGrp *staffGrp );
    
    /**
     * Replace the scoreDef with the content of the newScoreDef.
     */
    void Replace( ScoreDef *newScoreDef );
    
    /**
     * Replace the corresponding staffDef with the content of the newStaffDef.
     * Looks for the staffDef with the same m_n (@n) and replace the attribute set.
     * Attribute set is provided by the ScoreOrStaffDefAttrInterface.
     */
    void Replace( StaffDef *newStaffDef );
    
    /**
     * Get the staffDef with number n (NULL if not found).
     */
    StaffDef *GetStaffDef( int n );
    
    /**
     * Set the redraw flag to all staffDefs.
     * This is necessary at the beginning or when a scoreDef occurs.
     */
    void SetRedrawFlags( bool clef, bool keysig, bool mensur, bool meterSig );
    
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
    virtual int CastOffSystems( ArrayPtrVoid params );
    
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
    ///@}
	
	void AddStaffDef( StaffDef *staffDef );
    
	void AddStaffGrp( StaffGrp *staffGrp );
    
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
class StaffDef: public Object, public ScoreOrStaffDefAttrInterface,
    public AttCommon,
    public AttLabelsAddl,
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
    ///@}
    
    /**
     * @name Set and get the layer drawing flags for clef, keysig and mensur.
     * This will be true when starting a new system or when a scoreDef or staffDef changes occurs
     * This will be true only for the first layer in the staff.
     */
    ///@{
    bool DrawClef() const { return m_drawClef; };
    void SetDrawClef( bool drawClef ) { m_drawClef = drawClef; };
    bool DrawKeySig() const { return m_drawKeySig; };
    void SetDrawKeySig( bool drawKeySig ) { m_drawKeySig = drawKeySig; };
    bool DrawMensur() const { return m_drawMensur; };
    void SetDrawMensur( bool drawMensur ) { m_drawMensur = drawMensur; };
    bool DrawMeterSig() const { return m_drawMeterSig; };
    void SetDrawMeterSig( bool drawMeterSig ) { m_drawMeterSig = drawMeterSig; };
    ///@}

    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Replace all the staffDefs in a scoreDef.
     * Calls ScoreDef::Replace.
     * param 0: a pointer to the scoreDef we are going to replace the staffDefs
     */
    virtual int ReplaceStaffDefsInScoreDef( ArrayPtrVoid params );
    
    /**
     * Set flags for the staff set for indicating whether clefs, keysig, etc. needs to be redrawn.
     * This is typically occurs when a new system or a new scoreDef is encountered.
     * param 0: bool clef flag.
     * param 1: bool keysig flag.
     * param 2: bool the mensur flag.
     */
    virtual int SetStaffDefRedrawFlags( ArrayPtrVoid params );
    
private:
    
public:
    
private:
    /**
     *  @name Flags for indicating whether the clef, keysig and mensur needs to be drawn or not
     */
    ///@{
    bool m_drawClef;
    bool m_drawKeySig;
    bool m_drawMensur;
    bool m_drawMeterSig;
    ///@}
    
};
    
} // namespace vrv

#endif
