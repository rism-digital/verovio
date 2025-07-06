[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![PyPI](https://img.shields.io/pypi/v/verovio)](https://pypi.org/project/verovio/)
[![PyPI - Wheel](https://img.shields.io/pypi/wheel/verovio)](https://pypi.org/project/verovio/)
[![AppVeyor status](https://ci.appveyor.com/api/projects/status/121cxhmtwurxffh0?svg=true)](https://ci.appveyor.com/project/LaurentPugin/verovio-r1t6l)
[![GH Actions status](https://github.com/rism-digital/verovio/workflows/Verovio%20CI%20Build/badge.svg)](https://github.com/rism-digital/verovio/actions?query=workflow%3A%22Verovio+CI+Build%22)
[![PyPI - Downlaods](https://img.shields.io/pypi/dm/verovio?label=PyPI%20downloads)](https://pypi.org/project/verovio/)
[![NPM - Downlaods](https://img.shields.io/npm/dm/verovio?label=NPM%20-%20downloads)](https://www.npmjs.com/package/verovio)
[![DOI](https://zenodo.org/badge/15762693.svg)](https://zenodo.org/doi/10.5281/zenodo.10544792)
[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/rism-digital/verovio)

Verovio is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) digital scores into SVG images. Verovio also contains on-the-fly converters to render [Plaine & Easie Code](https://www.iaml.info/plaine-easie-code), [Humdrum](https://www.humdrum.org), [Musedata](https://musedata.org), [MusicXML](https://www.musicxml.com), [EsAC](http://esac-data.org), and [ABC](https://en.wikipedia.org/wiki/ABC_notation) digital scores. 

Verovio is written in standard 2020 C++ and can be compiled as a standalone command-line tool, used as a compiled music-rendering library for applications (Qt, python), or compiled into Javascript using the Emscripten LLVM-to-JavaScript compiler.  Check out the JavaScript toolkit version of verovio running in the [MEI Viewer](http://www.verovio.org/mei-viewer.xhtml) as well as the [app](http://www.verovio.org/app.html) or [tutorials](https://book.verovio.org/first-steps/) for web integration and user interaction.

![Choice interaction](https://raw.githubusercontent.com/rism-digital/verovio.org/gh-pages/movies/reflow.gif)


Verovio uses the [Standard Music Font Layout (SMuFL)](http://www.smufl.org) specification and the font can be changed for personalizing the output.

The project page is https://www.verovio.org. Verovio is available under the LGPL license (see [COPYING](https://github.com/rism-digital/verovio/blob/develop/COPYING) and [COPYING.LESSER](https://github.com/rism-digital/verovio/blob/develop/COPYING.LESSER)).

Building and use instructions by environment
--------------------------------------------

See the [Reference book](https://book.verovio.org/installing-or-building-from-sources/command-line.html)

LibMEI
------

The code for the attribute classes of Verovio are generated from the MEI schema using a modified version of LibMEI available [here](https://github.com/rism-digital/libmei). The code generated is included in the Verovio repository and the LibMEI repository does not need to be cloned for building Verovio.


Versioning
----------

As of 2025, major release cycles are increased once per year, with version 5.x.x for 2025. Verovio independently switches from a release version to a development version of MEI, whenever appropriate. This always corresponds to a minor Verovio release and is indicated in the release notes.

Embedded libraries
---------------

The following libraries are embedded in Verovio:

| library | purpose |
| ------- | ------- |
| [humlib](https://github.com/craigsapp/humlib)       | Humdrum file import/export |
| [JSON++](https://github.com/hjiang/jsonxx)          | JSON data parser           |
| [MidiFile](https://github.com/craigsapp/midifile)   | Standard MIDI file export  |
| [pugixml](https://pugixml.org/)                     | XML data parser            |
| [MINIZ-CPP](https://github.com/kkzi/miniz-cpp)      | ZIP files reading/writing  |

Contributing
------------

See the [Reference book](https://book.verovio.org/contributing/guidelines.html)

Example output
--------------

The sample page of music shown below was generated with version 2.4.0-dev-2748fed

![Example page](https://raw.githubusercontent.com/rism-digital/verovio.org/gh-pages/images/verovio-exp-2.4.0-dev.png)
