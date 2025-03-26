/////////////////////////////////////////////////////////////////////////////
// Name:        layer.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LAYER_H__
#define __VRV_LAYER_H__

#include "atts_shared.h"
#include "drawinginterface.h"
#include "object.h"

namespace vrv {

class Clef;
class DeviceContext;
class LayerElement;
class Measure;
class Note;
class StaffDef;

//----------------------------------------------------------------------------
// Layer
//----------------------------------------------------------------------------

/**
 * This class represents a layer in a laid-out score (Doc).
 * A Layer is contained in a Staff.
 * It contains LayerElement objects.
 */
class Layer : public Object,
              public DrawingListInterface,
              public ObjectListInterface,
              public AttCue,
              public AttNInteger,
              public AttTyped,
              public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Layer();
    virtual ~Layer();
    Object *Clone() const override { return new Layer(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "layer"; }
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    void CloneReset() override;

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * Return the index position of the layer in its staff parent.
     * The index position is 0-based.
     */
    int GetLayerIdx() const { return Object::GetIdx(); }

    LayerElement *GetPrevious(const LayerElement *element);
    const LayerElement *GetPrevious(const LayerElement *element) const;
    LayerElement *GetAtPos(int x);
    const LayerElement *GetAtPos(int x) const;

    /**
     * Get the current clef for the test element.
     * Goes back on the layer until a clef is found.
     * This is used when inserting a note by passing a y position because we need
     * to know the clef in order to get the pitch.
     */
    ///@{
    Clef *GetClef(const LayerElement *test);
    const Clef *GetClef(const LayerElement *test) const;
    ///@}

    /**
     * Get the current clef based on facsimile for the test element.
     * This goes back by facsimile position until a clef is found.
     * Returns NULL if a clef cannot be found via this method.
     */
    ///@{
    Clef *GetClefFacs(const LayerElement *test);
    const Clef *GetClefFacs(const LayerElement *test) const;
    ///@}

    /**
     * Return the clef offset for the position x.
     * The method uses Layer::GetClef first to find the clef before test.
     */
    int GetClefLocOffset(const LayerElement *test) const;

    /**
     * Return the clef offset for the position if there are cross-staff clefs on the same layer
     */
    int GetCrossStaffClefLocOffset(const LayerElement *element, int locOffset) const;

    /**
     * @name Set and get the stem direction of the layer.
     * This stays STEMDIRECTION_NONE with on single layer in the staff.
     */
    ///@{
    void SetDrawingStemDir(data_STEMDIRECTION stemDirection) { m_drawingStemDir = stemDirection; }
    data_STEMDIRECTION GetDrawingStemDir(const LayerElement *element) const;
    data_STEMDIRECTION GetDrawingStemDir(const ArrayOfBeamElementCoords *coords) const;
    data_STEMDIRECTION GetDrawingStemDir() const { return m_drawingStemDir; }
    ///@}

    /**
     * @name Get the layers used for the duration of an element.
     * Takes into account cross-staff situations: cross staff layers have negative N.
     */
    ///@{
    std::set<int> GetLayersNForTimeSpanOf(const LayerElement *element) const;
    int GetLayerCountForTimeSpanOf(const LayerElement *element) const;
    ///@}

    /**
     * @name Get the layers used within a time span.
     * Takes into account cross-staff situations: cross staff layers have negative N.
     */
    ///@{
    std::set<int> GetLayersNInTimeSpan(
        const Fraction &time, const Fraction &duration, const Measure *measure, int staff) const;
    int GetLayerCountInTimeSpan(
        const Fraction &time, const Fraction &duration, const Measure *measure, int staff) const;
    ///@}

    /**
     * Get the list of the layer elements for the duration of an element
     * Takes into account cross-staff situations.
     * If excludeCurrent is specified, gets the list of layer elements for all layers except current
     */
    ///@{
    ListOfObjects GetLayerElementsForTimeSpanOf(const LayerElement *element, bool excludeCurrent = false);
    ListOfConstObjects GetLayerElementsForTimeSpanOf(const LayerElement *element, bool excludeCurrent = false) const;
    ///@}

    /**
     * Get the list of the layer elements used within a time span.
     * Takes into account cross-staff situations.
     */
    ///@{
    ListOfObjects GetLayerElementsInTimeSpan(
        const Fraction &time, const Fraction &duration, const Measure *measure, int staff, bool excludeCurrent);
    ListOfConstObjects GetLayerElementsInTimeSpan(
        const Fraction &time, const Fraction &duration, const Measure *measure, int staff, bool excludeCurrent) const;
    ///@}

    /**
     * Get the current clef, keysig, mensur and meterSig
     */
    ///@{
    Clef *GetCurrentClef();
    const Clef *GetCurrentClef() const;
    KeySig *GetCurrentKeySig();
    const KeySig *GetCurrentKeySig() const;
    Mensur *GetCurrentMensur();
    const Mensur *GetCurrentMensur() const;
    MeterSig *GetCurrentMeterSig();
    const MeterSig *GetCurrentMeterSig() const;
    Proport *GetCurrentProport();
    const Proport *GetCurrentProport() const;
    ///@}

    void ResetStaffDefObjects();

    /**
     * Set drawing clef, keysig, mensur, metersig, metersiggrp if necessary and if available.
     */
    ///@{
    void SetDrawingStaffDefValues(StaffDef *currentStaffDef);

    bool DrawKeySigCancellation() const { return m_drawKeySigCancellation; }
    void SetDrawKeySigCancellation(bool drawKeySigCancellation) { m_drawKeySigCancellation = drawKeySigCancellation; }

    Clef *GetStaffDefClef() { return m_staffDefClef; }
    const Clef *GetStaffDefClef() const { return m_staffDefClef; }
    KeySig *GetStaffDefKeySig() { return m_staffDefKeySig; }
    const KeySig *GetStaffDefKeySig() const { return m_staffDefKeySig; }
    Mensur *GetStaffDefMensur() { return m_staffDefMensur; }
    const Mensur *GetStaffDefMensur() const { return m_staffDefMensur; }
    MeterSig *GetStaffDefMeterSig() { return m_staffDefMeterSig; }
    const MeterSig *GetStaffDefMeterSig() const { return m_staffDefMeterSig; }
    MeterSigGrp *GetStaffDefMeterSigGrp() { return m_staffDefMeterSigGrp; }
    const MeterSigGrp *GetStaffDefMeterSigGrp() const { return m_staffDefMeterSigGrp; }

    bool HasStaffDef() const
    {
        return (m_staffDefClef || m_staffDefKeySig || m_staffDefMensur || m_staffDefMeterSig || m_staffDefMeterSigGrp);
    }
    ///@}

    /**
     * Set drawing caution clef, keysig, mensur, metersig if necessary and if available.
     */
    ///@{
    void SetDrawingCautionValues(StaffDef *currentStaffDef);

    bool DrawCautionKeySigCancel() const { return m_drawCautionKeySigCancel; }
    void SetDrawCautionKeySigCancel(bool drawCautionKeySig) { m_drawCautionKeySigCancel = drawCautionKeySig; }

    Clef *GetCautionStaffDefClef() { return m_cautionStaffDefClef; }
    const Clef *GetCautionStaffDefClef() const { return m_cautionStaffDefClef; }
    KeySig *GetCautionStaffDefKeySig() { return m_cautionStaffDefKeySig; }
    const KeySig *GetCautionStaffDefKeySig() const { return m_cautionStaffDefKeySig; }
    Mensur *GetCautionStaffDefMensur() { return m_cautionStaffDefMensur; }
    const Mensur *GetCautionStaffDefMensur() const { return m_cautionStaffDefMensur; }
    MeterSig *GetCautionStaffDefMeterSig() { return m_cautionStaffDefMeterSig; }
    const MeterSig *GetCautionStaffDefMeterSig() const { return m_cautionStaffDefMeterSig; }

    bool HasCautionStaffDef() const
    {
        return (
            m_cautionStaffDefClef || m_cautionStaffDefKeySig || m_cautionStaffDefMensur || m_cautionStaffDefMeterSig);
    }
    ///@}

    /**
     * @name Setter and getter for the cross-staff flags
     */
    //@{
    void SetCrossStaffFromAbove(bool crossStaff) { m_crossStaffFromAbove = crossStaff; }
    bool HasCrossStaffFromAbove() const { return m_crossStaffFromAbove; }
    void SetCrossStaffFromBelow(bool crossStaff) { m_crossStaffFromBelow = crossStaff; }
    bool HasCrossStaffFromBelow() const { return m_crossStaffFromBelow; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
    //
private:
    /**
     * The drawing stem direction of the layer based on the number of layers in the staff
     */
    data_STEMDIRECTION m_drawingStemDir;

    /**
     * Two flags indicating when a layer is also used from cross-staff content from below or above
     */
    bool m_crossStaffFromBelow;
    bool m_crossStaffFromAbove;

    /** */
    Clef *m_staffDefClef;
    KeySig *m_staffDefKeySig;
    Mensur *m_staffDefMensur;
    MeterSig *m_staffDefMeterSig;
    MeterSigGrp *m_staffDefMeterSigGrp;
    bool m_drawKeySigCancellation;

    /** */
    Clef *m_cautionStaffDefClef;
    KeySig *m_cautionStaffDefKeySig;
    Mensur *m_cautionStaffDefMensur;
    MeterSig *m_cautionStaffDefMeterSig;
    bool m_drawCautionKeySigCancel;
};

} // namespace vrv

#endif
