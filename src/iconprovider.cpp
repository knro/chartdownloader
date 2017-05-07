/*  Image Provider
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include "iconprovider.h"

IconProvider::IconProvider():QQuickImageProvider(QQuickImageProvider::Pixmap) { }

QPixmap IconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{       
    if (QFileInfo(id).isDir())
    {
        int width = requestedSize.width() > 0 ? requestedSize.width() : 64;
        int height = width;
        if (size) *size = QSize(width, height);
        return m_provider.icon(QFileIconProvider::Folder).pixmap(width,height);
    }
    else
    {
        // Memory footprint?
        QPixmap preview;
        preview.load(id);
        if (size) *size = QSize(preview.width(), preview.height());
        return preview;
    }
}
