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

void enableLog(bool value)
{
    EnableLog(value);
}

void enableLogToBuffer(bool value)
{
    EnableLogToBuffer(value);
}

/****************************************************************
 * Methods exported to use the Toolkit class
 ****************************************************************/

void *vrvToolkit_constructor()
{
    SetDefaultResourcePath("/data");

    return new Toolkit();
}

void *vrvToolkit_constructorResourcePath(const char *resourcePath)
{
    SetDefaultResourcePath(resourcePath);

    return new Toolkit();
}

void vrvToolkit_destructor(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    LogInfo("Deleting toolkit");
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

const char *vrvToolkit_getDefaultOptions(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetDefaultOptions());
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

const char *vrvToolkit_convertHumdrumToMIDI(void *tkPtr, const char *humdrumData)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->ConvertHumdrumToMIDI(humdrumData));
    return tk->GetCString();
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

const char *vrvToolkit_getOptions(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetOptions());
    return tk->GetCString();
}

const char *vrvToolkit_getOptionUsageString(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->GetOptionUsageString());
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

void vrvToolkit_redoLayout(void *tkPtr, const char *c_options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->RedoLayout(c_options);
}

void vrvToolkit_redoPagePitchPosLayout(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->RedoPagePitchPosLayout();
}

const char *vrvToolkit_renderData(void *tkPtr, const char *data, const char *options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->RenderData(data, options));
    return tk->GetCString();
}

const char *vrvToolkit_renderToExpansionMap(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->RenderToExpansionMap());
    return tk->GetCString();
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

const char *vrvToolkit_renderToSVG(void *tkPtr, int page_no, bool xmlDeclaration)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->RenderToSVG(page_no, xmlDeclaration));
    return tk->GetCString();
}

const char *vrvToolkit_renderToTimemap(void *tkPtr, const char *c_options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->RenderToTimemap(c_options));
    return tk->GetCString();
}

void vrvToolkit_resetOptions(void *tkPtr)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->ResetOptions();
}

void vrvToolkit_resetXmlIdSeed(void *tkPtr, int seed)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->ResetXmlIdSeed(seed);
}

bool vrvToolkit_select(void *tkPtr, const char *selection)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    return tk->Select(selection);
}

bool vrvToolkit_setOptions(void *tkPtr, const char *options)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    return tk->SetOptions(options);
}

const char *vrvToolkit_validatePAE(void *tkPtr, const char *data)
{
    Toolkit *tk = static_cast<Toolkit *>(tkPtr);
    tk->SetCString(tk->ValidatePAE(data));
    return tk->GetCString();
}

} // extern C
