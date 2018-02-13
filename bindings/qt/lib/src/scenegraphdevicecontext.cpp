/////////////////////////////////////////////////////////////////////////////
// Name:        scenegraphdevicecontext.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrvqt/scenegraphdevicecontext.h"

//----------------------------------------------------------------------------

#include <QColor>
#include <QDebug>
#include <QGuiApplication>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <QSGSimpleRectNode>
#include <QScreen>
#include <cmath>

//----------------------------------------------------------------------------

#include "glyph.h"
#include "object.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "vrvqt/textquickitem.h"

namespace vrvQt {

SceneGraphDeviceContext::SceneGraphDeviceContext(QQuickItem *quickItem, QSGNode *node)
    : DeviceContext()
    , m_quickItem(quickItem)
    , m_node(node)
    , m_currentTextQuickItem(nullptr)
    , m_logicalOrigin(0, 0)
    , m_dpi(static_cast<float>(QGuiApplication::primaryScreen()->physicalDotsPerInch()))
{
}

void SceneGraphDeviceContext::Clear()
{
    m_currentTextQuickItem = nullptr;

    m_logicalOrigin = vrv::Point(0, 0);

    m_activeGraphicObjectsStack.clear();

    m_id2NodeMapping.clear();
    m_id2QuickItemMapping.clear();
}
//----------------------------------------------------------------------------

void SceneGraphDeviceContext::AddGeometryNode(QSGGeometryNode *node)
{
    m_node->appendChildNode(node);

    for (auto object : m_activeGraphicObjectsStack) {
        m_id2NodeMapping[object.id] << node;
    }
}

void SceneGraphDeviceContext::AddQuickItem(TextQuickItem *item)
{
    item->setParentItem(m_quickItem); // visual parent
    item->setParent(m_quickItem); // object parent (for proper cleanup)

    for (auto object : m_activeGraphicObjectsStack) {
        m_id2QuickItemMapping[object.id] << item;
    }
}

QList<QSGGeometryNode *> SceneGraphDeviceContext::GetGeometryNodesForId(QString id)
{
    return m_id2NodeMapping[id];
}

QList<TextQuickItem *> SceneGraphDeviceContext::GetQuickItemsForId(QString id)
{
    return m_id2QuickItemMapping[id];
}

QStringList SceneGraphDeviceContext::GetIdsForQuickItem(QQuickItem *item)
{
    QStringList ids;
    for (auto iter = m_id2QuickItemMapping.begin(); iter != m_id2QuickItemMapping.end(); ++iter) {
        for (auto quickItem : iter.value()) {
            if (quickItem == item) {
                ids << iter.key();
            }
        }
    }
    return ids;
}

void SceneGraphDeviceContext::SetBackground(int, int)
{
    // This function is also not implemented for SvgDeviceContext
}

void SceneGraphDeviceContext::SetBackgroundImage(void *, double)
{
    // This function is also not implemented for SvgDeviceContext
}

void SceneGraphDeviceContext::SetBackgroundMode(int)
{
    // This function is also not implemented for SvgDeviceContext
}

void SceneGraphDeviceContext::SetTextForeground(int colour)
{
    m_brushStack.top().SetColour(colour); // we use the brush colour for text
}

void SceneGraphDeviceContext::SetTextBackground(int)
{
    // This function is also not implemented for SvgDeviceContext
}

void SceneGraphDeviceContext::SetLogicalOrigin(int x, int y)
{
    m_logicalOrigin = vrv::Point(-x, -y);
}

vrv::Point SceneGraphDeviceContext::GetLogicalOrigin()
{
    return m_logicalOrigin;
}

void SceneGraphDeviceContext::DrawComplexBezierPath(vrv::Point bezier1[4], vrv::Point bezier2[4])
{
    // Note: No support for vertex antialiasing. Use a top-level QQuickView with multisample antialiasing.
    // TODO: Add vertex antialiasing, refer to
    // 1) Qt sources for "void QSGBasicInternalRectangleNode::updateGeometry()" in
    // qtdeclarative/src/quick/scenegraph/qsgbasicinternalrectanglenode.cpp
    // 2) https://stackoverflow.com/questions/28125425/smooth-painting-in-custom-qml-element

    vrv::Pen currentPen = m_penStack.top();

    int segmentCount = 16;

    QSGGeometryNode *node = new QSGGeometryNode;
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2 * segmentCount);
    geometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(static_cast<QRgb>(currentPen.GetColour()));
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);

    auto calculateCubicBezierPoint = [](vrv::Point p[4], float t) -> std::tuple<float, float> {
        auto invt = 1 - t;
        auto x = invt * invt * invt * p[0].x + 3 * invt * invt * t * p[1].x + 3 * invt * t * t * p[2].x
            + t * t * t * p[3].x;
        auto y = invt * invt * invt * p[0].y + 3 * invt * invt * t * p[1].y + 3 * invt * t * t * p[2].y
            + t * t * t * p[3].y;
        return std::make_tuple(x, y);
    };

    // This loop calculates the bezier points for the inner and the outer line and add them as vertices. The list of
    // vertices is built so that points from the inner and outer line are alternating. This allows to draw a filled area
    // with DrawTriangleStrip.
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < segmentCount; ++i) {
        float bezierPointX = 0;
        float bezierPointY = 0;
        float currentSegment = i / static_cast<float>(segmentCount - 1);

        // Calculate bezier point on bezier1
        std::tie(bezierPointX, bezierPointY) = calculateCubicBezierPoint(bezier1, currentSegment);
        vertices[i * 2].set(translateX(bezierPointX), translateY(bezierPointY));

        // Calculate bezier point on bezier2
        std::tie(bezierPointX, bezierPointY) = calculateCubicBezierPoint(bezier2, currentSegment);
        vertices[i * 2 + 1].set(translateX(bezierPointX), translateY(bezierPointY));
    }

    node->markDirty(QSGNode::DirtyGeometry);
    AddGeometryNode(node);
}

