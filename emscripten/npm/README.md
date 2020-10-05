This is an NPM of the stable version of the Verovio JavaScript toolkit.

Verovio is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) music scores into SVG.

See it running in the [MEI Viewer](http://www.verovio.org/mei-viewer.xhtml) and check out the [tutorial](http://www.verovio.org/tutorial.xhtml) for its web integration and for enabling user interaction.

## Usage

```javascript
var verovio = require( 'verovio' );
var fs = require( 'fs' );

/* Wait for verovio to load */
verovio.module.onRuntimeInitialized = function ()
{
    /* create the toolkit instance */
    var vrvToolkit = new verovio.toolkit();
    /* read the MEI file */
	mei = fs.readFileSync("hello.mei");
    /* load the MEI data as string into the toolkit */
	vrvToolkit.loadData(mei.toString());
    /* render the fist page as SVG */
	svg = vrvToolkit.renderToSVG(1, {});
    /* save the SVG into a file */
	fs.writeFileSync("hello.svg", svg);
}
```

### Memory allocation

The memory available for Verovio can be increased during import by passing a MB value to the `init` function.

```javascript
var verovio = require('verovio').init(512);
```

The value has to be less than 1024MB.
