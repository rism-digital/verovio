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
class Lem;
class Measure;
class Rdg;
class ScoreDef;
class Staff;
class StaffDef;
class StaffGrp;
    
enum VisibilityType {
    Hidden = 0,
    Visible
};
    
//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

/**
 * This class is a base class for the editorial element containing musical
 * content, for example <rgd> or <add>.
 * It is not an abstract class but should not be instantiated directly.
 */
class EditorialElement: public DocObject,
    public AttCommon
{
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
    virtual ClassId Is() { return EDITORIAL_ELEMENT; };
    ///@}

    /**
     * @name Add children to an editorial element.
     */
    ///@{
    void AddFloatingElement( FloatingElement *child );
    void AddLayerElement( LayerElement *child );
    void AddLayer( Layer *child );
    void AddMeasure( Measure *child );
    void AddScoreDef( ScoreDef *child );
    void AddStaff( Staff *child );
    void AddStaffDef( StaffDef *child );
    void AddStaffGrp( StaffGrp *child );
    ///@}
    
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Fill a page by adding systems with the appropriate length
     * For EditorialElement, this means only moving them since their width is not
     * taken into account. Only system children EditorialElement are processed.
     */
    virtual int CastOffSystems( ArrayPtrVoid *params );
    
protected:
    
private:
    
public:
    VisibilityType m_visibility;
    
private:
    
};

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

class App: public EditorialElement
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    App();
    App( EditorialLevel level );
    virtual ~App();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "App"; };
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
    
public:
    
private:

};
    
//----------------------------------------------------------------------------
// Lem
//----------------------------------------------------------------------------

class Lem: public EditorialElement,
    public AttSource
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Lem();
    virtual ~Lem();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Lem"; };
    virtual ClassId Is() { return LEM; };
    ///@}
    
protected:
    
private:
    
public:
    
private:
    
};

//----------------------------------------------------------------------------
// Rdg
//----------------------------------------------------------------------------

class Rdg: public EditorialElement,
    public AttSource
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rdg();
    virtual ~Rdg();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Rdg"; };
    virtual ClassId Is() { return RDG; };
    ///@}
    
protected:
    
private:
    
public:
    
private:
    
};    

//----------------------------------------------------------------------------
// Supplied
//----------------------------------------------------------------------------

class Supplied: public EditorialElement,
    public AttSource
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Supplied();
    virtual ~Supplied();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Supplied"; };
    virtual ClassId Is() { return SUPPLIED; };
    ///@}
    
protected:
    
private:
    
public:
    
private:
    
};
    
//----------------------------------------------------------------------------
// Annot
//----------------------------------------------------------------------------

class Annot: public EditorialElement,
    public AttPlist,
    public AttSource
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Annot();
    virtual ~Annot();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Annot"; };
    virtual ClassId Is() { return ANNOT; };
    ///@}
    
protected:
    
private:
    
public:
    /**
     * A copy of the annot content tree stored as pugi::xml_document
     */
    pugi::xml_document m_content;
    
private:
    
};

} // namespace vrv    
    
#endif
