/////////////////////////////////////////////////////////////////////////////
// Name:        svgdevicecontext.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "svgdevicecontext.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <numeric>
#include <regex>

//----------------------------------------------------------------------------

#include "doc.h"
#include "floatingobject.h"
#include "glyph.h"
#include "layerelement.h"
#include "staff.h"
#include "staffdef.h"
#include "view.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

#define space " "
#define semicolon ";"

//----------------------------------------------------------------------------
// SvgDeviceContext
//----------------------------------------------------------------------------

SvgDeviceContext::SvgDeviceContext(const std::string &docId) : DeviceContext(SVG_DEVICE_CONTEXT)
{
    m_docId = docId;

    m_originX = 0;
    m_originY = 0;

    m_smuflGlyphs.clear();

    m_committed = false;
    m_vrvTextFont = false;
    m_vrvTextFontFallback = false;

    m_mmOutput = false;
    m_svgBoundingBoxes = false;
    m_svgViewBox = false;
    m_html5 = false;
    m_formatRaw = false;
    m_removeXlink = false;
    m_facsimile = false;
    m_useLiberation = false;
    m_indent = 2;

    // create the initial SVG element
    // width and height need to be set later; these are taken care of in "commit"
    m_svgNode = m_svgDoc.append_child("svg");
    m_svgNode.append_attribute("version") = "1.1";
    m_svgNode.append_attribute("xmlns") = "http://www.w3.org/2000/svg";
    m_svgNode.append_attribute("xmlns:xlink") = "http://www.w3.org/1999/xlink";
    m_svgNode.append_attribute("overflow") = "visible";
    m_svgNode.append_attribute("id") = m_docId;

    // start the stack
    m_svgNodeStack.push_back(m_svgNode);
    m_currentNode = m_svgNode;

    m_outdata.clear();

    m_glyphPostfixId = m_docId;
}

SvgDeviceContext::~SvgDeviceContext() {}

bool SvgDeviceContext::CopyFileToStream(const std::string &filename, std::ostream &dest)
{
    std::ifstream source(filename.c_str(), std::ios::binary);
    dest << source.rdbuf();
    source.close();
    return true;
}

SvgDeviceContext::GlyphRef::GlyphRef(const Glyph *glyph, int count, const std::string &postfix) : m_glyph(glyph)
{
    // Add the counter only when necessary (more than one font for that glyph)
    if (count == 0) {
        m_refId = StringFormat("%s-%s", glyph->GetCodeStr().c_str(), postfix.c_str());
    }
    else {
        m_refId = StringFormat("%s-%d-%s", glyph->GetCodeStr().c_str(), count, postfix.c_str());
    }
};

const std::string SvgDeviceContext::InsertGlyphRef(const Glyph *glyph)
{
    const std::string code = glyph->GetCodeStr();

    // Check if glyph already exists
    for (const auto &[g, ref] : m_smuflGlyphs) {
        if (g == glyph) {
            return ref.GetRefId();
        }
    }

    int count = 0;
    auto it = m_glyphCodeFontCounter.find(code);
    if (it != m_glyphCodeFontCounter.end()) {
        count = it->second;
    }

    GlyphRef ref(glyph, count, m_glyphPostfixId);
    const std::string id = ref.GetRefId();

    m_smuflGlyphs.emplace_back(glyph, ref); // preserve insertion order
    m_glyphCodeFontCounter[code] = count + 1;

    return id;
}

