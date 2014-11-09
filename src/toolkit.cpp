/////////////////////////////////////////////////////////////////////////////
// Name:        toolkit.cpp
// Author:      Laurent Pugin
// Created:     17/10/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "toolkit.h"

//----------------------------------------------------------------------------

#include <iostream>

//----------------------------------------------------------------------------

#include "doc.h"
#include "iodarms.h"
#include "iomei.h"
#include "iomusxml.h"
#include "iopae.h"
#include "page.h"
#include "svgdevicecontext.h"
#include "view.h"

#ifdef USE_EMSCRIPTEN
#include "jsonxx.h"
#endif

namespace vrv {

//----------------------------------------------------------------------------
// Toolkit
//----------------------------------------------------------------------------

Toolkit::Toolkit()
{

    m_scale = DEFAULT_SCALE;
    m_format = pae_file;
    
    // default page size
    m_pageHeight = DEFAULT_PAGE_HEIGHT;
    m_pageWidth = DEFAULT_PAGE_WIDTH;
    m_border = DEFAULT_PAGE_LEFT_MAR;
    m_spacingStaff = DEFAULT_SPACING_STAFF;
    m_spacingSystem = DEFAULT_SPACING_SYSTEM;
    
    m_noLayout = false;
    m_ignoreLayout = false;
    m_adjustPageHeight = false;
    m_noJustification = false;
    m_showBoundingBoxes = false;
	
	m_cString = NULL;
}


Toolkit::~Toolkit()
{
    if (m_cString) {
        free( m_cString );
    }
}
    
bool Toolkit::SetBorder( int border )
{
    // We use left margin values because for now we cannot specify different values for each margin
    if (border < MIN_PAGE_LEFT_MAR || border > MAX_PAGE_LEFT_MAR) {
        LogError( "Border out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_PAGE_LEFT_MAR, MIN_PAGE_LEFT_MAR, MAX_PAGE_LEFT_MAR );
        return false;
    }
    m_border = border;
    return true;
}
    
bool Toolkit::SetScale( int scale )
{
    if (scale < MIN_SCALE || scale > MAX_SCALE) {
        LogError( "Scale out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_SCALE, MIN_SCALE, MAX_SCALE );
        return false;
    }
    m_scale = scale;
    return true;
}
    
bool Toolkit::SetPageHeight( int h )
{
    if (h < MIN_PAGE_HEIGHT || h > MAX_PAGE_HEIGHT) {
        LogError( "Page height out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_PAGE_HEIGHT, MIN_PAGE_HEIGHT, MAX_PAGE_HEIGHT );
        return false;
    }
    m_pageHeight = h;
    return true;
}
    
bool Toolkit::SetPageWidth( int w )
{
    if (w < MIN_PAGE_WIDTH || w > MAX_PAGE_WIDTH) {
        LogError( "Page width out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_PAGE_WIDTH, MIN_PAGE_WIDTH, MAX_PAGE_WIDTH );
        return false;
    }
    m_pageWidth = w;
    return true;
};
    
bool Toolkit::SetSpacingStaff( int spacingStaff )
{
    if (spacingStaff < MIN_SPACING_STAFF || spacingStaff > MAX_SPACING_STAFF) {
        LogError( "Spacing staff out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_SPACING_STAFF, MIN_SPACING_STAFF, MAX_SPACING_STAFF );
        return false;
    }
    m_spacingStaff = spacingStaff;
    return true;
}
    
    
bool Toolkit::SetSpacingSystem( int spacingSystem )
{
    if (spacingSystem < MIN_SPACING_SYSTEM || spacingSystem > MAX_SPACING_SYSTEM) {
        LogError( "Spacing system out of bounds; default is %d, minimun is %d, and maximum is %d", DEFAULT_SPACING_SYSTEM, MIN_SPACING_SYSTEM, MAX_SPACING_SYSTEM );
        return false;
    }
    m_spacingSystem = spacingSystem;
    return true;
}
    

bool Toolkit::SetFormat( std::string const &informat )
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
    

    
bool Toolkit::LoadFile( const std::string &filename )
{
    std::ifstream in( filename.c_str() );
    
    if (!in.is_open()) {
        return false;
    }
    
    in.seekg(0, std::ios::end);
    std::streamsize fileSize = (std::streamsize)in.tellg();
    in.seekg(0, std::ios::beg);
    
    // read the file into the string:
    std::string content( fileSize, 0 );
    in.read(&content[0], fileSize);

    return LoadString( content );
}

bool Toolkit::LoadString( const std::string &data )
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
    
    // rdg xpath query?
    if ( m_rdgXPathQuery.length() > 0 ) {
        input->SetRdgXPathQuery( m_rdgXPathQuery );
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
    m_doc.SetSpacingStaff( this->GetSpacingStaff() );
    m_doc.SetSpacingSystem( this->GetSpacingSystem() );
    
    m_doc.PrepareDrawing();
    
    // do the layout? this depends on the options and of the
    // file. PAE and DARMS of no layout information. MEI files
    // can have, but this might have been ignored because of the
    // --ignore-layout option. We won't do it if --no-layout option
    // was set, though.
    if (!input->HasLayoutInformation() && !m_noLayout) {
        //LogElapsedTimeStart();
        m_doc.LayOut();
        //LogElapsedTimeEnd("layout");
    }
    
    delete input;
    m_view.SetDoc( &m_doc );
    
    return true;
}
    

std::string Toolkit::GetMEI( int pageNo )
{
    MeiOutput meioutput( &m_doc, "" );
    return meioutput.GetOutput();
}

    
bool Toolkit::SaveFile( const std::string &filename )
{
    MeiOutput meioutput( &m_doc, filename.c_str());
    if (!meioutput.ExportFile()) {
        LogError( "Unknown error" );
        return false;
    }
    return true;
}

bool Toolkit::ParseOptions( const std::string &json_options ) {
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
    
    if (json.has<jsonxx::Number>("spacingStaff"))
        SetSpacingStaff( json.get<jsonxx::Number>("spacingStaff") );
    
    if (json.has<jsonxx::Number>("spacingSystem"))
        SetSpacingSystem( json.get<jsonxx::Number>("spacingSystem") );
    
    if (json.has<jsonxx::String>("rdgXPathQuery"))
        SetRdgXPathQuery( json.get<jsonxx::String>("rdgXPathQuery") );
    
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
    

std::string Toolkit::GetLogString() {
#ifdef USE_EMSCRIPTEN
    std::string str;
    std::vector<std::string>::iterator iter;
    for (iter = logBuffer.begin(); iter != logBuffer.end(); iter++) {
        str += (*iter);
    }
    return str;
#else
    // The non js version of the app should not use this function.
    return "";
#endif
}
  

void  Toolkit::ResetLogBuffer() {
#ifdef USE_EMSCRIPTEN 
    vrv::logBuffer.clear();
#endif
}

std::string Toolkit::RenderToSvg( int pageNo, bool xml_tag )
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
    
void Toolkit::RedoLayout()
{
    m_doc.SetPageHeight( this->GetPageHeight() );
    m_doc.SetPageWidth( this->GetPageWidth() );
    m_doc.SetPageRightMar( this->GetBorder() );
    m_doc.SetPageLeftMar( this->GetBorder() );
    m_doc.SetPageTopMar( this->GetBorder() );
    m_doc.SetSpacingStaff( this->GetSpacingStaff() );
    m_doc.SetSpacingSystem( this->GetSpacingSystem() );
    
    m_doc.LayOutContinuously();
    m_doc.LayOut();
}

bool Toolkit::RenderToSvgFile( const std::string &filename, int pageNo )
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


int Toolkit::GetPageCount() {
	return m_doc.GetPageCount();
}
    
int Toolkit::GetPageWithElement( const std::string &xmlId ) {
    Object *element = m_doc.FindChildByUuid(xmlId);
    if (!element) {
        return 0;
    }
    Page *page = dynamic_cast<Page*>( element->GetFirstParent( &typeid(Page) ) );
    if (!page) {
        return 0;
    }
    return page->GetIdx() + 1;
}

void Toolkit::SetCString( const std::string &data )
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

const char *Toolkit::GetCString( )
{
    if (m_cString) {
        return m_cString;
    }
    else {
        return "[unspecified]";
    }
}

} //namespace vrv