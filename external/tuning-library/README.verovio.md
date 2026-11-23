# Vendored Surge Synth Team Tuning Library

- Upstream: <https://github.com/surge-synthesizer/tuning-library>
- Commit: `6fa04a024dc9174eebba3b5703f2bbc5fa23fb9e` (2026-01-22)
- License: MIT, see `LICENSE.md`

The header-only library in `include/`, `LICENSE.md`, and a shortened
`README.md` are present. The command-line tools, tests, scripts, and build
files are not included.

## Local modifications

- `include/TuningsImpl.h`: three variables are initialized and three
  `size_t` to `int` conversions are made explicit to avoid compiler warnings.
- `README.md` is a shortened version that points to the upstream repository.
