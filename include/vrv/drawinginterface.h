/////////////////////////////////////////////////////////////////////////////
// Name:        drawinginterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DRAWING_INTERFACE_H__
#define __VRV_DRAWING_INTERFACE_H__

#include "clef.h"
#include "devicecontextbase.h"
#include "keysig.h"
#include "mensur.h"
#include "metersig.h"
#include "vrvdef.h"

namespace vrv {

class Object;

//----------------------------------------------------------------------------
// DrawingListInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciate directly.
 */
class DrawingListInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    DrawingListInterface();
    virtual ~DrawingListInterface();
    virtual void Reset();
    ///@}

    /**
     * Add an element to the drawing list.
     * The drawing list is used to postponed the drawing of elements
     * that need to be drawn in a particular order.
     * For example, we need to draw beams before tuplets
     */
    void AddToDrawingList(Object *element);

    /**
     * Return the drawing list.
     * This is used when actually drawing the list (see View::DrawLayerList)
     */
    ListOfObjects *GetDrawingList();

    /**
     * Reset the drawing list.
     * Clears the list - called when the layer starts to be drawn
     */
    void ResetDrawingList();

private:
    //
public:
    //
private:
    /** The list of object for which drawing is postponed */
    ListOfObjects m_drawingList;
};

//----------------------------------------------------------------------------
// StaffDefDrawingInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for MEI scoreDef or staffDef attributes clef, keysig and mensur.
 * It can either hold element or attribute values. Element values are hold in normal objects
 * (e.g., Clef) and attribute values are hold in dedicated Object classes (e.g., ClefAttr)
 * During rendering, only Element object are used. They are obained by the GetXXXCopy methods
 * that create a copy of the Element object or a corresponding Element object if a attribute value
 * object is hold.
 */
class StaffDefDrawingInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    StaffDefDrawingInterface();
    virtual ~StaffDefDrawingInterface();
    virtual void Reset();
    ///@}

    /**
     * @name Set and get the layer drawing flags for clef, keysig and mensur.
     * This will be true when starting a new system or when a scoreDef or staffDef changes occurs
     * This will be true only for the first layer in the staff.
     */
    ///@{
    bool DrawClef() const { return (m_drawClef && m_currentClef.HasShape()); };
    void SetDrawClef(bool drawClef) { m_drawClef = drawClef; };
    bool DrawKeySig() const { return (m_drawKeySig && (m_currentKeySig.GetAlterationType() != ACCIDENTAL_EXPLICIT_NONE)); };
    void SetDrawKeySig(bool drawKeySig) { m_drawKeySig = drawKeySig; };
    bool DrawKeySigCancellation() const { return m_drawKeySigCancellation; };
    void SetDrawKeySigCancellation(bool drawKeySigCancellation) { m_drawKeySigCancellation = drawKeySigCancellation; };
    bool DrawMensur() const {return (m_drawMensur && m_currentMensur.HasSign()); };
    void SetDrawMensur(bool drawMensur) { m_drawMensur = drawMensur; };
    bool DrawMeterSig() const { return (m_drawMeterSig && m_currentMeterSig.HasUnit()); };
    void SetDrawMeterSig(bool drawMeterSig) { m_drawMeterSig = drawMeterSig; };
    ///@}

    /**
     * @name Set the current clef, keySig, mensur and meterSig.
     */
    ///@{
    void SetCurrentClef(Clef *clef);
    void SetCurrentKeySig(KeySig *keySig);
    void SetCurrentMensur(Mensur *mensur);
    void SetCurrentMeterSig(MeterSig *meterSig);
    ///@}

    /**
     * @name Get the clef, keysig, mensur and meterSig object.
     * They will return a reference to the hold object (element or attribute).
     */
    ///@{
    Clef *GetCurrentClef() { return &m_currentClef; };
    KeySig *GetCurrentKeySig() { return &m_currentKeySig; };
    Mensur *GetCurrentMensur() { return &m_currentMensur; };
    MeterSig *GetCurrentMeterSig() { return &m_currentMeterSig; };
    ///@}

private:
    /** The clef or clef attributes */
    Clef m_currentClef;
    /** The key signature */
    KeySig m_currentKeySig;
    /** The mensur */
    Mensur m_currentMensur;
    /** The meter signature (time signature) */
    MeterSig m_currentMeterSig;

    /**
     *  @name Flags for indicating whether the clef, keysig and mensur needs to be drawn or not
     */
    ///@{
    bool m_drawClef;
    bool m_drawKeySig;
    bool m_drawKeySigCancellation;
    bool m_drawMensur;
    bool m_drawMeterSig;
    ///@}
};

//----------------------------------------------------------------------------
// StemmedDrawingInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for MEI stemmed element.
 * It stores stem drawing values for notes and chords.
 */
class StemmedDrawingInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    StemmedDrawingInterface();
    virtual ~StemmedDrawingInterface();
    virtual void Reset();
    ///@}

    /**
     * @name Set and get the stem direction and stem positions
     * The methods are virtual because they need to be overridden for Chords.
     */
    ///@{
    virtual void SetDrawingStemDir(data_STEMDIRECTION stemDir);
    virtual data_STEMDIRECTION GetDrawingStemDir();
    virtual void SetDrawingStemStart(Point stemStart);
    virtual Point GetDrawingStemStart();
    virtual void SetDrawingStemEnd(Point stemEnd);
    virtual Point GetDrawingStemEnd();
    ///@}

protected:
    /**
     * If this is a note, store here the stem coordinates (useful for ex. tuplets)
     */
    Point m_drawingStemStart; // beginning point, the one near the note
    Point m_drawingStemEnd; // end point (!), near beam or stem
    /**
     * Stem direction as drawn
     */
    data_STEMDIRECTION m_drawingStemDir;
};

} // namespace vrv

#endif
