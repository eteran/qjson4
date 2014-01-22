
#include "QJsonValueRef.h"

#if QT_VERSION < 0x050000

QJsonValueRef::QJsonValueRef(QJsonValue *p) : p_(p) {

}

#endif
