# Bundled runtime fonts

## Bravura

Bravura 1.392 is the bundled default and fallback SMuFL music font. These
files were imported into Verovio by commit
`5d15bf42a06478f3ae6d9115d7ff6c1ffd3b12a5`.

| File | SHA-256 |
| --- | --- |
| Source `Bravura.otf` | `dca2d90c88437a701b1c2e71fa54e76f9fa41d7deee935d74dc871ea66ecfdd2` |
| Bundled `Bravura.woff2` | `07547608313e4271a25cb8c564781c8b2e869fd4eec8abbed077f921a70ab4c6` |
| Source `Bravura_metadata.json` | `5c18a034a857c69be2720fe0cab655a17df934556b51925690789d5530c9881c` |
| Bundled `Bravura_metadata.json` | `7083c7715937cf6a67f57463373a09bf1e9b0609906f8a7a20c56a9bf76b052e` |

Bravura is licensed under the SIL Open Font License. Its copyright and
license information is stored in the font's OpenType name records.

## Tinos

Source: `google/fonts` commit
`ba95515f1333efe9342c2ad988b9c2f6bef6dbad`, directory `ofl/tinos`.

| File | SHA-256 |
| --- | --- |
| Source `Tinos-Regular.ttf` | `60a0e8ef0c04dd5dd69ffe91025fa2ae5836cbd35600a82ba031977557e2cb61` |
| Source `Tinos-Italic.ttf` | `5942266ed398b155d7dc23e36833e7ec6be988f2439bdbeb8ef1bede808eaa91` |
| Source `Tinos-Bold.ttf` | `393269dbab8899f938db19783eca5eac92eb431f7ae0ab45b8349ca895f1a06b` |
| Source `Tinos-BoldItalic.ttf` | `a5de79f0fe863ea0954757acb3d47b3ccd0a930ce3dd5b97230cd3866790a06e` |
| Bundled `Tinos-Regular.woff2` | `90a44b1ca994c9f23f23a5d7417843bcdd3b88e87e5bd342d00205d6301dd453` |
| Bundled `Tinos-Italic.woff2` | `c264ef6211fc500cf89d40777f0ab8eda3b7e57c7926ee8b4cfdd826c37828b9` |
| Bundled `Tinos-Bold.woff2` | `06591737d8fd8215b2fb703cabefcccd3431a34dc383774127e76516790a2e82` |
| Bundled `Tinos-BoldItalic.woff2` | `27aa9a1c511c5072c6d60475ae0a3d13fc33b3da8631005a05ecf1a431c8325e` |

The family metadata identifies the license as OFL. `OFL.txt` is the standard
SIL Open Font License text stored by Google Fonts.

The release resources use deterministic WOFF2 transcodes of the pinned static
OTF/TTF sources. Registration decodes them once to canonical SFNT bytes. This
keeps the installed CLI and WASM resources within the size gate without
subsetting the glyph repertoire or changing outlines, metrics, or shaping.

The bundled Bravura metadata is generated with
`fonts/compact-smufl-metadata.py`. It retains the font identity, version,
engraving defaults, and all glyph anchors used at runtime while omitting large
advance, bounding-box, alternate, ligature, optional-glyph, and set sections
that are derived from the font or unused by Verovio.
