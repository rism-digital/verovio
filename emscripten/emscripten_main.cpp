/////////////////////////////////////////////////////////////////////////////
// Name:        emscripten_main.cpp
// Author:      Rodolfo Zitellini
// Created:     05/11/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "toolkit.h"
#include "toolkitmanager.h"

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

    return new ToolkitManager();
}

void vrvToolkit_destructor(ToolkitManager *tkm)
{
    delete tkm;
}

bool vrvToolkit_edit(ToolkitManager *tkm, const char *editorAction)
{
    if (!tkm->GetInstance()->Edit(editorAction)) {
        LogError("Could not perform editor action.");
        return false;
    }
    return true;
}
   
const char *vrvToolkit_getElementsAtTime(ToolkitManager *tkm, int millisec)
{
    tkm->SetCString(tkm->GetInstance()->GetElementsAtTime(millisec));
    return tkm->GetCString();
}

const char *vrvToolkit_getElementAttr(ToolkitManager *tkm, const char *xmlId)
{
    tkm->SetCString(tkm->GetInstance()->GetElementAttr(xmlId));
    return tkm->GetCString();
}
  
const char *vrvToolkit_getHumdrum(ToolkitManager *tkm)
{
    const char* buffer = tkm->GetInstance()->GetHumdrumBuffer();
    return buffer;
}

const char *vrvToolkit_getLog(ToolkitManager *tkm)
{
    tkm->SetCString(tkm->GetInstance()->GetLog());
    return tkm->GetCString();
}

const char *vrvToolkit_getMEI(ToolkitManager *tkm, int page_no, bool score_based)
{
    tkm->SetCString(tkm->GetInstance()->GetMEI(page_no, score_based));
    return tkm->GetCString();
}
    
int vrvToolkit_getPageCount(ToolkitManager *tkm)
{
    return tkm->GetInstance()->GetPageCount();
}

int vrvToolkit_getPageWithElement(ToolkitManager *tkm, const char *xmlId)
{
    return tkm->GetInstance()->GetPageWithElement(xmlId);
}

double vrvToolkit_getTimeForElement(ToolkitManager *tkm, const char *xmlId)
{
    return tkm->GetInstance()->GetTimeForElement(xmlId);
}

const char *vrvToolkit_getVersion(ToolkitManager *tkm)
{
    tkm->SetCString(tkm->GetInstance()->GetVersion());
    return tkm->GetCString();
}
 
bool vrvToolkit_loadData(ToolkitManager *tkm, const char *data)
{
    tkm->GetInstance()->ResetLogBuffer();
    return tkm->GetInstance()->LoadData(data);
}

const char *vrvToolkit_renderPage(ToolkitManager *tkm, int page_no, const char *c_options)
{
    tkm->GetInstance()->ResetLogBuffer();
    tkm->SetCString(tkm->GetInstance()->RenderToSvg(page_no, false));
    return tkm->GetCString();
}

const char *vrvToolkit_renderToMidi(ToolkitManager *tkm, const char *c_options)
{
    tkm->GetInstance()->ResetLogBuffer();
    tkm->SetCString(tkm->GetInstance()->RenderToMidi());
    return tkm->GetCString();
}

const char *vrvToolkit_renderToTimemap(ToolkitManager *tkm)
{
    tkm->GetInstance()->ResetLogBuffer();
    tkm->SetCString(tkm->GetInstance()->RenderToTimemap());
    return tkm->GetCString();
}

void vrvToolkit_redoLayout(ToolkitManager *tkm)
{
    tkm->GetInstance()->RedoLayout();
}

void vrvToolkit_redoPagePitchPosLayout(ToolkitManager *tkm)
{
    tkm->GetInstance()->RedoPagePitchPosLayout();
}
    
const char *vrvToolkit_renderData(ToolkitManager *tkm, const char *data, const char *options)
{
    tkm->GetInstance()->ResetLogBuffer();
    vrvToolkit_setOptions(tkm, options);
    vrvToolkit_loadData(tkm, data);

    return vrvToolkit_renderPage(tkm, 1, options);
}

void vrvToolkit_setOptions(ToolkitManager *tkm, const char *options)
{
    if (!tkm->GetInstance()->ParseOptions(options)) {
        LogError("Could not load JSON options.");
    }
}

} // extern C
