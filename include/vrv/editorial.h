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
#include "boundary.h"
#include "object.h"

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
 * It can be both a container (in score-based MEI) and a boundary (in page-based MEI).
 */
class EditorialElement : public Object, public BoundaryStartInterface, public AttLabelled, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    EditorialElement();
    EditorialElement(const std::string &classid);
    virtual ~EditorialElement();
    virtual void Reset();
    virtual ClassId GetClassId() const { return EDITORIAL_ELEMENT; }
    ///@}

    /**
     * @name Add children to an editorial element.
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    virtual int ConvertToPageBased(FunctorParams *functorParams);
    virtual int ConvertToPageBasedEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareBoundaries
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::CastOffSystems
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

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
