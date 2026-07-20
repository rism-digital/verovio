# Runtime font benchmarks

Build the native harness with `-DBUILD_FONT_BENCHMARKS=ON`, then run it with a
fixture and resource directory. `compare.py` executes baseline/head in ABBA
order, combines at least 30 samples per run, and enforces the +5% cold and +3%
warm median gates. Native and Node/WASM warm samples average batches of 1,000
cached renders before taking the median, avoiding timer-tick quantization at
sub-microsecond durations. `node.mjs` records the same measurements for a
production Emscripten module; `compare_node.py` applies the same ABBA ordering,
automatic single rerun, and gates to baseline/head modules.

Benchmark reports are JSON so release-size and timing history can be archived
and compared without parsing human-readable logs.

`size_report.py` records stripped CLI, deterministic installed-bundle, resource,
vendor-source, optional WASM raw/gzip/Brotli, and optional npm-pack sizes.

`alias-smoke.mjs` exercises the browser-facing `Uint8Array` registration API
against a modularized Emscripten build and verifies aliased text and music
families in rendered SVG.
