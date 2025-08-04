/////////////////////////////////////////////////////////////////////////////
// Name:        vrvdef.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DEF_H__
#define __VRV_DEF_H__

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <vector>

//----------------------------------------------------------------------------

#include "attdef.h"

#define VRV_UNSET MEI_UNSET

//----------------------------------------------------------------------------

#define _USE_MATH_DEFINES // needed by Windows for math constants like "M_PI"
#include <math.h>

// In case it is not defined before...
#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

namespace vrv {

//----------------------------------------------------------------------------
// Version
//----------------------------------------------------------------------------

#define VERSION_MAJOR 5
#define VERSION_MINOR 5
#define VERSION_REVISION 0
// Adds "-dev" in the version number - should be set to false for releases
#define VERSION_DEV false

//----------------------------------------------------------------------------
// Resource directory
//----------------------------------------------------------------------------
#ifdef RESOURCE_DIR
#define VRV_RESOURCE_DIR RESOURCE_DIR
#else
#define VRV_RESOURCE_DIR "/usr/local/share/verovio"
#endif

//----------------------------------------------------------------------------
// Cast redefinition
//----------------------------------------------------------------------------

// Can be changed between static and dynamic for debug purposes
#ifdef VRV_DYNAMIC_CAST
// To be used for all cases where type is cheched through Object::m_type
#define vrv_cast dynamic_cast
#else
#define vrv_cast static_cast
#endif

//----------------------------------------------------------------------------
// Default MIDI values
//----------------------------------------------------------------------------

#define MIDI_VELOCITY 90
#define MIDI_TEMPO 120

#define UNACC_GRACENOTE_DUR 27 // in milliseconds
#define UNACC_GRACENOTE_FRACTION Fraction(1, 2048)

//----------------------------------------------------------------------------
// Object defines
//----------------------------------------------------------------------------

/**
 * The ClassIds are used to identify Object child classes through the Object::Is virtual method.
 * Each Object child class has to have its own id which is stored in the member variable m_classId.
 * Base classes (e.g., LayerElement) that are never instanciated have boundary ids
 * used for checking if an Object is child of a base class. See for example
 * Object::IsLayerElement.
 */
enum ClassId : uint16_t {
    BOUNDING_BOX = 0, // Should not be instanciated as is
    OBJECT, // Should not be instanciated as is
    DEVICE_CONTEXT, // Should not be instanciated as is,
    FLOATING_OBJECT,
    FLOATING_POSITIONER,
    FLOATING_CURVE_POSITIONER,
    // Ids for ungrouped objects
    ALIGNMENT,
    ALIGNMENT_REFERENCE,
    CLEF_ATTR,
    COURSE,
    DOC,
    FACSIMILE,
    FB,
    GRPSYM,
    GRACE_ALIGNER,
    GRAPHIC,
    INSTRDEF,
    KEYSIG_ATTR,
    LABEL,
    LABELABBR,
    LAYER,
    MEASURE,
    MEASURE_ALIGNER,
    MENSUR_ATTR,
    METERSIG_ATTR,
    PAGE,
    PAGES,
    STAFF,
    STAFF_ALIGNMENT,
    STAFFGRP,
    SURFACE,
    SVG,
    SYMBOLDEF,
    SYMBOLTABLE,
    SYSTEM,
    SYSTEM_ALIGNER,
    SYSTEM_ALIGNMENT,
    TIMESTAMP_ALIGNER,
    TUNING,
    ZONE,
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
    SUBST,
    SUPPLIED,
    UNCLEAR,
    EDITORIAL_ELEMENT_max,
    // Ids for TextLayoutElement child classes
    TEXT_LAYOUT_ELEMENT,
    DIV,
    // Ids for RunningElement child classes
    RUNNING_ELEMENT,
    PGFOOT,
    PGHEAD,
    RUNNING_ELEMENT_max,
    TEXT_LAYOUT_ELEMENT_max,
    // Ids for PageElement child classes
    PAGE_ELEMENT,
    PAGE_MILESTONE_END,
    MDIV,
    SCORE,
    PAGE_ELEMENT_max,
    // Ids for SystemElement child classes
    SYSTEM_ELEMENT,
    SYSTEM_MILESTONE_END,
    ENDING,
    EXPANSION,
    PB,
    SB,
    SECTION,
    SYSTEM_ELEMENT_max,
    // Ids for ControlElement child classes
    CONTROL_ELEMENT,
    ANCHOREDTEXT,
    ANNOTSCORE,
    ARPEG,
    BEAMSPAN,
    BRACKETSPAN,
    BREATH,
    CAESURA,
    CPMARK,
    DIR,
    DYNAM,
    FERMATA,
    FING,
    GLISS,
    HAIRPIN,
    HARM,
    LV,
    MORDENT,
    MNUM,
    ORNAM,
    OCTAVE,
    PEDAL,
    PHRASE,
    PITCHINFLECTION,
    REH,
    REPEATMARK,
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
    BARLINE,
    BEAM,
    BEATRPT,
    BTREM,
    CHORD,
    CLEF,
    CUSTOS,
    DIVLINE,
    DOT,
    DOTS,
    FLAG,
    FTREM,
    GENERIC_ELEMENT,
    GRACEGRP,
    HALFMRPT,
    KEYSIG,
    KEYACCID,
    LIGATURE,
    LIQUESCENT,
    MENSUR,
    METERSIG,
    METERSIGGRP,
    MREST,
    MRPT,
    MRPT2,
    MSPACE,
    MULTIREST,
    MULTIRPT,
    NC,
    NOTE,
    NEUME,
    ORISCUS,
    PLICA,
    PROPORT,
    QUILISMA,
    REST,
    SPACE,
    STEM,
    SYL,
    SYLLABLE,
    TABGRP,
    TABDURSYM,
    TIMESTAMP_ATTR,
    TUPLET,
    TUPLET_BRACKET,
    TUPLET_NUM,
    VERSE,
    LAYER_ELEMENT_max,
    // Ids for ScoreDefElement child classes
    SCOREDEF_ELEMENT,
    LAYERDEF,
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
    SYMBOL,
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
    INTERFACE_ALT_SYM,
    INTERFACE_AREA_POS,
    INTERFACE_BOUNDARY,
    INTERFACE_DURATION,
    INTERFACE_LINKING,
    INTERFACE_FACSIMILE,
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
class ClassIdComparison;
class BeamElementCoord;
class BoundingBox;
class Comparison;
class CurveSpannedElement;
class DivLine;
class FloatingPositioner;
class FloatingCurvePositioner;
class GraceAligner;
class InterfaceComparison;
class LayerElement;
class LedgerLine;
class LinkingInterface;
class Liquescent;
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

typedef std::vector<const Object *> ArrayOfConstObjects;

typedef std::list<Object *> ListOfObjects;

typedef std::list<const Object *> ListOfConstObjects;

typedef std::set<Object *> SetOfObjects;

typedef std::set<const Object *> SetOfConstObjects;

typedef std::set<const Object *> SetOfConstObjects;

typedef std::vector<Note *> ChordNoteGroup;

typedef std::vector<std::tuple<Alignment *, Alignment *, int>> ArrayOfAdjustmentTuples;

typedef std::vector<std::tuple<Alignment *, Arpeg *, int, bool>> ArrayOfAlignmentArpegTuples;

typedef std::vector<BeamElementCoord *> ArrayOfBeamElementCoords;

typedef std::vector<std::pair<int, int>> ArrayOfIntPairs;

typedef std::multimap<std::string, LinkingInterface *> MapOfLinkingInterfaceIDPairs;

typedef std::map<std::string, Note *> MapOfNoteIDPairs;

typedef std::vector<std::pair<Object *, std::string>> ArrayOfPlistObjectIDPairs;

typedef std::vector<CurveSpannedElement *> ArrayOfCurveSpannedElements;

typedef std::list<std::pair<Object *, data_MEASUREBEAT>> ListOfObjectBeatPairs;

typedef std::list<std::pair<const Object *, std::string>> ListOfObjectAttNamePairs;

typedef std::list<std::pair<TimePointInterface *, ClassId>> ListOfPointingInterClassIdPairs;

typedef std::list<std::pair<TimeSpanningInterface *, ClassId>> ListOfSpanningInterClassIdPairs;

typedef std::list<std::pair<TimeSpanningInterface *, Object *>> ListOfSpanningInterOwnerPairs;

typedef std::vector<FloatingPositioner *> ArrayOfFloatingPositioners;

typedef std::vector<FloatingCurvePositioner *> ArrayOfFloatingCurvePositioners;

typedef std::vector<BoundingBox *> ArrayOfBoundingBoxes;

typedef std::vector<LedgerLine> ArrayOfLedgerLines;

typedef std::vector<TextElement *> ArrayOfTextElements;

typedef std::map<const Staff *, std::multiset<int>> MapOfNoteLocs;

typedef std::map<const Staff *, std::set<int>> MapOfDotLocs;

typedef std::map<std::string, Option *> MapOfStrOptions;

typedef std::map<int, data_ACCIDENTAL_WRITTEN> MapOfOctavedPitchAccid;

typedef std::map<int, GraceAligner *> MapOfIntGraceAligners;

typedef std::vector<std::pair<std::u32string, bool>> ArrayOfStringDynamTypePairs;

typedef std::map<ClassId, std::function<Object *(void)>> MapOfClassIdConstructors;

typedef std::map<std::string, ClassId> MapOfStrClassIds;

typedef std::vector<std::pair<LayerElement *, LayerElement *>> MeasureTieEndpoints;

typedef bool (*NotePredicate)(const Note *);

typedef std::vector<std::pair<LayerElement *, data_DURATION>> ArrayOfElementDurPairs;

/**
 * Generic int map recursive structure for storing hierachy of values
 * For example, we want to process all staves one by one, and within each staff
 * all layer one by one, and so one (lyrics, etc.). In IntTree, we can store
 * \@n with all existing values (1 => 1 => 1; 2 => 1 => 1)
 * The stucture must be filled first and can then be used by instanciating a vector
 * of corresponding Comparison (typically AttNIntegerComparison for \@n attribute).
 * See Doc::PrepareData for an example.
 */
struct IntTree {
    std::map<int, IntTree> child;
};

typedef std::map<int, IntTree> IntTree_t;

/**
 * This is the alternate way for representing map of maps. With this solution,
 * we can easily have different types of key (attribute) at each level. We could
 * mix int, string, or even MEI data_* types. The drawback is that a type has to
 * be defined at each level. Also see Doc::PrepareData for an example.
 */
typedef std::map<int, bool> VerseN_t;
typedef std::map<int, VerseN_t> LayerN_VerserN_t;
typedef std::map<int, LayerN_VerserN_t> StaffN_LayerN_VerseN_t;

//----------------------------------------------------------------------------
// Global defines
//----------------------------------------------------------------------------

#define DEFINITION_FACTOR 10

#define isIn(x, a, b) (((x) >= std::min((a), (b))) && ((x) <= std::max((a), (b))))

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

/** Define the maximum levels between a tabGrp and its children **/
#define MAX_TABGRP_DEPTH -1

/** Define the maximum levels between a tuplet and its notes **/
#define MAX_TUPLET_DEPTH -1

/** Define the maximum levels of staffGrp within a scoreDef **/
#define MAX_STAFFGRP_DEPTH -1

/** Define the maximum levels between a note and its syls **/
#define MAX_NOTE_DEPTH -1

//----------------------------------------------------------------------------
// Unicode music codepoints
//----------------------------------------------------------------------------

#define UNICODE_FLAT U'\u266D' // ♭
#define UNICODE_NATURAL U'\u266E' // ♮
#define UNICODE_SHARP U'\u266F' // ♯

#define UNICODE_UNDERTIE U'\u203F' // ‿

#define UNICODE_DAL_SEGNO U'\U0001D109' // 𝄉
#define UNICODE_DA_CAPO U'\U0001D10A' // 𝄊
#define UNICODE_SEGNO U'\U0001D10B' // 𝄋
#define UNICODE_CODA U'\U0001D10C' // 𝄌

#define UNICODE_DOUBLE_FLAT U'\U0001D12B' // 𝄫
#define UNICODE_DOUBLE_SHARP U'\U0001D12A' // 𝄪

//----------------------------------------------------------------------------
// VerovioText codepoints
//----------------------------------------------------------------------------

/**
 * SMUFL Symbols used in figured bass included in VerovioText
 * Text accidentals (266D to 266F)
 * SMuFL accidentals (E260 to E264)
 * SMuFL figured-bass figures (EA50 to EA62)
 * SMuFL figured-bass supplement (ECC0)
 */

#define VRV_TEXT_HARM                                                                                                  \
    U"\u266D\u266E\u266F"                                                                                              \
    U"\uE260\uE261\uE262\uE263\uE264"                                                                                  \
    U"\uEA50\uEA51\uEA52\uEA53\uEA54\uEA55\uEA56\uEA57\uEA58\uEA59\uEA5A\uEA5B\uEA5C\uEA5D\uEA5E"                      \
    U"\uEA5F\uEA60\uEA61\uEA62\uEA63\uEA64\uEA65\uEA66\uEA67"                                                          \
    U"\uECC0"

//----------------------------------------------------------------------------
// data.LINEWIDTHTERM factors
//----------------------------------------------------------------------------

#define LINEWIDTHTERM_factor_narrow 1.0
#define LINEWIDTHTERM_factor_medium 2.0
#define LINEWIDTHTERM_factor_wide 4.0

//----------------------------------------------------------------------------
// Types for editorial element
//----------------------------------------------------------------------------

// the maximum is 255 (unsigned char)
enum EditorialLevel {
    EDITORIAL_UNDEFINED = 0,
    EDITORIAL_SCORE,
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

enum ElementScoreDefRole { SCOREDEF_NONE = 0, SCOREDEF_SYSTEM, SCOREDEF_INTERMEDIATE, SCOREDEF_CAUTIONARY };

//----------------------------------------------------------------------------
// Artic types
//----------------------------------------------------------------------------

enum ArticType { ARTIC_INSIDE = 0, ARTIC_OUTSIDE };

//----------------------------------------------------------------------------
// Visibility optimization
//----------------------------------------------------------------------------

enum VisibilityOptimization { OPTIMIZATION_NONE = 0, OPTIMIZATION_HIDDEN, OPTIMIZATION_SHOW };

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
// Ligature shape bitfields
//----------------------------------------------------------------------------

enum {
    LIGATURE_DEFAULT = 0,
    LIGATURE_STEM_LEFT_UP = 1,
    LIGATURE_STEM_LEFT_DOWN = 2,
    LIGATURE_STEM_RIGHT_UP = 4,
    LIGATURE_STEM_RIGHT_DOWN = 8,
    LIGATURE_OBLIQUE = 16,
    LIGATURE_STACKED = 32
};

//----------------------------------------------------------------------------
// Analytical markup bitfields
//----------------------------------------------------------------------------

enum {
    MARKUP_DEFAULT = 0,
    MARKUP_ANALYTICAL_TIE = 1,
    MARKUP_ANALYTICAL_FERMATA = 2,
    MARKUP_GRACE_ATTRIBUTE = 4,
    MARKUP_ARTIC_MULTIVAL = 8,
    MARKUP_SCOREDEF_DEFINITIONS = 16
};

//----------------------------------------------------------------------------
// Layout information
//----------------------------------------------------------------------------

enum LayoutInformation { LAYOUT_NONE = 0, LAYOUT_ENCODED, LAYOUT_DONE };

//----------------------------------------------------------------------------
// Bounding box access
//----------------------------------------------------------------------------

enum Accessor { SELF = 0, CONTENT };

//----------------------------------------------------------------------------
// Some keys
//----------------------------------------------------------------------------

enum { KEY_LEFT = 37, KEY_UP = 38, KEY_RIGHT = 39, KEY_DOWN = 40 };

//----------------------------------------------------------------------------
// Stem sameas drawing role
//----------------------------------------------------------------------------

enum StemSameasDrawingRole { SAMEAS_NONE = 0, SAMEAS_UNSET, SAMEAS_PRIMARY, SAMEAS_SECONDARY };

//----------------------------------------------------------------------------
// Smufl text font (selected font or fallback)
//----------------------------------------------------------------------------

enum SmuflTextFont { SMUFL_NONE = 0, SMUFL_FONT_SELECTED, SMUFL_FONT_FALLBACK };

//----------------------------------------------------------------------------
// Graphic ID type
//----------------------------------------------------------------------------

enum GraphicID { PRIMARY = 0, SPANNING, SYMBOLREF };

//----------------------------------------------------------------------------
// Measure type
//----------------------------------------------------------------------------

enum MeasureType { MEASURED = 0, UNMEASURED, NEUMELINE };

//----------------------------------------------------------------------------
// Focus status type
//----------------------------------------------------------------------------

enum FocusStatusType { FOCUS_UNSET = 0, FOCUS_SET, FOCUS_USED };

//----------------------------------------------------------------------------
// The score time unit (quarter note)
//----------------------------------------------------------------------------

#define SCORE_TIME_UNIT 4

//----------------------------------------------------------------------------
// Section representing a line in neon
//----------------------------------------------------------------------------

#define NEUME_LINE_TYPE "neon-neume-line"

//----------------------------------------------------------------------------
// Legacy Wolfgang defines
//----------------------------------------------------------------------------

#define OCTAVE_OFFSET 4

// in half staff spaces (but should be 6 in two-voice notation)
#define STANDARD_STEMLENGTH 7
#define STANDARD_STEMLENGTH_TAB 3

//----------------------------------------------------------------------------
// Temporary - to be made an option?
//----------------------------------------------------------------------------

#define TABLATURE_STAFF_RATIO 1.75
#define GERMAN_TAB_STAFF_RATIO 2.2

#define SUPER_SCRIPT_FACTOR 0.58
#define SUPER_SCRIPT_POSITION -0.20 // lowered down from the midline
#define SUB_SCRIPT_POSITION -0.17 // lowered down from the baseline

//----------------------------------------------------------------------------
// Neume notation note size to staff size ratio defines
//----------------------------------------------------------------------------

#define NOTE_HEIGHT_TO_STAFF_SIZE_RATIO 2
#define NOTE_WIDTH_TO_STAFF_SIZE_RATIO 1.4

} // namespace vrv

#endif // __VRV_DEF_H__
