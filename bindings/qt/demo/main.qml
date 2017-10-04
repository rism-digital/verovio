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

    VerovioToolkit
    {
        id: verovioToolkit
        displayWidth: root.width
        displayHeight: 100
        scale: 50
        adjustPageHeight: true

        resourcesDataPath: "../../../data"

        fileName: options.fileName
        musicFontName: options.musicFontName
        musicFontPath: {
            if (options.musicFontName == "Bravura")
                ":/fonts/Bravura.otf"
            else if (options.musicFontName == "Leipzig")
                ":/fonts/Leipzig.ttf"
            else if (options.musicFontName == "Gootville")
                ":/fonts/Gootville.otf"
        }
        verovioTextFontPath: ":/fonts/VerovioText.ttf"
    }

    ListView {
        anchors.fill: parent
        snapMode: ListView.NoSnap

        model: verovioToolkit.pageCount

        delegate: VerovioView {
            toolkit: verovioToolkit
            width:  root.width
            height: verovioToolkit.adjustedDisplayHeightForPage(index+1)
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
                    verovioToolkit.scale = verovioToolkit.scale * (1 + 0.1 * wheel.angleDelta.y / 120)

                    // put scale to limits
                    if (wheel.angleDelta.y < 0 && verovioToolkit.scale < 20)
                        verovioToolkit.scale = 20
                    else if (wheel.angleDelta.y > 0 && verovioToolkit.scale > 300)
                        verovioToolkit.scale = 300
                    else if (verovioToolkit.scale >= 95 && verovioToolkit.scale <= 105)
                        verovioToolkit.scale = 100
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
        property alias scale: verovioToolkit.scale
        property alias drawerVisible: drawer.visible
    }
}
