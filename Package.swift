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
            name: "VerovioCore",
            path: ".",
            sources: [
                "src",
                "libmei/dist",
                "libmei/addons",
                "tools/c_wrapper.cpp"
            ],
            publicHeadersPath: "bindings/swift-core",
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
