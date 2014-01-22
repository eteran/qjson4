
#ifndef QJSON_PARSEERROR_H_
#define QJSON_PARSEERROR_H_

#include <QtCore/QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtCore/QJsonParseError>
#else

#include <QtCore/QString>

class QJsonParseError {
public:
	enum ParseError {
		NoError 			   = 0,
		UnterminatedObject     = 1,
		MissingNameSeparator   = 2,
		UnterminatedArray	   = 3,
		MissingValueSeparator  = 4,
		IllegalValue		   = 5,
		TerminationByNumber    = 6,
		IllegalNumber		   = 7,
		IllegalEscapeSequence  = 8,
		IllegalUTF8String	   = 9,
		UnterminatedString     = 10,
		MissingObject		   = 11,
		DeepNesting 		   = 12,
		DocumentTooLarge       = 13
	};

public:
	QString errorString() const;

public:
	ParseError error;
	int        offset;
};

#endif

#endif
