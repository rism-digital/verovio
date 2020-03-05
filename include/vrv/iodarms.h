/////////////////////////////////////////////////////////////////////////////
// Name:        iodarms.h
// Author:      Rodolfo Zitellini
// Created:     02/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IODARMS_H__
#define __VRV_IODARMS_H__

//----------------------------------------------------------------------------

#include "attdef.h"
#include "io.h"

namespace vrv {

#ifndef NO_DARMS_SUPPORT

class Doc;
class Layer;
class Measure;
class Staff;
class Tie;

//----------------------------------------------------------------------------
// DarmsInput
//----------------------------------------------------------------------------

#define ASCII_NUMBER_OFFSET 0x30 // for homebrewed char to int

typedef struct _pitchmap {
    char oct;
    data_PITCHNAME pitch;
} pitchmap;

class DarmsInput : public Input {
public:
    // constructors and destructors
    DarmsInput(Doc *doc);
    virtual ~DarmsInput();

    virtual bool Import(const std::string &data);

private:
    int do_Note(int pos, const char *data, bool rest);
    int do_globalSpec(int pos, const char *data);
    int do_Clef(int pos, const char *data);
    int parseMeter(int pos, const char *data);
    void UnrollKeysig(int quantity, char alter);

public:
    //
private:
    Staff *m_staff;
    Measure *m_measure;
    Layer *m_layer;

    Tie *m_current_tie;

    unsigned int m_clef_offset;
    bool m_antique_notation; // support square notation

    static pitchmap PitchMap[];
};

#endif /* NO_DARMS_SUPPORT */

} // namespace vrv

#endif // __VRV_IODARMS_H__
