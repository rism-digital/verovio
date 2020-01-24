# Changelog

## [unreleased]
* Option for forcing first page to be condensed (--condense-first-page)
* Option for forcing pages qwith tempo to be condensed (--condense-tempo-pages)
* Option for forcing encoded layout rendering to be condensed (--condense-encoded)

## [2.4.0] - 2020-01-15
* Support for short and tick barlines  with `measure@bar.len` and `measure@bar.place` (@earboxer)
* Support for dashed and dotted slurs and ties (@earboxer and @napulen)
* Option to use xml entities for SMuFL charachters (--outputSmuflXmlEntities)
* Options for controlling system divider display (--systemDivider "none|left|left-right")
* Fix coloring of ties and slurs (@rettinghaus)
* Fix bug with tuplet number placement (@rettinghaus)
* LibMEI output generated from MEI 4.0.1

## [2.3.4] - 2020-01-03
* Fix bug with NPM build (allow access to verovio.module and use wasm)

## [2.3.3] - 2019-11-26
* Fix bug with NPM build (remove init function for adjusting memory)

## [2.3.2] - 2019-11-20
* Fix bug with mordent when loading MEI 3.0 files

## [2.3.1] - 2019-11-16
* Fix  bug with scoreDef / staffDef redefinition

## [2.3.0] - 2019-11-04
* Support for 512th and 1024th duration
* Support for iOS cocoapods build (@eltiren)
* Support for SVG currentColor (@rettinghaus)
* Options for controlling header and footer (--header / --footer "auto|none|encoded") (@rettinghaus)
* Bug fix with grace notes and mRests

### Changes
* Options --no-header and --no-footer deprecated

## [2.2.1] - 2019-10-23
* Fix bug with mensural notation layout

## [2.2.0] - 2019-10-17
* Support for superscript and subscript in `<rend>`
* Support for chord-symbol parsing in MusicXML (@earboxer)
* Support for dynamics with mixed text and music fonts
* Support for non-standard keys from MusicXML (@rettinghaus)
* Support for `<graceGrp>` (@rettinghaus)
* Support for `<keyAccid>` for custom key signatures
* Support for `@vel` on `<note>` in MIDI playback (@rettinghaus)
* Support for `<turn>` in MusicXML import (@wergo)
* Support for `<arpgeg>` in MusicXML import (@wergo)
* Support iOS Framework build (@Noroxs)
* Improved support for pedal marks in MusicXML import (@wergo)
* Improved import of key signatures from PAE (show cancellation accidentals)
* Improved import of tuplets from PAE
* Improved element attribute access also when not rendered in a page
* Options for vertical justication (--justify-include-last-page, --justify-system-only, --justify-vertically)
* Option to set minimum width for justification (--min-last-justification) (@earboxer).

### Changed
* Method getMIDIValuesForElement (JS) return a JSON object

## [2.1.0] - 2019-05-25
* Update pugixml
* Support for SMUFL figured bass figures
* Support for `@spacing` on `<staffDef>`
* Support for `@sameas` on `<beam>`, `<chord>`, `<layer>`, `<note>`, `<rest>`, and `<tuplet>`
* Support for editorial markup in `<label>` and `<labelAbbr>`
* Support of ties and slurs starting or ending with timestamp (but not both)
* Support for ABC notation import (@rettinghaus)
* Improved cross-staff MusicXML import (@wergo)
* Improved clef changes in MusicXML import (@wergo)
* Improved hairpin MusicXML import (@wergo)
* Improved slur MusicXML import (@wergo)
* Support for `<ending>` in MusicXML import (@wergo)
* Preserve MusicXML `<duration>` on notes, rests, and chords (@rettinghaus)
* Import MusicXML `<division>` with `@ppq` on `<staffDef>` (@rettinghaus)
* Avoid system optimisation when a tempo change occurs
* Improved tuplets and support for additional attributes
* Improved slur placement
* Option to use svg viewBox on root node (@dchisolm)
* Option for adjusting MIDI tempo output (--midi-tempo-adjustment)
* Option for top/bottom margin `<harm>` (--top/bottom-margin-harm)
* Option to use first footer / header for all pages (--use-pg-footer/header-for-all)
* Embedding of Petaluma font (1.055) from Steinberg

### Changed
* Option inputFormat (JS) deprecated (use input instead)

## [2.0.2] - 2019-02-06
* Fix issue with <app> and multiple <mdiv>

## [2.0.1] - 2019-02-04
* Fix for Plaine & Easie empty beams

## [2.0.0] - 2019-01-21
### Added
* Support for `<bracketSpan>`
* Support for extending line in `<f>`
* Support for `@beam.with` on `<beam>`
* Support for `@stem.pos` values "left" and "right" (@rettinghaus)
* Support for dynamic silent staff hiding (implements `@optimize` on `<scoreDef>`)
* Support for `<subst>` with child (`<add>`, `<del>` or `<subst>`) to be selected with `substXPathQuery`
* Support for `<dir>` and `<dynam>` connectors (triggered with `@next` and `@extender`)
* Sutomatic positioning of rests within beam (@CCInc)
* Fix for Plaine & Easie mensur changes in mensural notation
* Use SMuFL glyphs for mensural signs (@rettinghaus)
* Barline segmentation with text and dynamic indications overlapping measures
* Improved layout with text (dir, tempo, etc.) at the end of a system
* Support for dashed bar lines
* Improvement of MIDI output for mensural notation
* Support for `@stem.visible` on `<note>` and `<chord>`
* Fix for Plaine & Easie chords and ties
* Fix for Plaine & Easie dots in mensural notation
* Support for MIDI playback of `<beatRpt>`
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
* Support for `<lb>` within `<rend>`, `<dir>`, `<label>` and `<labelAbbr>`
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
