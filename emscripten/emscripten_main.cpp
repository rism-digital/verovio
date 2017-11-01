/////////////////////////////////////////////////////////////////////////////
// Name:        emscripten_main.cpp
// Author:      Rodolfo Zitellini
// Created:     05/11/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "toolkit.h"
#include "vrv.h"

using namespace std;
using namespace vrv;

extern "C" {

/****************************************************************
* Methods exported to use the Toolkit class from js
****************************************************************/
void *vrvToolkit_constructor()
{

    // set the resource path in the js blob
    Resources::SetPath("/data");

    return new Toolkit();
}

void vrvToolkit_destructor(Toolkit *tk)
{
    delete tk;
}

const char *vrvToolkit_getLog(Toolkit *tk)
{
    tk->SetCString(tk->GetLogString());
    return tk->GetCString();
}

const char *vrvToolkit_getVersion(Toolkit *tk)
{
    tk->SetCString(tk->GetVersion());
    return tk->GetCString();
}

int vrvToolkit_getPageCount(Toolkit *tk)
{
    return tk->GetPageCount();
}

int vrvToolkit_getPageWithElement(Toolkit *tk, const char *xmlId)
{
    return tk->GetPageWithElement(xmlId);
}

double vrvToolkit_getTimeForElement(Toolkit *tk, const char *xmlId)
{
    return tk->GetTimeForElement(xmlId);
}

bool vrvToolkit_loadData(Toolkit *tk, const char *data)
{
    tk->ResetLogBuffer();
    return tk->LoadData(data);
}

const char *vrvToolkit_getMEI(Toolkit *tk, int page_no, bool score_based)
{
    tk->SetCString(tk->GetMEI(page_no, score_based));
    return tk->GetCString();
}

const char *vrvToolkit_getHumdrum(Toolkit *tk)
{
    const char* buffer = tk->GetHumdrumBuffer();
    return buffer;
}

const char *vrvToolkit_renderPage(Toolkit *tk, int page_no, const char *c_options)
{
    tk->ResetLogBuffer();
    tk->SetCString(tk->RenderToSvg(page_no, false));
    return tk->GetCString();
}

const char *vrvToolkit_renderToMidi(Toolkit *tk, const char *c_options)
{
    tk->ResetLogBuffer();
    tk->SetCString(tk->RenderToMidi());
    return tk->GetCString();
}

const char *vrvToolkit_renderToTimemap(Toolkit *tk) 
{
    tk->ResetLogBuffer();
    tk->SetCString(tk->RenderToTimemap());
    return tk->GetCString();
}

const char *vrvToolkit_getElementsAtTime(Toolkit *tk, int millisec)
{
    tk->SetCString(tk->GetElementsAtTime(millisec));
    return tk->GetCString();
}

void vrvToolkit_setOptions(Toolkit *tk, const char *options)
{
    if (!tk->ParseOptions(options)) {
        LogError("Could not load JSON options.");
    }
}

void vrvToolkit_redoLayout(Toolkit *tk)
{
    tk->RedoLayout();
}

void vrvToolkit_redoPagePitchPosLayout(Toolkit *tk)
{
    tk->RedoPagePitchPosLayout();
}
    
const char *vrvToolkit_renderData(Toolkit *tk, const char *data, const char *options)
{
    tk->ResetLogBuffer();
    vrvToolkit_setOptions(tk, options);
    vrvToolkit_loadData(tk, data);

    return vrvToolkit_renderPage(tk, 1, options);
}

bool vrvToolkit_edit(Toolkit *tk, const char *editorAction)
{
    if (!tk->Edit(editorAction)) {
        LogError("Could not perform editor action.");
        return false;
    }
    return true;
}

const char *vrvToolkit_getElementAttr(Toolkit *tk, const char *xmlId)
{
    tk->SetCString(tk->GetElementAttr(xmlId));
    return tk->GetCString();
}
}
