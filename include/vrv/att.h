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

#include "attalternates.h"
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
    static bool SetAnalytical(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetCmn(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetCmnornaments(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetCritapp(Object *element, const std::string &attrType, const std::string &attrValue);
    // static bool SetEdittrans(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetExternalsymbols(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetFacsimile(Object *element, const std::string &attrType, const std::string &attrValue);
    // static bool SetFigtable(Object *element, const std::string &attrType, const std::string &attrValue);
    // static bool SetFingering(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetGestural(Object *element, const std::string &attrType, const std::string &attrValue);
    // static bool SetHarmony(Object *element, const std::string &attrType, const std::string &attrValue);
    // static bool SetHeader(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetMei(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetMensural(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetMidi(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetNeumes(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetPagebased(Object *element, const std::string &attrType, const std::string &attrValue);
    // static bool SetPerformance(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetShared(Object *element, const std::string &attrType, const std::string &attrValue);
    // static bool SetUsersymbols(Object *element, const std::string &attrType, const std::string &attrValue);
    static bool SetVisual(Object *element, const std::string &attrType, const std::string &attrValue);

    /**
     * Idem for getting attributes as strings
     */
    static void GetAnalytical(const Object *element, ArrayOfStrAttr *attributes);
    static void GetCmn(const Object *element, ArrayOfStrAttr *attributes);
    static void GetCmnornaments(const Object *element, ArrayOfStrAttr *attributes);
    static void GetCritapp(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetEdittrans(const Object *element, ArrayOfStrAttr *attributes);
    static void GetExternalsymbols(const Object *element, ArrayOfStrAttr *attributes);
    static void GetFacsimile(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetFigtable(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetFingering(const Object *element, ArrayOfStrAttr *attributes);
    static void GetGestural(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetHarmony(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetHeader(const Object *element, ArrayOfStrAttr *attributes);
    static void GetMei(const Object *element, ArrayOfStrAttr *attributes);
    static void GetMensural(const Object *element, ArrayOfStrAttr *attributes);
    static void GetMidi(const Object *element, ArrayOfStrAttr *attributes);
    static void GetNeumes(const Object *element, ArrayOfStrAttr *attributes);
    static void GetPagebased(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetPerformance(const Object *element, ArrayOfStrAttr *attributes);
    static void GetShared(const Object *element, ArrayOfStrAttr *attributes);
    // static void GetUsersymbols(const Object *element, ArrayOfStrAttr *attributes);
    static void GetVisual(const Object *element, ArrayOfStrAttr *attributes);
    ///@}

    static data_ACCIDENTAL_WRITTEN AccidentalGesturalToWritten(data_ACCIDENTAL_GESTURAL accid);
    static data_ACCIDENTAL_GESTURAL AccidentalWrittenToGestural(data_ACCIDENTAL_WRITTEN accid);

    static data_STAFFREL StaffrelBasicToStaffrel(data_STAFFREL_basic staffrelBasic);
    static data_STAFFREL_basic StaffrelToStaffrelBasic(data_STAFFREL staffrel);

    static bool IsMensuralType(data_NOTATIONTYPE notationType);
    static bool IsNeumeType(data_NOTATIONTYPE notationType);
    static bool IsTabType(data_NOTATIONTYPE notationType);

public:
    /** Dummy string converter */
    std::string StrToStr(std::string str) const;

    /** @name Basic converters for writing */
    ///@{
    std::string DblToStr(double data) const;
    std::string IntToStr(int data) const;
    std::string VUToStr(data_VU data) const;
    ///@}

    /** @name Basic converters for reading */
    ///@{
    double StrToDbl(std::string value) const;
    int StrToInt(std::string value) const;
    data_VU StrToVU(std::string value, bool logWarning = true) const;
    ///@}

    /** @name Converters for writing and reading */
    ///@{
    std::string ArticulationListToStr(data_ARTICULATION_List data) const;
    data_ARTICULATION_List StrToArticulationList(std::string value, bool logWarning = true) const;

    std::string BeatrptRendToStr(data_BEATRPT_REND data) const;
    data_BEATRPT_REND StrToBeatrptRend(std::string value, bool logWarning = true) const;

    std::string DurationToStr(data_DURATION data) const;
    data_DURATION StrToDuration(std::string value, bool logWarning = true) const;

    std::string FontsizenumericToStr(data_FONTSIZENUMERIC data) const;
    data_FONTSIZENUMERIC StrToFontsizenumeric(std::string value, bool logWarning = true) const;

    std::string HexnumToStr(data_HEXNUM data) const;
    data_HEXNUM StrToHexnum(std::string value, bool logWarning = true) const;

    std::string KeysignatureToStr(data_KEYSIGNATURE data) const;
    data_KEYSIGNATURE StrToKeysignature(std::string value, bool logWarning = true) const;

    std::string MeasurebeatToStr(data_MEASUREBEAT data) const;
    data_MEASUREBEAT StrToMeasurebeat(std::string value, bool logWarning = true) const;

    std::string MeasurementabsToStr(data_MEASUREMENTABS data) const { return VUToStr(data); }
    data_MEASUREMENTABS StrToMeasurementabs(std::string value, bool logWarning = true) const
    {
        return StrToVU(value, logWarning);
    }

    std::string MeasurementrelToStr(data_MEASUREMENTREL data) const { return VUToStr(data); }
    data_MEASUREMENTREL StrToMeasurementrel(std::string value, bool logWarning = true) const
    {
        return StrToVU(value, logWarning);
    }

    std::string ModusmaiorToStr(data_MODUSMAIOR data) const;
    data_MODUSMAIOR StrToModusmaior(std::string value, bool logWarning = true) const;

    std::string ModusminorToStr(data_MODUSMINOR data) const;
    data_MODUSMINOR StrToModusminor(std::string value, bool logWarning = true) const;

    std::string MidibpmToStr(data_MIDIBPM data) const { return IntToStr(data); }
    data_MIDIBPM StrToMidibpm(std::string value) const { return StrToInt(value); }

    std::string MidichannelToStr(data_MIDICHANNEL data) const { return IntToStr(data); }
    data_MIDICHANNEL StrToMidichannel(std::string value) const { return StrToInt(value); }

    std::string MidimspbToStr(data_MIDIMSPB data) const { return IntToStr(data); }
    data_MIDIMSPB StrToMidimspb(std::string value) const { return StrToInt(value); }

    std::string MidivalueToStr(data_MIDIVALUE data) const { return IntToStr(data); }
    data_MIDIVALUE StrToMidivalue(std::string value) const { return StrToInt(value); }

    std::string NcnameToStr(data_NCNAME data) const { return StrToStr(data); }
    data_NCNAME StrToNcname(std::string value) const { return StrToStr(value); }

    std::string OctaveToStr(data_OCTAVE data) const { return IntToStr(data); }
    data_OCTAVE StrToOctave(std::string value) const { return StrToInt(value); }

    std::string OctaveDisToStr(data_OCTAVE_DIS data) const;
    data_OCTAVE_DIS StrToOctaveDis(std::string value, bool logWarning = true) const;

    std::string OrientationToStr(data_ORIENTATION data) const;
    data_ORIENTATION StrToOrientation(std::string value, bool logWarning = true) const;

    std::string PercentToStr(data_PERCENT data) const;
    data_PERCENT StrToPercent(std::string value, bool logWarning = true) const;

    std::string PercentLimitedToStr(data_PERCENT_LIMITED_SIGNED data) const;
    data_PERCENT_LIMITED StrToPercentLimited(std::string value, bool logWarning = true) const;

    std::string PercentLimitedSignedToStr(data_PERCENT_LIMITED data) const;
    data_PERCENT_LIMITED_SIGNED StrToPercentLimitedSigned(std::string value, bool logWarning = true) const;

    std::string PitchnameToStr(data_PITCHNAME data) const;
    data_PITCHNAME StrToPitchname(std::string value, bool logWarning = true) const;

    std::string ProlatioToStr(data_PROLATIO data) const;
    data_PROLATIO StrToProlatio(std::string value, bool logWarning = true) const;

    std::string TempusToStr(data_TEMPUS data) const;
    data_TEMPUS StrToTempus(std::string value, bool logWarning = true) const;

    std::string TieToStr(data_TIE data) const;
    data_TIE StrToTie(std::string value, bool logWarning = true) const;

    std::string XsdAnyURIListToStr(xsdAnyURI_List data) const;
    xsdAnyURI_List StrToXsdAnyURIList(std::string value) const;

    std::string XsdPositiveIntegerListToStr(xsdPositiveInteger_List data) const;
    xsdPositiveInteger_List StrToXsdPositiveIntegerList(std::string value) const;
    ///@}

    /** @name Converters for writing and reading alternate data types not generated by LibMEI */
    ///@{
    std::string FontsizeToStr(data_FONTSIZE data) const;
    data_FONTSIZE StrToFontsize(std::string value, bool logWarning = true) const;

    std::string LinewidthToStr(data_LINEWIDTH data) const;
    data_LINEWIDTH StrToLinewidth(std::string value, bool logWarning = true) const;

    std::string MidivalueNameToStr(data_MIDIVALUE_NAME data) const;
    data_MIDIVALUE_NAME StrToMidivalueName(std::string value, bool logWarning = true) const;

    std::string MidivaluePanToStr(data_MIDIVALUE_PAN data) const;
    data_MIDIVALUE_PAN StrToMidivaluePan(std::string value, bool logWarning = true) const;
    ///@}

    /** @name Converters for writing and reading alternate data types unsing other alternate data types */
    ///@{
    std::string PlacementToStr(data_PLACEMENT data) const;
    data_PLACEMENT StrToPlacement(std::string value, bool logWarning = true) const;
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
    void RegisterInterfaceAttClass(AttClassId attClassId) { m_interfaceAttClasses.push_back(attClassId); }

    /**
     * Method for obtaining a pointer to the attribute class vector of the interface
     */
    std::vector<AttClassId> *GetAttClasses() { return &m_interfaceAttClasses; }

    /**
     * Virtual method returning the InterfaceId of the interface.
     * Needs to be overridden in child classes.
     */
    virtual InterfaceId IsInterface() { return INTERFACE; }

private:
    /**
     * A vector for storing all the MEI att classes grouped in the interface
     */
    std::vector<AttClassId> m_interfaceAttClasses;
};

} // namespace vrv

#endif
