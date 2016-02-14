/////////////////////////////////////////////////////////////////////////////
// Name:        aligner.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "aligner.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "note.h"
#include "style.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemAligner
//----------------------------------------------------------------------------

SystemAligner::SystemAligner():
    Object()
{
    Reset();
}

SystemAligner::~SystemAligner()
{
    
}


void SystemAligner::Reset()
{
    Object::Reset();
    m_bottomAlignment = NULL;
    m_bottomAlignment = GetStaffAlignment( 0 );
}

StaffAlignment* SystemAligner::GetStaffAlignment( int idx )
{
    // The last one is always the bottomAlignment (unless if not created)
    if ( m_bottomAlignment ) {
        // remove it temporarily
        this->m_children.pop_back();
    }
    
    if (idx < GetStaffAlignmentCount()) {
        this->m_children.push_back( m_bottomAlignment );
        return dynamic_cast<StaffAlignment*>(m_children.at(idx));
    }
    // check that we are searching for the next one (not a gap)
    assert( idx == GetStaffAlignmentCount() );
    //LogDebug("Creating staff alignment");
    
    // This is the first time we are looking for it (e.g., first staff)
    // We create the StaffAlignment
    StaffAlignment *alignment = new StaffAlignment();
    alignment->SetParent( this );
    m_children.push_back( alignment );
    
    if ( m_bottomAlignment ) {
        this->m_children.push_back( m_bottomAlignment );
    }
    
    return alignment;
}


//----------------------------------------------------------------------------
// StaffAlignment
//----------------------------------------------------------------------------

StaffAlignment::StaffAlignment():
    Object()
{
    m_yRel = 0;
    m_yShift = 0;
    m_maxHeight = 0;
    m_verseCount = 0;
}

StaffAlignment::~StaffAlignment()
{
    
}

void StaffAlignment::SetYShift( int yShift )
{
    if ( yShift < m_yShift )
    {
        m_yShift = yShift;
    }
}

void StaffAlignment::SetMaxHeight( int max_height )
{
    if ( max_height < m_maxHeight )
    {
        m_maxHeight = max_height;
    }
}


void StaffAlignment::SetVerseCount(int verse_count)
{
    // if 0, then assume 1;
    verse_count = std::max(verse_count, 1);
    if ( verse_count > m_verseCount )
    {
        m_verseCount = verse_count;
    }
}

//----------------------------------------------------------------------------
// MeasureAligner
//----------------------------------------------------------------------------

MeasureAligner::MeasureAligner():
    Object()
{
    m_leftAlignment = NULL;
    m_rightAlignment = NULL;
    m_nonJustifiableLeftMargin = 0;
}

MeasureAligner::~MeasureAligner()
{
}

void MeasureAligner::Reset()
{
    Object::Reset();
    m_leftAlignment = new Alignment( 0.0, ALIGNMENT_MEASURE_START );
    AddAlignment( m_leftAlignment );
    m_rightAlignment = new Alignment( 0.0, ALIGNMENT_MEASURE_END );
    AddAlignment( m_rightAlignment );
}

void MeasureAligner::AddAlignment( Alignment *alignment, int idx )
{
	alignment->SetParent( this );
    if ( idx == -1 ) {
        m_children.push_back( alignment );
    }
    else {
        InsertChild( alignment, idx );
    }
}

Alignment* MeasureAligner::GetAlignmentAtTime( double time, AlignmentType type, bool hasEndAlignment )
{
    int i;
    int idx = -1; // the index if we reach the end.
    Alignment *alignment = NULL;
    // First try to see if we already have something at the time position
    for (i = 0; i < GetAlignmentCount(); i++)
    {
        alignment = dynamic_cast<Alignment*>(m_children.at(i));
        assert( alignment );
        
        double alignment_time = alignment->GetTime();
        if ( vrv::AreEqual( alignment_time, time ) ) {
            if (alignment->GetType() == type) {
                return alignment;
            }
            else if ( alignment->GetType() > type ) {
                idx = i;
                break;
            }
        }
        // nothing found, do not go any further but keep the index
        if (alignment->GetTime() > time) {
            idx = i;
            break;
        }
    }
    // nothing found
    if ( idx == -1 ) {
        // this is tricky! Because we want m_rightAlignment to always stay at the end (with hasEndAlignment),
        // we always to insert _before_ the last one - m_rightAlignment is added in Reset()
        if ( hasEndAlignment ) idx = GetAlignmentCount() - 1;
        else idx = GetAlignmentCount();
    }
    Alignment *newAlignment = new Alignment( time, type );
    AddAlignment( newAlignment, idx );
    return newAlignment;
}

