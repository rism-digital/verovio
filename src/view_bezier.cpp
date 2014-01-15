/////////////////////////////////////////////////////////////////////////////
// Name:        view_bezier.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

namespace vrv {

/** static members **/
MusPoint View::point_[];
MusPoint View::bcoord[];
 
void View::pntswap (MusPoint *x1, MusPoint *x2)
{	MusPoint a;
	a = *x1;
	*x1 = *x2;
	*x2 = a;
}

} // namespace vrv