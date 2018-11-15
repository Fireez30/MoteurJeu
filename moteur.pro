QT       += core gui widgets

TARGET = moteur
TEMPLATE = app

SOURCES += main.cpp \
    windowsinput.cpp \
    baseobject.cpp \
    terrain.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    windowsinput.h \
    baseobject.h \
    terrain.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = C:\Users\Fireez\Documents\GitHub\MoteurJeu
INSTALLS += target

DISTFILES +=
