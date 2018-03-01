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
              public AttNInteger,
              public AttTyped,
              public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Layer(int n = 1);
    virtual ~Layer();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Layer"; }
    virtual ClassId GetClassId() const { return LAYER; }
    ///@}

    /**
     * Do not copy children for layers
     */
    virtual bool CopyChildren() const { return false; }

    /**
     * Overriding CopyReset() method to be called after copy / assignment calls.
     */
    virtual void CopyReset();

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual void AddChild(Object *object);
    ///@}

    /**
     * Return the index position of the layer in its staff parent.
     * The index position is 0-based.
     */
    int GetLayerIdx() const { return Object::GetIdx(); }

    LayerElement *GetPrevious(LayerElement *element);
    LayerElement *GetAtPos(int x);
    LayerElement *Insert(LayerElement *element, int x); // return a pointer to the inserted element

    /**
     * Get the current clef for the test element.
     * Goes back on the layer until a clef is found.
     * This is used when inserting a note by passing a y position because we need
     * to know the clef in order to get the pitch.
     */
    Clef *GetClef(LayerElement *test);

    /**
     * Return the clef offset for the position x.
     * The method uses Layer::GetClef first to find the clef before test.
     */
    int GetClefLocOffset(LayerElement *test);

    /**
     * @name Set and get the stem direction of the layer.
     * This stays STEMDIRECTION_NONE with on single layer in the staff.
     */
    ///@{
    void SetDrawingStemDir(data_STEMDIRECTION stemDirection) { m_drawingStemDir = stemDirection; }
    data_STEMDIRECTION GetDrawingStemDir(LayerElement *element);
    data_STEMDIRECTION GetDrawingStemDir(const ArrayOfBeamElementCoords *coords);
    data_STEMDIRECTION GetDrawingStemDir(double time, double duration, Measure *measure, int staff);
    ///@}

    Clef *GetCurrentClef() const;
    KeySig *GetCurrentKeySig() const;
    Mensur *GetCurrentMensur() const;
    MeterSig *GetCurrentMeterSig() const;

    void ResetStaffDefObjects();

    /**
     * Set drawing clef, keysig and mensur if necessary and if available.
     */
    void SetDrawingStaffDefValues(StaffDef *currentStaffDef);

    bool DrawKeySigCancellation() const { return m_drawKeySigCancellation; }
    void SetDrawKeySigCancellation(bool drawKeySigCancellation) { m_drawKeySigCancellation = drawKeySigCancellation; }
    Clef *GetStaffDefClef() { return m_staffDefClef; }
    KeySig *GetStaffDefKeySig() { return m_staffDefKeySig; }
    Mensur *GetStaffDefMensur() { return m_staffDefMensur; }
    MeterSig *GetStaffDefMeterSig() { return m_staffDefMeterSig; }
    bool HasStaffDef() { return (m_staffDefClef || m_staffDefKeySig || m_staffDefMensur || m_staffDefMeterSig); }

    /**
     * Set drawing clef, keysig and mensur if necessary and if available.
     */
    void SetDrawingCautionValues(StaffDef *currentStaffDef);

    bool DrawCautionKeySigCancel() const { return m_drawCautionKeySigCancel; }
    void SetDrawCautionKeySigCancel(bool drawCautionKeySig) { m_drawCautionKeySigCancel = drawCautionKeySig; }
    Clef *GetCautionStaffDefClef() { return m_cautionStaffDefClef; }
    KeySig *GetCautionStaffDefKeySig() { return m_cautionStaffDefKeySig; }
    Mensur *GetCautionStaffDefMensur() { return m_cautionStaffDefMensur; }
    MeterSig *GetCautionStaffDefMeterSig() { return m_cautionStaffDefMeterSig; }
    bool HasCautionStaffDef()
    {
        return (
            m_cautionStaffDefClef || m_cautionStaffDefKeySig || m_cautionStaffDefMensur || m_cautionStaffDefMeterSig);
    }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToCastOffMensural
     */
    virtual int ConvertToCastOffMensural(FunctorParams *params);

    /**
     * See Object::ConvertToUnCastOffMensural
     */
    virtual int ConvertToUnCastOffMensural(FunctorParams *params);

    /**
     * See Object::UnsetCurrentScoreDef
     */
    virtual int UnsetCurrentScoreDef(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::AlignHorizontally
     */
    virtual int AlignHorizontally(FunctorParams *functorParams);

    /**
     * See Object::AlignHorizontallyEnd
     */
    virtual int AlignHorizontallyEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareProcessingLists
     */
    virtual int PrepareProcessingLists(FunctorParams *functorParams);

    /**
     * See Object::PrepareRpt
     */
    virtual int PrepareRpt(FunctorParams *functorParams);

    /**
     * See Object::CalcStem
     */
    virtual int CalcStem(FunctorParams *);

    /**
     * See Object::AdjustSylSpacing
     */
    virtual int AdjustSylSpacing(FunctorParams *functorParams);

    /**
     * See Object::CalcOnsetOffset
     */
    ///@{
    virtual int CalcOnsetOffset(FunctorParams *functorParams);
    ///@}

private:
    //
public:
    //
private:
    /**
     *
     */
    data_STEMDIRECTION m_drawingStemDir;

    /** */
    Clef *m_staffDefClef;
    KeySig *m_staffDefKeySig;
    Mensur *m_staffDefMensur;
    MeterSig *m_staffDefMeterSig;
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
