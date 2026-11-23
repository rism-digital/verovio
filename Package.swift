// swift-tools-version:5.9
import PackageDescription

let package = Package(
    name: "VerovioToolkit",
    platforms: [
        .iOS(.v16),
        .macOS(.v11)
    ],
    products: [
        .library(
            name: "VerovioToolkit",
            targets: ["VerovioToolkit"]
        )
    ],
    targets: [
        .target(
            name: "VerovioBrotli",
            path: "external/brotli/c",
            sources: ["verovio-brotli.c"],
            publicHeadersPath: "swift-include",
            cSettings: [
                .headerSearchPath("include"),
                .define("BROTLI_STATIC")
            ]
        ),
        .target(
            name: "VerovioCore",
            dependencies: ["VerovioBrotli"],
            path: ".",
            exclude: ["src/smufl_names.inc"],
            sources: [
                "src",
                "libmei/dist",
                "libmei/addons",
                "tools/c_wrapper.cpp",
                "external/crc/crc.cpp",
                "external/jsonxx/jsonxx.cc",
                "external/midifile/src",
                "external/pugixml/src/pugixml.cpp",
                "external/harfbuzz/src/verovio-harfbuzz.cc",
                "external/woff2/src/verovio-woff2.cc"
            ],
            publicHeadersPath: "bindings/swift-core",
            cxxSettings: [
                .headerSearchPath("include/hum"),
                .headerSearchPath("include/utf8"),
                .headerSearchPath("include/vrv"),
                .headerSearchPath("libmei/dist"),
                .headerSearchPath("libmei/addons"),
                .headerSearchPath("external/crc"),
                .headerSearchPath("external/jsonxx"),
                .headerSearchPath("external/midifile/include"),
                .headerSearchPath("external/miniz-cpp"),
                .headerSearchPath("external/pugixml/src"),
                .headerSearchPath("external/tuning-library/include"),
                .headerSearchPath("external/harfbuzz/src"),
                .headerSearchPath("external/woff2/include"),
                .headerSearchPath("external/woff2/src"),
                .headerSearchPath("external/brotli/c/include"),
                .define("BROTLI_STATIC"),
                .define("HB_LEAN"),
                .define("HB_MINI"),
                .define("HB_OPTIMIZE_SIZE_MORE"),
                .define("HB_CONFIG_OVERRIDE_H", to: "\"verovio-hb-config.h\""),
                .unsafeFlags(["-std=c++23"])
            ]
        ),
        .target(
            name: "VerovioToolkit",
            dependencies: ["VerovioCore"],
            path: ".",
            sources: ["bindings/swift-toolkit"],
            resources: [.copy("data")]
        ),
        .testTarget(
            name: "VerovioToolkitTests",
            dependencies: ["VerovioToolkit"]
        )
    ]
)
