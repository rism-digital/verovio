/////////////////////////////////////////////////////////////////////////////
// Name:        emscripten_main.cpp
// Author:      Rodolfo Zitellini
// Created:     05/11/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "vrv.h"
#include "toolkit.h"

using namespace std;
using namespace vrv;

bool initialized = false;

extern "C" {

	/****************************************************************
	* Methods exported to use the Toolkit class from js
	****************************************************************/
	void *vrvToolkit_constructor() {
		
		// Init the random number generator
		// for mei ids
        if (!initialized) {
            std::srand(std::time(0));
            initialized = true;
        }
		
		// set the resource path in the js blob
		Resources::SetPath("/data");
		
		return new Toolkit();
	}
	
	void vrvToolkit_destructor(Toolkit *tk) {
		delete tk;
	}
	
	const char *vrvToolkit_getLog(Toolkit *tk) {
        tk->SetCString(tk->GetLogString());
        return tk->GetCString();
    }

    const char *vrvToolkit_getVersion(Toolkit *tk) {
        tk->SetCString(tk->GetVersion());
        return tk->GetCString();
    }
    
	int vrvToolkit_getPageCount(Toolkit *tk) {
		return tk->GetPageCount();
	}
    
    int vrvToolkit_getPageWithElement(Toolkit *tk, const char *xmlId) {
        return tk->GetPageWithElement( xmlId );
    }

	bool vrvToolkit_loadData(Toolkit *tk, const char *data) {
		tk->ResetLogBuffer();
        return tk->LoadString( data );
	}

    const char *vrvToolkit_getMEI(Toolkit *tk, int page_no, bool score_based) {
        tk->SetCString(tk->GetMEI( page_no, score_based ));
        return tk->GetCString();
    }
    
	const char *vrvToolkit_renderPage(Toolkit *tk, int page_no, const char *c_options) {
		tk->ResetLogBuffer();
		tk->SetCString(tk->RenderToSvg(page_no, false));
		return tk->GetCString();
	}
	
	void vrvToolkit_setOptions(Toolkit *tk, const char *options) {		
        if (!tk->ParseOptions( options )) {
            vrv::LogError( "Could not load JSON options." );
        }
	}
    
    void vrvToolkit_redoLayout(Toolkit *tk) {
        tk->RedoLayout();
	}

	const char* vrvToolkit_renderData(Toolkit *tk, const char *data, const char *options) {
		tk->ResetLogBuffer();
		vrvToolkit_setOptions(tk, options);
		vrvToolkit_loadData(tk, data);
		
		return vrvToolkit_renderPage(tk, 1, options);
	}
    
    bool vrvToolkit_edit(Toolkit *tk, const char *editorAction) {
        if (!tk->Edit( editorAction )) {
            vrv::LogError( "Could not perform editor action." );
            return false;
        }
        return true;
    }
    
    const char* vrvToolkit_getElementAttr(Toolkit *tk, const char *xmlId) {
        tk->SetCString(tk->GetElementAttr( xmlId ));
        return tk->GetCString();
    }
}
