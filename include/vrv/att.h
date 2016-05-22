/////////////////////////////////////////////////////////////////////////////
// Name:        att.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATT_H__
#define __VRV_ATT_H__

#include <string>

//----------------------------------------------------------------------------

#include "attconverter.h"
#include "vrvdef.h"

namespace vrv {

class Object;

//----------------------------------------------------------------------------
// Att
//----------------------------------------------------------------------------

/**
 * This is the base class for all MEI att classes.
 * It is not an abstract class but it should not be instanciated directly.
 * The att classes are generated with the libmei parser for Verovio.
*/
class Att : public AttConverter {
public:
    /** @name Constructors and destructor */
    ///@{
    Att();
    virtual ~Att();
    ///@}

    /**
     * @name static method for blind attribute modification
     * The implementation is implemented by LibMEI in each module corresponding file
     * Use in the toolkit for applying attribute modification to unspecified elements
     * See Toolkit::Set method
     * Files to be uncommented according to the inclusion of the corresponding LibMEI files
     */
    ///@{
    // static bool SetAnalysis(Object *element, std::string attrType, std::string attrValue);
    static bool SetCmn(Object *element, std::string attrType, std::string attrValue);
    // static bool SetCmnornaments(Object *element, std::string attrType, std::string attrValue);
    static bool SetCritapp(Object *element, std::string attrType, std::string attrValue);
    // static bool SetEdittrans(Object *element, std::string attrType, std::string attrValue);
    // static bool SetFacsimile(Object *element, std::string attrType, std::string attrValue);
    // static bool SetFigtable(Object *element, std::string attrType, std::string attrValue);
    // static bool SetHarmony(Object *element, std::string attrType, std::string attrValue);
    // static bool SetHeader(Object *element, std::string attrType, std::string attrValue);
    // static bool SetLyrics(Object *element, std::string attrType, std::string attrValue);
    static bool SetMei(Object *element, std::string attrType, std::string attrValue);
    static bool SetMensural(Object *element, std::string attrType, std::string attrValue);
    // static bool SetMidi(Object *element, std::string attrType, std::string attrValue);
    static bool SetPagebased(Object *element, std::string attrType, std::string attrValue);
    // static bool SetPerformance(Object *element, std::string attrType, std::string attrValue);
    // static bool SetNeumes(Object *element, std::string attrType, std::string attrValue);
    static bool SetShared(Object *element, std::string attrType, std::string attrValue);
    // static bool SetTablature(Object *element, std::string attrType, std::string attrValue);
    // static bool SetUsersymbols(Object *element, std::string attrType, std::string attrValue);

