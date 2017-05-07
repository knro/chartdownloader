/*  Service Provider
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef SERVICEPROVIDER_H
#define SERVICEPROVIDER_H

#include <QObject>
#include <QVector>

class FileDownloader;
class ChartInfo;

class ServiceProvider : public QObject
{
    Q_OBJECT
public:
    explicit ServiceProvider(QObject *parent = 0);
    virtual ~ServiceProvider() {}

    // Child must override

    // Can we download this airport info?
    virtual bool canDownload(const QString &airportID)=0;
    // Download charts
    virtual bool startDownload(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach)=0;
    // Cancel download and clear any data
    virtual void stopDownload()=0;

    QVector<ChartInfo*> getCharts() { return charts; }

signals:
    void parseError(const QString &errorMessage);
    void parseComplete();

protected:
    QVector<ChartInfo*> charts;
    QString m_airportID;
    bool getAirport=true, getMin=false, getSID=false, getStar = false, getApproach = false;

    FileDownloader *downloadJob=nullptr;

};

#endif // ServiceProvider_H
