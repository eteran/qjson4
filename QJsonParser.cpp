
#include "QJsonParser.h"
#include "QJsonArray.h"
#include "QJsonObject.h"
#include "QJsonValue.h"


#if QT_VERSION < 0x050000

#include <cctype>
#include <QScopedPointer>
#include <QVector>

namespace {

void throw_error(QJsonParseError::ParseError e, int offset) {
	QJsonParseError err;
	err.error  = e;
	err.offset = offset;
	throw err;
}

unsigned int to_hex(int ch) {

	static const int hexval[256] = {
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	};

	if(static_cast<unsigned int>(ch) < 256) {
		return hexval[static_cast<unsigned int>(ch)];
	} else {
		return 0;
	}
}

}

//------------------------------------------------------------------------------
// Name: QJsonParser
//------------------------------------------------------------------------------
QJsonParser::QJsonParser(const char *begin, const char *end) : begin_(begin), end_(end), p_(begin) {
	state_.error  = QJsonParseError::NoError;
	state_.offset = 0;
}

//------------------------------------------------------------------------------
// Name: parse
//------------------------------------------------------------------------------
QJsonRoot *QJsonParser::parse() {
	if(begin_ == end_) {
		return 0;
	}
	
	try {	
		const char ch = peek();
		switch(ch) {
		case '[':
			return getArray();
		case '{':
			return getObject();
		default:
			state_.error  = QJsonParseError::IllegalValue;
			state_.offset = p_ - begin_;
			return 0;
		}
	} catch(const QJsonParseError &e) {
		state_ = e;
		return 0;
	}
}

//------------------------------------------------------------------------------
// Name: peek
//------------------------------------------------------------------------------
char QJsonParser::peek() {
	// first eat up some whitespace
	while(p_ != end_ && isspace(*p_)) {
		++p_;
	}

	return *p_;
}

//------------------------------------------------------------------------------
// Name: getValue
//------------------------------------------------------------------------------
QJsonValue QJsonParser::getValue() {

	switch(peek()) {
	case '{':
		{
			QScopedPointer<QJsonObject> obj(getObject());
			return QJsonValue(*obj);
		}
	case '[':
		{
			QScopedPointer<QJsonArray> arr(getArray());
			return QJsonValue(*arr);
		}
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '-':
		return getNumber();
	case '"':
		return QJsonValue(getString());
	case 't':
		return getTrue();
	case 'f':
		return getFalse();
	case 'n':
		return getNull();
	}
	
	throw_error(QJsonParseError::MissingObject, p_ - begin_);
	return QJsonValue();
}

//------------------------------------------------------------------------------
// Name: getObject
//------------------------------------------------------------------------------
QJsonObject *QJsonParser::getObject() {

	QScopedPointer<QJsonObject> obj(new QJsonObject);

	char tok = peek();
	if(tok != '{') {
		throw_error(QJsonParseError::IllegalValue, p_ - begin_);
	}
	
	++p_;

	// handle empty object
	if(peek() == '}') {
		tok = '}';
		++p_;
	} else {

		do {
			QPair<QString, QJsonValue> p = getPair();
			obj->values_.insert(p.first, p.second);

			tok = peek();
			++p_;

		} while(tok == ',');
	}

	if(tok != '}') {
		throw_error(QJsonParseError::UnterminatedObject, p_ - begin_);
	}

	return obj.take();
}

//------------------------------------------------------------------------------
// Name: getArray
//------------------------------------------------------------------------------
QJsonArray *QJsonParser::getArray() {

	QScopedPointer<QJsonArray> arr(new QJsonArray);

	char tok = peek();
	
	if(tok != '[') {
		throw_error(QJsonParseError::IllegalValue, p_ - begin_);
	}
	
	++p_;

	// handle empty object
	if(peek() == ']') {
		tok = ']';
		++p_;
	} else {
		do {
			arr->values_.push_back(getValue());

			tok = peek();
			++p_;
			
		} while(tok == ',');
	}

	if(tok != ']') {
		throw_error(QJsonParseError::MissingValueSeparator, p_ - begin_);
	}

	return arr.take();
}

//------------------------------------------------------------------------------
// Name: getPair
//------------------------------------------------------------------------------
QPair<QString, QJsonValue> QJsonParser::getPair() {
	
	QString key = getString();

	if(peek() != ':') {
		throw_error(QJsonParseError::MissingNameSeparator, p_ - begin_);
	}
	++p_;

	return qMakePair(key, getValue());
}