    /**
     * Idem for getting attributes as strings
     */
    // static void GetAnalysis(Object *element, ArrayOfStrAttr *attributes);
    static void GetCmn(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetCmnornaments(Object *element, ArrayOfStrAttr *attributes);
    static void GetCritapp(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetEdittrans(Object *element, ArrayOfStrAttr *attributes);
    // static void GetFacsimile(Object *element, ArrayOfStrAttr *attributes);
    // static void GetFigtable(Object *element, ArrayOfStrAttr *attributes);
    // static void GetHarmony(Object *element, ArrayOfStrAttr *attributes);
    // static void GetHeader(Object *element, ArrayOfStrAttr *attributes);
    // static void GetLyrics(Object *element, ArrayOfStrAttr *attributes);
    static void GetMei(const Object *element, ArrayOfStrAttr *attributes);
    static void GetMensural(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetMidi(Object *element, ArrayOfStrAttr *attributes);
    static void GetPagebased(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetPerformance(Object *element, ArrayOfStrAttr *attributes);
    // static void GetNeumes(Object *element, ArrayOfStrAttr *attributes);
    static void GetShared(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetTablature(Object *element, ArrayOfStrAttr *attributes);
    // static void GetUsersymbols(Object *element, ArrayOfStrAttr *attributes);
    ///@}

public:
    /** Dummy string converter */
    std::string StrToStr(std::string str) const;

    /** @name Basic converters for writing */
    ///@{
    std::string DblToStr(double data) const;
    std::string IntToStr(int data) const;
    ///@}

    /** @name Basic converters for reading */
    ///@{
    double StrToDbl(std::string value) const;
    int StrToInt(std::string value) const;
    ///@}

    /** @name Converters for writing and reading */
    ///@{
    std::string BeatrptRendToStr(data_BEATRPT_REND data) const;
    data_BEATRPT_REND StrToBeatrptRend(std::string value) const;

    std::string DurationToStr(data_DURATION data) const;
    data_DURATION StrToDuration(std::string value) const;

    std::string KeysignatureToStr(data_KEYSIGNATURE data) const;
    data_KEYSIGNATURE StrToKeysignature(std::string value) const;

    std::string MeasurebeatToStr(data_MEASUREBEAT data) const;
    data_MEASUREBEAT StrToMeasurebeat(std::string value) const;

    std::string ModusmaiorToStr(data_MODUSMAIOR data) const;
    data_MODUSMAIOR StrToModusmaior(std::string value) const;

    std::string ModusminorToStr(data_MODUSMINOR data) const;
    data_MODUSMINOR StrToModusminor(std::string value) const;

    std::string OctaveDisToStr(data_OCTAVE_DIS data) const;
    data_OCTAVE_DIS StrToOctaveDis(std::string value) const;

    std::string OrientationToStr(data_ORIENTATION data) const;
    data_ORIENTATION StrToOrientation(std::string value) const;

    std::string PitchnameToStr(data_PITCHNAME data) const;
    data_PITCHNAME StrToPitchname(std::string value) const;

    std::string ProlatioToStr(data_PROLATIO data) const;
    data_PROLATIO StrToProlatio(std::string value) const;

    std::string StemdirectionToStr(data_STEMDIRECTION data) const;
    data_STEMDIRECTION StrToStemdirection(std::string value) const;

    std::string TempusToStr(data_TEMPUS data) const;
    data_TEMPUS StrToTempus(std::string value) const;

    std::string TieToStr(data_TIE data) const;
    data_TIE StrToTie(std::string value) const;

    std::string XsdPosintlistToStr(xsd_posIntList data) const;
    xsd_posIntList StrToXsdPosintlist(std::string value) const;
    ///@}
};

//----------------------------------------------------------------------------
// Interface
//----------------------------------------------------------------------------

/**
 * This is a base class for regrouping MEI att classes.
 * It is not an abstract class but it should not be instanciated directly.
 * The inherited classes should override the InterfaceId method for returning
 * their own InterfaceId.
 */

class Interface {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Interface(){};
    virtual ~Interface(){};
    ///@}

    /**
     * Method for registering an MEI att classes in the interface.
     */
    void RegisterInterfaceAttClass(AttClassId attClassId) { m_interfaceAttClasses.push_back(attClassId); };

    /**
     * Method for obtaining a pointer to the attribute class vector of the interface
     */
    std::vector<AttClassId> *GetAttClasses() { return &m_interfaceAttClasses; };

    /**
     * Virtual method returning the InterfaceId of the interface.
     * Needs to be overridden in child classes.
     */
    virtual InterfaceId IsInterface() { return INTERFACE; };

private:
    /**
     * A vector for storing all the MEI att classes grouped in the interface
     */
    std::vector<AttClassId> m_interfaceAttClasses;
};

//----------------------------------------------------------------------------
// AttComparison
//----------------------------------------------------------------------------

class AttComparison {

public:
    AttComparison(ClassId classId) { m_classId = classId; };

    virtual bool operator()(Object *object);

    ClassId GetType() { return m_classId; };

    bool MatchesType(Object *object);

protected:
    ClassId m_classId;
};

} // namespace vrv

#endif
