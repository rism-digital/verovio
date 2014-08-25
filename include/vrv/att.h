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

#include "attdef.h"

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
    
    /** @name Basic converters for reading */
    ///@{
    double StrToDbl(std::string value);
    int StrToInt(std::string value);
    
    /** @name Converters for writing and reading */
    ///@{
    std::string AccidentalImplicitToStr(data_ACCIDENTAL_IMPLICIT data);
    data_ACCIDENTAL_IMPLICIT StrToAccidentalImplicit(std::string value);
    
    std::string BarRenditionToStr(data_BARRENDITION data);
    data_BARRENDITION StrToBarRendition(std::string value);
    
    std::string BoolToStr(data_BOOLEAN data);
    data_BOOLEAN StrToBool(std::string value);
    
    std::string ClefShapeToStr(data_CLEFSHAPE data);
    data_CLEFSHAPE StrToClefShape(std::string value);
    
    std::string ConToStr(data_CON data);
    data_CON StrToCon(std::string value);
        
    std::string KeySignatureToStr(data_KEYSIGNATURE data);
    data_KEYSIGNATURE StrToKeySignature(std::string value);
    
    std::string LigatureToStr(data_LIGATURE data);
    data_LIGATURE StrToLigature(std::string value);
    
    std::string MensurationSignToStr(data_MENSURATIONSIGN data);
    data_MENSURATIONSIGN StrToMensurationSign(std::string value);
    
    std::string MeterSignToStr(data_METERSIGN data);
    data_METERSIGN StrToMeterSign(std::string value);
    
    std::string ModeToStr(data_MODE data);
    data_MODE StrToMode(std::string value);
    
    std::string OctaveDisToStr(data_OCTAVE_DIS data);
    data_OCTAVE_DIS StrToOctaveDis(std::string value);
    
    std::string OrientationToStr(data_ORIENTATION data);
    data_ORIENTATION StrToOrientation(std::string value);
    
    std::string PitchNameToStr(data_PITCHNAME data);
    data_PITCHNAME StrToPitchName(std::string value);
    
    std::string PlaceToStr(data_PLACE data);
    data_PLACE StrToPlace(std::string value);
    
    std::string StemDirectionToStr(data_STEMDIRECTION data);
    data_STEMDIRECTION StrToStemDirection(std::string value);
    
    std::string StemPositionToStr(data_STEMPOSITION data);
    data_STEMPOSITION StrToStemPosition(std::string value);
    
    std::string WordPosToStr(data_WORDPOS data);
    data_WORDPOS StrToWordPos(std::string value);
  
    ///@}

};
    
} // namespace vrv

#endif
