package verovio

/*
#cgo pkg-config: verovio
#include <stdbool.h>
#include <stdlib.h>
#include "../../tools/c_wrapper.h"
*/
import "C"

import (
	"os"
	"runtime"
	"unsafe"
)

// Toolkit wraps a Verovio toolkit instance from the C API.
type Toolkit struct {
	ptr unsafe.Pointer
}

func EnableLog(enabled bool) {
	C.enableLog(C.bool(enabled))
}

func EnableLogToBuffer(enabled bool) {
	C.enableLogToBuffer(C.bool(enabled))
}

func NewToolkit() *Toolkit {
	if resourcePath := os.Getenv("VEROVIO_RESOURCE_PATH"); resourcePath != "" {
		return NewToolkitWithResourcePath(resourcePath)
	}
	return NewToolkitWithoutResourcePath()
}

func NewToolkitWithResourcePath(resourcePath string) *Toolkit {
	cResourcePath := C.CString(resourcePath)
	defer C.free(unsafe.Pointer(cResourcePath))

	tk := &Toolkit{ptr: C.vrvToolkit_constructorResourcePath(cResourcePath)}
	runtime.SetFinalizer(tk, (*Toolkit).Close)
	return tk
}

func NewToolkitWithoutResourcePath() *Toolkit {
	tk := &Toolkit{ptr: C.vrvToolkit_constructorNoResource()}
	runtime.SetFinalizer(tk, (*Toolkit).Close)
	return tk
}

func (t *Toolkit) Close() {
	if t == nil || t.ptr == nil {
		return
	}
	C.vrvToolkit_destructor(t.ptr)
	t.ptr = nil
	runtime.SetFinalizer(t, nil)
}

func (t *Toolkit) Edit(editorAction string) bool {
	cEditorAction := C.CString(editorAction)
	defer C.free(unsafe.Pointer(cEditorAction))
	return bool(C.vrvToolkit_edit(t.ptr, cEditorAction))
}

func (t *Toolkit) EditInfo() string {
	return goString(C.vrvToolkit_editInfo(t.ptr))
}

func (t *Toolkit) AvailableOptions() string {
	return goString(C.vrvToolkit_getAvailableOptions(t.ptr))
}

func (t *Toolkit) DefaultOptions() string {
	return goString(C.vrvToolkit_getDefaultOptions(t.ptr))
}

func (t *Toolkit) DescriptiveFeatures(options string) string {
	cOptions := C.CString(options)
	defer C.free(unsafe.Pointer(cOptions))
	return goString(C.vrvToolkit_getDescriptiveFeatures(t.ptr, cOptions))
}

func (t *Toolkit) ElementAttr(xmlID string) string {
	cXMLID := C.CString(xmlID)
	defer C.free(unsafe.Pointer(cXMLID))
	return goString(C.vrvToolkit_getElementAttr(t.ptr, cXMLID))
}

func (t *Toolkit) ElementsAtTime(milliseconds int) string {
	return goString(C.vrvToolkit_getElementsAtTime(t.ptr, C.int(milliseconds)))
}

func (t *Toolkit) ExpansionIDsForElement(xmlID string) string {
	cXMLID := C.CString(xmlID)
	defer C.free(unsafe.Pointer(cXMLID))
	return goString(C.vrvToolkit_getExpansionIdsForElement(t.ptr, cXMLID))
}

func (t *Toolkit) Humdrum() string {
	return goString(C.vrvToolkit_getHumdrum(t.ptr))
}

func (t *Toolkit) HumdrumFile(filename string) bool {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	return bool(C.vrvToolkit_getHumdrumFile(t.ptr, cFilename))
}

func (t *Toolkit) ID() string {
	return goString(C.vrvToolkit_getID(t.ptr))
}

func (t *Toolkit) ConvertHumdrumToHumdrum(humdrumData string) string {
	cHumdrumData := C.CString(humdrumData)
	defer C.free(unsafe.Pointer(cHumdrumData))
	return goString(C.vrvToolkit_convertHumdrumToHumdrum(t.ptr, cHumdrumData))
}

func (t *Toolkit) ConvertHumdrumToMIDI(humdrumData string) string {
	cHumdrumData := C.CString(humdrumData)
	defer C.free(unsafe.Pointer(cHumdrumData))
	return goString(C.vrvToolkit_convertHumdrumToMIDI(t.ptr, cHumdrumData))
}

