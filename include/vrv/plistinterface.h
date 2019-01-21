/////////////////////////////////////////////////////////////////////////////
// Name:        plistinterface.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PLIST_INTERFACE_H__
#define __VRV_PLIST_INTERFACE_H__

#include "atts_shared.h"
#include "vrvdef.h"

namespace vrv {

class Object;
class FunctorParams;

//----------------------------------------------------------------------------
// PlistInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having a single time point, such as tempo, reh, etc..
 * It is not an abstract class but should not be instantiated directly.
 */
class PlistInterface : public Interface, public AttPlist {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    PlistInterface();
    virtual ~PlistInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_PLIST; }
    ///@}

    /**
     * Add a reference ref to the AttPlist vector (if not already there)
     */
    void AddRef(std::string ref);

    /**
     * Add a references, not checking if it is already in the list (for expansion@plist).
     */
    void AddRefAllowDuplicate(const std::string &ref);

    /**
     * Set a reference object when the uuid is found in the m_uuids.
     * Calls IsValidRef to check that the type of object is valid.
     */
    void SetRef(Object *object);

    const ArrayOfObjects *GetRefs() { return &m_references; }

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the  Object (implementation) to
     * the functor method. These not called by the Process/Call loop but by the implementaion
     * classes explicitely. See FloatingObject::FillStaffCurrentTimeSpanning for an example.
     */

    /**
     * See Object::PreparePlist
     */
    virtual int InterfacePreparePlist(FunctorParams *functorParams, Object *object);

    /**
     * See Object::ResetDrawing
     */
    virtual int InterfaceResetDrawing(FunctorParams *functorParams, Object *object);

protected:
    /**
     * Extract the fragment of the any uris given in @plist
     */
    void SetUuidStrs();

    /**
     * Method to be redefined in the child class if specific validation is required.
     * The method is called from PlistInterface::SetRef
     */
    virtual bool IsValidRef(Object *ref) { return true; }

private:
    //
public:
    //
protected:
private:
    /**
     * An array of resolved references.
     * Filled in InterfacePreparePlist (backward).
     */
    ArrayOfObjects m_references;

    /**
     * An array of parsed any uris stored as uuids.
     * Filled in InterfacePreparePlist (backward and forward).
     */
    std::vector<std::string> m_uuids;
};

} // namespace vrv

#endif
