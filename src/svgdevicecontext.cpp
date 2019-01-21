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

    SetBrush(AxBLACK, AxSOLID);
    SetPen(AxBLACK, 1, AxSOLID);

    m_smuflGlyphs.clear();

    m_committed = false;
    m_vrvTextFont = false;

    m_mmOutput = false;

    // create the initial SVG element
    // width and height need to be set later; these are taken care of in "commit"
    m_svgNode = m_svgDoc.append_child("svg");
    m_svgNode.append_attribute("version") = "1.1";
    m_svgNode.append_attribute("xmlns") = "http://www.w3.org/2000/svg";
    m_svgNode.append_attribute("xmlns:xlink") = "http://www.w3.org/1999/xlink";
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
    if (m_mmOutput) {
        m_svgNode.prepend_attribute("height")
            = StringFormat("%.2fmm", ((double)GetHeight() * GetUserScaleY()) / 10).c_str();
        m_svgNode.prepend_attribute("width")
            = StringFormat("%.2fmm", ((double)GetWidth() * GetUserScaleX()) / 10).c_str();
    }
    else {
        m_svgNode.prepend_attribute("height") = StringFormat("%.2fpx", ((double)GetHeight() * GetUserScaleY())).c_str();
        m_svgNode.prepend_attribute("width") = StringFormat("%.2fpx", ((double)GetWidth() * GetUserScaleX())).c_str();
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
        std::vector<std::string>::const_iterator it;
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
    m_svgDoc.save(m_outdata, "\t", output_flags);

    m_committed = true;
}

void SvgDeviceContext::StartGraphic(Object *object, std::string gClass, std::string gId)
{
    std::string baseClass = object->GetClassName();
    std::transform(baseClass.begin(), baseClass.begin() + 1, baseClass.begin(), ::tolower);
    if (gClass.length() > 0) {
        baseClass.append(" " + gClass);
    }

    if (object->HasAttClass(ATT_TYPED)) {
        AttTyped *att = dynamic_cast<AttTyped *>(object);
        assert(att);
        if (att->HasType()) {
            baseClass.append(" " + att->GetType());
        }
    }

    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = baseClass.c_str();
    if (gId.length() > 0) {
        m_currentNode.append_attribute("id") = gId.c_str();
    }

    // this sets staffDef styles for lyrics
    if (object->Is(STAFF)) {
        Staff *staff = dynamic_cast<Staff *>(object);
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

    // m_currentNode.append_attribute("style") = StringFormat("stroke: #%s; stroke-opacity: %f; fill: #%s; fill-opacity:
    // %f;",
    // GetColour(currentPen.GetColour()).c_str(), currentPen.GetOpacity(), GetColour(currentBrush.GetColour()).c_str(),
    // currentBrush.GetOpacity()).c_str();
}

void SvgDeviceContext::StartCustomGraphic(std::string name, std::string gClass, std::string gId)
{
    if (gClass.length() > 0) {
        name.append(" " + gClass);
    }

    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = name.c_str();
    if (gId.length() > 0) {
        m_currentNode.append_attribute("id") = gId.c_str();
    }
}

void SvgDeviceContext::StartTextGraphic(Object *object, std::string gClass, std::string gId)
{
    std::string baseClass = object->GetClassName();
    std::transform(baseClass.begin(), baseClass.begin() + 1, baseClass.begin(), ::tolower);
    if (gClass.length() > 0) {
        baseClass.append(" " + gClass);
    }

    m_currentNode = AppendChild("tspan");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = baseClass.c_str();
    m_currentNode.append_attribute("id") = gId.c_str();

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
    std::string xpath = "//g[@id=\"" + gId + "\"]";
    pugi::xpath_node selection = m_currentNode.select_single_node(xpath.c_str());
    if (selection) {
        m_currentNode = selection.node();
    }
    m_svgNodeStack.push_back(m_currentNode);
}

void SvgDeviceContext::EndGraphic(Object *object, View *view)
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
    DrawSvgBoundingBox(object, view);
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
    DrawSvgBoundingBox(object, view);
}

