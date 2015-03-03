/////////////////////////////////////////////////////////////////////////////
// Name:        durationinterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "durationinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "vrv.h"

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
    int note_dur = this->GetDurGes() != DURATION_NONE ? this->GetDurGes() : this->GetActualDur();
    
    if (this->HasNum()) num *=this->GetNum();
    if (this->HasNumbase()) numbase *=this->GetNumbase();
    
    double duration = DUR_MAX / pow (2.0, (double)(note_dur - 2.0)) * numbase / num;
    if ( GetDots() > 0 ) {
        duration = 2 * duration - (duration / pow(2, GetDots()));
    }
    //LogDebug("Duration %d; Dot %d; Alignement %f", note_dur, GetDots(), duration );
    return duration;
}
    
double DurationInterface::GetAlignementMensuralDuration( int num, int numbase, Mensur *currentMensur )
{
    return 512;
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
    // This should never happen because it is fully implemented
    LogError( "DurationInterface::HasIdenticalDurationInterface missing" );
    assert( false );
    return false;
    /*
    if ( !otherDurationInterface ) {
        return false;
    }
    */
}

} // namespace vrv
