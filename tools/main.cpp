/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <ctime>

#include <sstream>
#include <string>

#include <assert.h>

#include <getopt.h>

// Used to check if a dir exists
#include <sys/stat.h>

#include "vrv.h"
#include "interfacecontroller.h"

using namespace std;
using namespace vrv;

// Some redundant code to get basenames
// and remove extensions
// possible that it is not in std??
struct MatchPathSeparator
{
    bool operator()( char ch ) const
    {
        return ch == '\\' || ch == '/';
    }
};

std::string basename( std::string const& pathname )
{
    return std::string( 
                       std::find_if( pathname.rbegin(), pathname.rend(),
                                    MatchPathSeparator() ).base(),
                       pathname.end() );
}

std::string removeExtension( std::string const& filename )
{
    std::string::const_reverse_iterator
    pivot
    = std::find( filename.rbegin(), filename.rend(), '.' );
    return pivot == filename.rend()
    ? filename
    : std::string( filename.begin(), pivot.base() - 1 );
}

bool dir_exists (string dir) {
    struct stat st;
    if ((stat(dir.c_str(), &st) == 0) && (((st.st_mode) & S_IFMT) == S_IFDIR)) {
        return true;
    } else {
        return false;
    }
}

void display_usage() {
    
    cerr << "Verovio " << GetVersion() << endl << "Usage:" << endl << endl;
    cerr << " verovio [-f format] [-s scale] [-t type] [-r resources] [-o outfile] infile" << endl << endl;
    
    // These need to be kept in alphabetical order:
    // -short options first
    // -then long options only
    // -then debugging options

    // Options
    cerr << "Options" << endl;
    
    cerr << " -b, --border=BORDER        Add border (default is " << DEFAULT_PAGE_LEFT_MAR << ")" << endl;
    
    cerr << " -f, --format=INPUT_FORMAT  Select input format: darms, mei, pae (default is pae)" << endl;
    
    cerr << " -h, --page-height=HEIGHT   Specify the page height (default is " << DEFAULT_PAGE_HEIGHT << ")" << endl;
    
    cerr << " -o, --outfile=FILE_NAME    Output file name" << endl;
    
    cerr << " -r, --recources=PATH       Path to SVG resources (default is " <<  vrv::Resources::GetPath() << ")" << endl;
    
    cerr << " -s, --scale=FACTOR         Scale percent (default is " << DEFAULT_SCALE << ")" << endl;
    
    cerr << " -t, --type=OUTPUT_TYPE     Select output format: mei, svg (default is svg)" << endl;

    cerr << " -w, --page-width=WIDTH     Specify the page width (default is " << DEFAULT_PAGE_WIDTH << ")" << endl;
    
    // long options only
    cerr << endl << "Additional options" << endl;
    
    cerr << " --adjust-page-height       Crop the page height to the height of the content" << endl;
    
    cerr << " --all-pages                Output all pages with one output file per page" << endl;
    
    cerr << " --page=PAGE                Select the page to engrave (default is 1)" << endl;
    
    cerr << " --help                     Display this message" << endl;
    
    cerr << " --ignore-layout            Ignore all encoded layout information (if any)" << endl;
    cerr << "                            and fully recalculate the layout" << endl;
    
    cerr << " --no-layout                Ignore all encoded layout information (if any)" << endl;
    cerr << "                            and output one single page with one single system" << endl;
    
    cerr << " --page=PAGE                Select the page to engrave (default is 1)" << endl;

    // Debugging options
    cerr << endl << "Debugging options" << endl;
    
    cerr << " --no-justification         Do not justify the system" << endl;
    
    cerr << " --show-bounding-boxes      Show symbol bounding boxes" << endl;
}


