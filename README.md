Verovio is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) music scores into SVG. Verovio can be used as a standalone command-line tool for converting a MEI files, as the music rendering library for applications, or it can be compiled to Javascript using the Emscripten LLVM-to-JavaScript compiler.

See it running in the [MEI Viewer](http://www.verovio.org/mei-viewer.xhtml) and check out the [tutorial](http://www.verovio.org/tutorial.xhtml) for its web integration.

Verovio can also render Plain and Easy (PAE) code and DARMS code and has a basic MusicXML import function. It is designed as a tool usable as a one-step conversion tool and it is written in pure standard C++ with no dependencies on third-party frameworks and libraries.

Verovio follows the [Standard Music Font Layout (SMuFL)](http://www.smufl.org) specification and the font can be changed for personalizing the output.

The project page is http://www.verovio.org. Verovio is available under the LGPL license (see LICENSE.txt).

LibMEI
------

The code for the attribute classes of Verovio are generated from the MEI schema using a modified version of LibMEI available [here](https://github.com/rism-ch/libmei). The code generated is included in the Verovio repository and the LibMEI repository does not need to be cloned for building Verovio.

Coding style
------------

The Verovio coding style is available [here](doc/guidelines.md).

Example output
--------------

This page was generated with version 0.9.12-dev-03f3615

![Example page](/images/page-0.9.12-dev-03f3615.png)
