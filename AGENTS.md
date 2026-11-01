# Agent guidelines for Verovio

These instructions apply throughout this repository. Keep changes focused and follow the surrounding code rather than
introducing a new style or architecture. Preserve unrelated local changes and untracked files.

## Project and sources of guidance

Verovio is a portable C++20 music-notation engraving library and command-line tool, with multiple language bindings.
Read the relevant implementation and these references before making changes:

- [Coding guidelines](https://book.verovio.org/contributing/guidelines.html)
- [Contribution workflow and test examples](https://book.verovio.org/contributing/workflow.html)
- [LibMEI generation](https://book.verovio.org/contributing/generate-code-with-libmei.html)
- `README.md`, `.clang-format`, and the relevant workflows in `.github/workflows/`.

Use the checked-in configuration and code for exact paths and commands; the reference book can describe older layouts.
Contributions normally target `develop`. Do not open issues, publish reviews, or create PRs unless asked.

## Code map

- `include/vrv/` and `src/`: hand-written core classes, interfaces, layout functors, import/export, and rendering.
- `src/iomei.cpp`: MEI import/export; other `io*.cpp` files handle other formats.
- `src/*functor.cpp`: traversal and processing passes. Fix behavior in the appropriate pass rather than adding unrelated
  special cases to drawing code.
- `include/vrv/toolkit.h` and `src/toolkit.cpp`: toolkit API; `tools/main.cpp`: CLI;
  `include/vrv/options.h` and `src/options.cpp`: options and defaults.
- `libmei/`: schema inputs, Python generator, generated attributes, and hand-written attribute helpers (see below).
- `data/`: runtime engraving resources; `fonts/`: font sources; `bindings/` and `emscripten/`: language/platform integration.
- `doc/test-suite*.py` and `.github/workflows/tests_build.yml`: regression evaluation.
- Embedded third-party libraries live under directories such as `src/hum`, `src/midi`, `src/pugi`, and corresponding include
  directories. Avoid modifying or reformatting vendored code unless the task requires it.

## C++ style and implementation

- Compile as C++20. The `Standard: Cpp11` entry in `.clang-format` is a formatting setting, not the build language standard.
- Use `.clang-format` and clang-format **22**, matching CI. Format only the hand-written files you change, not the entire
  repository or generated/vendor code. Use four spaces, no tabs, and a 120-column limit.
- Use UpperCamelCase for classes and methods, lowerCamelCase for local variables, `m_` for instance members, and `s_` for
  static members. Preserve MEI element names in CapWords style in names such as `ScoreDef` (MEI `<scoreDef>`).
- Follow the project's conventions: `NULL` for null pointers, `true`/`false` for booleans, and explicit types rather than
  `auto`. Prefer `int`; avoid `short` and unsigned types unless needed. Use explicit container-size conversions where
  appropriate, but do not introduce narrowing or overflow; local container operations may use `size_t`.
- Keep loop variables local and prefer pre-increment. Short single-line `if` statements should have a single boolean
  evaluation; use blocks for more complex conditions.
- In headers, group includes as system, Verovio, then embedded libraries; alphabetize within each group. Prefer
  alphabetized forward declarations when a complete type is unnecessary. In `.cpp` files, include the matching header
  first, then the same groups. Separate include groups with standard comment lines (`//----------------------------------------------------------------------------`) and follow existing header guards.
- Put core implementation in `namespace vrv`. **Do not add empty namespace blocks at global scope**, whether named or
  anonymous (`namespace {}`). Do not leave an empty block after moving or removing code.
- Use `this->` for member method calls, not for `m_` member access. Declare methods before data members, with access groups
  ordered `public`, `protected`, `private`; group static methods and place their implementations at the end.
- For element classes, list the `Object`-derived base first, then drawing interfaces, attribute interfaces, and individual
  attribute classes, alphabetized within each group. Keep constructor initialization, interface/attribute registration,
  and `Reset()` calls consistent with that inheritance order. In `Reset()`, reset base classes, interfaces, and individual attributes (using `Reset<AttName>()` for attributes, e.g. `ResetColoration()`). Reset new state and check clone/reset behavior.
- Respect existing object-tree ownership and traversal semantics. Do not introduce double ownership, dangling pointers,
  unchecked casts, or mutations that invalidate traversal. Use the existing casting and interface-access patterns.
- Validate external input and handle unsupported/missing data gracefully; assertions are not a substitute for input
  validation. Avoid unnecessary API changes and check effects on bindings when changing public interfaces or options.
- Explain non-obvious engraving rules and invariants in comments; use the surrounding Doxygen style for API documentation.
  For grouping related method comments in Doxygen, use `/** @name ... */` with `///@{` and `///@}` delimiters.
  Avoid unrelated cleanup, speculative abstractions, and broad modernization.

## LibMEI: generated versus hand-written code

A normal Verovio build compiles the checked-in LibMEI output. It does **not** require cloning another LibMEI repository or
running the generator. `cmake/CMakeLists.txt` compiles both `libmei/dist/*.cpp` and `libmei/addons/*.cpp`.

**Never hand-edit `libmei/dist/`.** Its generated files include:

- `atts_*.cpp` / `atts_*.h`: schema-derived attribute classes, readers, and writers.
- `attconverter.cpp` / `attconverter.h`: generated value converters.
- `attmodule.cpp` / `attmodule.h`: generated attribute dispatch/copy support.
- `attclasses.h`, `atttypes.h`, and `meibasic.h`: attribute identifiers, data types, and MEI Basic allowlists.

In contrast, `libmei/addons/att.cpp`, `att.h`, `attalternates.h`, `attdef.h`, and `element.h` are hand-written support files.
Element classes such as `src/note.cpp` and `include/vrv/note.h` are also hand-written, not LibMEI output. Adding an attribute
to an element can require hand-written inheritance, registration, and reset changes as well as regenerated attributes.

For generation changes, modify the source of truth:

- `libmei/mei/mei-verovio.xml`: Verovio MEI customization.
- `libmei/mei/mei-verovio_compiled.odd`: compiled customization used by the generator.
- `libmei/mei/mei-basic_compiled.odd`: input for the Basic allowlist.
- `libmei/datatypes.yml`: explicit data-type mappings.
- `libmei/tools/schema.py` and `libmei/tools/cpp.py`: parsing and C++ generation/templates.
- `libmei/config.yml`: output/configuration (`elements: false`, `namespace: vrv`, `output_dir: ./dist`).

If changing the customization, rebuild its compiled ODD using the MEI toolchain or MEI Garage as described in the reference
book, then update the checked-in input. This checkout uses `libmei/mei/`, not the older `libmei/mei/develop/` path.
Do not treat a compiled ODD as the sole source of a customization change.

To regenerate, run **from `libmei/`**:

```sh
poetry install
poetry run python3 tools/parseschema2.py -c config.yml ./mei/mei-verovio_compiled.odd
```

**Warning:** `parseschema2.py` deletes and recreates the configured output directory. Check the configuration and local
changes before running it. Do not regenerate for unrelated tasks. Review the resulting diff, include the relevant inputs
and generated output together, and rebuild Verovio. With the current `elements: false` configuration, addons are compiled
directly from `libmei/addons/`, not copied into `dist/`.

Other generated artifacts, including `include/vrv/git_commit.h` and SWIG wrapper outputs, must not be hand-edited either.

## Build and verification

The CMake source directory is `cmake/`, not the repository root. From the repository root, a separate CLI build can use:

```sh
cmake -S cmake -B build/cli -DCMAKE_BUILD_TYPE=Debug
cmake --build build/cli --parallel 2
build/cli/verovio --help
```

Reuse an appropriate existing build when possible; do not overwrite another build's configuration. Adjust parallelism to
available memory. CMake configuration generates `include/vrv/git_commit.h`. For an uninstalled CLI, explicitly use `-r data`
so it loads the resources from this checkout. Consult the executable's help and `src/options.cpp` for actual flag names;
CLI flags normally use kebab-case while toolkit JSON keys use camelCase.

Run a targeted reproduction after behavior changes, and compare before/after output where possible. A successful compile
alone does not verify engraving, import/export, MIDI, or layout behavior. Report exactly which commands were run and any
tests that were unavailable; do not claim a reproduction or test passed without running it.

The main MEI regression corpus lives in the **separate** `rism-digital/verovio.org` repository (`gh-pages` branch, `_tests/`),
not in a root-level test directory here. The test workflow also uses its `musicxmlTestSuite`. With that corpus and a Python
binding built from the code being tested, `doc/test-suite.py` renders SVG/PNG and produces MIDI/timemap outputs:

```sh
python doc/test-suite.py /path/to/verovio.org/_tests /path/to/existing-output-dir --shortlist /path/to/shortlist.txt
```

The shortlist lists relative filenames, one per line (for example, `accid/accid-001.mei`). The runner requires CairoSVG and
its runtime dependencies. Use `doc/test-suite-diff.py` for before/after comparisons and `doc/test-suite-roundtrip.py` for
MEI round-trip evaluation; see their arguments and the CI workflow. For Python packaging changes, also run
`doc/python-smoke-test.py` against the newly built, installed wheel, not an unrelated system installation.

## Required PR-review reproduction artifacts

**Every PR review must include a minimal MEI test file and complete command-line instructions for human verification.**
For each reported behavioral problem, create a self-contained `.mei` file that actually exercises it; do not merely refer
to a large score, describe hypothetical XML, or provide a screenshot. Also exercise the PR's intended behavior when no
regression is found. For a strictly non-behavioral PR, provide a minimal rendering smoke-test MEI and explicitly state
that there is no behavioral defect being reproduced; never invent a problem.

- Prefer one or two measures with only the necessary staves, voices, and notation. Use the MEI namespace, an appropriate
  `meiversion`, and the required document structure. Use stable `xml:id` values for references and three-space XML
  indentation. Keep examples valid MEI unless malformed input is the specific behavior under test; identify that exception.
- Provide the actual file at a clearly named path, plus copy-pasteable commands run from a stated working directory.
  Include **all flags needed** to exhibit the behavior: resource directory, input/output format, font, page dimensions,
  breaks, spacing, scale, and any feature-specific options that matter. Do not rely on unstated local defaults.
- State the base and PR revisions/binaries, expected behavior, observed behavior, and the exact output/page/measure to
  inspect. When possible, run identical options against both versions and write separate outputs. Include relevant logs
  and exit status for crashes or parser errors. Distinguish executed tests from proposed commands.
- A basic CLI command, to be extended with the flags required by the particular problem, is:

  ```sh
  build/cli/verovio -r data -t svg -o /path/to/repro.svg /path/to/repro.mei
  ```

- Verify behavior-changing flag combinations separately, and provide minimal variants/negative controls where needed to
  establish the scope. Inspect the SVG visually for engraving issues; generated XML or successful rendering alone is not
  proof of correct appearance. If visual inspection is unavailable, say so and identify what a human should check.
- If the problem involves MusicXML, Humdrum, another importer, or a binding/API, retain the minimal original-format input
  or API driver **in addition to** the MEI example. A converted MEI alone cannot reproduce an importer or binding defect.
- Keep review artifacts separate from production code. State where they were saved; do not commit generated renders or
  add examples to the external corpus without being asked. If proposing a corpus test, use its element-based directory
  and `element-NNN.mei` naming/header conventions. Corpus-specific options can be embedded as JSON CDATA in
  `meiHead/extMeta`; the test runner reads these, but still supply explicit CLI flags for human reproduction.

Conclude reviews with actionable findings tied to source locations and reproduction artifacts, followed by verification
performed and remaining limitations. Prefer demonstrated defects over speculative style complaints.
