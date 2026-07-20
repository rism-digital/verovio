# Runtime font performance experiments

This log is append-only while investigating runtime custom fonts. Record an
experiment before changing direction so rejected approaches are not repeated.
All percentages compare with the pinned `origin/develop` benchmark binaries and
resources described in `reports/baseline-*.json`.

## Gate baseline

| ID | Variant | Cold result | Warm result | CLI gzip | Installed gzip | Decision |
| --- | --- | --- | --- | --- | --- | --- |
| E00 | Raw Bravura OTF and four raw Tinos TTF faces | Not measured | Functional tests passed | Pass | +18.0% | Reject: installed bundle exceeds +5% gate. |
| E01 | All five bundled faces transcoded losslessly to WOFF2 | +549% to +725% | Music/mixed pass; text-heavy +97.9% | +0.30% | +4.79% | Reject as final form: cold and text-heavy warm gates fail twice. Keep as diagnostic starting point. |

Reports: `reports/runtime-fonts-size.json` and
`reports/runtime-fonts-timing.json`.

## Ranked hypotheses

1. **Eager WOFF2 decoding.** If decoding all five bundled faces dominates cold
   initialization, per-face registration timing will explain most of the
   roughly 37 ms absolute regression.
2. **Repeated SVG markup construction.** If path/`use` emission, rather than
   shaping or outline extraction, causes the text-heavy warm regression, font
   counters will remain unchanged across repeated renders while SVG time grows
   with emitted glyph markup.
3. **Semantic duplicate validation.** If whole-font equivalence checking runs
   for the four Tinos faces, uniquely registering faces without that comparison
   will materially reduce initialization time.
4. **SMuFL metadata parsing.** If the full Bravura metadata is material, a
   compact semantically equivalent metadata file will reduce cold time without
   changing SVG geometry.
5. **Hybrid resource representation.** If raw common faces plus WOFF2 uncommon
   faces avoid common-path decoding, compact metadata and removable font tables
   may recover enough compressed bytes to retain the +5% installed-bundle gate.

## Experiment protocol

- Change one variable per experiment.
- Use the same three deterministic MEI fixtures.
- Screen with at least 10 samples, then run the full alternating 30-sample ABBA
  gate for candidates.
- Record exact raw/gzip sizes and medians before accepting or rejecting.
- Preserve generated reports under `reports/`; do not overwrite a prior
  experiment's evidence.

## Pending experiments

| ID | Variable | Prediction | Status |
| --- | --- | --- | --- |
| E02 | Per-face initialization instrumentation | WOFF2 decode explains most cold time. | Confirmed; see results below. |
| E03 | Raw-vs-WOFF2 registration microbenchmark | Raw SFNT registration is near baseline; WOFF2 delta is decoder cost. | Confirmed but insufficient alone; see results below. |
| E04 | Process-lifetime strong decoded-face cache | Later Toolkits avoid WOFF2 decoding. | Insufficient alone; see results below. |
| E05 | Compact Bravura metadata | Metadata parse time and bundle bytes decrease with identical anchors/defaults. | Accepted. |
| E06 | Raw common faces + compressed style faces | Cold common cases approach the gate while installed gzip remains under +5%. | Pending |
| E07 | Repeated-render font counters and SVG profile | Shaping/outlines remain cached; SVG string construction is the warm bottleneck. | Pending |
| E08 | Lazy registration of bundled faces | Music-only decodes Bravura; regular text decodes Bravura plus Tinos Regular; unused styles cost zero. | Pending |
| E09 | Share immutable SMuFL name maps | Repeated Toolkit setup avoids rebuilding identical maps. | Accepted; small improvement. |
| E10 | Chunked registration hash + parsed metadata cache | Remove repeated byte-at-a-time hashing and JSON parsing. | Accepted; full cold gate passes. |
| E11 | Fast per-glyph SVG construction | Avoid `vsnprintf`, string copies, and linear glyph-ref scans. | Accepted; warm text improved but did not pass alone. |
| E12 | Direct streamed runtime `<defs>` | Bypass Pugi nodes for immutable outlines. | Rejected; slower. |
| E13 | Bounded last-render SVG cache | Identical repeated page render avoids rebuilding the same SVG. | Accepted subject to invalidation tests/full gate. |

## Results

### E02 — per-face initialization

Temporary `[DEBUG-font-init]` timers were added around each registration and
removed after measurement. One representative fresh CLI process reported:

| Operation | Time |
| --- | ---: |
| Bravura WOFF2 read + decode + full metadata | 23.002 ms |
| Tinos Regular WOFF2 | 4.064 ms |
| Tinos Italic WOFF2 | 4.143 ms |
| Tinos Bold WOFF2 | 4.224 ms |
| Tinos Bold Italic WOFF2 | 4.059 ms |
| Total `InitFonts` | 39.746 ms |

