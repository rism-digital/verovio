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

void enableLog(bool value);
void enableLogToBuffer(bool value);

void *vrvToolkit_constructor();
void *vrvToolkit_constructorResourcePath(const char *resourcePath);

void vrvToolkit_destructor(void *tkPtr);
bool vrvToolkit_edit(void *tkPtr, const char *editorAction);
const char *vrvToolkit_editInfo(void *tkPtr);
const char *vrvToolkit_getAvailableOptions(void *tkPtr);
const char *vrvToolkit_getDefaultOptions(void *tkPtr);
const char *vrvToolkit_getDescriptiveFeatures(void *tkPtr, const char *options);
const char *vrvToolkit_getElementAttr(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getElementsAtTime(void *tkPtr, int millisec);
const char *vrvToolkit_getExpansionIdsForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getHumdrum(void *tkPtr);
bool vrvToolkit_getHumdrumFile(void *tkPtr, const char *filename);
const char *vrvToolkit_getID(void *tkPtr);
const char *vrvToolkit_convertHumdrumToHumdrum(void *tkPtr, const char *humdrumData);
const char *vrvToolkit_convertHumdrumToMIDI(void *tkPtr, const char *humdrumData);
const char *vrvToolkit_convertMEIToHumdrum(void *tkPtr, const char *meiData);
const char *vrvToolkit_getLog(void *tkPtr);
const char *vrvToolkit_getMEI(void *tkPtr, const char *options);
const char *vrvToolkit_getMIDIValuesForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getNotatedIdForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getOptions(void *tkPtr);
const char *vrvToolkit_getOptionUsageString(void *tkPtr);
int vrvToolkit_getPageCount(void *tkPtr);
int vrvToolkit_getPageWithElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getResourcePath(void *tkPtr);
int vrvToolkit_getScale(void *tkPtr);
double vrvToolkit_getTimeForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getTimesForElement(void *tkPtr, const char *xmlId);
const char *vrvToolkit_getVersion(void *tkPtr);
bool vrvToolkit_loadData(void *tkPtr, const char *data);
bool vrvToolkit_loadFile(void *tkPtr, const char *filename);
bool vrvToolkit_loadZipDataBase64(void *tkPtr, const char *data);
bool vrvToolkit_loadZipDataBuffer(void *tkPtr, const unsigned char *data, int length);
void vrvToolkit_redoLayout(void *tkPtr, const char *c_options);
void vrvToolkit_redoPagePitchPosLayout(void *tkPtr);
const char *vrvToolkit_renderData(void *tkPtr, const char *data, const char *options);
const char *vrvToolkit_renderToExpansionMap(void *tkPtr);
bool vrvToolkit_renderToExpansionMapFile(void *tkPtr, const char *filename);
const char *vrvToolkit_renderToMIDI(void *tkPtr);
bool vrvToolkit_renderToMIDIFile(void *tkPtr, const char *filename);
const char *vrvToolkit_renderToPAE(void *tkPtr);
bool vrvToolkit_renderToPAEFile(void *tkPtr, const char *filename);
const char *vrvToolkit_renderToSVG(void *tkPtr, int page_no, bool xmlDeclaration);
bool vrvToolkit_renderToSVGFile(void *tkPtr, const char *filename, int pageNo);
const char *vrvToolkit_renderToTimemap(void *tkPtr, const char *c_options);
bool vrvToolkit_renderToTimemapFile(void *tkPtr, const char *filename, const char *c_options);
void vrvToolkit_resetOptions(void *tkPtr);
void vrvToolkit_resetXmlIdSeed(void *tkPtr, int seed);
bool vrvToolkit_saveFile(void *tkPtr, const char *filename, const char *c_options);
bool vrvToolkit_select(void *tkPtr, const char *selection);
bool vrvToolkit_setInputFrom(void *tkPtr, const char *inputFrom);
bool vrvToolkit_setOptions(void *tkPtr, const char *options);
bool vrvToolkit_setOutputTo(void *tkPtr, const char *outputTo);
bool vrvToolkit_setResourcePath(void *tkPtr, const char *path);
bool vrvToolkit_setScale(void *tkPtr, int scale);
const char *vrvToolkit_validatePAE(void *tkPtr, const char *data);
const char *vrvToolkit_validatePAEFile(void *tkPtr, const char *filename);

#ifdef __cplusplus
} // extern C
#endif
