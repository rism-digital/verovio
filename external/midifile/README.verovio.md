# Vendored midifile

- Upstream: <https://github.com/craigsapp/midifile>
- Commit: `51eec11fe4f8e05bf4ce36ccc7d7e179f174291a` (2025-02-16)
- License: BSD 2-Clause, see `LICENSE.txt`

Only the library headers (`include/`) and sources (`src/`) are present. The
tools, build files, and Visual Studio project are not included. The
`.download` scripts fetch the files from the upstream `master` branch into the
directory they are run from.

## Local modifications

- `src/MidiMessage.cpp`: `MidiMessage::makeSysExMessage()` and
  `MidiMessage::makeMts2_KeyTuningsBySemitone()` do not write the
  variable-length size themselves, because `MidiFile::write()` already does it
  for system exclusive messages. Running the `.download` scripts overwrites
  this change.
