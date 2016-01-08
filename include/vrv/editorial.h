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
#include "object.h"

namespace vrv {

class FloatingElement;
class Layer;
class LayerElement;
class Measure;
class ScoreDef;
class Staff;
class StaffDef;
class StaffGrp;
class TextElement;

enum VisibilityType { Hidden = 0, Visible };

//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

/**
 * This class is a base class for the editorial element containing musical
 * content. For example <rgd> or <add>.
 * It is not an abstract class but should not be instantiated directly.
 */
class EditorialElement : public DocObject, public AttCommon, public AttCommonPart {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    EditorialElement();
    EditorialElement(std::string classid);
    virtual ~EditorialElement();
    virtual void Reset();
    virtual ClassId Is() { return EDITORIAL_ELEMENT; };
    ///@}

    /**
     * @name Add children to an editorial element.
     */
    ///@{
    void AddFloatingElement(FloatingElement *child);
    void AddLayerElement(LayerElement *child);
    void AddTextElement(TextElement *child);
    void AddLayer(Layer *child);
    void AddMeasure(Measure *child);
    void AddScoreDef(ScoreDef *child);
    void AddStaff(Staff *child);
    void AddStaffDef(StaffDef *child);
    void AddStaffGrp(StaffGrp *child);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Fill a page by adding systems with the appropriate length
     * For EditorialElement, this means only moving them since their width is not
     * taken into account. Only system children EditorialElement are processed.
     */
    virtual int CastOffSystems(ArrayPtrVoid *params);

protected:
private:
public:
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
    virtual std::string GetClassName() { return "Abbr"; };
    virtual ClassId Is() { return ABBR; };
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
    virtual std::string GetClassName() { return "Add"; };
    virtual ClassId Is() { return ADD; };
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// Annot
//----------------------------------------------------------------------------

class Annot : public EditorialElement, public AttPlist, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Annot();
    virtual ~Annot();
    virtual void Reset();
    virtual std::string GetClassName() { return "Annot"; };
    virtual ClassId Is() { return ANNOT; };
    ///@}

private:
    //
public:
    /**
     * A copy of the annot content tree stored as pugi::xml_document
     */
    pugi::xml_document m_content;

private:
};

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

class App : public EditorialElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    App();
    App(EditorialLevel level);
    virtual ~App();
    virtual void Reset();
    virtual std::string GetClassName() { return "App"; };
    virtual ClassId Is() { return APP; };
    ///@}

    /** Getter for level **/
    EditorialLevel GetLevel() { return m_level; };

    /**
     * Add children to a apparatus.
     * Tests if lemOrRdg is Lem or Rdg.
     */
    void AddLemOrRdg(EditorialElement *lemOrRdg);

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
    virtual std::string GetClassName() { return "Corr"; };
    virtual ClassId Is() { return CORR; };
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
    virtual std::string GetClassName() { return "Damage"; };
    virtual ClassId Is() { return DAMAGE; };
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
    virtual std::string GetClassName() { return "Del"; };
    virtual ClassId Is() { return DEL; };
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
    virtual std::string GetClassName() { return "Expan"; };
    virtual ClassId Is() { return EXPAN; };
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
     * Reset method reset all attribute classes
     */
    ///@{
    Lem();
    virtual ~Lem();
    virtual void Reset();
    virtual std::string GetClassName() { return "Lem"; };
    virtual ClassId Is() { return LEM; };
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
    virtual std::string GetClassName() { return "Orig"; };
    virtual ClassId Is() { return ORIG; };
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
     * Reset method reset all attribute classes
     */
    ///@{
    Rdg();
    virtual ~Rdg();
    virtual void Reset();
    virtual std::string GetClassName() { return "Rdg"; };
    virtual ClassId Is() { return RDG; };
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
    virtual std::string GetClassName() { return "Reg"; };
    virtual ClassId Is() { return REG; };
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
    virtual std::string GetClassName() { return "Restore"; };
    virtual ClassId Is() { return RESTORE; };
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
    virtual std::string GetClassName() { return "Sic"; };
    virtual ClassId Is() { return SIC; };
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
     * Reset method reset all attribute classes
     */
    ///@{
    Supplied();
    virtual ~Supplied();
    virtual void Reset();
    virtual std::string GetClassName() { return "Supplied"; };
    virtual ClassId Is() { return SUPPLIED; };
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
     * Reset method reset all attribute classes
     */
    ///@{
    Unclear();
    virtual ~Unclear();
    virtual void Reset();
    virtual std::string GetClassName() { return "Unclear"; };
    virtual ClassId Is() { return UNCLEAR; };
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