//------------------------------------------------------------------------------
// Name: getString
//------------------------------------------------------------------------------
QString QJsonParser::getString() {
	
	if(peek() != '"') {
		throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
	}
	++p_;

	QString s;

	while(p_ != end_ && *p_ != '"' && *p_ != '\n') {
		if(*p_ == '\\') {
			++p_;
			if(p_ != end_) {
				switch(*p_) {
				case '"':  s.append('"'); break;
				case '\\': s.append('\\'); break;
				case '/':  s.append('/'); break;
				case 'b':  s.append('\b'); break;
				case 'f':  s.append('\f'); break;
				case 'n':  s.append('\n'); break;
				case 'r':  s.append('\r'); break;
				case 't':  s.append('\t'); break;
				case 'u':
					{
						// convert \uXXXX escape sequences to UTF-8
						char hex[4];
						if(p_ == end_) { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); } hex[0] = *++p_;
						if(p_ == end_) { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); } hex[1] = *++p_;
						if(p_ == end_) { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); } hex[2] = *++p_;
						if(p_ == end_) { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); } hex[3] = *++p_;

						if(!std::isxdigit(hex[0])) throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
						if(!std::isxdigit(hex[1])) throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
						if(!std::isxdigit(hex[2])) throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
						if(!std::isxdigit(hex[3])) throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);

						quint16 w1 = 0;
						quint16 w2 = 0;
						
						w1 |= (to_hex(hex[0]) << 12);
						w1 |= (to_hex(hex[1]) << 8);
						w1 |= (to_hex(hex[2]) << 4);
						w1 |= (to_hex(hex[3]));
						
						s.append(QChar(w1));
						
						if((w1 & 0xfc00) == 0xdc00) {
							throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
						}				

						if((w1 & 0xfc00) == 0xd800) {
							// part of a surrogate pair
							if(p_ == end_ || *++p_ != '\\') { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); }
							if(p_ == end_ || *++p_ != 'u')  { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); }
							
							// convert \uXXXX escape sequences to UTF-8
							if(p_ == end_) { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); } hex[0] = *++p_;
							if(p_ == end_) { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); } hex[1] = *++p_;
							if(p_ == end_) { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); } hex[2] = *++p_;
							if(p_ == end_) { throw_error(QJsonParseError::IllegalEscapeSequence, p_ - begin_); } hex[3] = *++p_;

							if(!std::isxdigit(hex[0])) throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
							if(!std::isxdigit(hex[1])) throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
							if(!std::isxdigit(hex[2])) throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
							if(!std::isxdigit(hex[3])) throw_error(QJsonParseError::IllegalUTF8String, p_ - begin_);
							
							w2 |= (to_hex(hex[0]) << 12);
							w2 |= (to_hex(hex[1]) << 8);
							w2 |= (to_hex(hex[2]) << 4);
							w2 |= (to_hex(hex[3]));
							
							s.append(QChar(w2));					
						}
					}
					break;

				default:
					s.append('\\');
					break;
				}
			}
		} else {
			s.append(*p_);
		}
		++p_;
	}

	if(*p_ != '"' || p_ == end_) {
		throw_error(QJsonParseError::UnterminatedString, p_ - begin_);
	}

	++p_;
	
	return s;
}

//------------------------------------------------------------------------------
// Name: getNull
//------------------------------------------------------------------------------
QJsonValue QJsonParser::getNull() {
	if(p_ == end_ || *p_++ != 'n') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'u') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'l') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'l') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }

	return QJsonValue();
}

//------------------------------------------------------------------------------
// Name: getTrue
//------------------------------------------------------------------------------
QJsonValue QJsonParser::getTrue() {
	if(p_ == end_ || *p_++ != 't') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'r') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'u') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'e') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }

	return QJsonValue(true);
}

//------------------------------------------------------------------------------
// Name: getFalse
//------------------------------------------------------------------------------
QJsonValue QJsonParser::getFalse() {
	if(p_ == end_ || *p_++ != 'f') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'a') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'l') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 's') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }
	if(p_ == end_ || *p_++ != 'e') { throw_error(QJsonParseError::IllegalValue, p_ - begin_); }

	return QJsonValue(false);
}

//------------------------------------------------------------------------------
// Name: getNumber
//------------------------------------------------------------------------------
QJsonValue QJsonParser::getNumber() {
	// JSON numbers fit the regex: -?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)?

	const char *const first = p_;

	// -?
	if(p_ != end_ && *p_ == '-') {
		++p_;
	}

	// (0|[1-9][0-9]*)
	if(p_ != end_) {
		if(*p_ >= '1' && *p_ <= '9') {
			while(p_ != end_ && std::isdigit(*p_)) {
				++p_;
			}
		} else if(*p_ == '0') {
			++p_;
		} else {
			throw_error(QJsonParseError::IllegalNumber, p_ - begin_);
		}
	}

	// (\.[0-9]+)?
	if(p_ != end_ && *p_ == '.') {
		++p_;
		if(!std::isdigit(*p_)) {
			throw_error(QJsonParseError::IllegalNumber, p_ - begin_);
		}

		while(p_ != end_ && std::isdigit(*p_)) {
			++p_;
		}
	}

	// ([eE][+-]?[0-9]+)?
	if(p_ != end_ && (*p_ == 'e' || *p_ == 'E')) {
		++p_;
		if(p_ != end_ && (*p_ == '+' || *p_ == '-')) {
			++p_;
		}
		if(!std::isdigit(*p_)) {
			throw_error(QJsonParseError::IllegalNumber, p_ - begin_);
		}
		while(p_ != end_ && std::isdigit(*p_)) {
			++p_;
		}
	}
	
	if(p_ == end_) {
		throw_error(QJsonParseError::TerminationByNumber, p_ - begin_);
	}
	
	return QJsonValue(QByteArray::fromRawData(first, p_ - first).toDouble());
}

//------------------------------------------------------------------------------
// Name: state
//------------------------------------------------------------------------------
QJsonParseError QJsonParser::state() const {
	return state_;
}

#endif
