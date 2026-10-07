import commonjs from "@rollup/plugin-commonjs";
import addModuleExports from "./src/build/rollup-plugin-add-module-exports.mjs";

export default [
    {
        input: "src/prebundle-module.js",
        output: [
            {
                file: "dist/verovio-toolkit-wasm.js",
                format: "umd",
                name: "verovio",
            },
        ],
        plugins: [
            addModuleExports(),
            commonjs({
                ignore: [
                    "path",
                    "fs",
                    "crypto",
                ],
            }),
        ],
    },
];
