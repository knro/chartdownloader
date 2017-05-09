/*  File Downloader
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QFile>

#include "filedownloader.h"

FileDownloader::FileDownloader(QObject * parent) :  QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply *)), this, SLOT (dataFinished(QNetworkReply *)));
}

FileDownloader::~FileDownloader()
{
}

void FileDownloader::get(const QUrl &fileUrl)
{
    QNetworkRequest request(fileUrl);
    m_DownloadedData.clear();
    isCancelled = false;
    m_Reply = m_WebCtrl.get(request);

    connect(m_Reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError()));
    connect(m_Reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));    
    connect(m_Reply, SIGNAL(readyRead()), this, SLOT(dataReady()));
}

void FileDownloader::post(const QUrl &fileUrl, QByteArray &data)
{
    QNetworkRequest request(fileUrl);
    m_DownloadedData.clear();
    isCancelled = false;
    m_Reply = m_WebCtrl.post(request, data);

    connect(m_Reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError()));
    connect(m_Reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));    
    connect(m_Reply, SIGNAL(readyRead()), this, SLOT(dataReady()));
}

void FileDownloader::post(const QUrl &fileUrl, QHttpMultiPart * parts)
{
    QNetworkRequest request(fileUrl);
    m_DownloadedData.clear();
    isCancelled = false;
    m_Reply = m_WebCtrl.post(request, parts);

    connect(m_Reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError()));
    connect(m_Reply, SIGNAL(readyRead()), this, SLOT(dataReady()));
}

void FileDownloader::cancel()
{
    if (m_Reply && m_Reply->isRunning())
    {
        isCancelled = true;
        m_Reply->abort();
    }
}

void FileDownloader::dataReady()
{
    if (m_DownloadedFile.isOpen())
        m_DownloadedFile.write(m_Reply->readAll());
    else
        m_DownloadedData += m_Reply->readAll();
}

void FileDownloader::dataFinished(QNetworkReply * pReply)
{
    dataReady();
    if (m_DownloadedFile.isOpen())
    {
        m_DownloadedFile.flush();
        m_DownloadedFile.close();
    }

    if (isCancelled == false)
        emit downloaded();

    pReply->deleteLater();
}

void FileDownloader::slotError()
{    
    if (isCancelled)
    {
        // Remove partially downloaded file, should we download to %tmp first?
        if (m_DownloadedFile.isOpen())
        {
            m_DownloadedFile.close();
            QFile::remove(m_DownloadedFileURL.toLocalFile());
        }
        emit canceled();
    }
    else
        emit error(m_Reply->errorString());

    m_Reply->deleteLater();
}

QUrl FileDownloader::getDownloadedFileURL() const
{
    return m_DownloadedFileURL;
}

bool FileDownloader::setDownloadedFileURL(const QUrl &DownloadedFile)
{
    m_DownloadedFileURL = DownloadedFile;

    if (m_DownloadedFileURL.isEmpty() == false)
    {
        m_DownloadedFile.setFileName(m_DownloadedFileURL.toLocalFile());
        bool rc = m_DownloadedFile.open( QIODevice::WriteOnly);

        if (rc == false)
            qWarning() << m_DownloadedFile.errorString();

        return rc;
    }

    m_DownloadedFile.close();
    return true;
}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}
