# Vendored miniz-cpp

- Upstream: <https://github.com/kkzi/miniz-cpp>, file
  `include/miniz_cpp.hpp`, commit `62ec5c46c276d1880ba3b5a121d5a5dc24667b9e`.
  This repository is no longer available. It was a fork of
  <https://github.com/tfussell/miniz-cpp>, which distributes the header as
  `zip_file.hpp` in the repository root.
- Embedded: miniz 2.2.0 by Rich Geldreich
- License: MIT for miniz-cpp, see the notice at the top of `zip_file.hpp`.
  The embedded miniz code contains both MIT-licensed parts (copyright RAD Game
  Tools, Valve Software, Rich Geldreich, Tenacious Software LLC, and Martin
  Raiber) and public-domain parts; all original notices are retained in
  `zip_file.hpp`. No separate license file is included, because the upstream
  commit cannot be retrieved anymore.

Only the single header `zip_file.hpp` is present.

## Local modifications

Changes made in Verovio since the import of the upstream commit:

- The file is named `zip_file.hpp` instead of `miniz_cpp.hpp`.
- Uses of the comma operator are removed.
- A `#pragma message` about the file I/O path is commented out.
