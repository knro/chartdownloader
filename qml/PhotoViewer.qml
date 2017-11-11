import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Qt.labs.folderlistmodel 2.1

Page
{
    id: photoViewer
    width: 640
    height: 480
    visible: true

    property alias currentFolder : folderModel.folder
    property string downloadFolder
    property alias currentAirport : airportID.text
    property int selectedChartType: 0

    Connections
    {
        target: airportID
        //onTextEdited:
        onTextChanged:
        {
            // Force a refresh
            currentFolder = downloadFolder
            currentFolder = downloadFolder + "/" + currentAirport

            chartType.update()
            /*
            var chartIndex = chartType.currentIndex
            chartType.setCurrentIndex(0)
            chartType.setCurrentIndex(1)
            chartType.setCurrentIndex(2)
            chartType.setCurrentIndex(3)
            chartType.setCurrentIndex(4)
            chartType.setCurrentIndex(chartIndex)*/

            header.update()

            console.log("Connections Current folder is:" + currentFolder);
        }
    }

    header:
    RowLayout
    {
        id: headerLayout
        width: parent.width

        TextField {
            id: airportID
            width: 80
            height: 20
            Layout.columnSpan: 2
            Layout.minimumWidth: 100
            Layout.fillWidth: false
            font.pixelSize: 12
            placeholderText: "KSAN"
        }

        TabBar
        {
        id: chartType
        currentIndex: 0
        TabButton {
            text: qsTr("Airport")
        }
        TabButton {
            text: qsTr("Minimum")
        }
        TabButton {
            text: qsTr("SIDs")
        }
        TabButton {
            text: qsTr("STARs")
        }
        TabButton {
            text: qsTr("Approaches")
        }

        onCurrentIndexChanged:
        {
            switch(currentIndex)
            {
            // Airport
            case 0:
                folderModel.nameFilters = ["INF.AIRPORT*.png", "INF.AERO*.png"]
                break;
            // Minimums
            case 1:
                folderModel.nameFilters = ["INF.*MIN*.png", "INF.*VECTOR*.png"]
                break;
            // SIDs
            case 2:
                folderModel.nameFilters = ["DEP.*.png"]
                break;
            // Stars
            case 3:
                folderModel.nameFilters = ["ARR.*.png"]
                break;
            // Approach
            case 4:
                folderModel.nameFilters = ["APP.*.png"]
                break;
            }
        }
    }
    }

    FolderListModel
    {
        id: folderModel
        folder: downloadFolder + "/" + currentAirport
        nameFilters: ["INF.AIRPORT*.png"]

        onFolderChanged:
        {
            console.log("FolderListModel folder is updated: " + folder);
        }
    }

    ListView
    {
        id: folderListView
        anchors.fill: parent
        focus: true

        Component
        {
            id: fileDelegate            

            Rectangle
            {
                id:delegate
                width: folderListView.width
                height: fileIsDir? 48 : folderListView.height
                color: "black"                

                Row
                {
                    // This is retarded, what's a better way in QML?
                    visible: fileIsDir && fileBaseName != "AIRPORT" && fileBaseName != "MINIMUMS" && fileBaseName != "SIDS"
                    && fileBaseName != "STARS" && fileBaseName != "APPROACHES"
                    anchors.fill: parent
                    Image {
                        id: icon
                        width: delegate.height - 2
                        height:width
                        source: "image://iconProvider/"+filePath
                    }
                    Text {
                        text: fileName
                        color: "white"
                        font.pixelSize: 16
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Column
                {
                    visible: !fileIsDir
                    anchors.fill: parent
                    TextField
                    {
                        text: fileName.replace(".png", "")
                        color: "red"
                        font.pointSize: 18
                        font.bold: true
                        anchors.horizontalCenter: parent.horizontalCenter
                        //anchors.bottom: parent.bottom
                        //anchors.bottomMargin: previewImage.height/10
                        background: Rectangle { color: "black" }
                    }
                    Image
                    {
                        id: previewImage
                        width: delegate.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        source: "image://iconProvider/"+filePath
                        sourceSize.width: 1024
                        sourceSize.height: 1024
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                }

                MouseArea {
                    id:mouseArea
                    anchors.fill: parent
                    onClicked:
                    {
                        fileIsDir ? currentFolder = fileURL : Qt.openUrlExternally(fileURL)
                    }
                }
        }
        }

        header: Rectangle
        {
                    id: photoHeader
                    width: parent.width
                    height: 34
                    color: "gray"
                    z:2
                    Row
                    {
                        Button
                        {
                            width:32
                            height :32
                            text: "<<<"
                            onClicked:
                            {
                                currentFolder = folderModel.parentFolder
                                //console.log("<<< Current folder is:" + currentFolder);
                            }
                        }
                        anchors.fill: parent
                        Text
                        {
                            text: currentFolder
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
        }

        footerPositioning: ListView.OverlayHeader

        footer: Rectangle
        {
                    id: photoFooter
                    width: parent.width
                    height: 34
                    color: "gray"
                    z:2
                    Row
                    {
                        anchors.fill: parent
                        Button
                        {
                            width:32
                            height :32
                            text: "<<<"
                            onClicked:
                            {
                                currentFolder = folderModel.parentFolder
                                //console.log("<<< Current folder is:" + currentFolder);
                            }
                        }
                        Text
                        {
                            text: "["+folderModel.count+" Files]"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
        }

        model: folderModel
        delegate: fileDelegate
    }
}
