/*
Copyright (C) 2014 - 2016 Evan Teran
                          evan.teran@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	QFile f("test.json");
	if(f.open(QIODevice::ReadOnly)) {
		QJsonParseError e;
		QJsonDocument d = QJsonDocument::fromJson(f.readAll(), &e);
		if(!d.isNull() && e.error == QJsonParseError::NoError) {
			qDebug() << QString::fromUtf8(d.toJson(QJsonDocument::Compact));
		} else {
			qDebug() << e.errorString();
		}
	}
	
	qDebug() << "----------------------------------";

	// Regression test for issue #3
	QVariantMap map;
	map.insert("int", INT_MAX);
	map.insert("double", 3.1415926);
	map.insert("name", "hell");
	QJsonDocument doc = QJsonDocument::fromVariant(map);
	qDebug() << QString::fromUtf8(doc.toJson());
}
