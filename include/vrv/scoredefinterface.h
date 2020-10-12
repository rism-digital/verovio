/////////////////////////////////////////////////////////////////////////////
// Name:        scoredefinterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SCOREDEF_INTERFACE_H__
#define __VRV_SCOREDEF_INTERFACE_H__

#include "atts_analytical.h"
#include "atts_mensural.h"
#include "atts_midi.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// ScoreDefInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements implementing score attributes, such
 * as <scoreDef>, or <staffDef>
 * It is not an abstract class but should not be instanciated directly.
 */
class ScoreDefInterface : public Interface,
                          public AttLyricStyle,
                          public AttMeasureNumbers,
                          public AttMidiTempo,
                          public AttMultinumMeasures,
                          public AttSpacing,
                          public AttSystems {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    ScoreDefInterface();
    virtual ~ScoreDefInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_SCOREDEF; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
