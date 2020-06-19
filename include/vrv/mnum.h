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
    virtual Object *Clone() const { return new MNum(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "MNum"; }
    virtual ClassId GetClassId() const { return MNUM; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a dynam.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

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
    virtual int Save(FunctorParams *functorParams);
    virtual int SaveEnd(FunctorParams *functorParams);
    ///@}

private:
    /**
     * Flag indicating whether or not the element was generated
     */
    bool m_isGenerated;
};

} // namespace vrv

#endif
