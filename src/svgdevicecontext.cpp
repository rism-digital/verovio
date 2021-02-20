/////////////////////////////////////////////////////////////////////////////
// Name:        svgdevicecontext.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "svgdevicecontext.h"

//----------------------------------------------------------------------------

#include <assert.h>

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

SvgDeviceContext::SvgDeviceContext() : DeviceContext()
{
    m_originX = 0;
    m_originY = 0;

    SetBrush(AxNONE, AxSOLID);
    SetPen(AxNONE, 1, AxSOLID);

    m_smuflGlyphs.clear();

    m_committed = false;
    m_vrvTextFont = false;

    m_mmOutput = false;
    m_svgBoundingBoxes = false;
    m_svgViewBox = false;
    m_html5 = false;
    m_facsimile = false;
    m_indent = 2;

    // create the initial SVG element
    // width and height need to be set later; these are taken care of in "commit"
    m_svgNode = m_svgDoc.append_child("svg");
    m_svgNode.append_attribute("version") = "1.1";
    m_svgNode.append_attribute("xmlns") = "http://www.w3.org/2000/svg";
    m_svgNode.append_attribute("xmlns:xlink") = "http://www.w3.org/1999/xlink";
    m_svgNode.append_attribute("xmlns:mei") = "http://www.music-encoding.org/ns/mei";
    m_svgNode.append_attribute("overflow") = "visible";

    // start the stack
    m_svgNodeStack.push_back(m_svgNode);
    m_currentNode = m_svgNode;

    m_outdata.clear();
}

SvgDeviceContext::~SvgDeviceContext() {}

bool SvgDeviceContext::CopyFileToStream(const std::string &filename, std::ostream &dest)
{
    std::ifstream source(filename.c_str(), std::ios::binary);
    dest << source.rdbuf();
    source.close();
    return true;
}

