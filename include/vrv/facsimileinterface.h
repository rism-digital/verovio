/////////////////////////////////////////////////////////////////////////////
// Name:        facsimileinterface.h
// Author:      Juliette Regimbal
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FACSIMILE_INTERFACE_H__
#define __VRV_FACSIMILE_INTERFACE_H__

#include "atts_facsimile.h"
#include "atts_shared.h"
#include "interface.h"

namespace vrv {
class FunctorParams;
class View;
class Zone;

//----------------------------------------------------------------------------
// FacsimileInterface
//----------------------------------------------------------------------------

class FacsimileInterface : public Interface, public AttFacsimile {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    FacsimileInterface();
    virtual ~FacsimileInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_FACSIMILE; }
    ///@}

    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;

    int GetWidth() const;
    int GetHeight() const;

    double GetDrawingRotate() const;

    /** Check if the object has a facsimile */
    bool HasFacsimile() const { return this->HasFacs(); }

    /** Link to the zone */
    void AttachZone(Zone *zone);

    int GetSurfaceY() const;

    /** Get the zone */
    ///@{
    Zone *GetZone() { return m_zone; }
    const Zone *GetZone() const { return m_zone; }
    ///@}

private:
    Zone *m_zone = NULL;
};
} // namespace vrv
#endif
