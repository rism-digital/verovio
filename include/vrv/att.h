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
class Att
{
public:
    /** @name Constructors and destructor */
    ///@{
    Att( );
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
    //static bool SetAnalysis( Object *element, std::string attrType, std::string attrValue );
    static bool SetCmn( Object *element, std::string attrType, std::string attrValue );
    //static bool SetCmnornaments( Object *element, std::string attrType, std::string attrValue );
    static bool SetCritapp( Object *element, std::string attrType, std::string attrValue );
    //static bool SetEdittrans( Object *element, std::string attrType, std::string attrValue );
    //static bool SetFacsimile( Object *element, std::string attrType, std::string attrValue );
    //static bool SetFigtable( Object *element, std::string attrType, std::string attrValue );
    //static bool SetHarmony( Object *element, std::string attrType, std::string attrValue );
    //static bool SetHeader( Object *element, std::string attrType, std::string attrValue );
    //static bool SetLinkalign( Object *element, std::string attrType, std::string attrValue );
    //static bool SetLyrics( Object *element, std::string attrType, std::string attrValue );
    static bool SetMensural( Object *element, std::string attrType, std::string attrValue );
    //static bool SetMidi( Object *element, std::string attrType, std::string attrValue );
    static bool SetPagebased( Object *element, std::string attrType, std::string attrValue );
    //static bool SetNeumes( Object *element, std::string attrType, std::string attrValue );
    static bool SetShared( Object *element, std::string attrType, std::string attrValue );
    //static bool SetTablature( Object *element, std::string attrType, std::string attrValue );

    /**
     * Idem for getting attributes as strings
     */
    //static void GetAnalysis( Object *element, ArrayOfStrAttr *attributes );
    static void GetCmn( Object *element, ArrayOfStrAttr *attributes );
    //static void GetCmnornaments( Object *element, ArrayOfStrAttr *attributes );
    static void GetCritapp( Object *element, ArrayOfStrAttr *attributes );
    //static void GetEdittrans( Object *element, ArrayOfStrAttr *attributes );
    //static void GetFacsimile( Object *element, ArrayOfStrAttr *attributes );
    //static void GetFigtable( Object *element, ArrayOfStrAttr *attributes );
    //static void GetHarmony( Object *element, ArrayOfStrAttr *attributes );
    //static void GetHeader( Object *element, ArrayOfStrAttr *attributes );
    //static void GetLinkalign( Object *element, ArrayOfStrAttr *attributes );
    //static void GetLyrics( Object *element, ArrayOfStrAttr *attributes );
    static void GetMensural( Object *element, ArrayOfStrAttr *attributes );
    //static void GetMidi( Object *element, ArrayOfStrAttr *attributes );
    static void GetPagebased( Object *element, ArrayOfStrAttr *attributes );
    //static void GetNeumes( Object *element, ArrayOfStrAttr *attributes );
    static void GetShared( Object *element, ArrayOfStrAttr *attributes );
    //static void GetTablature( Object *element, ArrayOfStrAttr *attributes );
    ///@}
    
public:
    /** Dummy string converter */
    std::string StrToStr(std::string str);
    
    /** @name Basic converters for writing */
    ///@{
    std::string DblToStr(double data);
    std::string IntToStr(int data);
    ///@}
    
    /** @name Basic converters for reading */
    ///@{
    double StrToDbl(std::string value);
    int StrToInt(std::string value);
    
    /** @name Converters for writing and reading */
    ///@{
    std::string AccidentalExplicitToStr(data_ACCIDENTAL_EXPLICIT data);
    data_ACCIDENTAL_EXPLICIT StrToAccidentalExplicit(std::string value);
    
    std::string AccidentalImplicitToStr(data_ACCIDENTAL_IMPLICIT data);
    data_ACCIDENTAL_IMPLICIT StrToAccidentalImplicit(std::string value);
    
    std::string BarRenditionToStr(data_BARRENDITION data);
    data_BARRENDITION StrToBarRendition(std::string value);
    
    std::string BeatrptRendToStr(data_BEATRPT_REND data);
    data_BEATRPT_REND StrToBeatrptRend(std::string value);
    
    std::string BoolToStr(data_BOOLEAN data);
    data_BOOLEAN StrToBool(std::string value);
    
    std::string ClefShapeToStr(data_CLEFSHAPE data);
    data_CLEFSHAPE StrToClefShape(std::string value);
    
    std::string ConToStr(data_CON data);
    data_CON StrToCon(std::string value);
    
    std::string CurvedirToStr(data_CURVEDIR data);
    data_CURVEDIR StrToCurvedir(std::string value);
    
