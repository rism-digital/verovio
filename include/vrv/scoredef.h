/////////////////////////////////////////////////////////////////////////////
// Name:        scoredef.h
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SCOREDEF_H__
#define __VRV_SCOREDEF_H__

#include "object.h"

namespace vrv {

class Clef;
class KeySignature;
class Mensur;
class StaffGrp;
class StaffDef;

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
 * For simplification, the attributes are stored as Clef, KeySignature and Mensur.
 */
class ScoreOrStaffDefAttrInterface
{
public:
    // constructors and destructors
    ScoreOrStaffDefAttrInterface();
    virtual ~ScoreOrStaffDefAttrInterface();
    ScoreOrStaffDefAttrInterface( const ScoreOrStaffDefAttrInterface& interface ); // copy contructor
    ScoreOrStaffDefAttrInterface& operator=( const ScoreOrStaffDefAttrInterface& interface ); // copy assignement;
    
    /**
     * Replace the clef (if any) with the newClef (if any).
     */
    void ReplaceClef( Clef *newClef );
    
    /**
     * Replace the keysig (if any) with the newKeysig (if any).
     */
    void ReplaceKeySig( KeySignature *newKeySig );
    
    /**
     * Replace the mensur (if any) with the newMensur (if any).
     */
    void ReplaceMensur( Mensur *newMensur );
    
    /**
     * @name Get  the clef, keysig and mensure.
     */
    ///@{
    Clef *GetClefAttr() const { return m_clef; };
    KeySignature *GetKeySigAttr() const { return m_keySig; };
    Mensur *GetMensurAttr() const { return m_mensur; };
    ///@}
    
protected:
    /** The clef attribute */
    Clef *m_clef;
    /** The key signature */
    KeySignature *m_keySig;
    /** The mensure (time signature */
    Mensur *m_mensur;
    
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
    // constructors and destructors
    ScoreDef();
    virtual ~ScoreDef();
        
    virtual std::string GetClassName( ) { return "ScoreDef"; };
    
    void Clear();
    
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
    void SetRedraw( bool clef, bool keysig, bool mensur );
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep staffGrp for fast access.
     */
    virtual void FilterList();
    
private:
    
public:

private:
    
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
    // constructors and destructors
    StaffGrp();
    virtual ~StaffGrp();
    virtual Object* Clone() { return new StaffGrp(*this); };
    
    virtual std::string GetClassName( ) { return "StaffGrp"; };
	
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
    
    // functors
    virtual int Save( ArrayPtrVoid params );
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep staffDef for fast access.
     */
    virtual void FilterList();
    
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
class StaffDef: public Object, public ScoreOrStaffDefAttrInterface
{
public:
    // constructors and destructors
    StaffDef();
    virtual ~StaffDef();
    virtual Object* Clone() { return new StaffDef(*this); };
    
    virtual std::string GetClassName( ) { return "StaffDef"; };
    
    /**
     * @name Set and get  the staff number which is 1-based.
     * This can be different from the index position in the parent measure.
     */
    ///@{
    int GetStaffNo() const { return m_n; };
    void SetStaffNo( int n ) { m_n = n; };
    ///@}
    
    /**
     * @name Set and get the layer drawing flags for clef, keysig and mensur.
     * This will be true when starting a new system or when a scoreDef or staffDef changes occurs
     * This will be true only for the first layer in the staff.
     */
    ///@{
    int DrawClef() const { return m_drawClef; };
    void SetDrawClef( bool drawClef ) { m_drawClef = drawClef; };
    int DrawKeySig() const { return m_drawKeySig; };
    void SetDrawKeySig( bool drawKeySig ) { m_drawKeySig = drawKeySig; };
    int DrawMensur() const { return m_drawMensur; };
    void SetDrawMensur( bool drawMensur ) { m_drawClef = drawMensur; };
    ///@}

    // functors
    virtual int Save( ArrayPtrVoid params );
    
    /**
     * Replace all the staffDefs in a scoreDef.
     * Calls ScoreDef::Replace.
     * param 0: a pointer to the scoreDef we are going to replace the staffDefs
     */
    virtual int ReplaceStaffDefsInScoreDef( ArrayPtrVoid params );
    
    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     * param 0: bool clef flag.
     * param 1: bool keysig flag.
     * param 2: bool the mensur flag.
     */
    virtual int SetStaffDefDraw( ArrayPtrVoid params );
    
private:
    
public:
    
private:
    /** The staff number */
    int m_n;
    
    /**
     *  @name Flags for indicating where the clef, keysig and mensur needs to be drawn or not
     */
    ///@{
    bool m_drawClef;
    bool m_drawKeySig;
    bool m_drawMensur;
    ///@}
    
};
    
} // namespace vrv

#endif
