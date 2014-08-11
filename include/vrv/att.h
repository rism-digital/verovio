/////////////////////////////////////////////////////////////////////////////
// Name:        att.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATT_H__
#define __VRV_ATT_H__

#include "attdef.h"

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// Att
//----------------------------------------------------------------------------

/**
 * This base class for all MEI att classes.
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
    
protected:
    /** Dummy string converter */
    std::string StrToStr(std::string str);
    
    /** @name Basic converters for writing */
    ///@{
    std::string DblToStr(double data);
    std::string IntToStr(int data);
    ///@}
    
    /** @name Converters for writing */
    ///@{
    std::string AccidentalImplicitToStr(data_ACCIDENTAL_IMPLICIT data);
    std::string BarRenditionToStr(data_BARRENDITION data);
    std::string BoolToStr(data_BOOLEAN data);
    std::string ClefShapeToStr(data_CLEFSHAPE data);
    std::string KeySignatureToStr(data_KEYSIGNATURE data);
    std::string MensurationSignToStr(data_MENSURATIONSIGN data);
    std::string MeterSignToStr(data_METERSIGN data);
    std::string ModeToStr(data_MODE data);
    std::string OctaveDisToStr(data_OCTAVE_DIS data);
    std::string OrientationToStr(data_ORIENTATION data);
    std::string PitchNameToStr(data_PITCHNAME data);
    std::string PlaceToStr(data_PLACE data);
    ///@}

    /** @name Basic converters for reading */
    ///@{
    double StrToDbl(std::string value);
    int StrToInt(std::string value);
    
    /** @name Converters for reading */
    ///@{
    data_ACCIDENTAL_IMPLICIT StrToAccidentalImplicit(std::string value);
    data_BARRENDITION StrToBarRendition(std::string value);
    data_BOOLEAN StrToBool(std::string value);
    data_CLEFSHAPE StrToClefShape(std::string value);
    data_KEYSIGNATURE StrToKeySignature(std::string value);
    data_MENSURATIONSIGN StrToMensurationSign(std::string value);
    data_METERSIGN StrToMeterSign(std::string value);
    data_MODE StrToMode(std::string value);
    data_OCTAVE_DIS StrToOctaveDis(std::string value);
    data_ORIENTATION StrToOrientation(std::string value);
    data_PITCHNAME StrToPitchName(std::string value);
    data_PLACE StrToPlace(std::string value);
    ///@}

};
    
} // namespace vrv

#endif
