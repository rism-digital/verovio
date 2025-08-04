
import Foundation
import VerovioCore

public class VerovioToolkit {
    private var toolkitPtr: UnsafeMutableRawPointer?

    public init() {
        self.toolkitPtr = vrvToolkit_constructorNoResource()
    }
    
    public init(_ resourcePath: String) {
        resourcePath.withCString{ cStr in
            self.toolkitPtr = vrvToolkit_constructorResourcePath(cStr)
        }
    }

    deinit {
        if let tk = toolkitPtr {
            vrvToolkit_destructor(tk)
        }
    }
    
    public func edit(_ action: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return action.withCString { cStr in
            vrvToolkit_edit(tk, cStr)
        }
    }
    
    public func editInfo() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_editInfo(tk) else { return "" }
        return String(cString: cStr)
    }
    
    public func getAvailableOptions() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getAvailableOptions(tk) else { return "" }
        return String(cString: cStr)
    }
    
    public func getDefaultOptions() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getDefaultOptions(tk) else { return "" }
        return String(cString: cStr)
    }
    
    public func getDescriptiveFeatures(_ options: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return options.withCString { cStr in
            if let resultCStr = vrvToolkit_getDescriptiveFeatures(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }
    
    public func getElementAttr(_ element: String, _ attribute: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return element.withCString { elementCStr in
            if let resultCStr = vrvToolkit_getElementAttr(tk, elementCStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }
    
    public func getElementsAtTime(_ millisec: Int) -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getElementsAtTime(tk, Int32(millisec)) else { return "" }
        return String(cString: cStr)
    }
    
    public func getExpansionIdsForElement(_ xmlId: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return xmlId.withCString { cStr in
            if let resultCStr = vrvToolkit_getExpansionIdsForElement(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func getHumdrum() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getHumdrum(tk) else { return "" }
        return String(cString: cStr)
    }

    public func getHumdrumFile(_ filename: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return filename.withCString { cStr in
            vrvToolkit_getHumdrumFile(tk, cStr)
        }
    }

    public func getID() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getID(tk) else { return "" }
        return String(cString: cStr)
    }

    public func convertHumdrumToHumdrum(_ humdrumData: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return humdrumData.withCString { cStr in
            if let resultCStr = vrvToolkit_convertHumdrumToHumdrum(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func convertHumdrumToMIDI(_ humdrumData: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return humdrumData.withCString { cStr in
            if let resultCStr = vrvToolkit_convertHumdrumToMIDI(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func convertMEIToHumdrum(_ meiData: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return meiData.withCString { cStr in
            if let resultCStr = vrvToolkit_convertMEIToHumdrum(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func getLog() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getLog(tk) else { return "" }
        return String(cString: cStr)
    }

    public func getMEI(_ options: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return options.withCString { cStr in
            if let resultCStr = vrvToolkit_getMEI(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func getMIDIValuesForElement(_ xmlId: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return xmlId.withCString { cStr in
            if let resultCStr = vrvToolkit_getMIDIValuesForElement(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func getNotatedIdForElement(_ xmlId: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return xmlId.withCString { cStr in
            if let resultCStr = vrvToolkit_getNotatedIdForElement(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func getOptions() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getOptions(tk) else { return "" }
        return String(cString: cStr)
    }

    public func getOptionUsageString() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getOptionUsageString(tk) else { return "" }
        return String(cString: cStr)
    }
    
    public func getPageCount() -> Int {
        guard let tk = toolkitPtr else { return 0 }
        return Int(vrvToolkit_getPageCount(tk))
    }

    public func getPageWithElement(_ xmlId: String) -> Int {
        guard let tk = toolkitPtr else { return 0 }
        return xmlId.withCString { cStr in
            Int(vrvToolkit_getPageWithElement(tk, cStr))
        }
    }

    public func getResourcePath() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getResourcePath(tk) else { return "" }
        return String(cString: cStr)
    }

    public func getScale() -> Int {
        guard let tk = toolkitPtr else { return 0 }
        return Int(vrvToolkit_getScale(tk))
    }

    public func getTimeForElement(_ xmlId: String) -> Double {
        guard let tk = toolkitPtr else { return 0 }
        return xmlId.withCString { cStr in
            vrvToolkit_getTimeForElement(tk, cStr)
        }
    }

    public func getTimesForElement(_ xmlId: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return xmlId.withCString { cStr in
            if let resultCStr = vrvToolkit_getTimesForElement(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }
    
    public func getVersion() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_getVersion(tk) else { return "" }
        return String(cString: cStr)
    }
    
    public func loadData(_ data: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return data.withCString { cStr in
            vrvToolkit_loadData(tk, cStr)
        }
    }

    public func loadFile(_ filename: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return filename.withCString { cStr in
            vrvToolkit_loadFile(tk, cStr)
        }
    }

    public func loadZipDataBase64(_ base64: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return base64.withCString { cStr in
            vrvToolkit_loadZipDataBase64(tk, cStr)
        }
    }
    
    public func redoLayout(_ jsonOptions: String) {
        guard let tk = toolkitPtr else { return }
        jsonOptions.withCString { cStr in
            vrvToolkit_redoLayout(tk, cStr)
        }
    }

    public func redoPagePitchPosLayout() {
        guard let tk = toolkitPtr else { return }
        vrvToolkit_redoPagePitchPosLayout(tk)
    }

    public func renderToExpansionMap() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_renderToExpansionMap(tk) else { return "" }
        return String(cString: cStr)
    }

    public func renderToExpansionMapFile(_ filename: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return filename.withCString { cStr in
            vrvToolkit_renderToExpansionMapFile(tk, cStr)
        }
    }

    public func renderToMIDI() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_renderToMIDI(tk) else { return "" }
        return String(cString: cStr)
    }

    public func renderToMIDIFile(_ filename: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return filename.withCString { cStr in
            vrvToolkit_renderToMIDIFile(tk, cStr)
        }
    }

    public func renderToPAE() -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_renderToPAE(tk) else { return "" }
        return String(cString: cStr)
    }

    public func renderToPAEFile(_ filename: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return filename.withCString { cStr in
            vrvToolkit_renderToPAEFile(tk, cStr)
        }
    }
    
    
    public func renderToSVG(_ pageNo: Int, _ xmlDeclaration: Bool) -> String {
        guard let tk = toolkitPtr else { return "" }
        guard let cStr = vrvToolkit_renderToSVG(tk, Int32(pageNo), xmlDeclaration) else { return "" }
        return String(cString: cStr)
    }
    
    public func renderToSVGFile(_ filename: String, _ pageNo: Int) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return filename.withCString { cStr in
            vrvToolkit_renderToSVGFile(tk, cStr, Int32(pageNo))
        }
    }

    public func renderToTimemap(_ options: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return options.withCString { cStr in
            if let resultCStr = vrvToolkit_renderToTimemap(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func renderToTimemapFile(_ filename: String, _ options: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return filename.withCString { fStr in
            options.withCString { oStr in
                vrvToolkit_renderToTimemapFile(tk, fStr, oStr)
            }
        }
    }

    public func resetOptions() {
        guard let tk = toolkitPtr else { return }
        vrvToolkit_resetOptions(tk)
    }

    public func resetXmlIdSeed(_ seed: Int) {
        guard let tk = toolkitPtr else { return }
        vrvToolkit_resetXmlIdSeed(tk, Int32(seed))
    }

    public func saveFile(_ filename: String, _ options: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return filename.withCString { fStr in
            options.withCString { oStr in
                vrvToolkit_saveFile(tk, fStr, oStr)
            }
        }
    }

    public func select(_ selection: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return selection.withCString { cStr in
            vrvToolkit_select(tk, cStr)
        }
    }

    public func setInputFrom(_ input: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return input.withCString { cStr in
            vrvToolkit_setInputFrom(tk, cStr)
        }
    }
    
    
    public func setOptions(_ jsonOptions: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return jsonOptions.withCString { cStr in
            vrvToolkit_setOptions(tk, cStr)
        }
    }

    public func setOutputTo(_ output: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return output.withCString { cStr in
            vrvToolkit_setOutputTo(tk, cStr)
        }
    }
    
    public func setResourcePath(_ path: String) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return path.withCString { cStr in
            vrvToolkit_setResourcePath(tk, cStr)
        }
    }

    public func setScale(_ scale: Int) -> Bool {
        guard let tk = toolkitPtr else { return false }
        return vrvToolkit_setScale(tk, Int32(scale))
    }

    public func validatePAE(_ data: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return data.withCString { cStr in
            if let resultCStr = vrvToolkit_validatePAE(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }

    public func validatePAEFile(_ filename: String) -> String {
        guard let tk = toolkitPtr else { return "" }
        return filename.withCString { cStr in
            if let resultCStr = vrvToolkit_validatePAEFile(tk, cStr) {
                return String(cString: resultCStr)
            } else {
                return ""
            }
        }
    }
}
