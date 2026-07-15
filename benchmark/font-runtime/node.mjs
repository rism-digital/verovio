import fs from "node:fs";
import { pathToFileURL } from "node:url";
import { performance } from "node:perf_hooks";

function argument(name, fallback) {
    const index = process.argv.indexOf(`--${name}`);
    return index < 0 ? fallback : process.argv[index + 1];
}

const modulePath = argument("module");
const fixturePath = argument("fixture");
const iterations = Number(argument("iterations", "30"));
const warmups = Number(argument("warmups", "5"));
const warmBatch = Number(argument("warm-batch", "1000"));
if (!modulePath || !fixturePath || iterations < 1 || warmups < 0 || warmBatch < 1) {
    throw new Error(
        "Usage: node node.mjs --module verovio-module.mjs --fixture FILE "
        + "[--iterations 30] [--warmups 5] [--warm-batch 1000]",
    );
}

const { default: createVerovioModule } = await import(pathToFileURL(modulePath));
const { VerovioToolkit } = await import("../../emscripten/npm/src/verovio-toolkit.js");
const module = await createVerovioModule();
const mei = fs.readFileSync(fixturePath, "utf8");
const options = { breaks: "encoded", header: "none", footer: "none" };

function median(values) {
    const sorted = [...values].sort((a, b) => a - b);
    const middle = Math.floor(sorted.length / 2);
    return sorted.length % 2 ? sorted[middle] : (sorted[middle - 1] + sorted[middle]) / 2;
}

function prepare() {
    const toolkit = new VerovioToolkit(module);
    toolkit.setOptions(options);
    if (!toolkit.loadData(mei)) throw new Error(toolkit.getLog());
    return toolkit;
}

const cold = [];
const warm = [];
let svgBytes = 0;
for (let index = 0; index < iterations; index += 1) {
    const begin = performance.now();
    const toolkit = prepare();
    const svg = toolkit.renderToSVG(1);
    cold.push(performance.now() - begin);
    svgBytes = Buffer.byteLength(svg);
    toolkit.destroy();
}

const toolkit = prepare();
for (let index = 0; index < warmups; index += 1) toolkit.renderToSVG(1);
for (let index = 0; index < iterations; index += 1) {
    const begin = performance.now();
    let svg;
    for (let batch = 0; batch < warmBatch; batch += 1) svg = toolkit.renderToSVG(1);
    warm.push((performance.now() - begin) / warmBatch);
    svgBytes = Buffer.byteLength(svg);
}
toolkit.destroy();

process.stdout.write(`${JSON.stringify({
    fixture: fixturePath,
    iterations,
    warm_batch: warmBatch,
    cold_ms: { median: median(cold), samples: cold },
    warm_ms: { median: median(warm), samples: warm },
    svg_bytes: svgBytes,
})}\n`);
