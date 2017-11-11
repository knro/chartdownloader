/*  FAA Service
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef FAASERVICE_H
#define FAASERVICE_H

#include "serviceprovider.h"

class FileDownloader;

class FAAService : public ServiceProvider
{
    Q_OBJECT
public:
    typedef enum { IDLE, PARSE_HTML, DOWNLOAD_DATA } DownloadState;

    explicit FAAService(QObject *parent = 0);
    ~FAAService();

    virtual bool canDownload(const QString &airportID);

    virtual bool startDownload(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach);
    // Cancel download and clear any data
    virtual void stopDownload();

protected slots:
    void processDownloadSucess();
    void processDownloadError(const QString &errorString);

private:

    DownloadState state = IDLE;
};

#endif // FAASERVICE_H
