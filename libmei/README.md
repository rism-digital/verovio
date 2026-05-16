LibMEI
------

LibMEI is a C++ library for reading and writing [MEI](https://music-encoding.org/) files

It was originally developed by the [Distributed Digital Music Archives and Libraries Lab](https://ddmal.music.mcgill.ca/) at the Schulich School of Music at McGill University, Montréal, Canada

This is a modified version that is used for generating C++ code for Verovio.

1. it generates only attribute classes by default,
2. each attribute has a C++ type deduced from the MEI schema or given in a separated datatypes configuration file,
3. it uses the MEI page-based customization by default.

License
-------

LibMEI is released under the MIT license.

Compilation & Usage
-------------------

To install you can use Python `poetry` which will create a virtual environment and automatically
install the necessary dependencies. The first time you get things set up, from the `libmei` directory run:

```shell
poetry install
```

This will set up a virtual environment for you. When you want to activate the environment, run

```shell
poetry env activate
```

This will show you the command to activate the virtual environment.

To generate the code, activate the poetry environment and, from the `libmei` directory, run:

```shell
python3 tools/parseschema2.py ./mei/mei-verovio_compiled.odd
```

If you don't want to activate the virtual environment manually, use:

```shell
poetry run python3 tools/parseschema2.py ./mei/mei-verovio_compiled.odd
```

Where the positional argument points to an ODD file for which you wish to generate code.

Other options are:

* `-c`: A path to a YML config file. If not specified this will look for a file called "config.yml"
        in your current working directory. (provided by default, but you can make your own if you
        have custom requirements.)

Config file
-----------

The config file can contain several options:

```yaml
debug: true|false  # determines the log level while running 
output_dir: "../dist"  # path to where the output should go
addons_dir: "../addons"  # path to an optional addons directory
elements: true|false  # whether code for element handling should be generated   
namespace: "vrv"  # the namespace to use in generated CPP code
datatypes: "./datatypes.yml"  # path to a datatypes mapping file
# path to an MEI Basic ODD file. If not provided then the meibasic.h file will not
# be written. (This contains a map of the elements and attributes allowed in MEI Basic
# which is then used by Verovio to ensure the "full" MEI output is stripped when the user
# requests "Basic" output.)
basic_odd: "../mei/develop/mei-basic_compiled.odd"   
```

For the `basic_odd` option, if provided the generator will generate a map of notes and allowed
attributes within the MEI Basic ODD file and write it to a file called `meibasic.h`.

If the `addons_dir` is provided, the files in that directory will be copied to the output directory.
The files will also have the namespace in the addons replaced with the value provided in the
`namespace` option.
