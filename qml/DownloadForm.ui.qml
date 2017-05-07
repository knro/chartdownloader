import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Qt.labs.calendar 1.0
import Qt.labs.folderlistmodel 2.2
import Qt.labs.platform 1.0
import Qt.labs.settings 1.0

Item {
    id: downloadItem
    width: 640
    height: 480
    property alias qualityComboCurrentIndex: qualityCombo.currentIndex
    state: "idle"
    property alias busyIndicator: busyIndicator

    // I don't know much QML so this seems stupid to me
    // Maybe I should move implementation to this file? No need for these many aliases!!
    property alias downloadFolder: downloadFolder
    property alias selectDownloadFolder: selectDownloadFolder
    property alias downloadButton: downloadButton
    property alias downloadProgressBar: downloadProgressBar
    property alias airportID: airportID
    property alias isAirportChecked: airportCheck.checked
    property alias isMinimumChecked: minCheck.checked
    property alias isSIDChecked: sidCheck.checked
    property alias isStarChecked: starCheck.checked
    property alias isApproachChecked: approachCheck.checked
    property alias isAerosoftChecked: aerosoftCheck.checked
    property alias statusLabel: statusLabel.text

    Rectangle {
        id: betaRectangle
        x: 464
        width: 300
        height: 100
        color: "#4e4f53"
        radius: 1
        anchors.right: parent.right
        anchors.rightMargin: -124
        anchors.top: parent.top
        anchors.topMargin: -29
        enabled: false
        transformOrigin: Item.Center
        z: -1
        rotation: 45
        border.color: "#00000000"
    }

    Text {
        id: betaText
        x: 566
        color: "#ffffff"
        text: qsTr("Beta")
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 20
        rotation: 45
        font.pixelSize: 31
    }

    ColumnLayout {
        id: columnLayout
        width: 100
        height: 100
        anchors.top: parent.top
        anchors.topMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 25
        scale: 1
        spacing: 10

        GridLayout {
            id: gridLayout
            width: 100
            height: 100
            rows: 3
            columns: 3

            Label {
                id: airportLabel
                text: qsTr("Airport (ICAO):")
            }

            TextField {
                id: airportID
                width: 80
                height: 20
                Layout.columnSpan: 2
                Layout.minimumWidth: 100
                Layout.fillWidth: false
                font.pixelSize: 12
                placeholderText: qsTr("KSAN")
            }

            Label {
                id: downloadToLabel
                text: qsTr("Download To:")
                z: 1
            }

            TextField {
                id: downloadFolder
                width: 109
                height: 20
                Layout.minimumWidth: 300
                font.pixelSize: 12
            }

            Button {
                id: selectDownloadFolder
                text: qsTr("Select")
            }
        }

        RowLayout {
            id: rowLayout2
            width: 100
            height: 100

            CheckBox {
                id: airportCheck
                text: qsTr("AIRPORT")
                checked: true
            }

            CheckBox {
                id: minCheck
                x: 113
                y: 16
                text: qsTr("Minimums")
            }

            CheckBox {
                id: sidCheck
                text: qsTr("SIDs")
            }

            CheckBox {
                id: starCheck
                text: qsTr("STARs")
            }

            CheckBox {
                id: approachCheck
                text: qsTr("Approaches")
            }
        }

        RowLayout {
            id: rowLayout3
            width: 100
            height: 100

            CheckBox {
                id: aerosoftCheck
                text: qsTr("Make Aerosoft Charts")
            }

            Label {
                id: label
                text: qsTr("Quality:")
            }

            ComboBox {
                id: qualityCombo
                model: ["High", "Medium", "Low"]
                currentIndex: 1
            }
        }

        RowLayout {
            id: rowLayout
            width: 100
            height: 100

            Button {
                id: downloadButton
                text: qsTr("Download Now")
            }

            ProgressBar {
                id: downloadProgressBar
                clip: false
                visible: false
                indeterminate: false
                Layout.fillWidth: true
                to: 100
                value: 0
            }
        }
    }

    Label {
        id: statusLabel
        x: 25
        y: 306
        font.pointSize: 16
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
    }

    BusyIndicator {
        id: busyIndicator
        y: 380
        width: 100
        height: 100
        visible: false
        running: false
        anchors.rightMargin: 0
        anchors.bottomMargin: 50
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    states: [
        State {
            name: "running"
            PropertyChanges {
                target: downloadButton
                text: "Stop..."
            }
            PropertyChanges {
                target: busyIndicator
                visible: true
            }
            PropertyChanges {
                target: busyIndicator
                running: true
            }
            PropertyChanges {
                target: downloadProgressBar
                visible: true
            }
            PropertyChanges {
                target: downloadProgressBar
                indeterminate: true
            }
            PropertyChanges {
                target: statusLabel
                visible: true
                text: "Downloading..."
            }
        },
        State {
            name: "idle"
            PropertyChanges {
                target: downloadButton
                text: "Download"
            }
            PropertyChanges {
                target: busyIndicator
                visible: false
            }
            PropertyChanges {
                target: busyIndicator
                running: false
            }
            PropertyChanges {
                target: downloadProgressBar
                visible: false
            }
            PropertyChanges {
                target: downloadProgressBar
                indeterminate: false
            }
            PropertyChanges {
                target: statusLabel
                visible: false
            }
        }
    ]

    Settings {
        property alias downloadFolder: downloadFolder.text
        property alias isAirport: airportCheck.checked
        property alias isMinimum: minCheck.checked
        property alias isSID: sidCheck.checked
        property alias isSTAR: starCheck.checked
        property alias isIAP: approachCheck.checked
        property alias isAerosoft: aerosoftCheck.checked
        property alias conversionQuality: qualityCombo.currentIndex
    }
}
