/////////////////////////////////////////////////////////////////////////////
// Name:        fb.cpp
// Author:      Rodolfo Zitellini
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fb.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {
    
    //----------------------------------------------------------------------------
    // Fb
    //----------------------------------------------------------------------------
    
    Fb::Fb() : Object("fb-")
    {
        
        Reset();
    }
    
    Fb::~Fb()
    {
    }
    
    void Fb::Reset()
    {
        Object::Reset();
    }
    
    void Fb::AddChild(Object *child)
    {
        if (child->Is(FIGURE)) {
            assert(dynamic_cast<Figure *>(child));
        }
        else {
            LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
            assert(false);
        }
        
        child->SetParent(this);
        m_children.push_back(child);
        Modify();
    }
    
    //----------------------------------------------------------------------------
    // Figure
    //----------------------------------------------------------------------------
    
    Figure::Figure() : TextElement("f-")
    {
        Reset();
    }
    
    Figure::~Figure()
    {
    }
    
    void Figure::Reset()
    {
        TextElement::Reset();
    }
    
} // namespace vrv
