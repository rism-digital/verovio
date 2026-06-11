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
    bool ParseEditorCMNAction(const jsonxx::Object &json_editorAction);

protected:
#ifndef NO_EDIT_SUPPORT
    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool ParseInsertMeasureAction(const jsonxx::Object &param, std::string &targetId, int &number, bool &insertBefore);
    bool ParseInsertNoteAction(const jsonxx::Object &param, std::string &targetId, data_PITCHNAME &pname, int &oct,
        data_DURATION &dur, bool &chordMode);

    bool InsertMeasure(std::string &targetId, int number, bool insertBefore);
    bool InsertNote(const std::string &targetId, data_PITCHNAME pname, int oct, data_DURATION dur, bool chordMode);
    bool InsertNoteInChordMode(const std::string &targetId, data_PITCHNAME pname, int oct);

public:
    //
protected:
    //
#endif /* NO_EDIT_SUPPORT */
};

} // namespace vrv

#endif
