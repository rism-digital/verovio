/////////////////////////////////////////////////////////////////////////////
// Name:        timeinterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "timeinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "layerelement.h"
#include "vrv.h"

namespace vrv {


//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

TimeSpanningInterface::TimeSpanningInterface():
    AttStartendid(),
    AttStartid()
{
    Reset();
}


TimeSpanningInterface::~TimeSpanningInterface()
{
}    
    
void TimeSpanningInterface::Reset()
{
    ResetStartendid();
    ResetStartid();
    m_start = NULL;
    m_end = NULL;
    m_startUuid = "";
    m_endUuid = "";
}
    
void TimeSpanningInterface::SetStart( LayerElement *start )
{
    assert( !m_start );
    m_start = start;
}

void TimeSpanningInterface::SetEnd( LayerElement *end )
{
    assert( !m_end );
    m_end = end;
}
    
void TimeSpanningInterface::SetUuidStr()
{
    if (this->HasStartid()) {
        m_startUuid = this->ExtractUuidFragment(this->GetStartid());
    }
    if (this->HasEndid()) {
        m_endUuid = this->ExtractUuidFragment(this->GetEndid());
    }
}
    
bool TimeSpanningInterface::SetStartAndEnd( LayerElement *element )
{
    //LogDebug("%s - %s - %s", element->GetUuid().c_str(), m_startUuid.c_str(), m_endUuid.c_str() );
    if (!m_start && (element->GetUuid() == m_startUuid) ) {
        this->SetStart( element );
    }
    else if (!m_end && (element->GetUuid() == m_endUuid) ) {
        this->SetEnd( element );
    }
    return ( m_start && m_end );
}
    
std::string TimeSpanningInterface::ExtractUuidFragment(std::string refUuid)
{
    size_t pos = refUuid.find_last_of("#");
    if ( (pos != std::string::npos) && (pos < refUuid.length() - 1) ) {
        refUuid = refUuid.substr( pos + 1 );
    }
    return refUuid;
}

} // namespace vrv
