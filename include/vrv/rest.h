/////////////////////////////////////////////////////////////////////////////
// Name:        rest.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REST_H__
#define __VRV_REST_H__

#include "altsyminterface.h"
#include "atts_externalsymbols.h"
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
             public AltSymInterface,
             public DurationInterface,
             public PositionInterface,
             public AttColor,
             public AttCue,
             public AttExtSymAuth,
             public AttExtSymNames,
             public AttRestVisMensural {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rest();
    virtual ~Rest();
    Object *Clone() const override { return new Rest(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "rest"; }
    ///@}

    /**
     * Add an element to a rest.
     * Only Dots elements will be actually added to the rest.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Overwritten method for rest
     */
    void AddChild(Object *object) override;

    /**
     * @name Getter to interfaces
     */
    ///@{
    AltSymInterface *GetAltSymInterface() override { return vrv_cast<AltSymInterface *>(this); }
    const AltSymInterface *GetAltSymInterface() const override { return vrv_cast<const AltSymInterface *>(this); }
    PositionInterface *GetPositionInterface() override { return vrv_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override { return vrv_cast<const PositionInterface *>(this); }
    DurationInterface *GetDurationInterface() override { return vrv_cast<DurationInterface *>(this); }
    const DurationInterface *GetDurationInterface() const override { return vrv_cast<const DurationInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Get the SMuFL glyph or a rest considering its actual duration.
     * This is valid only for CMN.
     */
    ///@{
    char32_t GetRestGlyph() const;
    char32_t GetRestGlyph(const data_DURATION duration) const;
    ///@}

    /**
     * Get the vertical location for the rests that are located on other layers
     */
    int GetOptimalLayerLocation(const Staff *staff, const Layer *layer, int defaultLocation) const;

    /**
     * Update the rest location based on the input TransPitch
     */
    void UpdateFromTransLoc(const TransPitch &tp);

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
    /**
     * Get the rest vertical location relative to location of elements placed on other layers
     */
    std::pair<int, RestAccidental> GetLocationRelativeToOtherLayers(
        const Layer *currentLayer, bool isTopLayer, bool &restOverlap) const;

    /**
     * Get the rest vertical location relative to location of elements placed on current layers
     */
    int GetLocationRelativeToCurrentLayer(const Staff *currentStaff, const Layer *currentLayer, bool isTopLayer) const;

    /**
     * Get location of first/last element of the corresponding layer
     */
    int GetFirstRelativeElementLocation(
        const Staff *currentStaff, const Layer *currentLayer, bool isPrevious, bool isTopLayer) const;

    /**
     * For two layers, top layer shouldn't go below center and lower layer shouldn't go above it. Enforce this by
     * adding margin that will adjust rest position
     */
    int GetMarginLayerLocation(bool isTopLayer, bool restOverlap) const;

    /**
     * Get location of the object on the layer if it's note, chord or ftrem
     */
    std::pair<int, RestAccidental> GetElementLocation(const Object *object, const Layer *layer, bool isTopLayer) const;

    /**
     * Get correct offset for the rest from the options based on layer and location
     */
    int GetRestOffsetFromOptions(
        RestLayer layer, const std::pair<int, RestAccidental> &location, bool isTopLayer) const;

    /**
     * Find whether there is correct number of rests for automatic placement and determine position of the rest
     * (top/bottom) if there is.
     */
    bool DetermineRestPosition(const Staff *staff, const Layer *layer, bool &isTopLayer) const;

public:
    //
private:
};

} // namespace vrv

#endif
