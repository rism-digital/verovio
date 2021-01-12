[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![AppVeyor status](https://ci.appveyor.com/api/projects/status/121cxhmtwurxffh0?svg=true)](https://ci.appveyor.com/project/LaurentPugin/verovio-r1t6l)
[![GH Actions status](https://github.com/rism-ch/verovio/workflows/Verovio%20CI%20Build/badge.svg)](https://github.com/rism-ch/verovio/actions?query=workflow%3A%22Verovio+CI+Build%22)

Verovio is a fast, portable and lightweight library for engraving [Music Encoding Initiative (MEI)](http://www.music-encoding.org) digital scores into SVG images. Verovio also contains on-the-fly converters to render [Plaine & Easie Code](https://www.iaml.info/plaine-easie-code), [Humdrum](https://www.humdrum.org), [Musedata](https://musedata.org), [MusicXML](https://www.musicxml.com), [EsAC](http://esac-data.org), and [ABC](https://en.wikipedia.org/wiki/ABC_notation) digital scores. 

Verovio is written in standard 2017 C++ and can be compiled as a standalone command-line tool, used as a compiled music-rendering library for applications (Qt, python), or compiled into Javascript using the Emscripten LLVM-to-JavaScript compiler.  Check out the JavaScript toolkit version of verovio running in the [MEI Viewer](http://www.verovio.org/mei-viewer.xhtml) as well as the [app](http://www.verovio.org/app.html) or [tutorials](http://www.verovio.org/tutorial.xhtml) for web integration and user interaction.

![Choice interaction](https://raw.githubusercontent.com/rism-ch/verovio.org/gh-pages/movies/reflow.gif)


Verovio uses the [Standard Music Font Layout (SMuFL)](http://www.smufl.org) specification and the font can be changed for personalizing the output.

The project page is http://www.verovio.org. Verovio is available under the LGPL license (see [LICENSE.txt](https://github.com/rism-ch/verovio/blob/develop/LICENSE.txt)).

Building and use instructions by environment
--------------------------------------------

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

Other libraries
---------------

The following libraries are embedded in Verovio:

| library | purpose |
| ------- | ------- |
| [humlib](https://github.com/craigsapp/humlib)       | Humdrum file import/export |
| [JSON++](https://github.com/hjiang/jsonxx)          | JSON data parser           |
| [MidiFile](https://github.com/craigsapp/midifile)   | Standard MIDI file export  |
| [pugixml](https://pugixml.org/)                     | XML data parser            |
| [UTF-CPP](http://utfcpp.sourceforge.net/)           | UTF character conversions  |

Contributing
------------

If you are interested in contributing to Verovio, please read the [contribution](doc/contributing.md) and [coding style](doc/guidelines.md) guidelines.

Example output
--------------

The sample page of music shown below was generated with version 2.4.0-dev-2748fed

![Example page](https://raw.githubusercontent.com/rism-ch/verovio.org/gh-pages/images/verovio-exp-2.4.0-dev.png)

Example resources using verovio
-------------------------------

| name | type  | description |
| ---- | ----- | ----------- |
| [Verovio Humdrum Viewer](https://verovio.humdrum.org) | editor | An online semi-graphical Humdrum data editor (can also be used to textually edit other digital scores compliant with verovio). |
| [MoVI](https://dme.mozarteum.at/movi/en) | repertory | The digital Mozart digital score VIewer at the [Mozarteum](https://mozarteum.at) |
| [Tasso in Music Project](https://www.tassomusic.org/work/?id=Trm0047m) | repertory | Musical settings of the poetry of [Torquato Tasso](https://en.wikipedia.org/wiki/Torquato_Tasso)
| [Measuring Polyphony](https://measuringpolyphony.org/display.html?/assets/mensural/325_MENSURAL.mei) | repertory | Late medieval music in black mensural and modern notations |
| [Probstücke Digital](http://probstuecke-digital.de) | repertory | open and critical digital edition of Mattheson's test pieces |
| [370 Bach Chorales](https://chorales.sapp.org) | repertory | Online edition of Bach chorales, including an interactive [typesetter page](https://chorales.sapp.org/typesetter) that allows for creating musical examples for online display or use in papers. |
| [Humdrum Notation Plugin](https://plugin.humdrum.org) | tool | Javascript interface to verovio for displaying multiple musical examples on a webpage |
| [Music Sheet Viewer](http://www.partitionnumerique.com/music-sheet-viewer-wordpress-plugin) | tool | WordPress plugin for displaying graphical music from MEI data | 


Digital score repositories on Github
------------------------------------

Here is a list of digital score repositories on Github that can be displayed with verovio:


| link | encoding | description |
| ---- | -------- | ----------- | 
|  [MEI complete examples](https://github.com/music-encoding/sample-encodings/tree/master/MEI_4.0/Music/Complete_examples) | MEI | 86 various works encoded in MEI |
| [Mozart Piano Sonatas](https://github.com/craigsapp/mozart-piano-sonatas) | Humdrum | 17 Piano sonatas by W.A. Mozart from the Alte Mozart-Ausgabe ([in VHV](http://verovio.humdrum.org/?file=mozart/sonatas&k=e)) |
| [Beethoven Piano Sonatas](https://github.com/craigsapp/beethoven-piano-sonatas) | Humdrum | 32 Piano sonatas by L. van Beethoven, edited by Paul Dukas ([in VHV](http://verovio.humdrum.org/?file=beethoven/sonatas&k=e)) |
| [Josquin Research Project](https://github.com/josquin-research-project/jrp-scores) | Humdrum | Over 1000 scores of early Renaissance music in modern editions ([website](https://josquin.stanford.edu)) |
| [Tasso in Music Project](https://github.com/TassoInMusicProject/tasso-scores) | Humdrum | Critical edition of 650 Late Renaissance madrigals using the poetry of Torquato Tasso for lyrics. ([website](https://www.tassomusic.org)) |
| [Music of Scott Joplin](https://github.com/craigsapp/joplin) | Humdrum | Digital scores of most of Scott Joplins music |
| [Chopin mazurkas](https://github.com/craigsapp/chopin-mazurkas) | Humdrum | Digital scores of Chopin's mazurkas |
| [Chopin preludes](https://github.com/craigsapp/chopin-preludes) | Humdrum | Digital scores of Chopin's op. 24 preludes |
| [J.N. Hummel preludes, op. 67](https://github.com/craigsapp/hummel-preludes) | Humdrum | 24 improvisatory prelude examples in every key |
| [370 Bach chorales](https://github.com/craigsapp/bach-370-chorales) | Humdrum | Chorales collected by C.P.E. Bach after his father's death ([website](https://chorales.sapp.org)) |
| [Deutscher Liederschatz](https://github.com/craigsapp/erk-liederschatz) | Humdrum | 200 harmonized songs from vol. 1, edited by Ludwig Erk |
| [Beethoven string quartets](https://github.com/craigsapp/beethoven-string-quartets) | Humdrum | 18 string quartets by Ludwig van Beethoven |









