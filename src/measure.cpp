/////////////////////////////////////////////////////////////////////////////
// Name:        measure.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "measure.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "floatingelement.h"
#include "page.h"
#include "staff.h"
#include "system.h"
#include "timeinterface.h"
#include "timestamp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Measure
//----------------------------------------------------------------------------

Measure::Measure(bool measureMusic, int logMeasureNb) : Object("measure-"), AttCommon(), AttMeasureLog(), AttPointing()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_MEASURELOG);
    RegisterAttClass(ATT_POINTING);

    m_measuredMusic = measureMusic;
    // We set parent to it because we want to access the parent doc from the aligners
    m_measureAligner.SetParent(this);
    // Idem for timestamps
    m_timestampAligner.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;

    Reset();
}

Measure::~Measure()
{
    // We need to delete own objects
    Reset();
}

void Measure::Reset()
{
    Object::Reset();
    ResetCommon();
    ResetMeasureLog();
    ResetPointing();

    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    m_timestampAligner.Reset();
    m_measuredMusic = true;
    m_xAbs = VRV_UNSET;
    m_drawingXRel = 0;
    m_drawingX = 0;

    // by default, we have a single barLine on the right (none on the left)
    m_rightBarLine.SetForm(this->GetRight());
    m_leftBarLine.SetForm(this->GetLeft());

    if (!m_measuredMusic) {
        m_xAbs = 0;
    }
}

void Measure::AddFloatingElement(FloatingElement *element)
{
    element->SetParent(this);
    m_children.push_back(element);

    if (element->Is() == STAFF) {
        Staff *staff = dynamic_cast<Staff *>(element);
        assert(staff);
        if (staff->GetN() < 1) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one staff as a previous child.
            staff->SetN(this->GetChildCount());
        }
    }
}

void Measure::AddStaff(Staff *staff)
{
    staff->SetParent(this);
    m_children.push_back(staff);

    if (staff->GetN() < 1) {
        // This is not 100% safe if we have a <app> and <rdg> with more than
        // one staff as a previous child.
        staff->SetN(this->GetChildCount());
    }
}

