/////////////////////////////////////////////////////////////////////////////
// Name:        functorparams.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FUNCTOR_PARAMS_H__
#define __VRV_FUNCTOR_PARAMS_H__

//----------------------------------------------------------------------------

#include "doc.h"
#include "vrvdef.h"

namespace smf {
class MidiFile;
}

namespace vrv {

class Artic;
class SystemMilestoneInterface;
class Chord;
class ClassIdComparison;
class Clef;
class Doc;
class Dot;
class Dots;
class Dynam;
class Ending;
class Output;
class Facsimile;
class FeatureExtractor;
class Functor;
class Hairpin;
class Harm;
class KeySig;
class LabelAbbr;
class Layer;
class LayerElement;
class Measure;
class MeasureAligner;
class Mensur;
class MeterSig;
class MeterSigGrp;
class MRpt;
class Object;
class Page;
class Pedal;
class ScoreDef;
class ScoreDefElement;
class Slur;
class Staff;
class StaffAlignment;
class StaffDef;
class StemmedDrawingInterface;
class Syl;
class SymbolTable;
class System;
class SystemAligner;
class Timemap;
class Transposer;
class TupletNum;
class Turn;
class Verse;

//----------------------------------------------------------------------------
// FunctorParams
//----------------------------------------------------------------------------

/**
 * This is the base class for all MEI att classes.
 * It is not an abstract class but it should not be instanciated directly.
 * The att classes are generated with the libmei parser for Verovio.
 */

class FunctorParams {
public:
    FunctorParams() {}
    virtual ~FunctorParams() {}
};

//----------------------------------------------------------------------------
// Child classes of FunctorParams
//----------------------------------------------------------------------------

/**
 * This is a basic FunctorParams with only the doc pointer for cases where
 * it is the only parameter needed.
 * member 0: the Doc
 **/

class FunctorDocParams : public FunctorParams {
public:
    FunctorDocParams(Doc *doc) { m_doc = doc; }
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// FindLayerIDWithinStaffDefParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the element inside Layer StaffDef
 * member 1: ID of element to be found
 **/

class FindLayerIDWithinStaffDefParams : public FunctorParams {
public:
    explicit FindLayerIDWithinStaffDefParams(const std::string &xmlId)
    {
        m_id = xmlId;
        m_object = NULL;
    }

    const Object *m_object;
    std::string m_id;
};

//----------------------------------------------------------------------------
// GenerateFeaturesParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the Doc
 * member 1: a pointer to the FeatureExtractor to which extraction is delegated
 **/

class GenerateFeaturesParams : public FunctorParams {
public:
    GenerateFeaturesParams(Doc *doc, FeatureExtractor *extractor)
    {
        m_doc = doc;
        m_extractor = extractor;
    }
    Doc *m_doc;
    FeatureExtractor *m_extractor;
};

//----------------------------------------------------------------------------
// TransposeParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the document
 * member 1: the functor for redirection
 * member 2: the end functor for redirection
 * member 3: a pointer to the transposer
 * member 4: the transposition to be applied
 * member 5: the mdiv selected for transposition
 * member 6: the list of current (nested) mdivs
 * member 7: transpose to sounding pitch by evaluating @trans.semi
 * member 8: current KeySig for staff (ScoreDef key signatures are mapped to -1)
 * member 9: transposition interval for staff
 **/

class TransposeParams : public FunctorParams {
public:
    TransposeParams(Doc *doc, Functor *functor, Functor *functorEnd, Transposer *transposer)
    {
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
        m_transposer = transposer;
        m_transposeToSoundingPitch = false;
    }
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
    Transposer *m_transposer;
    std::string m_transposition;
    std::string m_selectedMdivID;
    std::list<std::string> m_currentMdivIDs;
    bool m_transposeToSoundingPitch;
    std::map<int, const KeySig *> m_keySigForStaffN;
    std::map<int, int> m_transposeIntervalForStaffN;
};

} // namespace vrv

#endif