void SceneGraphDeviceContext::DrawCircle(int x, int y, int radius)
{
    // Note: No support for vertex antialiasing. Use a top-level QQuickView with multisample antialiasing.
    // TODO: Add vertex antialiasing, refer to
    // 1) Qt sources for "void QSGBasicInternalRectangleNode::updateGeometry()" in
    // qtdeclarative/src/quick/scenegraph/qsgbasicinternalrectanglenode.cpp
    // 2) https://stackoverflow.com/questions/28125425/smooth-painting-in-custom-qml-element

    vrv::Pen currentPen = m_penStack.top();

    int segmentCount = 16;

    QSGGeometryNode *node = new QSGGeometryNode();
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), segmentCount);
    geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(static_cast<QRgb>(currentPen.GetColour()));
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);

    // This draws individual triangles from the first point (center) to every outer point by using DrawTriangleFan.
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    int numPoints = geometry->vertexCount();
    vertices[0].x = translateX(x);
    vertices[0].y = translateY(y);
    for (int i = 1; i < numPoints; ++i) {
        double theta = i * 2 * M_PI / (numPoints - 2);
        vertices[i].x = translateX(x + radius * static_cast<float>(std::cos(theta)));
        vertices[i].y = translateY(y - radius * static_cast<float>(std::sin(theta)));
    }

    node->markDirty(QSGNode::DirtyGeometry);
    AddGeometryNode(node);
}

void SceneGraphDeviceContext::DrawEllipse(int, int, int, int)
{
    qWarning() << "Warning:" << __FUNCTION__ << "not supported";
}

void SceneGraphDeviceContext::DrawEllipticArc(int, int, int, int, double, double)
{
    qWarning() << "Warning:" << __FUNCTION__ << "not supported";
}

void SceneGraphDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    vrv::Pen currentPen = m_penStack.top();

    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
    geometry->setDrawingMode(GL_LINES);
    geometry->setLineWidth(translate(currentPen.GetWidth()));
    geometry->vertexDataAsPoint2D()[0].set(translateX(x1), translateY(y1));
    geometry->vertexDataAsPoint2D()[1].set(translateX(x2), translateY(y2));

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(static_cast<QRgb>(currentPen.GetColour()));

    QSGGeometryNode *node = new QSGGeometryNode;
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);

    AddGeometryNode(node);
}

