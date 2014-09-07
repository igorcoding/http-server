TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread
QMAKE_LFLAGS += -std=c++11 -pthread

SOURCES += \
    src/main.cc \
    src/http_server.cc \
    src/request.cc \
    src/util/thread_pool.cc \
    src/response.cc \
    src/methods.cc \
    src/util/misc.cc \
    src/header.cc \
    src/http_protocol.cc \
    src/exceptions/malformed_components.cc

HEADERS += \
    src/http_server.h \
    src/request.h \
    src/header.h \
    src/methods.h \
    src/util/thread_pool.h \
    src/util/mapper.h \
    src/response.h \
    src/util/misc.h \
    src/http_protocol.h \
    src/exceptions/malformed_components.h


unix {
    LIBS += -lev -lboost_system
}
