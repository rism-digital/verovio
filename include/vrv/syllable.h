/////////////////////////////////////////////////////////////////////////////
// Name:        syllable.h
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYLLABLE_H__
#define __VRV_SYLLABLE_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syllable
//----------------------------------------------------------------------------

class ScoreDefInterface;

/**
 * This class models the MEI <mensur> element.
 */
class Syllable : public LayerElement, public ObjectListInterface, public AttColor, public AttSlashCount {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Syllable();
    void Init();
    virtual ~Syllable();
    Object *Clone() const override { return new Syllable(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "syllable"; }
    ///@}

    /**
     * Add an element (a note or a rest) to a syllable.
     * Only syl or neume will be added.
     */
    bool IsSupportedChild(ClassId classId) override;

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Add a default syl to this syllable if one does not exist.
     * Returns true if a syl was added, false otherwise.
     */
    bool MarkupAddSyl();

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
private:
};

} // namespace vrv

#endif
