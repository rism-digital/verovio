/////////////////////////////////////////////////////////////////////////////
// Name:        Options.qml
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import Qt.labs.settings 1.0

Item {
    id: root

    width: layout.width

    property string fileName: ":/mei/test.mei"
    property string musicFontName: "Leipzig"

    Column {
        id: layout
        padding: 10

        GridLayout {
            id: grid
            rows: 5
            columns: 2

            property int minimumInputSize: 10

            Label {
                text: "File Name"
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }
            Row {
                spacing: 5
                TextField {
                    text: root.fileName
                    Layout.fillWidth: true
                    Layout.minimumWidth: grid.minimumInputSize
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                }
                Button {
                    text: "Choose file"
                    onClicked: {
                        fileDialog.visible = true;
                    }
                }

                FileDialog {
                    id: fileDialog
                    title: "Please choose a file"
                    folder: shortcuts.home
                    onAccepted: {
                        root.fileName = fileDialog.fileUrl
                    }
                }
            }

            Label {
                text: "Music Font"
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }
            ComboBox {
                id: musicFontComboxBox
                model: ["Bravura", "Leipzig", "Gootville"]
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                onCountChanged: {
                    currentIndex = musicFontComboxBox.find(root.musicFontName)
                }
                onActivated: {
                    root.musicFontName = currentText
                }
            }
        }
    }

    Settings {
        property alias fileName: root.fileName
        property alias musicFontName: root.musicFontName
    }
}
