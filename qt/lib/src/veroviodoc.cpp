/////////////////////////////////////////////////////////////////////////////
// Name:        veroviodoc.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "veroviodoc.h"

//----------------------------------------------------------------------------

#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QUrl>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {
VerovioDoc::VerovioDoc()
    : m_verovioToolkit(false)
    , m_pageWidth(m_verovioToolkit.GetPageWidth())
    , m_pageHeight(m_verovioToolkit.GetPageHeight())
    , m_scale(m_verovioToolkit.GetScale())
    , m_border(m_verovioToolkit.GetBorder())
    , m_spacingStaff(m_verovioToolkit.GetSpacingStaff())
    , m_spacingSystem(m_verovioToolkit.GetSpacingSystem())
    , m_adjustPageHeight(m_verovioToolkit.GetAdjustPageHeight())
    , m_noLayout(m_verovioToolkit.GetNoLayout())
    , m_ignoreLayout(m_verovioToolkit.GetIgnoreLayout())
    , m_musicFont("Leipzig")
{
    connect(this, SIGNAL(documentLayoutInvalidated()), this, SLOT(documentRelayout()), Qt::QueuedConnection);
    connect(this, SIGNAL(fileNameInvalidated()), this, SLOT(readFile()), Qt::QueuedConnection);
    connect(this, SIGNAL(fileContentInvalidated()), this, SLOT(reloadData()), Qt::QueuedConnection);
}

int VerovioDoc::adjustedPageHeightForPage(int page)
{
    if (!m_adjustPageHeight) return m_pageHeight;

    Q_ASSERT(page <= m_adjustedPageHeights.count());

    int defaultPageHeight = 1000;

    if (m_adjustedPageHeights[page - 1] != 0) {
        return m_adjustedPageHeights[page - 1];
    }
    else {
        int averagePageHeight = 0;
        int num = 0;
        for (auto height : m_adjustedPageHeights) {
            if (height != 0) {
                averagePageHeight += height;
                num++;
            }
        }
        if (num == 0) return defaultPageHeight;
        return averagePageHeight / num;
    }
}

void VerovioDoc::setAdjustedPageHeightForPage(int pageNumber, int pageHeight)
{
    if (m_adjustPageHeight) {
        Q_ASSERT(pageNumber <= m_adjustedPageHeights.count());
        m_adjustedPageHeights[pageNumber - 1] = pageHeight;
    }
}

void VerovioDoc::renderPage(int page, DeviceContext *deviceContext)
{
    if (m_hasValidData) {
        m_verovioToolkit.RenderToCustomDevice(page, deviceContext);
    }
}

void VerovioDoc::setHasValidData(bool hasValidData)
{
    if (m_hasValidData != hasValidData) {
        m_hasValidData = hasValidData;
        emit hasValidDataChanged(hasValidData);
    }
}

void VerovioDoc::setPageCount(int pageCount)
{
    if (m_pageCount != pageCount) {
        m_pageCount = pageCount;
        m_adjustedPageHeights.fill(0, pageCount);

        emit pageCountChanged(pageCount);
    }
}

void VerovioDoc::setFileName(QString fileName)
{
    // file-names from QML start with file://
    if (fileName.startsWith("file://")) {
        fileName = QUrl(fileName).toLocalFile();
    }

    if (m_fileName != fileName) {
        m_fileName = fileName;
        requestReadFile();
    }
}

void VerovioDoc::setMusicFont(QString musicFont)
{
    if (m_musicFont != musicFont) {
        m_musicFont = musicFont;
        m_fontInitDone = false;
        initFont();
        requestDocumentRelayout();
    }
}

void VerovioDoc::setPageWidth(int pageWidth)
{
    if (m_pageWidth != pageWidth) {
        m_pageWidth = pageWidth;
        requestDocumentRelayout();
    }
}

void VerovioDoc::setPageHeight(int pageHeight)
{
    if (m_pageHeight != pageHeight) {
        m_pageHeight = pageHeight;
        requestDocumentRelayout();
    }
}

void VerovioDoc::setScale(int scale)
{
    if (m_scale != scale) {
        m_scale = scale;
        requestDocumentRelayout();
    }
}

void VerovioDoc::setBorder(int border)
{
    if (m_border != border) {
        m_border = border;
        m_verovioToolkit.SetAdjustPageHeight(border);
        requestDocumentRelayout();
    }
}

void VerovioDoc::setAdjustPageHeight(bool adjustPageHeight)
{
    if (m_adjustPageHeight != adjustPageHeight) {
        m_adjustPageHeight = adjustPageHeight;
        m_verovioToolkit.SetAdjustPageHeight(adjustPageHeight);
        requestDocumentRelayout();
    }
}

