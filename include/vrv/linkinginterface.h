/////////////////////////////////////////////////////////////////////////////
// Name:        linkinginterface.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LINKING_INTERFACE_H__
#define __VRV_LINKING_INTERFACE_H__

#include "atts_shared.h"
#include "vrvdef.h"

namespace vrv {

class FunctorParams;
class Measure;
class Object;

//----------------------------------------------------------------------------
// LinkingInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having a link
 * It is not an abstract class but should not be instantiated directly.
 */
class LinkingInterface : public Interface, public AttLinking {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    LinkingInterface();
    virtual ~LinkingInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_LINKING; }
    ///@}

    /**
     * @name Set and get the @next, @sameas, ... object
     * The setter asserts that no Object was previously set.
     */
    ///@{
    void SetNextLink(Object *next);
    Object *GetNextLink() { return m_next; }
    const Object *GetNextLink() const { return m_next; }
    void SetSameasLink(Object *sameas);
    Object *GetSameasLink() { return m_sameas; }
    const Object *GetSameasLink() const { return m_sameas; }
    ///@}

    /**
     * Return true if a object is given and resolved
     */
    ///@{
    bool HasNextLink() const { return (m_next); }
    bool HasSameasLink() const { return (m_sameas); }
    ///@}

    /**
     * Return the start measure of the next object (if any, NULL otherwise)
     */
    ///@{
    Measure *GetNextMeasure();
    const Measure *GetNextMeasure() const;
    ///@}

    /**
     * Set @corresp attribute to the ID (or @corresp) of the object
     */
    void AddBackLink(const Object *object);

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
     * See Object::PrepareStaffCurrentTimeSpanning
     */
    virtual int InterfacePrepareStaffCurrentTimeSpanning(FunctorParams *functorParams, Object *object);

    /**
     * See Object::PrepareLinking
     */
    virtual int InterfacePrepareLinking(FunctorParams *functorParams, Object *object);

    /**
     * See Object::ResetData
     */
    virtual int InterfaceResetData(FunctorParams *functorParams, Object *object);

protected:
    /**
     * Extract the fragment of the start or end @xml:id if given
     */
    void SetIDStr();

private:
    //
public:
    //
private:
    Object *m_next;
    std::string m_nextID;
    Object *m_sameas;
    std::string m_sameasID;
};

} // namespace vrv

#endif
