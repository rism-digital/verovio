/////////////////////////////////////////////////////////////////////////////
// Name:        timeinterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIME_INTERFACE_H__
#define __VRV_TIME_INTERFACE_H__

#include "atts_shared.h"
#include "interface.h"
#include "vrvdef.h"

namespace vrv {

class Object;
class LayerElement;
class Measure;
class PrepareStaffCurrentTimeSpanningFunctor;
class PrepareTimePointingFunctor;
class PrepareTimeSpanningFunctor;
class PrepareTimestampsFunctor;
class ResetDataFunctor;
class StaffAlignment;

//----------------------------------------------------------------------------
// TimePointInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having a single time point, such as tempo, reh, etc..
 * It is not an abstract class but should not be instantiated directly.
 */
class TimePointInterface : public Interface,
                           public AttPartIdent,
                           public AttStaffIdent,
                           public AttStartId,
                           public AttTimestampLog {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    TimePointInterface();
    virtual ~TimePointInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_TIME_POINT; }
    ///@}

    /**
     * @name Set and get the first LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetStart(LayerElement *start);
    LayerElement *GetStart() { return m_start; }
    const LayerElement *GetStart() const { return m_start; }
    ///@}

    /**
     * Set first LayerElement by verifying it is the correct one.
     */
    bool SetStartOnly(LayerElement *start);

    /**
     * Add a staff n to the AttStaffident vector (if not already there)
     */
    void AddStaff(int n);

    /**
     * Return true if a start is given (@startid or @tstamp)
     */
    bool HasStart() const { return (m_start); }

    /**
     * Return the start measure of the TimePointInterface
     */
    ///@{
    Measure *GetStartMeasure();
    const Measure *GetStartMeasure() const;
    ///@}

    /**
     * Return true if the TimePointInterface occurs on the staff @n
     * Looks that the parent staff if the using @stardid or at the @staff values.
     */
    bool IsOnStaff(int n) const;

    /**
     * Return a vector of staves looking at the @staff attribute or at the parent staff of the @startid
     */
    ///@{
    std::vector<const Staff *> GetTstampStaves(const Measure *measure, const Object *object) const;
    std::vector<Staff *> GetTstampStaves(const Measure *measure, const Object *object);
    ///@}

    /**
     * Return true if the interface owner is encoded in the measure of its start element
     * Display a warning if not
     */
    bool VerifyMeasure(const Object *owner) const;

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor code in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the object (implementation) to
     * the pseudo functor method.
     */
    ///@{
    FunctorCode InterfacePrepareTimePointing(PrepareTimePointingFunctor &functor, Object *object);
    virtual FunctorCode InterfacePrepareTimestamps(PrepareTimestampsFunctor &functor, Object *object);
    virtual FunctorCode InterfaceResetData(ResetDataFunctor &functor, Object *object);
    ///@}

protected:
    /**
     * Extract the fragment of the start or end \@xml:id if given
     */
    void SetIDStr();

private:
    //
public:
    //
protected:
    LayerElement *m_start;
    std::string m_startID;

private:
};

//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for spanning elements, such as slur, hairpin, etc..
 * It is not an abstract class but should not be instantiated directly.
 */
class TimeSpanningInterface : public TimePointInterface, public AttStartEndId, public AttTimestamp2Log {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    TimeSpanningInterface();
    virtual ~TimeSpanningInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_TIME_SPANNING; }
    ///@}

    /**
     * @name Set and get the first and second LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetEnd(LayerElement *end);
    LayerElement *GetEnd() { return m_end; }
    const LayerElement *GetEnd() const { return m_end; }
    ///@}

    /**
     *
     */
    bool SetStartAndEnd(LayerElement *element);

    /**
     *
     */
    bool HasStartAndEnd() const { return (m_start && m_end); }

    /**
     * Return the end measure of the TimePointInterface
     */
    ///@{
    Measure *GetEndMeasure();
    const Measure *GetEndMeasure() const;
    ///@}

    /**
     * Return true if the element is spanning over two or more measures
     */
    bool IsSpanningMeasures() const;

    /**
     *
     */
    void SetIDStr();

    /**
     * Check if the end points are temporally ordered
     * @Return true if end temporally occurs after start
     */
    ///@{
    bool IsOrdered() const;
    bool IsOrdered(const LayerElement *start, const LayerElement *end) const;
    ///@}

    /**
     * Check if the slur or tie needs to be taken into account as overflow above or below in case of cross-staff end
     * points. This methods assumes staff@n to be greater for the staff below.
     */

    void GetCrossStaffOverflows(
        StaffAlignment *alignment, curvature_CURVEDIR curveDir, bool &skipAbove, bool &skipBelow);

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor code in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the object (implementation) to
     * the pseudo functor method.
     */
    ///@{
    FunctorCode InterfacePrepareTimeSpanning(PrepareTimeSpanningFunctor &functor, Object *object);
    FunctorCode InterfacePrepareTimestamps(PrepareTimestampsFunctor &functor, Object *object) override;
    FunctorCode InterfacePrepareStaffCurrentTimeSpanning(
        PrepareStaffCurrentTimeSpanningFunctor &functor, Object *object);
    FunctorCode InterfaceResetData(ResetDataFunctor &functor, Object *object) override;
    ///@}

private:
    //
public:
    //
private:
    LayerElement *m_end;
    std::string m_endID;
};

} // namespace vrv

#endif
