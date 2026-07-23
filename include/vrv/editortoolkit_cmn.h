/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_cmn.h
// Author:      Juliette Regimbal
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_CMN_H__
#define __VRV_EDITOR_TOOLKIT_CMN_H__

#include <cmath>
#include <deque>
#include <string>
#include <utility>

//--------------------------------------------------------------------------------

#include "doc.h"
#include "editortoolkit_shared.h"
#include "view.h"

#include "jsonxx.h"

namespace vrv {

class EditorTreeObject;

//--------------------------------------------------------------------------------
// EditorToolkitCMN
//--------------------------------------------------------------------------------

class EditorToolkitCMN : public EditorToolkitShared {
public:
    EditorToolkitCMN(Doc *doc, View *view);
    virtual ~EditorToolkitCMN();

public:
#ifndef NO_EDIT_SUPPORT
    bool ParseEditorCMNAction(const jsonxx::Object &json_editorAction);

protected:
    enum CursorInsertType : int8_t {
        CURSOR_INSERT_NONE = 0,
        CURSOR_INSERT_REST,
        CURSOR_INSERT_TIE,
        CURSOR_INSERT_COPY
    };

    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool ParseInsertCursorByDurAction(const jsonxx::Object &param, data_DURATION &dur, int &dots);
    bool ParseInsertCursorByPitchAction(
        const jsonxx::Object &param, data_PITCHNAME &pname, int &oct, data_ACCIDENTAL_WRITTEN &accid, int &midi);
    bool ParseInsertCursorByTypeAction(const jsonxx::Object &param, CursorInsertType &insertType);
    bool ParseInsertMeasureAction(const jsonxx::Object &param, std::string &elementId, int &number, bool &insertBefore);
    bool ParseInsertNoteAction(const jsonxx::Object &param, std::string &elementId, data_PITCHNAME &pname, int &oct,
        data_ACCIDENTAL_WRITTEN &accid, data_ACCIDENTAL_GESTURAL &accidGes, data_DURATION &dur, int &dots,
        bool &chordMode);
    bool ParseInsertRestAction(const jsonxx::Object &param, std::string &elementId, data_DURATION &dur, int &dots);

    bool InsertCursorByDur(data_DURATION dur, int dots);
    bool InsertCursorByPitch(data_PITCHNAME pname, int oct, data_ACCIDENTAL_WRITTEN accid, int midi);
    bool InsertCursorByType(CursorInsertType insertType);
    bool InsertMeasure(std::string &elementId, int number, bool insertBefore);
    bool InsertNote(const std::string &elementId, data_PITCHNAME pname, int oct, data_ACCIDENTAL_WRITTEN accid,
        data_ACCIDENTAL_GESTURAL accidGes, data_DURATION dur, int dots, bool chordMode);
    bool InsertRest(const std::string &elementId, data_DURATION dur, int dots);

private:
    bool InsertNoteInChordMode(const std::string &elementId, data_PITCHNAME pname, int oct,
        data_ACCIDENTAL_WRITTEN accid, data_ACCIDENTAL_GESTURAL accidGes);
    void SetNoteAttributes(
        Note *note, data_PITCHNAME pname, int oct, data_ACCIDENTAL_WRITTEN accid, data_ACCIDENTAL_GESTURAL accidGes);
    void AutoBeam(LayerElement *noteOrRest);
    bool CopyCursorPosition(data_DURATION dur, int dots, bool tie);
    std::pair<Object *, Object *> GetTargetContainerFor(Object *target);
    void TieElements(const Object *start, const Object *end);

public:
    //
protected:
    //
#endif /* NO_EDIT_SUPPORT */
};

} // namespace vrv

#endif
