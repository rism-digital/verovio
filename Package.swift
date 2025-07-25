// swift-tools-version:5.9
import PackageDescription

let package = Package(
    name: "VerovioToolkit",
    platforms: [
        .iOS(.v16),
        .macOS(.v11),
    ],
    products: [
        .library(
            name: "VerovioToolkit",
            targets: ["VerovioToolkit"]
        ),
    ],
    targets: [
        .target(
            name: "VerovioToolkit",
            path: ".", // since sources are spread around root
            sources: [
                "src",
                "libmei/dist",
                "libmei/addons",
                "tools",
                "bindings/swift"
            ],
            publicHeadersPath: "bindings/swift", // needed for module map
            exclude: [
                "COPYING",
                "data", // data is handled separately
            ],
            resources: [
                .process("data")
            ],
            cxxSettings: [
                .headerSearchPath("include/crc"),
                .headerSearchPath("include/hum"),
                .headerSearchPath("include/json"),
                .headerSearchPath("include/midi"),
                .headerSearchPath("include/pugi"),
                .headerSearchPath("include/utf8"),
                .headerSearchPath("include/vrv"),
                .headerSearchPath("include/zip"),
                .headerSearchPath("libmei/dist"),
                .headerSearchPath("libmei/addons"),
                .headerSearchPath("bindings/swift"),
                .define("VEROVIO_LIBRARY"), // optional, if used in build
                .unsafeFlags(["-std=c++23"], .when(platforms: [.iOS, .macOS]))
            ]
        )
    ]
)