void SceneGraphDeviceContext::DrawPolygon(int n, vrv::Point points[], int xoffset, int yoffset, int)
{
    // Note: No support for vertex antialiasing. Use a top-level QQuickView with multisample antialiasing.
    // TODO: Add vertex antialiasing, refer to
    // 1) Qt sources for "void QSGBasicInternalRectangleNode::updateGeometry()" in
    // qtdeclarative/src/quick/scenegraph/qsgbasicinternalrectanglenode.cpp
    // 2) https://stackoverflow.com/questions/28125425/smooth-painting-in-custom-qml-element

    // TODO: This function only works for convex polygons. At the moment verovio calls this function only with n = 4.
    // Maybe this function should be renamed to DrawConvexPolygon

    vrv::Pen currentPen = m_penStack.top();

    QSGGeometry *geometry;
    geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), n);
    geometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(static_cast<QRgb>(currentPen.GetColour()));

    // Reorder points so that they can be drawn with DrawTriangleStrip.
    int counter1 = 0;
    int counter2 = n - 1;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            geometry->vertexDataAsPoint2D()[i].set(
                translateX(points[counter1].x + xoffset), translateY(points[counter1].y + yoffset));
            counter1++;
        }
        else {
            geometry->vertexDataAsPoint2D()[i].set(
                translateX(points[counter2].x + xoffset), translateY(points[counter2].y + yoffset));
            counter2--;
        }
    }

    QSGGeometryNode *node = new QSGGeometryNode;
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);

    AddGeometryNode(node);
}

void SceneGraphDeviceContext::DrawRectangle(int x, int y, int width, int height)
{
    vrv::Pen currentPen = m_penStack.top();

    QSGSimpleRectNode *node = new QSGSimpleRectNode;
    node->setColor(static_cast<QRgb>(currentPen.GetColour()));
    qreal rectX = static_cast<qreal>(translateX(x));
    qreal rectY = static_cast<qreal>(translateY(y));
    qreal rectWidth = static_cast<qreal>(translate(width));
    qreal rectHeight = static_cast<qreal>(translate(height));
    node->setRect(rectX, rectY, rectWidth, rectHeight);
    AddGeometryNode(node);
}

void SceneGraphDeviceContext::DrawRotatedText(const std::string &, int, int, double)
{
    // This function is also not implemented for SvgDeviceContext
}

void SceneGraphDeviceContext::DrawRoundedRectangle(int, int, int, int, double)
{
    qWarning() << "Warning:" << __FUNCTION__ << "not supported";
}

void SceneGraphDeviceContext::StartText(int x, int y, vrv::data_HORIZONTALALIGNMENT alignment)
{
    Q_ASSERT(m_currentTextQuickItem == nullptr);

    // Memory management is handled by m_quickItem (set as parentItem in EndText)
    m_currentTextQuickItem = new TextQuickItem();

    SetTextPositionAndAlignment(x, y, alignment);
}

void SceneGraphDeviceContext::DrawText(const std::string &text, const std::wstring, int x, int y)
{
    Q_ASSERT(m_currentTextQuickItem != nullptr);

    if (x != VRV_UNSET || y != VRV_UNSET) {
        qWarning() << "Warning:" << __FUNCTION__ << "does not yet support specifying x and y";
    }

    QFont font(QString::fromStdString(m_fontStack.top()->GetFaceName()));

    // Note: Verovio calls it point size but it is actually pixel size. Qt can only handle pixel size in int, thus it
    // would be better to use point size.
    int pixelSize = static_cast<int>(translate(m_fontStack.top()->GetPointSize()));
    font.setPixelSize(pixelSize);

    if (m_fontStack.top()->GetStyle() != vrv::FONTSTYLE_NONE) {
        if (m_fontStack.top()->GetStyle() == vrv::FONTSTYLE_italic) {
            font.setStyle(QFont::StyleItalic);
        }
        else if (m_fontStack.top()->GetStyle() == vrv::FONTSTYLE_normal) {
            font.setStyle(QFont::StyleNormal);
        }
        else if (m_fontStack.top()->GetStyle() == vrv::FONTSTYLE_oblique) {
            font.setStyle(QFont::StyleOblique);
        }
    }
    if (m_fontStack.top()->GetWeight() != vrv::FONTWEIGHT_NONE) {
        if (m_fontStack.top()->GetWeight() == vrv::FONTWEIGHT_bold) {
            font.setWeight(QFont::Bold);
        }
    }

    m_currentTextQuickItem->appendText(QString::fromStdString(text), font);
}

void SceneGraphDeviceContext::EndText()
{
    Q_ASSERT(m_currentTextQuickItem != nullptr);

    m_currentTextQuickItem->calcPos();

    AddQuickItem(m_currentTextQuickItem);
    m_currentTextQuickItem = nullptr;
}

