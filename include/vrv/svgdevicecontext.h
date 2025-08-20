/////////////////////////////////////////////////////////////////////////////
// Name:        svgdevicecontext.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SVG_DC_H__
#define __VRV_SVG_DC_H__

#include <fstream>
#include <list>
#include <set>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "object.h"
#include "options.h"

//----------------------------------------------------------------------------

class Glyph;
class Resources;

namespace vrv {

//----------------------------------------------------------------------------
// SvgDeviceContext
//----------------------------------------------------------------------------

/**
 * This class implements a drawing context for generating SVG files.
 * The music font is embedded by incorporating ./data/[fontname]/[glyph].xml glyphs within
 * the SVG file.
 */
class SvgDeviceContext : public DeviceContext {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    SvgDeviceContext(const std::string &docId);
    virtual ~SvgDeviceContext();
    ///@}

    /**
     * @name Setters
     */
    ///@{
    void SetBackground(int color, int style = PEN_SOLID) override;
    void SetBackgroundImage(void *image, double opacity = 1.0) override;
    void SetBackgroundMode(int mode) override;
    void SetTextForeground(int color) override;
    void SetTextBackground(int color) override;
    void SetLogicalOrigin(int x, int y) override;
    ///@}

    /**
     * @name Getters
     */
    ///@{
    Point GetLogicalOrigin() override;
    ///}

    /**
     * Get the SVG into a string.
     * Add the xml tag if necessary.
     */
    std::string GetStringSVG(bool xml_declaration = false);

    /**
     * @name Drawing methods
     */
    ///@{
    void DrawQuadBezierPath(Point bezier[3]) override;
    void DrawCubicBezierPath(Point bezier[4]) override;
    void DrawCubicBezierPathFilled(Point bezier1[4], Point bezier2[4]) override;
    void DrawBentParallelogramFilled(Point side[4], int height) override;
    void DrawCircle(int x, int y, int radius) override;
    void DrawEllipse(int x, int y, int width, int height) override;
    void DrawEllipticArc(int x, int y, int width, int height, double start, double end) override;
    void DrawLine(int x1, int y1, int x2, int y2) override;
    void DrawPolyline(int n, Point points[], bool close) override;
    void DrawPolygon(int n, Point points[]) override;
    void DrawRectangle(int x, int y, int width, int height) override;
    void DrawRotatedText(const std::string &text, int x, int y, double angle) override;
    void DrawRoundedRectangle(int x, int y, int width, int height, int radius) override;
    void DrawText(const std::string &text, const std::u32string &wtext = U"", int x = VRV_UNSET, int y = VRV_UNSET,
        int width = VRV_UNSET, int height = VRV_UNSET) override;
    void DrawMusicText(const std::u32string &text, int x, int y, bool setSmuflGlyph = false) override;
    void DrawSpline(int n, Point points[]) override;
    void DrawGraphicUri(int x, int y, int width, int height, const std::string &uri) override;
    void DrawSvgShape(int x, int y, int width, int height, double scale, pugi::xml_node svg) override;
    void DrawBackgroundImage(int x = 0, int y = 0) override;
    ///@}

    /**
     * @name Method for starting and ending a text
     */
    ///@{
    void StartText(int x, int y, data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_left) override;
    void EndText() override;

    /**
     * @name Move a text to the specified position, for example when starting a new line.
     */
    ///@{
    void MoveTextTo(int x, int y, data_HORIZONTALALIGNMENT alignment) override;
    void MoveTextVerticallyTo(int y) override;
    ///@}

    /**
     * @name Method for starting and ending a graphic
     */
    ///@{
    void StartGraphic(Object *object, const std::string &gClass, const std::string &gId, GraphicID graphicID = PRIMARY,
        bool prepend = false) override;
    void EndGraphic(Object *object, View *view) override;
    ///@}

    /**
     * @name Method for starting and ending a graphic custom graphic that do not correspond to an Object
     */
    ///@{
    void StartCustomGraphic(const std::string &name, std::string gClass = "", std::string gId = "") override;
    void EndCustomGraphic() override;
    ///@}

    /**
     * Method for changing the color of a custom graphic
     */
    void SetCustomGraphicColor(const std::string &color) override;

    /**
     * Method for adding custom graphic data-* attributes
     */
    void SetCustomGraphicAttributes(const std::string &data, const std::string &value) override;

