import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Utils 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Chart Downloader")

    Utils
    {
        id: utils
    }

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
              photoViewerPage.downloadFolder = utils.convertPathToURL(downloadPage.downloadFolder.text)
            }

            onDownloadFinished:
            {
                console.log("Download complete for Airport: " + airportID.text)
                photoViewerPage.currentAirport = airportID.text
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
        photoViewerPage.downloadFolder = utils.convertPathToURL(downloadPage.downloadFolder.text)
        photoViewerPage.currentAirport = (downloadPage.airportID.text ? downloadPage.airportID.text : "KSAN")
       //console.log("download folder is:" + photoViewerPage.downloadFolder);

    }

}