int main(int argc, char** argv)
{
    
    string infile;
    string svgdir;
    string outfile;
    string outformat = "svg";
    
    // Init random number generator for uuids
    std::srand(std::time(0));
    
    ConvertFileFormat type;
    int no_mei_hdr = 0;
    int adjust_page_height = 0;
    int all_pages = 0;
    int no_layout = 0;
    int ignore_layout = 0;
    int no_justification = 0;
    int show_bounding_boxes = 0;
    int page = 1;
    int show_help = 0;
      
    InterfaceController controller;
    
    // read pae by default
    type = pae_file;
    
    if (argc < 2) {
        cerr << "Expecting one input file." << endl << endl;
        display_usage();
        exit(1);
    }
    int c;
        
    static struct option long_options[] =
    {
        
        {"adjust-page-height",  no_argument,        &adjust_page_height, 1},
        {"all-pages",           no_argument,        &all_pages, 1},
        {"border",              required_argument,  0, 'b'},
        {"format",              required_argument,  0, 'f'},
        {"help",                no_argument,        &show_help, 1},
        {"ignore-layout",       no_argument,        &ignore_layout, 1},
        {"no-layout",           no_argument,        &no_layout, 1},
        {"no-mei-hdr",          no_argument,        &no_mei_hdr, 1},
        {"no-justification",    no_argument,        &no_justification, 1},
        {"outfile",             required_argument,  0, 'o'},
        {"page",                required_argument,  0, 0},
        {"page-height",         required_argument,  0, 'h'},
        {"page-width",          required_argument,  0, 'w'},
        {"resources",           required_argument,  0, 'r'},
        {"scale",               required_argument,  0, 's'},
        {"show-bounding-boxes", no_argument,        &show_bounding_boxes, 1},
        {"spacing-staff",       required_argument,  0, 0},
        {"spacing-system",      required_argument,  0, 0},
        {"type",                required_argument,  0, 't'},
        {0, 0, 0, 0}
    };
    
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "b:f:h:o:p:r:s:t:w:", long_options, &option_index)) != -1)
    {                
        switch (c)
        {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;
                if (strcmp(long_options[option_index].name,"page") == 0)
                    page = atoi(optarg);
                else if (strcmp(long_options[option_index].name,"spacing-staff") == 0) {
                    if ( !controller.SetSpacingStaff( atoi(optarg) ) ) {
                        exit(1);
                    }
                }
                else if (strcmp(long_options[option_index].name,"spacing-system") == 0) {
                    if ( !controller.SetSpacingSystem( atoi(optarg) ) ) {
                        exit(1);
                    }
                }
                break;
                
            case 'b':
                if ( !controller.SetBorder( atoi(optarg) ) ) {
                    exit(1);
                }
                break;
            
            case 'f':
                if ( !controller.SetFormat ( string(optarg) ) ) {
                    exit(1);
                };
                break;
                
            case 'h':
                if ( !controller.SetPageHeight( atoi(optarg) ) ) {
                    exit(1);
                };
                break;
                
            case 'o':
                outfile = string(optarg);
                break;
                
            case 'r':
                vrv::Resources::SetPath(optarg);
                break;
                
            case 't':
                outformat = string(optarg);
                break;
                
            case 's':
                if( !controller.SetScale( atoi(optarg) ) ) {
                     exit(1);
                }
                break;
            
            case 'w':
                if ( !controller.SetPageWidth( atoi(optarg) ) ) {
                    exit(1);
                }
                break;
            
            case '?':
                display_usage();
                exit(0);
                break;
                
            default:
                break;
        }
    }
    
    if (show_help) {
        display_usage();
        exit(0);
    }
    
    // Set the various flags
    controller.SetAdjustPageHeight(adjust_page_height);
    controller.SetNoLayout(no_layout);
    controller.SetIgnoreLayout(ignore_layout);
    controller.SetNoJustification(no_justification);
    controller.SetShowBoundingBoxes(show_bounding_boxes);
    
    if (optind <= argc - 1) {
        infile = string(argv[optind]);
    }
    else {
        cerr << "Incorrect number of arguments: expecting one input file." << endl << endl;
        display_usage();
        exit(1);
    }
    
    // Make sure the user uses a valid Resource path
    // Save many headaches for empty SVGs
    if(!dir_exists(vrv::Resources::GetPath())) {
        cerr << "The resources path " << vrv::Resources::GetPath() << " could not be found, please use -r option." << endl;
        exit(1);
    }
    
    if (outformat != "svg" && outformat != "mei") {
        cerr << "Output format can only be: mei svg" << endl;
        exit(1);
    }
    
    // Hardcode svg ext for now
    if (outfile.empty()) {
        outfile = removeExtension(infile);
    }
    else {
        outfile = removeExtension(outfile);
    }
    
    cerr << "Reading " << infile << "..." << endl;
    
    if ( !controller.LoadFile( infile ) ) {
        exit(1);
    }
    
    // Check the page range
    if (page > controller.GetPageCount()) {
        cerr << "The page requested (" << page << ") is not in the page range (max is " << controller.GetPageCount() << ")" << endl;
        exit(1);
    }
    if (page < 1) {
        cerr << "The page number has to be greater than 0" << endl;
        exit(1);
    }
    
    //cerr << "G1" << G1 << " C1=" << C1 << " C2=" << C2 << " C3=" << C3 << " F3=" << F3 << endl;
    
    int from = page;
    int to = page + 1;
    if (all_pages) {
        to = controller.GetPageCount() + 1;
    }
    
        
    int p;
    for (p = from; p < to; p++) {
        std::string cur_outfile = outfile;
        if (all_pages) {
            cur_outfile += StringFormat("_%03d", p);
        }
        // Create SVG or mei
        if (outformat == "svg") {
            cur_outfile += ".svg";
            if ( !controller.RenderToSvgFile( cur_outfile, p) ) {
                cerr << "Unable to write SVG to " << cur_outfile << "." << endl;
                exit(1);
            }
            // Write it to file
            
        } else {
            // To be implemented in InterfaceController
            cur_outfile += ".mei";
            if ( !controller.SaveFile( cur_outfile ) ) {
                cerr << "Unable to write MEI to " << cur_outfile << "." << endl;
                exit(1);
            }
        }
        cerr << "Output written to " << cur_outfile << endl;
    }
    
    return 0;
}
