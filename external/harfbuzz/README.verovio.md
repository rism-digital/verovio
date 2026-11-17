# Vendored HarfBuzz

- Upstream: <https://github.com/harfbuzz/harfbuzz>
- Release tag object: `77a832110d40b0179636f5be8f8781f8299d7e50`
- Peeled source commit: `56feae4035bdd48f62ba2b8d8c16232d4d89b3a4`
- Version: 14.2.1

Verovio compiles `src/verovio-harfbuzz.cc`, a feature-pruned amalgamation,
with `HB_LEAN`, `HB_MINI`, and `verovio-hb-config.h`. The override retains
thread-safe OpenType shaping, Unicode data, names, metrics, and CFF/TrueType
outline drawing. Platform integrations and the AAT, variable, color, bitmap,
subset, raster, serialization, utility, and development sources are excluded.
