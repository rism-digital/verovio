package main

import "C"
import (
	"os"
	"runtime"
	"unsafe"

	"github.com/ebitengine/purego"
)

var (
	libverovio                           uintptr
	err                                  error
	vrvToolkit_constructorNoResource     func() unsafe.Pointer
	vrvToolkit_constructorResourcePath   func(resourcePath string) unsafe.Pointer
	vrvToolkit_destructor                func(ptr unsafe.Pointer)
	vrvToolkit_edit                      func(ptr unsafe.Pointer, editorAction string) bool
	vrvToolkit_editResponse              func(ptr unsafe.Pointer) string
	vrvToolkit_editStatus                func(ptr unsafe.Pointer) string
	vrvToolkit_getAvailableOptions       func(ptr unsafe.Pointer) string
	vrvToolkit_getDefaultOptions         func(ptr unsafe.Pointer) string
	vrvToolkit_getDescriptiveFeatures    func(ptr unsafe.Pointer, options string) string
	vrvToolkit_getElementAttr            func(ptr unsafe.Pointer, xmlID string) string
	vrvToolkit_getElementsAtTime         func(ptr unsafe.Pointer, milliseconds int) string
	vrvToolkit_getExpansionIdsForElement func(ptr unsafe.Pointer, xmlID string) string
	vrvToolkit_getHumdrum                func(ptr unsafe.Pointer) string
	vrvToolkit_getHumdrumFile            func(ptr unsafe.Pointer, filename string) bool
	vrvToolkit_getID                     func(ptr unsafe.Pointer) string
	vrvToolkit_convertHumdrumToHumdrum   func(ptr unsafe.Pointer, humdrumData string) string
	vrvToolkit_convertHumdrumToMIDI      func(ptr unsafe.Pointer, humdrumData string) string
	vrvToolkit_convertMEIToHumdrum       func(ptr unsafe.Pointer, meiData string) string
	vrvToolkit_getLog                    func(ptr unsafe.Pointer) string
	vrvToolkit_getMEI                    func(ptr unsafe.Pointer, options string) string
	vrvToolkit_getMIDIValuesForElement   func(ptr unsafe.Pointer, xmlID string) string
	vrvToolkit_getNotatedIdForElement    func(ptr unsafe.Pointer, xmlID string) string
	vrvToolkit_getOptions                func(ptr unsafe.Pointer) string
	vrvToolkit_getOptionUsageString      func(ptr unsafe.Pointer) string
	vrvToolkit_getPageCount              func(ptr unsafe.Pointer) int
	vrvToolkit_getPageWithElement        func(ptr unsafe.Pointer, xmlID string) int
	vrvToolkit_getResourcePath           func(ptr unsafe.Pointer) string
	vrvToolkit_getScale                  func(ptr unsafe.Pointer) int
	vrvToolkit_getTimeForElement         func(ptr unsafe.Pointer, xmlID string) float64
	vrvToolkit_getTimesForElement        func(ptr unsafe.Pointer, xmlID string) string
	vrvToolkit_getVersion                func(ptr unsafe.Pointer) string
	vrvToolkit_loadData                  func(ptr unsafe.Pointer, data string) bool
	vrvToolkit_loadFile                  func(ptr unsafe.Pointer, filename string) bool
	vrvToolkit_loadZipDataBase64         func(ptr unsafe.Pointer, data string) bool
	vrvToolkit_loadZipDataBuffer         func(ptr1 unsafe.Pointer, ptr2 unsafe.Pointer, length int) bool
	vrvToolkit_redoLayout                func(ptr unsafe.Pointer, options string)
	vrvToolkit_redoPagePitchPosLayout    func(ptr unsafe.Pointer)
	vrvToolkit_renderData                func(ptr unsafe.Pointer, data string, options string) string
	vrvToolkit_renderToExpansionMap      func(ptr unsafe.Pointer) string
	vrvToolkit_renderToExpansionMapFile  func(ptr unsafe.Pointer, filename string) bool
	vrvToolkit_renderToMIDI              func(ptr unsafe.Pointer) string
	vrvToolkit_renderToMIDIFile          func(ptr unsafe.Pointer, filename string) bool
	vrvToolkit_renderToPAE               func(ptr unsafe.Pointer) string
	vrvToolkit_renderToPAEFile           func(ptr unsafe.Pointer, filename string) bool
	vrvToolkit_renderToSVG               func(ptr unsafe.Pointer, pageNumber int, xmlDeclaration bool) string
	vrvToolkit_renderToSVGFile           func(ptr unsafe.Pointer, filename string, pageNumber int) bool
	vrvToolkit_renderToTimemap           func(ptr unsafe.Pointer, options string) string
	vrvToolkit_renderToTimemapFile       func(ptr unsafe.Pointer, filename string, options string) bool
	vrvToolkit_resetOptions              func(ptr unsafe.Pointer)
	vrvToolkit_resetXmlIdSeed            func(ptr unsafe.Pointer, seed int)
	vrvToolkit_saveFile                  func(ptr unsafe.Pointer, filename string, options string) bool
	vrvToolkit_select                    func(ptr unsafe.Pointer, selection string) bool
	vrvToolkit_setInputFrom              func(ptr unsafe.Pointer, inputFrom string) bool
	vrvToolkit_setOptions                func(ptr unsafe.Pointer, options string) bool
	vrvToolkit_setOutputTo               func(ptr unsafe.Pointer, outputTo string) bool
	vrvToolkit_setResourcePath           func(ptr unsafe.Pointer, path string) bool
	vrvToolkit_setScale                  func(ptr unsafe.Pointer, scale int) bool
	vrvToolkit_validatePAE               func(ptr unsafe.Pointer, data string) string
	vrvToolkit_validatePAEFile           func(ptr unsafe.Pointer, filename string) string
)