void SvgDeviceContext::Commit(bool xml_declaration)
{

    if (m_committed) {
        return;
    }

    // take care of width/height once userScale is updated
    double height = (double)GetHeight() * GetUserScaleY();
    double width = (double)GetWidth() * GetUserScaleX();
    const char *format = "%gpx";

    if (m_mmOutput) {
        height /= 10;
        width /= 10;
        format = "%gmm";
    }

    if (m_svgViewBox) {
        m_svgNode.prepend_attribute("viewBox") = StringFormat("0 0 %g %g", width, height).c_str();
    }
    else {
        m_svgNode.prepend_attribute("height") = StringFormat(format, height).c_str();
        m_svgNode.prepend_attribute("width") = StringFormat(format, width).c_str();
    }

    // add the woff VerovioText font if needed
    if (m_vrvTextFont) {
        std::string woff = Resources::GetPath() + "/woff.xml";
        pugi::xml_document woffDoc;
        woffDoc.load_file(woff.c_str());
        m_svgNode.prepend_copy(woffDoc.first_child());
    }

    // header
    if (m_smuflGlyphs.size() > 0) {

        pugi::xml_node defs = m_svgNode.prepend_child("defs");
        pugi::xml_document sourceDoc;

        // for each needed glyph
        std::set<std::string>::const_iterator it;
        for (it = m_smuflGlyphs.begin(); it != m_smuflGlyphs.end(); ++it) {
            // load the XML file that contains it as a pugi::xml_document
            std::ifstream source((*it).c_str());
            sourceDoc.load(source);

            // copy all the nodes inside into the master document
            for (pugi::xml_node child = sourceDoc.first_child(); child; child = child.next_sibling()) {
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

    // add description statement
    pugi::xml_node desc = m_svgNode.prepend_child("desc");
    desc.append_child(pugi::node_pcdata)
        .set_value(StringFormat("Engraved by Verovio %s", GetVersion().c_str()).c_str());

    // save the glyph data to m_outdata
    std::string indent = (m_indent == -1) ? "\t" : std::string(m_indent, ' ');
    m_svgDoc.save(m_outdata, indent.c_str(), output_flags);

    m_committed = true;
}

void SvgDeviceContext::StartGraphic(Object *object, std::string gClass, std::string gId, bool primary, bool prepend)
{
    if (object->HasAttClass(ATT_TYPED)) {
        AttTyped *att = dynamic_cast<AttTyped *>(object);
        assert(att);
        if (att->HasType()) {
            gClass.append((gClass.empty() ? "" : " ") + att->GetType());
        }
    }

    if (prepend) {
        m_currentNode = m_currentNode.prepend_child("g");
    }
    else {
        m_currentNode = m_currentNode.append_child("g");
    }
    m_svgNodeStack.push_back(m_currentNode);
    AppendIdAndClass(gId, object->GetClassName(), gClass, primary);

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
            if (object->IsControlElement()) {
                m_currentNode.append_attribute("color") = att->GetColor().c_str();
            }
            m_currentNode.append_attribute("fill") = att->GetColor().c_str();
        }
    }

    if (object->HasAttClass(ATT_LABELLED)) {
        AttLabelled *att = dynamic_cast<AttLabelled *>(object);
        assert(att);
        if (att->HasLabel()) {
            pugi::xml_node svgTitle = m_currentNode.prepend_child("title");
            svgTitle.append_attribute("class") = "labelAttr";
            svgTitle.append_child(pugi::node_pcdata).set_value(att->GetLabel().c_str());
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
            m_currentNode.append_attribute("font-style")
                = att->AttConverter::FontstyleToStr(att->GetFontstyle()).c_str();
        if (att->HasFontweight())
            m_currentNode.append_attribute("font-weight")
                = att->AttConverter::FontweightToStr(att->GetFontweight()).c_str();
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

    if (object->HasAttClass(ATT_LINKING)) {
        AttLinking *att = dynamic_cast<AttLinking *>(object);
        assert(att);
        if (att->HasFollows()) {
            m_currentNode.append_attribute("mei:follows") = att->GetFollows().c_str();
        }
        if (att->HasPrecedes()) {
            m_currentNode.append_attribute("mei:precedes") = att->GetPrecedes().c_str();
        }
    }

    // m_currentNode.append_attribute("style") = StringFormat("stroke: #%s; stroke-opacity: %f; fill: #%s; fill-opacity:
    // %f;",
    // GetColour(currentPen.GetColour()).c_str(), currentPen.GetOpacity(), GetColour(currentBrush.GetColour()).c_str(),
    // currentBrush.GetOpacity()).c_str();
}

void SvgDeviceContext::StartCustomGraphic(std::string name, std::string gClass, std::string gId)
{
    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    AppendIdAndClass(gId, name, gClass);
}

void SvgDeviceContext::StartTextGraphic(Object *object, std::string gClass, std::string gId)
{
    m_currentNode = AppendChild("tspan");
    m_svgNodeStack.push_back(m_currentNode);
    AppendIdAndClass(gId, object->GetClassName(), gClass);

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
            svgTitle.append_child(pugi::node_pcdata).set_value(att->GetLabel().c_str());
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
            m_currentNode.append_attribute("font-style")
                = att->AttConverter::FontstyleToStr(att->GetFontstyle()).c_str();
        if (att->HasFontweight())
            m_currentNode.append_attribute("font-weight")
                = att->AttConverter::FontweightToStr(att->GetFontweight()).c_str();
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
    DrawSvgBoundingBox(object, view);
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::EndCustomGraphic()
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::EndResumedGraphic(Object *object, View *view)
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::EndTextGraphic(Object *object, View *view)
{
    DrawSvgBoundingBox(object, view);
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

    // default styles
    if (this->UseGlobalStyling()) {
        m_currentNode = m_currentNode.append_child("style");
        m_currentNode.append_attribute("type") = "text/css";
        m_currentNode.append_child(pugi::node_pcdata)
            .set_value("g.page-margin{font-family:Times;} "
                       //"g.page-margin{background: pink;} "
                       //"g.bounding-box{stroke:red; stroke-width:10} "
                       //"g.content-bounding-box{stroke:blue; stroke-width:10} "
                       "g.reh, g.tempo{font-weight:bold;} g.dir, g.dynam, "
                       "g.mNum{font-style:italic;} g.label{font-weight:normal;}");
        m_currentNode = m_svgNodeStack.back();
    }

    // a graphic for definition scaling
    m_currentNode = m_currentNode.append_child("svg");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = "definition-scale";
    m_currentNode.append_attribute("color") = "black";
    if (this->GetFacsimile()) {
        m_currentNode.append_attribute("viewBox") = StringFormat("0 0 %d %d", GetWidth(), GetHeight()).c_str();
    }
    else {
        m_currentNode.append_attribute("viewBox")
            = StringFormat("0 0 %d %d", GetWidth() * DEFINITION_FACTOR, GetContentHeight() * DEFINITION_FACTOR).c_str();
    }

    // a graphic for the origin
    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = "page-margin";
    m_currentNode.append_attribute("transform")
        = StringFormat("translate(%d, %d)", (int)((double)m_originX), (int)((double)m_originY)).c_str();

    // margin rectangle for debugging
    // pugi::xml_node rect = m_currentNode.append_child("rect");
    // rect.append_attribute("fill") = "pink";
    // rect.append_attribute("height") = StringFormat("%d", GetHeight() * DEFINITION_FACTOR - 2 * m_originY).c_str();
    // rect.append_attribute("width") = StringFormat("%d", GetWidth() * DEFINITION_FACTOR - 2 * m_originX).c_str();

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

void SvgDeviceContext::SetBackground(int colour, int style)
{
    // nothing to do, we do not handle Background
}

void SvgDeviceContext::SetBackgroundImage(void *image, double opacity) {}

void SvgDeviceContext::SetBackgroundMode(int mode)
{
    // nothing to do, we do not handle Background Mode
}

void SvgDeviceContext::SetTextForeground(int colour)
{
    m_brushStack.top().SetColour(colour); // we use the brush colour for text
}

void SvgDeviceContext::SetTextBackground(int colour)
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

pugi::xml_node SvgDeviceContext::AppendChild(std::string name)
{
    pugi::xml_node g = m_currentNode.child("g");
    if (g)
        return m_currentNode.insert_child_before(name.c_str(), g);
    else
        return m_currentNode.append_child(name.c_str());
}

// Drawing methods
void SvgDeviceContext::DrawSimpleBezierPath(Point bezier[4])
{
    pugi::xml_node pathChild = AppendChild("path");
    pathChild.append_attribute("d") = StringFormat("M%d,%d C%d,%d %d,%d %d,%d", // Base string
        bezier[0].x, bezier[0].y, // M Command
        bezier[1].x, bezier[1].y, bezier[2].x, bezier[2].y, bezier[3].x, bezier[3].y // Remaining bezier points.
        )
                                          .c_str();
    pathChild.append_attribute("fill") = "none";
    pathChild.append_attribute("stroke") = GetColour(m_penStack.top().GetColour()).c_str();
    pathChild.append_attribute("stroke-linecap") = "round";
    pathChild.append_attribute("stroke-linejoin") = "round";
    pathChild.append_attribute("stroke-width") = m_penStack.top().GetWidth();
    if (m_penStack.top().GetDashLength() > 0) {
        // Since we have stroke-linecap=round, change the dash length to be the percieved length.
        int dashOn = std::max(m_penStack.top().GetDashLength() - m_penStack.top().GetWidth(), 0);
        int dashOff = m_penStack.top().GetDashLength() + m_penStack.top().GetWidth();
        pathChild.append_attribute("stroke-dasharray") = StringFormat("%d, %d", dashOn, dashOff).c_str();
    }
}
void SvgDeviceContext::DrawComplexBezierPath(Point bezier1[4], Point bezier2[4])
{
    pugi::xml_node pathChild = AppendChild("path");
    pathChild.append_attribute("d")
        = StringFormat("M%d,%d C%d,%d %d,%d %d,%d C%d,%d %d,%d %d,%d", bezier1[0].x, bezier1[0].y, // M command
            bezier1[1].x, bezier1[1].y, bezier1[2].x, bezier1[2].y, bezier1[3].x, bezier1[3].y, // First bezier
            bezier2[2].x, bezier2[2].y, bezier2[1].x, bezier2[1].y, bezier2[0].x, bezier2[0].y // Second Bezier
            )
              .c_str();
    // pathChild.append_attribute("fill") = "currentColor";
    // pathChild.append_attribute("fill-opacity") = "1";
    pathChild.append_attribute("stroke") = GetColour(m_penStack.top().GetColour()).c_str();
    pathChild.append_attribute("stroke-linecap") = "round";
    pathChild.append_attribute("stroke-linejoin") = "round";
    // pathChild.append_attribute("stroke-opacity") = "1";
    pathChild.append_attribute("stroke-width") = m_penStack.top().GetWidth();
}

void SvgDeviceContext::DrawCircle(int x, int y, int radius)
{
    DrawEllipse(x - radius, y - radius, 2 * radius, 2 * radius);
}

void SvgDeviceContext::DrawEllipse(int x, int y, int width, int height)
{
    assert(m_penStack.size());
    assert(m_brushStack.size());

    Pen currentPen = m_penStack.top();
    Brush currentBrush = m_brushStack.top();

    int rh = height / 2;
    int rw = width / 2;

    pugi::xml_node ellipseChild = AppendChild("ellipse");
    ellipseChild.append_attribute("cx") = x + rw;
    ellipseChild.append_attribute("cy") = y + rh;
    ellipseChild.append_attribute("rx") = rw;
    ellipseChild.append_attribute("ry") = rh;
    if (currentBrush.GetOpacity() != 1.0) ellipseChild.append_attribute("fill-opacity") = currentBrush.GetOpacity();
    if (currentPen.GetOpacity() != 1.0) ellipseChild.append_attribute("stroke-opacity") = currentPen.GetOpacity();
    if (currentPen.GetWidth() > 0) {
        ellipseChild.append_attribute("stroke-width") = currentPen.GetWidth();
        ellipseChild.append_attribute("stroke") = GetColour(m_penStack.top().GetColour()).c_str();
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

    int fArc;
    // flag for large or small arc 0 means less than 180 degrees
    if ((theta2 - theta1) > 0)
        fArc = 1;
    else
        fArc = 0;

    int fSweep;
    if (fabs(theta2 - theta1) > M_PI)
        fSweep = 1;
    else
        fSweep = 0;

    pugi::xml_node pathChild = AppendChild("path");
    pathChild.append_attribute("d") = StringFormat(
        "M%d %d A%d %d 0.0 %d %d %d %d", int(xs), int(ys), abs(int(rx)), abs(int(ry)), fArc, fSweep, int(xe), int(ye))
                                          .c_str();
    // pathChild.append_attribute("fill") = "currentColor";
    if (currentBrush.GetOpacity() != 1.0) pathChild.append_attribute("fill-opacity") = currentBrush.GetOpacity();
    if (currentPen.GetOpacity() != 1.0) pathChild.append_attribute("stroke-opacity") = currentPen.GetOpacity();
    if (currentPen.GetWidth() > 0) {
        pathChild.append_attribute("stroke-width") = currentPen.GetWidth();
        pathChild.append_attribute("stroke") = GetColour(m_penStack.top().GetColour()).c_str();
    }
}

void SvgDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    pugi::xml_node pathChild = AppendChild("path");
    pathChild.append_attribute("d") = StringFormat("M%d %d L%d %d", x1, y1, x2, y2).c_str();
    pathChild.append_attribute("stroke") = GetColour(m_penStack.top().GetColour()).c_str();
    if (m_penStack.top().GetLineCap() > 0) {
        pathChild.append_attribute("stroke-linecap") = "round";
        pathChild.append_attribute("stroke-dasharray")
            = StringFormat("1, %d", int(2.5 * m_penStack.top().GetDashLength())).c_str();
    }
    else if (m_penStack.top().GetDashLength() > 0)
        pathChild.append_attribute("stroke-dasharray")
            = StringFormat("%d, %d", m_penStack.top().GetDashLength(), m_penStack.top().GetDashLength()).c_str();
    if (m_penStack.top().GetWidth() > 1) pathChild.append_attribute("stroke-width") = m_penStack.top().GetWidth();
}

void SvgDeviceContext::DrawPolygon(int n, Point points[], int xoffset, int yoffset, int fill_style)
{
    assert(m_penStack.size());
    assert(m_brushStack.size());

    Pen currentPen = m_penStack.top();
    Brush currentBrush = m_brushStack.top();

    pugi::xml_node polygonChild = AppendChild("polygon");
    // if (fillStyle == wxODDEVEN_RULE)
    //    polygonChild.append_attribute("fill-rule") = "evenodd;";
    // else
    if (currentPen.GetWidth() > 0) polygonChild.append_attribute("stroke") = GetColour(currentPen.GetColour()).c_str();
    if (currentPen.GetWidth() > 1)
        polygonChild.append_attribute("stroke-width") = StringFormat("%d", currentPen.GetWidth()).c_str();
    if (currentPen.GetOpacity() != 1.0)
        polygonChild.append_attribute("stroke-opacity") = StringFormat("%f", currentPen.GetOpacity()).c_str();
    if (currentBrush.GetColour() != AxNONE)
        polygonChild.append_attribute("fill") = GetColour(currentBrush.GetColour()).c_str();
    if (currentBrush.GetOpacity() != 1.0)
        polygonChild.append_attribute("fill-opacity") = StringFormat("%f", currentBrush.GetOpacity()).c_str();

    std::string pointsString;
    for (int i = 0; i < n; ++i) {
        pointsString += StringFormat("%d,%d ", points[i].x + xoffset, points[i].y + yoffset);
    }
    polygonChild.append_attribute("points") = pointsString.c_str();
}

void SvgDeviceContext::DrawRectangle(int x, int y, int width, int height)
{
    DrawRoundedRectangle(x, y, width, height, 0);
}

void SvgDeviceContext::DrawRoundedRectangle(int x, int y, int width, int height, int radius)
{
    pugi::xml_node rectChild = AppendChild("rect");

    if (m_penStack.size()) {
        Pen currentPen = m_penStack.top();
        if (currentPen.GetWidth() > 0) rectChild.append_attribute("stroke") = GetColour(currentPen.GetColour()).c_str();
        if (currentPen.GetWidth() > 1)
            rectChild.append_attribute("stroke-width") = StringFormat("%d", currentPen.GetWidth()).c_str();
        if (currentPen.GetOpacity() != 1.0)
            rectChild.append_attribute("stroke-opacity") = StringFormat("%f", currentPen.GetOpacity()).c_str();
    }

    if (m_brushStack.size()) {
        Brush currentBrush = m_brushStack.top();
        if (currentBrush.GetColour() != AxNONE)
            rectChild.append_attribute("fill") = GetColour(currentBrush.GetColour()).c_str();
        if (currentBrush.GetOpacity() != 1.0)
            rectChild.append_attribute("fill-opacity") = StringFormat("%f", currentBrush.GetOpacity()).c_str();
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
    m_currentNode.append_attribute("x") = x;
    m_currentNode.append_attribute("y") = y;
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
void SvgDeviceContext::DrawText(const std::string &text, const std::wstring wtext, int x, int y, int width, int height)
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

    std::string currentFaceName
        = (m_currentNode.attribute("font-family")) ? m_currentNode.attribute("font-family").value() : "";
    std::string fontFaceName = m_fontStack.top()->GetFaceName();

    pugi::xml_node textChild = AppendChild("tspan");
    // We still add @xml::space (No: this seems to create problems with Safari)
    // textChild.append_attribute("xml:space") = "preserve";
    // Set the @font-family only if it is not the same as in the parent node
    if (!fontFaceName.empty() && (fontFaceName != currentFaceName)) {
        textChild.append_attribute("font-family") = m_fontStack.top()->GetFaceName().c_str();
        // Special case where we want to specifiy if the VerovioText font (woff) needs to be included in the output
        if (fontFaceName == "VerovioText") this->VrvTextFont();
    }
    if (m_fontStack.top()->GetPointSize() != 0) {
        textChild.append_attribute("font-size") = StringFormat("%dpx", m_fontStack.top()->GetPointSize()).c_str();
    }
    if (m_fontStack.top()->GetStyle() != FONTSTYLE_NONE) {
        if (m_fontStack.top()->GetStyle() == FONTSTYLE_italic) {
            textChild.append_attribute("font-style") = "italic";
        }
        else if (m_fontStack.top()->GetStyle() == FONTSTYLE_normal) {
            textChild.append_attribute("font-style") = "normal";
        }
        else if (m_fontStack.top()->GetStyle() == FONTSTYLE_oblique) {
            textChild.append_attribute("font-style") = "oblique";
        }
    }
    textChild.append_attribute("class") = "text";
    textChild.append_child(pugi::node_pcdata).set_value(svgText.c_str());

    if ((x != 0) && (y != 0) && (x != VRV_UNSET) && (y != VRV_UNSET)
        && (((width != 0) && (height != 0)) || ((width != VRV_UNSET) && (height != VRV_UNSET)))) {
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

void SvgDeviceContext::DrawMusicText(const std::wstring &text, int x, int y, bool setSmuflGlyph)
{
    assert(m_fontStack.top());

    int w, h, gx, gy;

    // print chars one by one
    for (unsigned int i = 0; i < text.length(); ++i) {
        wchar_t c = text.at(i);
        Glyph *glyph = Resources::GetGlyph(c);
        if (!glyph) {
            continue;
        }

        std::string path = glyph->GetPath();

        // Add the glyph to the array for the <defs>
        m_smuflGlyphs.insert(path);

        // Write the char in the SVG
        pugi::xml_node useChild = AppendChild("use");
        useChild.append_attribute("xlink:href") = StringFormat("#%s", glyph->GetCodeStr().c_str()).c_str();
        useChild.append_attribute("x") = x;
        useChild.append_attribute("y") = y;
        useChild.append_attribute("height") = StringFormat("%dpx", m_fontStack.top()->GetPointSize()).c_str();
        useChild.append_attribute("width") = StringFormat("%dpx", m_fontStack.top()->GetPointSize()).c_str();
        if (m_fontStack.top()->GetWidthToHeightRatio() != 1.0f) {
            useChild.append_attribute("transform") = StringFormat("matrix(%f,0,0,1,%f,0)",
                m_fontStack.top()->GetWidthToHeightRatio(), x * (1. - m_fontStack.top()->GetWidthToHeightRatio()))
                                                         .c_str();
        }

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

void SvgDeviceContext::DrawSvgShape(int x, int y, int width, int height, pugi::xml_node svg)
{
    m_currentNode.append_attribute("transform")
        = StringFormat("translate(%d, %d) scale(%d, %d)", x, y, DEFINITION_FACTOR, DEFINITION_FACTOR).c_str();

    for (pugi::xml_node child : svg.children()) {
        m_currentNode.append_copy(child);
    }
}

void SvgDeviceContext::DrawBackgroundImage(int x, int y) {}

void SvgDeviceContext::AddDescription(const std::string &text)
{
    pugi::xml_node desc = m_currentNode.append_child("desc");
    desc.append_child(pugi::node_pcdata).set_value(text.c_str());
}

void SvgDeviceContext::AppendIdAndClass(std::string gId, std::string baseClass, std::string addedClasses, bool primary)
{
    std::transform(baseClass.begin(), baseClass.begin() + 1, baseClass.begin(), ::tolower);

    if (gId.length() > 0) {
        if (m_html5) {
            m_currentNode.append_attribute("data-id") = gId.c_str();
        }
        else if (primary) {
            // Don't write ids for HTML5 to avoid id clashes when embedding into
            // an HTML document.
            m_currentNode.append_attribute("id") = gId.c_str();
        }
    }

    if (m_html5) {
        m_currentNode.append_attribute("data-class") = baseClass.c_str();
    }

    if (!primary) {
        baseClass.append(" spanning id-" + gId);
    }
    if (!addedClasses.empty()) {
        baseClass.append(" " + addedClasses);
    }
    m_currentNode.append_attribute("class") = baseClass.c_str();
}

std::string SvgDeviceContext::GetColour(int colour)
{
    std::ostringstream ss;
    ss << "#";
    ss << std::hex;

    switch (colour) {
        case (AxNONE): return "currentColor";
        case (AxBLACK): return "#000000";
        case (AxWHITE): return "#FFFFFF";
        case (AxRED): return "#FF0000";
        case (AxGREEN): return "#00FF00";
        case (AxBLUE): return "#0000FF";
        case (AxCYAN): return "#00FFFF";
        case (AxLIGHT_GREY): return "#777777";
        default:
            int blue = (colour & 255);
            int green = (colour >> 8) & 255;
            int red = (colour >> 16) & 255;
            ss << red << green << blue;
            // std::strin = wxDecToHex(char(red)) + wxDecToHex(char(green)) + wxDecToHex(char(blue)) ;  // ax3
            return ss.str();
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

    pugi::xml_node rectChild = AppendChild("rect");
    rectChild.append_attribute("x") = x;
    rectChild.append_attribute("y") = y;
    rectChild.append_attribute("height") = height;
    rectChild.append_attribute("width") = width;

    rectChild.append_attribute("fill") = "transparent";
}

void SvgDeviceContext::DrawSvgBoundingBox(Object *object, View *view)
{
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

        StartGraphic(object, "bounding-box", "bbox-" + object->GetUuid(), true, true);

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
                    Glyph *glyph = Resources::GetGlyph(object->GetBoundingBoxGlyph());
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

                        SetPen(AxGREEN, 10, AxSOLID);
                        SetBrush(AxGREEN, AxSOLID);
                        this->DrawCircle(view->ToDeviceContextX(p.x), view->ToDeviceContextY(p.y), 5);
                        SetPen(AxNONE, 1, AxSOLID);
                        SetBrush(AxNONE, AxSOLID);
                    }
                }
            }
        }

        EndGraphic(object, NULL);

        if (groupInPage) {
            m_currentNode = m_pageNode;
        }

        if (drawContentBB) {
            if (object->HasContentBB()) {
                StartGraphic(object, "content-bounding-box", "cbbox-" + object->GetUuid(), true, true);
                if (object->HasContentBB()) {
                    this->DrawSvgBoundingBoxRectangle(
                        view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX1()),
                        view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY1()),
                        view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX2())
                            - view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX1()),
                        view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY2())
                            - view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY1()));
                }
                EndGraphic(object, NULL);
            }
        }

        if (groupInPage) {
            m_currentNode = currentNode;
        }
    }
}

} // namespace vrv
