/*  Airport Info
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef AIRPORT_H
#define AIRPORT_H

#include <QVector>

class Airport
{
public:
    Airport();

    QString name;
    QVector<QPair<QString,QString>> airportInfo, minInfo, sidInfo, starInfo, approachInfo;

};

#endif // AIRPORT_H
