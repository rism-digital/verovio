/////////////////////////////////////////////////////////////////////////////
// Name:        vrvdef.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DEF_H__
#define __VRV_DEF_H__

#include <algorithm>
#include <list>
#include <map>
#include <vector>

//----------------------------------------------------------------------------

#include "attdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Version
//----------------------------------------------------------------------------

#define VERSION_MAJOR 2
#define VERSION_MINOR 0
#define VERSION_REVISION 0
// Adds "-dev" in the version number - should be set to false for releases
#define VERSION_DEV true

enum MEIVersion { MEI_UNDEFINED = 0, MEI_2013, MEI_3_0_0, MEI_4_0_0 };

//----------------------------------------------------------------------------
// Object defines
//----------------------------------------------------------------------------

/**
 * The ClassIds are used to identify Object child classes through the Object::Is virtual method.
 * Each Object child class has to have its own id and has to override the GetClassId() method.
 * Base classes (e.g., LayerElement) that are never instanciated have boundary ids
 * used for checking if an Object is child of a base class. See for example
 * Object::IsLayerElement.
 */
enum ClassId {
    BOUNDING_BOX = 0, // Should not be instanciated as is
    OBJECT, // Should not be instanciated as is
    DEVICE_CONTEXT, // Should not be instanciated as is,
    FLOATING_OBJECT,
    FLOATING_POSITIONER,
    // Ids for ungrouped objects
    ALIGNMENT,
    ALIGNMENT_REFERENCE,
    CLEF_ATTR,
    DOC,
    FB,
    GRACE_ALIGNER,
    INSTRDEF,
    KEYSIG_ATTR,
    LABEL,
    LABELABBR,
    LAYER,
    MDIV,
    MEASURE,
    MEASURE_ALIGNER,
    MENSUR_ATTR,
    METERSIG_ATTR,
    PAGE,
    PAGES,
    SCORE,
    STAFF,
    STAFF_ALIGNMENT,
    STAFFGRP,
    SVG,
    SYSTEM,
    SYSTEM_ALIGNER,
    SYSTEM_ALIGNMENT,
    TIMESTAMP_ALIGNER,
    // Ids for EditorialElement child classes
    EDITORIAL_ELEMENT,
    ABBR,
    ADD,
    ANNOT,
    APP,
    CHOICE,
    CORR,
    DAMAGE,
    DEL,
    EXPAN,
    LEM,
    ORIG,
    RDG,
    REF,
    REG,
    RESTORE,
    SIC,
    SUPPLIED,
    UNCLEAR,
    EDITORIAL_ELEMENT_max,
    // Ids for RunningElement child classes
    RUNNING_ELEMENT,
    PGFOOT,
    PGFOOT2,
    PGHEAD,
    PGHEAD2,
    RUNNING_ELEMENT_max,
    // Ids for SystemElement child classes
    SYSTEM_ELEMENT,
    BOUNDARY_END,
    ENDING,
    EXPANSION,
    PB,
    SB,
    SECTION,
    SYSTEM_ELEMENT_max,
    // Ids for ControlElement child classes
    CONTROL_ELEMENT,
    ANCHORED_TEXT,
    ARPEG,
    BREATH,
    DIR,
    DYNAM,
    FERMATA,
    HAIRPIN,
    HARM,
    MORDENT,
    MNUM,
    OCTAVE,
    PEDAL,
    SLUR,
    TEMPO,
    TIE,
    TRILL,
    TURN,
    CONTROL_ELEMENT_max,
    // Ids for LayerElement child classes
    LAYER_ELEMENT,
    ACCID,
    ARTIC,
    ARTIC_PART,
    BARLINE,
    BARLINE_ATTR_LEFT,
    BARLINE_ATTR_RIGHT,
    BEAM,
    BEATRPT,
    BRACKET,
    BTREM,
    CHORD,
    CLEF,
    CUSTOS,
    DOT,
    DOTS,
    FLAG,
    FTREM,
    KEYSIG,
    LIGATURE,
    MENSUR,
    METERSIG,
    MREST,
    MRPT,
    MRPT2,
    MULTIREST,
    MULTIRPT,
    NC,
    NOTE,
    NEUME,
    TUPLET_NUM,
    PROPORT,
    REST,
    SPACE,
    STEM,
    SYL,
    SYLLABLE,
    TIMESTAMP_ATTR,
    TUPLET,
    VERSE,
    LAYER_ELEMENT_max,
    // Ids for ScoreDefElement child classes
    SCOREDEF_ELEMENT,
    SCOREDEF,
    STAFFDEF,
    SCOREDEF_ELEMENT_max,
    // Ids for TextElement child classes
    TEXT_ELEMENT,
    FIG,
    FIGURE,
    LB,
    NUM,
    REND,
    TEXT,
    TEXT_ELEMENT_max,
    //
    BBOX_DEVICE_CONTEXT,
    SVG_DEVICE_CONTEXT,
    CUSTOM_DEVICE_CONTEXT,
    //
    UNSPECIFIED
};