func init() {
	libverovio, err = purego.Dlopen("./libverovio.so", purego.RTLD_NOW|purego.RTLD_GLOBAL)
	if err != nil {
		panic(err)
	}
	purego.RegisterLibFunc(&vrvToolkit_constructorNoResource, libverovio, "vrvToolkit_constructorNoResource")
	purego.RegisterLibFunc(&vrvToolkit_constructorResourcePath, libverovio, "vrvToolkit_constructorResourcePath")
	purego.RegisterLibFunc(&vrvToolkit_destructor, libverovio, "vrvToolkit_destructor")
	purego.RegisterLibFunc(&vrvToolkit_edit, libverovio, "vrvToolkit_edit")
	purego.RegisterLibFunc(&vrvToolkit_editResponse, libverovio, "vrvToolkit_editResponse")
	purego.RegisterLibFunc(&vrvToolkit_editStatus, libverovio, "vrvToolkit_editStatus")
	purego.RegisterLibFunc(&vrvToolkit_getAvailableOptions, libverovio, "vrvToolkit_getAvailableOptions")
	purego.RegisterLibFunc(&vrvToolkit_getDefaultOptions, libverovio, "vrvToolkit_getDefaultOptions")
	purego.RegisterLibFunc(&vrvToolkit_getDescriptiveFeatures, libverovio, "vrvToolkit_getDescriptiveFeatures")
	purego.RegisterLibFunc(&vrvToolkit_getElementAttr, libverovio, "vrvToolkit_getElementAttr")
	purego.RegisterLibFunc(&vrvToolkit_getElementsAtTime, libverovio, "vrvToolkit_getElementsAtTime")
	purego.RegisterLibFunc(&vrvToolkit_getExpansionIdsForElement, libverovio, "vrvToolkit_getExpansionIdsForElement")
	purego.RegisterLibFunc(&vrvToolkit_getHumdrum, libverovio, "vrvToolkit_getHumdrum")
	purego.RegisterLibFunc(&vrvToolkit_getHumdrumFile, libverovio, "vrvToolkit_getHumdrumFile")
	purego.RegisterLibFunc(&vrvToolkit_getID, libverovio, "vrvToolkit_getID")
	purego.RegisterLibFunc(&vrvToolkit_convertHumdrumToHumdrum, libverovio, "vrvToolkit_convertHumdrumToHumdrum")
	purego.RegisterLibFunc(&vrvToolkit_convertHumdrumToMIDI, libverovio, "vrvToolkit_convertHumdrumToMIDI")
	purego.RegisterLibFunc(&vrvToolkit_convertMEIToHumdrum, libverovio, "vrvToolkit_convertMEIToHumdrum")
	purego.RegisterLibFunc(&vrvToolkit_getLog, libverovio, "vrvToolkit_getLog")
	purego.RegisterLibFunc(&vrvToolkit_getMEI, libverovio, "vrvToolkit_getMEI")
	purego.RegisterLibFunc(&vrvToolkit_getMIDIValuesForElement, libverovio, "vrvToolkit_getMIDIValuesForElement")
	purego.RegisterLibFunc(&vrvToolkit_getNotatedIdForElement, libverovio, "vrvToolkit_getNotatedIdForElement")
	purego.RegisterLibFunc(&vrvToolkit_getOptions, libverovio, "vrvToolkit_getOptions")
	purego.RegisterLibFunc(&vrvToolkit_getOptionUsageString, libverovio, "vrvToolkit_getOptionUsageString")
	purego.RegisterLibFunc(&vrvToolkit_getPageCount, libverovio, "vrvToolkit_getPageCount")
	purego.RegisterLibFunc(&vrvToolkit_getPageWithElement, libverovio, "vrvToolkit_getPageWithElement")
	purego.RegisterLibFunc(&vrvToolkit_getResourcePath, libverovio, "vrvToolkit_getResourcePath")
	purego.RegisterLibFunc(&vrvToolkit_getScale, libverovio, "vrvToolkit_getScale")
	purego.RegisterLibFunc(&vrvToolkit_getTimeForElement, libverovio, "vrvToolkit_getTimeForElement")
	purego.RegisterLibFunc(&vrvToolkit_getTimesForElement, libverovio, "vrvToolkit_getTimesForElement")
	purego.RegisterLibFunc(&vrvToolkit_getVersion, libverovio, "vrvToolkit_getVersion")
	purego.RegisterLibFunc(&vrvToolkit_loadData, libverovio, "vrvToolkit_loadData")
	purego.RegisterLibFunc(&vrvToolkit_loadFile, libverovio, "vrvToolkit_loadFile")
	purego.RegisterLibFunc(&vrvToolkit_loadZipDataBase64, libverovio, "vrvToolkit_loadZipDataBase64")
	purego.RegisterLibFunc(&vrvToolkit_loadZipDataBuffer, libverovio, "vrvToolkit_loadZipDataBuffer")
	purego.RegisterLibFunc(&vrvToolkit_redoLayout, libverovio, "vrvToolkit_redoLayout")
	purego.RegisterLibFunc(&vrvToolkit_redoPagePitchPosLayout, libverovio, "vrvToolkit_redoPagePitchPosLayout")
	purego.RegisterLibFunc(&vrvToolkit_renderData, libverovio, "vrvToolkit_renderData")
	purego.RegisterLibFunc(&vrvToolkit_renderToExpansionMap, libverovio, "vrvToolkit_renderToExpansionMap")
	purego.RegisterLibFunc(&vrvToolkit_renderToExpansionMapFile, libverovio, "vrvToolkit_renderToExpansionMapFile")
	purego.RegisterLibFunc(&vrvToolkit_renderToMIDI, libverovio, "vrvToolkit_renderToMIDI")
	purego.RegisterLibFunc(&vrvToolkit_renderToMIDIFile, libverovio, "vrvToolkit_renderToMIDIFile")
	purego.RegisterLibFunc(&vrvToolkit_renderToPAE, libverovio, "vrvToolkit_renderToPAE")
	purego.RegisterLibFunc(&vrvToolkit_renderToPAEFile, libverovio, "vrvToolkit_renderToPAEFile")
	purego.RegisterLibFunc(&vrvToolkit_renderToSVG, libverovio, "vrvToolkit_renderToSVG")
	purego.RegisterLibFunc(&vrvToolkit_renderToSVGFile, libverovio, "vrvToolkit_renderToSVGFile")
	purego.RegisterLibFunc(&vrvToolkit_renderToTimemap, libverovio, "vrvToolkit_renderToTimemap")
	purego.RegisterLibFunc(&vrvToolkit_renderToTimemapFile, libverovio, "vrvToolkit_renderToTimemapFile")
	purego.RegisterLibFunc(&vrvToolkit_resetOptions, libverovio, "vrvToolkit_resetOptions")
	purego.RegisterLibFunc(&vrvToolkit_resetXmlIdSeed, libverovio, "vrvToolkit_resetXmlIdSeed")
	purego.RegisterLibFunc(&vrvToolkit_saveFile, libverovio, "vrvToolkit_saveFile")
	purego.RegisterLibFunc(&vrvToolkit_select, libverovio, "vrvToolkit_select")
	purego.RegisterLibFunc(&vrvToolkit_setInputFrom, libverovio, "vrvToolkit_setInputFrom")
	purego.RegisterLibFunc(&vrvToolkit_setOptions, libverovio, "vrvToolkit_setOptions")
	purego.RegisterLibFunc(&vrvToolkit_setOutputTo, libverovio, "vrvToolkit_setOutputTo")
	purego.RegisterLibFunc(&vrvToolkit_setResourcePath, libverovio, "vrvToolkit_setResourcePath")
	purego.RegisterLibFunc(&vrvToolkit_setScale, libverovio, "vrvToolkit_setScale")
	purego.RegisterLibFunc(&vrvToolkit_validatePAE, libverovio, "vrvToolkit_validatePAE")
	purego.RegisterLibFunc(&vrvToolkit_validatePAEFile, libverovio, "vrvToolkit_validatePAEFile")
}

