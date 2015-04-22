/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_LAYER_ELEMENT_H__
#define __VRV_LAYER_ELEMENT_H__

#include "devicecontextbase.h"
#include "object.h"

namespace vrv {

class Alignment;
class BeamElementCoord;
class Layer;
class Mensur;
class MeterSig;
class Staff;

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

/** 
 * This class is a base class for the Layer (<layer>) content.
 * It is not an abstract class but should not be instantiated directly.
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
     * Reset the alignment values (m_drawingX, m_drawingXRel, etc.)
     * Called by AlignHorizontally
     */
    virtual void ResetHorizontalAlignment( );
    
    /** 
     * Adjust the pname and the octave for values outside the range
     */
    static void AdjustPname( int *pname, int *oct );
    
    /**
     * @name Set and get the flag for indication whether it is a ScoreDef or StaffDef attribute.
     * The value is false by default. Is it set to true of ScoreDef and StaffDef and used when
     * drawing the element
     */
    ///@{
    bool GetScoreOrStaffDefAttr() const { return m_isScoreOrStaffDefAttr; };
    void SetScoreOrStaffDefAttr( bool isScoreOrStaffDefAttr ) { m_isScoreOrStaffDefAttr = isScoreOrStaffDefAttr; };
    ///@}

    /**
     * @name Child type checkers.
     */
    ///@{
    bool IsAccid();
    bool IsBarline();
    bool IsBeam();
    bool IsChord();
    bool IsClef();
    bool IsCustos();
    bool IsDot();
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
    bool IsTie();
    bool IsTuplet();
    bool IsSpace();
    bool IsSyl();
    bool IsVerse();
    bool IsGraceNote();
    ///@}

    Alignment *GetAlignment() { return m_alignment; };
    
    int GetXRel();
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * See Object::AlignHorizontally
     */
    virtual int AlignHorizontally( ArrayPtrVoid params );
    
    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning( ArrayPtrVoid params );
    
    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY( ArrayPtrVoid params );
    
protected:
    /**
     * Returns the duration if the child element has a DurationInterface
     */
    virtual double GetAlignmentDuration( Mensur *mensur = NULL, MeterSig *meterSig = NULL );
    
private:
    
public:
	/** Absolute position X. This is used for facsimile (transcription) encoding */
    int m_xAbs;
    /** Indicates if cue size - to be changed to MEI equivalent (size="cue") */
    bool m_cueSize;

    /** If this is a note, store here the stem coordinates (useful for ex. tuplets) */
    Point m_drawingStemStart; // beginning point, the one near the note
    Point m_drawingStemEnd; // end point (!), near beam or stem
    /** 
     * Stem direction as drawn 
     */
    data_STEMDIRECTION m_drawingStemDir;
    /**
     * This store a pointer to the corresponding BeamElementCoord(currentDur > DUR_4)
     */
    BeamElementCoord *m_beamElementCoord;
    /**
     * This store a pointer to the cross-staff (if any) and the appropriate layer
     * Initialized in LayerElement::SetDrawingXY
     */
    Staff *m_crossStaff;
    Layer *m_crossLayer;
    
protected:
    Alignment *m_alignment;
    
private:
    /** Indicates whether it is a ScoreDef or StaffDef attribute */
    bool m_isScoreOrStaffDefAttr;
    
};

} // namespace vrv


#endif
