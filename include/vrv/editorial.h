/////////////////////////////////////////////////////////////////////////////
// Name:        editorial.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITORIAL_H__
#define __VRV_EDITORIAL_H__

#include "atts_critapp.h"
#include "atts_shared.h"
#include "drawinginterface.h"
#include "object.h"
#include "systemmilestone.h"

namespace vrv {

class Layer;
class LayerElement;
class Measure;
class ControlElement;
class ScoreDef;
class Staff;
class StaffDef;
class StaffGrp;
class TextElement;

//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

/**
 * This class is a base class for the editorial element containing musical
 * content, for example <rgd> or <add>.
 * It is not an abstract class but should not be instantiated directly.
 * It can be both a container (in score-based MEI) and a milestone (in page-based MEI).
 */
class EditorialElement : public Object,
                         public VisibilityDrawingInterface,
                         public SystemMilestoneInterface,
                         public AttLabelled,
                         public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    EditorialElement();
    EditorialElement(ClassId classId);
    virtual ~EditorialElement();
    void Reset() override;
    ///@}

    /**
     * @name Add children to an editorial element.
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    VisibilityDrawingInterface *GetVisibilityDrawingInterface() override
    {
        return vrv_cast<VisibilityDrawingInterface *>(this);
    }
    const VisibilityDrawingInterface *GetVisibilityDrawingInterface() const override
    {
        return vrv_cast<const VisibilityDrawingInterface *>(this);
    }
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
};

} // namespace vrv

#endif
