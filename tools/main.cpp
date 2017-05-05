/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>

#ifndef _WIN32
#include <getopt.h>
#else
#include "win_getopt.h"
#endif

//----------------------------------------------------------------------------

#include "style.h"
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
    cerr << "Verovio " << vrv::GetVersion() << endl;
}

void display_usage()
{

    display_version();
    cerr << endl << "Example usage:" << endl << endl;
    cerr << " verovio [-s scale] [-t type] [-r resources] [-o outfile] infile" << endl << endl;

    // These need to be kept in alphabetical order:
    // -options with both short and long forms first
    // -then options with long forms only
    // -then debugging options

    // Options with both short and long forms
    cerr << "Options (marked as * are repeatable)" << endl;

    cerr << " -                          Use \"-\" as input file for reading from the standard input" << endl;

    cerr << " -b, --border=BORDER        Add border (default is " << DEFAULT_PAGE_LEFT_MAR << ")" << endl;

    cerr << " -f, --format=INPUT_FORMAT  Select input format: darms, mei, pae, xml (default is mei)" << endl;

    cerr << " -h, --page-height=HEIGHT   Specify the page height (default is " << DEFAULT_PAGE_HEIGHT << ")" << endl;

    cerr << " -o, --outfile=FILE_NAME    Output file name (use \"-\" for standard output)" << endl;

    cerr << " -r, --resources=PATH       Path to SVG resources (default is " << vrv::Resources::GetPath() << ")"
         << endl;

    cerr << " -s, --scale=FACTOR         Scale percent (default is " << DEFAULT_SCALE << ")" << endl;

    cerr << " -t, --type=OUTPUT_TYPE     Select output format: mei, svg, or midi (default is svg)" << endl;

    cerr << " -v, --version              Display the version number" << endl;

    cerr << " -w, --page-width=WIDTH     Specify the page width (default is " << DEFAULT_PAGE_WIDTH << ")" << endl;

    // Options with long forms only
    cerr << endl << "Additional options" << endl;

    cerr << " --adjust-page-height       Crop the page height to the height of the content" << endl;

    cerr << " --all-pages                Output all pages with one output file per page" << endl;

    cerr << " --app-xpath-query=QR*      Set the xPath query for selecting <app> child elements," << endl;
    cerr << "                            for example: \"./rdg[contains(@source, 'source-id')]\";" << endl;
    cerr << "                            by default the <lem> or the first <rdg> is selected" << endl;

    cerr << " --choice-xpath-query=QR*   Set the xPath query for selecting <choice> child elements," << endl;
    cerr << "                            for example: \"./orig\"; by default the first child is selected" << endl;

    cerr << " --even-note-spacing        Space notes evenly and close together regardless of their durations" << endl;

    cerr << " --font=FONT                Select the music font to use (default is Leipzig;" << endl;
    cerr << "                            Bravura and Gootville are also available)" << endl;

    cerr << " --help                     Display this message" << endl;

    cerr << " --ignore-layout            Ignore all encoded layout information (if any)" << endl;
    cerr << "                            and fully recalculate the layout" << endl;

    cerr << " --mdiv-xpath-query=QR      Set the xPath query for selecting the <mdiv> to be rendered;" << endl;
    cerr << "                            only one <mdiv> can be rendered" << endl;

    cerr << " --no-layout                Ignore all encoded layout information (if any)" << endl;
    cerr << "                            and output one single page with one single system" << endl;

    cerr << " --page=PAGE                Select the page to engrave (default is 1)" << endl;

    cerr << " --spacing-linear=SP        Specify the linear spacing factor (default is " << DEFAULT_SPACING_LINEAR
         << ")" << endl;

    cerr << " --spacing-non-linear=SP    Specify the non-linear spacing factor (default is "
         << DEFAULT_SPACING_NON_LINEAR << ")" << endl;

    cerr << " --spacing-staff=SP         Specify the spacing above each staff (in MEI vu)" << endl;

    cerr << " --spacing-system=SP        Specify the spacing above each system (in MEI vu)" << endl;

    cerr << " --xml-id-seed=INT          Seed the random number generator for XML IDs" << endl;

    // Debugging options
    cerr << endl << "Debugging options" << endl;

    cerr << " --no-justification         Do not justify the system" << endl;

    cerr << " --show-bounding-boxes      Show symbol bounding boxes" << endl;
}

