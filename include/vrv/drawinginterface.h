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
class Stem;

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
    ArrayOfObjects *GetDrawingList();

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
    ArrayOfObjects m_drawingList;
};

//----------------------------------------------------------------------------
// BeamDrawingInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for MEI beam elements (beam, beamSpan).
 * It stores stem drawing values.
 */
class BeamDrawingInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    BeamDrawingInterface();
    virtual ~BeamDrawingInterface();
    virtual void Reset();
    ///@}

    /**
     * Initializes the m_beamElementCoords vector objects.
     * This is called by Beam::FilterList
     */
    void InitCoords(ArrayOfObjects *childList, Staff *staff, data_BEAMPLACE place);

    bool IsHorizontal();

    bool IsRepeatedPattern();

    /**
     * Checks whether difference between highest and lowest notes of the beam is just one step
     */
    bool HasOneStepHeight();

    /**
     * Clear the m_beamElementCoords vector and delete all the objects.
     */
    void ClearCoords();

protected:
    //
public:
    // values to be set before calling CalcBeam
    bool m_changingDur;
    bool m_beamHasChord;
    bool m_hasMultipleStemDir;
    bool m_cueSize;
    bool m_hasCrossStaffContent;
    int m_shortestDur;
    data_STEMDIRECTION m_notesStemDir;
    data_BEAMPLACE m_drawingPlace;
    Staff *m_beamStaff;

    // values set by CalcBeam
    int m_beamWidth;
    int m_beamWidthBlack;
    int m_beamWidthWhite;

    // position x for the stem (normal and cue-sized)
    int m_stemXAbove[2];
    int m_stemXBelow[2];

    /**
     * An array of coordinates for each element
     **/
    ArrayOfBeamElementCoords m_beamElementCoords;
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
    bool DrawClef() { return (m_drawClef && m_currentClef.HasShape()); }
    void SetDrawClef(bool drawClef) { m_drawClef = drawClef; }
    bool DrawKeySig() { return (m_drawKeySig); }
    void SetDrawKeySig(bool drawKeySig) { m_drawKeySig = drawKeySig; }
    bool DrawMensur() { return (m_drawMensur && m_currentMensur.HasSign()); }
    void SetDrawMensur(bool drawMensur) { m_drawMensur = drawMensur; }
    bool DrawMeterSig() { return (m_drawMeterSig && (m_currentMeterSig.HasUnit() || m_currentMeterSig.HasSym())); }
    void SetDrawMeterSig(bool drawMeterSig) { m_drawMeterSig = drawMeterSig; }
    ///@}

    /**
     * @name Set the current clef, keySig, mensur and meterSig.
     */
    ///@{
    void SetCurrentClef(Clef const *clef);
    void SetCurrentKeySig(KeySig const *keySig);
    void SetCurrentMensur(Mensur const *mensur);
    void SetCurrentMeterSig(MeterSig const *meterSig);
    ///@}

    /**
     * @name Get the clef, keysig, mensur and meterSig object.
     * They will return a reference to the hold object (element or attribute).
     */
    ///@{
    Clef *GetCurrentClef() { return &m_currentClef; }
    KeySig *GetCurrentKeySig() { return &m_currentKeySig; }
    Mensur *GetCurrentMensur() { return &m_currentMensur; }
    MeterSig *GetCurrentMeterSig() { return &m_currentMeterSig; }
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

    void SetDrawingStem(Stem *stem);
    Stem *GetDrawingStem() const { return m_drawingStem; }

    /**
     * @name Set and get the stem direction and stem positions
     * Wrapper that passes values to the Stem object.
     */
    ///@{
    void SetDrawingStemDir(data_STEMDIRECTION stemDir);
    data_STEMDIRECTION GetDrawingStemDir();
    void SetDrawingStemLen(int drawingStemLen);
    int GetDrawingStemLen();
    ///@}

    Point GetDrawingStemStart(Object *object = NULL);
    Point GetDrawingStemEnd(Object *object = NULL);

    /**
     * @name Virtual methods overriden in child classes (Chord and Note)
     */
    ///@{
    virtual Point GetStemUpSE(Doc *doc, int staffSize, bool graceSize) = 0;
    virtual Point GetStemDownNW(Doc *doc, int staffSize, bool graceSize) = 0;
    virtual int CalcStemLenInThirdUnits(Staff *staff) = 0;
    ///@}

protected:
    /**
     * A pointer to the child Stem set by PrepareLayerElementParts
     */
    Stem *m_drawingStem;
};

} // namespace vrv

#endif
