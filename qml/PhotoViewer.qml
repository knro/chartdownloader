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

    ListView
    {
        id: folderListView
        anchors.fill: parent
        focus: true

        FolderListModel
        {
            id: folderModel
            showDirs: true
            nameFilters: ["*.png"]
        }

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
                    visible: fileIsDir
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
                    onClicked: fileIsDir ? currentFolder = fileURL : Qt.openUrlExternally(fileURL)
                }
        }
        }

        header: Rectangle
        {
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
                            onClicked: currentFolder = folderModel.parentFolder
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
                    id: viewFooter
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
                            onClicked: currentFolder = folderModel.parentFolder
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
