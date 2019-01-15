/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QQuickView>
#include <QSettings>

//----------------------------------------------------------------------------

#include "vrvqt/toolkit.h"
#include "vrvqt/view.h"

/**
 * Retrieves GL_MAX_SAMPLES by creating an offscreen surface.
 */
int maxOpenGlSamples()
{
    QOffscreenSurface surface;
    surface.create();
    QOpenGLContext context;
    if (!context.create()) {
        return 0;
    }
    if (!context.makeCurrent(&surface)) {
        return 0;
    }

    int maxSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    context.doneCurrent();
    return maxSamples;
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
    // From Qt doc: Calling QSurfaceFormat::setDefaultFormat() before constructing the QApplication instance is
    // mandatory on some platforms (for example, macOS) when an OpenGL core profile context is requested.
    QSurfaceFormat format;
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);
#endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Verovio");
    app.setOrganizationDomain("verovio.org");
    app.setApplicationName("Verovio Qt Demo");
    QSettings settings;

    // Register custom QML types for Verovio
    qmlRegisterType<vrvQt::View>("VerovioQml", 1, 0, "VerovioView");
    qmlRegisterType<vrvQt::Toolkit>("VerovioQml", 1, 0, "VerovioToolkit");

    QQuickView view;

// Activate multisample antialiasing
#ifndef Q_OS_MAC
    int samples = maxOpenGlSamples();
    if (samples > 0) {
        QSurfaceFormat format = view.format();
        format.setSamples(samples);
        view.setFormat(format);
    }
    else {
        qWarning() << "multisample antialiasing is deactivated";
    }
#endif

    // restore geometry
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    if (settings.contains("view-x")) view.setX(settings.value("view-x").toInt());
    if (settings.contains("view-y")) view.setY(settings.value("view-y").toInt());

    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    QObject::connect(&view, &QQuickView::xChanged, [&settings](int x) { settings.setValue("view-x", x); });
    QObject::connect(&view, &QQuickView::yChanged, [&settings](int y) { settings.setValue("view-y", y); });

    return app.exec();
}
