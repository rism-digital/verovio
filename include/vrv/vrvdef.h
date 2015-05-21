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
 
class AttComparison;
class BeamElementCoord;
class Note;
class Object;
class Staff;

typedef std::vector<Object*> ArrayOfObjects;

typedef std::list<Object*> ListOfObjects;

typedef std::vector<void*> ArrayPtrVoid;

typedef std::vector<AttComparison*> ArrayOfAttComparisons;

typedef std::vector<Note*> ChordCluster;
    
typedef std::vector<BeamElementCoord*> ArrayOfBeamElementCoords;
    
typedef std::map<Staff*, std::vector<char> > MapOfLedgerLineFlags;
    
//----------------------------------------------------------------------------
// Global defines
//----------------------------------------------------------------------------
    
#define DEFINITON_FACTOR 10
#define PARAM_DENOMINATOR 10
    
#define VERSION_MAJOR 0
#define VERSION_MINOR 9
#define VERSION_REVISION 7
    
#define is_in(x,a,b) (((x) >= std::min((a),(b))) && ((x) <= std::max((a),(b))))

/**
 * Codes returned by Functors.
 * Default is FUNCTOR_CONTINUE.
 * FUNCTOR_SIBLINGS will no go any deeper in the tree.
 * FUNCTOR_STOP wil stop the tree processing.
 */
enum FunctorCode {
    FUNCTOR_CONTINUE = 0,
    FUNCTOR_SIBLINGS,
    FUNCTOR_STOP
};

//----------------------------------------------------------------------------
// Maximum number of levels between parent and children for optimizing search
//----------------------------------------------------------------------------
    
/** All values set to -1 (no limit) since this has not major incidence **/
    
/** Define the maximum levels between a beam and its notes **/
#define MAX_BEAM_DEPTH -1
    
/** Define the maximum levels between a beam and its notes **/
#define MAX_CHORD_DEPTH -1
    
/** Define the maximum levels between a tuplet and its notes **/
#define MAX_TUPLET_DEPTH -1
    
/** Define the maximum levels of staffGrp within a scoreDef **/
#define MAX_STAFFGRP_DEPTH -1

/** Define the maximum levels between a note and its syls **/
#define MAX_NOTE_DEPTH -1
    
//----------------------------------------------------------------------------
// VerovioText codpoints
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
// Legacy Wolfgang defines
//----------------------------------------------------------------------------
  
#define OCTAVE_OFFSET 4

// ACCID
#define ACCID_SHARP 1
#define ACCID_FLAT 2
#define ACCID_NATURAL 3
#define ACCID_DOUBLE_SHARP 4
#define ACCID_DOUBLE_FLAT 5
#define ACCID_QUARTER_SHARP 6
#define ACCID_QUARTER_FLAT 7

#define ON 1
#define OFF 0

// TODO // ax2

// RESTS
#define VALSilSpec 15	/* val indiquant silence sp‚cial avec chiffre */ // ???

// pour le flag tetenote
#define LOSANGEVIDE 1
#define OPTIONLIBRE 6
#define SANSQUEUE 7

/* valeurs des attributs de staccato in menu (getcode2()) */
#define STACC 0
#define LOURE 1
#define ACCENT_OBL 2
#define ACCENT_VERT 3
#define BEBUNG 4
#define STAC_AIGU 5
#define ACCENT_OBL_PNT 6
#define ACCENT_VERT_PNT 7

    
// the maximum is 255 (unsigned char)
enum EditorialLevel {
    EDITORIAL_UNDEFINED = 0,
    EDITORIAL_SYSTEM,
    EDITORIAL_SCOREDEF,
    EDITORIAL_STAFFGRP,    
    EDITORIAL_MEASURE,
    EDITORIAL_STAFF,
    EDITORIAL_LAYER,
    EDITORIAL_NOTE
};
    
// the maximum is 255 (unsigned char)
enum StaffGrpSymbol {
    STAFFGRP_NONE = 0,
    STAFFGRP_LINE,
    STAFFGRP_BRACE,
    STAFFGRP_BRACKET
};
    
/* This is used for fast clef offset calculation.
 * It uses 4 bytes with, from right to left
 * - line
 * - shape
 * - dis (0 or 1)
 * - dis.place (0 or 1)
 */
enum ClefId {
    G1 = CLEFSHAPE_G << 8 | 1,
    G2 = CLEFSHAPE_G << 8 | 2,
    G2_8va =  PLACE_above << 24 | OCTAVE_DIS_8 << 16 | G2,
    G2_8vb = PLACE_below << 24 | OCTAVE_DIS_8 << 16 | G2,
    F3 = CLEFSHAPE_F << 8 | 3,
    F4 = CLEFSHAPE_F << 8 | 4,
    F5 = CLEFSHAPE_F << 8 | 5,
    C1 = CLEFSHAPE_C << 8 | 1,
    C2 = CLEFSHAPE_C << 8 | 2,
    C3 = CLEFSHAPE_C << 8 | 3,
    C4 = CLEFSHAPE_C << 8 | 4,
    C5 = CLEFSHAPE_C << 8 | 5,
    perc = CLEFSHAPE_perc << 8 | 1
};
    
//The next four macros were tuned using the Leipzig font.
    
//Width (in half-drawing units) of an accidental; used to prevent overlap on complex chords
#define ACCID_WIDTH 4
    
//Height
#define ACCID_HEIGHT 12
    
//Only keeps track of this much of the top of flats so that their bottom can be drawn more concisely
//This can also be thought of as height(sharp)*F_B_H_M = height(flat)
#define FLAT_BOTTOM_HEIGHT_MULTIPLIER .75
    
//Ignores this much of the top/right of an accid for same purposes (empty space in top right of drawing)
#define FLAT_CORNER_HEIGHT_IGNORE .25
#define FLAT_CORNER_WIDTH_IGNORE .5
#define NATURAL_CORNER_HEIGHT_IGNORE .25
#define NATURAL_CORNER_WIDTH_IGNORE .5

} // namespace vrv

#endif // __VRV_DEF_H__