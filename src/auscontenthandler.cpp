/*  Content Handler - Australia
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QDebug>

#include "auscontenthandler.h"

AusContentHandler::AusContentHandler(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach)
{
    this->airportID = airportID;
    this->getAirport=getAirport;
    this->getMinimums=getMinimums;
    this->getSID=getSID;
    this->getSTAR=getSTAR;
    this->getApproach=getApproach;
}

bool AusContentHandler::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(qName);


    /*qDebug() << "Tag:" << localName;
    if (atts.count() > 0)
        qDebug() << "Attributes:";
    for (int i=0; i < atts.count(); i++)
        qDebug() << atts.type(i) << "=" << atts.value(i);
        */


    if (localName == "a")
        currentURL = atts.value(0);

    return true;
}

bool AusContentHandler::characters(const QString &ch)
{
    //qDebug() << "Content: \t" << ch;


    QString pcdata = ch.simplified();

    if (pcdata.isEmpty() || currentURL.isEmpty())
        return true;

    if (pcdata.contains("AERODROME") && getAirport)
        currentTag = ChartInfo::AIRPORT;
    else if (pcdata.contains("ARRIVAL") && getSTAR)
        currentTag = ChartInfo::STAR;
    else if (pcdata.contains("SID") && getSID)
        currentTag = ChartInfo::SID;
    else if (getApproach)
        currentTag = ChartInfo::IAP;

    ChartInfo *oneChart = new ChartInfo(airportID);
    oneChart->type = currentTag;
    oneChart->name = pcdata.remove('/');
    oneChart->url  = "https://www.airservicesaustralia.com/aip/pending/dap/" + currentURL;

    charts.append(oneChart);

    currentURL.clear();

    return true;
}

bool AusContentHandler::error(const QXmlParseException &exception)
{
    qWarning() << "Parsing error on line" << exception.lineNumber()
               << ", column" << exception.columnNumber() << ':'
               << exception.message();

    return true;
}

bool AusContentHandler::fatalError (const QXmlParseException & exception)
{
      qWarning() << "Fatal error on line" << exception.lineNumber()
                 << ", column" << exception.columnNumber() << ':'
                 << exception.message();

      return false;
}
