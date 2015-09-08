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
#include "staff.h"
#include "vrv.h"

namespace vrv {


//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

TimeSpanningInterface::TimeSpanningInterface(): Interface(),
    AttStartendid(),
    AttStartid(),
    AttTimestampMusical(),
    AttTimestamp2Musical()
{
    RegisterInterfaceAttClass(ATT_STARTENDID);
    RegisterInterfaceAttClass(ATT_STARTID);
    RegisterInterfaceAttClass(ATT_TIMESTAMPMUSICAL);
    RegisterInterfaceAttClass(ATT_TIMESTAMP2MUSICAL);
    Reset();
}


TimeSpanningInterface::~TimeSpanningInterface()
{
}    
    
void TimeSpanningInterface::Reset()
{
    ResetStartendid();
    ResetStartid();
    ResetTimestampMusical();
    ResetTimestamp2Musical();
    
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
    
//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------
   
int TimeSpanningInterface::InterfacePrepareTimeSpanning( ArrayPtrVoid *params, DocObject *object )
{
    // param 0: std::vector<DocObject*>* that holds the current elements to match
    // param 1: bool* fillList for indicating whether the elements have to be stack or not
    std::vector<DocObject*> *elements = static_cast<std::vector<DocObject*>*>((*params).at(0));
    bool *fillList = static_cast<bool*>((*params).at(1));
    
    if ((*fillList)==false) {
        return FUNCTOR_CONTINUE;
    }
    
    this->SetUuidStr();
    elements->push_back(object);
    
    return FUNCTOR_CONTINUE;
}
    
int TimeSpanningInterface::InterfaceFillStaffCurrentTimeSpanning( ArrayPtrVoid *params, DocObject *object  )
{
    // param 0: std::vector<DocObject*>* of the current running TimeSpanningInterface elements
    std::vector<DocObject*> *elements = static_cast<std::vector<DocObject*>*>((*params).at(0));
    
    if (this->HasStartAndEnd()) {
        if ( GetStart()->GetFirstParent( STAFF ) != GetEnd()->GetFirstParent( STAFF ) ) {
            // We have a running syl started in a previous measure
            elements->push_back(object);
        }
    }
    return FUNCTOR_CONTINUE;
}

int TimeSpanningInterface::InterfaceResetDrawing( ArrayPtrVoid *params, DocObject *object  )
{
    m_start = NULL;
    m_end = NULL;
    m_startUuid = "";
    m_endUuid = "";
    return FUNCTOR_CONTINUE;
}
    
} // namespace vrv
