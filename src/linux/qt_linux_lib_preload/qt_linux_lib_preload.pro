# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------

equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += widgets testlib
   CONFIG +=
}else {
  message(" ================ QT 4 ================ ")
   QT +=
   CONFIG += qtestlib
}
CONFIG += c++11

QMAKE_CXXFLAGS +=-std=c++11

QT += network xml
CONFIG += debug

TARGET = qt_linux_oht_preload
TEMPLATE = lib


####
#### Qt Lib Preload project
####

include(../../qt_lib_preload/qt_lib_preload.pri)


###
### linux preloading sources
###

INCLUDEPATH += ../../preloaders
SOURCES += ../../preloaders/qtpreloadingcontrol.cpp
HEADERS += ../../preloaders/qtpreloadingcontrol.h



