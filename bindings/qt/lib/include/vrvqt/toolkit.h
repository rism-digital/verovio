/////////////////////////////////////////////////////////////////////////////
// Name:        toolkit.h
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_QT_TOOLKIT_H__
#define __VRV_QT_TOOLKIT_H__

//----------------------------------------------------------------------------

#include <QObject>
#include <QVector>

//----------------------------------------------------------------------------

#include "vrv/toolkit.h"

namespace vrvQt {

//----------------------------------------------------------------------------
// vrvQt Toolkit
//----------------------------------------------------------------------------

/**
 * This class is a wrapper around the Verovio Toolkit (vrv::Toolkit).
 */
class Toolkit : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString resourcesDataPath MEMBER m_resourcesDataPath WRITE setResourcesDataPath)
    Q_PROPERTY(QString fileName MEMBER m_fileName WRITE setFileName)
    Q_PROPERTY(QString fileContent MEMBER m_fileContent WRITE setFileContent)
    Q_PROPERTY(QString musicFontName MEMBER m_musicFontName WRITE setMusicFontName)
    Q_PROPERTY(QString musicFontPath MEMBER m_musicFontPath WRITE setMusicFontPath)
    Q_PROPERTY(QString verovioTextFontPath MEMBER m_verovioTextFontPath WRITE setVerovioTextFontPath)
    Q_PROPERTY(QString breaks WRITE setBreaks READ getBreaks)
    Q_PROPERTY(int pageCount MEMBER m_pageCount READ pageCount NOTIFY pageCountChanged)
    Q_PROPERTY(int displayWidth MEMBER m_displayWidth WRITE setDisplayWidth)
    Q_PROPERTY(int displayHeight MEMBER m_displayHeight WRITE setDisplayHeight)
    Q_PROPERTY(int scale WRITE setScale READ getScale)
    Q_PROPERTY(int spacingStaff WRITE setSpacingStaff READ getSpacingStaff)
    Q_PROPERTY(int spacingSystem WRITE setSpacingSystem READ getSpacingSystem)
    Q_PROPERTY(bool adjustPageHeight WRITE setAdjustPageHeight READ getAdjustPageHeight)
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
     * @name Getter/setter for the adjusted display height. This returns the normal display height if adjustPageHeight
     * is deactivated.
     */
    ///@{
    Q_INVOKABLE int adjustedDisplayHeightForPage(int page);
    void setAdjustedDisplayHeightForPage(int pageNumber, int displayHeight);
    ///@}

    /**
     * @name Getter for the page count.
     */
    int pageCount() const { return m_pageCount; }

    /**
     * @name Render the given page of the currently set document to the specified deviceContext.
     */
    void renderPage(int page, vrv::DeviceContext *deviceContext);

    /**
     * @name Returns if the toolkit has valid data to show.
     */
    bool hasValidData() const { return m_hasValidData; }

    /**
     * @name Getters for public properties.
     */
    ///@{
    int getScale();
    bool getAdjustPageHeight() const;
    int getSpacingSystem() const;
    int getSpacingStaff() const;
    QString getBreaks() const;
    ///@}

public slots:
    /**
     * @name Setters for public properties.
     */
    ///@{
    void setResourcesDataPath(QString resourcesDataPath);
    void setFileName(QString fileName);
    void setFileContent(QString fileContent);
    void setMusicFontName(QString musicFontName);
    void setMusicFontPath(QString musicFontPath);
    void setVerovioTextFontPath(QString verovioTextFontPath);
    void setBreaks(QString breaks);
    void setDisplayWidth(int displayWidth);
    void setDisplayHeight(int displayHeight);
    void setScale(int scale);
    void setSpacingStaff(int spacingStaff);
    void setSpacingSystem(int spacingSystem);
    void setAdjustPageHeight(bool adjustPageHeight);
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
     * @name Initialise required fonts.
     */
    ///@{
    bool initFont();
    bool addFont(QString fontFilePath);
    ///@}

    /**
     * @name Setter for hasValidData.
     */
    void setHasValidData(bool hasValidData);

private:
    // Stores the dimensions to which the score should be rendered. This takes into account the current scale and thus
    // differs to the Verovio pageWidth and pageHeight.
    int m_displayWidth{ 0 };
    int m_displayHeight{ 0 };

    int m_pageCount{ 0 };

    // adjusted display heights for the currently layouted document
    QVector<int> m_adjustedDisplayHeights;

    // font name for the music symbols (e.g. notes)
    QString m_musicFontName;

    // font path to the music symbols font
    QString m_musicFontPath;

    // font path to the verovio text font (required e.g. for # in harmonies)
    QString m_verovioTextFontPath;

    // path to the resource dir
    QString m_resourcesDataPath;

    QString m_fileName;
    QString m_fileContent;

    // flags to store the state of the vrvQt toolkit
    bool m_documentRelayoutRequested{ false };
    bool m_reloadDataRequested{ false };
    bool m_readFileRequested{ false };
    bool m_fontInitDone{ false };
    bool m_hasValidData{ false };
    bool m_resourcesDataInitialized{ false };
};
} // namespace vrvQt
#endif // __VRV_QT_TOOLKIT_H__