void VerovioDoc::setNoLayout(bool noLayout)
{
    if (m_noLayout != noLayout) {
        m_noLayout = noLayout;
        m_verovioToolkit.SetNoLayout(noLayout);
        // "no layout" is used in LoadData
        requestReloadData();
    }
}

void VerovioDoc::setIgnoreLayout(bool ignoreLayout)
{
    if (m_ignoreLayout != ignoreLayout) {
        m_ignoreLayout = ignoreLayout;
        m_verovioToolkit.SetIgnoreLayout(ignoreLayout);
        // "ignore layout" is used in LoadData
        requestReloadData();
    }
}

void VerovioDoc::setFileContent(QString fileContent)
{
    if (m_fileContent != fileContent) {
        m_fileContent = fileContent;
        requestReloadData();
    }
}

void VerovioDoc::setResourcesDataPath(QString resourcesDataPath)
{
    if (m_resourcesDataPath != resourcesDataPath) {
        m_resourcesDataPath = resourcesDataPath;
        Resources::SetPath(resourcesDataPath.toStdString());

        if (!Resources::InitFonts()) {
            qWarning() << "The music font could not be loaded; please check the contents of the resource directory.";
            exit(1);
        }
        requestDocumentRelayout();
    }
}

void VerovioDoc::setFontDirPath(QString fontDirPath)
{
    if (m_fontDirPath != fontDirPath) {
        m_fontDirPath = fontDirPath;
        requestDocumentRelayout();
    }
}

void VerovioDoc::setSpacingStaff(int spacingStaff)
{
    if (m_spacingStaff != spacingStaff) {
        m_spacingStaff = spacingStaff;
        m_verovioToolkit.SetSpacingStaff(spacingStaff);
        requestDocumentRelayout();
    }
}

void VerovioDoc::setSpacingSystem(int spacingSystem)
{
    if (m_spacingSystem != spacingSystem) {
        m_spacingSystem = spacingSystem;
        m_verovioToolkit.SetSpacingSystem(spacingSystem);
        requestDocumentRelayout();
    }
}

void VerovioDoc::initFont()
{
    if (m_fontInitDone) return;
    m_fontInitDone = true;

    m_verovioToolkit.SetFont(m_musicFont.toStdString());

    if (m_musicFont == "Bravura")
        QFontDatabase::addApplicationFont(m_fontDirPath + "/Bravura-1.204.otf");
    else if (m_musicFont == "Leipzig")
        qDebug() << "Leipzig" << QFontDatabase::addApplicationFont(m_fontDirPath + "/Leipzig-5.2.ttf");
    else if (m_musicFont == "Gootville")
        QFontDatabase::addApplicationFont(m_fontDirPath + "/Gootville-1.2.otf");
    else
        qWarning() << "Using currently unsupported font:" << m_musicFont;

    // Always add VerovioText (required e.g. for # in harmonies)
    QFontDatabase::addApplicationFont(m_fontDirPath + "/VerovioText-1.0.ttf");
}

void VerovioDoc::requestReadFile()
{
    if (!m_readFileRequested) {
        m_readFileRequested = true;
        emit fileNameInvalidated();
    }
}

void VerovioDoc::requestReloadData()
{
    if (!m_reloadDataRequested) {
        m_reloadDataRequested = true;
        emit fileContentInvalidated();
    }
}

void VerovioDoc::requestDocumentRelayout()
{
    if (!m_documentRelayoutRequested) {
        m_documentRelayoutRequested = true;
        emit documentLayoutInvalidated();
    }
}

void VerovioDoc::readFile()
{
    bool success = false;
    m_readFileRequested = false;

    QFile inputFile(m_fileName);
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&inputFile);
        m_fileContent = stream.readAll();
        success = true;
    }

    if (success) {
        requestReloadData();
    }
}

void VerovioDoc::reloadData()
{
    m_reloadDataRequested = false;

    bool success = m_verovioToolkit.LoadData(m_fileContent.toStdString());
    setHasValidData(success);

    if (success) {
        requestDocumentRelayout();
    }
    else {
        setPageCount(0);
    }
}

void VerovioDoc::documentRelayout()
{
    m_documentRelayoutRequested = false;

    if (!m_hasValidData) {
        return;
    }

    initFont();

    m_verovioToolkit.SetScale(m_scale);
    m_verovioToolkit.SetPageWidth(static_cast<int>(m_pageWidth * 100.0 / m_scale));
    m_verovioToolkit.SetPageHeight(static_cast<int>(m_pageHeight * 100.0 / m_scale));

    m_verovioToolkit.RedoLayout();

    setPageCount(m_verovioToolkit.GetPageCount());

    emit documentLayoutChanged();
}
} // namespace vrv
