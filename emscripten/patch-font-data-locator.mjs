#!/usr/bin/env node

import fs from "node:fs";

const [file, mode] = process.argv.slice(2);
if (!file || (mode !== "classic" && mode !== "module")) {
    throw new Error("usage: patch-font-data-locator.mjs FILE classic|module");
}

const source = fs.readFileSync(file, "utf8");
const marker = mode === "module"
    ? /var Module\s*=\s*moduleArg\s*;/g
    : /var Module\s*=\s*typeof Module\s*!==?\s*["']undefined["']\s*\?\s*Module\s*:\s*\{\}\s*;/g;
const matches = [...source.matchAll(marker)];
if (matches.length !== 1) {
    throw new Error(`expected one Emscripten Module initializer, found ${matches.length}`);
}

const locator = mode === "module"
    ? `if(!Module["locateFile"])Module["locateFile"]=(path)=>new URL(path,import.meta.url);`
    : `var verovioDataScriptUrl=typeof document==="object"&&document.currentScript?document.currentScript.src:"";if(!Module["locateFile"])Module["locateFile"]=(path,prefix)=>typeof __dirname!=="undefined"?__dirname+"/"+path:verovioDataScriptUrl?new URL(path,verovioDataScriptUrl).href:prefix+path;`;
const output = source.replace(marker, match => match + locator);
fs.writeFileSync(file, output);
