
#include "QJsonValue.h"
#include "QJsonDocument.h"
#include "QJsonArray.h"
#include "QJsonObject.h"
#include "QJsonParseError.h"
#include <QFile>
#include <QCoreApplication>
#include <QTextCodec>
#include <QtDebug>

int main(int argc, char *argv[]) {

	QCoreApplication app(argc, argv);

#if QT_VERSION < 0x050000
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif


	QFile f("test.json");
	if(f.open(QIODevice::ReadOnly)) {
		QJsonParseError e;
		QJsonDocument d = QJsonDocument::fromJson(f.readAll(), &e);
		if(!d.isNull()) {
			qDebug() << d.toJson(QJsonDocument::Compact);
		} else {
			qDebug() << e.errorString();
		}
	}
}
