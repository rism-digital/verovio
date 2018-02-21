/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// NOTE: this file was generated with the Verovio libmei version and
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATTS_HEADER_H__
#define __VRV_ATTS_HEADER_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttRecordType
//----------------------------------------------------------------------------

class AttRecordType : public Att {
public:
    AttRecordType();
    virtual ~AttRecordType();

    /** Reset the default values for the attribute class **/
    void ResetRecordType();

    /** Read the values for the attribute class **/
    bool ReadRecordType(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteRecordType(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetRecordtype(recordType_RECORDTYPE recordtype_) { m_recordtype = recordtype_; }
    recordType_RECORDTYPE GetRecordtype() const { return m_recordtype; }
    bool HasRecordtype() const;
    ///@}

private:
    /** --- **/
    recordType_RECORDTYPE m_recordtype;

    /* include <attrecordtype> */
};

//----------------------------------------------------------------------------
// AttRegularMethod
//----------------------------------------------------------------------------

class AttRegularMethod : public Att {
public:
    AttRegularMethod();
    virtual ~AttRegularMethod();

    /** Reset the default values for the attribute class **/
    void ResetRegularMethod();

    /** Read the values for the attribute class **/
    bool ReadRegularMethod(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteRegularMethod(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMethod(regularMethod_METHOD method_) { m_method = method_; }
    regularMethod_METHOD GetMethod() const { return m_method; }
    bool HasMethod() const;
    ///@}

private:
    /** Indicates the method employed to mark corrections and normalizations. **/
    regularMethod_METHOD m_method;

    /* include <attmethod> */
};

} // vrv namespace

#endif // __VRV_ATTS_HEADER_H__
