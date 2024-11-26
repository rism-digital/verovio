/////////////////////////////////////////////////////////////////////////////
// Name:        iocmme.h
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOCMME_H__
#define __VRV_IOCMME_H__

#include <string>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------

#include "iobase.h"
#include "pugixml.hpp"
#include "vrvdef.h"

namespace vrv {

class Chord;
class Clef;
class KeySig;
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
        int proportNum = 1;
        int proportDen = 1;
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
    void CreateMetadata(pugi::xml_node metadataNode);
    void CreateSection(pugi::xml_node musicSectionNode);
    void CreateStaff(pugi::xml_node voiceNode);
    void CreateApp(pugi::xml_node appNode);
    void CreateLemOrRdg(pugi::xml_node lemOrRdgNode, bool isFirst);

    void ReadEvents(pugi::xml_node eventsNode);
    void ReadEditorialCommentary(pugi::xml_node evenNode, Object *object);

    void CreateAccid(pugi::xml_node accidNode);
    void CreateColorChange(pugi::xml_node colorChangeNode);
    void CreateBarline(pugi::xml_node barlineNode);
    void CreateBreak(pugi::xml_node breakNode);
    void CreateChord(pugi::xml_node chordNode);
    void CreateClef(pugi::xml_node clefNode);
    void CreateCustos(pugi::xml_node custosNode);
    void CreateDot(pugi::xml_node dotNode);
    void CreateEllipsis();
    void CreateKeySig(pugi::xml_node keyNode);
    void CreateLacuna(pugi::xml_node lacunaNode);
    void CreateMensuration(pugi::xml_node mensurationNode);
    void CreateOriginalText(pugi::xml_node originalTextNode);
    void CreateProport(pugi::xml_node proportNode);
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

    /**
     * Post-process the data to adjust the type of tempo change proport.
     */
    void PostProcessProport();

public:
    //
private:
    /** The current score (only one) */
    Score *m_score;
    /** The current un-measured measure acting a a MEI section */
    Measure *m_currentSection;
    /** The current layer (or container) to which the layer elements have to be added */
    Object *m_currentContainer;
    /** The current key signature to which extra flats might be added */
    KeySig *m_currentSignature;
    /** The current note */
    Note *m_currentNote;
    /** The syllable is not the first */
    bool m_isInSyllable;
    /** The mensural infos for all voices */
    std::vector<cmme::mensInfo> m_mensInfos;
    /** The mensural info for the current voice */
    cmme::mensInfo *m_mensInfo;
    /** The current color (if not black) */
    std::string m_currentColor;

    /** The number of voices as given in the general data */
    int m_numVoices;
    /** The name of the voices - if any */
    std::vector<std::string> m_voices;
    /** A flag indicating we had a tempo change */
    bool m_activeTempoChange;
};

} // namespace vrv

#endif