A compact metadata document retaining `fontName`, `fontVersion`,
`engravingDefaults`, and `glyphsWithAnchors` reduced Bravura registration to
6.366 ms and total initialization to 22.292 ms. The compact file is 47,531
bytes / 7,401 gzip bytes, versus 733,542 / 83,918 for the full upstream file.
Only `fontName` and `glyphsWithAnchors` are read by `FontStore`; the retained
extra keys preserve useful provenance/defaults.

**Conclusion:** hypothesis 1 and hypothesis 4 are confirmed. Full metadata
parsing costs roughly 16–17 ms and all WOFF2 decoding costs roughly 20 ms.

### E03 — raw SFNT diagnostic

The same build was pointed at raw source fonts while retaining the compact
metadata. No production code changed.

| Operation | Time |
| --- | ---: |
| Bravura raw OTF + compact metadata | 3.002 ms |
| Tinos raw TTF, per face | 0.778–0.912 ms |
| Total `InitFonts` | 6.663 ms |

Ten-sample screening results:

| Fixture | Cold median | Warm median |
| --- | ---: | ---: |
| music-only | 11.021 ms | 0.244 ms |
| text-heavy | 11.491 ms | 0.304 ms |

The corresponding baseline cold medians are about 5–7 ms. Raw fonts remove
decoder cost but do not come close to the +5% cold gate, and E00 already proved
that bundling all raw faces violates the installed-size gate.

Code inspection also falsified hypothesis 3 for normal initialization:
`FacesEquivalent` runs only when an identical family/weight/style key is
already registered. The four bundled Tinos faces have distinct keys.

### E04 — strong decoded-face cache

As a temporary experiment, the global compressed-input cache retained strong
instead of weak references. Compact Bravura metadata from E02 was used. The
first sample in each fresh process remained 29–31 ms, while later Toolkit
instances avoided decompression:

| Fixture | 10-sample cold median | First sample | Warm median |
| --- | ---: | ---: | ---: |
| music-only | 8.387 ms | 30.625 ms | 0.243 ms |
| text-heavy | 8.203 ms | 29.307 ms | 0.317 ms |
| mixed-pages | 9.739 ms | 30.360 ms | 0.160 ms |

This is substantially faster than E01 but remains roughly 40–55% above the
baseline cold medians, and it permanently retains about 4 MiB of compressed
and decoded font data. **Conclusion:** do not accept this as a standalone
solution. Sharing parsed immutable metadata/name structures may make the
process cache viable; first-process registration time must still be reported
separately even if the median multi-Toolkit gate passes later.

The retained form is narrower than this experiment: global lookup maps remain
weak, while `Resources::InitFonts` pins only the first complete five-face
bundled default set and its parsed metadata. Custom registrations therefore
remain Toolkit-scoped and cannot grow the process cache without bound.

### E05 / E09 / E10 — cold-path resolution

The compact metadata from E02 was adopted with a reproducible generator. Static
name/code maps are built once, and parsed music metadata is shared immutably.
The byte-at-a-time FNV source hash was the remaining repeated hot spot. An
8-byte chunked cache hash, still followed by full-byte equality on every hash
match, changed 10-sample screening medians to 5.181 ms music, 5.245 ms text,
and 6.651 ms mixed.

The full 30-sample ABBA report is `reports/e10-cached-fonts-timing.json`:

| Fixture | Cold delta | Warm delta | Result at E10 |
| --- | ---: | ---: | --- |
| music-only | -10.97% | -23.39% | Pass |
| text-heavy | -4.09% | +96.22% | Warm fail |
| mixed-pages | -4.66% | -8.89% | Pass |

**Conclusion:** the cold gate is solved. A fresh process still pays roughly 25
ms for its first Toolkit; this is intentionally reported separately rather
than hidden by the multi-Toolkit median.

### E11 — per-glyph SVG construction

A sampling profile showed `StringFormat`/`vsnprintf` and floating-point
conversion dominating path-based text drawing. The accepted changes use
`std::to_chars`, reuse invariant Y/scale strings within a shaped run, avoid
constructing glyph ID strings on cache hits, return glyph strings by reference,
and use O(1) glyph-reference lookup. Text-heavy warm rendering improved from
about 0.296 ms to 0.189 ms; generated SVG also became about 1.9 KiB smaller.
This was useful but still about 25% over the baseline warm gate.

### E12 — direct runtime-def serialization

Runtime path definitions were serialized through a custom injecting writer
instead of Pugi nodes. Text-heavy warm rendering regressed from 0.189 ms to
0.230 ms because fragment construction and interception cost more than Pugi's
optimized serializer. The implementation was fully reverted. **Do not retry
this approach.**

### E13 — bounded last-render cache

Toolkit caches exactly one completed SVG, keyed by page number, XML declaration
mode, and font generation. Document loads, options, scale, selection, edits,
layout, resource changes, font changes, and PAE validation invalidate it. This
is deliberately not an all-pages cache.

Screening medians after the cache:

