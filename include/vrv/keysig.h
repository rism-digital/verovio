
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
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

class Clef;
class ScoreDefInterface;

//----------------------------------------------------------------------------
// KeySig
//----------------------------------------------------------------------------

/**
 * This class models the MEI <keySig> element.
 */
class KeySig : public LayerElement,
               public ObjectListInterface,
               public AttAccidental,
               public AttPitch,
               public AttKeySigAnl,
               public AttKeySigLog,
               public AttKeySigVis,
               public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    KeySig();
    virtual ~KeySig();
    virtual Object *Clone() const { return new KeySig(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "KeySig"; }
    virtual ClassId GetClassId() const { return KEYSIG; }

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /** Override the method since check is required */
    virtual bool IsScoreDefElement() const { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

    /**
     * Add an element (a keyAccid) to a keySig.
     */
    virtual bool IsSupportedChild(Object *object);

    /** Accid number getter */
    int GetAccidCount();

    /** Accid type getter */
    data_ACCIDENTAL_WRITTEN GetAccidType();

    /**
     * Fill the map of modified pitches
     */
    void FillMap(MapOfPitchAccid &mapOfPitchAccid);

    /**
     * Return the string of the alteration at the positon pos.
     * Looks at keyAccid children if any.
     * The accid at pos is return in accid and the pname in pname.
     */
    std::wstring GetKeyAccidStrAt(int pos, data_ACCIDENTAL_WRITTEN &accid, data_PITCHNAME &pname);

    /**
     * Static methods for calculating position;
     */
    static data_PITCHNAME GetAccidPnameAt(data_ACCIDENTAL_WRITTEN alterationType, int pos);
    static int GetOctave(data_ACCIDENTAL_WRITTEN alterationType, data_PITCHNAME pitch, Clef *clef);

    int GetFifthsInt();

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::Transpose
     */
    virtual int Transpose(FunctorParams *);

protected:
    /**
     * Filter the flat list and keep only StaffDef elements.
     */
    virtual void FilterList(ArrayOfObjects *childList);

private:
    //
public:
    bool m_mixedChildrenAccidType;
    /**
     * Variables for storing cancellation introduced by the key sig.
     * The values are StaffDefDrawingInterface::ReplaceKeySig
     */
    data_ACCIDENTAL_WRITTEN m_drawingCancelAccidType;
    char m_drawingCancelAccidCount;

    static data_PITCHNAME s_pnameForFlats[];
    static data_PITCHNAME s_pnameForSharps[];

private:
    static int octave_map[2][9][7];
};

} // namespace vrv

#endif
