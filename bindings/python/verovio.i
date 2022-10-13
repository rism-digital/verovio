/////////////////////////////////////////////////////////////////////////////
// Name:        verovio.i
// Author:      Laurent Pugin
// Created:     09/11/2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

// Change method names to lowerCamelCase
%rename("%(lowercamelcase)s") "";
// Ignore enum items (e.g., for fileFormat.PAE)
%rename("%(upper)s", %$isenumitem) "";

// Method to ignore
%ignore vrv::Toolkit::GetShowBoundingBoxes( );
%ignore vrv::Toolkit::GetCString( );
%ignore vrv::Toolkit::GetLogString( );
%ignore vrv::Toolkit::GetOptionsObj( );
%ignore vrv::Toolkit::ResetLogBuffer( );
%ignore vrv::Toolkit::SetShowBoundingBoxes( bool );
%ignore vrv::Toolkit::SetCString( const std::string & );

// Because we transform the strings to dictionnaries, we need this module
%pythonbegin %{
    import json
%}

%feature("shadow") vrv::Toolkit::GetOptions %{
def getOptions(*args):
    if len(args) < 2: return {}
    optionsStr = $action(args[0], args[1])
    return json.loads(optionsStr)
%}


%feature("shadow") vrv::Toolkit::SetOptions( const std::string & ) %{
def setOptions(*args):
    return $action(args[0], json.dumps(args[1]))
%}

%module(package="verovio") verovio
%include "std_string.i"
%include "../../include/vrv/toolkit.h"

%{
    #include "../../include/vrv/toolkit.h"

    using namespace vrv;
    using namespace std;
%}
