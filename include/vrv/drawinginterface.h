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
#include "metersiggrp.h"
#include "proport.h"
#include "vrvdef.h"

namespace vrv {

class Object;
class Stem;

//----------------------------------------------------------------------------
// DrawingListInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instantiate directly.
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

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * Called explicitly from ResetDataFunctor
     */
    ///@{
    FunctorCode InterfaceResetData(ResetDataFunctor &functor);
    ///@}

private:
    //
public:
    //
protected:
    /** The list of object for which drawing is postponed */
    ArrayOfObjects m_drawingList;

private:
    //
};

//----------------------------------------------------------------------------
// BeamDrawingInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for MEI beam elements (beam, beamSpan).
 * It stores stem drawing values.
 * It is also an ObjectListInterface.
 */
class BeamDrawingInterface : public ObjectListInterface {
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
     * Return information about the position in the beam.
     */
    ///@{
    bool IsFirstIn(const LayerElement *element) const;
    bool IsLastIn(const LayerElement *element) const;
    ///@}

    /**
     * Initializes the m_beamElementCoords vector objects.
     */
    ///@{
    bool HasCoords() const { return !m_beamElementCoords.empty(); }
    void InitCoords(const ArrayOfObjects &childList, Staff *staff, data_BEAMPLACE place);
    void InitCoords(const ListOfObjects &childList, Staff *staff, data_BEAMPLACE place);
    ///@}

    /**
     * Initialize m_cueSize value based on the @cue attribute and presence of child elements with @cue
     * attributes
     */
    void InitCue(bool beamCue);

    /**
     * Initialize m_notesStemDir value based on the @graceGrp attribute and presence of child elements with @grace
     * attributes
     */
    void InitGraceStemDir(bool graceGrp);

    bool IsHorizontal() const;

    bool IsRepeatedPattern() const;

    /**
     * Checks whether difference between highest and lowest notes of the beam is just one step
     */
    bool HasOneStepHeight() const;

    /**
     * Get total beam width with regards to the shortest duration of the beam (counting both beams and whitespace
     * between them)
     */
    int GetTotalBeamWidth() const;

    /**
     * Clear the m_beamElementCoords vector and delete all the objects.
     */
    void ClearCoords();

    /**
     * Helper to find number of additional beams. Return { additional beams above main beam, additional beams below }
     */
    virtual std::pair<int, int> GetAdditionalBeamCount() const { return { 0, 0 }; }

    /**
     * Helper to get number of beams represented by attributes @beam and @beam.float
     */
    virtual std::pair<int, int> GetFloatingBeamCount() const { return { 0, 0 }; }

    /**
     * Get above/below overflow
     */
    void GetBeamOverflow(StaffAlignment *&above, StaffAlignment *&below) const;

    /**
     * Get above/below overflow for the children
     */
    void GetBeamChildOverflow(StaffAlignment *&above, StaffAlignment *&below) const;

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * Called explicitly from ResetDataFunctor
     */
    ///@{
    FunctorCode InterfaceResetData(ResetDataFunctor &functor);
    ///@}

protected:
    /**
     * Return the position of the element in the beam.
     * For notes, lookup the position of the parent chord.
     */
    int GetPosition(const LayerElement *element) const;

    //
private:
    /**
     * Determines whether beam should be horizontal based on item positions and relative beam place. Should be used with
     * mixed beams, where beam place can be different for separate elements
     */
    bool IsHorizontalMixedBeam(const std::vector<int> &items, const std::vector<data_BEAMPLACE> &directions) const;

public:
    // values to be set before calling CalcBeam
    bool m_changingDur;
    bool m_beamHasChord;
    bool m_hasMultipleStemDir;
    bool m_cueSize;
    Staff *m_crossStaffContent;
    data_STAFFREL_basic m_crossStaffRel;
    bool m_isSpanningElement;
    data_DURATION m_shortestDur;
    data_STEMDIRECTION m_notesStemDir;
    data_BEAMPLACE m_drawingPlace;
    Staff *m_beamStaff;

    // values set by CalcBeam
    int m_beamWidth;
    int m_beamWidthBlack;
    int m_beamWidthWhite;
    int m_fractionSize;

    /**
     * An array of coordinates for each element
     **/
    ArrayOfBeamElementCoords m_beamElementCoords;

private:
    //
};