/**
 * The InterfaceIds are used to identify Interface child classes through the Interface::IsInterface virtual method.
 * Each Interface child class has to have its own id and has to override the IsInterface() method.
 */
enum InterfaceId {
    INTERFACE,
    INTERFACE_AREA_POS,
    INTERFACE_BOUNDARY,
    INTERFACE_DURATION,
    INTERFACE_PITCH,
    INTERFACE_PLIST,
    INTERFACE_POSITION,
    INTERFACE_SCOREDEF,
    INTERFACE_TEXT_DIR,
    INTERFACE_TIME_POINT,
    INTERFACE_TIME_SPANNING
};

//----------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------

class Alignment;
class Arpeg;
class AttComparison;
class BeamElementCoord;
class BoundingBox;
class Comparison;
class FloatingPositioner;
class GraceAligner;
class InterfaceComparison;
class LayerElement;
class LedgerLine;
class Nc;
class Note;
class Neume;
class Object;
class PlistInterface;
class Point;
class Staff;
class Option;
class TextElement;
class TimePointInterface;
class TimeSpanningInterface;

typedef std::vector<Object *> ArrayOfObjects;

typedef std::vector<Object *> ListOfObjects;

typedef std::vector<Comparison *> ArrayOfComparisons;

typedef std::vector<Note *> ChordCluster;

typedef std::vector<std::tuple<Alignment *, Alignment *, int> > ArrayOfAdjustmentTuples;

typedef std::vector<std::tuple<Alignment *, Arpeg *, int, bool> > ArrayOfAligmentArpegTuples;

typedef std::vector<BeamElementCoord *> ArrayOfBeamElementCoords;

typedef std::vector<std::pair<PlistInterface *, std::string> > ArrayOfInterfaceUuidPairs;

typedef std::vector<std::pair<LayerElement *, Point> > ArrayOfLayerElementPointPairs;

typedef std::vector<std::pair<Object *, data_MEASUREBEAT> > ArrayOfObjectBeatPairs;

typedef std::vector<std::pair<TimePointInterface *, ClassId> > ArrayOfPointingInterClassIdPairs;

typedef std::vector<std::pair<TimeSpanningInterface *, ClassId> > ArrayOfSpanningInterClassIdPairs;

typedef std::vector<FloatingPositioner *> ArrayOfFloatingPositioners;

typedef std::vector<BoundingBox *> ArrayOfBoundingBoxes;

typedef std::vector<LedgerLine> ArrayOfLedgerLines;

typedef std::vector<TextElement *> ArrayOfTextElements;

typedef std::map<Staff *, std::list<int> > MapOfDotLocs;

typedef std::map<std::string, Option *> MapOfStrOptions;

typedef std::map<int, GraceAligner *> MapOfIntGraceAligners;

//----------------------------------------------------------------------------
// Global defines
//----------------------------------------------------------------------------

#define DEFINITION_FACTOR 10

#define isIn(x, a, b) (((x) >= std::min((a), (b))) && ((x) <= std::max((a), (b))))

#define durRound(dur) round(dur *pow(10, 8)) / pow(10, 8)

/**
 * Codes returned by Functors.
 * Default is FUNCTOR_CONTINUE.
 * FUNCTOR_SIBLINGS won't go any deeper in the tree.
 * FUNCTOR_STOP wil stop the tree processing.
 */
enum FunctorCode { FUNCTOR_CONTINUE = 0, FUNCTOR_SIBLINGS, FUNCTOR_STOP };

