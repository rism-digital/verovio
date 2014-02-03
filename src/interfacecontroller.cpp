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

#include "doc.h"
#include "iodarms.h"
#include "iomei.h"
#include "iomusxml.h"
#include "iopae.h"
#include "page.h"
#include "view.h"
#include "svgdevicecontext.h"

#ifdef USE_EMSCRIPTEN
#include "jsonxx.h"
#endif

namespace vrv {

//----------------------------------------------------------------------------
// InterfaceController
//----------------------------------------------------------------------------

InterfaceController::InterfaceController()
{

    m_scale = DEFAULT_SCALE;
    m_format = pae_file;
    
    // default page size
    m_pageHeight = DEFAULT_PAGE_HEIGHT;
    m_pageWidth = DEFAULT_PAGEWIDTH;
    m_border = DEFAULT_PAGE_LEFT_MAR;
    
    m_noLayout = false;
    m_ignoreLayout = false;
    m_adjustPageHeight = false;
    m_noJustification = false;
    m_showBoundingBoxes = false;
	
	m_cString = NULL;
}


InterfaceController::~InterfaceController()
{
    if (m_cString) {
        free( m_cString );
    }
}
    
bool InterfaceController::SetBorder( int border )
{
    // We use left margin values because for now we cannot specify different values for each margin
    if (border < MIN_PAGE_LEFT_MAR || border > MAX_PAGE_LEFT_MAR) {
        LogError( "Border out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_PAGE_LEFT_MAR, MIN_PAGE_LEFT_MAR, MAX_PAGE_LEFT_MAR );
        return false;
    }
    m_border = border;
    return true;
}
    
bool InterfaceController::SetScale( int scale )
{
    if (scale < MIN_SCALE || scale > MAX_SCALE) {
        LogError( "Scale out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_SCALE, MIN_SCALE, MAX_SCALE );
        return false;
    }
    m_scale = scale;
    return true;
}
    
bool InterfaceController::SetPageHeight( int h )
{
    if (h < MIN_PAGEHEIGHT || h > MAX_PAGEHEIGHT) {
        LogError( "Page height out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_PAGE_HEIGHT, MIN_PAGEHEIGHT, MAX_PAGEHEIGHT );
        return false;
    }
    m_pageHeight = h;
    return true;
}
    
bool InterfaceController::SetPageWidth( int w )
{
    if (w < MIN_PAGEWIDTH || w > MAX_PAGEWIDTH) {
        LogError( "Page width out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_PAGEWIDTH, MIN_PAGEWIDTH, MAX_PAGEWIDTH );
        return false;
    }
    m_pageWidth = w;
    return true;
};
    

bool InterfaceController::SetFormat( std::string informat )
{
    if (informat == "pae")
        m_format = pae_file;
    else if(informat == "darms")
        m_format = darms_file;
    else if(informat == "mei")
        m_format = mei_file;
    else {
        LogError("Input format can only be: pae mei or darms");
        return false;
    }
    return true;
};
    

    
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
    
    // ignore layout?
    if ( m_ignoreLayout || m_noLayout ) {
        input->IgnoreLayoutInformation();
    }
    
    // load the file
    if ( !input->ImportString( data )) {
        LogError( "Error importing data" );
        delete input;
        return false;
    }
    
    m_doc.SetPageHeight( this->GetPageHeight() );
    m_doc.SetPageWidth( this->GetPageWidth() );
    m_doc.SetPageRightMar( this->GetBorder() );
    m_doc.SetPageLeftMar( this->GetBorder() );
    m_doc.SetPageTopMar( this->GetBorder() );
    
    // do the layout? this depends on the options and of the
    // file. PAE and DARMS of no layout information. MEI files
    // can have, but this might have been ignored because of the
    // --ignore-layout option. We won't do it if --no-layout option
    // was set, though.
    if (!input->HasLayoutInformation() && !m_noLayout) {
        //LogElapsedTimeStart();
        m_doc.Layout();
        //LogElapsedTimeEnd("layout");
    }
    
    delete input;
    m_view.SetDoc( &m_doc );
    
    return true;
}

bool InterfaceController::ParseOptions( std::string json_options ) {
#ifdef USE_EMSCRIPTEN
    
    jsonxx::Object json;
        
    // Read JSON options
    if (!json.parse(json_options)) {
        LogError( "Can not parse JSON string." );
        return false;
    }
    
    if (json.has<jsonxx::String>("inputFormat"))
        SetFormat( json.get<jsonxx::String>("inputFormat") );
    
    if (json.has<jsonxx::Number>("scale"))
        SetScale( json.get<jsonxx::Number>("scale") );
    
    if (json.has<jsonxx::Number>("border"))
        SetBorder( json.get<jsonxx::Number>("border") );

    if (json.has<jsonxx::Number>("pageWidth"))
        SetPageWidth( json.get<jsonxx::Number>("pageWidth") );
    
    if (json.has<jsonxx::Number>("pageHeight"))
        SetPageHeight( json.get<jsonxx::Number>("pageHeight") );
    
    // Parse the various flags
    // Note: it seems that there is a bug with jsonxx and emscripten
    // Boolean value false do not parse properly. We have to use Number instead
    
    if (json.has<jsonxx::Number>("noLayout"))
        SetNoLayout(json.get<jsonxx::Number>("noLayout"));
    
    if (json.has<jsonxx::Number>("ignoreLayout"))
        SetIgnoreLayout(json.get<jsonxx::Number>("ignoreLayout"));

    if (json.has<jsonxx::Number>("adjustPageHeight"))
        SetAdjustPageHeight(json.get<jsonxx::Number>("adjustPageHeight"));

    if (json.has<jsonxx::Number>("noJustification"))
        SetNoJustification(json.get<jsonxx::Number>("noJustification"));

    if (json.has<jsonxx::Number>("showBoundingBoxes"))
        SetShowBoundingBoxes(json.get<jsonxx::Number>("showBoundingBoxes"));
        
    return true;
    
#else
    // The non js version of the app should not use this function.
    return false;
#endif
    
}

std::string InterfaceController::RenderToSvg( int pageNo, bool xml_tag )
{
    // Page number is one-based - correction to 0-based first
    pageNo--;
    
    // Get the current system for the SVG clipping size
    m_view.SetPage( pageNo );
    
    // Adjusting page width and height according to the options
    int width = m_pageWidth;
    if ( m_noLayout ) {
        width = m_doc.GetAdjustedDrawingPageWidth();
    }
    
    int height = m_pageHeight;
    if ( m_adjustPageHeight || m_noLayout ) {
        height = m_doc.GetAdjustedDrawingPageHeight();
    }
    
    // Create the SVG object, h & w come from the system
    // We will need to set the size of the page after having drawn it depending on the options
    SvgDeviceContext svg( width, height );
    
    // set scale and border from user options
    svg.SetUserScale((double)m_scale / 100, (double)m_scale / 100);
    
    // debug BB?
    svg.SetDrawBoundingBoxes(m_showBoundingBoxes);
    
    // render the page
    m_view.DrawCurrentPage( &svg, false );
    
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


int InterfaceController::GetPageCount() {
	return m_doc.GetPageCount();
}

void InterfaceController::SetCString( std::string data )
{
    if (m_cString) {
        free(m_cString);
        m_cString = NULL;
    }
    
    m_cString = (char *)malloc(strlen(data.c_str()) + 1);
    
    // something went wrong
    if (!m_cString) {
        return;
    }
    strcpy(m_cString, data.c_str());
}

const char *InterfaceController::GetCString( )
{
    if (m_cString) {
        return m_cString;
    }
    else {
        return "[unspecified]";
    }
}

} //namespace vrv