    /**
     * @name Methods for re-starting and ending a graphic for objects drawn in separate steps
     */
    ///@{
    void ResumeGraphic(Object *object, std::string gId) override;
    void EndResumedGraphic(Object *object, View *view) override;
    ///@}

    /**
     * @name Method for starting and ending a text (<tspan>) text graphic
     */
    ///@{
    void StartTextGraphic(Object *object, const std::string &gClass, const std::string &gId) override;
    void EndTextGraphic(Object *object, View *view) override;
    ///@}

    /**
     * @name Method for rotating a graphic (clockwise).
     */
    ///@{
    void RotateGraphic(Point const &orig, double angle) override;
    ///@}

    /**
     * @name Method for starting and ending page
     */
    ///@{
    void StartPage() override;
    void EndPage() override;
    ///@}

    /**
     * @name Method for adding description element
     */
    ///@{
    void AddDescription(const std::string &text) override;
    ///@}

    /**
     * Add id, data-id and class attributes
     */
    void AppendIdAndClass(const std::string &gId, const std::string &baseClass, const std::string &addedClasses,
        GraphicID graphicID = PRIMARY);

    /**
     * Append additional attributes, as given in m_svgAdditionalAttributes
     */
    void AppendAdditionalAttributes(Object *object);

    /**
     * In SVG use global styling but not with mm output (for pdf generation)
     */
    bool UseGlobalStyling() override { return !m_mmOutput; }

    /**
     * Setting mm output flag (false by default)
     */
    void SetMMOutput(bool mmOutput) { m_mmOutput = mmOutput; }

    void SetFacsimile(bool facsimile) { m_facsimile = facsimile; }
    bool GetFacsimile() { return m_facsimile; }

    /**
     * Setting use Liberation flag (false by default)
     */
    void SetUseLiberation(bool useLiberation) { m_useLiberation = useLiberation; }

    /**
     * Setting m_svgBoudingBoxes flag (false by default)
     */
    void SetSvgBoundingBoxes(bool svgBoundingBoxes) { m_svgBoundingBoxes = svgBoundingBoxes; }

    /**
     * Setting m_svgViewBox flag (false by default)
     */
    void SetSvgViewBox(bool svgViewBox) { m_svgViewBox = svgViewBox; }

    /**
     * Setting m_html5 flag (false by default)
     */
    void SetHtml5(bool html5) { m_html5 = html5; }

    /**
     * Setter for indent of the SVG (default is 3, -1 for tabs)
     */
    void SetIndent(int indent) { m_indent = indent; }

    /**
     * Set the SVG to have 'raw' formatting, with no extraneous whitespace or newlines.
     */
    void SetFormatRaw(bool rawFormat) { m_formatRaw = rawFormat; }

    /**
     * Removes the xlink: prefex on href attributes, necessary for some newer browsers.
     */
    void SetRemoveXlink(bool removeXlink) { m_removeXlink = removeXlink; }

    /**
     * Setter for an additional CSS
     */
    void SetCss(const std::string &css)
    {
        m_css = css;
        this->PrefixCssRules(m_css);
    }

    /**
     *  Copies additional attributes of defined elements to the SVG, each string in the form "elementName@attribute"
     * (e.g., "note@pname")
     */
    void SetAdditionalAttributes(const std::vector<std::string> &additionalAttributes)
    {
        for (std::string s : additionalAttributes) {
            std::string className = s.substr(0, s.find("@")); // parse <element@attribute>, e.g., "note@pname"
            std::string attributeName = s.substr(s.find("@") + 1);
            ClassId classId = ObjectFactory::GetInstance()->GetClassId(className);
            m_svgAdditionalAttributes.insert({ classId, attributeName });
        }
    }

    /**
     * Setter for a the smufl text font option
     */
    void SetSmuflTextFont(option_SMUFLTEXTFONT smuflTextFont) { m_smuflTextFont = smuflTextFont; }

private:
    /**
     * Copy the content of a file to the output stream.
     * This is used for copying <defs> items.
     */
    bool CopyFileToStream(const std::string &filename, std::ostream &dest);

    /**
     * Internal method for drawing debug SVG bounding box
     */
    void DrawSvgBoundingBox(Object *object, View *view);

    /**
     * Internal method for drawing debug SVG bounding box
     */
    void DrawSvgBoundingBoxRectangle(int x, int y, int width, int height);

    /**
     * Change the flag for indicating the use of the music font as text font
     */
    void VrvTextFont() { m_vrvTextFont = true; }

