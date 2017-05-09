/*  Content Handler (FAA Only)
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef MYCONTENTHANDLER_H
#define MYCONTENTHANDLER_H

#include <QXmlDefaultHandler>
#include "chartinfo.h"

class MyContentHandler : public QXmlDefaultHandler
{
public:    

    MyContentHandler(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach);

    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);

    bool characters(const QString &ch);

    bool error(const QXmlParseException &exception);

    bool fatalError(const QXmlParseException & exception);

    QVector<ChartInfo*> charts;

    ChartInfo::ChartType currentTag = ChartInfo::UNKNOWN;

    QString currentURL;
    QString airportID;

    bool getAirport=false, getMinimums=false, getSID=false, getSTAR=false, getApproach=false;
};

#endif // MYCONTENTHANDLER_H
