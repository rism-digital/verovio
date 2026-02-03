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

protected:
    //
public:
    //
protected:
    //
};

} // namespace vrv

#endif