void SvgDeviceContext::EndTextGraphic(Object *object, View *view)
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
    DrawSvgBoundingBox(object, view);
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
                       "g.tempo{font-weight:bold;} g.dir, g.dynam, "
                       "g.mNum{font-style:italic;} g.label{font-weight:normal;}");
        m_currentNode = m_svgNodeStack.back();
    }

    // a graphic for definition scaling
    m_currentNode = m_currentNode.append_child("svg");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = "definition-scale";
    m_currentNode.append_attribute("viewBox")
        = StringFormat("0 0 %d %d", GetWidth() * DEFINITION_FACTOR, GetHeight() * DEFINITION_FACTOR).c_str();

    // a graphic for the origin
    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = "page-margin";
    m_currentNode.append_attribute("transform")
        = StringFormat("translate(%d, %d)", (int)((double)m_originX), (int)((double)m_originY)).c_str();

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
void SvgDeviceContext::DrawComplexBezierPath(Point bezier1[4], Point bezier2[4])
{
    pugi::xml_node pathChild = AppendChild("path");
    pathChild.append_attribute("d")
        = StringFormat("M%d,%d C%d,%d %d,%d %d,%d C%d,%d %d,%d %d,%d", bezier1[0].x, bezier1[0].y, // M command
            bezier1[1].x, bezier1[1].y, bezier1[2].x, bezier1[2].y, bezier1[3].x, bezier1[3].y, // First bezier
            bezier2[2].x, bezier2[2].y, bezier2[1].x, bezier2[1].y, bezier2[0].x, bezier2[0].y // Second Bezier
            )
              .c_str();
    // pathChild.append_attribute("fill") = "#000000";
    // pathChild.append_attribute("fill-opacity") = "1";
    pathChild.append_attribute("stroke") = StringFormat("#%s", GetColour(m_penStack.top().GetColour()).c_str()).c_str();
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
        ellipseChild.append_attribute("stroke")
            = StringFormat("#%s", GetColour(m_penStack.top().GetColour()).c_str()).c_str();
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
    // pathChild.append_attribute("fill") = "#000000";
    if (currentBrush.GetOpacity() != 1.0) pathChild.append_attribute("fill-opacity") = currentBrush.GetOpacity();
    if (currentPen.GetOpacity() != 1.0) pathChild.append_attribute("stroke-opacity") = currentPen.GetOpacity();
    if (currentPen.GetWidth() > 0) {
        pathChild.append_attribute("stroke-width") = currentPen.GetWidth();
        pathChild.append_attribute("stroke")
            = StringFormat("#%s", GetColour(m_penStack.top().GetColour()).c_str()).c_str();
    }
}

void SvgDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    pugi::xml_node pathChild = AppendChild("path");
    pathChild.append_attribute("d") = StringFormat("M%d %d L%d %d", x1, y1, x2, y2).c_str();
    pathChild.append_attribute("stroke") = StringFormat("#%s", GetColour(m_penStack.top().GetColour()).c_str()).c_str();
    if (m_penStack.top().GetDashLength() > 0)
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
    if (currentPen.GetWidth() > 0)
        polygonChild.append_attribute("stroke")
            = StringFormat("#%s", GetColour(currentPen.GetColour()).c_str()).c_str();
    if (currentPen.GetWidth() > 1)
        polygonChild.append_attribute("stroke-width") = StringFormat("%d", currentPen.GetWidth()).c_str();
    if (currentPen.GetOpacity() != 1.0)
        polygonChild.append_attribute("stroke-opacity") = StringFormat("%f", currentPen.GetOpacity()).c_str();
    if (currentBrush.GetColour() != AxBLACK)
        polygonChild.append_attribute("fill")
            = StringFormat("#%s", GetColour(currentBrush.GetColour()).c_str()).c_str();
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

void SvgDeviceContext::DrawRoundedRectangle(int x, int y, int width, int height, double radius)
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
    if (radius != 0) rectChild.append_attribute("rx") = radius;
    // for empty rectangles with bounding boxes
    /*
    rectChild.append_attribute("fill-opacity") = "0.0";
    rectChild.append_attribute("stroke-opacity") = "1.0";
    rectChild.append_attribute("stroke-width") = "10";
    rectChild.append_attribute("stroke") = StringFormat("#%s", GetColour(m_penStack.top().GetColour()).c_str()).c_str();
     */
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

