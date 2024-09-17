/////////////////////////////////////////////////////////////////////////////
// Name:        iocmme.h
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOCMME_H__
#define __VRV_IOCMME_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "iobase.h"
#include "pugixml.hpp"
#include "vrvdef.h"

namespace vrv {

class Clef;
class Layer;
class Measure;
class Score;

namespace cmme {

    struct mensInfo {
        int prolatio = 2;
        int tempus = 2;
        int modusminor = 2;
        int modusmaior = 2;
    };

} // namespace cmme

//----------------------------------------------------------------------------
// CmmeInput
//----------------------------------------------------------------------------

class CmmeInput : public Input {
public:
    // constructors and destructors
    CmmeInput(Doc *doc);
    virtual ~CmmeInput();

    bool Import(const std::string &cmme) override;

private:
    void MakeSection(pugi::xml_node musicSectionNode);
    void MakeStaff(pugi::xml_node voiceNode);

    void MakeClef(pugi::xml_node clefNode);
    void MakeDot(pugi::xml_node dotNode);
    void MakeMensuration(pugi::xml_node mensurationNode);
    void MakeOriginalText(pugi::xml_node originalTextNode);
    void MakeNote(pugi::xml_node noteNode);
    void MakeRest(pugi::xml_node restNode);

    data_DURATION ReadDuration(pugi::xml_node durationNode, int &num, int &numbase) const;
    bool IsClef(pugi::xml_node clefNode) const;

    /**
     * Helper methods for accessing and converting text in elements
     */
    std::string AsString(const pugi::xml_node node) const;
    std::string ChildAsString(const pugi::xml_node node, const std::string &child) const;
    int AsInt(const pugi::xml_node node) const;
    int ChildAsInt(const pugi::xml_node node, const std::string &child) const;

public:
    //
private:
    //
    Score *m_score;
    Measure *m_currentSection;
    Layer *m_currentLayer;
    std::vector<cmme::mensInfo> m_mensInfos;
    cmme::mensInfo *m_mensInfo;

    int m_numVoices;
    std::vector<std::string> m_voices;
};

} // namespace vrv

#endif
