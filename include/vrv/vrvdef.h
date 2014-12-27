/////////////////////////////////////////////////////////////////////////////
// Name:        vrvdef.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DEF_H__
#define __VRV_DEF_H__

#include <algorithm>

//----------------------------------------------------------------------------

#include "attdef.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// Global defines
//----------------------------------------------------------------------------
    
#define VERSION_MAJOR 0
#define VERSION_MINOR 9
#define VERSION_REVISION 4
    
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

// Is this score for Mensural, neumes, or CMN
enum NotationMode {
    MENSURAL_MODE = 0,
    CMN_MODE
};

// Is the app in insert mode or edit (drag) mode
enum EditorMode {
	EDITOR_EDIT,
	EDITOR_INSERT
};
    
//----------------------------------------------------------------------------
// Default layout values
//----------------------------------------------------------------------------
    
#define DEFINITON_FACTOR 10

#define DEFAULT_UNIT 9
#define MIN_UNIT 6
#define MAX_UNIT 18
    
#define DEFAULT_PAGE_RIGHT_MAR 50
#define MIN_PAGE_RIGHT_MAR 0
#define MAX_PAGE_RIGHT_MAR 500
    
#define DEFAULT_PAGE_LEFT_MAR 50
#define MIN_PAGE_LEFT_MAR 0
#define MAX_PAGE_LEFT_MAR 500
    
#define DEFAULT_PAGE_TOP_MAR 50
#define MIN_PAGE_TOP_MAR 0
#define MAX_PAGE_TOP_MAR 500
    
#define DEFAULT_PAGE_HEIGHT 2970
#define MIN_PAGE_HEIGHT 100
#define MAX_PAGE_HEIGHT 60000
    
#define DEFAULT_PAGE_WIDTH 2100
#define MIN_PAGE_WIDTH 100
#define MAX_PAGE_WIDTH 60000
    
#define MIN_TIE_HEIGHT 12
#define MIN_TIE_THICKNESS 6
    
//----------------------------------------------------------------------------
// Default scaling (%) and spacing (units) values
//----------------------------------------------------------------------------
    
#define DEFAULT_SCALE 100
#define MIN_SCALE 1
#define MAX_SCALE 1000
    
#define DEFAULT_SPACING_STAFF 6
#define MIN_SPACING_STAFF 0
#define MAX_SPACING_STAFF 12
    
#define DEFAULT_SPACING_SYSTEM 0
#define MIN_SPACING_SYSTEM 0
#define MAX_SPACING_SYSTEM 12

//----------------------------------------------------------------------------
// Durations
//----------------------------------------------------------------------------
    
#define DUR_LG 0  // longa
#define DUR_BR 1  // brevis
#define DUR_1 2   // whole note
#define DUR_2 3   // ...
#define DUR_4 4
#define DUR_8 5
#define DUR_16 6
#define DUR_32 7
#define DUR_64 8
#define DUR_128 9
#define DUR_256 10
// used for alignement
#define DUR_MAX 1024

//----------------------------------------------------------------------------
// Legacy Wolfgang defines
//----------------------------------------------------------------------------
    
// ACCID
#define ACCID_SHARP 1
#define ACCID_FLAT 2
#define ACCID_NATURAL 3
#define ACCID_DOUBLE_SHARP 4
#define ACCID_DOUBLE_FLAT 5
#define ACCID_QUARTER_SHARP 6
#define ACCID_QUARTER_FLAT 7

// DIRECTIONS
#define FORWARD 1
#define BACKWARD 0
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

} // namespace vrv

#endif // __VRV_DEF_H__