void MeasureAligner::SetMaxTime( double time )
{
    if ( m_rightAlignment->GetTime() < time ) {
        m_rightAlignment->SetTime( time );
    }
}
    
//----------------------------------------------------------------------------
// GraceAligner
//----------------------------------------------------------------------------

GraceAligner::GraceAligner():
    MeasureAligner()
{
    m_totalWidth = 0;
}

GraceAligner::~GraceAligner()
{
    
}
    
void GraceAligner::StackNote( Note *note )
{
    m_noteStack.push_back( note );
}
    
void GraceAligner::AlignStack( )
{
    int i;
    double time = 0.0;
    for (i = (int)m_noteStack.size(); i > 0; i--) {
        Note *note = dynamic_cast<Note*>( m_noteStack.at(i-1) );
        assert( note );
        // get the duration of the event
        double duration = note->LayerElement::GetAlignmentDuration( NULL, NULL, false );
        // Time goes backward with grace notes
        time -= duration;
        // Set the hasEndAlignment to false with grace notes because we don't have an end-measure alignment
        note->SetGraceAlignment( this->GetAlignmentAtTime( time, ALIGNMENT_DEFAULT, false ) );
    }
    m_noteStack.clear();
}

//----------------------------------------------------------------------------
// Alignment
//----------------------------------------------------------------------------

Alignment::Alignment( ):
    Object()
{
    m_xRel = 0;
    m_xShift = 0;
    m_maxWidth = 0;
    m_time = 0.0;
    m_type = ALIGNMENT_DEFAULT;
    m_graceAligner = NULL;
}

Alignment::Alignment( double time, AlignmentType type ):
    Object()
{
    m_xRel = 0;
    m_xShift = 0;
    m_maxWidth = 0;
    m_time = time;
    m_type = type;
    m_graceAligner = NULL;
}

Alignment::~Alignment()
{
    if (m_graceAligner) {
        delete m_graceAligner;
    }
    
}

void Alignment::SetXRel( int x_rel )
{
    m_xRel = x_rel;
}

void Alignment::SetXShift( int xShift )
{
    if ( xShift > m_xShift ) {
        m_xShift = xShift;
    }
}

void Alignment::SetMaxWidth( int maxWidth )
{
    if ( maxWidth > m_maxWidth ) {
        m_maxWidth = maxWidth;
    }
}
    
