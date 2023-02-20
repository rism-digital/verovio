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

To generate the `css`, `woff2`, and `svg` fonts you should have `fontforge` installed. The script will try to 
auto-detect the path to fontforge, but you can also pass a path to the binary directly with the `--fontforge` argument.

If you are having problems, you can pass the `--debug` parameter, which will increase the verbosity of the script.

## Using poetry

Included are the necessary files to install a Python poetry-managed virtual environment. If you do not wish to use
these you may ignore them. You should make sure you have [Poetry](https://python-poetry.org) installed and then
run `poetry install` from this directory. `poetry shell` will then allow you to interact with the script using
the virtual environment.

## Installing svgpathtools

You may run into problems installing numpy and/or scipy when installing svgpathtools on a Mac using Homebrew, where it 
will complain of not being able to find LAPACK/BLAS. 

To fix this try:

```shell
 brew install openblas lapack

 export LDFLAGS="-L/opt/homebrew/opt/lapack/lib"
 export LDFLAGS="-L/opt/homebrew/opt/lapack/include"
 export CPPFLAGS="-L/opt/homebrew/opt/lapack/include"
 export PKG_CONFIG_PATH="-L/opt/homebrew/opt/lapack/pkgconfig"
 export CFLAGS=-Wno-error=implicit-function-declaration
 export LAPACK=/opt/homebrew/opt/lapack/lib/liblapack.dylib
 export BLAS=/opt/homebrew/opt/openblas/lib/libopenblas.dylib
```

After that, re-run `poetry install` and it should work.

## Contributing

Contributions are welcome. Please ensure you run the `mypy` and `black` utilities on any Python code submitted.
