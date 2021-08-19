/////////////////////////////////////////////////////////////////////////////
// Name:        metersiggrp.cpp
// Author:      Andriy Makarchuk
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "metersiggrp.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "metersig.h"
#include "vrv.h"

namespace vrv {
//----------------------------------------------------------------------------
// MeterSigGrp
//----------------------------------------------------------------------------

static const ClassRegistrar<MeterSigGrp> s_factory("meterSigGrp", METERSIGGRP);

MeterSigGrp::MeterSigGrp()
    : Object(METERSIGGRP, "metersiggrp-")
    , ObjectListInterface()
    , LinkingInterface()
    , AttBasic()
    , AttLabelled()
    , AttMeterSigGrpLog()
    , AttTyped()
{
    RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    RegisterAttClass(ATT_BASIC);
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_METERSIGGRPLOG);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

MeterSigGrp::~MeterSigGrp() {}

void MeterSigGrp::Reset()
{
    Object::Reset();
    LinkingInterface::Reset();
    ResetBasic();
    ResetLabelled();
    ResetTyped();
    ResetMeterSigGrpLog();
}

bool MeterSigGrp::IsSupportedChild(Object *child)
{
    if (child->Is(METERSIG)) {
        assert(dynamic_cast<MeterSig *>(child));
    }
    else {
        return false;
    }
    return true;
}

void MeterSigGrp::FilterList(ArrayOfObjects *childList)
{
    // We want to keep only MeterSig
    childList->erase(std::remove_if(childList->begin(), childList->end(),
                         [](const Object *object) -> bool { return !object->Is(METERSIG); }),
        childList->end());
}

void MeterSigGrp::AddAlternatingMeasureToVector(Measure *measure)
{
    m_alternatingMeasures.emplace_back(measure);
}

MeterSig *MeterSigGrp::GetSimplifiedMeterSig()
{
    MeterSig *newMeterSig = NULL;
    const ArrayOfObjects *childList = this->GetList(this);
    switch (GetFunc()) {
        // For alternating meterSig group alternate between children sequentially
        case meterSigGrpLog_FUNC_alternating: {
            const int index = m_count % childList->size();
            newMeterSig = vrv_cast<MeterSig *>((childList->at(index))->Clone());
            break;
        }
        // For interchanging meterSig group select the largest signature, but make sure to align unit with the shortest
        case meterSigGrpLog_FUNC_interchanging: {
            // Get element with highest count
            auto it = std::max_element(childList->begin(), childList->end(), [](Object *firstObj, Object *secondObj) {
                MeterSig *firstMeterSig = vrv_cast<MeterSig *>(firstObj);
                MeterSig *secondMeterSig = vrv_cast<MeterSig *>(secondObj);
                const double firstRatio = (double)firstMeterSig->GetTotalCount() / (double)firstMeterSig->GetUnit();
                const double secondRatio = (double)secondMeterSig->GetTotalCount() / (double)secondMeterSig->GetUnit();
                return firstRatio < secondRatio;
            });
            int maxUnit = 0;
            std::for_each(childList->begin(), childList->end(), [&maxUnit](Object *obj) {
                MeterSig *meterSig = vrv_cast<MeterSig *>(obj);
                if (meterSig->GetUnit() > maxUnit) maxUnit = meterSig->GetUnit();
            });

            newMeterSig = vrv_cast<MeterSig *>((*it)->Clone());
            if (newMeterSig->GetUnit() < maxUnit) {
                const int ratio = maxUnit / newMeterSig->GetUnit();
                data_SUMMAND_List currentCount = newMeterSig->GetCount();
                std::transform(currentCount.begin(), currentCount.end(), currentCount.begin(),
                    [&ratio](int elem) -> int { return elem * ratio; });
                newMeterSig->SetCount(currentCount);
                newMeterSig->SetUnit(maxUnit);
            }
            break;
        }
        // For mixed meterSig group we want to accumulate total count of all child meterSig
        // (and keep the highest meter count), since it is what counts for the timestamps
        case meterSigGrpLog_FUNC_mixed: {
            int maxUnit = 0;
            int currentCount = 0;
            for (const auto object : *childList) {
                if (!object->Is(METERSIG)) {
                    LogWarning("Skipping over non-meterSig child of <MeterSigGrp>");
                    continue;
                }
                MeterSig *meterSig = vrv_cast<MeterSig *>(object);
                if (!newMeterSig) {
                    newMeterSig = vrv_cast<MeterSig *>(meterSig->Clone());
                }
                const int currentUnit = meterSig->GetUnit();
                if (maxUnit == 0) maxUnit = currentUnit;
                if (maxUnit == currentUnit) {
                    currentCount += meterSig->GetTotalCount();
                }
                else if (maxUnit > currentUnit) {
                    const int ratio = maxUnit / currentUnit;
                    currentCount += meterSig->GetTotalCount() * ratio;
                }
                else if (maxUnit < currentUnit) {
                    const int ratio = currentUnit / maxUnit;
                    currentCount *= ratio;
                    currentCount += meterSig->GetTotalCount();
                    maxUnit = currentUnit;
                }
            }
            newMeterSig->SetUnit(maxUnit);
            newMeterSig->SetCount({ currentCount });
            break;
        }
        default: {
            // should not be happening
            assert(false);
        }
    }
    return newMeterSig;
}

void MeterSigGrp::SetMeasureBasedCount(Measure *measure)
{
    auto it = std::find(m_alternatingMeasures.begin(), m_alternatingMeasures.end(), measure);
    m_count = int(std::distance(m_alternatingMeasures.begin(), it));
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MeterSigGrp::AlignHorizontally(FunctorParams *)
{
    return this->IsScoreDefElement() ? FUNCTOR_STOP : FUNCTOR_CONTINUE;
}

} // namespace vrv
