
#ifndef QJSON_BASE_H_
#define QJSON_BASE_H_

#include <QtCore/QtGlobal>

#if QT_VERSION < 0x050000

class QJsonObject;
class QJsonArray;

class QJsonRoot {
public:
	virtual ~QJsonRoot() {};

public:
	virtual QJsonRoot *clone() const = 0;

public:
	virtual QJsonArray *toArray() = 0;
	virtual QJsonObject *toObject() = 0;
	virtual const QJsonArray *toArray() const = 0;
	virtual const QJsonObject *toObject() const = 0;
};

#endif

#endif
