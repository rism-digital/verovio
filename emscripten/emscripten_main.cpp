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
char *_persistent_log_string = NULL;


extern "C" {
    const char * convertMusic(const char * c_notation, const char * c_options) {
        
        string notation(c_notation);
        string json_options(c_options);
        string out_str;
        InterfaceController controller;
        
        if (!initialized) {
            std::srand(std::time(0));
            initialized = true;
        }
        
        if (!controller.ParseOptions( json_options )) {
            vrv::LogError( "Could not load JSON options." );
            return NULL;
        }
        
        Resources::SetPath("/data");
        
        // default to mei if unset.
        controller.LoadString( notation );

        // in the future we will be able to render to mei too
        out_str = controller.RenderToSvg();
        return out_str.c_str();
        // return self allocated pointer?
        // see https://github.com/DDMAL/aruspix/commit/cb2b957ebcc8edf9787bc6f6271217115a6959e3
    }
	
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
		free(_persistent_log_string);
	}
	
	const char *vrvInterfaceController_getLog(InterfaceController *ic) {
		std::vector<std::string> v = vrv::GetLogBuffer();
		std::string str;
		
		for( size_t i = 0; i != v.size(); ++i)
		        str = str + v[i];
		
		if (_persistent_log_string == NULL) {
			_persistent_log_string = (char*)malloc(strlen(str.c_str() + 1));
		} else {
			_persistent_log_string = (char*)realloc(_persistent_log_string, strlen(str.c_str() + 1));
		}
		
		strcpy(_persistent_log_string, str.c_str());
		
		return _persistent_log_string;
	}
	
	
	int vrvInterfaceController_getPageCount(InterfaceController *ic) {
		return ic->GetPageCount();
	}

	bool vrv_InterfaceController_loadData(InterfaceController *ic, const char *data) {
		ResetLogBuffer();
        return ic->LoadString( data );
	}


	const char *vrvInterfaceController_renderPage(InterfaceController *ic, int page_no, const char *c_options) {
		ResetLogBuffer();
		ic->SetCString(ic->RenderToSvg(page_no, false));
		return ic->GetCString();
	}
	
	void vrvInterfaceController_setOptions(InterfaceController *ic, const char *options) {		
        if (!ic->ParseOptions( options )) {
            vrv::LogError( "Could not load JSON options." );
        }
	}

	const char* vrvInterfaceController_renderData(InterfaceController *ic, const char *data, const char *options) {
		ResetLogBuffer();
		vrvInterfaceController_setOptions(ic, options);
		vrv_InterfaceController_loadData(ic, data);
		
		return vrvInterfaceController_renderPage(ic, 1, options);
	}


}
