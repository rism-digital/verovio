/////////////////////////////////////////////////////////////////////////////
// Name:        tempo.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEMPO_H__
#define __VRV_TEMPO_H__

#include "atts_midi.h"
#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

/**
 * This class is an interface for <tempo> elements at the measure level
 */
class Tempo : public ControlElement,
              public TextDirInterface,
              public TimePointInterface,
              public AttLang,
              public AttMidiTempo,
              public AttMmTempo {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    Tempo();
    virtual ~Tempo();
    virtual Object *Clone() const { return new Tempo(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Tempo"; }
    virtual ClassId GetClassId() const { return TEMPO; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a tempo.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
