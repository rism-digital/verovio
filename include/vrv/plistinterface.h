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
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_PLIST; }
    ///@}

    /**
     * Add a reference ref to the AttPlist vector (if not already there)
     */
    void AddRef(const std::string &ref);

    /**
     * Add a reference, not checking if it is already in the list (for expansion@plist).
     */
    void AddRefAllowDuplicate(const std::string &ref);

    /**
     * Set a reference object when the id is found in the m_ids.
     * Calls IsValidRef to check that the type of object is valid.
     */
    void SetRef(const Object *object);

    /**
     * Retrieve all reference objects
     */
    ///@{
    ArrayOfObjects GetRefs();
    const ArrayOfConstObjects &GetRefs() const { return m_references; }
    ///@}

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the  Object (implementation) to
     * the functor method. These not called by the Process/Call loop but by the implementaion
     * classes explicitely. See FloatingObject::PrepareStaffCurrentTimeSpanning for an example.
     */

    /**
     * See Object::PreparePlist
     */
    virtual int InterfacePreparePlist(FunctorParams *functorParams, Object *object);

    /**
     * See Object::ResetData
     */
    virtual int InterfaceResetData(FunctorParams *functorParams, Object *object);

protected:
    /**
     * Extract the fragment of the any URIs given in @plist
     */
    void SetIDStrs();

    /**
     * Method to be redefined in the child class if specific validation is required.
     * The method is called from PlistInterface::SetRef
     */
    virtual bool IsValidRef(const Object *ref) const { return true; }

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
    ArrayOfConstObjects m_references;

    /**
     * An array of parsed any URIs stored as ids.
     * Filled in InterfacePreparePlist (backward and forward).
     */
    std::vector<std::string> m_ids;
};

} // namespace vrv

#endif
