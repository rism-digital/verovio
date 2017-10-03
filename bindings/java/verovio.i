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
%ignore vrv::Toolkit::ParseOptions( const std::string & );
%ignore vrv::Toolkit::ResetLogBuffer( );
%ignore vrv::Toolkit::SetShowBoundingBoxes( bool );
%ignore vrv::Toolkit::SetCString( const std::string & );

%module verovio
%include "std_string.i"
%include "../../include/vrv/toolkit.h"


%{
    #include "../../include/vrv/toolkit.h"
    
    using namespace vrv;
    using namespace std;
%}

