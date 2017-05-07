/*  Chart Info
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include "chartinfo.h"

ChartInfo::ChartInfo(const QString &airportID)
{
    this->airportID = airportID;
}

// Can't use Q_ENUM because we're not child of QObject
QString ChartInfo::getTypeDescription()
{
    switch (type)
    {
    case AIRPORT:
        return "AIRPORT";
        break;

    case MINIMUMS:
        return "MINIMUMS";
        break;

    case SID:
        return "SIDS";
        break;

    case STAR:
        return "STARS";
        break;

    case IAP:
        return "APPROACHES";
        break;

    default:
        return "UNKNOWN";
        break;
    }
}
