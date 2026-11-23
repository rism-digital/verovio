# Third-party notices

Verovio's runtime font support contains the following statically compiled,
feature-pruned components. They are built from the checked-in sources; the
build does not download dependencies.

| Component | Version | License | Local license |
| --- | --- | --- | --- |
| HarfBuzz | 14.2.1 (`56feae4035bdd48f62ba2b8d8c16232d4d89b3a4`) | MIT-style | `external/harfbuzz/COPYING` |
| Google WOFF2 | `fb9c3379f2605b10f3e8f1d9636664ab5576775c` | MIT | `external/woff2/LICENSE` |
| Brotli | 1.2.0 (`028fb5a23661f123017c060daa546b55cf4bde29`) | MIT | `external/brotli/LICENSE` |
| Bravura | 1.392 | SIL Open Font License | OpenType name records in `data/fonts/Bravura.woff2` |
| Tinos | Google Fonts `ba95515f1333efe9342c2ad988b9c2f6bef6dbad` | SIL Open Font License | `data/fonts/Tinos-OFL.txt` |

The exact font checksums and provenance are recorded in
`data/fonts/README.md`. Component-specific pruning notes are stored beside
each external library.

## Other embedded libraries

Verovio also contains the following statically compiled libraries. Their
origin and local modifications are recorded in the `README.verovio.md` file
beside each library. humlib is maintained in `include/hum` and `src/hum`.

| Component | Version | License | Local license |
| --- | --- | --- | --- |
| CRC implementation by Michael Barr | not versioned | Public domain | Notice in `external/crc/crc.h` and `external/crc/crc.cpp` |
| JSON++ (jsonxx) | `1a3738d0062053b7fcdc1c0ea58b1675397f75ef` | MIT | `external/jsonxx/LICENSE` |
| midifile | `51eec11fe4f8e05bf4ce36ccc7d7e179f174291a` | BSD 2-Clause | `external/midifile/LICENSE.txt` |
| miniz-cpp | `62ec5c46c276d1880ba3b5a121d5a5dc24667b9e` (with miniz 2.2.0) | MIT (miniz: MIT and public domain) | Notices in `external/miniz-cpp/zip_file.hpp` |
| pugixml | 1.16 (`974e736f2dc5c7e47c5abfa3f071ee6bc168f5af`) | MIT | `external/pugixml/LICENSE.md` |
| Surge Synth Team Tuning Library | `6fa04a024dc9174eebba3b5703f2bbc5fa23fb9e` | MIT | `external/tuning-library/LICENSE.md` |
