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

#define VERSION_MAJOR 0
#define VERSION_MINOR 9
#define VERSION_REVISION 14
// Adds "-dev" in the version number - should be set to false for releases
#define VERSION_DEV true

//----------------------------------------------------------------------------
// Object defines
//----------------------------------------------------------------------------

/**
 * The ClassIds are used to identify Object child classes through the Object::Is virtual method.
 * Each Object child class has to have its own id and has to override the Is() method.
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
    //
    ALIGNMENT,
    CLEF_ATTR,
    DOC,
    GRACE_ALIGNER,
    KEYSIG_ATTR,
    LAYER,
    MEASURE,
    MEASURE_ALIGNER,
    MENSUR_ATTR,
    METERSIG_ATTR,
    PAGE,
    SCORE,
    STAFF,
    STAFF_ALIGNMENT,
    STAFFGRP,
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
    REG,
    RESTORE,
    SIC,
    SUPPLIED,
    UNCLEAR,
    EDITORIAL_ELEMENT_max,
    // Ids for SystemElement child classes
    SYSTEM_ELEMENT,
    BOUNDARY_END,
    ENDING,
    PB,
    SB,
    SECTION,
    SYSTEM_ELEMENT_max,
    // Ids for ControlElement child classes
    CONTROL_ELEMENT,
    ANCHORED_TEXT,
    DIR,
    DYNAM,
    FERMATA,
    HAIRPIN,
    HARM,
    OCTAVE,
    PEDAL,
    SLUR,
    TEMPO,
    TIE,
    TRILL,
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
    BTREM,
    CHORD,
    CLEF,
    CUSTOS,
    DOT,
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
    NOTE,
    PROPORT,
    REST,
    SPACE,
    SYL,
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
    REND,
    TEXT,
    TEXT_ELEMENT_max,
    //
    BBOX_DEVICE_CONTEXT,
    SVG_DEVICE_CONTEXT,
    //
    UNSPECIFIED
};

/**
 * The InterfaceIds are used to identify Interface child classes through the Interface::IsInterface virtual method.
 * Each Interface child class has to have its own id and has to override the IsInterface() method.
 */
enum InterfaceId {
    INTERFACE,
    INTERFACE_BOUNDARY,
    INTERFACE_DURATION,
    INTERFACE_PITCH,
    INTERFACE_POSITION,
    INTERFACE_SCOREDEF,
    INTERFACE_TEXT_DIR,
    INTERFACE_TIME_POINT,
    INTERFACE_TIME_SPANNING
};

//----------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------

class AttComparison;
class BeamElementCoord;
class BoundingBox;
class FloatingPositioner;
class LayerElement;
class Note;
class Object;
class Point;
class Staff;
class TimePointInterface;
class TimeSpanningInterface;

typedef std::vector<Object *> ArrayOfObjects;

typedef std::list<Object *> ListOfObjects;

typedef std::vector<AttComparison *> ArrayOfAttComparisons;

typedef std::vector<Note *> ChordCluster;

typedef std::vector<BeamElementCoord *> ArrayOfBeamElementCoords;

typedef std::map<Staff *, std::vector<char> > MapOfLedgerLineFlags;

typedef std::vector<std::pair<LayerElement *, Point> > ArrayOfLayerElementPointPairs;

typedef std::vector<std::pair<Object *, data_MEASUREBEAT> > ArrayOfObjectBeatPairs;

typedef std::vector<std::pair<TimePointInterface *, ClassId> > ArrayOfPointingInterClassIdPairs;

typedef std::vector<std::pair<TimeSpanningInterface *, ClassId> > ArrayOfSpanningInterClassIdPairs;

typedef std::vector<FloatingPositioner *> ArrayOfFloatingPositioners;

typedef std::vector<BoundingBox *> ArrayOfBoundingBoxes;

//----------------------------------------------------------------------------
// Global defines
//----------------------------------------------------------------------------

#define DEFINITION_FACTOR 10
#define PARAM_DENOMINATOR 10

#define is_in(x, a, b) (((x) >= std::min((a), (b))) && ((x) <= std::max((a), (b))))

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

/** Define the maximum levels between a beam and its notes **/
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
    EDITORIAL_TEXT
};

//----------------------------------------------------------------------------
// Types for layer element
//----------------------------------------------------------------------------

/**
 * The types are used to distinguish LayerElement that are default layer elements,
 * scoreDef layer elements and cautionary scoreDef layer elements
 */

enum ElementScoreDefRole { NONE = 0, SYSTEM_SCOREDEF, INTERMEDIATE_SCOREDEF, CAUTIONARY_SCOREDEF };

//----------------------------------------------------------------------------
// Drawing groups (reserved values)
//----------------------------------------------------------------------------

/**
 * We need fix values for types that are all groupes together
 */

enum { DRAWING_GRP_NONE = 0, DRAWING_GRP_VERSE, DRAWING_GRP_HARM, DRAWING_GRP_OTHER };

//----------------------------------------------------------------------------
// Artic part types
//----------------------------------------------------------------------------

enum ArticPartType { ARTIC_PART_INSIDE = 0, ARTIC_PART_OUTSIDE };

//----------------------------------------------------------------------------
// Legacy Wolfgang defines
//----------------------------------------------------------------------------

#define OCTAVE_OFFSET 4

// The next four macros were tuned using the Leipzig font.

// Width (in half-drawing units) of an accidental; used to prevent overlap on complex chords
#define ACCID_WIDTH 4

// Height
#define ACCID_HEIGHT 12

// Only keeps track of this much of the top of flats so that their bottom can be drawn more concisely
// This can also be thought of as height(sharp)*F_B_H_M = height(flat)
#define FLAT_BOTTOM_HEIGHT_MULTIPLIER .75

// Ignores this much of the top/right of an accid for same purposes (empty space in top right of drawing)
#define FLAT_CORNER_HEIGHT_IGNORE .25
#define FLAT_CORNER_WIDTH_IGNORE .5
#define NATURAL_CORNER_HEIGHT_IGNORE .25
#define NATURAL_CORNER_WIDTH_IGNORE .5

// in half staff spaces (but should be 6 in two-voice notation)
#define STANDARD_STEMLENGTH 7

} // namespace vrv

#endif // __VRV_DEF_H__