int Measure::GetLeftBarLineXRel() const
{
    if (m_measureAligner.GetLeftBarLineAlignment()) {
        return m_measureAligner.GetLeftBarLineAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetLeftBarLineX1Rel() const
{
    int x = GetLeftBarLineXRel();
    if (m_leftBarLine.HasUpdatedBB()) {
        x += m_leftBarLine.m_contentBB_x1;
    }
    return x;
}

int Measure::GetLeftBarLineX2Rel() const
{
    int x = GetLeftBarLineXRel();
    if (m_leftBarLine.HasUpdatedBB()) {
        x += m_leftBarLine.m_contentBB_x2;
    }
    return x;
}

int Measure::GetRightBarLineXRel() const
{
    if (m_measureAligner.GetRightBarLineAlignment()) {
        return m_measureAligner.GetRightBarLineAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetRightBarLineX1Rel() const
{
    int x = GetRightBarLineXRel();
    if (m_rightBarLine.HasUpdatedBB()) {
        x += m_rightBarLine.m_contentBB_x1;
    }
    return x;
}

int Measure::GetRightBarLineX2Rel() const
{
    int x = GetRightBarLineXRel();
    if (m_rightBarLine.HasUpdatedBB()) {
        x += m_rightBarLine.m_contentBB_x2;
    }
    return x;
}

int Measure::GetWidth() const
{
    assert(m_measureAligner.GetRightAlignment());
    if (m_measureAligner.GetRightAlignment()) {
        return m_measureAligner.GetRightAlignment()->GetXRel();
    }
    return 0;
}

void Measure::SetDrawingScoreDef(ScoreDef *drawingScoreDef)
{
    assert(!m_drawingScoreDef); // We should always call UnsetCurrentScoreDef before

    m_drawingScoreDef = new ScoreDef();
    *m_drawingScoreDef = *drawingScoreDef;
}

//----------------------------------------------------------------------------
// Measure functor methods
//----------------------------------------------------------------------------

int Measure::UnsetCurrentScoreDef(ArrayPtrVoid *params)
{
    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    return FUNCTOR_CONTINUE;
};

int Measure::ResetHorizontalAlignment(ArrayPtrVoid *params)
{
    m_drawingXRel = 0;
    m_drawingX = 0;
    if (m_measureAligner.GetLeftAlignment()) {
        m_measureAligner.GetLeftAlignment()->SetXRel(0);
    }
    if (m_measureAligner.GetRightAlignment()) {
        m_measureAligner.GetRightAlignment()->SetXRel(0);
    }

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontally(ArrayPtrVoid *params)
{
    // param 0: the measureAligner
    // param 1: the time (unused)
    // param 2: the current Mensur (unused)
    // param 3: the current MeterSig (unused)
    // param 4: the functor for passing it to the TimeStampAligner (unused)
    MeasureAligner **measureAligner = static_cast<MeasureAligner **>((*params).at(0));

    // clear the content of the measureAligner
    m_measureAligner.Reset();

    // here we transfer the @left and @right values to the barLine objects
    this->SetLeftBarLineType(this->GetLeft());
    this->SetRightBarLineType(this->GetRight());

    // point to it
    (*measureAligner) = &m_measureAligner;

    if (m_leftBarLine.GetForm() != BARRENDITION_NONE) {
        m_leftBarLine.SetAlignment(m_measureAligner.GetLeftBarLineAlignment());
    }

    if (m_rightBarLine.GetForm() != BARRENDITION_NONE) {
        m_rightBarLine.SetAlignment(m_measureAligner.GetRightBarLineAlignment());
    }

    // LogDebug("\n ***** Align measure %d", this->GetN());

    assert(*measureAligner);

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontallyEnd(ArrayPtrVoid *params)
{
    // param 0: the measureAligner (unused)
    // param 1: the time (unused)
    // param 2: the current Mensur (unused)
    // param 3: the current MeterSig (unused)
    // param 4: the functor for passing it to the TimeStampAligner
    Functor *alignHorizontally = static_cast<Functor *>((*params).at(4));

    // We also need to align the timestamps - we do it at the end since we need the *meterSig to be initialized by a
    // Layer. Obviously this will not work with different time signature. However, I am not sure how this would work in
    // MEI anyway.
    m_timestampAligner.Process(alignHorizontally, params);

    return FUNCTOR_CONTINUE;
}

int Measure::AlignVertically(ArrayPtrVoid *params)
{
    // param 0: the systemAligner (unused)
    // param 1: the staffIdx
    // param 2: the staffN (unused)
    // param 3: the doc (unused)
    int *staffIdx = static_cast<int *>((*params).at(1));

    // we also need to reset the staffNb
    (*staffIdx) = 0;

    return FUNCTOR_CONTINUE;
}

int Measure::SetBoundingBoxXShift(ArrayPtrVoid *params)
{
    // param 0: the minimum position (i.e., the width of the previous element)
    // param 1: the maximum width in the current measure
    // param 2: the Doc (unused)
    // param 3: the functor to be redirected to Aligner
    // param 4: the functor to be redirected to Aligner at the end (unused)
    int *min_pos = static_cast<int *>((*params).at(0));
    int *measure_width = static_cast<int *>((*params).at(1));
    Functor *setBoundingBoxXShift = static_cast<Functor *>((*params).at(3));

    // we reset the measure width and the minimum position
    (*measure_width) = 0;
    (*min_pos) = 0;

    m_measureAligner.Process(setBoundingBoxXShift, params);

    return FUNCTOR_CONTINUE;
}

int Measure::SetBoundingBoxXShiftEnd(ArrayPtrVoid *params)
{
    // param 0: the minimum position (i.e., the width of the previous element)
    // param 1: the maximum width in the current measure
    // param 2: the Doc (unused)
    // param 3: the functor to be redirected to Aligner (unused)
    // param 4: the functor to be redirected to Aligner at the end
    int *min_pos = static_cast<int *>((*params).at(0));
    int *measure_width = static_cast<int *>((*params).at(1));
    Functor *setBoundingBoxXShiftEnd = static_cast<Functor *>((*params).at(4));

    // use the measure width as minimum position of the barLine
    (*min_pos) = (*measure_width);

    m_measureAligner.Process(setBoundingBoxXShiftEnd, params);

    return FUNCTOR_CONTINUE;
}

int Measure::IntegrateBoundingBoxGraceXShift(ArrayPtrVoid *params)
{
    // param 0: the functor to be redirected to Aligner
    Functor *integrateBoundingBoxGraceXShift = static_cast<Functor *>((*params).at(0));

    m_measureAligner.Process(integrateBoundingBoxGraceXShift, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::IntegrateBoundingBoxXShift(ArrayPtrVoid *params)
{
    // param 0: the cumulated shift (unused)
    // param 1: the cumulated justifiable shift (unused)
    // param 3: the doc for accessing drawing parameters (unused)
    // param 4: the functor to be redirected to Aligner
    Functor *integrateBoundingBoxShift = static_cast<Functor *>((*params).at(3));

    m_measureAligner.Process(integrateBoundingBoxShift, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::SetAlignmentXPos(ArrayPtrVoid *params)
{
    // param 0: the previous time position (unused)
    // param 1: the previous x rel position (unused)
    // param 2: duration of the longest note (unused)
    // param 3: the doc (unused)
    // param 4: the functor to be redirected to Aligner
    Functor *setAligmnentPosX = static_cast<Functor *>((*params).at(4));

    m_measureAligner.Process(setAligmnentPosX, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::JustifyX(ArrayPtrVoid *params)
{
    // param 0: the justification ratio
    // param 1: the justification ratio for the measure (depends on the margin) (unused)
    // param 2: the non-justifiable margin (unused)
    // param 3: the system full width (without system margins) (unused)
    // param 4: the functor to be redirected to the MeasureAligner
    double *ratio = static_cast<double *>((*params).at(0));
    Functor *justifyX = static_cast<Functor *>((*params).at(4));

    this->m_drawingXRel = ceil((*ratio) * (double)this->m_drawingXRel);

    m_measureAligner.Process(justifyX, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::AlignMeasures(ArrayPtrVoid *params)
{
    // param 0: the cumulated shift
    int *shift = static_cast<int *>((*params).at(0));

    this->m_drawingXRel = (*shift);

    (*shift) += this->GetWidth();

    return FUNCTOR_SIBLINGS;
}

int Measure::ResetDrawing(ArrayPtrVoid *params)
{
    this->m_leftBarLine.Reset();
    this->m_rightBarLine.Reset();
    this->m_timestampAligner.Reset();
    return FUNCTOR_CONTINUE;
};

int Measure::CastOffSystems(ArrayPtrVoid *params)
{
    // param 0: a pointer to the system we are taking the content from
    // param 1: a pointer the page we are adding system to
    // param 2: a pointer to the current system
    // param 3: the cummulated shift (m_drawingXRel of the first measure of the current system)
    // param 4: the system width
    // param 5: the current scoreDef width
    System *contentSystem = static_cast<System *>((*params).at(0));
    Page *page = static_cast<Page *>((*params).at(1));
    System **currentSystem = static_cast<System **>((*params).at(2));
    int *shift = static_cast<int *>((*params).at(3));
    int *systemWidth = static_cast<int *>((*params).at(4));
    int *currentScoreDefWidth = static_cast<int *>((*params).at(5));

    if (((*currentSystem)->GetChildCount() > 0)
        && (this->m_drawingXRel + this->GetWidth() + (*currentScoreDefWidth) - (*shift) > (*systemWidth))) {
        (*currentSystem) = new System();
        page->AddSystem(*currentSystem);
        (*shift) = this->m_drawingXRel;
    }

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    Measure *measure = dynamic_cast<Measure *>(contentSystem->Relinquish(this->GetIdx()));
    assert(measure);
    (*currentSystem)->AddMeasure(measure);

    return FUNCTOR_SIBLINGS;
}

int Measure::SetDrawingXY(ArrayPtrVoid *params)
{
    // param 0: a pointer doc
    // param 1: a pointer to the current system
    // param 2: a pointer to the current measure
    // param 3: a pointer to the current staff (unused)
    // param 4: a pointer to the current layer (unused)
    // param 5: a pointer to the view (unused)
    // param 6: a bool indicating if we are processing layer elements or not
    // param 7: a pointer to the functor for passing it to the timestamps
    Doc *doc = static_cast<Doc *>((*params).at(0));
    System **currentSystem = static_cast<System **>((*params).at(1));
    Measure **currentMeasure = static_cast<Measure **>((*params).at(2));
    bool *processLayerElements = static_cast<bool *>((*params).at(6));
    Functor *setDrawingXY = static_cast<Functor *>((*params).at(7));

    (*currentMeasure) = this;

    // Second pass where we do just process layer elements
    if ((*processLayerElements)) {
        // However, we need to process the timestamps
        m_timestampAligner.Process(setDrawingXY, params);
        return FUNCTOR_CONTINUE;
    }

    // Here we set the appropriate y value to be used for drawing
    // With Raw documents, we use m_drawingXRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_xAbs
    if (this->m_xAbs == VRV_UNSET) {
        assert(doc->GetType() == Raw);
        this->SetDrawingX(this->m_drawingXRel + (*currentSystem)->GetDrawingX());
    }
    else {
        assert(doc->GetType() == Transcription);
        this->SetDrawingX(this->m_xAbs);
    }

    // Process the timestamps - we can do it already since the first pass in only taking care of X position for the
    // LayerElements
    m_timestampAligner.Process(setDrawingXY, params);

    // For avoiding unused variable warning in non debug mode
    doc = NULL;

    return FUNCTOR_CONTINUE;
}

int Measure::FillStaffCurrentTimeSpanningEnd(ArrayPtrVoid *params)
{
    // param 0: the current Syl
    std::vector<Object *> *elements = static_cast<std::vector<Object *> *>((*params).at(0));

    std::vector<Object *>::iterator iter = elements->begin();
    while (iter != elements->end()) {
        TimeSpanningInterface *interface = (*iter)->GetTimeSpanningInterface();
        assert(interface);
        Measure *endParent = dynamic_cast<Measure *>(interface->GetEnd()->GetFirstParent(MEASURE));
        assert(endParent);
        // We have reached the end of the spanning - remove it from the list of running elements
        if (endParent == this) {
            iter = elements->erase(iter);
        }
        else {
            iter++;
        }
    }
    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimeSpanningEnd(ArrayPtrVoid *params)
{
    // param 0: std::vector< Object*>* that holds the current elements to match
    // param 1: bool* fillList for indicating whether the elements have to be stacked or not (unused)
    ArrayOfInterfaceClassIdPairs *elements = static_cast<ArrayOfInterfaceClassIdPairs *>((*params).at(0));

    ArrayOfInterfaceClassIdPairs::iterator iter = elements->begin();
    while (iter != elements->end()) {
        // At the end of the measure (going backward) we remove element for which we do not need to match the end (for
        // now). Eventually, we could consider them, for example if we want to display their spanning or for improved
        // midi output
        if ((iter->second == DIR) || (iter->second == DYNAM)) {
            iter = elements->erase(iter);
        }
        else {
            iter++;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimestampsEnd(ArrayPtrVoid *params)
{
    // param 0: std::vector< Object*>* that holds the current elements to match
    // param 1:  ArrayOfObjectBeatPairs* that holds the tstamp2 elements for attach to the end measure
    std::vector<Object *> *elements = static_cast<std::vector<Object *> *>((*params).at(0));
    ArrayOfObjectBeatPairs *tstamps = static_cast<ArrayOfObjectBeatPairs *>((*params).at(1));

    ArrayOfObjectBeatPairs::iterator iter = tstamps->begin();
    // Loop throught the object/beat pairs and create the TimestampAttr when necessary
    while (iter != tstamps->end()) {
        // -1 means that we have a @tstamp (start) to add to the current measure
        if ((*iter).second.first == -1) {
            TimePointInterface *interface = ((*iter).first)->GetTimePointInterface();
            assert(interface);
            TimestampAttr *timestampAttr = m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetStart(timestampAttr);
            // purge the list of unmatched element is this is a TimeSpanningInterface element
            if ((*iter).first->HasInterface(INTERFACE_TIME_SPANNING)) {
                TimeSpanningInterface *tsInterface = ((*iter).first)->GetTimeSpanningInterface();
                assert(tsInterface);
                if (tsInterface->HasStartAndEnd()) {
                    elements->erase(std::remove(elements->begin(), elements->end(), (*iter).first), elements->end());
                }
            }
            // remove it
            iter = tstamps->erase(iter);
        }
        // 0 means that we have a @tstamp2 (end) to add to the current measure
        else if ((*iter).second.first == 0) {
            TimeSpanningInterface *interface = ((*iter).first)->GetTimeSpanningInterface();
            assert(interface);
            TimestampAttr *timestampAttr = m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetEnd(timestampAttr);
            // We can check if the interface is now fully mapped (start / end) and purge the list of unmatched
            // elements
            if (interface->HasStartAndEnd()) {
                elements->erase(std::remove(elements->begin(), elements->end(), (*iter).first), elements->end());
            }
            iter = tstamps->erase(iter);
        }
        // we have not reached the correct end measure yet
        else {
            (*iter).second.first--;
            iter++;
        }
    }

    return FUNCTOR_CONTINUE;
};

int Measure::GenerateMIDI(ArrayPtrVoid *params)
{
    // param 0: MidiFile*: the MidiFile we are writing to (unused)
    // param 1: int*: the midi track number (unused)
    // param 2: int*: the current time in the measure (incremented by each element)
    // param 3: int*: the current total measure time (incremented by each measure (unused)
    // param 4: std::vector<double>: a stack of maximum duration filled by the functor (unused)
    // param 5: int* the semi tone transposition for the current track (unused)
    double *currentMeasureTime = static_cast<double *>((*params).at(2));

    // Here we need to reset the currentMeasureTime because we are starting a new measure
    (*currentMeasureTime) = 0;

    return FUNCTOR_CONTINUE;
}

int Measure::GenerateMIDIEnd(ArrayPtrVoid *params)
{
    // param 0: MidiFile*: the MidiFile we are writing to (unused)
    // param 1: int*: the midi track number (unused)
    // param 2: int*: the current time in the measure (incremented by each element) (unused)
    // param 3: int*: the current total measure time (incremented by each measure
    // param 4: std::vector<double>: a stack of maximum duration filled by the functor
    // param 5: int* the semi tone transposition for the current track (unused)
    double *totalTime = static_cast<double *>((*params).at(3));
    std::vector<double> *maxValues = static_cast<std::vector<double> *>((*params).at(4));

    // We a to the total time the maximum duration of the measure so if there is no layer, if the layer is not full or
    // if there is an encoding error in the measure, the next one will be properly aligned
    assert(!maxValues->empty());
    (*totalTime) += maxValues->front();
    maxValues->erase(maxValues->begin());

    return FUNCTOR_CONTINUE;
}

int Measure::CalcMaxMeasureDuration(ArrayPtrVoid *params)
{
    // param 0: std::vector<double>: a stack of maximum duration filled by the functor
    // param 1: double: the duration of the current measure (unused)
    std::vector<double> *maxValues = static_cast<std::vector<double> *>((*params).at(0));

    // We just need to add a value to the stack
    maxValues->push_back(0.0);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
