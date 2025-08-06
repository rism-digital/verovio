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

Verovio is written in standard C++20 and is available in several bindings (JavaScript, Python, Java, Swift). It
can be compiled as a standalone command-line tool or as a music-rendering library for applications.  Check out the JavaScript toolkit version of Verovio running in the [Verovio Online Editor / Viewer](http://editor.verovio.org), or the [tutorials](https://book.verovio.org/first-steps/) to know more about web integration and user interaction.

![Choice interaction](https://raw.githubusercontent.com/rism-digital/verovio.org/gh-pages/movies/reflow.gif)

### Minimal sample applications

| C++ with Qt | Java binding on Android | Swift binding on iOS |
| --- | --- | --- |
| [Code repository](https://github.com/rism-digital/verovio-qt-demo) | [Code repository](https://github.com/rism-digital/verovio-android-demo) - [GooglePlay](https://play.google.com/store/apps/details?id=org.verovio.android.demo) | [AppStore](https://apps.apple.com/app/verovio-mei-viewer/id6747756332) |
| ![iPad](https://raw.githubusercontent.com/rism-digital/verovio.org/gh-pages/images/qt.png)| ![iPad](https://raw.githubusercontent.com/rism-digital/verovio.org/gh-pages/images/android.png) | ![iPad](https://raw.githubusercontent.com/rism-digital/verovio.org/gh-pages/images/ios.png) |

Verovio uses the [Standard Music Font Layout (SMuFL)](http://www.smufl.org) specification and the font can be changed for personalizing the output.

### License

Verovio is available under the LGPL license (see [COPYING](https://github.com/rism-digital/verovio/blob/develop/COPYING) and [COPYING.LESSER](https://github.com/rism-digital/verovio/blob/develop/COPYING.LESSER)).

## Building and use instructions by environment

See the [Reference book](https://book.verovio.org/installing-or-building-from-sources/command-line.html)

## Libraries

### LibMEI

The code for the attribute classes of Verovio are generated from the MEI schema using a modified version of LibMEI available [here](https://github.com/rism-digital/libmei). The code generated is included in the Verovio repository and the LibMEI repository does not need to be cloned for building Verovio.

### Embedded libraries

The following libraries are embedded in Verovio:

| library | purpose |
| ------- | ------- |
| [humlib](https://github.com/craigsapp/humlib)       | Humdrum file import/export |
| [JSON++](https://github.com/hjiang/jsonxx)          | JSON data parser           |
| [MidiFile](https://github.com/craigsapp/midifile)   | Standard MIDI file export  |
| [pugixml](https://pugixml.org/)                     | XML data parser            |
| [MINIZ-CPP](https://github.com/kkzi/miniz-cpp)      | ZIP files reading/writing  |

## Contributing

See the [Reference book](https://book.verovio.org/contributing/guidelines.html)