void SvgDeviceContext::EndText()
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::DrawText(const std::string &text, const std::wstring wtext, int x, int y)
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
    textChild.append_attribute("class") = "text";
    textChild.append_child(pugi::node_pcdata).set_value(svgText.c_str());

    if ((x != VRV_UNSET) && (y != VRV_UNSET)) {
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
        std::vector<std::string>::const_iterator it = std::find(m_smuflGlyphs.begin(), m_smuflGlyphs.end(), path);
        if (it == m_smuflGlyphs.end()) {
            m_smuflGlyphs.push_back(path);
        }

        // Write the char in the SVG
        pugi::xml_node useChild = AppendChild("use");
        useChild.append_attribute("xlink:href") = StringFormat("#%s", glyph->GetCodeStr().c_str()).c_str();
        useChild.append_attribute("x") = x;
        useChild.append_attribute("y") = y;
        useChild.append_attribute("height") = StringFormat("%dpx", m_fontStack.top()->GetPointSize()).c_str();
        useChild.append_attribute("width") = StringFormat("%dpx", m_fontStack.top()->GetPointSize()).c_str();

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

std::string SvgDeviceContext::GetColour(int colour)
{
    std::ostringstream ss;
    ss << std::hex;

    switch (colour) {
        case (AxBLACK): return "000000";
        case (AxWHITE): return "FFFFFF";
        case (AxRED): return "FF0000";
        case (AxGREEN): return "00FF00";
        case (AxBLUE): return "0000FF";
        case (AxCYAN): return "00FFFF";
        case (AxLIGHT_GREY): return "777777";
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

void SvgDeviceContext::DrawSvgBoundingBox(Object *object, View *view)
{
    bool drawBoundingBox = false;
    if (drawBoundingBox && view) {
        BoundingBox *box = object;
        // For floating elements, get the current bounding box set by System::SetCurrentFloatingPositioner
        if (object->IsFloatingObject()) {
            FloatingObject *floatingObject = dynamic_cast<FloatingObject *>(object);
            assert(floatingObject);
            box = floatingObject->GetCurrentFloatingPositioner();
            // No bounding box found, ignore the object - this happens when the @staff is missing because the element is
            // never drawn but there is still a EndGraphic call.
            if (!box) return;
        }

        SetPen(AxRED, 10, AxDOT_DASH);
        // SetBrush(AxWHITE, AxTRANSPARENT);
        pugi::xml_node currentNode = m_currentNode;
        m_currentNode = m_pageNode;
        StartGraphic(object, "self-bounding-box", "bbox-" + object->GetUuid());
        if (box->HasSelfBB()) {
            this->DrawRectangle(view->ToDeviceContextX(object->GetDrawingX() + box->GetSelfX1()),
                view->ToDeviceContextY(object->GetDrawingY() + box->GetSelfY1()),
                view->ToDeviceContextX(object->GetDrawingX() + box->GetSelfX2())
                    - view->ToDeviceContextX(object->GetDrawingX() + box->GetSelfX1()),
                view->ToDeviceContextY(object->GetDrawingY() + box->GetSelfY2())
                    - view->ToDeviceContextY(object->GetDrawingY() + box->GetSelfY1()));
        }

        SetPen(AxBLACK, 1, AxSOLID);
        SetBrush(AxBLACK, AxSOLID);

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
                    SetPen(AxBLACK, 1, AxSOLID);
                    SetBrush(AxBLACK, AxSOLID);
                }
            }
        }

        EndGraphic(object, NULL);

        m_currentNode = m_pageNode;

        // Rend *rend = dynamic_cast<Rend *>(object);
        // if (rend && rend->HasHalign()) {
        if (object->IsTextElement()) {

            SetPen(AxBLUE, 20, AxDOT_DASH);
            StartGraphic(object, "content-bounding-box", "cbbox-" + object->GetUuid());
            if (object->HasContentBB()) {
                this->DrawRectangle(view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX1()),
                    view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY1()),
                    view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX2())
                        - view->ToDeviceContextX(object->GetDrawingX() + box->GetContentX1()),
                    view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY2())
                        - view->ToDeviceContextY(object->GetDrawingY() + box->GetContentY1()));
            }
            EndGraphic(object, NULL);
        }

        m_currentNode = currentNode;

        SetPen(AxBLACK, 1, AxSOLID);
        SetBrush(AxBLACK, AxSOLID);
    }
}

} // namespace vrv
