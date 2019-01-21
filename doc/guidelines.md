# Coding guidelines

This document describes the coding style for the Verovio project.

## Table of Contents

* [General remarks](#general-remarks)
* [Includes and forward declarations](#includes-and-forward-declarations)
* [Null and boolean](#null-and-boolean)
* [Class, method and member names](#class-method-and-member-names)
* [Comments](#comments)
* [LibMEI](#libmei)

## General remarks

Verovio uses a [Clang-Format](http://clang.llvm.org/docs/ClangFormat.html) (**5.0**) coding style based on the [WebKit](https://webkit.org/code-style-guidelines/) style, with a few minor modifications. The modifications include:

    AllowShortIfStatementsOnASingleLine: true
    AllowShortLoopsOnASingleLine: true
    ColumnLimit: 120
    ConstructorInitializerAllOnOneLineOrOnePerLine: true
    PointerAlignment: Right

The simplest way to fullfil the Verovio coding style is to use a clang-format tool and to apply the style defined in the 
[.clang-format](../.clang-format) file available in the project root directory.

### Downloading clang-format for OS X

An easy way to install clang-format on OS X computers is to use [Hombrew](http://brew.sh).  Type this command in the terminal to install:

```
brew install clang-format
```

### Running clang-format

*Please make sure you use version 5.0*

To use clang-format to adjust a single file:

```
clang-format -style=file -i   some-directory/some-file.cpp
```

The `-style=file` option instructs clang-format to search for the .clang-format configuration file (recursively in some parent directory).  The `-i` option is used to alter the file "in-place".  If you don't give the `-i` option, a fomatted copy of the file will be sent to standard output.


## Includes and forward declarations

Includes in the header files must list first the system includes followed by the Verovio includes, if any, and then the includes for the libraries included in Verovio. All includes have to be ordered alphabetically:

    #include <string>
    #include <utility>
    #include <vector>
    
    //----------------------------------------------------------------------------
    
    #include "attclasses.h"
    #include "atttypes.h"

    //----------------------------------------------------------------------------  
    
    #include "pugixml.hpp"
    #include "utf8.h"

In the header files, always use forward declarations (and not includes) whenever possible. Forward declaration have to be ordered alphabetically:

    class DeviceContext;
    class Layer;
    class StaffAlignment;
    class Syl;
    class TimeSpanningInterface;


In the implementation files, the first include in always the include of the corresponding header file, followed by the system includes and then the other Verovio includes with libraries at the end too, if any, also ordered alphabetically:

    #include "att.h"
    
    //----------------------------------------------------------------------------
    
    #include <sstream>
    #include <stdlib.h>
    
    //----------------------------------------------------------------------------
    
    #include "object.h"
    #include "vrv.h"
    
    //----------------------------------------------------------------------------  
    
    #include "pugixml.hpp"
    
## Null and boolean

The null pointer value should be written as ```NULL```. Boolean values should be written as ```true``` and ```false```.

## Class, method and member names

All class names must be in upper CamelCase. The internal capitalization follows the MEI one:

     class Measure;
     class ScoreDef;
     class StaffDef;
     

All method names must also be in upper CamelCase:

     void Measure::AddStaff(Staff *staff) {}
     
All member names must be in lower camelCase. Instance members must be prefixed with ```m_``` and class (static) members with ```s_```:

    class Glyph {
    public:
    
        /** An instance member */
        int m_unitsPerEm;
        
        /** A static member */
        static std::string s_systemPath;
    };
    
In the class declaration, the methods are declared first, and then the member variables. For both, the declaration order is ```public```, ```protected```, and ```private```.
    
## Comments

Comments for describing methods can be grouped using `///@{` and `///@}` delimiters together with the `@name` indication:

    /**
     * @name Add children to an editorial element.
     */
    ///@{
    void AddFloatingElement(FloatingElement *child);
    void AddLayerElement(LayerElement *child);
    void AddTextElement(TextElement *child);
    ///@}
    
## LibMEI

The code for the attribute classes of Verovio are generated from the MEI schema using a modified version of LibMEI available [here](https://github.com/rism-ch/libmei). The code generated is included in the Verovio repository in ./libmei and the LibMEI repository does not need to be cloned for building Verovio.

The attribute classes generated from the MEI schema provide all the members for the element classes of Verovio. They are implemented via multiple inheritance in element classes. The element classes corresponding to the MEI elements are not generated by LibMEI but are implemented explicitly in Verovio. They all inherit from the `Object` class (of the `vrv` namespace) or from a `Object` child class. They can inherit from various interfaces used for the rendering. All the MEI member are defined through the inheritance of generated attribute classes, either grouped as interfaces or individually.

For example, the MEI &lt;note&gt; is implemented as a `Note` class that inherit from `Object` through `LayerElement`. It also inherit from the StemmedDrawingInterface that holds data used for the rendering.

Its MEI members are defined through the `DurationInterface` and `PitchInterface` that regroup common functionnalities for durational and pitched MEI elements respectively plus some additional individual attribute classes.

The inheritance should always list `Object` (or the `Object` child class) first, followed by the rendering interfaces, followed by the attribute class interfaces, followed by the individual attribute classes, each of them ordered alphabetically:

    class Note : public LayerElement,
                 public StemmedDrawingInterface,
                 public DurationInterface,
                 public PitchInterface,
                 public AttColoration,
                 public AttGraced,
                 public AttStems,
                 public AttTiepresent
                 
In the implementation, the same order must be followed, for the constructor calls and for the registration of the interfaces and individual attribute classes:

    Note::Note()
        : LayerElement("note-")
        , StemmedDrawingInterface()
        , DurationInterface()
        , PitchInterface()
        , AttColoration()
        , AttGraced()
        , AttStems()
        , AttTiepresent()
    {
        RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
        RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
        RegisterAttClass(ATT_COLORATION);
        RegisterAttClass(ATT_GRACED);
        RegisterAttClass(ATT_STEMS);
        RegisterAttClass(ATT_TIEPRESENT);

        Reset();
    }

Resetting the attributes is required and follows the same order

    void Note::Reset()
    {
        LayerElement::Reset();
        StemmedDrawingInterface::Reset();
        DurationInterface::Reset();
        PitchInterface::Reset();
        ResetColoration();
        ResetGraced();
        ResetStems();
        ResetTiepresent();
        
        // ...
    }
