/////////////////////////////////////////////////////////////////////////////
// Name:        staff.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_STAFF_H__
#define __VRV_STAFF_H__

#include "object.h"

namespace vrv {

#define STAFF_OFFSET 190
#define MAXCLE 100	// maximum number of clef on one staff

class Clef;
class DeviceContext;
class Layer;
class StaffAlignment;

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

/**
 * This class represents a staff in a laid-out score (Doc).
 * A Staff is contained in a System.
 * It contains Measure objects.
 * For unmeasured music, on single Measure is added for simplifying internal processing
*/
class Staff: public DocObject
{
    
public:
    // constructors and destructors
    Staff( int n = -1 );
	Staff( const Staff& staff ); // copy contructor
    virtual ~Staff();

    virtual std::string GetClassName( ) { return "Staff"; };	
    
    void Clear();
    
    void AddLayer( Layer *layer );
	
	int GetLayerCount() const { return (int)m_children.size(); };
    
    /**
     * Return the index position of the staff in its measure parent
     */
    int GetStaffIdx() const { return Object::GetIdx(); };
    
    /**
     * @name Set and get  the staff number which is 1-based.
     * This can be different from the index position in the parent measure.
     */
    ///@{
    int GetStaffNo() const { return m_n; };
    void SetStaffNo( int n ) { m_n = n; };
    ///@}
    
    /**
     * Return the default horizontal spacing of staves.
     */
    int GetVerticalSpacing();
    
    /**
     * Reset the drawing values (m_drawingX, m_drawingXRel, etc.)
     * Called by SetCurrentScoreDef functor.
     */
    virtual void ResetDrawingValues( );
    
    StaffAlignment *GetAlignment() { return m_staffAlignment; };
    
    int GetYRel();

    // functors
    virtual int Save( ArrayPtrVoid params );
    virtual bool GetPosOnPage( ArrayPtrVoid params );
    
	void CopyAttributes( Staff *staff ); // copy all attributes but none of the elements

	Layer *GetFirst( );
	Layer *GetLast( );
	Layer *GetNext( Layer *layer );
	Layer *GetPrevious( Layer *layer );
    Layer *GetLayerWithIdx( int LayerIdx );
    
    /**
     * Align the content of a system.
     */
    virtual int Align( ArrayPtrVoid params );
    
public:
	/** numero dans le groupe auquel appartient la portee */
	unsigned short noGrp;
	/** nombre total de portee dans le groupe */
	unsigned short totGrp;
	/** numero de systeme */
	//unsigned short noLigne; // ax2
	/** type d'armure. 0 = aucune1 = dieses2 = bemols */
	unsigned char armTyp;
	/** nombre d'alterations a l'armures */
	unsigned char armNbr;
	/** portee en notation ancienne */
	char notAnc;
	/** portee grise */
	char grise;
	/** portee invisible */
	char invisible;
	/** barre verticale. 0 = aucun1 = debut2 = fin */
	unsigned char vertBarre;
	/** accolade ou bloc. 0 = aucun1 = debut2 = fin */
	unsigned char brace;
	/** taille. 0 = normale1 = petite  */
	unsigned char staffSize;
	
    /** portee indentee
		char dans Wolfgang - ici indentation par portee
	*/
	//int indent; // ax2 
	/** portee indentee a droite */
	//char indentDroite; // ax2
	/** type de portee (nombre de lignes) */
	unsigned char portNbLine;
	/** type d'accolade. 0 = bloc1 = accolade ronde */
	unsigned char accol;
	/** ???? */
	unsigned char accessoire;
	/** 
     * The Y absolute position of the staff for facsimile (transcription) encodings.
     * This is the top left corner of the staff (the X position is the position of the system).
     */
    int m_yAbs;
    /**
     * The Y relative position of the staff.
     * It is used internally when calculating the layout andd it is not stored in the file.
     */
    //int m_drawingYRel;
	/** 
     * The Y drawing position of the staff.
     * It is re-computed everytime the staff is drawn and it is not stored in the file.
     */
    int m_drawingY;
    
    /** indicates this staff is in anchent notation
     it has to be carried on to the LayedOutStaf */
    bool m_mensuralNotation;

private:
    /** The staff number */
    int m_n;
    
    /**
     * A pointer to a StaffAlignment for aligning the staves
     */
    StaffAlignment *m_staffAlignment;

    
};

} // namespace vrv

#endif