    /**
     * Change the flag for indicating the use of the fallback music font as text font
     */
    void VrvTextFontFallback() { m_vrvTextFontFallback = true; }

    /**
     * Include the smufl text font either embedded or linked depending on m_smuflTextFont
     */
    void IncludeTextFont(const std::string &fontname, const Resources *resources);

    /**
     * Flush the data to the internal buffer.
     * Adds the xml tag if necessary and the <defs> from m_smuflGlyphs
     */
    void Commit(bool xml_declaration);

    void WriteLine(std::string);

    std::string GetColor(int color) const;

    pugi::xml_node AddChild(std::string name);

    /**
     * Transform pen properties into stroke attributes
     */
    ///@{
    void AppendStrokeLineCap(pugi::xml_node node, const Pen &pen);
    void AppendStrokeLineJoin(pugi::xml_node node, const Pen &pen);
    void AppendStrokeDashArray(pugi::xml_node node, const Pen &pen);
    ///@}

    /**
     * Prefix the CSS rules with a #docId for scoping them to the SVG
     */
    void PrefixCssRules(std::string &rules);

public:
    //
private:
    /**
     * Flag for indicating if the music font is currently used as text font.
     * If used, it has to be initialized to false (e.g., in the overriden version of StartPage) and will be changed in
     * DeviceContext::VrvTextFont. The font is included as woff2 in Commit()
     */
    bool m_vrvTextFont;

    /**
     * Flag indicating we need a fallback font for the music Glyphs
     */
    bool m_vrvTextFontFallback;

    // we use a std::stringstream because we want to prepend the <defs> which will know only when we reach the end of
    // the page
    // some viewer seem to support to have the <defs> at the end, but some do not (pdf2svg, for example)
    // for this reason, the full svg is finally written a string from the destructor or when Flush() is called
    std::ostringstream m_outdata;

    bool m_committed; // did we flushed the file?
    int m_originX, m_originY;

    // Here we hold references to all different glyphs used so far,
    // including any glyph for the same code but from different fonts.
    // They will be added at the end of the file as <defs>.
    // With multiple font support we need to keep track of:
    //  a) the glyph (to check if is has been already added)
    //  b) the id assigned to glyphs on the (that is has been consumed by the already rendered elements)
    // To keep things as similar as possible to previous versions we generate ids with as uuuu-ss (where uuuu is the
    // Smulf code for the glyph and ss the per-session suffix) for most of the cases (single font usage). When the same
    // glyph has been used from several fonts we use uuuu-n-ss where n indicates the collision count.
    class GlyphRef {
    public:
        GlyphRef(const Glyph *glyph, int count, const std::string &postfix);
        const Glyph *GetGlyph() const { return m_glyph; };
        const std::string &GetRefId() const { return m_refId; };

    private:
        const Glyph *m_glyph;
        std::string m_refId;
    };
    const std::string InsertGlyphRef(const Glyph *glyph);
    std::vector<std::pair<const Glyph *, GlyphRef>> m_smuflGlyphs;
    std::map<std::string, int> m_glyphCodeFontCounter;

    // pugixml data
    pugi::xml_document m_svgDoc;
    pugi::xml_node m_svgNode;
    pugi::xml_node m_pageNode;
    pugi::xml_node m_currentNode;
    std::list<pugi::xml_node> m_svgNodeStack;

    // output as mm (for pdf generation with a 72 dpi)
    bool m_mmOutput;
    bool m_facsimile;
    // use LiberationTextFont
    bool m_useLiberation;
    // add bouding boxes in svg output
    bool m_svgBoundingBoxes;
    // use viewbox on svg root element
    bool m_svgViewBox;
    // output HTML5 data-* attributes
    bool m_html5;
    // additional CSS
    std::string m_css;
    // copy additional attributes of given elements to the SVG, in the form "note@pname; layer@n"
    std::multimap<ClassId, std::string> m_svgAdditionalAttributes;
    // format output as raw, stripping extraneous whitespace and non-content newlines
    bool m_formatRaw;
    // remove xlink from href attributes
    bool m_removeXlink;
    // indentation value (-1 for tabs)
    int m_indent;
    // postfix to be added to font glyphs
    std::string m_glyphPostfixId;
    // embedding of the smufl text font
    option_SMUFLTEXTFONT m_smuflTextFont;
    // the document id
    std::string m_docId;
};

} // namespace vrv

#endif // __VRV_SVG_DC_H__
