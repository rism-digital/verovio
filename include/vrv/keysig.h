
/////////////////////////////////////////////////////////////////////////////
// Name:        keysig.h
// Author:      Rodolfo Zitellini
// Created:     10/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_KEYSIG_H__
#define __VRV_KEYSIG_H__

#include <optional>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "clef.h"
#include "layerelement.h"

namespace vrv {

class ScoreDefInterface;

//----------------------------------------------------------------------------
// KeyAccidInfo
//----------------------------------------------------------------------------
/**
 * Useful information regarding a KeyAccid child
 */
struct KeyAccidInfo {
    data_ACCIDENTAL_WRITTEN accid;
    data_PITCHNAME pname;
};

//----------------------------------------------------------------------------
// KeySig
//----------------------------------------------------------------------------

/**
 * This class models the MEI <keySig> element.
 */
class KeySig : public LayerElement,
               public ObjectListInterface,
               public AttAccidental,
               public AttColor,
               public AttKeyMode,
               public AttKeySigLog,
               public AttKeySigVis,
               public AttPitch,
               public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    KeySig();
    virtual ~KeySig();
    Object *Clone() const override { return new KeySig(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "keySig"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /** Override the method since check is required */
    bool IsScoreDefElement() const override { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

    /**
     * Add an element (a keyAccid) to a keySig.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Additional check when adding a child.
     */
    bool AddChildAdditionalCheck(Object *child) override;

    /** Accid number getter */
    int GetAccidCount(bool fromAttribute = false) const;

    /** Accid type getter */
    data_ACCIDENTAL_WRITTEN GetAccidType() const;

    /**
     * Generate KeyAccid attribute children
     */
    ///@{
    bool HasNonAttribKeyAccidChildren() const;
    void GenerateKeyAccidAttribChildren();
    ///@}

    /**
     * Try to convert a keySig content (keyAccid) to a @sig value
     * This can work only if the content represents a standard accidental series
     * Return an empty @sig when the content cannot be converted
     */
    data_KEYSIGNATURE ConvertToSig() const;

    /**
     * Fill the map of modified pitches
     */
    void FillMap(MapOfOctavedPitchAccid &mapOfPitchAccid) const;

    int GetFifthsInt() const;

    /**
     * Set/Get the drawing clef
     */
    ///@{
    Clef *GetDrawingClef();
    void ResetDrawingClef();
    void SetDrawingClef(Clef *clef);
    ///@}

    //----------------//
    // Static methods //
    //----------------//

    /**
     * Static methods for calculating position;
     */
    static data_PITCHNAME GetAccidPnameAt(data_ACCIDENTAL_WRITTEN alterationType, int pos);
    static int GetOctave(data_ACCIDENTAL_WRITTEN alterationType, data_PITCHNAME pitch, const Clef *clef);

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

protected:
    /**
     * Filter the flat list and keep only StaffDef elements.
     */
    void FilterList(ListOfConstObjects &childList) const override;

private:
    /**
     * Generate key accid information for a given position
     */
    std::optional<KeyAccidInfo> GetKeyAccidInfoAt(int pos) const;

public:
    /**
     * Variables for storing cancellation introduced by the key sig.
     * Values are set in StaffDefDrawingInterface::ReplaceKeySig
     */
    ///@{
    bool m_skipCancellation;
    data_ACCIDENTAL_WRITTEN m_drawingCancelAccidType;
    char m_drawingCancelAccidCount;
    ///@}

    //----------------//
    // Static members //
    //----------------//

    static const data_PITCHNAME s_pnameForFlats[];
    static const data_PITCHNAME s_pnameForSharps[];

private:
    /**
     * The clef used for drawing
     * Calculated from layer if not set
     */
    std::optional<Clef> m_drawingClef;

    //----------------//
    // Static members //
    //----------------//

    static const int octave_map[2][9][7];
};

} // namespace vrv

#endif
