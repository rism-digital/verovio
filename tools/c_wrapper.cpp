/////////////////////////////////////////////////////////////////////////////
// Name:        c_wrapper.cpp (previously emscripten_main.cpp)
// Author:      Rodolfo Zitellini
// Created:     05/11/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "c_wrapper.h"
#include "toolkit.h"
#include "vrv.h"

using namespace std;
using namespace vrv;

extern "C" {

/****************************************************************
 * Methods exported to use the Toolkit class
 ****************************************************************/

void *vrvToolkit_constructor()
{
    // set the default resource path
    Resources::SetPath("/data");

    return new Toolkit();
}

void *vrvToolkit_constructorResourcePath(const char *resourcePath)
{
    // set the resource path
    Resources::SetPath(resourcePath);

    return new Toolkit();
}

void vrvToolkit_destructor(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    LogMessage("Deleting toolkit");
    delete tk;
}

bool vrvToolkit_edit(void *tkPtr, const char *editorAction)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    if (!tk->Edit(editorAction)) {
        LogError("Could not perform editor action.");
        return false;
    }
    return true;
}

const char *vrvToolkit_editInfo(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->EditInfo());
    return tk->GetCString();
}

const char *vrvToolkit_getAvailableOptions(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetAvailableOptions());
    return tk->GetCString();
}

const char *vrvToolkit_getDescriptiveFeatures(void *tkPtr, const char *options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetDescriptiveFeatures(options));
    return tk->GetCString();
}

const char *vrvToolkit_getElementAttr(void *tkPtr, const char *xmlId)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetElementAttr(xmlId));
    return tk->GetCString();
}

const char *vrvToolkit_getElementsAtTime(void *tkPtr, int millisec)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetElementsAtTime(millisec));
    return tk->GetCString();
}

const char *vrvToolkit_getExpansionIdsForElement(void *tkPtr, const char *xmlId)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetExpansionIdsForElement(xmlId));
    return tk->GetCString();
}

const char *vrvToolkit_getHumdrum(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    const char *buffer = tk->GetHumdrumBuffer();
    return buffer;
}

const char *vrvToolkit_convertHumdrumToHumdrum(void *tkPtr, const char *humdrumData)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->ConvertHumdrumToHumdrum(humdrumData);
    const char *buffer = tk->GetHumdrumBuffer();
    return buffer;
}

const char *vrvToolkit_convertMEIToHumdrum(void *tkPtr, const char *meiData)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->ConvertMEIToHumdrum(meiData);
    const char *buffer = tk->GetHumdrumBuffer();
    return buffer;
}

const char *vrvToolkit_getLog(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetLog());
    return tk->GetCString();
}

const char *vrvToolkit_getMEI(void *tkPtr, const char *options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetMEI(options));
    return tk->GetCString();
}

const char *vrvToolkit_getMIDIValuesForElement(void *tkPtr, const char *xmlId)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetMIDIValuesForElement(xmlId));
    return tk->GetCString();
}

const char *vrvToolkit_getNotatedIdForElement(void *tkPtr, const char *xmlId)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetNotatedIdForElement(xmlId));
    return tk->GetCString();
}

const char *vrvToolkit_getOptions(void *tkPtr, bool default_values)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetOptions(default_values));
    return tk->GetCString();
}

int vrvToolkit_getPageCount(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    return tk->GetPageCount();
}

int vrvToolkit_getPageWithElement(void *tkPtr, const char *xmlId)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    return tk->GetPageWithElement(xmlId);
}

double vrvToolkit_getTimeForElement(void *tkPtr, const char *xmlId)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    return tk->GetTimeForElement(xmlId);
}

const char *vrvToolkit_getTimesForElement(void *tkPtr, const char *xmlId)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetTimesForElement(xmlId));
    return tk->GetCString();
}

const char *vrvToolkit_getVersion(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetVersion());
    return tk->GetCString();
}

bool vrvToolkit_loadData(void *tkPtr, const char *data)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    return tk->LoadData(data);
}

bool vrvToolkit_loadZipDataBase64(void *tkPtr, const char *data)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    return tk->LoadZipDataBase64(data);
}

bool vrvToolkit_loadZipDataBuffer(void *tkPtr, const unsigned char *data, int length)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    return tk->LoadZipDataBuffer(data, length);
}

const char *vrvToolkit_renderToMIDI(void *tkPtr, const char *c_options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->RenderToMIDI());
    return tk->GetCString();
}

const char *vrvToolkit_renderToPAE(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->RenderToPAE());
    return tk->GetCString();
}

const char *vrvToolkit_renderToSVG(void *tkPtr, int page_no, const char *c_options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->RenderToSVG(page_no, false));
    return tk->GetCString();
}

const char *vrvToolkit_renderToTimemap(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->RenderToTimemap());
    return tk->GetCString();
}

void vrvToolkit_redoLayout(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->RedoLayout();
}

void vrvToolkit_redoPagePitchPosLayout(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->RedoPagePitchPosLayout();
}

const char *vrvToolkit_renderData(void *tkPtr, const char *data, const char *options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    vrvToolkit_setOptions(tk, options);
    vrvToolkit_loadData(tk, data);

    return vrvToolkit_renderToSVG(tk, 1, options);
}

void vrvToolkit_resetXmlIdSeed(void *tkPtr, int seed)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->ResetXmlIdSeed(seed);
}

void vrvToolkit_setOptions(void *tkPtr, const char *options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    if (!tk->SetOptions(options)) {
        LogError("Could not load JSON options.");
    }
}

} // extern C
