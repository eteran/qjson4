
#ifndef QJSON_VALUEREF_H_
#define QJSON_VALUEREF_H_

#include <QtCore/QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtCore/QJsonValueRef>
#else

class QJsonValue;

class QJsonValueRef {
public:
	explicit QJsonValueRef(QJsonValue *p);

private:
	QJsonValue *const p_;
};

#endif

#endif