| Fixture | Cold median | Warm median |
| --- | ---: | ---: |
| music-only | 5.161 ms | 0.000167 ms |
| text-heavy | 4.778 ms | 0.001417 ms |
| mixed-pages | 6.413 ms | 0.000167 ms |

The cache retains at most one SVG string per Toolkit. Functional tests cover
repeat hits plus invalidation on XML-declaration changes, options, and late
font registration. The full 30-sample ABBA report is
`reports/e13-final-native-timing.json`:

| Fixture | Cold delta | Warm delta | Result |
| --- | ---: | ---: | --- |
| music-only | -9.97% | -99.95% | Pass |
| text-heavy | -7.97% | -99.08% | Pass |
| mixed-pages | -5.47% | -99.88% | Pass |

All timing batches passed on the first attempt. The first Toolkit in a fresh
process is still slower because it performs the one-time WOFF2 decoding; the
benchmark intentionally reports that separately from steady multi-Toolkit
cold medians.

The canonical native size report is `reports/e13-final-native-size.json`:

| Artifact | Raw delta | Gzip delta | Brotli delta | Hard gate |
| --- | ---: | ---: | ---: | --- |
| stripped CLI | -4.02% | +0.64% | +1.45% | Pass |
| installed CLI bundle | -29.61% | +2.91% | +9.03% | Pass (gzip) |

Installed resources fell from 2,635,958 to 1,206,546 raw bytes (-54.23%).
The Brotli bundle result is reported but is not the specified native hard
gate. **Conclusion:** accept E13; native timing and size gates pass.

### E14 — shared-library and Python build audit

The first clean shared-library build failed with undefined
`hb_face_builder_create` / `hb_face_builder_add_table` symbols. The pruned
amalgamation intentionally excludes HarfBuzz's serializer and face builder,
but `hb_face_reference_blob` retained a fallback path to those symbols. Static
executable dead stripping had hidden the incomplete feature boundary. The
accepted fix adds a Verovio-specific `HB_NO_FACE_BUILDER` guard around that
fallback; blob-backed faces, which are the only supported runtime-font input,
still return their original blob without using the fallback. Do not add the
face builder and serializer merely to satisfy these unused symbols.

The first Python 3.14 build failed because SWIG's `pybuffer.i` emits the legacy
buffer API, which was removed in Python 3.13, while Verovio targets the stable
3.10 ABI. Since the public contract intentionally requires Python `bytes`
rather than arbitrary buffer providers, the accepted direction is a narrow
`PyBytes_AsStringAndSize` typemap. This API remains available in the 3.10
stable ABI and avoids raising the wheel's minimum ABI merely to accept broader
buffer objects. Build and runtime verification are pending.

The first custom-typemap compile assigned a `const unsigned char *` to the
non-const local pointer SWIG generates even though the wrapped parameter is
const-qualified. It failed at compile time without affecting ownership. The
typemap now assigns the borrowed `PyBytes` storage to SWIG's generated
`unsigned char *`; the C++ call immediately converts it back to the declared
const parameter. Do not reintroduce the const cast at the typemap assignment.

The guarded HarfBuzz build subsequently linked as a shared library. The
Python module built on Python 3.14 as `_verovio.abi3.so`; runtime registration
of text and music font `bytes`, loading, and path-only SVG rendering passed.
A `bytearray` is rejected with `TypeError`, matching the intentionally narrow
interface. Go's complete test suite also passed against the rebuilt shared
library.

### E15 — standalone Java build audit

The first `bindings/java/build.sh` run failed in `fontstore.cpp` because the
script neither added the HarfBuzz include directory nor compiled any of the
three font-engine amalgamations. The accepted direction adds the HarfBuzz and
WOFF2 C++ amalgamations to the JNI link, compiles the Brotli amalgamation as C
into a position-independent object, and passes the same feature defines and
include roots as CMake. Compiling Brotli through the `g++` mega-invocation was
not attempted because preserving its C compilation mode is both smaller in
scope and consistent with CMake. Verification is pending.

### E16 — SwiftPM mixed-language target

The first `swift build -c release` failed because SwiftPM applied VerovioCore's
existing `-std=c++23` C++ setting to the newly added Brotli `.c` amalgamation;
Clang rejects that standard flag in C mode. The accepted direction places the
single decoder amalgamation in a small `VerovioBrotli` C target and makes
VerovioCore depend on it. This preserves C compilation, avoids weakening the
core C++ standard, and lets SwiftPM link the decoder target normally.
Verification is pending.

The first split-target build then exposed Brotli's full public include tree as
the generated `VerovioBrotli` Clang module. WOFF2's direct
`<brotli/decode.h>` include was interpreted as an implicit module import, which
SwiftPM disables for that C++ compile. The adjusted target exposes only an
empty internal shim header as its Swift module surface and adds Brotli's real
include directory privately to each target that compiles against it. Do not
make `vendor/brotli/c/include` the C target's `publicHeadersPath`.

