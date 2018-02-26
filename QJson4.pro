
TEMPLATE = app
TARGET   = test

include($$PWD/QJson4.pri)

SOURCES += main.cpp

CONFIG(debug, debug|release) {
	OBJECTS_DIR = $${OUT_PWD}/.debug/obj
	MOC_DIR     = $${OUT_PWD}/.debug/moc
	RCC_DIR     = $${OUT_PWD}/.debug/rcc
	UI_DIR      = $${OUT_PWD}/.debug/uic
}

CONFIG(release, debug|release) {
	OBJECTS_DIR = $${OUT_PWD}/.release/obj
	MOC_DIR     = $${OUT_PWD}/.release/moc
	RCC_DIR     = $${OUT_PWD}/.release/rcc
	UI_DIR      = $${OUT_PWD}/.release/uic
}
