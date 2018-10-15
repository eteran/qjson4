#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T13:36:19
#
#-------------------------------------------------

QT       -= gui

TARGET = qjson4
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/QJsonValueRef.cpp \
    src/QJsonValue.cpp \
    src/QJsonParser.cpp \
    src/QJsonParseError.cpp \
    src/QJsonObject.cpp \
    src/QJsonDocument.cpp \
    src/QJsonArray.cpp


HEADERS += \
    src/QJsonValueRef.h \
    src/QJsonValue.h \
    src/QJsonRoot.h \
    src/QJsonParser.h \
    src/QJsonParseError.h \
    src/QJsonObject.h \
    src/QJsonDocument.h \
    src/QJsonArray.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
