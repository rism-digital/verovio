# Fonts

*   **[Leipzig](https://github.com/rism-digital/leipzig)** is Verovio's own font. 
*   **[Bravura](https://github.com/steinbergmedia/bravura)** is designed by Daniel Spreadbury. 
*   **[Gootville](https://github.com/musescore/MuseScore/tree/master/fonts/gootville)** is designed by Grzegorz Pruchniakowski and comes with [MuseScore](https://musescore.org/).
*   **[Petaluma](https://github.com/steinbergmedia/petaluma)** is designed by [Steinberg](https://www.steinberg.net/).
*   **[Leland](https://github.com/MuseScoreFonts/Leland)** is designed by [MuseScore](https://musescore.org/) (3.6).

All fonts included in Verovio are licensed under the [SIL Open Font License](http://scripts.sil.org/cms/scripts/page.php?item_id=OFL).

# Generate Script

The `generate.py` script is a utility for working with font files and preparing them for Verovio. Calling it with the
`--help` argument will list the possible sub-commands and options for working with the font files.

To extract the fonts, the `svgpathtools` module is required to exist in your Python environment.

To generate the `woff2` and `svg` fonts you should have `fontforge` installed. The script will try to auto-detect the
path to fontforge, but you can also pass a path to the binary directly.
