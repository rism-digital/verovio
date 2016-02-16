/////////////////////////////////////////////////////////////////////////////
// Name:        timeinterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIME_INTERFACE_H__
#define __VRV_TIME_INTERFACE_H__

#include "atts_shared.h"
#include "vrvdef.h"

namespace vrv {

class DocObject;
class LayerElement;
class Measure;

//----------------------------------------------------------------------------
// TimePointInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having a single time point, such as tempo, reh, etc..
 * It is not an abstract class but should not be instanciate directly.
 */
class TimePointInterface : public Interface, public AttStaffident, public AttStartid, public AttTimestampMusical {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    TimePointInterface();
    virtual ~TimePointInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_TIME_POINT; };
    ///@}

    /**
     * @name Set and get the first LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetStart(LayerElement *start);
    LayerElement *GetStart() { return m_start; };
    ///@}

    /**
     * Return true if a start is given (@startid or @tstamp)
     */
    bool HasStart() { return (m_start); };

    /**
     * Return the start measure of the TimePointInterface
     */
    Measure *GetStartMeasure();

    /**
     * Return true if the TimePointInterface occurs on the staff @n
     * Looks that the parent staff if the using @stardid or at the @staff values.
     */
    bool IsOnStaff(int n);

    /**
     * Return a vector of staves looking at the @staff attribute or at the parent staff or the @startid
     */
    std::vector<Staff *> GetTstampStaves(Measure *measure);

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the DocObject (implementation) to
     * the functor method. These not called by the Process/Call loop but by the implementaion
     * classes explicitely. See FloatingElement::FillStaffCurrentTimeSpanning for an example.
     */

    /**
     * See Object::PrepareTimestamps
     */
    virtual int InterfacePrepareTimestamps(ArrayPtrVoid *params, DocObject *object);

    /**
     * See Object::ResetDrawing
     */
    virtual int InterfaceResetDrawing(ArrayPtrVoid *params, DocObject *object);

protected:
    /**
     * Extract the fragment of the start or end @xml:id if given
     */
    void SetUuidStr();

    /**
     * Extract the uuid from a string
     */
    std::string ExtractUuidFragment(std::string refUuid);

private:
    //
public:
    //
protected:
    LayerElement *m_start;
    std::string m_startUuid;

private:
};

//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for spanning elements, such as slur, hairpin, etc..
 * It is not an abstract class but should not be instanciate directly.
 */
class TimeSpanningInterface : public TimePointInterface, public AttStartendid, public AttTimestamp2Musical {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    TimeSpanningInterface();
    virtual ~TimeSpanningInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_TIME_SPANNING; };
    ///@}

    /**
     * @name Set and get the first and second LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetEnd(LayerElement *end);
    LayerElement *GetEnd() { return m_end; };
    ///@}

    /**
     *
     */
    bool SetStartAndEnd(LayerElement *element);

    /**
     *
     */
    bool HasStartAndEnd() { return (m_start && m_end); };

    /**
     * Return the end measure of the TimePointInterface
     */
    Measure *GetEndMeasure();

    /**
     * Return true if the element is spanning over two or more measures
     */
    bool IsSpanningMeasures();

    /**
     *
     */
    void SetUuidStr();

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the DocObject (implementation) to
     * the functor method. These not called by the Process/Call loop but by the implementaion
     * classes explicitely. See FloatingElement::FillStaffCurrentTimeSpanning for an example.
     */

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int InterfaceFillStaffCurrentTimeSpanning(ArrayPtrVoid *params, DocObject *object);

    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int InterfacePrepareTimeSpanning(ArrayPtrVoid *params, DocObject *object);

    /**
     * See Object::PrepareTimestamps
     */
    virtual int InterfacePrepareTimestamps(ArrayPtrVoid *params, DocObject *object);

    /**
     * See Object::ResetDrawing
     */
    virtual int InterfaceResetDrawing(ArrayPtrVoid *params, DocObject *object);

private:
    //
public:
    //
private:
    LayerElement *m_end;
    std::string m_endUuid;
};

} // namespace vrv

#endif
