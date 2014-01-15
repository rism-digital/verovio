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


void display_usage() {
    cerr << "Verovio command line usage:" << endl;
    cerr << "aruspix [-f pae | mei | darms] [-s scale] [-t mei | svg] [-r resources] [-o outfile] infile" << endl << endl;

    cerr << "-p, --page-height=PAGE_HEIGHT" << endl;
    cerr << "\t\tSpecify the page height (default is 2970)" << endl;

    cerr << "-w, --page-width=PAGE_WIDTH" << endl;
    cerr << "\t\tSpecify the page width (default is 2100)" << endl;
    
    cerr << "-b, --border=BORDER" << endl;
    cerr << "\t\tAdd border (10 px default, max 1000)" << endl;

    cerr << "-f, --format=INPUT_FORMAT" << endl;
    cerr << "\t\tSelect input format: darms, mei, pae" << endl;
    cerr << "\t\t(optional, default is pae)" << endl;
    
    cerr << "-o, --output=FILE_NAME" << endl;
    cerr << "\t\tOutput file name" << endl;
    
    cerr << "-r, --recources=PATH" << endl;
    cerr << "\t\tSpecify path for SVG resoucces" << endl;
    cerr << "\t\t(default in " <<  vrv::Resources::GetPath() << ")" << endl;
    
    cerr << "-s, --scale=FACTOR" << endl;
    cerr << "\t\tscale percent (100 default, max 1000)" << endl;
    
    cerr << "-t, --type=OUTPUT_TYPE" << endl;
    cerr << "\t\tSelect output format: mei, svg" << endl;
    cerr << "\t\t(optional, default is svg)" << endl;

    cerr << "--no-layout" << endl;
    cerr << "\t\tIgnore all encoded layout information (if any)" << endl;
    cerr << "\t\tand output one single page with one single system" << endl;
    
    cerr << "--ignore-layout" << endl;
    cerr << "\t\tIgnore all encoded layout information (if any)" << endl;
    cerr << "\t\tand fully recalculate the layout" << endl;
    
    cerr << "--adjust-page-height" << endl;
    cerr << "\t\tCrop the page height to the height of the content" << endl;
    
    cerr << "--no-justification" << endl;
    cerr << "\t\tDo not justify the system (for debugging purposes)" << endl;
    
    cerr << "--show-bounding-boxes" << endl;
    cerr << "\t\tShow symbol bounding boxes (for debugging purposes)" << endl;
}


int main(int argc, char** argv)
{
    
    string m_infile;
    string m_svgdir;
    string m_outfile;
    string m_outformat = "svg";
    
    // Init random number generator for uuids
    std::srand(std::time(0));
    
    ConvertFileFormat m_type;
    int m_no_mei_hdr = 0;
    int m_adjust_page_height = 0;
    int m_no_layout = 0;
    int m_ignore_layout = 0;
    int m_no_justification = 0;
    int m_show_bounding_boxes = 0;
      
    InterfaceController controller;
    
    // read pae by default
    m_type = pae_file;
    
    if (argc < 2) {
        cerr << "Expecting one input file." << endl << endl;
        display_usage();
        exit(1);
    }
    int c;
        
    static struct option long_options[] =
    {
        
        {"adjust-page-height",  no_argument,        &m_adjust_page_height, 1},
        {"border",              required_argument,  0, 'b'},
        {"format",              required_argument,  0, 'f'},
        {"no-layout",           no_argument,        &m_no_layout, 1},
        {"no-mei-hdr",          no_argument,        &m_no_mei_hdr, 1},
        {"no-justification",    no_argument,        &m_no_justification, 1},
        {"output",              required_argument,  0, 'o'},
        {"page-height",         required_argument,  0, 'p'},
        {"page-width",          required_argument,  0, 'w'},        
        {"resources",           required_argument,  0, 'r'},
        {"scale",               required_argument,  0, 's'},
        {"show-bounding-boxes", no_argument,        &m_show_bounding_boxes, 1},
        {"type",                required_argument,  0, 't'},
        {0, 0, 0, 0}
    };
    
    while ((c = getopt_long(argc, argv, "b:f:o:p:r:s:t:w:", long_options, NULL)) != -1)
    {                
        switch (c)
        {
            case '0': break;

            case 'f': {
                string informat = string(optarg);
                if (informat == "pae")
                    m_type = pae_file;
                else if(informat == "darms")
                    m_type = darms_file;
                else if(informat == "mei")
                    m_type = mei_file;
                else {
                    cerr << "Input format can only be: pae mei darms" << endl;
                    display_usage();
                    exit(1);
                }
            }
                break;
                
            case 'r':
                vrv::Resources::SetPath(optarg);
                break;
                
            case 'o':
                m_outfile = string(optarg);
                break;
            
            case 'p':
                controller.SetPageHeight(atoi(optarg));
                break;
 
            case 'w':
                controller.SetPageWidth(atoi(optarg));
                break;
                
            case 't':
                m_outformat = string(optarg);
                break;
                
            case 's':
                controller.SetScale( atoi(optarg) );
                break;
                
            case 'b':
                controller.SetBorder( atoi(optarg) );
                break;
                
            case 'h':
                display_usage();
                exit(0);
                break;
            
            case '?':
                display_usage();
                exit(0);
                break;
                
            default:
                break;
        }
    }
    
    // Set the various flags
    controller.SetAdjustPageHeight(m_adjust_page_height);
    controller.SetNoLayout(m_no_layout);
    controller.SetIgnoreLayout(m_ignore_layout);
    controller.SetNoJustification(m_no_justification);
    controller.SetShowBoundingBoxes(m_show_bounding_boxes);
    
    if (optind <= argc - 1) {
        m_infile = string(argv[optind]);
    }
    else {
        cerr << "Incorrect number of options: expecting one input file." << endl << endl;
        display_usage();
        exit(1);
    }
    
    if (m_outformat != "svg" && m_outformat != "mei") {
        cerr << "Output format can only be: mei svg" << endl;
        exit(1);
    }
    
    if (m_outformat == "mei" && m_type == mei_file) {
        cerr << "Cannot convert from mei to mei (do not specify -f mei and -t mei)." << endl;
        exit(1);
    }
    
    if (controller.GetScale() > 1000) {
        cerr << "Scale cannot be > 1000." << endl;
        exit(1);
    }
    
    if (m_outfile.empty()) {
        m_outfile = removeExtension(basename(m_infile)) + "." + m_outformat;
    }
    
    cerr << "Reading " << m_infile << "..." << endl;

    controller.SetFormat(m_type);
    
    if ( !controller.LoadFile( m_infile ) ) {
        exit(1);
    }
    
    // Create SVG or mei
    if (m_outformat == "svg") {
        if ( !controller.RenderToSvgFile( m_outfile ) ) {
            cerr << "Unable to write SVG to " << m_outfile << "." << endl;
            exit(1);
        }
        // Write it to file
        
    } else {
        // To be implemented in InterfaceController
        /*
        XMLOutput meioutput(doc, m_outfile.c_str());
        if (!meioutput.ExportFile()) {
            cerr << "Unable to write MEI to " << m_outfile << "." << endl;
            exit(1);
        }
        */
        
    }
    cerr << "Output written to " << m_outfile << endl;
    
    return 0;
}
