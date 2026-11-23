# Vendored CRC implementation

- Origin: `crc.c` and `crc.h` by Michael Barr, published with the article
  "CRC Implementation Code in C/C++"
  (<https://barrgroup.com/blog/crc-series-part-3-crc-implementation-code-cc>)
- Version: not determinable; the code is not maintained in a public repository
- License: public domain, see the notice at the top of both files. The
  original distribution has no separate license file.

## Local modifications

The modifications cannot be listed exactly because the original version is
not identified. Known changes:

- `crc.c` is compiled as C++ under the name `crc.cpp`, with the functions in
  an `extern "C"` block.
- The scope of some local variables in `crc.cpp` is reduced.
