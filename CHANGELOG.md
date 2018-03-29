# Changelog

## [Unreleased]
### Added
* Additional options for controlling grace notes alignment
* Support for `<instrDef>` (@rettinghaus)
* Support for `mnum.visible` on `<scoreDef>` (@rettinghaus)
* Implementation of `<mNum>` and generation from `measure@n` if necessary (@rettinghaus)
* Support for mulitple lines or `<harm>` according to `@n` value
* Adjustment of `<hairpin>` length with surrounding `<dynam>` or `<hairpin>`
* Automatic vertical alignment of `<dynam>` and `<haripin>` starting / ending at the same xml:id or tstamp
* Support for `@vgrp` on `<dynam>` and `<hairpin>`
* Preservation of unsupported attributes in the MEI output and in JS getElementAttr
* Support for `<ref>` elements (preserved in the SVG but with no predefined behaviour)
* Support for chords with mixed durations
* Adding Travis CI (Linux and OS X)
* Preservation of the full `<body>` subtree (`<mdiv>`, `<score>`) in the MEI output
* Allowing multiple JS instances
* Adding support for `@metcon` on measure
* Adding getAvailableOptions and getOptions(bool) methods to the JS toolkit
* Adding several layout options
* Support for `<pgHead>`, `<pgHead2>`, `<pgFoot>` and `<pgFoot2>`
* Support for `<lb>` within `<rend>`
* Generation of header and footer form the MEI header when none provided
* Support for `<fig>` and `<svg>`
* Distribution as NPM package
* Adding Qt binding (@yeonoson)
* Support for `<arpeg>`
* Adding mmOutput option for better PDF generation
* Support for trill extensions
* Upgrade to MEI 4.0 (in preparation)

### Changed
* Method renderPage (JS) deprecated (use renderToSVG instead)
* Method renderToMidi (JS) deprecated (use renderToMIDI instead)
* Option --appXPathQueries (JS) deprecated (use appXPathQuery instead)
* Option --choiceXPathQueries (JS) deprecated (use choiceXPathQuery instead)
* Output CLI help and version to standard output
* Option -b/--border (CLI) and border (JS) deprecated
* Option -h/--height and -w/--width (CLI) deprecated
* Option --ignore-layout (CLI) and ignoreLayout deprecated (use breaks instead)
* Option --no-layout (CLI) and noLayout deprecated (use breaks instead)
* Handling `@tie` on `<note>` as `<tie>` internally
* Handling `@fermata` as `<fermata>` internally

## [1.1.6] - 2017-07-11
* Fix for Plaine & Easie ties
* Fix for Plaine & Easie mensural incipits

## [1.1.5] - 2017-07-03
* Fix for Plaine & Easie grace notes (@rettinghaus)

## [1.1.4] - 2017-06-26
* Improved stem direction for grace notes (back-ported)
* Improved stem direction with multiple layers (back-ported)
* Improved shared accidental rendering (back-ported)
* Fix for missing slash with grace notes (back-ported)

## [1.1.3] - 2017-06-20
* Fix missing slash on grace notes

## [1.1.2] - 2017-06-19
* Fix visibility attribute handling (@rettinghaus)

## [1.1.1] - 2017-06-19
* Fix for breath placement
* Fix for control event placement

## [1.1.0] - 2017-06-13
* Support for breath elements (@rettinghaus)
* Support for nested tuplets and ratio number display (@rettinghaus)
* Support for label attribute passed to the SVG (@rettinghaus)
* Support to breaks, key and time changes in MusicXML import (@rettinghaus)
* Improved multi rests (@rettinghaus)
* Improved tuplets (@rettinghaus)
* Improved rest positioning (@rettinghaus)
* Improved SVG stem structure (@rettinghaus)
* Improved ledge line thickness (@rettinghaus)
* Fix for the Python setup
* Update of pugixml (version 1.8)

## [1.0.0] - 2017-05-15
* Support for multi-layer layout (accidental and note placement)
* Support for figured bass (@xhero)
* Support for mordents and turns
* Support for SMuFL substitution in /harm and /f
* Support for xml:space on rend (@rettinghaus)
* Support (experimental) for WebAssembly
* Support for type attribute set as SVG class
* Support for SMuFL glyph anchor points for accidental layout
* Support for coloring (@rettinghaus)
* Support for loc attribute placement
* Support for stemless chords
* Upgrading page-based MEI to version 3.0.0
* Improved of Humdrum import (@craigsapp)
* Improved mensural notation support (@donbyrd)
* Improved SVG structure (e.g., for dots, stems and flags)
* Improved SVG font style (@rettinghaus)
* Improved horizontal layout
* Improved cross-staff notation
* Improved grace note layout
* Improved direct MusicXML import (@rettinghaus)
* Improved meterSig support
* Improved ledger lines
* Additional SMuFL glyphs to Leipzig (version 5.2.28)
* Several bug fixes and code refactoring enhancement

## [Pre-release versions]
- [0.9.13](https://github.com/rism-ch/verovio/releases/tag/version-0.9.13)
- [0.9.12](https://github.com/rism-ch/verovio/releases/tag/version-0.9.12)
- [0.9.11](https://github.com/rism-ch/verovio/releases/tag/version-0.9.11)
- [0.9.10](https://github.com/rism-ch/verovio/releases/tag/version-0.9.10)
- [0.9.9](https://github.com/rism-ch/verovio/releases/tag/version-0.9.9)
- [0.9.8](https://github.com/rism-ch/verovio/releases/tag/version-0.9.8)
- [0.9.7](https://github.com/rism-ch/verovio/releases/tag/version-0.9.7)
- [0.9.6](https://github.com/rism-ch/verovio/releases/tag/version-0.9.6)
- [0.9.5](https://github.com/rism-ch/verovio/releases/tag/version-0.9.5)
- [0.9.4](https://github.com/rism-ch/verovio/releases/tag/version-0.9.4)
- [0.9.3](https://github.com/rism-ch/verovio/releases/tag/version-0.9.3)
- [0.9.2](https://github.com/rism-ch/verovio/releases/tag/version-0.9.2)
- [0.9.1](https://github.com/rism-ch/verovio/releases/tag/version-0.9.1)
- [0.9.0](https://github.com/rism-ch/verovio/releases/tag/version-0.9.0)