//----------------------------------------------------------------------------
// Maximum number of levels between parent and children for optimizing search
//----------------------------------------------------------------------------

/** All values set to -1 (no limit) since this has not major incidence **/

/** Define the maximum levels between a note and its accids **/
#define MAX_ACCID_DEPTH -1

/** Define the maximum levels between a beam and its notes **/
#define MAX_BEAM_DEPTH -1

/** Define the maximum levels between a chord and its children **/
#define MAX_CHORD_DEPTH -1

/** Define the maximum levels between a fTrem and its notes **/
#define MAX_FTREM_DEPTH -1

/** Define the maximum levels between a ligature and its notes **/
#define MAX_LIGATURE_DEPTH -1

/** Define the maximum levels between a tuplet and its notes **/
#define MAX_TUPLET_DEPTH -1

/** Define the maximum levels of staffGrp within a scoreDef **/
#define MAX_STAFFGRP_DEPTH -1

/** Define the maximum levels between a note and its syls **/
#define MAX_NOTE_DEPTH -1

//----------------------------------------------------------------------------
// VerovioText codepoints
//----------------------------------------------------------------------------

/**
 * These are SMuFL codepoints for the VerovioText font used for embedding
 * SMuFL text glyph within text, as for example with <annot> or <syl>
 * Verovio uses a very small subset of glyph defined below (for now)
 */

#define VRV_TEXT_E550 0xE550
#define VRV_TEXT_E551 0xE551
#define VRV_TEXT_E552 0xE552

//----------------------------------------------------------------------------
// Types for editorial element
//----------------------------------------------------------------------------

// the maximum is 255 (unsigned char)
enum EditorialLevel {
    EDITORIAL_UNDEFINED = 0,
    EDITORIAL_TOPLEVEL,
    EDITORIAL_SCOREDEF,
    EDITORIAL_STAFFGRP,
    EDITORIAL_MEASURE,
    EDITORIAL_STAFF,
    EDITORIAL_LAYER,
    EDITORIAL_NOTE,
    EDITORIAL_TEXT,
    EDITORIAL_FB,
    EDITORIAL_RUNNING,
};

//----------------------------------------------------------------------------
// Visibility for editorial and mdiv elements
//----------------------------------------------------------------------------

enum VisibilityType { Hidden = 0, Visible };

//----------------------------------------------------------------------------
// The used SMuFL glyph anchors
//----------------------------------------------------------------------------

enum SMuFLGlyphAnchor {
    SMUFL_stemDownNW = 0,
    SMUFL_stemUpSE,
    SMUFL_cutOutNE,
    SMUFL_cutOutNW,
    SMUFL_cutOutSE,
    SMUFL_cutOutSW
};

//----------------------------------------------------------------------------
// Spanning types for control events
//----------------------------------------------------------------------------

enum { SPANNING_START_END = 0, SPANNING_START, SPANNING_END, SPANNING_MIDDLE };

//----------------------------------------------------------------------------
// Types for layer element
//----------------------------------------------------------------------------

/**
 * The types are used to distinguish LayerElement that are default layer elements,
 * scoreDef layer elements and cautionary scoreDef layer elements
 */

enum ElementScoreDefRole { NONE = 0, SYSTEM_SCOREDEF, INTERMEDIATE_SCOREDEF, CAUTIONARY_SCOREDEF };

//----------------------------------------------------------------------------
// Artic part types
//----------------------------------------------------------------------------

enum ArticPartType { ARTIC_PART_INSIDE = 0, ARTIC_PART_OUTSIDE };

//----------------------------------------------------------------------------
// Layout positions (3 x 3 grid)
//----------------------------------------------------------------------------

enum {
    POSITION_LEFT = 0,
    POSITION_CENTER,
    POSITION_RIGHT,
};

enum {
    POSITION_TOP = 0,
    POSITION_MIDDLE = 3,
    POSITION_BOTTOM = 6,
};

//----------------------------------------------------------------------------
// Legacy Wolfgang defines
//----------------------------------------------------------------------------

#define OCTAVE_OFFSET 4

// in half staff spaces (but should be 6 in two-voice notation)
#define STANDARD_STEMLENGTH 7

} // namespace vrv

#endif // __VRV_DEF_H__
