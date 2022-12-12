/////////////////////////////////////////////////////////////////////////////
// Name:        altsyminterface.h
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ALTSYM_INTERFACE_H__
#define __VRV_ALTSYM_INTERFACE_H__

#include "att.h"
#include "atts_usersymbols.h"
#include "vrvdef.h"

namespace vrv {

class FunctorParams;
class SymbolDef;

//----------------------------------------------------------------------------
// AltSymInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having a @altsym
 * It is not an abstract class but should not be instantiated directly.
 */
class AltSymInterface : public Interface, public AttAltSym {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    AltSymInterface();
    virtual ~AltSymInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_ALT_SYM; }
    ///@}

    /**
     * @name Set and get the @next, @sameas, ... object
     * The setter asserts that no Object was previously set.
     */
    ///@{
    void SetAltSymbolDef(SymbolDef *symbolDef);
    SymbolDef *GetAltSymbolDef() { return m_symbolDef; }
    const SymbolDef *GetAltSymbolDef() const { return m_symbolDef; }
    ///@}

    /**
     * Return true if a symbolDef is given and resolved
     */
    ///@{
    bool HasAltSymbolDef() const { return (m_symbolDef); }
    ///@}

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the  Object (implementation) to
     * the functor method. These not called by the Process/Call loop but by the implementaion
     * classes explicitely. See FloatingObject::PrepareStaffCurrentTimeSpanning for an example.
     */

    /**
     * See Object::PrepareAltSym
     */
    virtual int InterfacePrepareAltSym(FunctorParams *functorParams, Object *object);

    /**
     * See Object::ResetData
     */
    virtual int InterfaceResetData(FunctorParams *functorParams, Object *object);

protected:
    /**
     * Extract the fragment of the start or end @xml:id if given
     */
    void SetIDStr();

private:
    //
public:
    //
private:
    SymbolDef *m_symbolDef;
    std::string m_symbolDefID;
};

} // namespace vrv

#endif
