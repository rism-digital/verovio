# Bundled runtime fonts

## Leipzig

Leipzig 5.2.102 is the bundled default SMuFL music font. It is developed in
`fonts/Leipzig`, from where `Leipzig.woff2` is copied.

| File | SHA-256 |
| --- | --- |
| Bundled `Leipzig.woff2` | `c7db72b33a382d5589612c1f3176ccdf5a0c38231fa839806ff1367a6cc69fe6` |
| Source `leipzig_metadata.json` | `5e5c534bf3195748473eeffe6c809145305788c7b804eb679b95365d9d908040` |
| Bundled `Leipzig_metadata.json` | `ad6f0732e5a803a6bba89a5c4f64cb03f7e5e494949ef062bfa934e8defa79d8` |

Leipzig is licensed under the SIL Open Font License.

## Bravura

Bravura 1.392 is the bundled fallback SMuFL music font. Missing glyphs of any
music font fall back to it. Its source files in `fonts/Bravura` were imported
into Verovio by commit `5d15bf42a06478f3ae6d9115d7ff6c1ffd3b12a5`.

| File | SHA-256 |
| --- | --- |
| Source `Bravura.otf` | `dca2d90c88437a701b1c2e71fa54e76f9fa41d7deee935d74dc871ea66ecfdd2` |
| Bundled `Bravura.woff2` | `a10e9fb553a823203beed6dfa6d0f02464ca943a59d1cab91cf581cc4c0783c0` |
| Source `bravura_metadata.json` | `5c18a034a857c69be2720fe0cab655a17df934556b51925690789d5530c9881c` |
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

The Leipzig and Bravura files are generated from `fonts/<Font>` with
`fonts/generate_all.sh` (see `fonts/README.md`). A font without a WOFF2 source,
such as Bravura, is converted losslessly from its OTF source with fontTools,
keeping the timestamp of the source for a deterministic output. The Tinos files
are deterministic WOFF2 transcodes of the pinned static TTF sources. The fonts
are not subset, so outlines, metrics, and shaping are unchanged.

The bundled metadata retains the font identity, version, engraving defaults,
and all glyph anchors used at runtime while omitting large advance,
bounding-box, alternate, ligature, optional-glyph, and set sections that are
derived from the font or unused by Verovio.
