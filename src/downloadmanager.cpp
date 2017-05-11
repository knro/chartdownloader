/*  Download Manager
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QDir>
#include <QDebug>
#include <QImage>

#include "downloadmanager.h"
#include "filedownloader.h"
#include "faaservice.h"
#include "chartinfo.h"

DownloadManager::DownloadManager()
{
    m_downloadFolder = QDir::homePath();

    ServiceProvider *ffaDownloader = new FAAService();

    connect(ffaDownloader, SIGNAL(parseError(QString)), this, SLOT(processDownloadError(QString)));
    connect(ffaDownloader, SIGNAL(parseComplete()), this, SLOT(processParseComplete()));

    downloadServices.append(ffaDownloader);
}

QString DownloadManager::downloadFolder() const
{
    return m_downloadFolder;
}

void DownloadManager::setDownloadFolder(const QString &downloadFolder)
{
    m_downloadFolder = downloadFolder;
}

bool DownloadManager::startDownload(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach, bool convertToAerobask, int conversionQuality)
{
    this->m_currentAirportID = airportID;
    this->getAirport         = getAirport;
    this->getMinimums        = getMinimums;
    this->getSID             = getSID;
    this->getSTAR            = getSTAR;
    this->getApproach        = getApproach;
    this->convertToAerobask  = convertToAerobask;
    this->conversionQuality  = conversionQuality;

    totalCharts = downloadedCharts = 0;

    foreach (ServiceProvider *service, downloadServices)
    {
        if (service->canDownload(airportID))
        {
            m_currentService = service;
            return service->startDownload(airportID, getAirport, getMinimums, getSID, getSTAR, getApproach);
        }
    }

    // No services found?
    m_downloadProgressStatus = tr("No download services available for this airport.");
    emit downloadError();
    return false;
}

void DownloadManager::stopDownload()
{    
    if (m_currentService && totalCharts == 0)
        m_currentService->stopDownload();
    else
    {
        downloadJob->disconnect();
        downloadJob->cancel();
        downloadJob->deleteLater();
    }
}

void DownloadManager::processParseComplete()
{
    qDeleteAll(charts);
    charts.clear();

    charts = conversionCharts= m_currentService->getCharts();
    downloadedCharts = 0;
    totalCharts = charts.count();

    m_currentRootDir = m_downloadFolder + QDir::separator() + m_currentAirportID;
    if (getAirport)
    {
        //airportInfo = m_currentService->getAirportInfo();
        QDir().mkpath(m_currentRootDir + QDir::separator() + "AIRPORT");
    }

    if (getMinimums)
    {
        //minInfo     = m_currentService->getMinInfo();
        QDir().mkpath(m_currentRootDir + QDir::separator() + "MINIMUMS");
    }

    if (getSID)
    {
        //sidInfo     = m_currentService->getSidInfo();
        QDir().mkpath(m_currentRootDir + QDir::separator() + "SIDS");
    }

    if (getSTAR)
    {
        //starInfo    = m_currentService->getStarInfo();
        QDir().mkpath(m_currentRootDir + QDir::separator() + "STARS");
    }

    if (getApproach)
    {
        //approachInfo= m_currentService->getApproachInfo();
        QDir().mkpath(m_currentRootDir + QDir::separator() + "APPROACHES");
    }



    // Download all the PDFs now and optionally convert them to PNGs    
    foreach (ChartInfo *chart, charts)
    {
        chart->filename = m_currentRootDir + QDir::separator() + chart->getTypeDescription() + QDir::separator() + chart->name + "." + chart->fileType;
        chart->filename = chart->filename.remove(",");

#if 0
        qDebug() << chart->getTypeDescription();
        qDebug() << chart->name + ":" + chart->url;
        qDebug() << chart->filename;
#endif
    }

    downloadNextChart();
}

void DownloadManager::downloadNextChart()
{
    if (charts.isEmpty() || downloadedCharts == totalCharts)
    {
        if (convertToAerobask)
        {
            downloadedCharts = 0;
            convertNextChart();
        }
        else
        {
            m_downloadProgressStatus = "Download Complete!";
            emit downloadComplete();
        }
        return;
    }

    ChartInfo *oneChart = charts.takeFirst();

    // If already exists, let's move to the next chart
    if (QFileInfo::exists(oneChart->filename))
    {
        m_downloadProgress = ((double) downloadedCharts)/((double)totalCharts) * 100.0;
        m_downloadProgressStatus = tr("Downloading %1").arg(oneChart->name);
        emit downloadProgressChanged(m_downloadProgress);
        downloadedCharts++;
        downloadNextChart();
        return;
    }

    downloadJob = new FileDownloader();

    // Assuming PDF here, have to check
    downloadJob->setDownloadedFileURL(QUrl::fromLocalFile(oneChart->filename));

    QObject::connect(downloadJob, SIGNAL(downloaded()), this, SLOT(processDownloadSucess()));
    QObject::connect(downloadJob, SIGNAL(error(QString)), this, SLOT(processDownloadError(QString)));

    m_downloadProgress = ((double) downloadedCharts)/((double)totalCharts) * 100.0;
    m_downloadProgressStatus = tr("Downloading %1").arg(oneChart->name);
    emit downloadProgressChanged(m_downloadProgress);

    downloadJob->get(oneChart->url);
}

void DownloadManager::processDownloadSucess()
{
    downloadedCharts++;

    downloadJob->deleteLater();

    downloadNextChart();
}

void DownloadManager::processDownloadError(const QString &errorString)
{
    qCritical() << QTime::currentTime().toString("HH:mm:ss.zzz") << "Download Error:" << errorString;
    downloadJob->deleteLater();

    m_downloadProgressStatus = errorString;
    emit downloadError();
}

void DownloadManager::convertNextChart()
{
    if (conversionCharts.isEmpty())
    {
        m_downloadProgressStatus = "Download and Conversion Complete!";
        emit downloadComplete();
        return;
    }

    ChartInfo *oneChart = conversionCharts.takeFirst();

    QString fileType;
    if (oneChart->type == ChartInfo::AIRPORT || oneChart->type == ChartInfo::MINIMUMS)
        fileType = "INF.";
    else if (oneChart->type == ChartInfo::SID)
        fileType = "DEP.";
    else if (oneChart->type == ChartInfo::STAR)
        fileType = "ARR.";
    else
        fileType = "APP.";

    QString pngFile = m_currentRootDir + QDir::separator() + fileType + oneChart->name + "." + oneChart->imageType;

    if (QFileInfo::exists(pngFile))
    {
        m_downloadProgress = ((double) downloadedCharts)/((double)totalCharts) * 100.0;
        m_downloadProgressStatus = tr("Converting %1").arg(oneChart->name);
        emit downloadProgressChanged(m_downloadProgress);
        downloadedCharts++;
        convertNextChart();
        return;
    }

    Poppler::Document* document = Poppler::Document::load(oneChart->filename);
    if (!document || document->isLocked())
    {
      // ... error message ....
      delete document;

      m_downloadProgressStatus = tr("Error converting chart %1").arg(oneChart->name);
      emit downloadError();
      return;
    }

    uint8_t pageNumber = 0;

    // Search for Airport Page
    if (oneChart->type == ChartInfo::MINIMUMS)
    {
        // e.g. KSAN to (SAN) so we can find it in the PDF pages
        QString id = "(" + oneChart->airportID.remove(0, 1) + ")";

        int page = pageNumber;
        while (page < document->numPages())
        {

            QList<QRectF> result = document->page(page)->search(id, Poppler::Page::WholeWords);
            if (result.isEmpty() == false)
            {
                pageNumber = page;
                break;
            }

            page++;
        }
    }

    Poppler::Page* pdfPage = document->page(pageNumber);
    if (pdfPage == 0)
    {
      m_downloadProgressStatus = tr("Error converting chart %1").arg(oneChart->name);
      emit downloadError();
      return;
    }

    uint16_t dpi=150, quality=70;
    if (conversionQuality == QUALITY_HIGH)
    {
        dpi = 300;
        quality = 15;
    }
    else if (conversionQuality == QUALITY_MEDIUM)
    {
        dpi = 200;
        quality = 10;
    }
    else
    {
        dpi = 100;
        quality = 5;
    }

    // Generate a QImage of the rendered page
    QImage image = pdfPage->renderToImage(dpi, dpi);
    if (image.isNull())
    {
      m_downloadProgressStatus = tr("Error converting chart %1").arg(oneChart->name);
      emit downloadError();
      return;
    }

    bool rc = image.save(pngFile, 0, quality);

    // after the usage, the page must be deleted
    delete pdfPage;

    delete document;

    if (rc)
    {
        m_downloadProgress = ((double) downloadedCharts)/((double)totalCharts) * 100.0;
        m_downloadProgressStatus = tr("Converting %1").arg(oneChart->name);
        emit downloadProgressChanged(m_downloadProgress);
        downloadedCharts++;
        convertNextChart();
    }
    else
    {
        m_downloadProgressStatus = tr("Error converting chart %1").arg(oneChart->name);
        downloadError();
    }

}
