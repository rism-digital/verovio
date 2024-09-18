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
class Note;
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
    void CreateSection(pugi::xml_node musicSectionNode);
    void CreateStaff(pugi::xml_node voiceNode);

    void CreateClef(pugi::xml_node clefNode);
    void CreateDot(pugi::xml_node dotNode);
    void CreateKeySig(pugi::xml_node keyNode);
    void CreateMensuration(pugi::xml_node mensurationNode);
    void CreateOriginalText(pugi::xml_node originalTextNode);
    void CreateNote(pugi::xml_node noteNode);
    void CreateRest(pugi::xml_node restNode);

    void CreateVerse(pugi::xml_node verseNode);

    data_DURATION ReadDuration(pugi::xml_node durationNode, int &num, int &numbase) const;
    bool IsClef(pugi::xml_node clefNode) const;

    /**
     * Helper methods for accessing and converting text in elements.
     * Return "" or VRV_UNSET if the node is NULL or the node or the text not found
     */
    ///@{
    std::string AsString(const pugi::xml_node node) const;
    std::string ChildAsString(const pugi::xml_node node, const std::string &child) const;
    int AsInt(const pugi::xml_node node) const;
    int ChildAsInt(const pugi::xml_node node, const std::string &child) const;
    ///@}

public:
    //
private:
    /** The current score (only one) */
    Score *m_score;
    /** The current un-measured measure acting a a MEI section */
    Measure *m_currentSection;
    /** The current layer (or container) to which the layer elements have to be added */
    Layer *m_currentLayer;
    /** The current note */
    Note *m_currentNote;
    /** The syllable is not the first */
    bool m_isInSyllable;
    /** The mensural infos for all voices */
    std::vector<cmme::mensInfo> m_mensInfos;
    /** The mensural info for the current voice */
    cmme::mensInfo *m_mensInfo;

    /** The number of voices as given in the general data */
    int m_numVoices;
    /** The name of the voices - if any */
    std::vector<std::string> m_voices;
};

} // namespace vrv

#endif
