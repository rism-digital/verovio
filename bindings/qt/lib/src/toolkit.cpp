/////////////////////////////////////////////////////////////////////////////
// Name:        toolkit.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrvqt/toolkit.h"

//----------------------------------------------------------------------------

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QUrl>

namespace vrvQt {
Toolkit::Toolkit()
    : m_verovioToolkit(false)
    , m_displayWidth(std::stoi(m_verovioToolkit.GetOption("pageWidth")))
    , m_displayHeight(std::stoi(m_verovioToolkit.GetOption("pageHeight")))
{
    connect(this, SIGNAL(documentLayoutInvalidated()), this, SLOT(documentRelayout()), Qt::QueuedConnection);
    connect(this, SIGNAL(fileNameInvalidated()), this, SLOT(readFile()), Qt::QueuedConnection);
    connect(this, SIGNAL(fileContentInvalidated()), this, SLOT(reloadData()), Qt::QueuedConnection);
}

int Toolkit::adjustedDisplayHeightForPage(int page)
{
    if (!getAdjustPageHeight()) return m_displayHeight;

    Q_ASSERT(page <= m_adjustedDisplayHeights.count());

    if (m_adjustedDisplayHeights[page - 1] != 0) {
        return m_adjustedDisplayHeights[page - 1];
    }
    else {
        int averagePageHeight = 0;
        int defaultPageHeight = 1000;
        int num = 0;
        for (auto height : m_adjustedDisplayHeights) {
            if (height != 0) {
                averagePageHeight += height;
                num++;
            }
        }
        if (num == 0) return defaultPageHeight;
        return averagePageHeight / num;
    }
}

void Toolkit::setAdjustedDisplayHeightForPage(int pageNumber, int pageHeight)
{
    if (getAdjustPageHeight()) {
        Q_ASSERT(pageNumber <= m_adjustedDisplayHeights.count());
        m_adjustedDisplayHeights[pageNumber - 1] = pageHeight;
    }
}

void Toolkit::renderPage(int page, vrv::DeviceContext *deviceContext)
{
    if (m_hasValidData) {
        m_verovioToolkit.RenderToDeviceContext(page, deviceContext);
    }
}

void Toolkit::setHasValidData(bool hasValidData)
{
    if (m_hasValidData != hasValidData) {
        m_hasValidData = hasValidData;
        emit hasValidDataChanged(hasValidData);
    }
}

void Toolkit::setPageCount(int pageCount)
{
    if (m_pageCount != pageCount) {
        m_pageCount = pageCount;
        m_adjustedDisplayHeights.fill(0, pageCount);

        emit pageCountChanged(pageCount);
    }
}

void Toolkit::setFileName(QString fileName)
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

void Toolkit::setMusicFontName(QString musicFont)
{
    if (m_musicFontName != musicFont) {
        m_musicFontName = musicFont;
        m_fontInitDone = false;
        initFont();
        requestDocumentRelayout();
    }
}

void Toolkit::setMusicFontPath(QString musicFontPath)
{
    if (m_musicFontPath != musicFontPath) {
        m_musicFontPath = musicFontPath;
        m_fontInitDone = false;
        initFont();
        requestDocumentRelayout();
    }
}

void Toolkit::setVerovioTextFontPath(QString verovioTextFontPath)
{
    if (m_verovioTextFontPath != verovioTextFontPath) {
        m_verovioTextFontPath = verovioTextFontPath;
        m_fontInitDone = false;
        initFont();
        requestDocumentRelayout();
    }
}

void Toolkit::setDisplayWidth(int pageWidth)
{
    if (m_displayWidth != pageWidth) {
        m_displayWidth = pageWidth;
        requestDocumentRelayout();
    }
}

void Toolkit::setDisplayHeight(int pageHeight)
{
    if (m_displayHeight != pageHeight) {
        m_displayHeight = pageHeight;
        requestDocumentRelayout();
    }
}

void Toolkit::setScale(int scale)
{
    if (m_verovioToolkit.GetScale() != scale) {
        m_verovioToolkit.SetScale(scale);
        requestDocumentRelayout();
    }
}

void Toolkit::setAdjustPageHeight(bool adjustPageHeight)
{
    if (getAdjustPageHeight() != adjustPageHeight) {
        m_verovioToolkit.SetOption("adjustPageHeight", adjustPageHeight ? "true" : "false");
        requestDocumentRelayout();
    }
}

void Toolkit::setBreaks(QString breaks)
{
    if (getBreaks() != breaks) {
        m_verovioToolkit.SetOption("breaks", breaks.toStdString());
        // "breaks" is used in LoadData
        requestReloadData();
    }
}

void Toolkit::setFileContent(QString fileContent)
{
    if (m_fileContent != fileContent) {
        m_fileContent = fileContent;
        requestReloadData();
    }
}

void Toolkit::setResourcesDataPath(QString resourcesDataPath)
{
    if (m_resourcesDataPath != resourcesDataPath) {
        m_resourcesDataPath = resourcesDataPath;
        bool success = m_verovioToolkit.SetResourcePath(resourcesDataPath.toStdString());

        if (!success) {
            m_resourcesDataInitialized = false;
            qWarning() << "The music font could not be loaded; please check the contents of the resource directory.";
            return;
        }
        m_resourcesDataInitialized = true;
        requestReloadData();
    }
}

void Toolkit::setSpacingStaff(int spacingStaff)
{
    if (getSpacingStaff() != spacingStaff) {
        m_verovioToolkit.SetOption("spacingStaff", std::to_string(spacingStaff));
        requestDocumentRelayout();
    }
}

void Toolkit::setSpacingSystem(int spacingSystem)
{
    if (getSpacingSystem() != spacingSystem) {
        m_verovioToolkit.SetOption("spacingSystem", std::to_string(spacingSystem));
        requestDocumentRelayout();
    }
}

bool Toolkit::getAdjustPageHeight() const
{
    return m_verovioToolkit.GetOption("adjustPageHeight") == "true";
}

int Toolkit::getScale()
{
    return m_verovioToolkit.GetScale();
}

int Toolkit::getSpacingSystem() const
{
    return std::stoi(m_verovioToolkit.GetOption("spacingSystem"));
}

int Toolkit::getSpacingStaff() const
{
    return std::stoi(m_verovioToolkit.GetOption("spacingStaff"));
}

QString Toolkit::getBreaks() const
{
    return QString::fromStdString(m_verovioToolkit.GetOption("spacingStaff"));
}

bool Toolkit::addFont(QString fontFilePath)
{
    int fontId = QFontDatabase::addApplicationFont(fontFilePath);
    if (fontId == -1) {
        QFileInfo fontFile(fontFilePath);
        if (!fontFile.exists() || !fontFile.isFile()) {
            qWarning() << "Font file" << fontFilePath << "does not exist!";
        }
        else {
            qWarning() << "Could not add font" << fontFilePath << "(file exists)";
        }
        return false;
    }
    return true;
}

bool Toolkit::initFont()
{
    if (m_musicFontName.isEmpty() || m_musicFontPath.isEmpty() || m_verovioTextFontPath.isEmpty()) {
        return false;
    }

    if (m_fontInitDone) return true;
    m_fontInitDone = true;

    m_verovioToolkit.SetOption("font", m_musicFontName.toStdString());

    addFont(m_musicFontPath);
    addFont(m_verovioTextFontPath);

    return true;
}

void Toolkit::requestReadFile()
{
    if (!m_readFileRequested) {
        m_readFileRequested = true;
        emit fileNameInvalidated();
    }
}

void Toolkit::requestReloadData()
{
    if (!m_reloadDataRequested) {
        m_reloadDataRequested = true;
        emit fileContentInvalidated();
    }
}

void Toolkit::requestDocumentRelayout()
{
    if (!m_documentRelayoutRequested) {
        m_documentRelayoutRequested = true;
        emit documentLayoutInvalidated();
    }
}

void Toolkit::readFile()
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

void Toolkit::reloadData()
{
    m_reloadDataRequested = false;

    if (!m_resourcesDataInitialized) return;

    bool success = m_verovioToolkit.LoadData(m_fileContent.toStdString());
    setHasValidData(success);

    if (success) {
        requestDocumentRelayout();
    }
    else {
        setPageCount(0);
    }
}

void Toolkit::documentRelayout()
{
    m_documentRelayoutRequested = false;

    if (!m_hasValidData) {
        return;
    }

    if (!initFont()) {
        qWarning() << "Could not layout document because fonts are not correctly initialized";
        return;
    }

    m_verovioToolkit.SetOption(
        "pageWidth", std::to_string(static_cast<int>(m_displayWidth * 100.0 / m_verovioToolkit.GetScale())));
    m_verovioToolkit.SetOption(
        "pageHeight", std::to_string(static_cast<int>(m_displayHeight * 100.0 / m_verovioToolkit.GetScale())));

    m_verovioToolkit.RedoLayout();

    setPageCount(m_verovioToolkit.GetPageCount());

    emit documentLayoutChanged();
}

} // namespace vrvQt
