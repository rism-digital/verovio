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
            path: "vendor/brotli/c",
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
                "vendor/harfbuzz/src/verovio-harfbuzz.cc",
                "vendor/woff2/src/verovio-woff2.cc"
            ],
            publicHeadersPath: "bindings/swift-core",
            cxxSettings: [
                .headerSearchPath("include/crc"),
                .headerSearchPath("include/hum"),
                .headerSearchPath("include/json"),
                .headerSearchPath("include/midi"),
                .headerSearchPath("include/pugi"),
                .headerSearchPath("include/tuning-library"),
                .headerSearchPath("include/utf8"),
                .headerSearchPath("include/vrv"),
                .headerSearchPath("include/zip"),
                .headerSearchPath("libmei/dist"),
                .headerSearchPath("libmei/addons"),
                .headerSearchPath("vendor/harfbuzz/src"),
                .headerSearchPath("vendor/woff2/include"),
                .headerSearchPath("vendor/woff2/src"),
                .headerSearchPath("vendor/brotli/c/include"),
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
            dependencies: ["VerovioToolkit"],
            path: "bindings/swift-toolkit-tests"
        )
    ]
)
