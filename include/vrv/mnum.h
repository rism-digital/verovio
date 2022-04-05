/////////////////////////////////////////////////////////////////////////////
// Name:        mnum.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MNUM_H__
#define __VRV_MNUM_H__

#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// MNum
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mNum> element.
 */
class MNum : public ControlElement,
             public TextListInterface,
             public TextDirInterface,
             public TimePointInterface,
             public AttColor,
             public AttLang,
             public AttTypography {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    MNum();
    virtual ~MNum();
    Object *Clone() const override { return new MNum(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "MNum"; }
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
    ///@}

    /**
     * Add an element (text, rend. etc.) to a dynam.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * @name Setter and getter of the generated flag
     */
    ///@{
    bool IsGenerated() const { return m_isGenerated; }
    void IsGenerated(bool isGenerated) { m_isGenerated = isGenerated; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::Save
     */
    ///@{
    int Save(FunctorParams *functorParams) override;
    int SaveEnd(FunctorParams *functorParams) override;
    ///@}

private:
    /**
     * Flag indicating whether or not the element was generated
     */
    bool m_isGenerated;
};

} // namespace vrv

#endif
