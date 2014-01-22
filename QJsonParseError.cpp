
#include "QJsonParseError.h"

#if QT_VERSION < 0x050000

//------------------------------------------------------------------------------
// Name: errorString
// Desc: The QJsonParseError class is used to report errors during JSON parsing.
//------------------------------------------------------------------------------
QString QJsonParseError::errorString() const {
	switch(error) {
	case NoError:
		return "No error occurred";
	case UnterminatedObject:
		return "unterminated object";
	case MissingNameSeparator:
		return "missing name separator";
	case UnterminatedArray:
		return "unterminated array";
	case MissingValueSeparator:
		return "missing value separator";
	case IllegalValue:
		return "illegal value";
	case TerminationByNumber:
		return "invalid termination by number";
	case IllegalNumber:
		return "illegal number";
	case IllegalEscapeSequence:
		return "illegal escape sequence";
	case IllegalUTF8String:
		return "invalid UTF8 string";
	case UnterminatedString:
		return "unterminated string";
	case MissingObject:
		return "object is missing after a comma";
	case DeepNesting:
		return "too deeply nested document";
	case DocumentTooLarge:
		return "too large document";
	}

	return QString();
}

#endif
