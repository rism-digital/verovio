/////////////////////////////////////////////////////////////////////////////
// Name:        dynam.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DYNAM_H__
#define __VRV_DYNAM_H__

#include "atts_midi.h"
#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Dynam
//----------------------------------------------------------------------------

class Dynam : public ControlElement,
              public TextListInterface,
              public TextDirInterface,
              public TimeSpanningInterface,
              public AttEnclosingChars,
              public AttExtender,
              public AttLineRendBase,
              public AttMidiValue,
              public AttMidiValue2,
              public AttVerticalGroup {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Dynam();
    virtual ~Dynam();
    Object *Clone() const override { return new Dynam(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "dynam"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TextDirInterface *GetTextDirInterface() override { return vrv_cast<TextDirInterface *>(this); }
    const TextDirInterface *GetTextDirInterface() const override { return vrv_cast<const TextDirInterface *>(this); }
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return vrv_cast<TimeSpanningInterface *>(this); }
    const TimeSpanningInterface *GetTimeSpanningInterface() const override
    {
        return vrv_cast<const TimeSpanningInterface *>(this);
    }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a dynam.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Return true if the dynam text is only composed of f, p, r, z, etc. letters (e.g. sfz)
     */
    bool IsSymbolOnly() const;

    /**
     * Return the SMuFL str for the dynamic symbol.
     * Call IsSymbolOnly first to check.
     */
    std::u32string GetSymbolStr(const bool singleGlyphs) const;

    /**
     * See FloatingObject::IsExtenderElement
     */
    bool IsExtenderElement() const override { return GetExtender() == BOOLEAN_true; }

    /**
     * Retrieve parentheses / brackets from the enclose attribute
     */
    std::pair<char32_t, char32_t> GetEnclosingGlyphs() const;

    //----------------//
    // Static methods //
    //----------------//

    static bool GetSymbolsInStr(std::u32string str, ArrayOfStringDynamTypePairs &tokens);

    static bool IsSymbolOnly(const std::u32string &str);

    static std::u32string GetSymbolStr(const std::u32string &str, const bool singleGlyphs);

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
    //
private:
    //
public:
    //
private:
    /** A cached version of the symbol str instanciated by IsSymbolOnly() */
    mutable std::u32string m_symbolStr;
};

} // namespace vrv

#endif
