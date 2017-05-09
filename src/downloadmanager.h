/*  Download Manager
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QVector>
#ifdef WIN32
#include <poppler/qt5/poppler-qt5.h>
#else
#include <poppler-qt5.h>
#endif
class ServiceProvider;
class FileDownloader;
class ChartInfo;

class DownloadManager: public QObject
{
Q_OBJECT

Q_PROPERTY(double downloadProgress READ downloadProgress NOTIFY downloadProgressChanged)
Q_PROPERTY(QString downloadProgressStatus READ downloadProgressStatus)
Q_PROPERTY(QString downloadFolder READ downloadFolder WRITE setDownloadFolder)

public:
    // Conversion quality
    enum { QUALITY_HIGH, QUALITY_MEDIUM, QUALITY_LOW };

    DownloadManager();

    Q_INVOKABLE bool startDownload(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach, bool convertToAerosoft, int conversionQuality);
    Q_INVOKABLE void stopDownload();

    QString downloadFolder() const;
    void setDownloadFolder(const QString &downloadFolder);

    QString downloadProgressStatus() const { return m_downloadProgressStatus; }

    double downloadProgress() { return m_downloadProgress;}

    QVector<ChartInfo*> getCharts() { return charts; }

public slots:
    // When we get all the URLs ready
    void processParseComplete();

protected slots:
    void processDownloadSucess();
    void processDownloadError(const QString &errorString);

signals:
    void downloadProgressChanged(double value);
    void downloadComplete();
    void downloadError();

private:

    void downloadNextChart();
    void convertNextChart();

    QString m_downloadProgressStatus, m_downloadFolder, m_currentAirportID, m_currentRootDir, m_currentChartFilename;
    double m_downloadProgress=0;
    uint8_t conversionQuality=0;

    bool getAirport, getMinimums, getSID, getSTAR, getApproach, convertToAerosoft;

    // Current Supported Services
    QVector<ServiceProvider*> downloadServices;

    QVector<ChartInfo*> charts, conversionCharts;
    uint16_t totalCharts=0, downloadedCharts=0;

    ServiceProvider *m_currentService=nullptr;    

    FileDownloader *downloadJob=nullptr;
};

#endif // DOWNLOADMANAGER_H
