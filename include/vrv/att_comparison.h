/////////////////////////////////////////////////////////////////////////////
// Name:        att_comparison.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATT_COMPARISON_H__
#define __VRV_ATT_COMPARISON_H__

#include "atts_shared.h"
#include "durationinterface.h"

namespace vrv {
    
enum DurExtreme {
    LONGEST = 0,
    SHORTEST
};
    
//----------------------------------------------------------------------------
// AttCommonNComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and has a @n of value n.
 */
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
    
//----------------------------------------------------------------------------
// AttDurExtreme
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object the extreme duration so far
 * The object has to have a DurationInterface and to have a @dur.
 * The class can look for LONGEST or SHORTEST duration (Constructor)
 */
class AttDurExtreme: public AttComparison
{
    
public:
    AttDurExtreme(DurExtreme extremeType):
        AttComparison(OBJECT)
    {
        m_extremeType = extremeType;
        if (m_extremeType == LONGEST) m_extremeDur = -VRV_UNSET;
        else m_extremeDur = VRV_UNSET;
    };
    
    virtual bool operator() (Object *object)
    {
        if (!object->HasInterface(INTERFACE_DURATION)) return false;
        DurationInterface *interface = dynamic_cast<DurationInterface*>(object);
        assert( interface );
        if (interface->HasDur()) {
            if ((m_extremeType == LONGEST) && (interface->GetActualDur() < m_extremeDur)) {
                m_extremeDur = interface->GetActualDur();
                return true;
            }
            else if ((m_extremeType == SHORTEST) && (interface->GetActualDur() > m_extremeDur)) {
                m_extremeDur = interface->GetActualDur();
                return true;
            }
        }
        return false;
    }
    
private:
    int m_extremeDur;
    DurExtreme m_extremeType;
    
};
    
} // namespace vrv

#endif
