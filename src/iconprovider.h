/*  Image Provider
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QFileIconProvider>
#include <QMimeDatabase>

class IconProvider : public QQuickImageProvider
{
public:

    IconProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

protected:
    QFileIconProvider   m_provider;
    QMimeDatabase       m_mimeDB;
};
#endif // IMAGEPROVIDER_H