type Toolkit struct {
	ptr unsafe.Pointer
}

func NewToolkit() *Toolkit {
	if resourcePath := os.Getenv("VEROVIO_RESOURCE_PATH"); resourcePath != "" {
		return NewToolkitWithResourcePath(resourcePath)
	}
	return NewToolkitWithoutResourcePath()
}

func NewToolkitWithResourcePath(resourcePath string) *Toolkit {
	tk := &Toolkit{ptr: vrvToolkit_constructorResourcePath(resourcePath)}
	runtime.SetFinalizer(tk, (*Toolkit).Close)
	return tk
}

func NewToolkitWithoutResourcePath() *Toolkit {
	tk := &Toolkit{ptr: vrvToolkit_constructorNoResource()}
	runtime.SetFinalizer(tk, (*Toolkit).Close)
	return tk
}

func (t *Toolkit) Close() {
	if t == nil || t.ptr == nil {
		return
	}
	vrvToolkit_destructor(t.ptr)
	t.ptr = nil
	runtime.SetFinalizer(t, nil)
}

func (t *Toolkit) Edit(editorAction string) bool {
	return vrvToolkit_edit(t.ptr, editorAction)
}

func (t *Toolkit) EditResponse() string {
	return vrvToolkit_editResponse(t.ptr)
}