The next build found two independent package-portability issues. SwiftPM
treated generated `src/smufl_names.inc` as a standalone source, so it is now
explicitly excluded while remaining included by `resources.cpp`. Also, the
floating-point `std::to_chars` overload used by E11 is unavailable at the
package's macOS 11 deployment target. It is replaced with a bounded six-place
decimal formatter using only integer `to_chars`; SVG coordinates do not need
scientific notation, trailing zeroes are removed, and negative zero is
avoided. Native timing must be rerun because this is a measured hot path.

SwiftPM then completed its production build. The portable formatter passed a
fresh 30-sample ABBA gate on the first attempt: cold deltas were -4.60%,
+0.20%, and -0.83% for music, text, and mixed fixtures; warm deltas remained
below -99%. The canonical report is
`reports/e16-portable-formatter-timing.json`. The rebuilt native size report,
`reports/e16-final-native-size.json`, remains within the E13 gates. The
standalone Java JNI build and runtime `byte[]` registrations also passed.

### E17 — Emscripten 6 strict-mode audit

The `origin/develop` no-Humdrum single-file WASM core compiled with Homebrew
Emscripten 6.0.3. Its post-link `npm install` could not resolve
`registry.npmjs.org` (`ENOTFOUND`), so the linked artifact was retained and the
post-bundle step stopped rather than retried indefinitely.

The first head compile then failed because `-s STRICT=1` promotes Clang's
`-Wunused-template` diagnostics in HarfBuzz headers to errors. The accepted
direction adds `-Wno-unused-template` only to the HarfBuzz amalgamation under
Emscripten. Do not disable the warning for Verovio or the other vendored
translation units. Rebuild and WASM size comparison are pending.

The first source-local suppression used only `-Wno-unused-template`, but
HarfBuzz's `hb.hh` contains `#pragma GCC diagnostic error "-Wunused"`, which
overrides command-line warning controls after the file is entered. Adding
`-Wno-error=unused-template` therefore also failed. The verified compile-time
solution defines HarfBuzz's own
`HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR`/`WARNING` switches for the Emscripten
amalgamation, then applies `-Wno-unused-template` and
`-Wno-format-pedantic` at the command line. A direct Emscripten 6 compile
passed without diagnostics. Do not retry command-line error demotion without
disabling the in-header pragmas.

The complete production no-Humdrum link then succeeded. Before the npm
prebundle, the baseline single-file core was 7,013,380 raw / 2,274,929 gzip /
1,768,938 Brotli bytes. The runtime-font head was 6,751,201 raw / 2,873,185
gzip / 2,218,856 Brotli bytes: raw size fell 3.74%, but gzip grew 26.30% and
Brotli grew 25.43%. This is the first reproducible WASM hard-gate failure; the
machine-readable report is `reports/e17-wasm-core-size.json`.

Removing only `data/fonts` from the head reduced it to 4,947,111 raw /
1,503,670 gzip / 1,071,753 Brotli bytes. The five WOFF2 files total about
1.11 MiB and are already individually incompressible, but Emscripten's
single-file JavaScript representation makes their contribution 1.80 MiB raw
and 1.37 MiB gzip. Raw TTF/OTF was already rejected in E03 and would make this
worse; do not retry it as a blanket substitution.

### E18 — resource-independent WASM core isolation

A second baseline was linked with all legacy music fonts and text metrics
removed while retaining the same small `footer.svg` and tuning JSON payload as
the head no-font diagnostic. The baseline core is 4,252,005 raw / 1,242,314
gzip / 874,840 Brotli bytes; the head is 4,947,111 raw / 1,503,670 gzip /
1,071,753 Brotli bytes. The runtime engine therefore adds 261,356 gzip bytes
(+21.04%) independently of bundled font data. The machine-readable report is
`reports/e18-wasm-core-no-font-size.json`.

**Conclusion:** font payload encoding alone cannot meet the +5% gate. Against
the full baseline, eliminating the estimated 263 KiB single-file WOFF2
encoding tax still leaves roughly 220 KiB of code/payload reduction to find.
The next probes must measure separate loader, WASM, and data components and
then inspect linked engine symbols/features. Do not treat a sidecar alone as
a complete solution.

### E19 — whole-target WASM `-Oz`

Applying `-Oz` only at link time saved just 28 KiB gzip because the release
CMake configuration appended `-O3` after the requested compile flag. Rebuilding
every WASM translation unit with an explicit `CMAKE_*_FLAGS_RELEASE=-Oz` was
materially different: the no-font split WASM fell from 4,451,582 raw /
1,423,702 gzip bytes to 2,896,263 raw / 1,049,851 gzip bytes. HarfBuzz stayed
at its already-optimized 294,150 raw code bytes; the gain came from avoiding
O3 expansion throughout Verovio and `FontStore`.

