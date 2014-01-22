
#ifndef QJSON_PARSER_H_
#define QJSON_PARSER_H_

#include <QtCore/QtGlobal>

#if QT_VERSION < 0x050000

#include "QJsonParseError.h"
#include <QPair>
class QJsonRoot;
class QJsonObject;
class QJsonArray;
class QJsonValue;

class QJsonParser {
	friend class QJsonDocument;

public:
	QJsonParser(const char *begin, const char *end);
	
public:
	QJsonRoot *parse();

public:
	QJsonParseError state() const;
	
private:
	char peek();
	QJsonObject *getObject();
	QJsonArray *getArray();
	QJsonValue getValue();
	QString getString();
	QJsonValue getTrue();
	QJsonValue getFalse();
	QJsonValue getNull();
	QJsonValue getNumber();
	QPair<QString, QJsonValue> getPair();

private:
	QJsonParseError   state_;
	const char *const begin_;
	const char *const end_;
	const char *      p_;
};

#endif

#endif
