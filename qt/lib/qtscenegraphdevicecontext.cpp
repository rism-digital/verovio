/////////////////////////////////////////////////////////////////////////////
// Name:        qtscenegraphdevicecontext.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "qtscenegraphdevicecontext.h"

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

#include "textquickitem.h"

namespace vrv {

QtSceneGraphDeviceContext::QtSceneGraphDeviceContext(QQuickItem *quickItem, QSGNode *node)
    : DeviceContext()
    , m_quickItem(quickItem)
    , m_node(node)
    , m_currentTextQuickItem(nullptr)
    , m_logicalOrigin(0, 0)
    , m_dpi(static_cast<float>(QGuiApplication::primaryScreen()->physicalDotsPerInch()))
{
}

void QtSceneGraphDeviceContext::Clear()
{
    m_currentTextQuickItem = nullptr;

    m_logicalOrigin = Point(0, 0);

    m_activeGraphicObjectsStack.clear();

    m_id2NodeMapping.clear();
    m_id2QuickItemMapping.clear();
}
//----------------------------------------------------------------------------

void QtSceneGraphDeviceContext::AddGeometryNode(QSGGeometryNode *node)
{
    m_node->appendChildNode(node);

    for (auto object : m_activeGraphicObjectsStack) {
        m_id2NodeMapping[object.id] << node;
    }
}

void QtSceneGraphDeviceContext::AddQuickItem(TextQuickItem *item)
{
    item->setParentItem(m_quickItem); // visual parent
    item->setParent(m_quickItem); // object parent (for proper cleanup)

    for (auto object : m_activeGraphicObjectsStack) {
        m_id2QuickItemMapping[object.id] << item;
    }
}

QList<QSGGeometryNode *> QtSceneGraphDeviceContext::GetGeometryNodesForId(QString id)
{
    return m_id2NodeMapping[id];
}

QList<TextQuickItem *> QtSceneGraphDeviceContext::GetQuickItemsForId(QString id)
{
    return m_id2QuickItemMapping[id];
}

QStringList QtSceneGraphDeviceContext::GetIdsForQuickItem(QQuickItem *item)
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

void QtSceneGraphDeviceContext::SetBackground(int, int)
{
    // This function is also not implemented for SvgDeviceContext
}

void QtSceneGraphDeviceContext::SetBackgroundImage(void *, double)
{
    // This function is also not implemented for SvgDeviceContext
}

void QtSceneGraphDeviceContext::SetBackgroundMode(int)
{
    // This function is also not implemented for SvgDeviceContext
}

void QtSceneGraphDeviceContext::SetTextForeground(int colour)
{
    m_brushStack.top().SetColour(colour); // we use the brush colour for text
}

void QtSceneGraphDeviceContext::SetTextBackground(int)
{
    // This function is also not implemented for SvgDeviceContext
}

void QtSceneGraphDeviceContext::SetLogicalOrigin(int x, int y)
{
    m_logicalOrigin = Point(-x, -y);
}

Point QtSceneGraphDeviceContext::GetLogicalOrigin()
{
    return m_logicalOrigin;
}

void QtSceneGraphDeviceContext::DrawComplexBezierPath(Point bezier1[4], Point bezier2[4])
{
    // Note: No support for vertex antialiasing. Use a top-level QQuickView with multisample antialiasing.
    // TODO: Add vertex antialiasing, refer to
    // 1) Qt sources for "void QSGBasicInternalRectangleNode::updateGeometry()" in
    // qtdeclarative/src/quick/scenegraph/qsgbasicinternalrectanglenode.cpp
    // 2) https://stackoverflow.com/questions/28125425/smooth-painting-in-custom-qml-element

    Pen currentPen = m_penStack.top();

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

    auto calculateCubicBezierPoint = [](Point p[4], float t) -> std::tuple<float, float> {
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

void QtSceneGraphDeviceContext::DrawCircle(int x, int y, int radius)
{
    // Note: No support for vertex antialiasing. Use a top-level QQuickView with multisample antialiasing.
    // TODO: Add vertex antialiasing, refer to
    // 1) Qt sources for "void QSGBasicInternalRectangleNode::updateGeometry()" in
    // qtdeclarative/src/quick/scenegraph/qsgbasicinternalrectanglenode.cpp
    // 2) https://stackoverflow.com/questions/28125425/smooth-painting-in-custom-qml-element

    Pen currentPen = m_penStack.top();

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

void QtSceneGraphDeviceContext::DrawEllipse(int, int, int, int)
{
    qWarning() << "Warning:" << __FUNCTION__ << "not supported";
}

void QtSceneGraphDeviceContext::DrawEllipticArc(int, int, int, int, double, double)
{
    qWarning() << "Warning:" << __FUNCTION__ << "not supported";
}

void QtSceneGraphDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    Pen currentPen = m_penStack.top();

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

void QtSceneGraphDeviceContext::DrawPolygon(int n, Point points[], int xoffset, int yoffset, int)
{
    // Note: No support for vertex antialiasing. Use a top-level QQuickView with multisample antialiasing.
    // TODO: Add vertex antialiasing, refer to
    // 1) Qt sources for "void QSGBasicInternalRectangleNode::updateGeometry()" in
    // qtdeclarative/src/quick/scenegraph/qsgbasicinternalrectanglenode.cpp
    // 2) https://stackoverflow.com/questions/28125425/smooth-painting-in-custom-qml-element

    // TODO: This function only works for convex polygons. At the moment verovio calls this function only with n = 4.
    // Maybe this function should be renamed to DrawConvexPolygon

    Pen currentPen = m_penStack.top();

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

void QtSceneGraphDeviceContext::DrawRectangle(int x, int y, int width, int height)
{
    Pen currentPen = m_penStack.top();

    QSGSimpleRectNode *node = new QSGSimpleRectNode;
    node->setColor(static_cast<QRgb>(currentPen.GetColour()));
    qreal rectX = static_cast<qreal>(translateX(x));
    qreal rectY = static_cast<qreal>(translateY(y));
    qreal rectWidth = static_cast<qreal>(translate(width));
    qreal rectHeight = static_cast<qreal>(translate(height));
    node->setRect(rectX, rectY, rectWidth, rectHeight);
    AddGeometryNode(node);
}

void QtSceneGraphDeviceContext::DrawRotatedText(const std::string &, int, int, double)
{
    // This function is also not implemented for SvgDeviceContext
}

void QtSceneGraphDeviceContext::DrawRoundedRectangle(int, int, int, int, double)
{
    qWarning() << "Warning:" << __FUNCTION__ << "not supported";
}

void QtSceneGraphDeviceContext::StartText(int x, int y, char alignment)
{
    Q_ASSERT(m_currentTextQuickItem == nullptr);

    // Memory management is handled by m_quickItem (set as parentItem in EndText)
    m_currentTextQuickItem = new TextQuickItem();
    m_currentTextQuickItem->setX(static_cast<double>(translateX(x)));
    m_currentTextQuickItem->setY(static_cast<double>(translateY(y)));

    switch (alignment) {
        case RIGHT: m_currentTextQuickItem->setAlignment(Qt::AlignRight); break;
        case CENTER: m_currentTextQuickItem->setAlignment(Qt::AlignHCenter); break;
        case LEFT: m_currentTextQuickItem->setAlignment(Qt::AlignLeft); break;
    }
}

void QtSceneGraphDeviceContext::DrawText(const std::string &text, const std::wstring)
{
    Q_ASSERT(m_currentTextQuickItem != nullptr);

    QFont font(QString::fromStdString(m_fontStack.top()->GetFaceName()));

    // Note: Verovio calls it point size but it is actually pixel size. Qt can only handle pixel size in int, thus we
    // convert the pixel size to point size.
    double pointSize = static_cast<double>(translate(ConvertPixelSizeToPointSize(m_fontStack.top()->GetPointSize())));
    font.setPointSizeF(pointSize);

    if (m_fontStack.top()->GetStyle() != FONTSTYLE_NONE) {
        if (m_fontStack.top()->GetStyle() == FONTSTYLE_italic) {
            font.setStyle(QFont::StyleItalic);
        }
        else if (m_fontStack.top()->GetStyle() == FONTSTYLE_normal) {
            font.setStyle(QFont::StyleNormal);
        }
        else if (m_fontStack.top()->GetStyle() == FONTSTYLE_oblique) {
            font.setStyle(QFont::StyleOblique);
        }
    }
    if (m_fontStack.top()->GetWeight() != FONTWEIGHT_NONE) {
        if (m_fontStack.top()->GetWeight() == FONTWEIGHT_bold) {
            font.setWeight(QFont::Bold);
        }
    }

    m_currentTextQuickItem->appendText(QString::fromStdString(text), font);
}

void QtSceneGraphDeviceContext::EndText()
{
    Q_ASSERT(m_currentTextQuickItem != nullptr);

    m_currentTextQuickItem->calcPos();

    AddQuickItem(m_currentTextQuickItem);
    m_currentTextQuickItem = nullptr;
}

void QtSceneGraphDeviceContext::DrawMusicText(const std::wstring &text, int x, int y, bool)
{
    Q_ASSERT(m_fontStack.top());

    QFont font(QString::fromStdString(m_fontStack.top()->GetFaceName()));

    // Note: Verovio calls it point size but it is actually pixel size. Qt can only handle pixel size in int, thus we
    // convert the pixel size to point size.
    double pointSize = static_cast<double>(translate(ConvertPixelSizeToPointSize(m_fontStack.top()->GetPointSize())));
    font.setPointSizeF(pointSize);

    // Memory management is handled by m_quickItem (set in AddQuickItem)
    auto musicTextQuickItem = new TextQuickItem();

    musicTextQuickItem->appendText(QString::fromStdWString(text), font);

    musicTextQuickItem->setX(static_cast<double>(translateX(x)));
    musicTextQuickItem->setY(static_cast<double>(translateY(y)));
    musicTextQuickItem->calcPos();

    AddQuickItem(musicTextQuickItem);
}

void QtSceneGraphDeviceContext::DrawSpline(int, Point[])
{
    // This function is also not implemented for SvgDeviceContext
}

void QtSceneGraphDeviceContext::DrawBackgroundImage(int, int)
{
    // This function is also not implemented for SvgDeviceContext
}

void QtSceneGraphDeviceContext::MoveTextTo(int, int)
{
    // This function is also not implemented for SvgDeviceContext
}

void QtSceneGraphDeviceContext::StartGraphic(Object *object, std::string, std::string gId)
{
    m_activeGraphicObjectsStack.push(ActiveGraphic(QString::fromStdString(gId), object));
}

void QtSceneGraphDeviceContext::EndGraphic(Object *, View *)
{
    m_activeGraphicObjectsStack.pop();
}

void QtSceneGraphDeviceContext::ResumeGraphic(Object *object, std::string gId)
{
    m_activeGraphicObjectsStack.push(ActiveGraphic(QString::fromStdString(gId), object));
}

void QtSceneGraphDeviceContext::EndResumedGraphic(Object *, View *)
{
    m_activeGraphicObjectsStack.pop();
}

void QtSceneGraphDeviceContext::RotateGraphic(const Point &, double)
{
    qWarning() << "Warning:" << __FUNCTION__ << "not supported";
}

void QtSceneGraphDeviceContext::StartPage()
{
    // No action required
}

void QtSceneGraphDeviceContext::EndPage()
{
    // No action required
}
} // namespace vrv
