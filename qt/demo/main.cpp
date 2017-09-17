/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <QGuiApplication>
#include <QQuickView>
#include <QSettings>

//----------------------------------------------------------------------------

#include "veroviodoc.h"
#include "veroviopagedemo.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Verovio");
    app.setOrganizationDomain("verovio.org");
    app.setApplicationName("Verovio Qt Demo");
    QSettings settings;

    // Register custom QML types for Verovio
    qmlRegisterType<VerovioPageDemo>("VerovioQml", 1, 0, "VerovioPage");
    qmlRegisterType<vrv::VerovioDoc>("VerovioQml", 1, 0, "VerovioDoc");

    QQuickView view;

    // Activate multisample antialiasing
    QSurfaceFormat format = view.format();
    format.setSamples(8);
    view.setFormat(format);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    if (settings.contains("view-x")) view.setX(settings.value("view-x").toInt());
    if (settings.contains("view-y")) view.setY(settings.value("view-y").toInt());
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    QObject::connect(&view, &QQuickView::xChanged, [&settings](int x) { settings.setValue("view-x", x); });
    QObject::connect(&view, &QQuickView::yChanged, [&settings](int y) { settings.setValue("view-y", y); });

    return app.exec();
}
