# Surge Synth Team Tuning Library 

In [Surge](https://surge-synthesizer.github.io), we added microtuning 
and spent a lot of time making sure our
SCL/KBM implementation was properly calibrated and available for C++
programs. We then added that same implementation to [dexed](https://asb2m10.github.io/dexed/)
through a copy.

But we realized we could make the functions available as standalone C++ header
only library and get three benefits.

1. Share more code between Surge and our Dexed fork
2. Make the code available to other soft synths where we or others may add microtuning like our tuning workbench synth 
3. Have a set of standalone command line utilities and well documented tests

So we took the code and re-factored it here under an MIT license.

Although Surge and Dexed are GPL3, the copyright holders and authors of the original
Surge microtuning implementation (and only that implementation) were happy to relicense.

## Using the library in your C++ project

The C++ library is a standalone header only C++-11 library. There are a variety of ways
to use it in your project but our approach is generally:

1. Make this github repo a submodule of your project
2. Add the "include/" directory to your compiler include path
3. `#include "Tunings.h"`

If you use cmake you can also do

```cmake
add_subdirectory(this/library EXCLUDE_FROM_ALL)
```

and then add the target `tuning-library` as a library target to your project.

The code is organized such that Tunings.h is the API and TuningsImpl.h is the header with more 
involved implementation bodies. Tunings.h includes TuningsImpl.h automatically.

## Building the command line tools and test suite

```shell
cmake -Bbuild
cmake --build build --target run-all-tests
cmake --build build --target showmapping
cmake --build build --target parsecheck
```

## Using the showmapping command

`showmapping` takes one or two arguments. It either takes an .scl file, in which
case it dumps the frequency table for that .scl file with midi note 60 being the
scale start tuned to 261hz, or it takes an .scl and .kbm file, in which case it
prints the entire internal tuning table for the combination.

## Using the parsecheck command

`parsecheck` takes a list of files and tries to parse them showing errors if it 
fails. `parsecheck test/data/*scl` will test that all those SCL parse.

## Related Projects

Github user @chinenual has ported this library to `go` here: https://github.com/chinenual/go-scala/

Python bindings for this library are available
[here](https://github.com/surge-synthesizer/tuning-library-python).

## Bugs, Problems, etc

If you find bugs, please open a github issue and we will fix it right away!

If you have a non-bug problem, you can do the same or you can hop on the slack as
detailed at https://surge-synth-team.org/

If you would like to expand our test cases, we are always thrilled for you to do
so. Drop in a pull request.

If you choose to use the software in your synth, you can go right ahead of course.
That's the point of the MIT license! But if you want to let us know, again pop open
a github or drop in our slack. Always glad to hear from you.

Enjoy!