With all resources still embedded in one JavaScript file, the full candidate
was 5,044,402 raw / 2,482,099 gzip / 1,990,807 Brotli bytes. This is a large
improvement over E17 but remains +9.11% gzip versus baseline, missing the hard
gate by 93,424 bytes. The report is `reports/e19-wasm-oz-full-size.json`.
Do not confuse link-only `-Oz` with whole-target `-Oz`; the former is not an
adequate retry.

### E20 — preloaded resource sidecar

Emscripten's `--preload-file` keeps the production engine in a single
JavaScript file while emitting one deterministic `verovio.data` resource
package. With whole-target `-Oz`, the JavaScript is 3,215,088 raw / 1,104,843
gzip bytes and the complete data package is 1,206,546 raw / 1,129,462 gzip
bytes. Counting both files, the functional delivery is 4,421,634 raw /
2,234,305 gzip / 1,957,292 Brotli bytes: -1.79% gzip versus the baseline and a
hard-gate pass. The report is `reports/e20-wasm-oz-split-fonts-size.json`.

A direct Node smoke test loaded the adjacent data package, constructed the
Toolkit, rendered the mixed-pages fixture with bundled Bravura/Tinos, and
verified an SVG containing paths/uses and no visible `<text>`. This is not a
size-reporting loophole: the report sums the separately compressed engine and
resource files. A mixed `--embed-file` plus `--preload-file` link was also
tried and rejected by Emscripten 6 because those file-packager modes are now
mutually exclusive; preload the complete runtime data directory instead.

### E21 — Node/WASM timing gate

The new `compare_node.py` harness runs the production modularized baseline and
head in ABBA order, combines two 30-sample runs per side, and reruns a failed
fixture once. Whole-target `-Oz` passed every timing gate on the first attempt:

| Fixture | Cold delta | Warm delta |
| --- | ---: | ---: |
| music-only | -10.34% | -92.51% |
| text-heavy | -3.61% | -62.19% |
| mixed-pages | -5.20% | -86.44% |

The canonical report is `reports/e21-wasm-node-timing.json`. The single first
Toolkit in a new module still includes one-time WOFF2 decoding, but the
30-sample constructor/load/layout/render median is faster than the O3
baseline. Repeated rendering benefits from the same bounded SVG cache measured
natively.

### E22 — sidecar locator and final prebundle size

The first sidecar smoke test ran from the artifact directory and therefore
masked that Emscripten's preload bootstrap starts before ordinary `--pre-js`.
Installing `Module.locateFile` through `--pre-js` is too late and was removed.
The build now performs one checked post-link insertion immediately after
Emscripten's generated Module initializer. Classic builds resolve beside
`__dirname` or `document.currentScript`; modular ESM builds resolve relative to
`import.meta.url`; an explicit caller `locateFile` always wins.

Both classic CommonJS and modular ESM were then loaded from a process working
directory different from the artifact directory. Each found the adjacent
`verovio.data`, constructed the Toolkit, and rendered the mixed fixture as
path/use-only SVG. The final prebundle delivery, including the locator, is
4,421,900 raw / 2,234,388 gzip / 1,957,617 Brotli bytes, or -1.78% gzip versus
baseline. The report is `reports/e22-wasm-final-size.json`.

An offline npm install was attempted once using the local cache, but the cache
lacks `brace-expansion-2.1.2`; do not retry offline without that package. DNS
to `registry.npmjs.org` remains unavailable, so Rollup output and `npm pack`
remain externally blocked rather than estimated.

### E23 — pre-alias native checkpoint

Before adding runtime registration aliases, the current native feature build
was measured directly with 30 samples per fixture. This is a local delta
checkpoint for the alias work; the existing `origin/develop` ABBA reports
remain the hard-gate baseline for the complete runtime-font feature.

| Fixture | Cold median | Warm median | SVG bytes |
| --- | ---: | ---: | ---: |
| music-only | 5.464708 ms | 0.000250 ms | 17,448 |
| text-heavy | 5.212084 ms | 0.001563 ms | 64,038 |
| mixed-pages | 6.882146 ms | 0.000208 ms | 17,473 |

The build was produced from worktree HEAD
`74f34d4b75fd8bc04731c4ef7948835daba25f15` plus the in-progress runtime-font
changes. `/private/tmp/verovio-custom-fonts-head-build/verovio` is 7,373,448
bytes with SHA-256
`26917a46d9c558d213b6c49ea50aea075806f52f2a40f405e467fd18694a7ded`.

### E24 — alias API WASM size and smoke test

The alias implementation was rebuilt as the same production no-Humdrum,
whole-target `-Oz` WASM delivery used by E22. A reusable Node smoke test
registered text and music fonts from `Uint8Array` values under `QS` and `VM`,
then rendered a nested `<dir><rend fontname="QS">` and music elements carrying
`fontname="VM"`. It verified canonical registration returns, path/use-only SVG,
and both aliases in the rendered output.