func (t *Toolkit) ConvertMEIToHumdrum(meiData string) string {
	cMEIData := C.CString(meiData)
	defer C.free(unsafe.Pointer(cMEIData))
	return goString(C.vrvToolkit_convertMEIToHumdrum(t.ptr, cMEIData))
}

func (t *Toolkit) Log() string {
	return goString(C.vrvToolkit_getLog(t.ptr))
}

func (t *Toolkit) MEI(options string) string {
	cOptions := C.CString(options)
	defer C.free(unsafe.Pointer(cOptions))
	return goString(C.vrvToolkit_getMEI(t.ptr, cOptions))
}

func (t *Toolkit) MIDIValuesForElement(xmlID string) string {
	cXMLID := C.CString(xmlID)
	defer C.free(unsafe.Pointer(cXMLID))
	return goString(C.vrvToolkit_getMIDIValuesForElement(t.ptr, cXMLID))
}

func (t *Toolkit) NotatedIDForElement(xmlID string) string {
	cXMLID := C.CString(xmlID)
	defer C.free(unsafe.Pointer(cXMLID))
	return goString(C.vrvToolkit_getNotatedIdForElement(t.ptr, cXMLID))
}

func (t *Toolkit) Options() string {
	return goString(C.vrvToolkit_getOptions(t.ptr))
}

func (t *Toolkit) OptionUsageString() string {
	return goString(C.vrvToolkit_getOptionUsageString(t.ptr))
}

func (t *Toolkit) PageCount() int {
	return int(C.vrvToolkit_getPageCount(t.ptr))
}

func (t *Toolkit) PageWithElement(xmlID string) int {
	cXMLID := C.CString(xmlID)
	defer C.free(unsafe.Pointer(cXMLID))
	return int(C.vrvToolkit_getPageWithElement(t.ptr, cXMLID))
}

func (t *Toolkit) ResourcePath() string {
	return goString(C.vrvToolkit_getResourcePath(t.ptr))
}

func (t *Toolkit) Scale() int {
	return int(C.vrvToolkit_getScale(t.ptr))
}

func (t *Toolkit) TimeForElement(xmlID string) float64 {
	cXMLID := C.CString(xmlID)
	defer C.free(unsafe.Pointer(cXMLID))
	return float64(C.vrvToolkit_getTimeForElement(t.ptr, cXMLID))
}

func (t *Toolkit) TimesForElement(xmlID string) string {
	cXMLID := C.CString(xmlID)
	defer C.free(unsafe.Pointer(cXMLID))
	return goString(C.vrvToolkit_getTimesForElement(t.ptr, cXMLID))
}

func (t *Toolkit) Version() string {
	return goString(C.vrvToolkit_getVersion(t.ptr))
}

func (t *Toolkit) LoadData(data string) bool {
	cData := C.CString(data)
	defer C.free(unsafe.Pointer(cData))
	return bool(C.vrvToolkit_loadData(t.ptr, cData))
}

func (t *Toolkit) LoadFile(filename string) bool {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	return bool(C.vrvToolkit_loadFile(t.ptr, cFilename))
}

func (t *Toolkit) LoadZipDataBase64(data string) bool {
	cData := C.CString(data)
	defer C.free(unsafe.Pointer(cData))
	return bool(C.vrvToolkit_loadZipDataBase64(t.ptr, cData))
}

func (t *Toolkit) LoadZipDataBuffer(data []byte) bool {
	if len(data) == 0 {
		return bool(C.vrvToolkit_loadZipDataBuffer(t.ptr, nil, 0))
	}
	return bool(C.vrvToolkit_loadZipDataBuffer(
		t.ptr,
		(*C.uchar)(unsafe.Pointer(&data[0])),
		C.int(len(data)),
	))
}

func (t *Toolkit) RedoLayout(options string) {
	cOptions := C.CString(options)
	defer C.free(unsafe.Pointer(cOptions))
	C.vrvToolkit_redoLayout(t.ptr, cOptions)
}

func (t *Toolkit) RedoPagePitchPosLayout() {
	C.vrvToolkit_redoPagePitchPosLayout(t.ptr)
}

func (t *Toolkit) RenderData(data string, options string) string {
	cData := C.CString(data)
	defer C.free(unsafe.Pointer(cData))
	cOptions := C.CString(options)
	defer C.free(unsafe.Pointer(cOptions))
	return goString(C.vrvToolkit_renderData(t.ptr, cData, cOptions))
}

