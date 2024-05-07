/////////////////////////////////////////////////////////////////////////////
// Name:        featureextractor.h
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FEATURE_EXTRACTOR_H__
#define __VRV_FEATURE_EXTRACTOR_H__

//----------------------------------------------------------------------------

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
    void Extract(const Object *object);

    /**
     * Write the current content of the extractor to a JSON string
     */
    void ToJson(std::string &output);

private:
    //
public:
    /**
     * A list of previous notes for interval calculation.
     * Usually one note, but sometimes more than one with tied notes.
     */
    std::list<const Note *> m_previousNotes;

    jsonxx::Array m_pitchesChromatic;
    jsonxx::Array m_pitchesChromaticWithDuration;
    jsonxx::Array m_pitchesDiatonic;
    jsonxx::Array m_pitchesIds;

    jsonxx::Array m_intervalsChromatic;
    jsonxx::Array m_intervalsDiatonic;
    jsonxx::Array m_intervalGrossContour;
    jsonxx::Array m_intervalRefinedContour;
    jsonxx::Array m_intervalsIds;

private:
};

} // namespace vrv

#endif
