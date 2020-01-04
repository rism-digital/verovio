[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![Build Status](https://travis-ci.org/rism-ch/verovio.svg?branch=develop)](https://travis-ci.org/rism-ch/verovio)
[![Build status](https://ci.appveyor.com/api/projects/status/121cxhmtwurxffh0?svg=true)](https://ci.appveyor.com/project/LaurentPugin/verovio-r1t6l)

Verovio is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) music scores into SVG. Verovio can be used as a standalone command-line tool for converting a MEI files, as the music rendering library for applications, or it can be compiled to Javascript using the Emscripten LLVM-to-JavaScript compiler.

See the JavaScript toolkit running in the [MEI Viewer](http://www.verovio.org/mei-viewer.xhtml) and check out the [app](http://www.verovio.org/app.html) or the [tutorial](http://www.verovio.org/tutorial.xhtml) for its web integration and for enabling user interaction.

![Choice interaction](https://raw.githubusercontent.com/rism-ch/verovio/gh-pages/movies/reflow.gif)

Verovio can also render [Plaine & Easie Code](https://www.iaml.info/plaine-easie-code) and imports MusicXML, Humdrum data, and ABC notation. It is designed as a tool usable as a one-step conversion tool and it is written in pure standard C++.

Verovio follows the [Standard Music Font Layout (SMuFL)](http://www.smufl.org) specification and the font can be changed for personalizing the output.

The project page is http://www.verovio.org. Verovio is available under the LGPL license (see LICENSE.txt).

Building and using instructions
-------------------------------

See the [wiki](https://github.com/rism-ch/verovio/wiki/Building-instructions), with instructions for:
* [Command-line tool](https://github.com/rism-ch/verovio/wiki/Building-instructions#command-line-tool)
* [XCode](https://github.com/rism-ch/verovio/wiki/Building-instructions#xcode)
* [Visual Studio](https://github.com/rism-ch/verovio/wiki/Building-instructions#visual-studio)
* [JavaScript](https://github.com/rism-ch/verovio/wiki/Building-instructions#javascript-toolkit)
* [NPM](https://github.com/rism-ch/verovio/wiki/Building-instructions#npm)
* [Python](https://github.com/rism-ch/verovio/wiki/Building-instructions#building-the-python-toolkit)
* [Java](https://github.com/rism-ch/verovio/wiki/Building-instructions#building-the-java-toolkit)
* [CocoaPods](https://github.com/rism-ch/verovio/wiki/Building-instructions#cocoapods)


LibMEI
------

The code for the attribute classes of Verovio are generated from the MEI schema using a modified version of LibMEI available [here](https://github.com/rism-ch/libmei). The code generated is included in the Verovio repository and the LibMEI repository does not need to be cloned for building Verovio.

Contributing
------------

If you are willing to contribute to Verovio, please follow the [contribution](doc/contributing.md) and [coding style](doc/guidelines.md) guidelines.

Example output
--------------

This page was generated with version 2.4.0-dev-2748fed

![Example page](https://raw.githubusercontent.com/rism-ch/verovio/gh-pages/images/verovio-exp-2.4.0-dev.png)
