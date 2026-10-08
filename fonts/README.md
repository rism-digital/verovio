# Fonts

* **[Leipzig](https://github.com/rism-digital/leipzig)** is Verovio's own font and its default music font.
* **[Bravura](https://github.com/steinbergmedia/bravura)** is designed by Daniel Spreadbury and is Verovio's music fallback font.

## Text font

* **[Tinos](https://fonts.google.com/specimen/Tinos)** is Verovio's default text font (see `data/fonts/README.md`).

All fonts included in Verovio are licensed under the [SIL Open Font License](http://scripts.sil.org/cms/scripts/page.php?item_id=OFL).

Other fonts, such as Gootville, Leland, or Petaluma, are no longer bundled but can be registered at runtime.

## Generate Script

The `generate.py` script is a utility for working with font files and preparing them for Verovio. Calling it with the
`--help` argument will list the possible sub-commands and options for working with the font files. The
`generate_all.sh` script runs the sub-commands needed after a change to a bundled font or to `supported.xml`:

* `smufl` generates `include/vrv/smufl.h` and `src/smufl_names.inc` from `supported.xml`.
* `check` reports the glyphs supported by Verovio that are missing in a font.
* `bundle` generates the WOFF2 font, its subset with the supported glyphs for embedding in the SVG, and the
  compacted SMuFL metadata of a bundled font in `data/fonts`.

The `bundle` sub-command requires the `fonttools` and `brotli` modules in your Python environment.

To generate `svg` and `woff2` fonts you should have `fontforge` installed. The script will try to
auto-detect the path to fontforge, but you can also pass a path to the binary directly with the `--fontforge` argument.

If you are having problems, you can pass the `--debug` parameter, which will increase the verbosity of the script.

### Using poetry

Included are the necessary files to install a Python poetry-managed virtual environment. If you do not wish to use
these you may ignore them. You should make sure you have [Poetry](https://python-poetry.org) installed and then
run `poetry install` from this directory. `poetry env activate` will then show you the command to start
the virtual environment in which you may interact with the script.

## Contributing

Contributions are welcome. Please ensure you run the `mypy` and `black` utilities on any Python code submitted.
