/////////////////////////////////////////////////////////////////////////////
// Name:        toolkit.h
// Author:      Laurent Pugin
// Created:     17/10/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_TOOLKIT_H__
#define __VRV_TOOLKIT_H__

#include <string>

#ifdef USE_EMSCRIPTEN
#include "jsonxx.h"
#endif

#include "utf8.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "view.h"

namespace vrv {

typedef enum _file_formats {
    mei_file = 0,
    pae_file,
    darms_file
} ConvertFileFormat;


//----------------------------------------------------------------------------
// Toolkit
//----------------------------------------------------------------------------

class Toolkit
{
public:
    /**
     * @name Constructors and destructors
     */
    ///@{
    /** If initFont is set to false, Resources::InitFonts will have to be called explicitely */
    Toolkit( bool initFont = true );
    virtual ~Toolkit();
    ///@}
    
    /**
     * Load a file with the specified type.
     */
    bool LoadFile( const std::string &filename );
    
    /**
     * Load a string data witht he specified type.
     */
    bool LoadString( const std::string &data );
    
    /**
     * Save an MEI file.
     */
    bool SaveFile( const std::string &filename );
    
    /**
     * Parse the options passed as JSON string
     * only available for Emscripten based compiles
     **/
    bool ParseOptions( const std::string &json_options );
    
    /**
     * Parse the editor actions passed as JSON string
     * only available for Emscripten based compiles
     **/
    bool Edit( const std::string &json_editorAction );
    
    /**
     * Concatenates the vrv::logBuffer into a string an returns it.
     * This is used only for Emscripten based compilation.
     * The vrv::logBuffer is filled by the vrv::LogXXX functions.
     */
    std::string GetLogString( );
    
    /**
     * Returns the version number as a string.
     * This is used only for Emscripten based compilation.
     */
    std::string GetVersion( );
    
    /**
     * Resets the vrv::logBuffer.
     * This is used only for Emscripten based compilation.
     */
    void ResetLogBuffer();
    
    /**
     * Render the page in SVG and returns it as a string
     * Page number is 1-based
     */
    std::string RenderToSvg( int pageNo =  1, bool xml_declaration = false );

    /**
     * Render the page in SVG and save it to the file.
     * Page number is 1-based.
     */
    bool RenderToSvgFile( const std::string &filename, int pageNo =  1 );
    
    /**
     * Get the MEI as a string.
     * Get all the pages unless a page number (1-based) is specified
     */
    std::string GetMEI( int pageNo =  0, bool scoreBased = false );
    
    /**
     * Return element attributes as a JSON string
     */
    std::string GetElementAttr( const std::string &xmlId );
    
    /**
     * Redo the layout of the loaded data
     * This can be called once the rendering option were changed,
     * For example with a new page (sceen) height or a new zoom level.
     */
    void RedoLayout();
    
    /**
     * Return the page on which the element is the ID (xml:id) is rendered
     * This takes into account the current layout options.
     * Retruns 0 if the element is not found.
     */
    int GetPageWithElement( const std::string &xmlId );
    
	/**
	* @name Set and get a std::string into a char * buffer.
	* This is used for returning a string buffer to emscripten.
	* The buffer is freed when reset or in MusController destructor.
	*/
	///@{
	void SetCString( const std::string &data );
	const char *GetCString( );
	///@}

    /**
     * @name Set and get the border
     */
    ///@{
    bool SetBorder( int border );
    int GetBorder() { return m_border; };
    ///@}
    
    /**
     * @name Set and get the scale
     */
    ///@{
    bool SetScale( int scale );
    int GetScale() { return m_scale; };
    ///@}
    
    /**
     * @name Set and get the page height (in pixels)
     */
    ///@{
    bool SetPageHeight( int h );
    int GetPageHeight() { return m_pageHeight; };
    ///@}
    
    /**
     * @name Set and get the page width (in pixels)
     */
    ///@{
    bool SetPageWidth( int w );
    int GetPageWidth() { return m_pageWidth; };
    ///@}
    
