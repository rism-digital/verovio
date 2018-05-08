/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <sys/stat.h>

#ifndef _WIN32
#include <getopt.h>
#else
#include "win_getopt.h"
#endif

#ifndef USE_EMSCRIPTEN
#define USE_EMSCRIPTEN
#endif

#ifndef EMSCRIPTEN
#define EMSCRIPTEN
#endif

//----------------------------------------------------------------------------

#include "options.h"
#include "toolkit.h"
#include "vrv.h"

using namespace std;
using namespace vrv;

// Some redundant code to get basenames
// and remove extensions
// possible that it is not in std??
struct MatchPathSeparator {
    bool operator()(char ch) const { return ch == '\\' || ch == '/'; }
};

std::string basename(std::string const &pathname)
{
    return std::string(std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator()).base(), pathname.end());
}

std::string removeExtension(std::string const &filename)
{
    std::string::const_reverse_iterator pivot = std::find(filename.rbegin(), filename.rend(), '.');
    return pivot == filename.rend() ? filename : std::string(filename.begin(), pivot.base() - 1);
}

std::string fromCamelCase(const std::string &s)
{
    std::regex regExp1("(.)([A-Z][a-z]+)");
    std::regex regExp2("([a-z0-9])([A-Z])");

    std::string result = s;
    result = std::regex_replace(result, regExp1, "$1-$2");
    result = std::regex_replace(result, regExp2, "$1-$2");

    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string toCamelCase(const std::string &s)
{
    std::istringstream iss(s);
    std::string token;
    std::string result;

    while (getline(iss, token, '-')) {
        token[0] = toupper(token[0]);
        result += token;
    }

    result[0] = tolower(result[0]);

    return result;
}

bool dir_exists(string dir)
{
    struct stat st;
    if ((stat(dir.c_str(), &st) == 0) && (((st.st_mode) & S_IFMT) == S_IFDIR)) {
        return true;
    }
    else {
        return false;
    }
}

void display_version()
{
    cout << "Verovio " << vrv::GetVersion() << endl;
}

void display_usage()
{
    cout.precision(2);

    display_version();
    cout << endl << "Example usage:" << endl << endl;
    cout << " verovio [-s scale] [-t type] [-r resources] [-o outfile] infile" << endl << endl;

    // These need to be kept in alphabetical order:
    // -options with both short and long forms first
    // -then options with long forms only
    // -then debugging options

    Options style;

    // Options with both short and long forms
    cout << "Options (marked as * are repeatable)" << endl;

    cout << " -                     Use \"-\" as input file for reading from the standard input" << endl;
    cout << " -?, --help            Display this message" << endl;
    cout << " -a, --all-pages       Output all pages" << endl;
    cout << " -f, --format <s>      Select input format: darms, mei, pae, xml (default is mei)" << endl;
    cout << " -o, --outfile <s>     Output file name (use \"-\" for standard output)" << endl;
    cout << " -p, --page <i>        Select the page to engrave (default is 1)" << endl;
    cout << " -r, --resources <s>   Path to SVG resources (default is " << vrv::Resources::GetPath() << ")" << endl;
    cout << " -s, --scale <i>       Scale percent (default is " << DEFAULT_SCALE << ")" << endl;
    cout << " -t, --type <s>        Select output format: mei, svg, or midi (default is svg)" << endl;
    cout << " -v, --version         Display the version number" << endl;
    cout << " -x, --xml-id-seed <i> Seed the random number generator for XML IDs" << endl;

    Options options;
    std::vector<OptionGrp *> *grp = options.GetGrps();
    std::vector<OptionGrp *>::iterator grpIter;

    for (grpIter = grp->begin(); grpIter != grp->end(); grpIter++) {

        // Options with long forms only
        cout << endl << (*grpIter)->GetLabel() << endl;
        const std::vector<Option *> *options = (*grpIter)->GetOptions();
        std::vector<Option *>::const_iterator iter;

        for (iter = options->begin(); iter != options->end(); iter++) {

            std::string option = fromCamelCase((*iter)->GetKey());

            const OptionDbl *optDbl = dynamic_cast<const OptionDbl *>(*iter);
            const OptionInt *optInt = dynamic_cast<const OptionInt *>(*iter);
            const OptionIntMap *optIntMap = dynamic_cast<const OptionIntMap *>(*iter);
            const OptionString *optString = dynamic_cast<const OptionString *>(*iter);
            const OptionArray *optArray = dynamic_cast<const OptionArray *>(*iter);
            const OptionBool *optBool = dynamic_cast<const OptionBool *>(*iter);

            if (optDbl) {
                option.append(" <f>");
            }
            else if (optInt) {
                option.append(" <i>");
            }
            else if (optString) {
                option.append(" <s>");
            }
            else if (optArray) {
                option.append("* <s>");
            }
            else if (!optBool) {
                option.append(" <s>");
            }

            if (option.size() < 32) {
                option.insert(option.end(), 32 - option.size(), ' ');
            }
            else {
                option.append("\t");
            }

            cout << " --" << option << (*iter)->GetDescription();

            if (optInt) {
                cout << " (default: " << optInt->GetDefault();
                cout << "; min: " << optInt->GetMin();
                cout << "; max: " << optInt->GetMax() << ")";
            }
            if (optDbl) {
                cout << fixed << " (default: " << optDbl->GetDefault();
                cout << fixed << "; min: " << optDbl->GetMin();
                cout << fixed << "; max: " << optDbl->GetMax() << ")";
            }
            if (optString) {
                cout << " (default: \"" << optString->GetDefault() << "\")";
            }
            if (optIntMap) {
                cout << " (default: \"" << optIntMap->GetDefaultStrValue()
                     << "\"; other values: " << optIntMap->GetStrValuesAsStr(true) << ")";
            }
            cout << endl;
        }
    }
}

int runExtraTests(Toolkit *toolkit)
{
    return 0;
}

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

int main2(int argc, char **argv)
{
    string infile;
    string svgdir;
    string outfile;
    string outformat = "svg";
    bool std_output = false;

    int all_pages = 0;
    int page = 1;
    int show_help = 0;
    int show_version = 0;

    int drag_test = 0;

    // Create the toolkit instance without loading the font because
    // the resource path might be specified in the parameters
    // The fonts will be loaded later with Resources::InitFonts()
    Toolkit toolkit(false);

    if (argc < 2) {
        cerr << "Expected one input file but found none." << endl << endl;
        display_usage();
        exit(1);
    }

    static struct option base_options[] = {
        { "all-pages", no_argument, 0, 'a' },
        { "format", required_argument, 0, 'f' },
        { "help", no_argument, 0, '?' },
        { "outfile", required_argument, 0, 'o' },
        { "page", required_argument, 0, 'p' },
        { "resources", required_argument, 0, 'r' },
        { "scale", required_argument, 0, 's' },
        { "type", required_argument, 0, 't' },
        { "version", no_argument, 0, 'v' },
        { "xml-id-seed", required_argument, 0, 'x' },
        // deprecated - some use undocumented short options to catch them as such
        { "border", required_argument, 0, 'b' },
        { "ignore-layout", no_argument, 0, 'i' },
        { "no-layout", no_argument, 0, 'n' },
        { "page-height-deprecated", required_argument, 0, 'h' },
        { "page-width-deprecated", required_argument, 0, 'w' },
        { 0, 0, 0, 0 }
    };
    
    int baseSize = sizeof(base_options) / sizeof(option);

    Options *options = toolkit.GetOptions();
    const MapOfStrOptions *params = options->GetItems();
    int mapSize = (int)params->size();

    struct option *long_options;
    int i = 0;
    long_options = (struct option *)malloc(sizeof(struct option) * (baseSize + mapSize));

    // A vector of string for storing names as const char* for long_options
    std::vector<std::string> optNames;
    optNames.reserve(mapSize);

    MapOfStrOptions::const_iterator iter;
    for (iter = params->begin(); iter != params->end(); iter++) {
        // Double check that back and forth convertion is correct
        assert(toCamelCase(fromCamelCase(iter->first)) == iter->first);

        optNames.push_back(fromCamelCase(iter->first));
        long_options[i].name = optNames.at(i).c_str();
        OptionBool *optBool = dynamic_cast<OptionBool *>(iter->second);
        long_options[i].has_arg = (optBool) ? no_argument : required_argument;
        long_options[i].flag = 0;
        long_options[i].val = 0;
        i++;
    }

    // Concatenate the base options
    assert(i == mapSize);
    for (; i < mapSize + baseSize; i++) {
        long_options[i].name = base_options[i - mapSize].name;
        long_options[i].has_arg = base_options[i - mapSize].has_arg;
        long_options[i].flag = base_options[i - mapSize].flag;
        long_options[i].val = base_options[i - mapSize].val;
    }

    int c;
    std::string key;
    int option_index = 0;
    Option *opt = NULL;
    OptionBool *optBool = NULL;
    while ((c = getopt_long(argc, argv, "?ab:f:h:ino:p:r:s:t:w:vx:", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                key = long_options[option_index].name;
                opt = params->at(toCamelCase(key));
                optBool = dynamic_cast<OptionBool *>(opt);
                if (optBool) {
                    optBool->SetValue(true);
                }
                else if (opt) {
                    if (!opt->SetValue(optarg)) {
                        LogWarning("Setting option %s with %s failed, default value used",
                            long_options[option_index].name, optarg);
                    }
                }
                else {
                    LogError("Something went wrong with option %s", long_options[option_index].name);
                    exit(1);
                }
                break;

            case 'a': all_pages = 1; break;

            case 'b':
                LogWarning("Option -b and --border is deprecated; use --page-margin-bottom, --page-margin-left, --page-margin-right and "
                           "--page-margin-top instead");
                options->m_pageMarginBottom.SetValue(optarg);
                options->m_pageMarginLeft.SetValue(optarg);
                options->m_pageMarginRight.SetValue(optarg);
                options->m_pageMarginTop.SetValue(optarg);
                break;
            case 'd':
                drag_test = 1;
            case 'f':
                if (!toolkit.SetFormat(string(optarg))) {
                    exit(1);
                };
                break;

            case 'h':
                LogWarning("Option -h is deprecated; use --page-height instead");
                options->m_pageHeight.SetValue(optarg);
                break;

            case 'i':
                LogWarning("Option --ignore-layout is deprecated; use --breaks auto");
                options->m_breaks.SetValue(BREAKS_auto);
                break;

            case 'n':
                LogWarning("Option --no-layout is deprecated; use --breaks none");
                options->m_breaks.SetValue(BREAKS_none);
                break;

            case 'o': outfile = string(optarg); break;

            case 'p': page = atoi(optarg); break;

            case 'r': vrv::Resources::SetPath(optarg); break;

            case 't':
                outformat = string(optarg);
                toolkit.SetOutputFormat(string(optarg));
                break;

            case 's':
                if (!toolkit.SetScale(atoi(optarg))) {
                    exit(1);
                }
                break;

            case 'v': show_version = 1; break;

            case 'w':
                LogWarning("Option -w is deprecated; use --page-width instead");
                options->m_pageWidth.SetValue(optarg);
                break;

            case 'x': Object::SeedUuid(atoi(optarg)); break;

            case '?':
                display_usage();
                exit(0);
                break;

            default: break;
        }
    }

    if (show_version) {
        display_version();
        exit(0);
    }

    if (show_help) {
        display_usage();
        exit(0);
    }

    if (optind <= argc - 1) {
        infile = string(argv[optind]);
    }
    else {
        cerr << "Incorrect number of arguments: expected one input file but found none." << endl << endl;
        display_usage();
        exit(1);
    }

    // Make sure the user uses a valid Resource path
    // Save many headaches for empty SVGs
    if (!dir_exists(vrv::Resources::GetPath())) {
        cerr << "The resources path " << vrv::Resources::GetPath() << " could not be found; please use -r option."
             << endl;
        exit(1);
    }

    // Load the music font from the resource directory
    if (!Resources::InitFonts()) {
        cerr << "The music font could not be loaded; please check the contents of the resource directory." << endl;
        exit(1);
    }

    // Load a specified font
    if (!Resources::SetFont(options->m_font.GetValue())) {
        cerr << "Font '" << options->m_font.GetValue() << "' could not be loaded." << endl;
        exit(1);
    }

    if ((outformat != "svg") && (outformat != "mei") && (outformat != "midi") && (outformat != "timemap")
        && (outformat != "humdrum") && (outformat != "hum")) {
        cerr << "Output format (" << outformat << ") can only be 'mei', 'svg', 'midi', or 'humdrum'." << endl;
        exit(1);
    }

    // Make sure we provide a file name or output to std output with std input
    if ((infile == "-") && (outfile.empty())) {
        cerr << "Standard input can be used only with standard output or output filename." << endl;
        exit(1);
    }

    // Hardcode svg ext for now
    if (outfile.empty()) {
        outfile = removeExtension(infile);
    }
    else if (outfile == "-") {
        // DisableLog();
        std_output = true;
    }
    else {
        outfile = removeExtension(outfile);
    }

    // Load the std input or load the file
    if (infile == "-") {
        ostringstream data_stream;
        for (string line; getline(cin, line);) {
            data_stream << line << endl;
        }
        if (!toolkit.LoadData(data_stream.str())) {
            cerr << "The input could not be loaded." << endl;
            exit(1);
        }
    }
    else {
        if (!toolkit.LoadFile(infile)) {
            cerr << "The file '" << infile << "' could not be opened." << endl;
            exit(1);
        }
    }

    if (toolkit.GetOutputFormat() != HUMDRUM) {
        // Check the page range
        if (page > toolkit.GetPageCount()) {
            cerr << "The page requested (" << page << ") is not in the page range (max is " << toolkit.GetPageCount()
                 << ")." << endl;
            exit(1);
        }
        if (page < 1) {
            cerr << "The page number has to be greater than 0." << endl;
            exit(1);
        }
    }

    int from = page;
    int to = page + 1;
    if (all_pages) {
        to = toolkit.GetPageCount() + 1;
    }

    // Run extra test commands
    if (drag_test) {
        if (!runExtraTests(&toolkit)) {
            cerr << "An error occured in the extra tests!" << endl;
            exit(1);
        }
    }

    if (outformat == "svg") {
        int p;
        for (p = from; p < to; p++) {
            std::string cur_outfile = outfile;
            if (all_pages) {
                cur_outfile += StringFormat("_%03d", p);
            }
            cur_outfile += ".svg";
            if (std_output) {
                cout << toolkit.RenderToSVG(p);
            }
            else if (!toolkit.RenderToSVGFile(cur_outfile, p)) {
                cerr << "Unable to write SVG to " << cur_outfile << "." << endl;
                exit(1);
            }
            else {
                cerr << "Output written to " << cur_outfile << "." << endl;
            }
        }
    }

    else if (outformat == "midi") {
        outfile += ".mid";
        if (std_output) {
            cerr << "Midi cannot write to standard output." << endl;
            exit(1);
        }
        else if (!toolkit.RenderToMIDIFile(outfile)) {
            cerr << "Unable to write MIDI to " << outfile << "." << endl;
            exit(1);
        }
        else {
            cerr << "Output written to " << outfile << "." << endl;
        }
    }
    else if (outformat == "timemap") {
        outfile += ".json";
        if (std_output) {
            std::string output;
            std::cout << toolkit.RenderToTimemap();
        }
        else if (!toolkit.RenderToTimemapFile(outfile)) {
            cerr << "Unable to write MIDI to " << outfile << "." << endl;
            exit(1);
        }
        else {
            cerr << "Output written to " << outfile << "." << endl;
        }
    }
    else if (outformat == "humdrum" || outformat == "hum") {
        outfile += ".krn";
        if (std_output) {
            toolkit.GetHumdrum(std::cout);
        }
        else {
            if (!toolkit.GetHumdrumFile(outfile)) {
                cerr << "Unable to write Humdrum to " << outfile << "." << endl;
                exit(1);
            }
            else {
                cerr << "Output written to " << outfile << "." << endl;
            }
        }
    }
    else {
        if (all_pages) {
            toolkit.SetScoreBasedMei(true);
            outfile += ".mei";
            if (std_output) {
                cerr << "MEI output of all pages to standard output is not possible." << endl;
                exit(1);
            }
            else if (!toolkit.SaveFile(outfile)) {
                cerr << "Unable to write MEI to " << outfile << "." << endl;
                exit(1);
            }
            else {
                cerr << "Output written to " << outfile << "." << endl;
            }
        }
        else {
            if (std_output) {
                cout << toolkit.GetMEI(page);
            }
            else {
                cerr << "MEI output of one page is available only to standard output." << endl;
                exit(1);
            }
        }
    }

    return 0;
}
