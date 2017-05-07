/*  Airport Info
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef CHARTINFO_H
#define CHARTINFO_H

#include <QString>

class QImage;

class ChartInfo
{
public:    
    ChartInfo(const QString &airportID);

    enum ChartType { UNKNOWN, AIRPORT, MINIMUMS, SID, STAR, IAP };

    QString airportID;

    QString name;
    QString url;

    ChartType type = AIRPORT;
    QString getTypeDescription();

    QString filename;
    // Always PDF for now
    QString fileType = "pdf";

    QImage *image = nullptr;
    // Always PNG for now
    QString imageType = "png";


};

#endif // CHARTINFO_H
