import fs from "node:fs";
import { pathToFileURL } from "node:url";

import { VerovioToolkit } from "../../emscripten/npm/src/verovio-toolkit.js";

function argument(name) {
    const index = process.argv.indexOf(`--${name}`);
    return index < 0 ? undefined : process.argv[index + 1];
}

const modulePath = argument("module");
const textFontPath = argument("text-font");
const musicFontPath = argument("music-font");
const musicMetadataPath = argument("music-metadata");
const fixturePath = argument("fixture");
if (!modulePath || !textFontPath || !musicFontPath || !musicMetadataPath || !fixturePath) {
    throw new Error("Usage: node alias-smoke.mjs --module MODULE --text-font FONT --music-font FONT "
        + "--music-metadata JSON --fixture MEI");
}

const { default: createVerovioModule } = await import(pathToFileURL(modulePath));
const module = await createVerovioModule();
const toolkit = new VerovioToolkit(module);

try {
    const textFamily = toolkit.registerTextFont(new Uint8Array(fs.readFileSync(textFontPath)), "QS");
    const musicFamily = toolkit.registerMusicFont(
        new Uint8Array(fs.readFileSync(musicFontPath)), fs.readFileSync(musicMetadataPath, "utf8"), "VM");
    if (textFamily !== "Verovio Test Ligature" || musicFamily !== "Verovio Test Music") {
        throw new Error(`Unexpected canonical families: ${textFamily}, ${musicFamily}`);
    }
    if (!toolkit.loadData(fs.readFileSync(fixturePath, "utf8"))) throw new Error(toolkit.getLog());
    const svg = toolkit.renderToSVG(1);
    if (svg.includes("<text") || !svg.includes('font-family="QS"') || !svg.includes('font-family="VM"')
        || !svg.includes("text-8BFEB250B0FDDA0E-4-")) {
        throw new Error("The aliased text/music faces were not used in the rendered SVG");
    }
}
finally {
    toolkit.destroy();
}
