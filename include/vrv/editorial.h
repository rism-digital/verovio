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
class EditorialElement : public Object, public SystemMilestoneInterface, public AttLabelled, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    EditorialElement();
    EditorialElement(ClassId classId);
    EditorialElement(ClassId classId, const std::string &classIdStr);
    virtual ~EditorialElement();
    void Reset() override;
    ///@}

    /**
     * @name Add children to an editorial element.
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::Save
     */
    ///@{
    int Save(FunctorParams *functorParams) override;
    int SaveEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::ConvertToPageBased
     */
    int ConvertToPageBased(FunctorParams *functorParams) override;
    int ConvertToPageBasedEnd(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareMilestones
     */
    int PrepareMilestones(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffSystems
     */
    int CastOffSystems(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffEncoding
     */
    int CastOffEncoding(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffToSelection
     */
    int CastOffToSelection(FunctorParams *) override;

private:
    //
public:
    /**
     * Holds the visibility (hidden or visible) for an editorial element.
     * By default, all editorial elements are visible. However, in an <app>, only one <rdg> is visible at the time. When
     * loading the file, the first <rdg> (or the <lem>) is made visible.
     */
    VisibilityType m_visibility;

private:
};

} // namespace vrv

#endif
