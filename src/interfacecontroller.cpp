/////////////////////////////////////////////////////////////////////////////
// Name:        controller.cpp
// Author:      Laurent Pugin
// Created:     17/10/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "interfacecontroller.h"

//----------------------------------------------------------------------------

#include <iostream>

//----------------------------------------------------------------------------

#include "bboxdc.h"
#include "doc.h"
#include "iodarms.h"
#include "iomei.h"
#include "iomusxml.h"
#include "iopae.h"
#include "page.h"
#include "view.h"
#include "svgdc.h"
#include "system.h"

#ifdef USE_EMSCRIPTEN
#include "jsonxx.h"
#endif

namespace vrv {

//----------------------------------------------------------------------------
// InterfaceController
//----------------------------------------------------------------------------

InterfaceController::InterfaceController()
{
    m_border = 10;
    m_scale = 100;
    m_format = pae_file;
    
    // default page size
    m_pageHeight = 2970;
    m_pageWidth = 2100;
    
    m_noLayout = false;
    m_ignoreLayout = false;
    m_adjustPageHeight = false;
    m_noJustification = false;
    m_showBoundingBoxes = false;
    
}


InterfaceController::~InterfaceController()
{
}

bool InterfaceController::LoadFile( std::string filename )
{
    std::ifstream in( filename.c_str() );
    in.seekg(0, std::ios::end);
    std::streamsize fileSize = (std::streamsize)in.tellg();
    in.seekg(0, std::ios::beg);
    
    // read the file into the string:
    std::string content( fileSize, 0 );
    in.read(&content[0], fileSize);
    
    return LoadString( content );
}

bool InterfaceController::LoadString( std::string data )
{
    FileInputStream *input = NULL;
    if (m_format == pae_file) {
        input = new PaeInput( &m_doc, "" );
    } else if (m_format == darms_file) {
        input = new DarmsInput( &m_doc, "" );
    } else if (m_format == mei_file) {
        input = new MeiInput( &m_doc, "" );
    }
    else {
        LogError( "Unknown format" );
        return false;
    }
    
    // something went wrong
    if ( !input ) {
        LogError( "Unknown error" );
        return false;
    }
    
    // load the file
    if ( !input->ImportString( data )) {
        LogError( "Error importing data" );
        delete input;
        return false;
    }
    
    delete input;
    
    m_view.SetDoc( &m_doc );
    m_doc.Layout();
    
    return true;
}

bool InterfaceController::ParseOptions( std::string json_options ) {
#ifdef USE_EMSCRIPTEN
    std::string in_format = "pae";
    
    int scale = m_scale;
    int border = m_border;
    
    int width = m_pageWidth;
    int height = m_pageHeight;
    
    jsonxx::Object json;
        
    // Read JSON options
    if (!json.parse(json_options)) {
        LogError( "Can not parse JSON string." );
        return false;
    }
    
    if (json.has<jsonxx::String>("inputFormat"))
        in_format = json.get<jsonxx::String>("inputFormat");
    
    if (json.has<jsonxx::Number>("scale"))
        scale = json.get<jsonxx::Number>("scale");
    
    if (json.has<jsonxx::Number>("border"))
        border = json.get<jsonxx::Number>("border");

    if (json.has<jsonxx::Number>("pageWidth"))
        width = json.get<jsonxx::Number>("pageWidth");
    
    if (json.has<jsonxx::Number>("pageHeight"))
        height = json.get<jsonxx::Number>("pageHeight");
    
    // Parse the various flags
    if (json.has<jsonxx::Boolean>("noLayout"))
        SetNoLayout(json.get<jsonxx::Boolean>("noLayout"));
    
    if (json.has<jsonxx::Boolean>("ignoreLayout"))
        SetIgnoreLayout(json.get<jsonxx::Boolean>("ignoreLayout"));

    if (json.has<jsonxx::Boolean>("adjustPageHeight"))
        SetAdjustPageHeight(json.get<jsonxx::Boolean>("adjustPageHeight"));

    if (json.has<jsonxx::Boolean>("noJustification"))
        SetNoJustification(json.get<jsonxx::Boolean>("noJustification"));

    if (json.has<jsonxx::Boolean>("showBoundingBoxes"))
        SetShowBoundingBoxes(json.get<jsonxx::Boolean>("showBoundingBoxes"));
    
    // set file type
    if (in_format == "pae") 
        SetFormat(pae_file);
    else if (in_format == "mei")
        SetFormat(mei_file);
    else if (in_format == "darms")
        SetFormat(darms_file);
    else { // fail if format in invalid
        LogError( "InputFormat is invalid: %s\n", in_format.c_str() );
        return false;
    }
    
    // Check boundaries for scale and border
    
    if (border < 0 || border > 1000)
        LogError( "Border out of bounds, use 10 (default)." );
    else
        SetBorder(border);
        
    if (scale < 0 || scale > 1000)
        LogError( "Scale out of bounds, use 10 (default)." );
    else
        SetScale(scale);
    
    if (width < 0 || width > 5000)
        LogError( "Page width out of bounds" );
    else
        SetPageWidth(width);
    
    if (height < 0 || height > 5000)
        LogError( "Page Height out of bounds." );
    else
        SetPageHeight(height);
    
    return true;
    
#else
    // The non js version of the app should not use this function.
    return false;
#endif
    
}

std::string InterfaceController::RenderToSvg( int pageNo, bool xml_tag )
{
    // Get the current system for the SVG clipping size
    Page *page = dynamic_cast<Page*>(m_doc.m_children[pageNo]);
    //System *system = dynamic_cast<System*>(page->m_children[0]);
    
    // Create the SVG object, h & w come from the system
    // we add border*2 so it is centered into the image
    //SvgDeviceContext svg(system->m_contentBB_x2 - system->m_contentBB_x1 + m_border * 2, (system->m_contentBB_y2 - system->m_contentBB_y1) + m_border * 2);
    SvgDeviceContext svg( m_pageWidth + m_border * 2, m_pageHeight + m_border * 2 );
    
    // set scale and border from user options
    svg.SetUserScale((double)m_scale / 100, (double)m_scale / 100);
    svg.SetLogicalOrigin(m_border, m_border);
    
    // debug BB?
    svg.SetDrawBoundingBoxes(m_showBoundingBoxes);
    
    // render the page
    m_view.DrawPage( &svg, page , false);
    
    std::string out_str = svg.GetStringSVG( xml_tag );
    return out_str;
}


bool InterfaceController::RenderToSvgFile( std::string filename, int pageNo )
{
    std::string output = RenderToSvg( pageNo, true );
    
    std::ofstream outfile;
    outfile.open ( filename.c_str() );
    
    if ( !outfile.is_open() ) {
        // add message?
        return false;
    }
    
    outfile << output;
    outfile.close();
    return true;
}
    
} //namespace vrv