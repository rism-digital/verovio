/////////////////////////////////////////////////////////////////////////////
// Name:        scoredefinterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SCOREDEF_INTERFACE_H__
#define __VRV_SCOREDEF_INTERFACE_H__

#include "atts_analytical.h"
#include "atts_cmn.h"
#include "atts_mensural.h"
#include "atts_midi.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "interface.h"
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
                          public AttBarring,
                          public AttDurationDefault,
                          public AttLyricStyle,
                          public AttMeasureNumbers,
                          public AttMidiTempo,
                          public AttMmTempo,
                          public AttMultinumMeasures,
                          public AttOctaveDefault,
                          public AttPianoPedals,
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
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_SCOREDEF; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
