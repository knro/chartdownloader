/*  General Utilities
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef UTILS_H
#define UTILS_H

#include <QUrl>
#include <QObject>

class Utils : public QObject
{
Q_OBJECT

public:    
    explicit Utils(QObject *parent=nullptr);

    Q_INVOKABLE QString convertUrlToNativeFilePath(const QUrl& urlStylePath) const;
    Q_INVOKABLE QUrl convertPathToURL(const QString &path) const;
};

#endif // UTILS_H
