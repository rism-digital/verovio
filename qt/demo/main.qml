/////////////////////////////////////////////////////////////////////////////
// Name:        main.qml
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

import QtQuick 2.7
import QtQuick.Controls 2.2
import Qt.labs.settings 1.0
import VerovioQml 1.0

Item {
    id: root
    width: 800
    height: 600

    Drawer {
        id: drawer
        visible: false
        width: options.width
        height: root.height
        edge: Qt.RightEdge
        modal: false

        Options {
            id: options
            anchors.top: closeButton.bottom
        }

        Button {
            id: closeButton
            anchors.topMargin: 5
            anchors.rightMargin: 10
            anchors.right: parent.right
            anchors.top: parent.top
            text: "Close Options"
            onClicked: {
                drawer.close();
            }
        }
    }

    VerovioDoc
    {
        id: verovioDocument
        pageWidth: root.width
        pageHeight: options.viewMode == "Continuous" ? 100 : root.height
        scale: 50
        adjustPageHeight: options.viewMode == "Continuous" ? true : false

        resourcesDataPath: "../../data"
        fontDirPath: "../../fonts"

        fileName: options.fileName
        musicFont: options.musicFont
        border: options.border
        spacingStaff: options.spacingStaff
        spacingSystem: options.spacingSystem
        noLayout: options.noLayout
        ignoreLayout: options.ignoreLayout
    }

    ListView {
        anchors.fill: parent
        snapMode: options.viewMode == "Page" ? ListView.SnapOneItem : ListView.NoSnap
        orientation: options.orientation

        model: verovioDocument.pageCount

        delegate: VerovioPage {
            verovioDoc: verovioDocument
            width:  root.width
            height: verovioDocument.adjustedPageHeightForPage(index+1)
            pageNumber: index + 1
        }
        ScrollBar.vertical: ScrollBar { }
        ScrollBar.horizontal: ScrollBar { }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            onWheel: {
                if (wheel.modifiers & Qt.ControlModifier) {
                    // calculate new scale
                    verovioDocument.scale = verovioDocument.scale * (1 + 0.1 * wheel.angleDelta.y / 120)

                    // put scale to limits
                    if (wheel.angleDelta.y < 0 && verovioDocument.scale < 20)
                        verovioDocument.scale = 20
                    else if (wheel.angleDelta.y > 0 && verovioDocument.scale > 300)
                        verovioDocument.scale = 300
                    else if (verovioDocument.scale >= 95 && verovioDocument.scale <= 105)
                        verovioDocument.scale = 100
                }
                else {
                    wheel.accepted = false
                }
            }
        }
    }

    Button {
        anchors.topMargin: 5
        anchors.rightMargin: 10
        anchors.right: parent.right
        anchors.top: parent.top
        text: "Options"
        onClicked: {
            drawer.open();
        }
    }

    DropArea {
        anchors.fill: parent
        keys: ["text/plain"]
        onDropped: {
            if (drop.hasText) {
                options.fileName = drop.text.trim()
            }
        }
    }

    Settings {
        property alias width: root.width
        property alias height: root.height
        property alias scale: verovioDocument.scale
        property alias drawerVisible: drawer.visible
    }
}
