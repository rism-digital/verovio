/////////////////////////////////////////////////////////////////////////////
// Name:        veroviodoc.h
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VEROVIOQTTOOLKIT_H__
#define __VEROVIOQTTOOLKIT_H__

//----------------------------------------------------------------------------

#include <QObject>
#include <QVector>

//----------------------------------------------------------------------------

#include "toolkit.h"

namespace vrv_qt {

//----------------------------------------------------------------------------
// VerovioDoc
//----------------------------------------------------------------------------

/**
 * This class is a wrapper around the Verovio Toolkit (vrv::Toolkit).
 */
class Toolkit : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString resourcesDataPath MEMBER m_resourcesDataPath WRITE setResourcesDataPath)
    Q_PROPERTY(QString fontDirPath MEMBER m_fontDirPath WRITE setFontDirPath)
    Q_PROPERTY(QString fileName MEMBER m_fileName WRITE setFileName)
    Q_PROPERTY(QString fileContent MEMBER m_fileContent WRITE setFileContent)
    Q_PROPERTY(QString musicFont MEMBER m_musicFont WRITE setMusicFont)
    Q_PROPERTY(int pageCount MEMBER m_pageCount READ pageCount NOTIFY pageCountChanged)
    Q_PROPERTY(int displayWidth MEMBER m_displayWidth WRITE setDisplayWidth)
    Q_PROPERTY(int displayHeight MEMBER m_displayHeight WRITE setDisplayHeight)
    Q_PROPERTY(int scale WRITE setScale READ getScale)
    Q_PROPERTY(int border WRITE setBorder)
    Q_PROPERTY(int spacingStaff WRITE setSpacingStaff)
    Q_PROPERTY(int spacingSystem WRITE setSpacingSystem)
    Q_PROPERTY(bool adjustPageHeight WRITE setAdjustPageHeight)
    Q_PROPERTY(bool noLayout WRITE setNoLayout)
    Q_PROPERTY(bool ignoreLayout WRITE setIgnoreLayout)
    Q_PROPERTY(bool hasValidData MEMBER m_hasValidData READ hasValidData NOTIFY hasValidDataChanged)

public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    Toolkit();
    virtual ~Toolkit() = default;
    ///@}

    /**
     * @name Getter/setter for the adjusted page height if the adjustPageHeight is activated.
     */
    ///@{
    Q_INVOKABLE int adjustedPageHeightForPage(int page);
    void setAdjustedPageHeightForPage(int pageNumber, int displayHeight);
    ///@}

    /**
     * @name Getter for the page count.
     */
    int pageCount() const { return m_pageCount; }

    /**
     * @name Render the given page of this document to the specified deviceContext.
     */
    void renderPage(int page, vrv::DeviceContext *deviceContext);

    /**
     * @name Returns if the document has valid data to show.
     */
    bool hasValidData() const { return m_hasValidData; }

    /**
     * @name Returns the current scale of the document.
     */
    int getScale() { return m_verovioToolkit.GetScale(); }

public slots:
    /**
     * @name Setters for public properties.
     */
    ///@{
    void setResourcesDataPath(QString resourcesDataPath);
    void setFontDirPath(QString fontDirPath);
    void setFileName(QString fileName);
    void setFileContent(QString fileContent);
    void setMusicFont(QString musicFont);
    void setDisplayWidth(int displayWidth);
    void setDisplayHeight(int displayHeight);
    void setScale(int scale);
    void setBorder(int border);
    void setSpacingStaff(int spacingStaff);
    void setSpacingSystem(int spacingSystem);
    void setAdjustPageHeight(bool adjustPageHeight);
    void setNoLayout(bool noLayout);
    void setIgnoreLayout(bool ignoreLayout);
    ///@}

signals:
    /**
     * @name This signal is emitted if the document layout changed. VerovioPages attach to this signal to update the
     * scene graph.
     */
    void documentLayoutChanged();

    /**
     * @name These signals are used internally to notify that the document layout has to be done or the file content has
     * to be read.
     */
    ///@{
    void documentLayoutInvalidated();
    void fileContentInvalidated();
    void fileNameInvalidated();
    ///@}

    /**
     * @name Signals for Qt properties.
     */
    ///@{
    void pageCountChanged(int pageCount);
    void hasValidDataChanged(bool hasValidData);
    ///@}

protected:
    // instance of the verovio Toolkit class used to communicate to the C++ library.
    vrv::Toolkit m_verovioToolkit;

private slots:
    /**
     * @name Loads or reloads the data stored in the m_content member variable.
     */
    void reloadData();

    /**
     * @name Read the content of the file specified by the fileName property.
     */
    void readFile();

    /**
     * @name Worker function that recalculates the document layout. This function is called asynchronously if
     * requestDocumentLayout() is called.
     */
    void documentRelayout();

private:
    /**
     * @name These function trigger an asynchronous read file, reload data or document relayout request.
     */
    ///@{
    void requestReadFile();
    void requestReloadData();
    void requestDocumentRelayout();
    ///@}

    /**
     * @name Internal setter for the page count.
     */
    void setPageCount(int pageCount);

    /**
     * @name Initialies required fonts.
     */
    void initFont();

    /**
     * @name Setter for hasValidData.
     */
    void setHasValidData(bool hasValidData);

private:
    // Stores the dimensions to which the score should be rendered. This takes into account the current scale and thus
    // differs to the Verovio pageWidth and pageHeight.
    int m_displayWidth;
    int m_displayHeight;

    int m_pageCount{ 0 };

    // adjusted page heights for the currently layouted document
    QVector<int> m_adjustedPageHeights;

    // font name for the music symbols (e.g. notes)
    QString m_musicFont;

    // paths to the resource and font dirs
    QString m_resourcesDataPath;
    QString m_fontDirPath;

    QString m_fileName;
    QString m_fileContent;

    // flags to store state of document
    bool m_documentRelayoutRequested{ false };
    bool m_reloadDataRequested{ false };
    bool m_readFileRequested{ false };
    bool m_fontInitDone{ false };
    bool m_hasValidData{ false };
};
} // namespace vrv_qt
#endif // __VEROVIOQTTOOLKIT_H__
