/*  Content Handler
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QDebug>

#include "mycontenthandler.h"

MyContentHandler::MyContentHandler(const QString &airportID)
{
    this->airportID = airportID;
}

bool MyContentHandler::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(qName);

#if 0
    qDebug() << "Tag:" << localName;
    if (atts.count() > 0)
        qDebug() << "Attributes:";
    for (int i=0; i < atts.count(); i++)
        qDebug() << atts.type(i) << "=" << atts.value(i);
#endif

    if (localName == "a")
        currentURL = atts.value(0);

    return true;
}

bool MyContentHandler::characters(const QString &ch)
{
#if 0
    qDebug() << "Content: \t" << ch;
#endif

    QString pcdata = ch.simplified();

    if (pcdata.isEmpty())
        return true;

    if (currentTag == ChartInfo::UNKNOWN)
    {
        if (pcdata == "APD")
            currentTag = ChartInfo::AIRPORT;
        else if (pcdata == "MIN")
            currentTag = ChartInfo::MINIMUMS;
        else if (pcdata == "DP")
            currentTag = ChartInfo::SID;
        else if (pcdata == "STAR")
            currentTag = ChartInfo::STAR;
        else if (pcdata == "IAP")
            currentTag = ChartInfo::IAP;

        return true;
    }

    ChartInfo *oneChart = new ChartInfo(airportID);
    oneChart->type = currentTag;
    oneChart->name = pcdata.remove('/');
    oneChart->url  = currentURL;

    charts.append(oneChart);

    currentTag = ChartInfo::UNKNOWN;

    return true;
}

bool MyContentHandler::error(const QXmlParseException &exception)
{
    qWarning() << "Parsing error on line" << exception.lineNumber()
               << ", column" << exception.columnNumber() << ':'
               << exception.message();

    return true;
}

bool MyContentHandler::fatalError (const QXmlParseException & exception)
{
      qWarning() << "Fatal error on line" << exception.lineNumber()
                 << ", column" << exception.columnNumber() << ':'
                 << exception.message();

      return false;
}
