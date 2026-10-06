# Tablature regression inputs

- [tab-chord-ties.musicxml](tab-chord-ties.musicxml) reproduces cross-string tie matching in issue #4473.
- [tab-chord-tie-geometry.musicxml](tab-chord-tie-geometry.musicxml) reduces accompaniment 18614, measure 4, to one mixed conventional/TAB measure. Its TAB staff is scaled to 150%, with five ties between identical B7 chords. Correct endpoint matching alone does not prevent misplaced, oversized curves.
- [tab-tuplet.musicxml](tab-tuplet.musicxml) reproduces the six-line TAB tuplet placement in issue #4471.

Run the native regression checker from the repository root:

```sh
python3 doc/check-tablature.py /path/to/verovio \
  --baseline /path/to/previous/verovio \
  --output build/tablature-checks
```

The checker uses only the Python standard library. `--output` keeps generated MusicXML, MEI, and SVG for review; otherwise it uses a temporary directory. `--baseline` enables exact conventional-score SVG comparisons, excluding the version description.

The 53 cases cover tie pairing, reversed chord order, missing pitches, different-string unisons, different voices, tie chains, four- through seven-line tuplet staves, beamed and unbeamed tuplets, bracket visibility, and above/below placement. Geometry checks use 100%, 150%, and 200% TAB staff sizes, one- and two-digit frets, single-note ties, and system breaks. They verify fret-relative anchors, stroke scaling, orientation, and sampled curve clearance from all fret glyphs in the chord example.

Tuplet numbers remain controlled by the source; these inputs do not request blanket TAB number suppression.
