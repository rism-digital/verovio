/////////////////////////////////////////////////////////////////////////////
// Name:        c_wrapper.h
// Author:      Laurent Pugin
// Created:     19/03/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

/****************************************************************
 * Methods exported a functions to use the Toolkit class
 ****************************************************************/

void *vrvToolkit_constructor();
void *vrvToolkit_constructorResourcePath(const char * resourcePath);
void vrvToolkit_destructor(Toolkit *tk);
bool vrvToolkit_edit(Toolkit *tk, const char *editorAction);
const char *vrvToolkit_getAvailableOptions(Toolkit *tk);
const char *vrvToolkit_getElementAttr(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getElementsAtTime(Toolkit *tk, int millisec);
const char *vrvToolkit_getHumdrum(Toolkit *tk);
const char *vrvToolkit_getLog(Toolkit *tk);
const char *vrvToolkit_getMEI(Toolkit *tk, int page_no, bool score_based);
const char *vrvToolkit_getMIDIValuesForElement(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getOptions(Toolkit *tk, bool default_values);
int vrvToolkit_getPageCount(Toolkit *tk);
int vrvToolkit_getPageWithElement(Toolkit *tk, const char *xmlId);
double vrvToolkit_getTimeForElement(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getVersion(Toolkit *tk);
bool vrvToolkit_loadData(Toolkit *tk, const char *data);
const char *vrvToolkit_renderToMIDI(Toolkit *tk, const char *c_options);
const char *vrvToolkit_renderToSVG(Toolkit *tk, int page_no, const char *c_options);
const char *vrvToolkit_renderToTimemap(Toolkit *tk);
void vrvToolkit_redoLayout(Toolkit *tk);
void vrvToolkit_redoPagePitchPosLayout(Toolkit *tk);
const char *vrvToolkit_renderData(Toolkit *tk, const char *data, const char *options);
void vrvToolkit_setOptions(Toolkit *tk, const char *options);
