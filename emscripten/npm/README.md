This is an NPM of the stable version of the Verovio JavaScript toolkit.

Verovio is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) music scores into SVG.

See it running in the [MEI Viewer](http://www.verovio.org/mei-viewer.xhtml) and check out the [tutorial](http://www.verovio.org/tutorial.xhtml) for its web integration and for enabling user interaction.

## Basic usage

```js
const verovio = require('verovio');
const fs = require('fs');

/* Wait for verovio to load */
verovio.module.onRuntimeInitialized = function ()
{
   // create the toolkit instance
   const vrvToolkit = new verovio.toolkit();
   // read the MEI file
   mei = fs.readFileSync('hello.mei');
   // load the MEI data as string into the toolkit
   vrvToolkit.loadData(mei.toString());
   // render the fist page as SVG
   svg = vrvToolkit.renderToSVG(1, {});
   // save the SVG into a file
   fs.writeFileSync('hello.svg', svg);
}
```

## Usage with ESM

This package also provides an ESM version which can be used with a modularized build of the Verovio module. Use `.mjs` as file extension when using this directly in Node.js or set `"type": "module"` in your `package.json`.

```js
import createVerovioModule from 'verovio/wasm';
import { VerovioToolkit } from 'verovio/esm';
import fs from 'node:fs';

createVerovioModule().then(VerovioModule => {
   const verovioToolkit = new VerovioToolkit(VerovioModule);
   const score = fs.readFileSync('hello.mei').toString();
   verovioToolkit.loadData(score);
   const data = verovioToolkit.renderToSVG(1);
   console.log(data);
});
```

This is the recommended way to use Verovio when creating a website or web app with bundlers like webpack or Vite or when using JavaScript frameworks like React or Vue.js.

## Usage with CommonJS

Alternatively this package also exports a version compatible with CommonJS

```js
const createVerovioModule = require('verovio/wasm');
const { VerovioToolkit } = require('verovio/esm');
```

## Humdrum support

Since version 3.11.0 the npm package provides an additional module with Humdrum support:

```js
import createVerovioModule from 'verovio/wasm-hum';
```

