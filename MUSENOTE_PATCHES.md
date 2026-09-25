# MuseNote patch series

This branch is Verovio `version-6.2.0` (43f806031) plus the patches listed
below. They are carried for the MuseNote and MuseSync apps. Every other
file is unmodified upstream Verovio, under the same LGPL-3.0 licence.

## Patches

| # | Date | Commit | Files | Output change | Upstream status |
| --- | --- | --- | --- | --- | --- |
| 1 | 2026-09-25 | Fix two crashes in the PAE importer on malformed input (cherry-picked from 82a6aa3e5) | `src/iopae.cpp`, +13 / -5 | None for multi-line (`@clef:`) Plaine & Easie. A one-line (`%`) input with a key or time signature after the clef now reads them as the opening signatures instead of a change after an empty opening; ids of such input change, pitches do not | Offered as rism-digital/verovio#4450 and withdrawn; not upstream |
| 2 | 2026-09-25 | Keep an explicit xmlIdSeed on a thread that has not built an object yet (95ad8e834) | `include/vrv/object.h`, `src/object.cpp`, +10 / -2 | None for a load on a thread that already built a Verovio object. A seeded load on a fresh thread now keeps the seed (it got random ids) | Not offered upstream |

### 1. PAE importer crashes

- `PAEInput::CheckHierarchy` started each pass with an empty container
  stack, so content before the first measure token read `stack.back()`
  of an empty list (SIGSEGV). Each pass now starts with the layer token
  on the stack, and an unmatched closing token never pops it.
- `PAEInput::SingleLineToJson` built the data string from `end() + 1`
  when a one-line input had no space after the scoreDef (abort), and
  dereferenced past the end on a trailing space. Both are guarded; a
  line that is only a scoreDef has empty data. The `||` tautology in the
  scoreDef-end test is now `&&`, as the code comment intends.

Regression inputs: `@clef:G-2` + `@data:{=6}B` (was SIGSEGV) and `%G-2`
(was SIGABRT) now load and render.

### 2. Explicit id seed on a fresh thread

Id counters are `thread_local`, and the first object built on a thread
reseeded them randomly. A `setOptions` seed on a thread that had not built an
object yet was therefore replaced by the first object of the next `loadData`,
and that load got random ids. MuseNote's engine worker is a Dart isolate,
which can run a message on another OS thread than the one that built its
toolkit; its annotations are anchored to seeded ids. `SeedID` now marks the
thread seeded, and the first-object reseed runs only on a thread never seeded.

Regression test: MuseNote's `corelib/verovio/tool/parity/seedthread.cpp`
(toolkit built on one thread, seeded import on a fresh thread, ids compared)
fails on tb.1 and passes on tb.2.

## Verification, 2026-09-25

tb.2 against tb.1 (same toolchain): 972 outputs compared, 0 differ; the
fresh-thread seeded import matches on 3 of 3 scores (0 of 54 notes matched on
tb.1).

tb.1 against `version-6.2.0`:

- 209 MusicXML and MEI scores: SVG, timemap and MIDI byte-identical to
  `version-6.2.0`, so no note id and no pitch changed.
- 86 PAE inputs (the 78 in `doc/tests/pae` plus 8 regression inputs):
  none crash. Every output the unpatched engine produced is identical
  apart from the MEI export timestamp, except the one-line form noted
  above.

## Rules for this branch

- A patch is carried only when the change cannot be made outside the
  engine. Speed work and new features live in the app's own code, not
  here.
- At most about five patches of about 30 lines each. None may change the
  ids or the output of a seeded load on one thread — in practice, nothing
  in drawing (`view_*`), layout functors, or importer object construction.
- Every patch lists its date, why, test input, output change and
  upstream status in this file.
- Tags `musenote-<base>-tb.<n>` mark exact shipped states. They are
  never moved or deleted.
- A move to a newer upstream release starts a new `musenote/<base>`
  branch from that release's tag and re-applies this series.
