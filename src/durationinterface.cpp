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
#include <stdlib.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "mensur.h"
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
    AttFermatapresent(),
    AttStaffident()
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
    ResetStaffident();
}

double DurationInterface::GetAlignmentDuration( int num, int numbase )
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
    
double DurationInterface::GetAlignmentMensuralDuration( int num, int numbase, Mensur *currentMensur )
{
    int note_dur = this->GetDurGes() != DURATION_NONE ? this->GetDurGes() : this->GetActualDur();
    
    if (!currentMensur) {
        LogWarning("No current mensur for calculating duration" );
        return DUR_MENSURAL_REF;
    }
    
    if (this->HasNum()) num *=this->GetNum();
    if (this->HasNumbase()) numbase *=this->GetNumbase();
    if (currentMensur->HasNum()) num *=currentMensur->GetNum();
    if (currentMensur->HasNumbase()) numbase *=currentMensur->GetNumbase();
    
    double ratio = 0.0;
    double duration = (double)DUR_MENSURAL_REF;
    switch (note_dur) {
        case DUR_MX : duration *= (double)abs(currentMensur->GetModusminor()) * (double)abs(currentMensur->GetModusmaior()); break;
        case DUR_LG : duration *= (double)abs(currentMensur->GetModusminor()); break;
        case DUR_BR : break;
        case DUR_1 : duration /= (double)abs(currentMensur->GetTempus()); break;
        default:
            ratio = pow(2.0, (double)(note_dur - DUR_2));
            duration /= (double)abs(currentMensur->GetTempus()) * (double)abs(currentMensur->GetProlatio()) * ratio;
            break;
    }
    duration *= (double)numbase / (double)num;
    //LogDebug("Duration %d; %d/%d; Alignement %f; Ratio %f", note_dur, num, numbase, duration, ratio );
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
