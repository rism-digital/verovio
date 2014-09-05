/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_LAYER_ELEMENT_H__
#define __VRV_LAYER_ELEMENT_H__

#include "devicecontext.h" // for MusPoint
#include "object.h"
#include "vrvdef.h"

namespace vrv {

class Alignment;

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

/** 
 * This class is a base class for the Layer (<layer>) content.
 * It is not an abstract class but should not be instanciate directly.
 */
class LayerElement: public DocObject
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    LayerElement();
    LayerElement(std::string classid);
    virtual ~LayerElement();
    virtual void Reset();
    ///@}
    
    LayerElement& operator=( const LayerElement& element ); // copy assignement - this need to be changed to the Object::Clone way;
    
    /**
     * Return a copy of the LayerElement (child class).
     * By default, a new uuid is generated
     */
    LayerElement *GetChildCopy( bool newUuid = true );
    
    /**
     * Return the default horizontal spacing of elements.
     * This method should be redefined whenever necessary in the child classes.
     */
    virtual int GetHorizontalSpacing( );
    
    /**
     * Reset the alignment values (m_drawingX, m_drawingXRel, etc.)
     * Called by AlignHorizontally
     */
    virtual void ResetHorizontalAlignment( );
    
    /** Adjust the pname and the octave for values outside the range */
    static void AdjustPname( int *pname, int *oct );
    
    /** 
     * Set the pitch or position for PitchInterface or PositionInterface elements.
     * Because PitchInterface and PositionInterface are not child classes of LayerElement,
     * the call had to be done explicitly from this method. The method can still be overriden.
     */
    virtual void SetPitchOrPosition( int pname, int oct );
    
    /**
     * Get the pitch or position for PitchInterface or PositionInterface elements.
     * See LayerElement::SetPitchOrPosition for more comments.
     */
    virtual bool GetPitchOrPosition( int *pname, int *oct );
    
    /**
     * Set the value for child element of LayerElement.
     * For example, set the duration for DurationInterface elements (call explicitly) 
     */
	virtual void SetValue( int value, int flag = 0 );
    
    /**
     * @name Set and get the flag for indication whether it is a ScoreDef or StaffDef attribute.
     * The value is false by default. Is it set to true of ScoreDef and StaffDef and used when
     * drawing the element
     */
    ///@{
    bool GetScoreOrStaffDefAttr() const { return m_isScoreOrStaffDefAttr; };
    void SetScoreOrStaffDefAttr( bool isScoreOrStaffDefAttr ) { m_isScoreOrStaffDefAttr = isScoreOrStaffDefAttr; };
    ///@}
    
    int GetElementIdx() const;

    
    bool IsBarline();
    bool IsBeam();
    bool IsClef();
    bool HasDurationInterface();
    bool IsKeySig();
    bool IsMensur();
    bool IsMeterSig();
    bool IsMultiRest();
    bool IsMRest();
    bool IsNote();
    bool HasPitchInterface();
    bool HasPositionInterface();
    bool IsRest();
    bool IsSymbol( SymbolType type );
    bool IsSymbol( );
    bool IsTie();
    bool IsTuplet();
    bool IsGraceNote();
    
    virtual double GetAlignementDuration();
    
    Alignment *GetAlignment() { return m_alignment; };
    
    int GetXRel();
    
    // functors
    /**
     * Save the object (virtual).
     * Most of the child classes do not override it. In these cases, the actual 
     * saving occurs in the FileOutputStream::WriteLayerElement method
     * A few classes, such as LayerApp, have an overriden version.
     */
    virtual int Save( ArrayPtrVoid params );
    
    /**
     * Align horizontally the LayerElement.
     */
    virtual int AlignHorizontally( ArrayPtrVoid params );
    
private:
    
public:
	/** Absolute position X. This is used for facsimile (transcription) encoding */
    int m_xAbs;
    /** Indicates if cue size - to be changed to MEI equivalent */
    bool m_cueSize;

    /** If this is a note, store here the stem coordinates (useful for ex. tuplets) */
    MusPoint m_drawingStemStart; // beginning point, the one near the note
    MusPoint m_drawingStemEnd; // end point (!), near beam or stem
    /** stem direction as drawn, true = up, false = down */
    bool m_drawingStemDir;
    
protected:
    Alignment *m_alignment;
    
private:
    /** Indicates whether it is a ScoreDef or StaffDef attribute */
    bool m_isScoreOrStaffDefAttr;
    
};

} // namespace vrv


#endif