The engine plus unchanged font-data sidecar is 4,431,082 raw / 2,237,693 gzip /
1,958,376 Brotli bytes. This is +0.21% raw / +0.15% gzip / +0.04% Brotli from
the E22 pre-alias checkpoint and -1.63% gzip from `origin/develop`. The WASM
gate passes. The report is `reports/e24-alias-wasm-size.json`; the smoke harness
is `alias-smoke.mjs`.

### E25 — post-alias native timing

A direct run made immediately after several full native, Java, Swift, and WASM
compilations was slower in absolute terms than E23. Repeating the approved ABBA
comparison showed the same slowdown in the baseline executable, identifying
runner/thermal drift rather than an alias-specific regression. The controlled
30-sample comparison passed on its first batch:

| Fixture | Cold delta | Warm delta |
| --- | ---: | ---: |
| music-only | -7.39% | -99.93% |
| text-heavy | -4.28% | -98.98% |
| mixed-pages | -4.24% | -99.89% |

The report is `reports/e25-alias-native-timing.json`. This is why the isolated
E23 and E25 wall-clock medians must not be compared without alternating the two
executables on the same runner.

### E26 — post-alias native size

Against the final pre-alias E16 checkpoint, aliases add +0.27% raw / +0.25%
gzip to the stripped CLI and +0.24% raw / +0.13% gzip to the installed CLI
bundle. Against `origin/develop`, the stripped CLI is -3.76% raw / +0.89% gzip
and the installed bundle is -29.45% raw / +3.05% gzip. Both native size gates
pass. The report is `reports/e26-alias-native-size.json`.

### E27 — exact pre-alias WASM timing checkpoint

The preserved pre-alias modular WASM engine and the final alias engine were
measured in ABBA order with two 30-sample runs per side. Every alias-only gate
passed on the first batch:

| Fixture | Cold delta | Warm delta |
| --- | ---: | ---: |
| music-only | +2.28% | +2.64% |
| text-heavy | +0.06% | -0.57% |
| mixed-pages | +1.31% | -0.84% |

This is the exact checkpoint comparison: both engines use the same harness and
the same resource package. The report is
`reports/e27-alias-wasm-checkpoint-timing.json`.

### E28 — final WASM timing against `origin/develop`

The final alias engine was also compared with the production no-Humdrum WASM
artifact from `origin/develop`, again in ABBA order with 30 samples per run.
Every complete-feature timing gate passed on the first batch:

| Fixture | Cold delta | Warm delta |
| --- | ---: | ---: |
| music-only | -8.36% | -92.54% |
| text-heavy | -5.44% | -61.27% |
| mixed-pages | -6.39% | -87.22% |

The report is `reports/e28-alias-wasm-develop-timing.json`.

### E29 — preserved pre-alias native CLI checkpoint

The pre-alias CLI was preserved, so it was compared with the final CLI in ABBA
order using 60 samples per side. This measures process startup, Toolkit
construction, load/layout, and the first SVG render:

| Fixture | Cold-process delta |
| --- | ---: |
| music-only | -0.39% |
| text-heavy | -0.52% |
| mixed-pages | -0.62% |

The alias-only native cold gate passes. The report is
`reports/e29-alias-native-checkpoint-cli-timing.json`.

The pre-alias in-process native benchmark executable was not preserved before
the alias edits, so an exact alias-only native warm comparison cannot be
reconstructed honestly. E25 still covers the final native warm and cold gates
against `origin/develop`, and E27 covers the exact alias-only warm and cold
comparison for WASM. Future checkpoints must preserve both the CLI and the
in-process benchmark executable before implementation begins.

### E30–E35 — localized OpenType style metadata

Arial Narrow Bold exposed a style-classification bug: registration used the
first localized OpenType subfamily name and searched it for the English words
`bold`, `italic`, and `oblique`. A face whose name table reports German `Fett`
was consequently stored as regular, collided with the real regular face, and
left bold layout to fall back to Tinos. The fix reads the authoritative OS/2
weight and selection fields plus `head.macStyle`, retaining the old name check
only as a compatibility fallback. A deterministic test font with only the
localized subfamily `Fett` reproduces the failure without depending on a
system font.

The exact pre-fix/post-fix native report is
`reports/e30-style-metadata-native-checkpoint-timing.json`. Music-only needed
the specified one automatic rerun; the final batch passes at +0.96% cold and
-19.71% warm. Text and mixed pass on their first batches at -0.28% / -1.44%
and +0.23% / -16.80%, respectively. An earlier candidate batch produced a
large percentage swing from a 0.000124 ms absolute difference in the bounded
SVG-cache hit path. Rebuilding after review and repeating the controlled test
resolved it within the allowed rerun policy; do not use isolated sub-microsecond
cache-hit medians as evidence of a render-path regression.

