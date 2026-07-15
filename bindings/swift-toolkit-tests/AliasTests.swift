import Foundation
import XCTest
@testable import VerovioToolkit

final class AliasTests: XCTestCase {
    func testRuntimeFontAliases() throws {
        let root = URL(fileURLWithPath: #filePath)
            .deletingLastPathComponent()
            .deletingLastPathComponent()
            .deletingLastPathComponent()
        let tests = root.appending(path: "tests/font-runtime")
        let toolkit = VerovioToolkit(root.appending(path: "data").path)
        let textFamily = toolkit.registerTextFont(
            try Data(contentsOf: tests.appending(path: "VerovioTestLigature.ttf")), alias: "QS")
        let musicFamily = toolkit.registerMusicFont(
            try Data(contentsOf: tests.appending(path: "VerovioTestMusic.ttf")),
            smuflMetadataJSON: try String(contentsOf: tests.appending(path: "VerovioTestMusic_metadata.json")),
            alias: "VM")
        XCTAssertEqual(textFamily, "Verovio Test Ligature")
        XCTAssertEqual(musicFamily, "Verovio Test Music")
        XCTAssertTrue(toolkit.loadData(try String(contentsOf: tests.appending(path: "alias-dir.mei"))))
        let svg = toolkit.renderToSVG(1, false)
        XCTAssertFalse(svg.contains("<text"))
        XCTAssertTrue(svg.contains("font-family=\"QS\""))
        XCTAssertTrue(svg.contains("font-family=\"VM\""))
        XCTAssertTrue(svg.contains("text-8BFEB250B0FDDA0E-4-"))
    }
}