void SvgDeviceContext::IncludeTextFont(const std::string &fontname, const Resources *resources)
{
    assert(resources);

    std::string cssContent;

    if (m_smuflTextFont == SMUFLTEXTFONT_embedded) {
        cssContent = resources->GetCSSFontFor(fontname);
    }
    else {
        std::string versionPath
            = (VERSION_DEV) ? "develop" : StringFormat("%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION);
        cssContent = StringFormat("@import url(\"https://www.verovio.org/javascript/%s/data/%s.css\");",
            versionPath.c_str(), fontname.c_str());
    }

    pugi::xml_node css = m_svgNode.append_child("style");
    css.append_attribute("type") = "text/css";
    css.text().set(cssContent.c_str());
}

void SvgDeviceContext::Commit(bool xml_declaration)
{
    if (m_committed) {
        return;
    }

    // take care of width/height once userScale is updated
    double height = (double)this->GetHeight() * this->GetUserScaleY();
    double width = (double)this->GetWidth() * this->GetUserScaleX();
    const char *format = "%gpx";

    if (m_mmOutput) {
        height /= 10;
        width /= 10;
        format = "%gmm";
    }
    else {
        const auto [baseWidth, baseHeight] = this->GetBaseSize();
        if (baseWidth && baseHeight) {
            height = baseHeight;
            width = baseWidth;
        }
        else {
            height = std::ceil(height);
            width = std::ceil(width);
        }
    }

    if (m_svgViewBox) {
        m_svgNode.prepend_attribute("viewBox") = StringFormat("0 0 %g %g", width, height).c_str();
    }
    else {
        m_svgNode.prepend_attribute("height") = StringFormat(format, height).c_str();
        m_svgNode.prepend_attribute("width") = StringFormat(format, width).c_str();
    }

    // add the woff2 font if needed
    if (m_smuflTextFont != SMUFLTEXTFONT_none) {
        const Resources *resources = this->GetResources(true);
        // include the selected font
        if (m_vrvTextFont && resources) {
            this->IncludeTextFont(resources->GetCurrentFont(), resources);
        }
        // include the fallback font
        if (m_vrvTextFontFallback && resources) {
            this->IncludeTextFont(resources->GetFallbackFont(), resources);
        }
    }
    if (m_useLiberation) {
        const Resources *resources = this->GetResources(true);
        if (resources) {
            this->IncludeTextFont(resources->GetTextFont(), resources);
        }
    }

    // header
    if (m_smuflGlyphs.size() > 0) {

        pugi::xml_node defs = m_svgNode.prepend_child("defs");
        pugi::xml_document sourceDoc;

        // for each needed glyph
        for (const auto &entry : m_smuflGlyphs) {
            const Glyph *glyph = entry.first;
            const SvgDeviceContext::GlyphRef &ref = entry.second;
            // load the XML as a pugi::xml_document
            sourceDoc.load_string(glyph->GetXML().c_str());

            // copy all the nodes inside into the master document
            for (pugi::xml_node child = sourceDoc.first_child(); child; child = child.next_sibling()) {
                child.attribute("id").set_value(ref.GetRefId().c_str());
                defs.append_copy(child);
            }
        }
    }

    unsigned int output_flags = pugi::format_default | pugi::format_no_declaration;
    if (xml_declaration) {
        // edit the xml declaration
        output_flags = pugi::format_default;
        pugi::xml_node decl = m_svgDoc.prepend_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";
        decl.append_attribute("standalone") = "no";
    }

    if (m_formatRaw) {
        output_flags |= pugi::format_raw;
    }

    // add description statement
    pugi::xml_node desc = m_svgNode.prepend_child("desc");
    desc.text().set(StringFormat("Engraved by Verovio %s", GetVersion().c_str()).c_str());

    // save the glyph data to m_outdata
    std::string indent = (m_indent == -1) ? "\t" : std::string(m_indent, ' ');
    m_svgDoc.save(m_outdata, indent.c_str(), output_flags);

    m_committed = true;
}

void SvgDeviceContext::StartGraphic(
    Object *object, const std::string &gClass, const std::string &gId, GraphicID graphicID, bool prepend)
{
    std::string gClassFull = gClass;

    if (object->HasAttClass(ATT_TYPED)) {
        AttTyped *att = dynamic_cast<AttTyped *>(object);
        assert(att);
        if (att->HasType()) {
            gClassFull.append((gClassFull.empty() ? "" : " ") + att->GetType());
        }
    }

    if (prepend) {
        m_currentNode = m_currentNode.prepend_child("g");
    }
    else {
        m_currentNode = m_currentNode.append_child("g");
    }
    m_svgNodeStack.push_back(m_currentNode);
    this->AppendIdAndClass(gId, object->GetClassName(), gClassFull, graphicID);
    this->AppendAdditionalAttributes(object);

    // Add data-plist with html5 (now only for annot)
    if (m_html5 && object->HasPlistReferences()) {
        auto plist = object->GetPlistReferences();
        std::string ids = ConcatenateIDs(*plist);
        this->SetCustomGraphicAttributes("plist-referring", ids);
    }

    // this sets staffDef styles for lyrics
    if (object->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(object);
        assert(staff);

        assert(staff->m_drawingStaffDef);

        std::string styleStr;
        if (staff->m_drawingStaffDef->HasLyricFam()) {
            styleStr.append("font-family:" + staff->m_drawingStaffDef->GetLyricFam() + ";");
        }
        if (staff->m_drawingStaffDef->HasLyricName()) {
            styleStr.append("font-family:" + staff->m_drawingStaffDef->GetLyricName() + ";");
        }
        if (staff->m_drawingStaffDef->HasLyricStyle()) {
            styleStr.append(
                "font-style:" + staff->AttTyped::FontstyleToStr(staff->m_drawingStaffDef->GetLyricStyle()) + ";");
        }
        if (staff->m_drawingStaffDef->HasLyricWeight()) {
            styleStr.append(
                "font-weight:" + staff->AttTyped::FontweightToStr(staff->m_drawingStaffDef->GetLyricWeight()) + ";");
        }
        if (!styleStr.empty()) m_currentNode.append_attribute("style") = styleStr.c_str();
    }

    if (object->HasAttClass(ATT_COLOR)) {
        AttColor *att = dynamic_cast<AttColor *>(object);
        assert(att);
        if (att->HasColor()) {
            m_currentNode.append_attribute("color") = att->GetColor().c_str();
            m_currentNode.append_attribute("fill") = att->GetColor().c_str();
        }
    }

    if (object->HasAttClass(ATT_LABELLED)) {
        AttLabelled *att = dynamic_cast<AttLabelled *>(object);
        assert(att);
        if (att->HasLabel()) {
            pugi::xml_node svgTitle = m_currentNode.prepend_child("title");
            svgTitle.append_attribute("class") = "labelAttr";
            svgTitle.text().set(att->GetLabel().c_str());
        }
    }

    if (object->HasAttClass(ATT_LANG)) {
        AttLang *att = dynamic_cast<AttLang *>(object);
        assert(att);
        if (att->HasLang()) {
            m_currentNode.append_attribute("xml:lang") = att->GetLang().c_str();
        }
    }

    if (object->HasAttClass(ATT_TYPOGRAPHY)) {
        AttTypography *att = dynamic_cast<AttTypography *>(object);
        assert(att);
        if (att->HasFontname()) m_currentNode.append_attribute("font-family") = att->GetFontname().c_str();
        if (att->HasFontstyle())
            m_currentNode.append_attribute("font-style") = att->FontstyleToStr(att->GetFontstyle()).c_str();
        if (att->HasFontweight())
            m_currentNode.append_attribute("font-weight") = att->FontweightToStr(att->GetFontweight()).c_str();
    }

    if (object->HasAttClass(ATT_VISIBILITY)) {
        AttVisibility *att = dynamic_cast<AttVisibility *>(object);
        assert(att);
        if (att->HasVisible()) {
            if (att->GetVisible() == BOOLEAN_true) {
                m_currentNode.append_attribute("visibility") = "visible";
            }
            else if (att->GetVisible() == BOOLEAN_false) {
                m_currentNode.append_attribute("visibility") = "hidden";
            }
        }
    }

    // m_currentNode.append_attribute("style") = StringFormat("stroke: #%s; stroke-opacity: %f; fill: #%s; fill-opacity:
    // %f;",
    // this->GetColor(currentPen.GetColor()).c_str(), currentPen.GetOpacity(),
    // this->GetColor(currentBrush.GetColor()).c_str(), currentBrush.GetOpacity()).c_str();
}

void SvgDeviceContext::StartCustomGraphic(const std::string &name, std::string gClass, std::string gId)
{
    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    this->AppendIdAndClass(gId, name, gClass);
}

void SvgDeviceContext::StartTextGraphic(Object *object, const std::string &gClass, const std::string &gId)
{
    m_currentNode = AddChild("tspan");
    m_svgNodeStack.push_back(m_currentNode);
    this->AppendIdAndClass(gId, object->GetClassName(), gClass);
    this->AppendAdditionalAttributes(object);

    if (object->HasAttClass(ATT_COLOR)) {
        AttColor *att = dynamic_cast<AttColor *>(object);
        assert(att);
        if (att->HasColor()) m_currentNode.append_attribute("fill") = att->GetColor().c_str();
    }

    if (object->HasAttClass(ATT_LABELLED)) {
        AttLabelled *att = dynamic_cast<AttLabelled *>(object);
        assert(att);
        if (att->HasLabel()) {
            pugi::xml_node svgTitle = m_currentNode.prepend_child("title");
            svgTitle.append_attribute("class") = "labelAttr";
            svgTitle.text().set(att->GetLabel().c_str());
        }
    }

    if (object->HasAttClass(ATT_LANG)) {
        AttLang *att = dynamic_cast<AttLang *>(object);
        assert(att);
        if (att->HasLang()) {
            m_currentNode.append_attribute("xml:lang") = att->GetLang().c_str();
        }
    }

    if (object->HasAttClass(ATT_TYPOGRAPHY)) {
        AttTypography *att = dynamic_cast<AttTypography *>(object);
        assert(att);
        if (att->HasFontname()) m_currentNode.append_attribute("font-family") = att->GetFontname().c_str();
        if (att->HasFontstyle())
            m_currentNode.append_attribute("font-style") = att->FontstyleToStr(att->GetFontstyle()).c_str();
        if (att->HasFontweight())
            m_currentNode.append_attribute("font-weight") = att->FontweightToStr(att->GetFontweight()).c_str();
    }

    if (object->HasAttClass(ATT_WHITESPACE)) {
        AttWhitespace *att = dynamic_cast<AttWhitespace *>(object);
        assert(att);
        if (att->HasSpace()) {
            m_currentNode.append_attribute("xml:space") = att->GetSpace().c_str();
            ;
        }
    }
}

void SvgDeviceContext::ResumeGraphic(Object *object, std::string gId)
{
    std::string xpathPrefix = m_html5 ? "//g[@data-id=\"" : "//g[@id=\"";
    std::string xpath = xpathPrefix + gId + "\"]";
    pugi::xpath_node selection = m_currentNode.select_node(xpath.c_str());
    if (selection) {
        m_currentNode = selection.node();
    }
    m_svgNodeStack.push_back(m_currentNode);
}

void SvgDeviceContext::EndGraphic(Object *object, View *view)
{
    this->DrawSvgBoundingBox(object, view);
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::EndCustomGraphic()
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::SetCustomGraphicColor(const std::string &color)
{
    m_currentNode.append_attribute("color") = color.c_str();
    m_currentNode.append_attribute("fill") = color.c_str();
}

void SvgDeviceContext::SetCustomGraphicAttributes(const std::string &data, const std::string &value)
{
    m_currentNode.append_attribute(("data-" + data).c_str()) = value.c_str();
}

void SvgDeviceContext::EndResumedGraphic(Object *object, View *view)
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::EndTextGraphic(Object *object, View *view)
{
    this->DrawSvgBoundingBox(object, view);
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::RotateGraphic(Point const &orig, double angle)
{
    if (m_currentNode.attribute("transform")) {
        return;
    }

    m_currentNode.append_attribute("transform") = StringFormat("rotate(%f %d,%d)", angle, orig.x, orig.y).c_str();
}

void SvgDeviceContext::StartPage()
{
    // Initialize the flag to false because we want to know if the font needs to be included in the SVG
    m_vrvTextFont = false;
    m_vrvTextFontFallback = false;

    // default styles
    if (this->UseGlobalStyling()) {
        m_currentNode = m_currentNode.append_child("style");
        m_currentNode.append_attribute("type") = "text/css";
        const Resources *resources = this->GetResources();
        assert(resources);
        std::string css = "g.page-margin{font-family:" + resources->GetTextFont()
            + ",serif;} "
              "g.ending, g.fing, g.reh, g.tempo{font-weight:bold;} g.dir, g.dynam, "
              "g.mNum{font-style:italic;} g.label{font-weight:normal;} path{stroke:currentColor}";
        // bounding box css - for debugging
        // css += " g.bounding-box{stroke:red; stroke-width:10} "
        //        "g.content-bounding-box{stroke:blue; stroke-width:10}";
        this->PrefixCssRules(css);
        m_currentNode.text().set(css);
        m_currentNode = m_svgNodeStack.back();
    }

    if (!m_css.empty()) {
        m_currentNode = m_currentNode.append_child("style");
        m_currentNode.append_attribute("type") = "text/css";
        m_currentNode.text().set(m_css);
        m_currentNode = m_svgNodeStack.back();
    }

    // a graphic for definition scaling
    m_currentNode = m_currentNode.append_child("svg");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = "definition-scale";
    m_currentNode.append_attribute("color") = "black";
    if (this->GetFacsimile()) {
        m_currentNode.append_attribute("viewBox")
            = StringFormat("0 0 %d %d", this->GetWidth(), this->GetHeight()).c_str();
    }
    else {
        m_currentNode.append_attribute("viewBox") = StringFormat("0 0 %d %d",
            int(this->GetWidth() * this->GetViewBoxFactor()), int(this->GetContentHeight() * this->GetViewBoxFactor()))
                                                        .c_str();
    }

    // page rectangle - for debugging
    // pugi::xml_node pageRect = m_currentNode.append_child("rect");
    // pageRect.append_attribute("fill") = "pink";
    // pageRect.append_attribute("height") = StringFormat("%d", this->GetHeight()* DEFINITION_FACTOR).c_str();
    // pageRect.append_attribute("width") = StringFormat("%d", this->GetWidth() * DEFINITION_FACTOR).c_str();

    // a graphic for the origin
    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = "page-margin";
    m_currentNode.append_attribute("transform")
        = StringFormat("translate(%d, %d)", (int)((double)m_originX), (int)((double)m_originY)).c_str();

    // margin rectangle - for debugging
    // pugi::xml_node marginRect = m_currentNode.append_child("rect");
    // marginRect.append_attribute("fill") = "yellow";
    // marginRect.append_attribute("height") = StringFormat("%d", this->GetHeight() * DEFINITION_FACTOR - 2 *
    // m_originY).c_str();
    // marginRect.append_attribute("width") = StringFormat("%d", this->GetWidth() * DEFINITION_FACTOR - 2
    //* m_originX).c_str();

    m_pageNode = m_currentNode;
}

void SvgDeviceContext::EndPage()
{
    // end page-margin
    m_svgNodeStack.pop_back();
    // end definition-scale
    m_svgNodeStack.pop_back();
    // end page-scale
    // m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::SetBackground(int color, int style)
{
    // nothing to do, we do not handle Background
}

void SvgDeviceContext::SetBackgroundImage(void *image, double opacity) {}

void SvgDeviceContext::SetBackgroundMode(int mode)
{
    // nothing to do, we do not handle Background Mode
}

void SvgDeviceContext::SetTextForeground(int color)
{
    m_brushStack.top().SetColor(color); // we use the brush color for text
}

void SvgDeviceContext::SetTextBackground(int color)
{
    // nothing to do, we do not handle Text Background Mode
}

void SvgDeviceContext::SetLogicalOrigin(int x, int y)
{
    m_originX = -x;
    m_originY = -y;
}

Point SvgDeviceContext::GetLogicalOrigin()
{
    return Point(m_originX, m_originY);
}

pugi::xml_node SvgDeviceContext::AddChild(std::string name)
{
    pugi::xml_node g = m_currentNode.child("g");
    if (g) {
        return m_currentNode.insert_child_before(name.c_str(), g);
    }
    else {
        return (m_pushBack) ? m_currentNode.prepend_child(name.c_str()) : m_currentNode.append_child(name.c_str());
    }
}

void SvgDeviceContext::AppendStrokeLineCap(pugi::xml_node node, const Pen &pen)
{
    switch (pen.GetLineCap()) {
        case LINECAP_BUTT: node.append_attribute("stroke-linecap") = "butt"; break;
        case LINECAP_ROUND: node.append_attribute("stroke-linecap") = "round"; break;
        case LINECAP_SQUARE: node.append_attribute("stroke-linecap") = "square"; break;
        default: break;
    }
}

void SvgDeviceContext::AppendStrokeLineJoin(pugi::xml_node node, const Pen &pen)
{
    switch (pen.GetLineJoin()) {
        case LINEJOIN_ARCS: node.append_attribute("stroke-linejoin") = "arcs"; break;
        case LINEJOIN_BEVEL: node.append_attribute("stroke-linejoin") = "bevel"; break;
        case LINEJOIN_MITER: node.append_attribute("stroke-linejoin") = "miter"; break;
        case LINEJOIN_MITER_CLIP: node.append_attribute("stroke-linejoin") = "miter-clip"; break;
        case LINEJOIN_ROUND: node.append_attribute("stroke-linejoin") = "round"; break;
        default: break;
    }
}

void SvgDeviceContext::AppendStrokeDashArray(pugi::xml_node node, const Pen &pen)
{
    if (pen.GetDashLength() > 0) {
        const int dashLength = pen.GetDashLength();
        const int gapLength = (pen.GetGapLength() > 0) ? pen.GetGapLength() : dashLength;
        node.append_attribute("stroke-dasharray") = StringFormat("%d %d", dashLength, gapLength).c_str();
    }
}

void SvgDeviceContext::PrefixCssRules(std::string &rules)
{
    static std::regex selectorRegex(R"(([^{}]+)\s*\{([^}]*)\})");

    std::sregex_iterator it(rules.begin(), rules.end(), selectorRegex);
    std::sregex_iterator end;

    std::string result;

    while (it != end) {
        std::string selectors = (*it)[1].str();
        std::string properties = (*it)[2].str();

        // Split by comma to handle multi-selectors
        std::stringstream ss(selectors);
        std::string selector;
        std::vector<std::string> prefixedSelectors;

        while (std::getline(ss, selector, ',')) {
            // Trim whitespace
            selector = std::regex_replace(selector, std::regex(R"(^\s+|\s+$)"), "");
            prefixedSelectors.push_back("#" + m_docId + " " + selector);
        }

        if (prefixedSelectors.empty()) continue;

        std::string finalSelector = std::accumulate(std::next(prefixedSelectors.begin()), prefixedSelectors.end(),
            prefixedSelectors.at(0), [](std::string a, std::string b) { return a + ", " + b; });

        result += finalSelector + " {" + properties + "}";
        ++it;
    }

    rules = result;
}

// Drawing methods
void SvgDeviceContext::DrawQuadBezierPath(Point bezier[3])
{
    pugi::xml_node pathChild = AddChild("path");
    pathChild.append_attribute("d") = StringFormat("M%d,%d Q%d,%d %d,%d", // Base string
        bezier[0].x, bezier[0].y, // M Command
        bezier[1].x, bezier[1].y, bezier[2].x, bezier[2].y)
                                          .c_str();
    pathChild.append_attribute("fill") = "none";
    if (m_penStack.top().HasColor()) {
        pathChild.append_attribute("stroke") = this->GetColor(m_penStack.top().GetColor()).c_str();
    }
    pathChild.append_attribute("stroke-linecap") = "round";
    pathChild.append_attribute("stroke-linejoin") = "round";
    pathChild.append_attribute("stroke-width") = m_penStack.top().GetWidth();
    this->AppendStrokeDashArray(pathChild, m_penStack.top());
}

void SvgDeviceContext::DrawCubicBezierPath(Point bezier[4])
{
    pugi::xml_node pathChild = AddChild("path");
    pathChild.append_attribute("d") = StringFormat("M%d,%d C%d,%d %d,%d %d,%d", // Base string
        bezier[0].x, bezier[0].y, // M Command
        bezier[1].x, bezier[1].y, bezier[2].x, bezier[2].y, bezier[3].x, bezier[3].y // Remaining bezier points.
        )
                                          .c_str();
    pathChild.append_attribute("fill") = "none";
    if (m_penStack.top().HasColor()) {
        pathChild.append_attribute("stroke") = this->GetColor(m_penStack.top().GetColor()).c_str();
    }
    pathChild.append_attribute("stroke-linecap") = "round";
    pathChild.append_attribute("stroke-linejoin") = "round";
    pathChild.append_attribute("stroke-width") = m_penStack.top().GetWidth();
    this->AppendStrokeDashArray(pathChild, m_penStack.top());
}

void SvgDeviceContext::DrawCubicBezierPathFilled(Point bezier1[4], Point bezier2[4])
{
    pugi::xml_node pathChild = AddChild("path");
    pathChild.append_attribute("d")
        = StringFormat("M%d,%d C%d,%d %d,%d %d,%d C%d,%d %d,%d %d,%d", bezier1[0].x, bezier1[0].y, // M command
            bezier1[1].x, bezier1[1].y, bezier1[2].x, bezier1[2].y, bezier1[3].x, bezier1[3].y, // First bezier
            bezier2[2].x, bezier2[2].y, bezier2[1].x, bezier2[1].y, bezier2[0].x, bezier2[0].y // Second Bezier
            )
              .c_str();
    // pathChild.append_attribute("fill") = "currentColor";
    // pathChild.append_attribute("fill-opacity") = "1";
    if (m_penStack.top().HasColor()) {
        pathChild.append_attribute("stroke") = this->GetColor(m_penStack.top().GetColor()).c_str();
    }
    pathChild.append_attribute("stroke-linecap") = "round";
    pathChild.append_attribute("stroke-linejoin") = "round";
    // pathChild.append_attribute("stroke-opacity") = "1";
    pathChild.append_attribute("stroke-width") = m_penStack.top().GetWidth();
}

void SvgDeviceContext::DrawBentParallelogramFilled(Point side[4], int height)
{
    pugi::xml_node pathChild = AddChild("path");
    pathChild.append_attribute("d") = StringFormat("M%d,%d C%d,%d %d,%d %d,%d L%d,%d C%d,%d %d,%d %d,%d Z", side[0].x,
        side[0].y, side[1].x, side[1].y, side[2].x, side[2].y, side[3].x, side[3].y, side[3].x, side[3].y + height,
        side[2].x, side[2].y + height, side[1].x, side[1].y + height, side[0].x, side[0].y + height)
                                          .c_str();
    pathChild.append_attribute("stroke") = this->GetColor(m_penStack.top().GetColor()).c_str();
    pathChild.append_attribute("stroke-linecap") = "round";
    pathChild.append_attribute("stroke-linejoin") = "round";
    pathChild.append_attribute("stroke-width") = m_penStack.top().GetWidth();
}

void SvgDeviceContext::DrawCircle(int x, int y, int radius)
{
    this->DrawEllipse(x - radius, y - radius, 2 * radius, 2 * radius);
}

void SvgDeviceContext::DrawEllipse(int x, int y, int width, int height)
{
    assert(m_penStack.size());
    assert(m_brushStack.size());

    Pen currentPen = m_penStack.top();
    Brush currentBrush = m_brushStack.top();

    int rh = height / 2;
    int rw = width / 2;

    pugi::xml_node ellipseChild = AddChild("ellipse");
    ellipseChild.append_attribute("cx") = x + rw;
    ellipseChild.append_attribute("cy") = y + rh;
    ellipseChild.append_attribute("rx") = rw;
    ellipseChild.append_attribute("ry") = rh;
    if (currentBrush.HasOpacity()) {
        ellipseChild.append_attribute("fill-opacity") = currentBrush.GetOpacity();
    }
    if (currentPen.HasOpacity()) {
        ellipseChild.append_attribute("stroke-opacity") = currentPen.GetOpacity();
    }
    if (currentPen.GetWidth() > 0) {
        ellipseChild.append_attribute("stroke-width") = currentPen.GetWidth();
    }
    if (currentPen.HasColor()) {
        ellipseChild.append_attribute("stroke") = this->GetColor(currentPen.GetColor()).c_str();
    }
}

void SvgDeviceContext::DrawEllipticArc(int x, int y, int width, int height, double start, double end)
{
    /*
    Draws an arc of an ellipse. The current pen is used for drawing the arc
    and the current brush is used for drawing the pie. This function is
    currently only available for X window and PostScript device contexts.

    x and y specify the x and y coordinates of the upper-left corner of the
    rectangle that contains the ellipse.

    width and height specify the width and height of the rectangle that
    contains the ellipse.

    start and end specify the start and end of the arc relative to the
    three-o'clock position from the center of the rectangle. Angles are
    specified in degrees (360 is a complete circle). Positive values mean
    counter-clockwise motion. If start is equal to end, a complete ellipse
    will be drawn. */

    // known bug: SVG draws with the current pen along the radii, but this does not happen in wxMSW

    assert(m_penStack.size());
    assert(m_brushStack.size());

    Pen currentPen = m_penStack.top();
    Brush currentBrush = m_brushStack.top();

    std::string s;
    // radius
    double rx = width / 2;
    double ry = height / 2;
    // center
    double xc = x + rx;
    double yc = y + ry;

    double xs, ys, xe, ye;
    xs = xc + rx * cos(DegToRad(start));
    xe = xc + rx * cos(DegToRad(end));
    ys = yc - ry * sin(DegToRad(start));
    ye = yc - ry * sin(DegToRad(end));

    /// now same as circle arc...
    double theta1 = atan2(ys - yc, xs - xc);
    double theta2 = atan2(ye - yc, xe - xc);

    // flag for large or small arc 0 means less than 180 degrees
    int fArc = ((theta2 - theta1) > 0) ? 1 : 0;

    int fSweep = (fabs(theta2 - theta1) > M_PI) ? 1 : 0;

    pugi::xml_node pathChild = AddChild("path");
    pathChild.append_attribute("d") = StringFormat(
        "M%d %d A%d %d 0.0 %d %d %d %d", int(xs), int(ys), abs(int(rx)), abs(int(ry)), fArc, fSweep, int(xe), int(ye))
                                          .c_str();
    // pathChild.append_attribute("fill") = "currentColor";
    if (currentBrush.HasOpacity()) {
        pathChild.append_attribute("fill-opacity") = currentBrush.GetOpacity();
    }
    if (currentPen.HasOpacity()) {
        pathChild.append_attribute("stroke-opacity") = currentPen.GetOpacity();
    }
    if (currentPen.GetWidth() > 0) {
        pathChild.append_attribute("stroke-width") = currentPen.GetWidth();
    }
    if (currentPen.HasColor()) {
        pathChild.append_attribute("stroke") = this->GetColor(currentPen.GetColor()).c_str();
    }
}

void SvgDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    pugi::xml_node pathChild = AddChild("path");
    pathChild.append_attribute("d") = StringFormat("M%d %d L%d %d", x1, y1, x2, y2).c_str();
    if (m_penStack.top().HasColor() || !this->UseGlobalStyling()) {
        pathChild.append_attribute("stroke") = this->GetColor(m_penStack.top().GetColor()).c_str();
    }
    if (m_penStack.top().GetWidth() > 1) pathChild.append_attribute("stroke-width") = m_penStack.top().GetWidth();
    this->AppendStrokeLineCap(pathChild, m_penStack.top());
    this->AppendStrokeDashArray(pathChild, m_penStack.top());
}

void SvgDeviceContext::DrawPolyline(int n, Point points[], bool close)
{
    assert(m_penStack.size());
    const Pen &currentPen = m_penStack.top();

    pugi::xml_node polylineChild = (close) ? AddChild("polygon") : AddChild("polyline");

    if (currentPen.GetWidth() > 0) {
        polylineChild.append_attribute("stroke") = this->GetColor(currentPen.GetColor()).c_str();
    }
    if (currentPen.GetWidth() > 1) {
        polylineChild.append_attribute("stroke-width") = currentPen.GetWidth();
    }
    if (currentPen.HasOpacity()) {
        polylineChild.append_attribute("stroke-opacity") = currentPen.GetOpacity();
    }

    this->AppendStrokeLineCap(polylineChild, currentPen);
    this->AppendStrokeLineJoin(polylineChild, currentPen);
    this->AppendStrokeDashArray(polylineChild, currentPen);

    if (n > 2) polylineChild.append_attribute("fill") = "none";

    std::string pointsString;
    for (int i = 0; i < n; ++i) {
        pointsString += StringFormat("%d,%d ", points[i].x, points[i].y);
    }
    polylineChild.append_attribute("points") = pointsString.c_str();
}

void SvgDeviceContext::DrawPolygon(int n, Point points[])
{
    assert(m_penStack.size());
    assert(m_brushStack.size());

    const Pen &currentPen = m_penStack.top();
    const Brush &currentBrush = m_brushStack.top();

    pugi::xml_node polygonChild = AddChild("polygon");

    if (currentPen.GetWidth() > 0) {
        polygonChild.append_attribute("stroke") = this->GetColor(currentPen.GetColor()).c_str();
    }
    if (currentPen.GetWidth() > 1) {
        polygonChild.append_attribute("stroke-width") = currentPen.GetWidth();
    }
    if (currentPen.HasOpacity()) {
        polygonChild.append_attribute("stroke-opacity") = currentPen.GetOpacity();
    }

    this->AppendStrokeLineJoin(polygonChild, currentPen);
    this->AppendStrokeDashArray(polygonChild, currentPen);

    if (currentBrush.HasColor()) {
        polygonChild.append_attribute("fill") = this->GetColor(currentBrush.GetColor()).c_str();
    }
    if (currentBrush.HasOpacity()) {
        polygonChild.append_attribute("fill-opacity") = currentBrush.GetOpacity();
    }

    std::string pointsString = StringFormat("%d,%d", points[0].x, points[0].y);
    for (int i = 1; i < n; ++i) {
        pointsString += " " + StringFormat("%d,%d", points[i].x, points[i].y);
    }
    polygonChild.append_attribute("points") = pointsString.c_str();
}

void SvgDeviceContext::DrawRectangle(int x, int y, int width, int height)
{
    this->DrawRoundedRectangle(x, y, width, height, 0);
}

void SvgDeviceContext::DrawRoundedRectangle(int x, int y, int width, int height, int radius)
{
    pugi::xml_node rectChild = AddChild("rect");

    if (m_penStack.size()) {
        Pen currentPen = m_penStack.top();
        if (currentPen.GetWidth() > 0) {
            rectChild.append_attribute("stroke") = this->GetColor(currentPen.GetColor()).c_str();
        }
        if (currentPen.GetWidth() > 1) {
            rectChild.append_attribute("stroke-width") = currentPen.GetWidth();
        }
        if (currentPen.HasOpacity()) {
            rectChild.append_attribute("stroke-opacity") = currentPen.GetOpacity();
        }
    }

    if (m_brushStack.size()) {
        Brush currentBrush = m_brushStack.top();
        if (currentBrush.HasColor()) {
            rectChild.append_attribute("fill") = this->GetColor(currentBrush.GetColor()).c_str();
        }
        if (currentBrush.HasOpacity()) {
            rectChild.append_attribute("fill-opacity") = currentBrush.GetOpacity();
        }
    }

    // negative heights or widths are not allowed in SVG
    if (height < 0) {
        height = -height;
        y -= height;
    }
    if (width < 0) {
        width = -width;
        x -= width;
    }

    rectChild.append_attribute("x") = x;
    rectChild.append_attribute("y") = y;
    rectChild.append_attribute("height") = height;
    rectChild.append_attribute("width") = width;
    if (radius != 0) rectChild.append_attribute("rx") = radius;
}

void SvgDeviceContext::StartText(int x, int y, data_HORIZONTALALIGNMENT alignment)
{
    std::string s;
    std::string anchor;

    if (alignment == HORIZONTALALIGNMENT_right) {
        anchor = "end";
    }
    if (alignment == HORIZONTALALIGNMENT_center) {
        anchor = "middle";
    }

    m_currentNode = m_currentNode.append_child("text");
    m_svgNodeStack.push_back(m_currentNode);
    if (x) m_currentNode.append_attribute("x") = x;
    if (y) m_currentNode.append_attribute("y") = y;
    // unless dx, dy have a value they don't need to be set
    // m_currentNode.append_attribute("dx") = 0;
    // m_currentNode.append_attribute("dy") = 0;
    if (!anchor.empty()) {
        m_currentNode.append_attribute("text-anchor") = anchor.c_str();
    }
    // font-size seems to be required in <text> in FireFox and also we set it to 0px so space
    // is not added between tspan elements
    m_currentNode.append_attribute("font-size") = "0px";
    //
    if (!m_fontStack.top()->GetFaceName().empty()) {
        m_currentNode.append_attribute("font-family") = m_fontStack.top()->GetFaceName().c_str();
    }
    if (m_fontStack.top()->GetStyle() != FONTSTYLE_NONE) {
        if (m_fontStack.top()->GetStyle() == FONTSTYLE_italic) {
            m_currentNode.append_attribute("font-style") = "italic";
        }
        else if (m_fontStack.top()->GetStyle() == FONTSTYLE_normal) {
            m_currentNode.append_attribute("font-style") = "normal";
        }
        else if (m_fontStack.top()->GetStyle() == FONTSTYLE_oblique) {
            m_currentNode.append_attribute("font-style") = "oblique";
        }
    }
    if (m_fontStack.top()->GetWeight() != FONTWEIGHT_NONE) {
        if (m_fontStack.top()->GetWeight() == FONTWEIGHT_bold) {
            m_currentNode.append_attribute("font-weight") = "bold";
        }
    }
}

void SvgDeviceContext::MoveTextTo(int x, int y, data_HORIZONTALALIGNMENT alignment)
{
    m_currentNode.append_attribute("x") = x;
    m_currentNode.append_attribute("y") = y;
    if (alignment != HORIZONTALALIGNMENT_NONE) {
        std::string anchor = "start";
        if (alignment == HORIZONTALALIGNMENT_right) {
            anchor = "end";
        }
        if (alignment == HORIZONTALALIGNMENT_center) {
            anchor = "middle";
        }
        m_currentNode.append_attribute("text-anchor") = anchor.c_str();
    }
}

void SvgDeviceContext::MoveTextVerticallyTo(int y)
{
    m_currentNode.append_attribute("y") = y;
}

void SvgDeviceContext::EndText()
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

// draw text element with optional parameters to specify the bounding box of the text
// if the bounding box is specified then append a rect child
void SvgDeviceContext::DrawText(
    const std::string &text, const std::u32string &wtext, int x, int y, int width, int height)
{
    assert(m_fontStack.top());

    std::string svgText = text;

    // Because IE does not support xml:space="preserve", we need to replace the initial
    // space with a non breakable space
    if ((svgText.length() > 0) && (svgText[0] == ' ')) {
        svgText.replace(0, 1, "\xC2\xA0");
    }
    if ((svgText.length() > 0) && (svgText[svgText.size() - 1] == ' ')) {
        svgText.replace(svgText.size() - 1, 1, "\xC2\xA0");
    }

    pugi::xpath_node fontNode = m_currentNode.select_node("ancestor::*[@font-family][1]");
    std::string currentFaceName = (fontNode) ? fontNode.node().attribute("font-family").value() : "";
    std::string fontFaceName = m_fontStack.top()->GetFaceName();

    pugi::xml_node textChild = AddChild("tspan");
    // We still add @xml:space (No: this seems to create problems with Safari)
    // textChild.append_attribute("xml:space") = "preserve";
    // Set the @font-family only if it is not the same as in the parent node
    if (!fontFaceName.empty() && (fontFaceName != currentFaceName)) {
        // Special case where we want to specifiy if the woff2 font needs to be included in the output
        if (m_fontStack.top()->GetSmuflFont() != SMUFL_NONE) {
            if (m_fontStack.top()->GetSmuflFont() == SMUFL_FONT_FALLBACK) {
                this->VrvTextFontFallback();
                textChild.append_attribute("font-family") = "Leipzig";
            }
            else {
                this->VrvTextFont();
                textChild.append_attribute("font-family") = m_fontStack.top()->GetFaceName().c_str();
            }
            if (m_fontStack.top()->GetStyle() == FONTSTYLE_normal) {
                textChild.append_attribute("font-style") = "normal";
            }
        }
        else {
            textChild.append_attribute("font-family") = m_fontStack.top()->GetFaceName().c_str();
        }
    }
    if (m_fontStack.top()->GetPointSize() != 0) {
        textChild.append_attribute("font-size") = StringFormat("%dpx", m_fontStack.top()->GetPointSize()).c_str();
    }
    if (m_fontStack.top()->GetLetterSpacing() != 0.0) {
        textChild.append_attribute("letter-spacing")
            = StringFormat("%dpx", m_fontStack.top()->GetLetterSpacing()).c_str();
    }
    textChild.text().set(svgText.c_str());

    if ((x != 0) && (y != 0) && (x != VRV_UNSET) && (y != VRV_UNSET) && (width != 0) && (height != 0)
        && (width != VRV_UNSET) && (height != VRV_UNSET)) {
        pugi::xml_node g = m_currentNode.parent().parent();
        pugi::xml_node rectChild = g.append_child("rect");
        rectChild.append_attribute("class") = "sylTextRect";
        rectChild.append_attribute("x") = StringFormat("%d", x).c_str();
        rectChild.append_attribute("y") = StringFormat("%d", y).c_str();
        rectChild.append_attribute("width") = StringFormat("%d", width).c_str();
        rectChild.append_attribute("height") = StringFormat("%d", height).c_str();
        rectChild.append_attribute("opacity") = "0.0";
    }
    else if ((x != 0) && (y != 0) && (x != VRV_UNSET) && (y != VRV_UNSET)) {
        textChild.append_attribute("x") = StringFormat("%d", x).c_str();
        textChild.append_attribute("y") = StringFormat("%d", y).c_str();
    }
}

void SvgDeviceContext::DrawRotatedText(const std::string &text, int x, int y, double angle)
{
    // TODO
}

void SvgDeviceContext::DrawMusicText(const std::u32string &text, int x, int y, bool setSmuflGlyph)
{
    assert(m_fontStack.top());

    const Resources *resources = this->GetResources();
    assert(resources);

    int w, h, gx, gy;

    // remove the `xlink:` prefix for backwards compatibility with older SVG viewers.
    std::string hrefAttrib = "href";
    if (!m_removeXlink) {
        hrefAttrib.insert(0, "xlink:");
    }

    // print chars one by one
    for (char32_t c : text) {
        const Glyph *glyph = resources->GetGlyph(c);
        if (!glyph) {
            continue;
        }

        // Add the glyph to the array for the <defs>
        const std::string id = InsertGlyphRef(glyph);

        // Write the char in the SVG
        pugi::xml_node useChild = AddChild("use");
        useChild.append_attribute(hrefAttrib.c_str()) = StringFormat("#%s", id.c_str()).c_str();
        double scaleX = (double)m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm() * DEFINITION_FACTOR;
        double scaleY = scaleX;
        if (m_fontStack.top()->GetWidthToHeightRatio() != 1.0f) scaleX *= m_fontStack.top()->GetWidthToHeightRatio();
        useChild.append_attribute("transform") = StringFormat("translate(%d, %d) scale(%g, %g)", x, y, scaleX, scaleY);

        // Get the bounds of the char
        if (glyph->GetHorizAdvX() > 0)
            x += glyph->GetHorizAdvX() * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
        else {
            glyph->GetBoundingBox(gx, gy, w, h);
            x += w * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
        }
    }
}

void SvgDeviceContext::DrawSpline(int n, Point points[]) {}

void SvgDeviceContext::DrawGraphicUri(int x, int y, int width, int height, const std::string &uri)
{
    pugi::xml_node image = m_currentNode.append_child("image");
    image.append_attribute("xlink:href") = uri.c_str();
    image.append_attribute("x") = StringFormat("%d", x).c_str();
    image.append_attribute("y") = StringFormat("%d", y).c_str();
    image.append_attribute("width") = StringFormat("%d", width).c_str();
    image.append_attribute("height") = StringFormat("%d", height).c_str();
}

void SvgDeviceContext::DrawSvgShape(int x, int y, int width, int height, double scale, pugi::xml_node svg)
{
    m_currentNode.append_attribute("transform")
        = StringFormat("translate(%d, %d) scale(%f, %f)", x, y, scale * DEFINITION_FACTOR, scale * DEFINITION_FACTOR)
              .c_str();

    // Remove the ID in the SVG because it might be duplicated and that will not be valid
    m_currentNode.remove_attribute("id");

    for (pugi::xml_node child : svg.children()) {
        m_currentNode.append_copy(child);
    }
}

void SvgDeviceContext::DrawBackgroundImage(int x, int y) {}

void SvgDeviceContext::AddDescription(const std::string &text)
{
    pugi::xml_node desc = m_currentNode.append_child("desc");
    desc.text().set(text.c_str());
}

void SvgDeviceContext::AppendIdAndClass(
    const std::string &gId, const std::string &baseClass, const std::string &addedClasses, GraphicID graphicID)
{
    std::string baseClassFull = baseClass;

    if (gId.length() > 0) {
        if (m_html5) {
            m_currentNode.append_attribute("data-id") = gId.c_str();
        }
        else if (graphicID == PRIMARY) {
            // Don't write ids for HTML5 to avoid id clashes when embedding into
            // an HTML document.
            m_currentNode.append_attribute("id") = gId.c_str();
        }
    }

    if (m_html5) {
        m_currentNode.append_attribute("data-class") = baseClassFull.c_str();
    }

    if (graphicID != PRIMARY) {
        std::string addClass = (graphicID == SPANNING) ? " spanning" : " symbol-ref";
        baseClassFull.append(" id-" + gId + addClass);
    }
    if (!addedClasses.empty()) {
        baseClassFull.append(" " + addedClasses);
    }
    m_currentNode.append_attribute("class") = baseClassFull.c_str();
}

void SvgDeviceContext::AppendAdditionalAttributes(Object *object)
{
    std::pair<std::multimap<ClassId, std::string>::iterator, std::multimap<ClassId, std::string>::iterator> range;
    range = m_svgAdditionalAttributes.equal_range(object->GetClassId()); // if correct class name...
    for (std::multimap<ClassId, std::string>::iterator it = range.first; it != range.second; ++it) {
        ArrayOfStrAttr attributes;
        object->GetAttributes(&attributes);
        for (ArrayOfStrAttr::iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
            if (it->second == (*iter).first) // ...and attribute exists in class name, add it to SVG element
                m_currentNode.append_attribute(("data-" + it->second).c_str()) = (*iter).second.c_str();
        }
    }
}

std::string SvgDeviceContext::GetColor(int color) const
{
    switch (color) {
        case (COLOR_NONE): return "currentColor";
        case (COLOR_BLACK): return "#000000";
        case (COLOR_WHITE): return "#FFFFFF";
        case (COLOR_RED): return "#FF0000";
        case (COLOR_GREEN): return "#00FF00";
        case (COLOR_BLUE): return "#0000FF";
        case (COLOR_CYAN): return "#00FFFF";
        case (COLOR_LIGHT_GREY): return "#777777";
        default: return StringFormat("#%06X", color);
    }
}

std::string SvgDeviceContext::GetStringSVG(bool xml_declaration)
{
    if (!m_committed) Commit(xml_declaration);

    return m_outdata.str();
}

void SvgDeviceContext::DrawSvgBoundingBoxRectangle(int x, int y, int width, int height)
{
    std::string s;

    // negative heights or widths are not allowed in SVG
    if (height < 0) {
        height = -height;
        y -= height;
    }
    if (width < 0) {
        width = -width;
        x -= width;
    }

    pugi::xml_node rectChild = AddChild("rect");
    rectChild.append_attribute("x") = x;
    rectChild.append_attribute("y") = y;
    rectChild.append_attribute("height") = height;
    rectChild.append_attribute("width") = width;

    rectChild.append_attribute("fill") = "transparent";
}

void SvgDeviceContext::DrawSvgBoundingBox(Object *object, View *view)
{
    const Resources *resources = this->GetResources();
    assert(resources);

    bool groupInPage = false;
    bool drawAnchors = false;
    bool drawContentBB = false;

    if (m_svgBoundingBoxes && view) {
        BoundingBox *box = object;
        // For floating elements, get the current bounding box set by System::SetCurrentFloatingPositioner
        if (object->IsFloatingObject()) {
            FloatingObject *floatingObject = vrv_cast<FloatingObject *>(object);
            assert(floatingObject);
            box = floatingObject->GetCurrentFloatingPositioner();
            // No bounding box found, ignore the object - this happens when the @staff is missing because the element is
            // never drawn but there is still a EndGraphic call.
            if (!box) return;
        }

        pugi::xml_node currentNode = m_currentNode;
        if (groupInPage) {
            m_currentNode = m_pageNode;
        }

        this->StartGraphic(object, "bounding-box", "bbox-" + object->GetID(), PRIMARY, true);

        if (box->HasSelfBB()) {
            this->DrawSvgBoundingBoxRectangle(view->ToDeviceContextX(object->GetDrawingX() + box->GetSelfX1()),
                view->ToDeviceContextY(object->GetDrawingY() + box->GetSelfY1()),
                view->ToDeviceContextX(object->GetDrawingX() + box->GetSelfX2())
                    - view->ToDeviceContextX(object->GetDrawingX() + box->GetSelfX1()),
                view->ToDeviceContextY(object->GetDrawingY() + box->GetSelfY2())
                    - view->ToDeviceContextY(object->GetDrawingY() + box->GetSelfY1()));
        }

        if (drawAnchors) {
            std::vector<SMuFLGlyphAnchor> anchors = { SMUFL_cutOutNE, SMUFL_cutOutNW, SMUFL_cutOutSE, SMUFL_cutOutSW };
            std::vector<SMuFLGlyphAnchor>::iterator iter;

            for (iter = anchors.begin(); iter != anchors.end(); ++iter) {
                if (object->GetBoundingBoxGlyph() != 0) {
                    const Glyph *glyph = resources->GetGlyph(object->GetBoundingBoxGlyph());
                    assert(glyph);

                    if (glyph->HasAnchor(*iter)) {
                        const Point *fontPoint = glyph->GetAnchor(*iter);
                        assert(fontPoint);
                        Point p;
                        int x, y, w, h;
                        glyph->GetBoundingBox(x, y, w, h);
                        int smuflGlyphFontSize = object->GetBoundingBoxGlyphFontSize();

                        p.x = object->GetSelfLeft() - x * smuflGlyphFontSize / glyph->GetUnitsPerEm();
                        p.x += (fontPoint->x * smuflGlyphFontSize / glyph->GetUnitsPerEm());
                        p.y = object->GetSelfBottom() - y * smuflGlyphFontSize / glyph->GetUnitsPerEm();
                        p.y += (fontPoint->y * smuflGlyphFontSize / glyph->GetUnitsPerEm());

                        this->SetPen(10, PEN_SOLID, 0, 0, LINECAP_DEFAULT, LINEJOIN_DEFAULT, 1.0, COLOR_GREEN);
                        this->SetBrush(1.0, COLOR_GREEN);
                        this->DrawCircle(view->ToDeviceContextX(p.x), view->ToDeviceContextY(p.y), 5);
                        this->ResetBrush();
                        this->ResetPen();
                    }
                }
            }
        }

        this->EndGraphic(object, NULL);

        if (groupInPage) {
            m_currentNode = m_pageNode;
        }

        if (drawContentBB) {
            if (object->HasContentBB()) {
                StartGraphic(object, "content-bounding-box", "cbbox-" + object->GetID(), PRIMARY, true);
                if (object->HasContentBB()) {
                    this->DrawSvgBoundingBoxRectangle(
                        view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX1()),
                        view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY1()),
                        view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX2())
                            - view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX1()),
                        view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY2())
                            - view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY1()));
                }
                this->EndGraphic(object, NULL);
            }
        }

        if (groupInPage) {
            m_currentNode = currentNode;
        }
    }
}

} // namespace vrv
