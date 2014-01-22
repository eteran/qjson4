
#ifndef QJSON_DOCUMENT_H_
#define QJSON_DOCUMENT_H_

#include <QtCore/QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtCore/QJsonDocument>
#else

class QVariant;
class QByteArray;
class QTextStream;
class QJsonObject;
class QJsonValue;
class QJsonArray;
class QJsonParseError;
class QJsonRoot;

class QJsonDocument {
public:
	enum DataValidation {
		Validate         = 0,
		BypassValidation = 1
	};
	
	enum JsonFormat {
		Indented,
		Compact
	};

public:
	QJsonDocument();
	QJsonDocument(const QJsonObject &object);
	QJsonDocument(const QJsonArray &array);
	QJsonDocument(const QJsonDocument &other);
	~QJsonDocument();

public:
	QJsonDocument &operator=(const QJsonDocument &other);

public:
	bool operator!=(const QJsonDocument &other) const;
	bool operator==(const QJsonDocument &other) const;

public:
	bool isArray() const;
	bool isEmpty() const;
	bool isNull() const;
	bool isObject() const;

public:
	QByteArray toBinaryData() const;
	QByteArray toJson(JsonFormat format = Indented) const;
	QVariant toVariant() const;

public:
	QJsonArray array() const;
	QJsonObject object() const;
	const char *rawData(int *size) const;
	
public:
	void setArray(const QJsonArray &array);
	void setObject(const QJsonObject &object);

public: 
	static QJsonDocument fromBinaryData(const QByteArray &data, DataValidation validation = Validate);
	static QJsonDocument fromJson(const QByteArray &json, QJsonParseError *error = 0);
	static QJsonDocument fromRawData(const char *data, int size, DataValidation validation = Validate);
	static QJsonDocument fromVariant(const QVariant &variant);
	
private:
	void setRoot(const QJsonRoot &root);
	QString toJson(const QJsonValue &v, JsonFormat format) const;
	QString escapeString(const QString &s) const;
	
private:
	void swap(QJsonDocument &other);
	
private:
	QJsonRoot *root_;
};

#endif

#endif