func (t *Toolkit) EditStatus() string {
	return vrvToolkit_editStatus(t.ptr)
}

func (t *Toolkit) AvailableOptions() string {
	return vrvToolkit_getAvailableOptions(t.ptr)
}

func (t *Toolkit) DefaultOptions() string {
	return vrvToolkit_getDefaultOptions(t.ptr)
}

func (t *Toolkit) DescriptiveFeatures(options string) string {
	return vrvToolkit_getDescriptiveFeatures(t.ptr, options)
}

func (t *Toolkit) ElementAttr(xmlID string) string {
	return vrvToolkit_getElementAttr(t.ptr, xmlID)
}

func (t *Toolkit) ElementsAtTime(milliseconds int) string {
	return vrvToolkit_getElementsAtTime(t.ptr, milliseconds)
}

func (t *Toolkit) ExpansionIDsForElement(xmlID string) string {
	return vrvToolkit_getExpansionIdsForElement(t.ptr, xmlID)
}

func (t *Toolkit) Humdrum() string {
	return vrvToolkit_getHumdrum(t.ptr)
}

func (t *Toolkit) HumdrumFile(filename string) bool {
	return vrvToolkit_getHumdrumFile(t.ptr, filename)
}

func (t *Toolkit) ID() string {
	return vrvToolkit_getID(t.ptr)
}

func (t *Toolkit) ConvertHumdrumToHumdrum(humdrumData string) string {
	return vrvToolkit_convertHumdrumToHumdrum(t.ptr, humdrumData)
}

func (t *Toolkit) ConvertHumdrumToMIDI(humdrumData string) string {
	return vrvToolkit_convertHumdrumToMIDI(t.ptr, humdrumData)
}

func (t *Toolkit) ConvertMEIToHumdrum(meiData string) string {
	return vrvToolkit_convertMEIToHumdrum(t.ptr, meiData)
}

func (t *Toolkit) Log() string {
	return vrvToolkit_getLog(t.ptr)
}

func (t *Toolkit) MEI(options string) string {
	return vrvToolkit_getMEI(t.ptr, options)
}

func (t *Toolkit) MIDIValuesForElement(xmlID string) string {
	return vrvToolkit_getMIDIValuesForElement(t.ptr, xmlID)
}

func (t *Toolkit) NotatedIDForElement(xmlID string) string {
	return vrvToolkit_getNotatedIdForElement(t.ptr, xmlID)
}

func (t *Toolkit) Options() string {
	return vrvToolkit_getOptions(t.ptr)
}

func (t *Toolkit) OptionUsageString() string {
	return vrvToolkit_getOptionUsageString(t.ptr)
}

func (t *Toolkit) PageCount() int {
	return vrvToolkit_getPageCount(t.ptr)
}

func (t *Toolkit) PageWithElement(xmlID string) int {
	return vrvToolkit_getPageWithElement(t.ptr, xmlID)
}

func (t *Toolkit) ResourcePath() string {
	return vrvToolkit_getResourcePath(t.ptr)
}

func (t *Toolkit) Scale() int {
	return vrvToolkit_getScale(t.ptr)
}

