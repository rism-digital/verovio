/////////////////////////////////////////////////////////////////////////////
// Name:        anchoredtext.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ANCHORED_TEXT_H__
#define __VRV_ANCHORED_TEXT_H__

#include "controlelement.h"
#include "textdirinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// AnchoredText
//----------------------------------------------------------------------------

/**
 * This class is an interface for <anchoredText> elements at the measure level
 */
class AnchoredText : public ControlElement, public TextDirInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    AnchoredText();
    virtual ~AnchoredText();
    Object *Clone() const override { return new AnchoredText(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "anchoredText"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TextDirInterface *GetTextDirInterface() override { return vrv_cast<TextDirInterface *>(this); }
    const TextDirInterface *GetTextDirInterface() const override { return vrv_cast<const TextDirInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a tempo.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(ClassId classId) override;

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

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
