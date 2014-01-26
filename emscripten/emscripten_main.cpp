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

#include <sstream>
#include <string>

#include "vrv.h"
#include "interfacecontroller.h"

using namespace std;
using namespace vrv;

bool initialized = false;

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
}
