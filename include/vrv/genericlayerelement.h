/////////////////////////////////////////////////////////////////////////////
// Name:        genericlayerelement.h
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GENERICLAYERELEMENT_H__
#define __VRV_GENERICLAYERELEMENT_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// GenericLayerElement
//----------------------------------------------------------------------------

/**
 * This class holds generic elements existing within MEI <layer> but not supported by Verovio
 */
class GenericLayerElement : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    GenericLayerElement();
    GenericLayerElement(const std::string &name);
    virtual ~GenericLayerElement();
    Object *Clone() const override { return new GenericLayerElement(*this); }
    void Reset() override;
    std::string GetClassName() const override { return m_className; }
    ///@}

    /**
     * Return the MEI element original name
     */
    std::string GetMEIName() const { return m_meiName; }

    /**
     * Return the MEI element original name
     */
    std::string GetContent() { return m_content; }
    void SetContent(std::string content) { m_content = content; }

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
    /** The class name */
    std::string m_className;
    /** The MEI element name */
    std::string m_meiName;
    /** The MEI element content */
    std::string m_content;

public:
    //
private:
};

} // namespace vrv

#endif
