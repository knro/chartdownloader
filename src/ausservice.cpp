/*  Australia Service
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QDate>
#include <QUrl>
#include <QXmlSimpleReader>
#include <QRegularExpression>

#include "ausservice.h"
#include "filedownloader.h"
#include "auscontenthandler.h"

AUSService::AUSService(QObject *parent) : ServiceProvider(parent)
{

}

AUSService::~AUSService()
{
    delete (downloadJob);
}

bool AUSService::startDownload(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach)
{
    m_airportID  = airportID.toUpper();

    this->getAirport = getAirport;
    this->getMin     = getMinimums;
    this->getSID     = getSID;
    this->getStar    = getSTAR;
    this->getApproach = getApproach;

    // No need to redownload the page again
    if (ausData.isEmpty() == false)
    {
        state = PARSE_HTML;
        processDownloadSucess();
        return true;
    }

    QUrl    url         = QUrl(QString("https://www.airservicesaustralia.com/aip/current/dap/AeroProcChartsTOC.htm"));

    if (downloadJob)
        delete (downloadJob);

    downloadJob = new FileDownloader();

    QObject::connect(downloadJob, SIGNAL(downloaded()), this, SLOT(processDownloadSucess()));
    QObject::connect(downloadJob, SIGNAL(error(QString)), this, SLOT(processDownloadError(QString)));

    state = PARSE_HTML;

    downloadJob->get(url);

    return true;

}

void AUSService::processDownloadSucess()
{
    if (state == PARSE_HTML)
    {
        QXmlSimpleReader xmlReader;
        QXmlInputSource *source = new QXmlInputSource();

        if (ausData.isEmpty())
        {
            QString data = downloadJob->downloadedData().simplified();

            int openingBody = data.indexOf(QString("(%1)</h3>").arg(m_airportID)) + 12;
            int closingBody = data.mid(openingBody).indexOf("</table>");

            data = data.mid(openingBody, closingBody + 8);

            QRegularExpression re("<\\s*([^a][a-z][a-z0-9]*)\\s.*?>");

            // Fix stupid HTML. Who the hell wrote this junk?
            data = data.remove("<td width=\"5%\">");
            data = data.replace("href=", "href=\"");
            data = data.replace(".PDF", ".PDF\"");

            data = data.replace(re, "<\\1>");
            // Fix stupid broken HTML. Who the hell developed this?
            //data = data.remove("<td width=\"5%\">");
            qDebug() << data;

            ausData = data;
        }

        //qDebug() << data;

        //HTMLParser *parser = new HTMLParser();
        //parser->parse(data);

        //qDebug() << ausData;
        source->setData(ausData);

        AusContentHandler *handler = new AusContentHandler(m_airportID, getAirport, getMin, getSID, getStar, getApproach);
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

        emit parseComplete();

    }
}

void AUSService::processDownloadError(const QString &errorString)
{
    qCritical() << "Download Error:" << errorString;
    downloadJob->deleteLater();
    emit parseError(errorString);
}

void AUSService::stopDownload()
{
    qDebug() << QTime::currentTime().toString("HH:mm:ss.zzz") << "AUS service cancel download job";
    downloadJob->disconnect();
    downloadJob->cancel();
}