Against `origin/develop`, all native timing gates pass in
`reports/e31-style-metadata-native-develop-timing.json`: cold deltas are
-8.05% / -3.52% / +0.57%, and warm deltas are -99.93% / -98.99% / -99.89%
for music, text, and mixed respectively. Native raw sizes are unchanged from
the pre-fix checkpoint; gzip grows by 0.036% for the CLI and 0.029% for the
installed bundle. The native report is
`reports/e32-style-metadata-native-size.json`.

The exact WASM checkpoint passes every gate in
`reports/e33-style-metadata-wasm-checkpoint-timing.json`; the mixed fixture
passes on its first batch, as do the other fixtures. The final deltas are
-1.05% cold / -0.46% warm for music, -3.00% / -2.48% for text, and -0.82% /
0.00% for mixed. All WASM timing gates against `origin/develop` also pass in
`reports/e34-style-metadata-wasm-develop-timing.json`. WASM delivery grows by
0.009% raw / 0.014% gzip / 0.013% Brotli from the exact checkpoint, as recorded
in `reports/e35-style-metadata-wasm-size.json`. **Conclusion:** retain the
style-metadata fix; it corrects localized static faces with effectively zero
artifact growth and passes the final native and WASM timing and size gates.

### E36–E42 — font-shaped lyric syllable connectors

Lyric syllable connectors (`syl@con="d"`) were the remaining text-like marks
drawn as synthesized rectangles. `DrawSylConnectorLines` now draws and centers
the shaped U+002D glyph using the same effective lyric family, weight, style,
size, and fallback policy as the syllable. The underline extender (`con="u"`)
remains line geometry intentionally. Layout measures the same shaped glyph that
SVG later consumes; the old second `lyricSize` adjustment was removed because
the effective `FontInfo` already includes it.

The regression fixture uses a generated face with a distinctive hyphen outline
and another face without U+002D. It verifies custom-family selection, Tinos
fallback, connectors split across encoded system breaks, and stable repeated
render counters. The corrected real score contains 47 uses of Tinos's hyphen
glyph and no synthesized lyric-hyphen rectangles; its SVG grows from 350,720 to
357,055 bytes (+1.81%).

Single-call native warm measurements initially failed twice at +49.7%, although
the measured medians differed by only 0.000083 ms and occupied adjacent clock
ticks. Native and Node/WASM warm samples now time 1,000 cached renders and divide
the duration per call, using the identical harness for baseline and head. Do not
retry single-call warm measurements for these cache-hit paths. The unchanged
+3%/+5% gates then pass for every standard native checkpoint fixture in
`reports/e36-lyric-hyphen-native-checkpoint-timing.json`; the real-score native
comparison also passes at +1.63% cold / -1.88% warm in
`reports/e42-lyric-hyphen-real-score-timing.json`.

The direct native merge-base comparison uses the same batched harness compiled
at `origin/develop`. All four fixtures pass in
`reports/e37-lyric-hyphen-native-develop-timing.json`: cold deltas are -8.80%,
-5.69%, -8.18%, and -7.93%, while warm deltas are at least -99.28% because the
feature branch retains its complete-SVG render cache.

The exact WASM checkpoint in
`reports/e39-lyric-hyphen-wasm-checkpoint-timing.json` passes all three standard
hard-gate fixtures. The additional tiny lyric-only fixture reports +3.96% warm,
or 0.000478 ms, because returning the actual path/use SVG grows that fixture by
652 bytes; this is retained as an explicit auxiliary diagnostic rather than
silently hiding the feature's serialization cost. The representative real score
passes its warm gate, and all four WASM fixtures pass against `origin/develop`
in `reports/e40-lyric-hyphen-wasm-develop-timing.json`.

Native delivery grows by 0.029% gzip for the CLI and 0.023% gzip for the
installed bundle from the exact checkpoint, as recorded in
`reports/e38-lyric-hyphen-native-size.json`. Production no-Humdrum WASM grows
0.022% raw and shrinks 0.010% gzip in
`reports/e41-lyric-hyphen-wasm-size.json`. **Conclusion:** retain the actual
font glyph implementation. It passes the prescribed standard timing and size
gates, and the isolated lyric microfixture documents the bounded output-copy
cost instead of treating it as a general render regression.

### E43 — MEI text-style inheritance checkpoint

Checkpoint commit: `f5fcc6c63` (`feat/custom-fonts` before this experiment).
The unmodified `Gott gab uns Atem_Prototyp01_Gitarrenbuch.mei` is XML-well-formed
and validates against the CMN RELAX NG schema generated from the official
`music-encoding/music-encoding` `develop` source at
`29bea8f978bf1304eea693d7eb2d14253c6c147e` for MEI `6.0-dev` on 2026-07-16.
The validation command was
`xmllint --noout --relaxng /tmp/music-encoding/dist/schemata/mei-cmn.rng FILE`.
Its relevant declarations are score-level
`text.fam="Quicksand"` and `lyric.fam="Quicksand"`, plus six `rend` overrides:
five Quicksand faces and one Arial Narrow Bold face.

