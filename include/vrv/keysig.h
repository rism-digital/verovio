
/////////////////////////////////////////////////////////////////////////////
// Name:        keysig.h
// Author:      Rodolfo Zitellini
// Created:     10/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_KEYSIG_H__
#define __VRV_KEYSIG_H__

#include "atts_analytical.h"
#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

class Clef;
class ScoreDefInterface;

//----------------------------------------------------------------------------
// KeySig
//----------------------------------------------------------------------------

/**
 * This class models the MEI <keySig> element.
 * Since it is unclear how to encode the logical key signature in keySig, an
 * internal representation is used (there is no equivalent of staffDef @key
 * within keySig. Currently the @accid and @pname are used for this. The
 * @pname stores the value of the latest accidental, which is doubtfully the
 * expected use of it.
 * Two temporary methods KeySig::ConvertToMei and KeySig::ConvertToInternal
 * are available for converting from and to the MEI representation to the
 * internal (and vice versa)
 */
class KeySig : public LayerElement, public AttAccidental, public AttPitch, public AttKeySigAnl {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    KeySig();
    KeySig(int alterationNumber, data_ACCIDENTAL_WRITTEN alterationType);
    KeySig(const ScoreDefInterface *keySigAttr);
    void Init();
    virtual ~KeySig();
    virtual void Reset();
    virtual Object *Clone() const { return new KeySig(*this); }
    virtual std::string GetClassName() const { return "KeySig"; }
    virtual ClassId GetClassId() const { return KEYSIG; }

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /* Alteration number getter/setter */
    int GetAlterationNumber() const { return m_alterationNumber; }
    void SetAlterationNumber(int alterationNumber) { m_alterationNumber = alterationNumber; }

    /* Alteration number getter/setter */
    data_ACCIDENTAL_WRITTEN GetAlterationType() const { return m_alterationType; }
    void SetAlterationType(data_ACCIDENTAL_WRITTEN alterationType) { m_alterationType = alterationType; }

    /* Temporary methods for turning @accid and @pitch into num_alter and alter */
    void ConvertToMei();
    void ConvertToInternal();

    /* Convert to KeySigLog */
    data_KEYSIGNATURE ConvertToKeySigLog();

    /**
     * Static methods for calculating position;
     */
    static data_PITCHNAME GetAlterationAt(data_ACCIDENTAL_WRITTEN alterationType, int pos);
    static int GetOctave(data_ACCIDENTAL_WRITTEN alterationType, data_PITCHNAME pitch, Clef *clef);

private:
    //
public:
    /**
     * Variables for storing cancellation introduced by the key sig.
     * The values are StaffDefDrawingInterface::ReplaceKeySig
     */
    data_ACCIDENTAL_WRITTEN m_drawingCancelAccidType;
    char m_drawingCancelAccidCount;
    /**
     * Equivalent to @key.sig.show and @showchange, but set for drawing
     * KeySig has no equivalent in MEI and will be true and false by default
     * See KeySig::KeySig(KeySigAttr *keySigAttr) for initialisation
     */
    bool m_drawingShow;
    bool m_drawingShowchange;

private:
    static data_PITCHNAME flats[];
    static data_PITCHNAME sharps[];
    static int octave_map[2][9][7];

    // This is temporary - it needs to be changed to libMEI atts
    int m_alterationNumber;
    data_ACCIDENTAL_WRITTEN m_alterationType;
};

} // namespace vrv

#endif
