export default [
    {
        input: "src/index.js",
        output: [
            {
                file: "dist/verovio.mjs",
                format: "es",
                name: "verovio",
            },
            {
                file: "dist/verovio.cjs",
                format: "cjs",
                name: "verovio",
            },
        ],
    },
];