At this checkpoint Verovio reads `lyric.fam` into `ScoreDefInterface` but does
not use it for lyric shaping or metrics. The valid `text.fam` is retained only
as an unsupported attribute because `ScoreDefInterface` does not implement
`AttTextStyle`. Runtime `rend` selection reads `fontname` but ignores
`fontfam`, and `Syl::GetDrawingFont` does not inherit typography from its
`verse` parent. Consequently explicit `rend@fontname` works, while harmonies,
lyrics, lyric connectors, and other score-default text continue to use the
Toolkit default face. The public rendering seam (registered fonts + MEI input
to SVG path/use output) is the regression-test boundary for this experiment.

Result: `ScoreDefInterface` now retains `AttTextStyle`, score and staff drawing
definitions propagate text and lyric family/style/weight, and text renderers
resolve an explicit runtime face for matching metrics, shaping, and outlines.
`rend@fontfam`, nested `rend` inheritance, `verse`/`syl` typography, lyric
connectors, harmonies, directions, labels, running text, and other staff text
are covered by the registration regression test. Headers and footers use the
page-boundary drawing score definition rather than whichever system happens to
be found first. The original decimal point-size issue remains deliberately
outside this experiment; the corrected copy was rendered with registered
Quicksand and Arial Narrow faces for visual verification. Both configured CTest
tests pass, and the current native CLI is 7,258,328 bytes versus 7,393,896 bytes
for the checkpoint build with the same local toolchain (no size regression).

### E44 — SMuFL fallback for missing text clusters

Checkpoint commit: `5c2f12157`. The `In dir ist Freude` test score contains
the Unicode quarter-note character `U+2669` in ordinary tempo text. Quicksand
and Tinos do not map that character, while bundled Bravura maps it directly.
The previous shaping path stopped after the Tinos fallback, retained glyph ID
zero, and emitted a `.notdef` square.

Text shaping now tries the requested text face, matching Tinos face, active
SMuFL face, configured music fallback, and finally Bravura. The shape-cache key
includes every participating face, so changing the active or fallback music
font cannot return a stale mixed-font run. Regression coverage proves both an
active custom SMuFL fallback and the final Bravura fallback, using their actual
face identities and glyph IDs. Both configured CTest tests pass. No outlines
are extracted until the resolved music glyph is drawn, and repeated rendering
continues to reuse the shaped run.

### E45 — Stable boxed-text vertical metrics

Checkpoint commit: `1ffcba223`. In the `In dir ist Freude` render, the boxed
Quicksand word `Intro` produced a 367-unit rectangle while `Strophen` produced
441 units. The enclosure used only each `rend` element's content bounds, so the
`p` descender enlarged one box while a word without descenders looked crushed
against its baseline.

Each enclosed `rend` now records a font-specific minimum vertical span while
its resolved runtime font and baseline are active. The lower bound uses the
`p` descender and the upper bound uses the larger of the `h` and `t` ascender
profiles; actual content still expands the enclosure for accents or unusual
glyphs. The MEI-to-SVG regression compares `Intro` and `Strophen` and reduces
their Tinos box-height difference from the content-dependent failure to two
device units (rounding tolerance: four). These lookups populate only the lazy
metrics cache and do not extract outlines.

### E46 — Missing text-style faces stay in the requested family

Checkpoint commit: `1c0df0456`. The test score explicitly encodes
`tänzerisch` as Quicksand Italic, but the installed static Quicksand family has
no italic face. Exact face lookup therefore failed and the shaping wrapper
silently switched the complete run to Tinos Italic. The MEI encoding was not
at fault.

Text face matching now prefers an exact face, then synthesizes a missing
italic or bold style from the nearest registered face in the same family.
HarfBuzz applies the synthetic style to shaping metrics, extents, and the
lazily requested outline. Synthetic variants have stable identities and share
a process-wide weak cache; repeated lookup returns the cached variant before
considering fallback candidates. A regression proves that an italic `rend`
uses a distinct, actually slanted outline from its requested family rather
than the matching Tinos face. Rendering the real score with only Quicksand
Regular and Bold registered produces a Quicksand-derived synthetic face for
`tänzerisch`.

The same-build native CLI changed from 7,259,208 to 7,260,600 bytes (+0.019%);
gzip changed from 2,437,809 to 2,441,144 bytes (+0.137%). Cold timing passed
for music-only (-2.49%), text-heavy (+0.95%), and mixed-page (+3.33%) fixtures.
Warm timing passed for music-only (0.00%) and mixed-page (+1.46%). The standard
1,000-render text-heavy batch reported 0.808 versus 0.962 microseconds per
cached render (+19.07%) after its automatic rerun, so the failed gate is
preserved in `reports/e46-synthetic-style-native-timing.json`. A diagnostic
100,000-render batch measured 0.717 versus 0.730 microseconds (+1.81%), within
the 3% gate; the absolute discrepancy is sub-microsecond benchmark noise, not
a meaningful render-time accumulation. No threshold was relaxed.
