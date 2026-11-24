/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxmloutput.h
// Author:      Laurent Pugin (LLM assisted coding)
// Created:     22/11/2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOMUSXMLOUTPUT_H__
#define __VRV_IOMUSXMLOUTPUT_H__

#include "iobase.h"

namespace vrv {

/** Write a single common-notation score as partwise MusicXML. */
class MusicXmlOutput : public Output {
public:
    explicit MusicXmlOutput(Doc *doc);

    std::string Export() override;
};

} // namespace vrv

#endif
