This is an NPM of the stable version of the Verovio JavaScript toolkit with Humdrum support.

[Verovio](https://www.verovio.org/) is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) music scores into SVG.

This version of the toolkit includes Humdrum support. See it running in the [Verovio Humdrum Viewer](http://verovio.humdrum.org/).

## Usage

```javascript
var verovio = require( 'verovio-humdrum' );
var fs = require( 'fs' );

/* Wait for verovio to load */
verovio.module.onRuntimeInitialized = function ()
{
    /* create the toolkit instance */
    var vrvToolkit = new verovio.toolkit();
    /* read the Humdrum file */
	krn = fs.readFileSync("hello.krn");
    /* load the Humdrum data as string into the toolkit */
	vrvToolkit.loadData(krn.toString());
    /* render the fist page as SVG */
	svg = vrvToolkit.renderToSVG(1, {});
    /* save the SVG into a file */
	fs.writeFileSync("hello.svg", svg);
}
```