# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

CONFIG(debug, debug|release):   BUILD_CONFIG = debug
CONFIG(release, debug|release): BUILD_CONFIG = release

symbian:TARGET.UID3 = 0xE57DC587

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

INCLUDEPATH += ../qjson4/src
LIBS += -L"../../qjson4/bin/$$BUILD_CONFIG" -lqjson4
SOURCES += main.cpp
HEADERS +=
FORMS +=

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()
