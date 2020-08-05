[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![Build Status](https://travis-ci.org/rism-ch/verovio.svg?branch=develop)](https://travis-ci.org/rism-ch/verovio)
[![Build status](https://ci.appveyor.com/api/projects/status/121cxhmtwurxffh0?svg=true)](https://ci.appveyor.com/project/LaurentPugin/verovio-r1t6l)

Verovio is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) digital scores into SVG images. Verovio also contains on-the-fly converters to render [Plaine & Easie Code](https://www.iaml.info/plaine-easie-code), [Humdrum](https://www.humdrum.org), [Musedata](https://musedata.org), [MusicXML](https://www.musicxml.com), [EsAC](http://esac-data.org), and [ABC](https://en.wikipedia.org/wiki/ABC_notation) digital scores. 

Verovio is written in standard 2017 C++.   Verovio can be compiled as a standalone command-line tool, used as a compiled music-rendering library for applications (Qt, python), or it can be compiled to Javascript using the Emscripten LLVM-to-JavaScript compiler.  See the JavaScript toolkit running in the [MEI Viewer](http://www.verovio.org/mei-viewer.xhtml) and check out the [app](http://www.verovio.org/app.html) or the [tutorial](http://www.verovio.org/tutorial.xhtml) for its web integration and for enabling user interaction.

![Choice interaction](https://raw.githubusercontent.com/rism-ch/verovio.org/gh-pages/movies/reflow.gif)


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

Major releases of Verovio and MEI versions:
* Verovio 1.x.x ⇔ MEI 3.0
* Verovio 2.x.x ⇔ MEI 4.0
* Verovio 3.x.x ⇔ Development of MEI since 4.0

From Verovio 2.x.x, the plan is to have even version numbers for Verovio releases using a stable version of MEI, and odd version numbers for releases using a development version of MEI. It means that once MEI 5.0 will be released, Verovio will move to version 4.x.x. Older versions of MEI are still supported by newer versions of Verovio. MEI files are internally upgraded when loaded into Verovio. This applies only to the features supported by Verovio. We will try to maintain this in the future.

Contributing
------------

If you are willing to contribute to Verovio, please follow the [contribution](doc/contributing.md) and [coding style](doc/guidelines.md) guidelines.

Example output
--------------

This page was generated with version 2.4.0-dev-2748fed

![Example page](https://raw.githubusercontent.com/rism-ch/verovio.org/gh-pages/images/verovio-exp-2.4.0-dev.png)

Used libraries
--------------

The following libraries are embedded in Verovio:
| library | purpose |
| ------- | ------- |
| [humlib](https://github.com/craigsapp/humlib)       | Humdrum file import/export |
| [JSON++](https://github.com/hjiang/jsonxx)          | JSON data parser           |
| [MidiFile](https://github.com/craigsapp/midifile)   | Standard MIDI file export  |
| [pugixml](https://pugixml.org/)                     | XML data parser            |
| [UTF-CPP](http://utfcpp.sourceforge.net/)           | UTF character conversions  |
