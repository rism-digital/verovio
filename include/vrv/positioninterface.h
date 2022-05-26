/////////////////////////////////////////////////////////////////////////////
// Name:        positioninterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_POSITION_INTERFACE_H__
#define __VRV_POSITION_INTERFACE_H__

#include "atts_shared.h"

namespace vrv {

class FunctorParams;
class Layer;
class LayerElement;

//----------------------------------------------------------------------------
// PositionInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements with a position on the staff, such as rests.
 * It is not an abstract class but should not be instanciated directly.
 * For now, the position is handled in a similar way that for PitchInterface, that is with a pitch and octave.
 */
class PositionInterface : public Interface, public AttStaffLoc, public AttStaffLocPitched {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    PositionInterface();
    virtual ~PositionInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_POSITION; }
    ///@}

    /**
     * @name Setter and getter for the drawing staff loc.
     * This is set by the CalcAlignmentPitchPos functor.
     */
    ///@{
    void SetDrawingLoc(int drawingLoc) { m_drawingLoc = drawingLoc; }
    int GetDrawingLoc() const { return m_drawingLoc; }
    ///@}

    int CalcDrawingLoc(const Layer *layer, const LayerElement *element);

    /**
     * Inteface comparison operator.
     * Checks if the LayerElement has a PositionInterface and compares attributes
     */
    bool HasIdenticalPositionInterface(const PositionInterface *otherPositionInterface) const;

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * See Object::ResetData
     */
    virtual int InterfaceResetData(FunctorParams *functorParams, Object *object);

    /**
     * See Object::ResetData
     */
    virtual int InterfaceResetHorizontalAlignment(FunctorParams *functorParams, Object *object);

private:
    //
public:
    //
private:
    /**
     * The drawing location of the object
     */
    int m_drawingLoc;
};

} // namespace vrv

#endif