void SceneGraphDeviceContext::MoveTextTo(int x, int y, vrv::data_HORIZONTALALIGNMENT alignment)
{
    Q_ASSERT(m_currentTextQuickItem != nullptr);

    // If the current text item already has some text, we have to create a new one which automatically sets the position
    // and alignment. If no alignment is specified, we use the alignment of the current text item.
    if (!m_currentTextQuickItem->isEmpty()) {
        if (alignment == vrv::HORIZONTALALIGNMENT_NONE) {
            switch (m_currentTextQuickItem->getAlignment()) {
                case Qt::AlignLeft: alignment = vrv::HORIZONTALALIGNMENT_left; break;
                case Qt::AlignRight: alignment = vrv::HORIZONTALALIGNMENT_right; break;
                case Qt::AlignHCenter: alignment = vrv::HORIZONTALALIGNMENT_center; break;
                case Qt::AlignJustify: alignment = vrv::HORIZONTALALIGNMENT_justify; break;
            }
        }
        EndText();
        StartText(x, y, alignment);
        return;
    }
    else {
        SetTextPositionAndAlignment(x, y, alignment);
    }
}

void SceneGraphDeviceContext::SetTextPositionAndAlignment(int x, int y, vrv::data_HORIZONTALALIGNMENT alignment)
{
    m_currentTextQuickItem->setX(static_cast<double>(translateX(x)));
    m_currentTextQuickItem->setY(static_cast<double>(translateY(y)));

    switch (alignment) {
        case vrv::HORIZONTALALIGNMENT_left: m_currentTextQuickItem->setAlignment(Qt::AlignLeft); break;
        case vrv::HORIZONTALALIGNMENT_right: m_currentTextQuickItem->setAlignment(Qt::AlignRight); break;
        case vrv::HORIZONTALALIGNMENT_center: m_currentTextQuickItem->setAlignment(Qt::AlignHCenter); break;
        case vrv::HORIZONTALALIGNMENT_justify: m_currentTextQuickItem->setAlignment(Qt::AlignJustify); break;
        default: break;
    }
}

void SceneGraphDeviceContext::DrawMusicText(const std::wstring &text, int x, int y, bool)
{
    Q_ASSERT(m_fontStack.top());

    QFont font(QString::fromStdString(m_fontStack.top()->GetFaceName()));

    // Note: Verovio calls it point size but it is actually pixel size. Qt can only handle pixel size in int, thus it
    // would be better to use point size.
    int pixelSize = static_cast<int>(translate(m_fontStack.top()->GetPointSize()));
    font.setPixelSize(pixelSize);

    // Memory management is handled by m_quickItem (set in AddQuickItem)
    auto musicTextQuickItem = new TextQuickItem();

    musicTextQuickItem->appendText(QString::fromStdWString(text), font);

    musicTextQuickItem->setX(static_cast<double>(translateX(x)));
    musicTextQuickItem->setY(static_cast<double>(translateY(y)));
    musicTextQuickItem->calcPos();

    AddQuickItem(musicTextQuickItem);
}

void SceneGraphDeviceContext::DrawSpline(int, vrv::Point[])
{
    // This function is also not implemented for SvgDeviceContext
}

void SceneGraphDeviceContext::DrawSvgShape(int x, int y, int width, int height, pugi::xml_node svg)
{
    // This function is not yet supported
}

void SceneGraphDeviceContext::DrawBackgroundImage(int, int)
{
    // This function is also not implemented for SvgDeviceContext
}

void SceneGraphDeviceContext::StartGraphic(vrv::Object *object, std::string, std::string gId)
{
    m_activeGraphicObjectsStack.push(ActiveGraphic(QString::fromStdString(gId), object));
}

void SceneGraphDeviceContext::EndGraphic(vrv::Object *, vrv::View *)
{
    m_activeGraphicObjectsStack.pop();
}

void SceneGraphDeviceContext::ResumeGraphic(vrv::Object *object, std::string gId)
{
    m_activeGraphicObjectsStack.push(ActiveGraphic(QString::fromStdString(gId), object));
}

void SceneGraphDeviceContext::EndResumedGraphic(vrv::Object *, vrv::View *)
{
    m_activeGraphicObjectsStack.pop();
}

void SceneGraphDeviceContext::RotateGraphic(const vrv::Point &, double)
{
    qWarning() << "Warning:" << __FUNCTION__ << "not supported";
}

void SceneGraphDeviceContext::StartPage()
{
    // No action required
}

void SceneGraphDeviceContext::EndPage()
{
    // No action required
}
} // namespace vrvQt
