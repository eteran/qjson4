/*
Copyright (C) 2014 - 2014 Evan Teran
                          eteran@alum.rit.edu

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

#if QT_VERSION < 0x050000
	// unfortunately this part seems needed for Qt4 :-(
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