//----------------------------------------------------------------------------
// StaffDefDrawingInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for MEI scoreDef or staffDef attributes clef, keysig and mensur.
 * It can either hold element or attribute values. Element values are hold in normal objects
 * (e.g., Clef) and attribute values are hold in dedicated Object classes (e.g., ClefAttr)
 * During rendering, only Element object are used. They are obtained by the GetXXXCopy methods
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
    bool DrawClef() const { return (m_drawClef && m_currentClef.HasShape()); }
    void SetDrawClef(bool drawClef) { m_drawClef = drawClef; }
    bool DrawKeySig() const { return (m_drawKeySig); }
    void SetDrawKeySig(bool drawKeySig) { m_drawKeySig = drawKeySig; }
    bool DrawMensur() const { return (m_drawMensur && (m_currentMensur.HasSign() || m_currentMensur.HasNum())); }
    void SetDrawMensur(bool drawMensur) { m_drawMensur = drawMensur; }
    bool DrawMeterSig() const
    {
        return (m_drawMeterSig && (m_currentMeterSig.HasUnit() || m_currentMeterSig.HasSym()));
    }
    void SetDrawMeterSig(bool drawMeterSig) { m_drawMeterSig = drawMeterSig; }
    bool DrawMeterSigGrp() const;
    void SetDrawMeterSigGrp(bool drawMeterSigGrp) { m_drawMeterSigGrp = drawMeterSigGrp; }
    ///@}

    /**
     * @name Set the current clef, keySig, mensur and meterSig.
     */
    ///@{
    void SetCurrentClef(const Clef *clef);
    void SetCurrentKeySig(const KeySig *keySig);
    void SetCurrentMensur(const Mensur *mensur);
    void SetCurrentMeterSig(const MeterSig *meterSig);
    void SetCurrentMeterSigGrp(const MeterSigGrp *meterSigGrp);
    void AlternateCurrentMeterSig(const Measure *measure);
    void SetCurrentProport(const Proport *proport);
    ///@}

    /**
     * @name Get the clef, keysig, mensur and meterSig object.
     * They will return a reference to the hold object (element or attribute).
     */
    ///@{
    Clef *GetCurrentClef() { return &m_currentClef; }
    const Clef *GetCurrentClef() const { return &m_currentClef; }
    KeySig *GetCurrentKeySig() { return &m_currentKeySig; }
    const KeySig *GetCurrentKeySig() const { return &m_currentKeySig; }
    Mensur *GetCurrentMensur() { return &m_currentMensur; }
    const Mensur *GetCurrentMensur() const { return &m_currentMensur; }
    MeterSig *GetCurrentMeterSig() { return &m_currentMeterSig; }
    const MeterSig *GetCurrentMeterSig() const { return &m_currentMeterSig; }
    MeterSigGrp *GetCurrentMeterSigGrp() { return &m_currentMeterSigGrp; }
    const MeterSigGrp *GetCurrentMeterSigGrp() const { return &m_currentMeterSigGrp; }
    Proport *GetCurrentProport() { return &m_currentProport; }
    const Proport *GetCurrentProport() const { return &m_currentProport; }
    ///@}

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * Called explicitly from ResetDataFunctor
     */
    ///@{
    FunctorCode InterfaceResetData(ResetDataFunctor &functor);
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
    /** The meter signature group */
    MeterSigGrp m_currentMeterSigGrp;
    /** The proport */
    Proport m_currentProport;

    /**
     *  @name Flags for indicating whether the clef, keysig and mensur needs to be drawn or not
     */
    ///@{
    bool m_drawClef;
    bool m_drawKeySig;
    bool m_drawMensur;
    bool m_drawMeterSig;
    bool m_drawMeterSigGrp;
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
     * @name Set and get the stem object managed by the interface
     */
    ///@{
    void SetDrawingStem(Stem *stem);
    Stem *GetDrawingStem() { return m_drawingStem; }
    const Stem *GetDrawingStem() const { return m_drawingStem; }
    ///@}

    /**
     * @name Set and get the stem direction and stem positions
     * Wrapper that passes values to the Stem object.
     */
    ///@{
    void SetDrawingStemDir(data_STEMDIRECTION stemDir);
    data_STEMDIRECTION GetDrawingStemDir() const;
    void SetDrawingStemLen(int drawingStemLen);
    int GetDrawingStemLen() const;
    int GetDrawingStemModRelY() const;
    ///@}

    Point GetDrawingStemStart(const Object *object = NULL) const;
    Point GetDrawingStemEnd(const Object *object = NULL) const;

    /**
     * @name Virtual methods overriden in child classes (Chord and Note)
     */
    ///@{
    virtual Point GetStemUpSE(const Doc *doc, int staffSize, bool graceSize) const = 0;
    virtual Point GetStemDownNW(const Doc *doc, int staffSize, bool graceSize) const = 0;
    virtual int CalcStemLenInThirdUnits(const Staff *staff, data_STEMDIRECTION stemDir) const = 0;
    ///@}

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * Called explicitly from ResetDataFunctor
     */
    ///@{
    FunctorCode InterfaceResetData(ResetDataFunctor &functor);
    ///@}

protected:
    /**
     * A pointer to the child Stem set by PrepareLayerElementParts
     */
    Stem *m_drawingStem;
};

//----------------------------------------------------------------------------
// VisibilityDrawingInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for MEI element that can be hidden during drawing.
 */
class VisibilityDrawingInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    VisibilityDrawingInterface();
    virtual ~VisibilityDrawingInterface();
    virtual void Reset();
    ///@}

    /**
     * @name Set and get the visibility
     */
    ///@{
    void SetVisibility(VisibilityType visibility) { m_visibility = visibility; }
    bool IsHidden() const { return (m_visibility == Hidden); }
    ///@}

    //-----------------//
    // Pseudo functors //
    //-----------------//

private:
    /**
     * Holds the visibility (hidden or visible) for an element implementing the interface.
     * By default, all editorial elements are visible. However, in an <app>, only one <rdg> is visible at the time. When
     * loading the file, the first <rdg> (or the <lem>) is made visible.
     */
    VisibilityType m_visibility;
};

} // namespace vrv

#endif
