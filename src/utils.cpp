/*  General Utilities
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QDir>

#include "utils.h"

Utils::Utils(QObject *parent) : QObject(parent)
{    

}

QString Utils::convertUrlToNativeFilePath(const QUrl& urlStylePath) const
{
    return QDir::toNativeSeparators(urlStylePath.toLocalFile());
}

QUrl Utils::convertPathToURL(const QString& path) const
{
    return QUrl::fromLocalFile(path);
}


