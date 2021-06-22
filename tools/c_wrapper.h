/////////////////////////////////////////////////////////////////////////////
// Name:        c_wrapper.h
// Author:      Laurent Pugin
// Created:     19/03/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
 * Methods exported a functions to use the Toolkit class
 ****************************************************************/

void *vrvToolkit_constructor();
void *vrvToolkit_constructorResourcePath(const char * resourcePath);
<<<<<<< HEAD
void vrvToolkit_destructor(Toolkit *tk);
bool vrvToolkit_edit(Toolkit *tk, const char *editorAction);
const char *vrvToolkit_getAvailableOptions(Toolkit *tk);
const char *vrvToolkit_getElementAttr(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getElementsAtTime(Toolkit *tk, int millisec);
const char *vrvToolkit_getExpansionIdsForElement(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getHumdrum(Toolkit *tk);
const char *vrvToolkit_convertHumdrumToHumdrum(Toolkit *tk, const char *humdrumData);
const char *vrvToolkit_convertMEIToHumdrum(Toolkit *tk, const char *meiData);
const char *vrvToolkit_getLog(Toolkit *tk);
const char *vrvToolkit_getMEI(Toolkit *tk, const char *options);
const char *vrvToolkit_getMIDIValuesForElement(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getNotatedIdForElement(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getOptions(Toolkit *tk, bool default_values);
int vrvToolkit_getPageCount(Toolkit *tk);
int vrvToolkit_getPageWithElement(Toolkit *tk, const char *xmlId);
double vrvToolkit_getTimeForElement(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getVersion(Toolkit *tk);
bool vrvToolkit_loadData(Toolkit *tk, const char *data);
bool vrvToolkit_loadZipDataBase64(Toolkit *tk, const char *data);
bool vrvToolkit_loadZipDataBuffer(Toolkit *tk, const unsigned char *data, int length);
const char *vrvToolkit_renderToMIDI(Toolkit *tk, const char *c_options);
const char *vrvToolkit_renderToPAE(Toolkit *tk);
const char *vrvToolkit_renderToSVG(Toolkit *tk, int page_no, const char *c_options);
const char *vrvToolkit_renderToTimemap(Toolkit *tk);
void vrvToolkit_redoLayout(Toolkit *tk);
void vrvToolkit_redoPagePitchPosLayout(Toolkit *tk);
const char *vrvToolkit_renderData(Toolkit *tk, const char *data, const char *options);
void vrvToolkit_setOptions(Toolkit *tk, const char *options);
=======
void vrvToolkit_destructor(void *tkPtr);
bool vrvToolkit_edit(void *tkPtr, const char *editorAction);
const char *vrvToolkit_getAvailableOptions(void *tkPtr);
const char *vrvToolkit_getElementAttr(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getElementsAtTime(void *tkPtr, int millisec);
const char *vrvToolkit_getExpansionIdsForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getHumdrum(void *tkPtr);
const char *vrvToolkit_getLog(void *tkPtr);
const char *vrvToolkit_getMEI(void *tkPtr, const char *options);
const char *vrvToolkit_getMIDIValuesForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getNotatedIdForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getOptions(void *tkPtr, bool default_values);
int vrvToolkit_getPageCount(void *tkPtr);
int vrvToolkit_getPageWithElement(void *tkPtr, const char *xmlId);
double vrvToolkit_getTimeForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getVersion(void *tkPtr);
bool vrvToolkit_loadData(void *tkPtr, const char *data);
bool vrvToolkit_loadZipDataBase64(void *tkPtr, const char *data);
bool vrvToolkit_loadZipDataBuffer(void *tkPtr, const unsigned char *data, int length);
const char *vrvToolkit_renderToMIDI(void *tkPtr, const char *c_options);
const char *vrvToolkit_renderToPAE(void *tkPtr);
const char *vrvToolkit_renderToSVG(void *tkPtr, int page_no, const char *c_options);
const char *vrvToolkit_renderToTimemap(void *tkPtr);
void vrvToolkit_redoLayout(void *tkPtr);
void vrvToolkit_redoPagePitchPosLayout(void *tkPtr);
const char *vrvToolkit_renderData(void *tkPtr, const char *data, const char *options);
void vrvToolkit_setOptions(void *tkPtr, const char *options);

#ifdef __cplusplus
} // extern C
#endif
>>>>>>> develop
