QT += qml quick widgets network xml

CONFIG += c++11

SOURCES += src\main.cpp \
           src\utils.cpp \
           src\filedownloader.cpp \
           src\mycontenthandler.cpp \
           src\downloadmanager.cpp \
           src\airport.cpp \
           src\serviceprovider.cpp \
           src\faaservice.cpp \
           src\ausservice.cpp \
           src\auscontenthandler.cpp \
           src\chartinfo.cpp \
           src\iconprovider.cpp \

RESOURCES += qml.qrc

RC_ICONS = chartdownloader.ico

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#QMAKE_LFLAGS += -static

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += src\utils.h \
           src\filedownloader.h \
           src\mycontenthandler.h \
           src\downloadmanager.h \
           src\airport.h \
           src\serviceprovider.h \
           src\faaservice.h \
           src\ausservice.h \
           src\auscontenthandler.h \
           src\chartinfo.h \
           src\iconprovider.h

linux: CONFIG += link_pkgconfig
linux: PKGCONFIG += poppler-qt5

win32:CONFIG(release, debug|release): LIBS += -LR:/lib/ -lpoppler-qt5
else:win32:CONFIG(debug, debug|release): LIBS += -LZ:/lib/ -lpoppler-qt5

win32:CONFIG(release, debug|release): INCLUDEPATH += R:/include
else:win32:CONFIG(debug, debug|release): Z:/include

macx: LIBS += -L/usr/local/lib -lpoppler-qt5
macx: INCLUDEPATH += /usr/local/include/poppler/qt5
ICON = chartdownloader.icns
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.3
