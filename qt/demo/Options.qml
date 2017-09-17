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

    property string viewMode: "Continuous"
    property var orientation: Qt.Vertical

    property string fileName: "../../doc/tests/mei/01_mensural/01_durations.mei"
    property string musicFont: "Leipzig"
    property int border: 50
    property int spacingStaff: 10
    property int spacingSystem: 10
    property bool noLayout: false
    property bool ignoreLayout: false

    Column {
        id: layout
        rightPadding: 10
        leftPadding: 10
        spacing: 20

        GroupBox {
            title: "View Mode"

            GridLayout {
                columns: 2

                Label {
                    text: "File Name"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                }
                Row {
                    ButtonGroup { id: viewModeButtonGroup }
                    RadioButton {
                        checked: root.viewMode == "Continuous"
                        text: "Continuous"
                        ButtonGroup.group: viewModeButtonGroup
                        onCheckedChanged: {
                            if (checked) {
                                checked = true; // break binding loop
                                root.viewMode = "Continuous"
                            }
                        }
                    }
                    RadioButton {
                        checked: root.viewMode == "Page"
                        text: "Page based"
                        ButtonGroup.group: viewModeButtonGroup
                        onCheckedChanged: {
                            if (checked) {
                                checked = true; // break binding loop
                                root.viewMode = "Page"
                            }
                        }
                    }
                }

                Label {
                    text: "Orientation"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                }
                Row {
                    ButtonGroup { id: orientationButtonGroup }
                    RadioButton {
                        checked: root.orientation == Qt.Vertical
                        text: "Vertical"
                        ButtonGroup.group: orientationButtonGroup
                        onCheckedChanged: {
                            if (checked) {
                                checked = true; // break binding loop
                                root.orientation = Qt.Vertical;
                            }
                        }
                    }
                    RadioButton {
                        checked: root.orientation == Qt.Horizontal
                        text: "Horizontal"
                        ButtonGroup.group: orientationButtonGroup
                        onCheckedChanged: {
                            if (checked) {
                                checked = true; // break binding loop
                                root.orientation = Qt.Horizontal
                            }
                        }
                    }
                }
            }
        }

        GroupBox {
            title: "Verovio Settings"

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
                        currentIndex = musicFontComboxBox.find(root.musicFont)
                    }
                    onActivated: {
                        root.musicFont = currentText
                    }
                }

                Label {
                    text: "Border"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                }
                SpinBox {
                    from: 0
                    value: root.border
                    to: 500
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                    editable: true
                    onValueChanged: {
                        root.border = value
                    }
                }

                Label {
                    text: "Spacing Staff"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                }
                SpinBox {
                    from: 0
                    value: root.spacingStaff
                    to: 24
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                    editable: true
                    onValueChanged: {
                        root.spacingStaff = value
                    }
                }

                Label {
                    text: "Spacing System"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                }
                SpinBox {
                    from: 0
                    value: root.spacingSystem
                    to: 12
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                    editable: true
                    onValueChanged: {
                        root.spacingSystem = value
                    }
                }

                Label {
                    text: "No Layout"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                }
                CheckBox {
                    checked: root.noLayout
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                    onCheckedChanged: {
                        root.noLayout = checked
                    }
                }

                Label {
                    text: "Ignore Layout"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                }
                CheckBox {
                    checked: root.ignoreLayout
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                    onCheckedChanged: {
                        root.ignoreLayout = checked
                    }
                }
            }
        }
    }

    Settings {
        property alias viewMode: root.viewMode
        property alias orientation: root.orientation
        property alias fileName: root.fileName
        property alias musicFont: root.musicFont
        property alias border: root.border
        property alias spacingStaff: root.spacingStaff
        property alias spacingSystem: root.spacingSystem
        property alias noLayout: root.noLayout
        property alias ignoreLayout: root.ignoreLayout
    }
}
