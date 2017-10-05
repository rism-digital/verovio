This is an NPM of the development version of the Verovio JavaScript toolkit.

Verovio is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) music scores into SVG.

See it running in the [MEI Viewer](http://www.verovio.org/mei-viewer.xhtml) and check out the [tutorial](http://www.verovio.org/tutorial.xhtml) for its web integration and for enabling user interaction.

## Usage

```javascript
var verovio = require('verovio-dev');

var vrvToolkit = new verovio.toolkit();

mei = fs.readFileSync("hello.mei");

vrvToolkit.loadData(mei.toString());

svg = vrvToolkit.renderPage(1, {});

fs.writeFileSync("hello.svg", svg);
```
