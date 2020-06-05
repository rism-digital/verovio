/////////////////////////////////////////////////////////////////////////////
// Name:        harm.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_HARM_H__
#define __VRV_HARM_H__

#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"
#include "transposition.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Harm (harmony)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <harm> element.
 */
class Harm : public ControlElement,
             public TextListInterface,
             public TextDirInterface,
             public TimeSpanningInterface,
             public AttLang,
             public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Harm();
    virtual ~Harm();
    virtual Object *Clone() const { return new Harm(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Harm"; }
    virtual ClassId GetClassId() const { return HARM; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a harm.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Transposition related. The int tracks where we have iterated through the string.
     */
    bool GetRootPitch(TransPitch &pitch, unsigned int &pos);
    void SetRootPitch(const TransPitch &pitch, unsigned int endPos);
    bool GetBassPitch(TransPitch &pitch);
    void SetBassPitch(const TransPitch &pitch);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareFloatingGrps
     */
    virtual int PrepareFloatingGrps(FunctorParams *functoParams);

    /**
     * See Object::AdjustHarmGrpsSpacing
     */
    virtual int AdjustHarmGrpsSpacing(FunctorParams *functorParams);

    /**
     * See Object::Transpose
     */
    virtual int Transpose(FunctorParams *functorParams);

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
