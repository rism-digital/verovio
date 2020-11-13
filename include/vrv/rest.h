/////////////////////////////////////////////////////////////////////////////
// Name:        rest.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REST_H__
#define __VRV_REST_H__

#include "atts_mensural.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

class TransPitch;

enum RestLayer { RL_UNSET = -1, RL_sameLayer, RL_otherLayer };

enum RestAccidental { RA_UNSET = -1, RA_none, RA_s, RA_f, RA_x, RA_n };

enum RestLayerPlace { RLP_UNSET = -1, RLP_restOnTopLayer, RLP_restOnBottomLayer };

enum RestNotePlace { RNP_UNSET = -1, RNP_noteInSpace, RNP_noteOnLine };

//----------------------------------------------------------------------------
// Rest
//----------------------------------------------------------------------------

/**
 * This class models the MEI <rest> element.
 */
class Rest : public LayerElement,
             public DurationInterface,
             public PositionInterface,
             public AttColor,
             public AttCue,
             public AttRestVisMensural {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rest();
    virtual ~Rest();
    virtual Object *Clone() const { return new Rest(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Rest"; }
    virtual ClassId GetClassId() const { return REST; }
    ///@}

    /**
     * Add an element to a rest.
     * Only Dots elements will be actually added to the rest.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Overwritten method for rest
     */
    virtual void AddChild(Object *object);

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PositionInterface *GetPositionInterface() { return dynamic_cast<PositionInterface *>(this); }
    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Get the SMuFL glyph or a rest considering its actual duration.
     * This is valid only for CMN and for duration shorter than half notes.
     */
    wchar_t GetRestGlyph() const;

    /**
     * Get the vertical offset for each glyph.
     */
    int GetRestLocOffset(int loc);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertMarkupAnalytical
     */
    virtual int ConvertMarkupAnalytical(FunctorParams *functorParams);

    /**
     * See Object::CalcDots
     */
    virtual int CalcDots(FunctorParams *functorParams);

    /**
     * See Object::PrepareLayerElementParts
     */
    virtual int PrepareLayerElementParts(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::Transpose
     */
    virtual int Transpose(FunctorParams *);

    /**
     * Get the vertical location for the rests that are located on other layers
     */
    int GetOptimalLayerLocation(Staff *staff, Layer *layer, int defaultLocation);

private:
    /**
     * Helper function to update rest oloc/ploc based on the input TransPitch
     */
    void UpdateFromTransLoc(const TransPitch &tp);

    /**
     * Get the rest vertical location relative to location of elements placed on other layers
     */
    std::pair<int, RestAccidental> GetLocationRelativeToOtherLayers(
        const ListOfObjects &layersList, Layer *currentLayer);

    /**
     * Get the rest vertical location relative to location of elements placed on current layers
     */
    int GetLocationRelativeToCurrentLayer(Staff *currentStaff, Layer *currentLayer, bool isTopLayer);

    /**
     * Get location of first/last element of the corresponding layer
     */
    int GetFirstRelativeElementLocation(Staff *currentStaff, Layer *currentLayer, bool isPrevious, bool isTopLayer);

    /**
     * Get location of the object on the layer if it's note, chord or ftrem
     */
    std::pair<int, RestAccidental> GetElementLocation(Object *object, Layer *layer, bool isTopLayer);

    /**
     * Get correct offset for the rest from the options based on layer and location
     */
    int GetRestOffsetFromOptions(
        RestLayer layer, const std::pair<int, RestAccidental> &location, bool isTopLayer) const;

public:
    //
private:
};

} // namespace vrv

#endif
