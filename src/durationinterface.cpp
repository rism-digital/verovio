/////////////////////////////////////////////////////////////////////////////
// Name:        durationinterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "durationinterface.h"

//----------------------------------------------------------------------------

#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"

namespace vrv {


//----------------------------------------------------------------------------
// DurationInterface
//----------------------------------------------------------------------------

DurationInterface::DurationInterface():
    AttAugmentdots(),
    AttBeamsecondary(),
    AttDurationMusical(),
    AttDurationPerformed(),
    AttDurationRatio(),
    AttFermatapresent()
{
    Reset();
}


DurationInterface::~DurationInterface()
{
}
    
    
void DurationInterface::Reset()
{
    ResetAugmentdots();
    ResetBeamsecondary();
    ResetDurationMusical();
    ResetDurationPerformed();
    ResetDurationRatio();
    ResetFermatapresent();
}

double DurationInterface::GetAlignementDuration( int num, int numbase )
{
    int note_dur = this->GetDurGes() != DURATION_NONE ? this->GetDurGes() : this->GetDur();
    
    double duration = DUR_MAX / pow (2.0, (double)(note_dur - 2.0)) * numbase / num;
    if ( GetDots() > 0 ) {
        duration = 2 * duration - (duration / pow(2, GetDots()));
    }
    //LogDebug("Duration %d; Dot %d; Alignement %f", m_dur, m_dots, duration );
    return duration;
}

bool DurationInterface::IsInBeam( Object *noteOrRest )
{
    Beam *beam = dynamic_cast<Beam*>( noteOrRest->GetFirstParent( &typeid(Beam), MAX_BEAM_DEPTH ) );
    if ( !beam ) {
        return false;
    }
    return true;
}

bool DurationInterface::IsFirstInBeam( Object *noteOrRest )
{
    Beam *beam = dynamic_cast<Beam*>( noteOrRest->GetFirstParent( &typeid(Beam), MAX_BEAM_DEPTH ) );
    if ( !beam ) {
        return false;
    }
    ListOfObjects *notesOrRests = beam->GetList( beam );
    ListOfObjects::iterator iter = notesOrRests->begin();
    if ( *iter == noteOrRest ) {
        return true;
    }
    return false;    
}

bool DurationInterface::IsLastInBeam( Object *noteOrRest )
{
    Beam *beam = dynamic_cast<Beam*>( noteOrRest->GetFirstParent( &typeid(Beam), MAX_BEAM_DEPTH ) );
    if ( !beam ) {
        return false;
    }
    ListOfObjects *notesOrRests = beam->GetList( beam );
    ListOfObjects::reverse_iterator iter = notesOrRests->rbegin();
    if ( *iter == noteOrRest ) {
        return true;
    }
    return false;    
    
}
    
int DurationInterface::GetActualDur()
{
    // maxima (-1) is a mensural only value
    if (this->GetDur() == DURATION_maxima) return DUR_MX;
    return (this->GetDur() & DUR_MENSURAL_MASK);
}

bool DurationInterface::IsMensural()
{
    // maxima (-1) is a mensural only value
    if (this->GetDur() == DURATION_maxima) return true;
    return (this->GetDur() > DUR_MENSURAL_MASK);
}

bool DurationInterface::HasIdenticalDurationInterface( DurationInterface *otherDurationInterface )
{
    if ( !otherDurationInterface ) {
        return false;
    }
    // beam requires value by value comparison
    //if ( this->m_beam != otherDurationInterface->m_beam ) {
    //    return false;
    //}
    //if ( this->m_breakSec != otherDurationInterface->m_breakSec ) {
    //    return false;
    //}
    if ( this->GetDots() != otherDurationInterface->GetDots() ) {
        return false;
    }
    if ( this->GetDur() != otherDurationInterface->GetDur() ) {
        return false;
    }
    if ( this->GetNum() != otherDurationInterface->GetNum() ) {
        return false;
    }
    if ( this->GetNumbase() != otherDurationInterface->GetNumbase() ) {
        return false;
    }
    // tuplet requires value by value comparison
    //if ( this->m_tuplet != otherDurationInterface->m_tuplet ) {
    //    return false;
    //}
    if ( this->m_fermata != otherDurationInterface->m_fermata ) {
        return false;
    }
    return true;
}

} // namespace vrv
