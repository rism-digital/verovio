Verovio is a fast, portable and lightweight library for engraving Music Encoding Initiative (MEI) music scores into SVG. Verovio can also render Plain and Easy (PAE) code and DARMS code. It is designed as a tool usable as a one-step conversion tool and it is written in pure standard C++ with no dependencies on third-party frameworks and libraries.

The main project page is at http://www.verovio.org.

Verovio can be used as a standalone command-line tool for converting a files, as the music rendering library for applications, or it can be compiled to Javascript using the Emscripten LLVM-to-JavaScript compiler.

Verovio is available under the GPL license, see LICENSE.txt.

LibMEI
------

The code for the attribute classes of Verovio are generated from the MEI schema using a modified version of LibMEI available [here](https://github.com/rism-ch/libmei). The code generated is included in the Verovio repository and the LibMEI repository does not need to be cloned for building Verovio.

Example output
--------------

![Example page](/images/page.png)
