#include <iostream>
#include <string>

#ifndef USE_EMSCRIPTEN
#define USE_EMSCRIPTEN
#endif

#ifndef EMSCRIPTEN
#define EMSCRIPTEN
#endif

//----------------------------------------------------------------------------

#include "toolkit.h"
#include "vrv.h"

using namespace std;
using namespace vrv;

int main(int argc, char **argv)
{
    string filename = "mei4page.mei";
    string inputFormat = "mei";
    string outputFormat = "svg";

    string vrvOptions = "{\"noLayout\":1,\"noFooter\":1,\"noHeader\":1}"; // Taken from Neon2
    string editorAction = "{\"action\":\"drag\",\"param\":{\"elementId\":\"nc-0000001233928944\",\"x\":648,\"y\":1260}}";

    Toolkit toolkit;
    toolkit.SetFormat(inputFormat);
    toolkit.SetOutputFormat(outputFormat);
    toolkit.SetOptions(vrvOptions);
    toolkit.LoadFile(filename);
    toolkit.RenderToSVGFile("output.svg", 1);
    //toolkit.LoadData(toolkit.GetMEI());
    toolkit.Edit(editorAction);

    toolkit.RenderToSVGFile("output.svg", 1);
    return 0; 
}
