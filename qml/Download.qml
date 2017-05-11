import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import Qt.labs.platform 1.0
import QtQuick.Dialogs 1.2
import Utils 1.0
import DownloadManager 1.0

DownloadForm
{
    id: downloadFormPage

    property bool isRunning: false    

    Utils
    {
        id: utils
    }

    Popup
    {
           id: popup
           modal: true
           focus: true

           x: parent.width/2 - popup.width/2
           y: parent.height/2 + popup.height/2

           background: Rectangle
           {
               id: contentBackground
               implicitWidth: 400
               implicitHeight: 100
               color: "transparent"
               border.color: "#444"
           }

           RowLayout
           {
               id: contentLayout
               anchors.fill: parent
               Image
               {
                   id: popupImage
                   source: "images/checkmark.png"
               }

               Label
               {
                   id: popupLabel
                   text: "Download Complete!"
               }
           }

           MouseArea
           {
               anchors.fill: parent
               onClicked:
               {
                   popup.close()
               }
           }
    }

    DownloadManager
    {
        id: downloadManager
        onDownloadProgressChanged:
        {
            downloadProgressBar.indeterminate = false
            downloadProgressBar.value = downloadProgress
            statusLabel = downloadProgressStatus
        }

        onDownloadComplete:
        {
            state = "idle"
            popupImage.source = "images/checkmark.png"
            popupLabel.text = downloadProgressStatus
            popup.open()

            var airportFolder = utils.convertPathToURL(downloadFolder+"/"+airportID.text);
            photoViewerPage.currentFolder = airportFolder
        }

        onDownloadError:
        {
            state = "idle"
            popupImage.source = "images/error.png"
            popupLabel.text = downloadProgressStatus
            popup.open()
        }
    }

    MessageDialog
    {
        id: errorDialog
    }

    FolderDialog
    {
        id: downloadFolderDialog
        title: "Select Download Folder"

        onAccepted:
        {
            console.log("Select folder is " + folder);
            downloadFolder.text = utils.convertUrlToNativeFilePath(folder)
            downloadFolderChanged(downloadFolder.text)
        }
    }

    downloadFolder.placeholderText: utils.convertUrlToNativeFilePath(StandardPaths.writableLocation(StandardPaths.DocumentsLocation))

    selectDownloadFolder.onClicked:
    {
        downloadFolderDialog.open()
    }

    downloadButton.onClicked:
    {
        if (downloadFolder.text.length == 0)
        {
            errorDialog.title = "Missing Folder"
            errorDialog.text = "You must specify the download folder"
            errorDialog.open()
            return
        }

        if (airportID.text.length != 4)
        {
            errorDialog.title = "Invalid Airport"
            errorDialog.text = "You must enter a valid ICAO airport code"
            errorDialog.open()
            return
        }

        console.log("Airport Code is " + airportID.text);

        downloadManager.downloadFolder = downloadFolder.text
        if (state == "idle")
        {
            var isSuccess = downloadManager.startDownload(airportID.text, isAirportChecked, isMinimumChecked, isSIDChecked, isStarChecked, isApproachChecked, isAerobaskChecked, qualityComboCurrentIndex);
            if (isSuccess)
            {
                state = "running"
            }
        }
        else
        {
            downloadManager.stopDownload();
            state = "idle";
        }                
    }
}
