/////////////////////////////////////////////////////////////////////////////
// Name:        textdirective.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEXT_DIRECTIVE_H__
#define __VRV_TEXT_DIRECTIVE_H__

#include "floatingelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// AnchoredText
//----------------------------------------------------------------------------

/**
 * This class is an interface for <anchoredText> elements at the measure level
 */
class AnchoredText : public FloatingElement, public TextDirInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    AnchoredText();
    virtual ~AnchoredText();
    virtual void Reset();
    virtual std::string GetClassName() { return "AnchoredText"; };
    virtual ClassId Is() { return ANCHORED_TEXT; };
    ///@}

    /**
     * Add an element (text, rend. etc.) to a tempo.
     * Only supported elements will be actually added to the child list.
     */
    void AddTextElement(TextElement *element);

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

/**
 * This class is an interface for <tempo> elements at the measure level
 */
class Tempo : public FloatingElement, public TextDirInterface, public TimePointInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    Tempo();
    virtual ~Tempo();
    virtual void Reset();
    virtual std::string GetClassName() { return "Tempo"; };
    virtual ClassId Is() { return TEMPO; };
    ///@}

    /**
     * Add an element (text, rend. etc.) to a tempo.
     * Only supported elements will be actually added to the child list.
     */
    void AddTextElement(TextElement *element);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