int main(int argc, char **argv)
{

    string infile;
    string svgdir;
    string outfile;
    string outformat = "svg";
    string font = "";
    vector<string> appXPathQueries;
    vector<string> choiceXPathQueries;
    bool std_output = false;

    int no_mei_hdr = 0;
    int adjust_page_height = 0;
    int all_pages = 0;
    int no_layout = 0;
    int ignore_layout = 0;
    int no_justification = 0;
    int even_note_spacing = 0;
    int show_bounding_boxes = 0;
    int page = 1;
    int show_help = 0;
    int show_version = 0;

    // Create the toolkit instance without loading the font because
    // the resource path might be specified in the parameters
    // The fonts will be loaded later with Resources::InitFonts()
    Toolkit toolkit(false);

    if (argc < 2) {
        cerr << "Expected one input file but found none." << endl << endl;
        display_usage();
        exit(1);
    }
    int c;

    static struct option long_options[] = { { "adjust-page-height", no_argument, &adjust_page_height, 1 },
        { "all-pages", no_argument, &all_pages, 1 }, { "app-xpath-query", required_argument, 0, 0 },
        { "border", required_argument, 0, 'b' }, { "choice-xpath-query", required_argument, 0, 0 },
        { "even-note-spacing", no_argument, &even_note_spacing, 1 }, { "font", required_argument, 0, 0 },
        { "format", required_argument, 0, 'f' }, { "help", no_argument, &show_help, 1 },
        { "ignore-layout", no_argument, &ignore_layout, 1 }, { "mdiv-xpath-query", required_argument, 0, 0 },
        { "no-layout", no_argument, &no_layout, 1 }, { "no-mei-hdr", no_argument, &no_mei_hdr, 1 },
        { "no-justification", no_argument, &no_justification, 1 }, { "outfile", required_argument, 0, 'o' },
        { "page", required_argument, 0, 0 }, { "page-height", required_argument, 0, 'h' },
        { "page-width", required_argument, 0, 'w' }, { "resources", required_argument, 0, 'r' },
        { "scale", required_argument, 0, 's' }, { "show-bounding-boxes", no_argument, &show_bounding_boxes, 1 },
        { "spacing-linear", required_argument, 0, 0 }, { "spacing-non-linear", required_argument, 0, 0 },
        { "spacing-staff", required_argument, 0, 0 }, { "spacing-system", required_argument, 0, 0 },
        { "type", required_argument, 0, 't' }, { "version", no_argument, &show_version, 1 },
        { "xml-id-seed", required_argument, 0, 0 }, { 0, 0, 0, 0 } };

    int option_index = 0;
    while ((c = getopt_long(argc, argv, "b:f:h:o:p:r:s:t:w:v", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;
                else if (strcmp(long_options[option_index].name, "app-xpath-query") == 0) {
                    cout << string(optarg) << endl;
                    appXPathQueries.push_back(string(optarg));
                }
                else if (strcmp(long_options[option_index].name, "choice-xpath-query") == 0) {
                    cout << string(optarg) << endl;
                    choiceXPathQueries.push_back(string(optarg));
                }
                if (strcmp(long_options[option_index].name, "font") == 0) {
                    font = string(optarg);
                }
                else if (strcmp(long_options[option_index].name, "mdiv-xpath-query") == 0) {
                    cout << string(optarg) << endl;
                    toolkit.SetMdivXPathQuery(string(optarg));
                }
                else if (strcmp(long_options[option_index].name, "page") == 0) {
                    page = atoi(optarg);
                }
                else if (strcmp(long_options[option_index].name, "spacing-linear") == 0) {
                    if (!toolkit.SetSpacingLinear(atof(optarg))) {
                        exit(1);
                    }
                }
                else if (strcmp(long_options[option_index].name, "spacing-non-linear") == 0) {
                    if (!toolkit.SetSpacingNonLinear(atof(optarg))) {
                        exit(1);
                    }
                }
                else if (strcmp(long_options[option_index].name, "spacing-staff") == 0) {
                    if (!toolkit.SetSpacingStaff(atoi(optarg))) {
                        exit(1);
                    }
                }
                else if (strcmp(long_options[option_index].name, "spacing-system") == 0) {
                    if (!toolkit.SetSpacingSystem(atoi(optarg))) {
                        exit(1);
                    }
                }
                else if (strcmp(long_options[option_index].name, "xml-id-seed") == 0) {
                    Object::SeedUuid(atoi(optarg));
                }
                break;

            case 'b':
                if (!toolkit.SetBorder(atoi(optarg))) {
                    exit(1);
                }
                break;

            case 'f':
                if (!toolkit.SetFormat(string(optarg))) {
                    exit(1);
                };
                break;

            case 'h':
                if (!toolkit.SetPageHeight(atoi(optarg))) {
                    exit(1);
                };
                break;

            case 'o': outfile = string(optarg); break;

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
                if (!toolkit.SetPageWidth(atoi(optarg))) {
                    exit(1);
                }
                break;

            case '?':
                display_usage();
                exit(0);
                break;

            default: break;
        }
    }

    if (appXPathQueries.size() > 0) {
        toolkit.SetAppXPathQueries(appXPathQueries);
    }
    if (choiceXPathQueries.size() > 0) {
        toolkit.SetChoiceXPathQueries(choiceXPathQueries);
    }

    if (show_version) {
        display_version();
        exit(0);
    }

    if (show_help) {
        display_usage();
        exit(0);
    }

    // Set the various flags in accordance with the options given
    toolkit.SetAdjustPageHeight(adjust_page_height);
    toolkit.SetNoLayout(no_layout);
    toolkit.SetIgnoreLayout(ignore_layout);
    toolkit.SetNoJustification(no_justification);
    toolkit.SetEvenNoteSpacing(even_note_spacing);
    toolkit.SetShowBoundingBoxes(show_bounding_boxes);

    if (optind <= argc - 1) {
        infile = string(argv[optind]);
    }
    else {
        cerr << "Incorrect number of arguments: expected one input file but found none." << endl << endl;
        display_usage();
        exit(1);
    }
    
    // If we output svg or do not request no layout to be performed then we need the font
    if ((outformat == "svg") || !toolkit.GetNoLayout()) {
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
        if (!font.empty() && !toolkit.SetFont(font)) {
            cerr << "Font '" << font << "' could not be loaded." << endl;
            exit(1);
        }
    }

    if (outformat != "svg" && outformat != "mei" && outformat != "midi" && outformat != "humdrum") {
        cerr << "Output format can only be 'mei', 'svg', 'midi', or 'humdrum'." << endl;
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

    if (outformat == "svg") {
        int p;
        for (p = from; p < to; p++) {
            std::string cur_outfile = outfile;
            if (all_pages) {
                cur_outfile += StringFormat("_%03d", p);
            }
            cur_outfile += ".svg";
            if (std_output) {
                cout << toolkit.RenderToSvg(p);
            }
            else if (!toolkit.RenderToSvgFile(cur_outfile, p)) {
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
        else if (!toolkit.RenderToMidiFile(outfile)) {
            cerr << "Unable to write MIDI to " << outfile << "." << endl;
            exit(1);
        }
        else {
            cerr << "Output written to " << outfile << "." << endl;
        }
    }
    else if (outformat == "humdrum") {
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
