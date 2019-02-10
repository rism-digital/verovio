/////////////////////////////////////////////////////////////////////////////
// Name:        scenegraphdevicecontext.h
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_QT_SCENEGRAPH_DC_H__
#define __VRV_QT_SCENEGRAPH_DC_H__

//----------------------------------------------------------------------------

#include <QQuickItem>
#include <QSGNode>
#include <QStack>
#include <QStringList>

//----------------------------------------------------------------------------

#include "devicecontext.h"

namespace vrvQt {

class TextQuickItem;

//----------------------------------------------------------------------------
// SceneGraphDeviceContext
//----------------------------------------------------------------------------

/**
 * This class implements a drawing context for the Qt scene graph. This allows
 * to create a QQuickItem that can be used in QML.
 */
class SceneGraphDeviceContext : public vrv::DeviceContext {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    SceneGraphDeviceContext(QQuickItem *quickItem, QSGNode *node);
    virtual ~SceneGraphDeviceContext() = default;
    vrv::ClassId GetClassId() const override { return vrv::CUSTOM_DEVICE_CONTEXT; }
    void Clear();
    ///@}

    /**
     * @name Getters for retrieving the scaled height and width of this device context.
     */
    ///@{
    int GetScaledHeight() { return static_cast<int>(GetHeight() * GetUserScaleY()); }
    int GetScaledWidth() { return static_cast<int>(GetWidth() * GetUserScaleX()); }
    ///@}

    /**
     * @name Methods that allow a mapping between object IDs and graphical items.
     */
    ///@{
    QList<QSGGeometryNode *> GetGeometryNodesForId(QString id);
    QList<TextQuickItem *> GetQuickItemsForId(QString id);
    QStringList GetIdsForQuickItem(QQuickItem *item);
    ///@}

    /*** DeviceContext interface ***/

    /**
     * @name Setters
     */
    ///@{
    void SetBackground(int colour, int style) override;
    void SetBackgroundImage(void *image, double opacity) override;
    void SetBackgroundMode(int mode) override;
    void SetTextForeground(int colour) override;
    void SetTextBackground(int colour) override;
    void SetLogicalOrigin(int x, int y) override;
    ///@}

    /**
     * @name Getters
     */
    ///@{
    vrv::Point GetLogicalOrigin() override;
    ///}

    /**
     * @name Drawing methods
     */
    ///@{
    void DrawComplexBezierPath(vrv::Point bezier1[4], vrv::Point bezier2[4]) override;
    void DrawCircle(int x, int y, int radius) override;
    void DrawEllipse(int x, int y, int width, int height) override;
    void DrawEllipticArc(int x, int y, int width, int height, double start, double end) override;
    void DrawLine(int x1, int y1, int x2, int y2) override;
    void DrawPolygon(
        int n, vrv::Point points[], int xoffset, int yoffset, int fill_style = vrv::AxODDEVEN_RULE) override;
    void DrawRectangle(int x, int y, int width, int height) override;
    void DrawRotatedText(const std::string &text, int x, int y, double angle) override;
    void DrawRoundedRectangle(int x, int y, int width, int height, double radius) override;
    void DrawText(
        const std::string &text, const std::wstring wtext = L"", int x = VRV_UNSET, int y = VRV_UNSET) override;
    void DrawMusicText(const std::wstring &text, int x, int y, bool setSmuflGlyph) override;
    void DrawSpline(int n, vrv::Point points[]) override;
    void DrawSvgShape(int x, int y, int width, int height, pugi::xml_node svg) override;
    void DrawBackgroundImage(int x = 0, int y = 0) override;
    ///@}

    /**
     * @name Method for starting, ending and moving a text
     */
    ///@{
    void StartText(int x, int y, vrv::data_HORIZONTALALIGNMENT alignment = vrv::HORIZONTALALIGNMENT_left) override;
    void EndText() override;
    void MoveTextTo(int x, int y, vrv::data_HORIZONTALALIGNMENT alignment) override;
    ///@}

    /**
     * @name Method for starting, restarting and ending a graphic
     */
    ///@{
    void StartGraphic(vrv::Object *object, std::string gClass, std::string gId) override;
    void EndGraphic(vrv::Object *object, vrv::View *view) override;
    void ResumeGraphic(vrv::Object *object, std::string gId) override;
    void EndResumedGraphic(vrv::Object *object, vrv::View *view) override;
    ///@}

    /**
     * @name Method for rotating a graphic (clockwise).
     */
    ///@{
    void RotateGraphic(const vrv::Point &orig, double angle) override;
    ///@}

    /**
     * @name Method for starting and ending page
     */
    ///@{
    void StartPage() override;
    void EndPage() override;
    ///@}

private:
    /**
     * Helper function that sets the position and alignment of the current text item.
     */
    void SetTextPositionAndAlignment(int x, int y, vrv::data_HORIZONTALALIGNMENT alignment);

    /**
     * Helper function that allows converting a pixel size to a point size, as used for the font size.
     */
    float ConvertPixelSizeToPointSize(int pixel) { return pixel * 72 / m_dpi; }

    /**
     * @name Add a graphical item to the scene graph.
     */
    ///@{
    void AddGeometryNode(QSGGeometryNode *node);
    void AddQuickItem(TextQuickItem *item);
    ///@}

    /**
     * @name Functions used to translate from the logical to device coordinates.
     */
    ///@{
    template <class T> float translate(T x)
    {
        // User-scale: Everything has to be scaled, thus it is not possible to differentiate between UserScaleX and
        // UserScaleY. If there is no requirement in other DeviceContexts to have different user-scales it makes sense
        // to merge the two values to a single user-scale.
        return x / DEFINITION_FACTOR * GetUserScaleX();
    }
    template <class T> float translateX(T x) { return translate(x + m_logicalOrigin.x); }
    template <class T> float translateY(T y) { return translate(y + m_logicalOrigin.y); }
    ///@}

private:
    // All graphical items are either added as a childItem to m_quickItem or as a childNode to m_node:
    //  - text items are added as QQuickItem
    //  - geometric objects (e.g. lines, rects) are added as QSGNodes.
    QQuickItem *m_quickItem{ nullptr };
    QSGNode *m_node{ nullptr };

    struct ActiveGraphic {
        ActiveGraphic() : object(nullptr) {}
        ActiveGraphic(QString id_, vrv::Object *object_) : id(id_), object(object_) {}
        QString id;
        vrv::Object *object;
    };
    QStack<ActiveGraphic> m_activeGraphicObjectsStack;

    TextQuickItem *m_currentTextQuickItem{ nullptr };
    vrv::Point m_logicalOrigin{ 0, 0 };
    float m_dpi{ 0 };

    // datastructures for mapping object ids to graphical items
    QMap<QString, QList<QSGGeometryNode *> > m_id2NodeMapping;
    QMap<QString, QList<TextQuickItem *> > m_id2QuickItemMapping;
};
} // namespace vrvQt

#endif // __VRV_QT_SCENEGRAPH_DC_H__
