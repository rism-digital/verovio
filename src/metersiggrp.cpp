/////////////////////////////////////////////////////////////////////////////
// Name:        metersiggrp.cpp
// Author:      Andriy Makarchuk
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "metersiggrp.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "metersig.h"
#include "vrv.h"

namespace vrv {
//----------------------------------------------------------------------------
// MeterSigGrp
//----------------------------------------------------------------------------

static const ClassRegistrar<MeterSigGrp> s_factory("meterSigGrp", METERSIGGRP);

MeterSigGrp::MeterSigGrp() : LayerElement(METERSIGGRP), ObjectListInterface(), AttBasic(), AttMeterSigGrpLog()
{
    this->RegisterAttClass(ATT_BASIC);
    this->RegisterAttClass(ATT_METERSIGGRPLOG);

    this->Reset();
}

MeterSigGrp::~MeterSigGrp() {}

void MeterSigGrp::Reset()
{
    LayerElement::Reset();
    this->ResetBasic();
    this->ResetMeterSigGrpLog();
}

bool MeterSigGrp::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ METERSIG };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

void MeterSigGrp::FilterList(ListOfConstObjects &childList) const
{
    // We want to keep only MeterSig
    childList.erase(std::remove_if(childList.begin(), childList.end(),
                        [](const Object *object) -> bool { return !object->Is(METERSIG); }),
        childList.end());
}

void MeterSigGrp::AddAlternatingMeasureToVector(const Measure *measure)
{
    m_alternatingMeasures.emplace_back(measure);
}

MeterSig *MeterSigGrp::GetSimplifiedMeterSig() const
{
    MeterSig *newMeterSig = NULL;
    const ListOfConstObjects &childList = this->GetList();
    switch (this->GetFunc()) {
        // For alternating meterSig group alternate between children sequentially
        case meterSigGrpLog_FUNC_alternating: {
            const int index = m_count % childList.size();
            auto iter = std::next(childList.begin(), index);
            newMeterSig = vrv_cast<MeterSig *>((*iter)->Clone());
            break;
        }
        // For interchanging meterSig group select the largest signature, but make sure to align unit with the shortest
        case meterSigGrpLog_FUNC_interchanging: {
            // Get element with highest count
            auto it = std::max_element(
                childList.begin(), childList.end(), [](const Object *firstObj, const Object *secondObj) {
                    const MeterSig *firstMeterSig = vrv_cast<const MeterSig *>(firstObj);
                    const MeterSig *secondMeterSig = vrv_cast<const MeterSig *>(secondObj);
                    const double firstRatio = (double)firstMeterSig->GetTotalCount() / (double)firstMeterSig->GetUnit();
                    const double secondRatio
                        = (double)secondMeterSig->GetTotalCount() / (double)secondMeterSig->GetUnit();
                    return firstRatio < secondRatio;
                });
            int maxUnit = 0;
            std::for_each(childList.begin(), childList.end(), [&maxUnit](const Object *obj) {
                const MeterSig *meterSig = vrv_cast<const MeterSig *>(obj);
                if (meterSig->GetUnit() > maxUnit) maxUnit = meterSig->GetUnit();
            });

            newMeterSig = vrv_cast<MeterSig *>((*it)->Clone());
            if (newMeterSig->GetUnit() < maxUnit) {
                const int ratio = maxUnit / newMeterSig->GetUnit();
                auto [currentCount, sign] = newMeterSig->GetCount();
                std::transform(currentCount.begin(), currentCount.end(), currentCount.begin(),
                    [&ratio](int elem) -> int { return elem * ratio; });
                newMeterSig->SetCount({ currentCount, sign });
                newMeterSig->SetUnit(maxUnit);
            }
            break;
        }
        // For mixed meterSig group we want to accumulate total count of all child meterSig
        // (and keep the highest meter count), since it is what counts for the timestamps
        case meterSigGrpLog_FUNC_mixed: {
            int maxUnit = 0;
            int currentCount = 0;
            for (const auto object : childList) {
                if (!object->Is(METERSIG)) {
                    LogWarning("Skipping over non-meterSig child of <MeterSigGrp>");
                    continue;
                }
                const MeterSig *meterSig = vrv_cast<const MeterSig *>(object);
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
                else {
                    const int ratio = currentUnit / maxUnit;
                    currentCount *= ratio;
                    currentCount += meterSig->GetTotalCount();
                    maxUnit = currentUnit;
                }
            }
            if (newMeterSig) {
                newMeterSig->SetUnit(maxUnit);
                newMeterSig->SetCount({ { currentCount }, MeterCountSign::None });
            }
            break;
        }
        default: {
            // should not be happening
            assert(false);
        }
    }
    return newMeterSig;
}

void MeterSigGrp::SetMeasureBasedCount(const Measure *measure)
{
    auto it = std::find(m_alternatingMeasures.begin(), m_alternatingMeasures.end(), measure);
    m_count = int(std::distance(m_alternatingMeasures.begin(), it));
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode MeterSigGrp::Accept(Functor &functor)
{
    return functor.VisitMeterSigGrp(this);
}

FunctorCode MeterSigGrp::Accept(ConstFunctor &functor) const
{
    return functor.VisitMeterSigGrp(this);
}

FunctorCode MeterSigGrp::AcceptEnd(Functor &functor)
{
    return functor.VisitMeterSigGrpEnd(this);
}

FunctorCode MeterSigGrp::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMeterSigGrpEnd(this);
}

} // namespace vrv