func (t *Toolkit) TimeForElement(xmlID string) float64 {
	return vrvToolkit_getTimeForElement(t.ptr, xmlID)
}

func (t *Toolkit) TimesForElement(xmlID string) string {
	return vrvToolkit_getTimesForElement(t.ptr, xmlID)
}

func (t *Toolkit) Version() string {
	return vrvToolkit_getVersion(t.ptr)
}

func (t *Toolkit) LoadData(data string) bool {
	return vrvToolkit_loadData(t.ptr, data)
}

func (t *Toolkit) LoadFile(filename string) bool {
	return vrvToolkit_loadFile(t.ptr, filename)
}

func (t *Toolkit) LoadZipDataBase64(data string) bool {
	return vrvToolkit_loadZipDataBase64(t.ptr, data)
}

func (t *Toolkit) LoadZipDataBuffer(data []byte) bool {
	if len(data) == 0 {
		return vrvToolkit_loadZipDataBuffer(t.ptr, nil, 0)
	}
	return vrvToolkit_loadZipDataBuffer(
		t.ptr,
		unsafe.Pointer(&data[0]),
		len(data),
	)
}

func (t *Toolkit) RedoLayout(options string) {
	vrvToolkit_redoLayout(t.ptr, options)
}

func (t *Toolkit) RedoPagePitchPosLayout() {
	vrvToolkit_redoPagePitchPosLayout(t.ptr)
}

func (t *Toolkit) RenderData(data string, options string) string {
	return vrvToolkit_renderData(t.ptr, data, options)
}

func (t *Toolkit) RenderToExpansionMap() string {
	return vrvToolkit_renderToExpansionMap(t.ptr)
}

func (t *Toolkit) RenderToExpansionMapFile(filename string) bool {
	return vrvToolkit_renderToExpansionMapFile(t.ptr, filename)
}

func (t *Toolkit) RenderToMIDI() string {
	return vrvToolkit_renderToMIDI(t.ptr)
}

func (t *Toolkit) RenderToMIDIFile(filename string) bool {
	return vrvToolkit_renderToMIDIFile(t.ptr, filename)
}

func (t *Toolkit) RenderToPAE() string {
	return vrvToolkit_renderToPAE(t.ptr)
}

func (t *Toolkit) RenderToPAEFile(filename string) bool {
	return vrvToolkit_renderToPAEFile(t.ptr, filename)
}

func (t *Toolkit) RenderToSVG(pageNumber int, xmlDeclaration bool) string {
	return vrvToolkit_renderToSVG(t.ptr, pageNumber, xmlDeclaration)
}

func (t *Toolkit) RenderToSVGFile(filename string, pageNumber int) bool {
	return vrvToolkit_renderToSVGFile(t.ptr, filename, pageNumber)
}

func (t *Toolkit) RenderToTimemap(options string) string {
	return vrvToolkit_renderToTimemap(t.ptr, options)
}

func (t *Toolkit) RenderToTimemapFile(filename string, options string) bool {
	return vrvToolkit_renderToTimemapFile(t.ptr, filename, options)
}

func (t *Toolkit) ResetOptions() {
	vrvToolkit_resetOptions(t.ptr)
}

func (t *Toolkit) ResetXMLIDSeed(seed int) {
	vrvToolkit_resetXmlIdSeed(t.ptr, seed)
}

func (t *Toolkit) SaveFile(filename string, options string) bool {
	return vrvToolkit_saveFile(t.ptr, filename, options)
}

func (t *Toolkit) Select(selection string) bool {
	return vrvToolkit_select(t.ptr, selection)
}

func (t *Toolkit) SetInputFrom(inputFrom string) bool {
	return vrvToolkit_setInputFrom(t.ptr, inputFrom)
}

func (t *Toolkit) SetOptions(options string) bool {
	return vrvToolkit_setOptions(t.ptr, options)
}

func (t *Toolkit) SetOutputTo(outputTo string) bool {
	return vrvToolkit_setOutputTo(t.ptr, outputTo)
}

func (t *Toolkit) SetResourcePath(path string) bool {
	return vrvToolkit_setResourcePath(t.ptr, path)
}

func (t *Toolkit) SetScale(scale int) bool {
	return vrvToolkit_setScale(t.ptr, scale)
}

func (t *Toolkit) ValidatePAE(data string) string {
	return vrvToolkit_validatePAE(t.ptr, data)
}

func (t *Toolkit) ValidatePAEFile(filename string) string {
	return vrvToolkit_validatePAEFile(t.ptr, filename)
}
