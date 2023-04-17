const targets = [
    "verovio.js",
];

function appendModuleExports(string) {
    return `${string}
    module.exports = Module;`;
}

export default function addModuleExports() {
    return {
        name: "add-module-exports",
        transform(source, id) {
            if (!targets.some(s => id.endsWith(s))) {
                return;
            }
            return {
                code: appendModuleExports(source),
                map: null,
            };
        },
    };
}
