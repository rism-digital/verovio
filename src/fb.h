/////////////////////////////////////////////////////////////////////////////
// Name:        fb.h
// Author:      Rodolfo Zitellini
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FB_H__
#define __VRV_FB_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {
    
    //----------------------------------------------------------------------------
    // Fb
    //----------------------------------------------------------------------------
    
    /**
     * This class models the MEI <fb>
     */
    class Fb : public Object {
    public:
        /**
         * @name Constructors, destructors, reset and class name methods
         * Reset method reset all attribute classes
         */
        ///@{
        Fb();
        virtual ~Fb();
        virtual void Reset();
        virtual std::string GetClassName() const { return "Fb"; }
        virtual ClassId GetClassId() const { return FB; }
        ///@}
        
        /**
         * Add an element (f) to an fb.
         * Only supported elements will be actually added to the child list.
         */
        virtual void AddChild(Object *object);
        
    private:
        //
    public:
        //
    private:
    };
    
    //----------------------------------------------------------------------------
    // F / Figure
    //----------------------------------------------------------------------------
    
    /**
     * This if the Figure class
     */
    class F : public TextElement {
    public:
        /**
         * @name Constructors, destructors, reset and class name methods
         * Reset method reset all attribute classes
         */
        ///@{
        F();
        virtual ~F();
        virtual void Reset();
        virtual std::string GetClassName() const { return "F"; }
        virtual ClassId GetClassId() const { return FIGURE; }
        ///@}
        
        /**
         * @name Set and get the text content.
         * The text content is a std::wstring that needs to be converted to UTF16.
         * See MeiInput::ReadText and MeiInput ::WriteText
         */
        ///@{
        void SetText(std::wstring text) { m_text = text; }
        std::wstring GetText() const { return m_text; }
        ///@}
        
    private:
        //
    public:
        //
    protected:
        /** The text content */
        std::wstring m_text;
        
    private:
    };
    
} // namespace vrv

#endif
