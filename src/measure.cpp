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

Measure::Measure(bool measureMusic, int logMeasureNb)
    : DocObject("measure-"), AttCommon(), AttMeasureLog(), AttPointing()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_MEASURELOG);
    RegisterAttClass(ATT_POINTING);

    m_measuredMusic = measureMusic;
    // We set parent to it because we want to access the parent doc from the aligners
    m_measureAligner.SetParent(this);
    // Idem for timestamps
    m_timestampAligner.SetParent(this);

    Reset();
}

Measure::~Measure()
{
}

void Measure::Reset()
{
    DocObject::Reset();
    ResetCommon();
    ResetMeasureLog();
    ResetPointing();

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

int Measure::GetXRel()
{
    if (m_measureAligner.GetLeftAlignment()) {
        return m_measureAligner.GetLeftAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetLeftBarLineX()
{
    if (m_measureAligner.GetLeftAlignment()) {
        return m_measureAligner.GetLeftAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetRightBarLineX()
{
    if (m_measureAligner.GetRightAlignment()) {
        return m_measureAligner.GetRightAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetWidth()
{
    if (m_measureAligner.GetRightAlignment()) {
        return GetRightBarLineX() + m_measureAligner.GetRightAlignment()->GetMaxWidth();
    }
    return 0;
}

//----------------------------------------------------------------------------
// Measure functor methods
//----------------------------------------------------------------------------

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
    MeasureAligner **measureAligner = static_cast<MeasureAligner **>((*params).at(0));

    // clear the content of the measureAligner
    m_measureAligner.Reset();

    // here we transfer the @left and @right values to the barLine objects
    this->SetLeftBarLineType(this->GetLeft());
    this->SetRightBarLineType(this->GetRight());

    // point to it
    (*measureAligner) = &m_measureAligner;

    if (m_leftBarLine.GetForm() != BARRENDITION_NONE) {
        m_leftBarLine.SetAlignment(m_measureAligner.GetLeftAlignment());
    }

    if (m_rightBarLine.GetForm() != BARRENDITION_NONE) {
        m_rightBarLine.SetAlignment(m_measureAligner.GetRightAlignment());
    }

    // LogDebug("\n ***** Align measure %d", this->GetN());

    assert(*measureAligner);

    return FUNCTOR_CONTINUE;
}

int Measure::AlignVertically(ArrayPtrVoid *params)
{
    // param 0: the systemAligner (unused)
    // param 1: the staffNb
    int *staffNb = static_cast<int *>((*params).at(1));

    // we also need to reset the staffNb
    (*staffNb) = 0;

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
    // param 2: the minimum measure width (unused)
    // param 3: the doc for accessing drawing parameters (unused)
    // param 4: the functor to be redirected to Aligner
    Functor *integrateBoundingBoxShift = static_cast<Functor *>((*params).at(4));

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
    // param 2: the non justifiable margin (unused)
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

    assert(m_measureAligner.GetRightAlignment());

    (*shift) += m_measureAligner.GetRightAlignment()->GetXRel();

    // We also need to take into account the measure end (right) barLine with here
    if (GetRightBarLineType() != BARRENDITION_NONE) {
        // shift the next measure of the total with
        (*shift) += GetRightBarLine()->GetAlignment()->GetMaxWidth();
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::ResetDrawing(ArrayPtrVoid *params)
{
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
        ;
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
    Doc *doc = static_cast<Doc *>((*params).at(0));
    System **currentSystem = static_cast<System **>((*params).at(1));
    Measure **currentMeasure = static_cast<Measure **>((*params).at(2));
    bool *processLayerElements = static_cast<bool *>((*params).at(6));

    (*currentMeasure) = this;

    // Second pass where we do just process layer elements
    if ((*processLayerElements)) return FUNCTOR_CONTINUE;

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

    // For avoiding unused variable warning in non debug mode
    doc = NULL;

    return FUNCTOR_CONTINUE;
}

int Measure::FillStaffCurrentTimeSpanningEnd(ArrayPtrVoid *params)
{
    // param 0: the current Syl
    std::vector<DocObject *> *elements = static_cast<std::vector<DocObject *> *>((*params).at(0));

    std::vector<DocObject *>::iterator iter = elements->begin();
    while (iter != elements->end()) {
        TimeSpanningInterface *interface = dynamic_cast<TimeSpanningInterface *>(*iter);
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

int Measure::PrepareTimestampsEnd(ArrayPtrVoid *params)
{
    // param 0: std::vector<DocObject*>* that holds the current elements to match
    // param 1: ArrayOfDocObjectBeatPairs* that holds the tstamp2 elements for attach to the end measure
    std::vector<DocObject *> *elements = static_cast<std::vector<DocObject *> *>((*params).at(0));
    ArrayOfDocObjectBeatPairs *tstamps = static_cast<ArrayOfDocObjectBeatPairs *>((*params).at(1));

    ArrayOfDocObjectBeatPairs::iterator iter = tstamps->begin();
    // Loop throught the object/beat pairs and create the TimestampAttr when necessary
    while (iter != tstamps->end()) {
        // -1 means that we have a @tstamp (start) to add to the current measure
        if ((*iter).second.first == -1) {
            TimePointInterface *interface = dynamic_cast<TimePointInterface *>((*iter).first);
            assert(interface);
            TimestampAttr *timestampAttr = m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetStart(timestampAttr);
            iter = tstamps->erase(iter);
        }
        // 0 means that we have a @tstamp2 (end) to add to the current measure
        else if ((*iter).second.first == 0) {
            TimeSpanningInterface *interface = dynamic_cast<TimeSpanningInterface *>((*iter).first);
            assert(interface);
            TimestampAttr *timestampAttr = m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetEnd(timestampAttr);
            // We can check if the interface is now fully mapped (start / end)
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

    LogMessage("%d", tstamps->size());

    return FUNCTOR_CONTINUE;
};

} // namespace vrv