func (t *Toolkit) RenderToExpansionMap() string {
	return goString(C.vrvToolkit_renderToExpansionMap(t.ptr))
}

func (t *Toolkit) RenderToExpansionMapFile(filename string) bool {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	return bool(C.vrvToolkit_renderToExpansionMapFile(t.ptr, cFilename))
}

func (t *Toolkit) RenderToMIDI() string {
	return goString(C.vrvToolkit_renderToMIDI(t.ptr))
}

func (t *Toolkit) RenderToMIDIFile(filename string) bool {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	return bool(C.vrvToolkit_renderToMIDIFile(t.ptr, cFilename))
}

func (t *Toolkit) RenderToPAE() string {
	return goString(C.vrvToolkit_renderToPAE(t.ptr))
}

func (t *Toolkit) RenderToPAEFile(filename string) bool {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	return bool(C.vrvToolkit_renderToPAEFile(t.ptr, cFilename))
}

func (t *Toolkit) RenderToSVG(pageNumber int, xmlDeclaration bool) string {
	return goString(C.vrvToolkit_renderToSVG(t.ptr, C.int(pageNumber), C.bool(xmlDeclaration)))
}

func (t *Toolkit) RenderToSVGFile(filename string, pageNumber int) bool {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	return bool(C.vrvToolkit_renderToSVGFile(t.ptr, cFilename, C.int(pageNumber)))
}

func (t *Toolkit) RenderToTimemap(options string) string {
	cOptions := C.CString(options)
	defer C.free(unsafe.Pointer(cOptions))
	return goString(C.vrvToolkit_renderToTimemap(t.ptr, cOptions))
}

func (t *Toolkit) RenderToTimemapFile(filename string, options string) bool {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	cOptions := C.CString(options)
	defer C.free(unsafe.Pointer(cOptions))
	return bool(C.vrvToolkit_renderToTimemapFile(t.ptr, cFilename, cOptions))
}

func (t *Toolkit) ResetOptions() {
	C.vrvToolkit_resetOptions(t.ptr)
}

func (t *Toolkit) ResetXMLIDSeed(seed int) {
	C.vrvToolkit_resetXmlIdSeed(t.ptr, C.int(seed))
}

func (t *Toolkit) SaveFile(filename string, options string) bool {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	cOptions := C.CString(options)
	defer C.free(unsafe.Pointer(cOptions))
	return bool(C.vrvToolkit_saveFile(t.ptr, cFilename, cOptions))
}

func (t *Toolkit) Select(selection string) bool {
	cSelection := C.CString(selection)
	defer C.free(unsafe.Pointer(cSelection))
	return bool(C.vrvToolkit_select(t.ptr, cSelection))
}

func (t *Toolkit) SetInputFrom(inputFrom string) bool {
	cInputFrom := C.CString(inputFrom)
	defer C.free(unsafe.Pointer(cInputFrom))
	return bool(C.vrvToolkit_setInputFrom(t.ptr, cInputFrom))
}

func (t *Toolkit) SetOptions(options string) bool {
	cOptions := C.CString(options)
	defer C.free(unsafe.Pointer(cOptions))
	return bool(C.vrvToolkit_setOptions(t.ptr, cOptions))
}

func (t *Toolkit) SetOutputTo(outputTo string) bool {
	cOutputTo := C.CString(outputTo)
	defer C.free(unsafe.Pointer(cOutputTo))
	return bool(C.vrvToolkit_setOutputTo(t.ptr, cOutputTo))
}

func (t *Toolkit) SetResourcePath(path string) bool {
	cPath := C.CString(path)
	defer C.free(unsafe.Pointer(cPath))
	return bool(C.vrvToolkit_setResourcePath(t.ptr, cPath))
}

func (t *Toolkit) SetScale(scale int) bool {
	return bool(C.vrvToolkit_setScale(t.ptr, C.int(scale)))
}

func (t *Toolkit) ValidatePAE(data string) string {
	cData := C.CString(data)
	defer C.free(unsafe.Pointer(cData))
	return goString(C.vrvToolkit_validatePAE(t.ptr, cData))
}

func (t *Toolkit) ValidatePAEFile(filename string) string {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))
	return goString(C.vrvToolkit_validatePAEFile(t.ptr, cFilename))
}

func goString(value *C.char) string {
	if value == nil {
		return ""
	}
	return C.GoString(value)
}
