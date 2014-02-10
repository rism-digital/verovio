//
//  emscripten_main.cpp
//  aruspix
//
//  Created by Rodolfo Zitellini on 05/11/13.
//  Copyright (c) 2013 com.aruspix.www. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <assert.h>

#include <sstream>
#include <string>
#include <vector>

#include "vrv.h"
#include "interfacecontroller.h"

using namespace std;
using namespace vrv;

bool initialized = false;

extern "C" {

	/****************************************************************
	* Methods exported to use the InterfaceController class from js
	****************************************************************/
	void *vrvInterfaceController_constructor() {
		
		// Init the random number generator
		// for mei ids
        if (!initialized) {
            std::srand(std::time(0));
            initialized = true;
        }
		
		// set the resource path in the js blob
		Resources::SetPath("/data");
		
		return new InterfaceController();
	}
	
	void vrvInterfaceController_destructor(InterfaceController *ic) {
		delete ic;
	}
	
	const char *vrvInterfaceController_getLog(InterfaceController *ic) {
        ic->SetCString(ic->GetLogString());
        return ic->GetCString();
    }
	
	int vrvInterfaceController_getPageCount(InterfaceController *ic) {
		return ic->GetPageCount();
	}

	bool vrv_InterfaceController_loadData(InterfaceController *ic, const char *data) {
		ic->ResetLogBuffer();
        return ic->LoadString( data );
	}


	const char *vrvInterfaceController_renderPage(InterfaceController *ic, int page_no, const char *c_options) {
		ic->ResetLogBuffer();
		ic->SetCString(ic->RenderToSvg(page_no, false));
		return ic->GetCString();
	}
	
	void vrvInterfaceController_setOptions(InterfaceController *ic, const char *options) {		
        if (!ic->ParseOptions( options )) {
            vrv::LogError( "Could not load JSON options." );
        }
	}

	const char* vrvInterfaceController_renderData(InterfaceController *ic, const char *data, const char *options) {
		ic->ResetLogBuffer();
		vrvInterfaceController_setOptions(ic, options);
		vrv_InterfaceController_loadData(ic, data);
		
		return vrvInterfaceController_renderPage(ic, 1, options);
	}


}
