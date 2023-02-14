LibMEI
------

LibMEI is a C++ library for reading and writing [MEI](http://music-encoding.org) files

It was originally developed by the [Distributed Digital Music Archives and Libraries Lab](http://ddmal.music.mcgill.ca/) at the Schulich School of Music at McGill University, Montréal, Canada

This is a modified version that is used for generating C++ code for Verovio.

1. it generates only attribute classes by default,
2. each attribute has a C++ type deduced from the MEI schema or given in a separated datatypes configuration file,
3. it uses the MEI page-based customization by default.


License
-------
LibMEI is released under the MIT license.

Compilation & Usage
-------------------

To generate the code, do:

    python3 parseschema2.py ../mei/develop/mei-verovio_compiled.odd

Where the positional argument points to an ODD file for which you wish to
generate code.

Other options are:
* `-c`: A path to a YML config file.

Config file
-----------

The config file can contain several options:

```yaml
debug: true|false  # determines the log level while running 
output_dir: "../dist"  # path to where the output should go
addons_dir: "../addons"  # path to an optional addons directory
elements: true|false  # whether or not code for element handling should be generated   
namespace: "vrv"  # the namespace to use in generated CPP code
datatypes: "./datatypes.yml"  # path to a datatypes mapping file
basic_odd: "../mei/develop/mei-basic_compiled.odd"  # path to an MEI basic ODD file
```

For the `basic_odd` option, if provided the generator will generate a map of notes and allowed
attributes within the MEI basic ODD file and write it to a file called `meibasic.h`. 

If the `addons_dir` is provided, the files in that directory will be copied to the output directory.
The files will also have the namespace in the addons replaced with the value provided in the
`namespace` option.