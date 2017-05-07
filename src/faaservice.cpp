/*  FAA Service
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QDate>
#include <QUrl>
#include <QXmlSimpleReader>

#include "faaservice.h"
#include "filedownloader.h"
#include "mycontenthandler.h"

FAAService::FAAService(QObject *parent) : ServiceProvider(parent)
{

}

bool FAAService::startDownload(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach)
{
    m_airportID  = airportID;

    this->getAirport = getAirport;
    this->getMin     = getMinimums;
    this->getSID     = getSID;
    this->getStar    = getSTAR;
    this->getApproach= getApproach;

    // Get current cycle from 1-week in the future
    QString cycle       = QDate::currentDate().addDays(7).toString("yyMM");
    QUrl    url         = QUrl(QString("https://www.faa.gov/air_traffic/flight_info/aeronav/digital_products/dtpp/search/results/?cycle=%1&ident=%2").arg(cycle,airportID));

    downloadJob = new FileDownloader();

    QObject::connect(downloadJob, SIGNAL(downloaded()), this, SLOT(processDownloadSucess()));
    QObject::connect(downloadJob, SIGNAL(error(QString)), this, SLOT(processDownloadError(QString)));

    //QString output  = KSPaths::writableLocation(QStandardPaths::GenericDataLocation) + "catalog.min.json";
    //downloadJob->setDownloadedFileURL(QUrl::fromLocalFile(output));

    state = PARSE_HTML;

    downloadJob->get(url);

    return true;

}

void FAAService::processDownloadSucess()
{
    downloadJob->deleteLater();

    if (state == PARSE_HTML)
    {
        QXmlSimpleReader xmlReader;
        QXmlInputSource *source = new QXmlInputSource();
        QString data = downloadJob->downloadedData().simplified();

        int openingBody = data.indexOf("<tbody>");
        int closingBody = data.indexOf("</tbody>");

        int diff = closingBody - openingBody + strlen("</tbody>");
        data = data.mid(openingBody, diff);

        //qDebug() << data;

        //HTMLParser *parser = new HTMLParser();
        //parser->parse(data);

        source->setData(data);

        MyContentHandler *handler = new MyContentHandler(m_airportID);
        xmlReader.setContentHandler(handler);
        xmlReader.setErrorHandler(handler);

        bool ok = xmlReader.parse(source);

        if (ok == false)
        {
            emit parseError(QString("Parsing failed %1.").arg(handler->errorString()));
            state = IDLE;
            return;
        }

        charts = handler->charts;

        delete (handler);
        delete (source);

        qDebug() << "Finished parsing!!";

        emit parseComplete();

    }
}

void FAAService::processDownloadError(const QString &errorString)
{
    qCritical() << "Download Error:" << errorString;
    downloadJob->deleteLater();
    emit parseError(errorString);
}

void FAAService::stopDownload()
{
    downloadJob->cancel();
}
