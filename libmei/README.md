LibMEI
------

LibMEI is a C++ library for reading and writing [MEI](http://music-encoding.org) files

It was originally developed by the [Distributed Digital Music Archives and Libraries Lab](http://ddmal.music.mcgill.ca/) at the Schulich School of Music at McGill University, Montréal, Canada

This is a modified version that is used for generating C++ code for Verovio. The main differences with LibMEI are:

1. it generates only attribute classes (Mixin in LibMEI) by default,
2. each attribute has a C++ type deduced from the MEI schema or given in a separated configuration file,
3. it uses the MEI page-based customization not included in the MEI 2013 v2.1.0 (see [MEI](http://www.music-encoding.org)).


License
-------
LibMEI is released under the MIT license.

Compilation & Usage
-------------------

To generate the code, do:

    python3 tools/parseschema2.py  mei/develop/mei-verovio_compiled.odd

Other options are:
* `-o`: specific output directory. It will contain `dist` with the generated files and `addons` with the other files
* `-e`: will generate element classes in addition to the attribute classes. This option disables the generation of the Verovio specific code `attmodule.h/cpp`
* `-ns`: specify the CPP namespaces to be used. This option should only be used with `-o` and `-e`.
