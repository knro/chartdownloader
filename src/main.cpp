/*  Chart Downloader
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include "utils.h"
#include "downloadmanager.h"
#include "iconprovider.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<Utils>("Utils", 1, 0, "Utils");
    qmlRegisterType<DownloadManager>("DownloadManager", 1, 0, "DownloadManager");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    engine.addImageProvider(QLatin1String("iconProvider"), new IconProvider());
    return app.exec();
}
