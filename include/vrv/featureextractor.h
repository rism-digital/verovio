/////////////////////////////////////////////////////////////////////////////
// Name:        featureextractor.h
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FEATURE_EXTRACTOR_H__
#define __VRV_FEATURE_EXTRACTOR_H__

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "options.h"

//----------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

class FeatureExtractor {

public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    FeatureExtractor(const std::string &options);
    virtual ~FeatureExtractor();
    ///@}

    /*
     * Clear the content of the feature extractor.
     */
    virtual void Reset();

    /**
     * Extract a feature for the object
     */
    void Extract(Object *object, GenerateFeaturesParams *params);

    /**
     * Write the current content of the extractor to a JSON string
     */
    void ToJson(std::string &output);

private:
    //
public:
    Note *m_previousNote;
    jsonxx::Array m_pitches;
    jsonxx::Array m_pitchesIds;
    jsonxx::Array m_intervals;
    jsonxx::Array m_intervalsIds;

private:
};

} // namespace vrv

#endif
