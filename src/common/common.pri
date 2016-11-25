
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += network

# directories ($$PWD) are set before files because of an error in Qt5

SOURCES += $$PWD/datamodel.cpp \
           $$PWD/comm.cpp \
           $$PWD/messageclientserver.cpp \
           $$PWD/utilclasses.cpp \
           $$PWD/uuid.cpp \
           $$PWD/controlsignaling.cpp

HEADERS += $$PWD/datamodel.h \
           $$PWD/comm.h \
           $$PWD/messageclientserver.h \
           $$PWD/utilclasses.h \
           $$PWD/uuid.h \
           $$PWD/controlsignaling.h \
           $$PWD/ohtbaseconfig.h \
           $$PWD/debug.h

# define platform variable
    DEFINES += LINUX_OHT  _GLIBCXX_USE_CXX11_ABI=0

# add here your boost includes if needed
    LIBS += -L/home/fbergmann/Development/boost_1_62_0/stage/lib/
    INCLUDEPATH += /home/fbergmann/Development/boost_1_62_0
    LIBS += -lboost_serialization -lboost_system -lboost_thread

# define platform variable
linux {
    DEFINES += LINUX_OHT _GLIBCXX_USE_CXX11_ABI=0
}
win32 {
    DEFINES += WINDOWS_OHT
}



# add here your boost includes if needed
linux {
    message(" Linux Boost libs selected")
    LIBS += -L/home/fbergmann/Development/boost_1_62_0/stage/lib/
    INCLUDEPATH += /home/fbergmann/Development/boost_1_62_0
    LIBS += -lboost_serialization
}
win32 {
    message(" Win32 Boost libs selected")
    LIBS += -LC:/boost/lib
    INCLUDEPATH += C:/boost/include/
    LIBS += -lboost_serialization-mgw49-mt-d-1_60
}


