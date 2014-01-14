Verovio is a fast, portable and lightweight library for engraving Music Encoding Initiative (MEI) music scores into SVG. Verovio can also render Plain and Easy (PAE) code and DARMS code. It is designed as a tool usable as a one-step conversion tool and it is written in pure standard C++ with no dependencies on third-party frameworks and libraries. This ensures maximum portability of the codebase. Verovio implements its own rendering engine, which can render SVG with all the musical symbols embedded in it and without dependency on external fonts. Since the rendering engine is defined as an abstract class, it is possible easily to implement a rendering output different from SVG if needed (e.g., PDF, or HTML Canvas).

It can be used as a standalone command-line tool for converting a files, as the music rendering library for applications, or it can be compiled to Javascript using the Emscripten LLVM-to-JavaScript compiler.

The main project page is http://www.verovio.org

Verovio is available under the GPL license, see LICENSE.txt.