    /**
     * @name Set and get the spacing staff and system
     */
    ///@{
    bool SetSpacingStaff( int spacingStaff );
    bool SetSpacingSystem( int spacingSystem );
    int GetSpacingStaff() { return m_spacingStaff; };
    int GetSpacingSystem() { return m_spacingSystem; };
    ///@}
    
    /**
     * @name Ignore all encoded layout information (if any) 
     * and output one single page with one single system
     */
    ///@{
    void SetNoLayout( bool l ) { m_noLayout = l; };
    int GetNoLayout() { return m_noLayout; };
    ///@}
    
    /**
     * @name Ignore all encoded layout information (if any) 
     * and fully recalculate the layout 
     */
    ///@{
    void SetIgnoreLayout( bool l ) { m_ignoreLayout = l; };
    int GetIgnoreLayout() { return m_ignoreLayout; };
    ///@}
    
    /**
     * @name Crop the page height to the height of the content 
     */
    ///@{
    void SetAdjustPageHeight( bool a ) { m_adjustPageHeight = a; };
    int GetAdjustPageHeight() { return m_adjustPageHeight; };
    ///@}
    
    /**
     * @name Do not justify the system (for debugging purposes) 
     */
    ///@{
    void SetNoJustification( bool j ) { m_noJustification = j; };
    int GetNoJustification() { return m_noJustification; };
    ///@}
    
    /**
     * @name Do not justify the system (for debugging purposes) 
     */
    ///@{
    void SetShowBoundingBoxes( bool b ) { m_showBoundingBoxes = b; };
    int GetShowBoundingBoxes() { return m_showBoundingBoxes; };
    ///@}
    
    /**
     * @name Get the input file format (defined as ConvertFileFormat)
     * The SetFormat with ConvertFileFormat does not perform any validation
     */
    ///@{
    bool SetFormat( std::string const &informat );
    void SetFormat( ConvertFileFormat format ) { m_format = format; };
    int GetFormat() { return m_format; };
    ///@}
    
    /**
     * @name Set and get the xPath query for selecting <rdg> (if any)
     */
    ///@{
    void SetRdgXPathQuery( std::string const &rdgXPathQuery ) { m_rdgXPathQuery = rdgXPathQuery; };
    std::string GetRdgXPathQuery() { return m_rdgXPathQuery; };
    ///@}

    /**
     * @name Set a specific font
     */
    ///@{
    bool SetFont( std::string const &font );
    ///@}
    
    /**
     * @name Get the pages for a loaded file
     * The SetFormat with ConvertFileFormat does not perform any validation
     */
    ///@{
    int GetPageCount( );
    ///@}
    
    /**
     * Experimental editor method
     */
    ///@{
    bool Drag( std::string elementId, int x, int y );
    bool Insert( std::string elementType, std::string startId, std::string endId );
    bool Set( std::string elementId, std::string attrType, std::string attrValue );
    ///@}

private:
    bool IsUTF16( const std::string &filename );
    bool LoadUTF16File( const std::string &filename );
    
    
protected:
#ifdef USE_EMSCRIPTEN
    /**
     * Experimental editor method
     */
    ///@{
    bool ParseDragAction( jsonxx::Object param, std::string *elementId, int *x, int *y );
    bool ParseInsertAction( jsonxx::Object param, std::string *elementType, std::string *startid, std::string *endid );
    bool ParseSetAction( jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue );
    ///@}
#endif
    
public:
    
private:
    Doc m_doc;
    View m_view;
    int m_scale;
    ConvertFileFormat m_format;
    
    int m_pageHeight;
    int m_pageWidth;
    int m_border; // to be replace by pageRightMar, pageLeftMar, pageTopMar
    int m_unit;
    /** given in units **/
    int m_spacingStaff;
    int m_spacingSystem;
    
    bool m_noLayout;
    bool m_ignoreLayout;
    bool m_adjustPageHeight;
    std::string m_rdgXPathQuery;
    // for debugging
    bool m_noJustification;
    bool m_showBoundingBoxes;
	
	char *m_cString;
};

} // namespace vrv
#endif
