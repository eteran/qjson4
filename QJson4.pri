DEPENDPATH  += .
INCLUDEPATH += .

# Input
HEADERS += $$PWD/QJsonArray.h        \
           $$PWD/QJsonDocument.h     \
           $$PWD/QJsonObject.h       \
           $$PWD/QJsonParseError.h   \
           $$PWD/QJsonValue.h        \
           $$PWD/QJsonValueRef.h     \
           $$PWD/QJsonParser.h       \
           $$PWD/QJsonRoot.h

SOURCES += $$PWD/QJsonArray.cpp      \
           $$PWD/QJsonDocument.cpp   \
           $$PWD/QJsonObject.cpp     \
           $$PWD/QJsonParseError.cpp \
           $$PWD/QJsonValue.cpp      \
           $$PWD/QJsonValueRef.cpp   \
           $$PWD/QJsonParser.cpp