GraceAligner *Alignment::GetGraceAligner( )
{
    if (!m_graceAligner) {
        m_graceAligner = new GraceAligner( );
    }
    return m_graceAligner;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int StaffAlignment::SetAligmentYPos( ArrayPtrVoid *params )
{
    // param 0: the previous staff height
    // param 1: the staff margin
    // param 2: the staff interline sizes (int[2])
    // param 3: the functor to be redirected to SystemAligner (unused)
    int *previousStaffHeight = static_cast<int*>((*params).at(0));
    int *staffMargin = static_cast<int*>((*params).at(1));
    int *interlineSize = static_cast<int*>((*params).at(2));
    
    // take into account the number of lyrics
    if (this->GetVerseCount() > 0) {
        // We need + 1 lyric line space
        m_yShift -= (this->GetVerseCount() + 1) * TEMP_STYLE_LYIRC_LINE_SPACE * (*interlineSize / 2) / PARAM_DENOMINATOR;
    }

    int min_shift = (*staffMargin) + (*previousStaffHeight);
    
    if ( m_yShift > -min_shift) {
        m_yShift = -min_shift;
    }
    
    // for now always four interlines, eventually should be taken from the staffDef, so should the staff size
    (*previousStaffHeight) = 4 * (*interlineSize);
    
    return FUNCTOR_CONTINUE;
}

int StaffAlignment::IntegrateBoundingBoxYShift( ArrayPtrVoid *params )
{
    // param 0: the accumulated shift
    // param 1: the functor to be redirected to the SystemAligner (unused)
    int *shift = static_cast<int*>((*params).at(0));
    
    // integrates the m_yShift into the m_yRel
    m_yRel += m_yShift + (*shift);
    
    // cumulate the shift value
    (*shift) += m_yShift;
    m_yShift = 0;
    
    return FUNCTOR_CONTINUE;
}

int MeasureAligner::IntegrateBoundingBoxXShift( ArrayPtrVoid *params )
{
    // param 0: the accumulated shift
    // param 1: the accumulated justifiable shift
    // param 2: the minimum measure width (unused)
    // param 3: the doc for accessing drawing parameters
    // param 4: the functor to be redirected to the MeasureAligner (unused)
    int *shift = static_cast<int*>((*params).at(0));
    int *justifiable_shift = static_cast<int*>((*params).at(1));
    Doc *doc = static_cast<Doc*>((*params).at(3));
    
    // We start a new MeasureAligner
    // Reset the accumulated shift to 0;
    (*shift) = doc->GetLeftPosition() * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR;;
    (*justifiable_shift) = -1;
    
    return FUNCTOR_CONTINUE;
}
    
int Alignment::IntegrateBoundingBoxGraceXShift( ArrayPtrVoid *params )
{
    if (!m_graceAligner) {
        return FUNCTOR_CONTINUE;
    }
    
    int i;
    int shift = 0;
    for (i = 0; i < (int)m_graceAligner->m_children.size(); i++) {
        Alignment *alignment = dynamic_cast<Alignment*>(m_graceAligner->m_children.at(i));
        assert( alignment );
        alignment->SetXRel( alignment->GetXShift() + shift );
        shift += alignment->GetXShift();
    }
    
    // Set the total width by looking at the position and maximum width of the last alignment
    if ( m_graceAligner->m_children.empty() ) {
        return FUNCTOR_CONTINUE;
    }
    Alignment *alignment = dynamic_cast<Alignment*>(m_graceAligner->m_children.back());
    assert( alignment );
    m_graceAligner->SetWidth( alignment->GetXRel() + alignment->GetMaxWidth() );
    
    return FUNCTOR_CONTINUE;
}

int Alignment::IntegrateBoundingBoxXShift( ArrayPtrVoid *params )
{
    // param 0: the accumulated shift
    // param 1: the accumulated justifiable shift
    // param 2: the minimum measure width
    // param 3: the functor to be redirected to the MeasureAligner (unused)
    int *shift = static_cast<int*>((*params).at(0));
    int *justifiable_shift = static_cast<int*>((*params).at(1));
    int *minMeasureWidth = static_cast<int*>((*params).at(2));
    
    // integrates the m_xShift into the m_xRel
    m_xRel += m_xShift + (*shift);
    // cumulate the shift value and the width
    (*shift) += m_xShift;

    if ((GetType() <= ALIGNMENT_METERSIG_ATTR) && ((*justifiable_shift) < 0)) {
        MeasureAligner *aligner = dynamic_cast<MeasureAligner*>(m_parent);
        assert( aligner );
        aligner->SetNonJustifiableMargin(this->m_xRel + this->m_maxWidth);
    }
    else if ((GetType() > ALIGNMENT_METERSIG_ATTR) && ((*justifiable_shift) < 0)) {
        MeasureAligner *aligner = dynamic_cast<MeasureAligner*>(m_parent);
        assert( aligner );
        (*justifiable_shift) = aligner->GetNonJustifiableMargin();
    }

    if (GetType() == ALIGNMENT_FULLMEASURE2) {
       (*minMeasureWidth) *= 2;
    }
    else if (GetType() == ALIGNMENT_MEASURE_END) {
        m_xRel = std::max( m_xRel, (*minMeasureWidth) + (*justifiable_shift) );
    }

    // reset member to 0
    m_xShift = 0;
    
    return FUNCTOR_CONTINUE;
}

int MeasureAligner::SetAlignmentXPos( ArrayPtrVoid *params )
{
    // param 0: the previous time position
    // param 1: the previous x rel position
    // param 2: duration of the longest note (unused)
    // param 3: the doc (unused)
    // param 4: the functor to be redirected to the MeasureAligner (unused)
    double *previousTime = static_cast<double*>((*params).at(0));
    int *previousXRel = static_cast<int*>((*params).at(1));
    
    // We start a new MeasureAligner
    // Reset the previous time position and x_rel to 0;
    (*previousTime) = 0.0;
    (*previousXRel) = 0;
    
    return FUNCTOR_CONTINUE;
}

    
/* Compute "ideal" horizontal space to allow for a given time interval, ignoring the need
to keep consecutive symbols from overlapping or nearly overlapping: we assume spacing
will be increased as necessary later to avoid that. For modern notation (CMN), ideal space
is a function of time interval.
 
The power function we currently use isn't quite right; see _Behind Bars_, p. 39. We also
need more flexibility: for example, for some purposes, spacing proportional to duration is
desirable. The best solution is probably to get ideal spacing from a user-definable table. */

int Alignment::HorizontalSpaceForDuration(double intervalTime, int maxActualDur, double spacingLinear, double spacingNonLinear)
{
    /* If the longest duration interval in the score is longer than semibreve, adjust spacing so
       that interval gets the space a semibreve would ordinarily get. (maxActualDur is in our
       internal code format: cf. attdef.h). ??TO BE DONE */
    if (maxActualDur < DUR_1) intervalTime /= pow(2.0, DUR_1 - maxActualDur);
    int intervalXRel = pow( intervalTime, spacingNonLinear ) * pow( spacingLinear * 5.0, 2 );
    return intervalXRel;
}

int Alignment::SetAlignmentXPos( ArrayPtrVoid *params )
{
    // param 0: the previous time position
    // param 1: the previous x rel position
    // param 2: duration of the longest note
    // param 3: the doc (unused)
    // param 4: the functor to be redirected to the MeasureAligner (unused)
    double *previousTime = static_cast<double*>((*params).at(0));
    int *previousXRel = static_cast<int*>((*params).at(1));
    int *maxActualDur = static_cast<int*>((*params).at(2));
    Doc *doc = static_cast<Doc*>((*params).at(3));
    
    int intervalXRel = 0;
    double intervalTime = (m_time - (*previousTime));
    if ( intervalTime > 0.0 ) {
        intervalXRel = HorizontalSpaceForDuration(intervalTime, *maxActualDur, doc->GetSpacingLinear(), doc->GetSpacingNonLinear());
        //LogDebug("SetAlignmentXPos: intervalTime=%.2f intervalXRel=%d", intervalTime, intervalXRel);
    }
    m_xRel = (*previousXRel) + (intervalXRel) * DEFINITON_FACTOR;
    (*previousTime) = m_time;
    (*previousXRel) = m_xRel;
    
    return FUNCTOR_CONTINUE;
}
    
int MeasureAligner::JustifyX( ArrayPtrVoid *params )
{
    // param 0: the justification ratio
    // param 1: the justification ratio for the measure (depends on the margin)
    // param 2: the non justifiable margin
    // param 3: the system full width (without system margins) (unused)
    // param 4: the functor to be redirected to the MeasureAligner (unused)
    double *ratio =static_cast<double*>((*params).at(0));
    double *measureRatio =static_cast<double*>((*params).at(1));
    int *margin =static_cast<int*>((*params).at(2));
    
    int width = GetRightAlignment()->GetXRel() + GetRightAlignment()->GetMaxWidth();
    
    // the ratio in the measure has to take into account the non-justifiable width
    // for elements within the margin, we do not move them
    // for after the margin (right) we have a position that is given by:
    // (m_xRel - margin) * measureRatio + margin, where measureRatio is given by:
    // (ratio - 1) * (margin / justifiable) + ratio
    
    (*measureRatio) = ((*ratio) - 1) * ((double)m_nonJustifiableLeftMargin / (double)width) + (*ratio);
    (*margin) = m_nonJustifiableLeftMargin;
    
    return FUNCTOR_CONTINUE;
}


int Alignment::JustifyX( ArrayPtrVoid *params )
{
    // param 0: the justification ratio
    // param 1: the justification ratio for the measure (depends on the margin)
    // param 2: the non-justifiable margin
    // param 3: the system full width (without system margins) (unused)
    // param 4: the functor to be redirected to the MeasureAligner (unused)
    double *ratio =static_cast<double*>((*params).at(0));
    double *measureRatio =static_cast<double*>((*params).at(1));
    int *margin =static_cast<int*>((*params).at(2));
    
    if (GetType() == ALIGNMENT_MEASURE_START) {
        return FUNCTOR_CONTINUE;
    }
    else if (GetType() == ALIGNMENT_MEASURE_END) {
        this->m_xRel = ceil((*ratio) * (double)this->m_xRel);
        return FUNCTOR_CONTINUE;
    }
    
    // the ratio in the measure has to take into account the non-justifiable width
    // for elements within the margin, we do not move them
    // for after the margin (right) we have a position that is given by:
    // (m_xRel - margin) * measureRatio + margin, where measureRatio is given by:
    // (ratio - 1) * (margin / justifiable) + ratio
    
    if ((GetType() < ALIGNMENT_CLEF_ATTR) || (GetType() > ALIGNMENT_METERSIG_ATTR)) {
        this->m_xRel = ceil(((double)this->m_xRel - (double)(*margin)) * (*measureRatio)) + (*margin);
    }

    return FUNCTOR_CONTINUE;
}
    
} // namespace vrv
