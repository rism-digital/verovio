/////////////////////////////////////////////////////////////////////////////
// Name:        layer.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_LAYER_H__
#define __VRV_LAYER_H__

#include <typeinfo>

#include "clef.h"
#include "object.h"
#include "scoredef.h"

namespace vrv {

class DeviceContext;
class LayerElement;
class Note;

//----------------------------------------------------------------------------
// Layer
//----------------------------------------------------------------------------

/**
 * This class represents a layer in a laid-out score (Doc).
 * A Layer is contained in a Staff.
 * It contains LayerElement objects.
*/
class Layer: public DocObject, public ObjectListInterface, public ScoreOrStaffDefAttrInterface
{
public:
    // constructors and destructors
    Layer( int n );
    virtual ~Layer();
    
    virtual std::string GetClassName( ) { return "Layer"; };
    
    void Clear();
	
	void AddElement( LayerElement *element, int idx = -1 );
    	
	int GetElementCount() const { return (int)m_children.size(); };
    
    /**
     * Return the index position of the layer in its staff parent.
     * The index position is 0-based.
     */
    int GetLayerIdx() const { return Object::GetIdx(); };
    
    
    /**
     * @name Set and get the layer number which is 1-based.
     * This can be different from the index position in the parent staff.
     */
    ///@{
    int GetLayerNo() const { return m_n; };
    void SetLayerNo( int n ) { m_n = n; };
    ///@}

    // functors
    /**
     * Copy the elements to a Layer passed in parameters. 
     * Also take into account a start and end uuid for the page (if any)
     */ 
    virtual int CopyToLayer( ArrayPtrVoid params );
    void CopyElements( ArrayPtrVoid params ); // unused
    void GetMaxXY( ArrayPtrVoid params ); // unused
    virtual int Save( ArrayPtrVoid params );
    void CheckAndResetSectionOrMeasure( ArrayPtrVoid params ); // unused
    
	void CopyAttributes( Layer *layer ); // copy all attributes but none of the elements
    
	LayerElement *GetFirst( );
	LayerElement *GetLast( );
	LayerElement *GetNext( LayerElement *element );
	LayerElement *GetPrevious( LayerElement *element );
	LayerElement *GetAtPos( int x );
	LayerElement *Insert( LayerElement *element, int x ); // return a pointer on the inserted element
    
    void Insert( LayerElement *element, LayerElement *before );
    
	void Delete( LayerElement *element );
	
    /**
     * Looks for the first LayerElement of type elementType.
     * Looks FORWARD of BACKWARD depending on the direction parameter.
     * Returns the retrieved element if *succ == true or the original element if not.
     */
    LayerElement *GetFirst( LayerElement *element, unsigned int direction, const std::type_info *elementType, bool *succ );
    
    /** 
     * Get the current clef for the test element.
     * Goes back on the layer until a clef is found.
     * This is used when inserting a note by passing a y position because we need
     * to know the clef in order to get the pitch.
     */
	Clef *GetClef ( LayerElement *test );
    
    /** 
     * Return the clef offset for the position x.
     * The method uses Layer::GetClef first to find the clef before test.
     */
    int GetClefOffset( LayerElement *test  );
    
    /**
     * Add an element to the drawing list.
     * The drawing list is used to postponed the drawing of elements
     * that need to be drawn in a particular order.
     * For example, we need to draw beams before tuplets
     */
    void AddToDrawingList( LayerElement *element );

    /**
     * Return the drawing list.
     * This is used when actually drawing the list (see View::DrawLayerList)
     */
    ListOfObjects *GetDrawingList( );

    /**
     * Reset the drawing list.
     * Clears the list - called when the layer starts to be drawn
     */
    void ResetDrawingList( );
    
    /**
     * Basic method that remove intermediate clefs and custos.
     * Used for building collations (See CmpFile::Collate).
     */
    void RemoveClefAndCustos( );
    
    /**
     * Checks that the X position of the currentElement is not before the previous element or after the next one.
     * This works for facsimile (transcription) encodings only because it looks at the m_xAbs position
     */ 
    void CheckXPosition( LayerElement *currentElement );
    
    /**
     * Set drawing clef, keysig and mensur if necessary and if available.
     * Also set the current clef.
     */
    void SetDrawingValues( ScoreDef *currentScoreDef, StaffDef *currentStaffDef );
    

    //Lyric related methods
    /*
	int GetLyricPos( Symbol *lyric );
	Symbol *GetPreviousLyric( Symbol *lyric );
	Symbol *GetNextLyric( Symbol *lyric );
	Symbol *GetFirstLyric( );
	Symbol *GetLastLyric( );
	Symbol *GetLyricAtPos( int x );
	void DeleteLyric( Symbol *symbol );
	Note *GetNextNote( Symbol * lyric );
	Note *GetPreviousNote( Symbol * lyric );
	void SwitchLyricNoteAssociation( Symbol *lyric, Note *oldNote, Note* newNote, bool beginning );
	void AdjustLyricLineHeight( int delta );
    */
    
    /**
     * Align the content of a system.
     */
    
    ///@{

    virtual int Align( ArrayPtrVoid params );
    
private:
    
public:
	/** voix de la portee*/
	unsigned short voix;
    
protected:
    // drawing variables
    //LayerElement *beamListPremier; // we need to replace this with a proper beam class that handles a list of notes/rests
    ListOfObjects m_drawingList;
    
private:
    /** The layer number */
    int m_n;

    /**
     * The current clef.
     * This is used for getting the clef when interacting, for example by clicking in order to get the pitch from a position
     */
    Clef m_currentClef;
    
};

} // namespace vrv

#endif
