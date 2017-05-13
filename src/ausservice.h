/*  Australia Service
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef AUSSERVICE_H
#define AUSSERVICE_H

#include "serviceprovider.h"

class FileDownloader;

class AUSService : public ServiceProvider
{
    Q_OBJECT
public:
    typedef enum { IDLE, PARSE_HTML, DOWNLOAD_DATA } DownloadState;

    explicit AUSService(QObject *parent = 0);
    ~AUSService();

    virtual bool canDownload(const QString &airportID) { return airportID.startsWith("Y", Qt::CaseInsensitive); }

    virtual bool startDownload(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach);
    // Cancel download and clear any data
    virtual void stopDownload();

protected slots:
    void processDownloadSucess();
    void processDownloadError(const QString &errorString);

private:

    DownloadState state = IDLE;
    QString ausData;
};

#endif // AUSService_H
