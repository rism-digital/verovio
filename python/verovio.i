/////////////////////////////////////////////////////////////////////////////
// Name:        verovio.i
// Author:      Laurent Pugin
// Created:     09/11/2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


%module verovio
%include "std_string.i"
%include "../include/vrv/toolkit.h"

%{
    #include "../include/vrv/toolkit.h"
    
    using namespace vrv;
    using namespace std;
%}

