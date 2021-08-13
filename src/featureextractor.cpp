/////////////////////////////////////////////////////////////////////////////
// Name:        featureextractor.cpp
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "featureextractor.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "chord.h"
#include "doc.h"
#include "gracegrp.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "rest.h"
#include "space.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// FeatureExtractor
//----------------------------------------------------------------------------

FeatureExtractor::FeatureExtractor() {}

FeatureExtractor::~FeatureExtractor() {}

void FeatureExtractor::Reset()
{
    m_previousNote = NULL;
}

void FeatureExtractor::Extract(Object *object, GenerateFeaturesParams *params)
{
    
}

void FeatureExtractor::ToJson(std::string &output)
{
    output = "{}";
}

} // namespace vrv
