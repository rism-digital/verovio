# Third-party notices

Verovio's runtime font support contains the following statically compiled,
feature-pruned components. They are built from the checked-in sources; the
build does not download dependencies.

| Component | Version | License | Local license |
| --- | --- | --- | --- |
| HarfBuzz | 14.2.1 (`56feae4035bdd48f62ba2b8d8c16232d4d89b3a4`) | MIT-style | `vendor/harfbuzz/COPYING` |
| Google WOFF2 | `fb9c3379f2605b10f3e8f1d9636664ab5576775c` | MIT | `vendor/woff2/LICENSE` |
| Brotli | 1.2.0 (`028fb5a23661f123017c060daa546b55cf4bde29`) | MIT | `vendor/brotli/LICENSE` |
| Bravura | 1.392 | SIL Open Font License | OpenType name records in `data/fonts/Bravura.woff2` |
| Tinos | Google Fonts `ba95515f1333efe9342c2ad988b9c2f6bef6dbad` | SIL Open Font License | `data/fonts/Tinos-OFL.txt` |

The exact font checksums and provenance are recorded in
`data/fonts/README.md`. Component-specific pruning notes are stored beside
each vendored library.
