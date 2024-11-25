/////////////////////////////////////////////////////////////////////////////
// Name:        positioninterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_POSITION_INTERFACE_H__
#define __VRV_POSITION_INTERFACE_H__

#include "atts_shared.h"
#include "interface.h"

namespace vrv {

class Layer;
class LayerElement;
class ResetDataFunctor;
class ResetHorizontalAlignmentFunctor;

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
     * This is set by the CalcAlignmentPitchPosFunctor.
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

    /**
     * Check if the note has ledger lines.
     * If staff is passed, use it for getting the staff line number.
     * Otherwise, it will look for the Staff ancestor.
     * Set the value of ledger lines above or below.
     */
    bool HasLedgerLines(int &linesAbove, int &linesBelow, const Staff *staff) const;

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor code in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the object (implementation) to
     * the pseudo functor method.
     */
    ///@{
    FunctorCode InterfaceResetData(ResetDataFunctor &functor, Object *object);
    FunctorCode InterfaceResetHorizontalAlignment(ResetHorizontalAlignmentFunctor &functor, Object *object);
    ///@}

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
