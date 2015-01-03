/////////////////////////////////////////////////////////////////////////////
// Name:        att_comparison.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATT_COMPARISON_H__
#define __VRV_ATT_COMPARISON_H__

#include "atts_shared.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// AttCommonNComparison
//----------------------------------------------------------------------------

class AttCommonNComparison: public AttComparison
{
    
public:
    AttCommonNComparison( const std::type_info *elementType, const int n ):
    AttComparison( elementType )
    {
        m_n = n;
    };
    
    virtual bool operator() (Object *object)
    {
        if  (typeid(*object) != *m_elementType) {
            return false;
        }
        AttCommon *element = dynamic_cast<AttCommon*>(object);
        if (!element) {
            return false;
        }
        return (element->GetN() == m_n);
    }
    
private:
    int m_n;
    
};
    
} // namespace vrv

#endif
