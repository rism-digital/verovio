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
    AttCommonNComparison( ClassId AttClassId, const int n ):
        AttComparison( AttClassId )
    {
        m_n = n;
    };
    
    void SetN( int n ) { m_n = n; }
    
    virtual bool operator() (Object *object)
    {
        if (!MatchesType(object)) return false;
        // This should not happen but, but just in case
        if (!object->HasAttClass( ATT_COMMON )) return false;
        AttCommon *element = dynamic_cast<AttCommon*>(object);
        assert( element );
        return (element->GetN() == m_n);
    }
    
private:
    int m_n;
    
};
    
} // namespace vrv

#endif
