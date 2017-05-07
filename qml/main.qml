import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Chart Downloader")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Download
        {
            id: downloadPage

            onDownloadFolderChanged:
            {
              console.log("Download folder change!");
              photoViewerPage.currentFolder = "file://" + downloadFolder.text
            }

        }

        PhotoViewer
        {
            id: photoViewerPage
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("Download")
        }
        TabButton {
            text: qsTr("View")
        }
    }

    Component.onCompleted:
    {
        photoViewerPage.currentFolder = "file://" + downloadPage.downloadFolder.text
    }

}
