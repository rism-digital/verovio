/////////////////////////////////////////////////////////////////////////////
// Name:        editorial.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITORIAL_H__
#define __VRV_EDITORIAL_H__

#include "atts_critapp.h"
#include "atts_shared.h"
#include "boundary.h"
#include "object.h"

namespace vrv {

class Layer;
class LayerElement;
class Measure;
class ControlElement;
class ScoreDef;
class Staff;
class StaffDef;
class StaffGrp;
class TextElement;

//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

/**
 * This class is a base class for the editorial element containing musical
 * content, for example <rgd> or <add>.
 * It is not an abstract class but should not be instantiated directly.
 * It can be both a container (in score-based MEI) and a boundary (in page-based MEI).
 */
class EditorialElement : public Object, public BoundaryStartInterface, public AttLabelled, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    EditorialElement();
    EditorialElement(std::string classid);
    virtual ~EditorialElement();
    virtual void Reset();
    virtual ClassId GetClassId() const { return EDITORIAL_ELEMENT; }
    ///@}

    /**
     * @name Add children to an editorial element.
     */
    ///@{
    virtual void AddChild(Object *object);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    virtual int ConvertToPageBased(FunctorParams *functorParams);
    virtual int ConvertToPageBasedEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareBoundaries
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::CastOffSystems
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

private:
    //
public:
    /**
     * Holds the visibility (hidden or visible) for an editorial element.
     * By default, all editorial elements are visible. However, in an <app>, only one <rdg> is visible at the time. When
     * loading the file, the first <rdg> (or the <lem>) is made visible.
     */
    VisibilityType m_visibility;

private:
};

//----------------------------------------------------------------------------
// Abbr
//----------------------------------------------------------------------------

class Abbr : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Abbr();
    virtual ~Abbr();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Abbr"; }
    virtual ClassId GetClassId() const { return ABBR; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Add
//----------------------------------------------------------------------------

class Add : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Add();
    virtual ~Add();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Add"; }
    virtual ClassId GetClassId() const { return ADD; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

class App : public EditorialElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    App();
    App(EditorialLevel level);
    virtual ~App();
    virtual void Reset();
    virtual std::string GetClassName() const { return "App"; }
    virtual ClassId GetClassId() const { return APP; }
    ///@}

    /** Getter for level **/
    EditorialLevel GetLevel() { return m_level; }

    /**
     * Add children to a apparatus.
     */
    virtual void AddChild(Object *object);

protected:
    /** We store the level of the <app> for integrity check */
    EditorialLevel m_level;

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Choice
//----------------------------------------------------------------------------

class Choice : public EditorialElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Choice();
    Choice(EditorialLevel level);
    virtual ~Choice();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Choice"; }
    virtual ClassId GetClassId() const { return CHOICE; }
    ///@}

    /** Getter for level **/
    EditorialLevel GetLevel() { return m_level; }

    /**
     * Add children to a apparatus.
     */
    virtual void AddChild(Object *object);

protected:
    /** We store the level of the <choice> for integrity check */
    EditorialLevel m_level;

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Corr
//----------------------------------------------------------------------------

class Corr : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Corr();
    virtual ~Corr();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Corr"; }
    virtual ClassId GetClassId() const { return CORR; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Damage
//----------------------------------------------------------------------------

class Damage : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Damage();
    virtual ~Damage();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Damage"; }
    virtual ClassId GetClassId() const { return DAMAGE; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Del
//----------------------------------------------------------------------------

class Del : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Del();
    virtual ~Del();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Del"; }
    virtual ClassId GetClassId() const { return DEL; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Expan
//----------------------------------------------------------------------------

class Expan : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Expan();
    virtual ~Expan();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Expan"; }
    virtual ClassId GetClassId() const { return EXPAN; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Lem
//----------------------------------------------------------------------------

class Lem : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Lem();
    virtual ~Lem();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Lem"; }
    virtual ClassId GetClassId() const { return LEM; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Orig
//----------------------------------------------------------------------------

class Orig : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Orig();
    virtual ~Orig();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Orig"; }
    virtual ClassId GetClassId() const { return ORIG; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Rdg
//----------------------------------------------------------------------------

class Rdg : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rdg();
    virtual ~Rdg();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Rdg"; }
    virtual ClassId GetClassId() const { return RDG; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Reg
//----------------------------------------------------------------------------

class Reg : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Reg();
    virtual ~Reg();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Reg"; }
    virtual ClassId GetClassId() const { return REG; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------------

class Restore : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Restore();
    virtual ~Restore();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Restore"; }
    virtual ClassId GetClassId() const { return RESTORE; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Sic
//----------------------------------------------------------------------------

class Sic : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Sic();
    virtual ~Sic();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Sic"; }
    virtual ClassId GetClassId() const { return SIC; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Supplied
//----------------------------------------------------------------------------

class Supplied : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Supplied();
    virtual ~Supplied();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Supplied"; }
    virtual ClassId GetClassId() const { return SUPPLIED; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Unclear
//----------------------------------------------------------------------------

class Unclear : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Unclear();
    virtual ~Unclear();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Unclear"; }
    virtual ClassId GetClassId() const { return UNCLEAR; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