    std::string DurToStr(data_DURATION data);
    data_DURATION StrToDur(std::string value);
    
    std::string FontstyleToStr(data_FONTSTYLE data);
    data_FONTSTYLE StrToFontstyle(std::string value);
    
    std::string FontweightToStr(data_FONTWEIGHT data);
    data_FONTWEIGHT StrToFontweight(std::string value);
    
    std::string FuncToStr(data_FUNC data);
    data_FUNC StrToFunc(std::string value);
    
    std::string GraceToStr(data_GRACE data);
    data_GRACE StrToGrace(std::string value);
        
    std::string KeySignatureToStr(data_KEYSIGNATURE data);
    data_KEYSIGNATURE StrToKeySignature(std::string value);
    
    std::string LayerschemeToStr(data_LAYERSCHEME data);
    data_LAYERSCHEME StrToLayerscheme(std::string value);
    
    std::string LigatureToStr(data_LIGATURE data);
    data_LIGATURE StrToLigature(std::string value);
    
    std::string MensurationSignToStr(data_MENSURATIONSIGN data);
    data_MENSURATIONSIGN StrToMensurationSign(std::string value);
    
    std::string MeterSignToStr(data_METERSIGN data);
    data_METERSIGN StrToMeterSign(std::string value);
    
    std::string ModeToStr(data_MODE data);
    data_MODE StrToMode(std::string value);

    std::string ModusmaiorToStr(data_MODUSMAIOR data);
    data_MODUSMAIOR StrToModusmaior(std::string value);
    
    std::string ModusminorToStr(data_MODUSMINOR data);
    data_MODUSMINOR StrToModusminor(std::string value);
    
    std::string NumformatToStr(data_NUMFORMAT data);
    data_NUMFORMAT StrToNumformat(std::string value);
    
    std::string PercentToStr(data_PERCENT data);
    data_PERCENT StrToPercent(std::string value);

    std::string OctaveDisToStr(data_OCTAVE_DIS data);
    data_OCTAVE_DIS StrToOctaveDis(std::string value);
    
    std::string OrientationToStr(data_ORIENTATION data);
    data_ORIENTATION StrToOrientation(std::string value);
    
    std::string PitchNameToStr(data_PITCHNAME data);
    data_PITCHNAME StrToPitchName(std::string value);
    
    std::string PlaceToStr(data_PLACE data);
    data_PLACE StrToPlace(std::string value);
    
    std::string ProlatioToStr(data_PROLATIO data);
    data_PROLATIO StrToProlatio(std::string value);

    std::string StaffRelToStr(data_STAFFREL data);
    data_STAFFREL StrToStaffRel(std::string value);
    
    std::string StemDirectionToStr(data_STEMDIRECTION data);
    data_STEMDIRECTION StrToStemDirection(std::string value);
    
    std::string StemModifierToStr(data_STEMMODIFIER data);
    data_STEMMODIFIER StrToStemModifier(std::string value);
    
    std::string StemPositionToStr(data_STEMPOSITION data);
    data_STEMPOSITION StrToStemPosition(std::string value);
    
    std::string SymbolToStr(data_SYMBOL data);
    data_SYMBOL StrToSymbol(std::string value);
    
    std::string TempusToStr(data_TEMPUS data);
    data_TEMPUS StrToTempus(std::string value);
    
    std::string TieToStr(data_TIE data);
    data_TIE StrToTie(std::string value);
    
    std::string Tstamp2ToStr(typedef_TSTAMP2 data);
    typedef_TSTAMP2 StrToTstamp2(std::string value);
    
    std::string WordPosToStr(data_WORDPOS data);
    data_WORDPOS StrToWordPos(std::string value);
  
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
    
class Interface
{
    
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Interface() {};
    virtual ~Interface() {};
    ///@}
    
    /**
     * Method for registering an MEI att classes in the interface.
     */
    void RegisterInterfaceAttClass( AttClassId attClassId ) { m_interfaceAttClasses.push_back( attClassId ); };
    
    /**
     * Method for obtaining a pointer to the attribute class vector of the interface
     */
    std::vector<AttClassId> *GetAttClasses() { return &m_interfaceAttClasses; };
    
    /**
     * Virtual method returning the InterfaceId of the interface.
     * Needs to be overriden in child classes.
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

class AttComparison
{
    
public:
    AttComparison( ClassId classId ) {
        m_classId = classId;
    };
    
    virtual bool operator() (Object *object);
    
    ClassId GetType() { return m_classId; };
    
    bool MatchesType( Object *object );
    
protected:
    ClassId m_classId;
};
    
} // namespace vrv

#endif
