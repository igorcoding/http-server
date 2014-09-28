TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread
QMAKE_LFLAGS += -std=c++11 -pthread

SOURCES += \
    src/main.cc \
    src/http_server.cc \
    src/util/thread_pool.cc \
    src/util/misc.cc \
    src/config/server_config.cc \
    src/config/config_reader.cc \
    src/fs/file.cc \
    src/fs/file_error.cc \
    src/fs/file_reader.cc \
    src/http/header.cc \
    src/http/methods.cc \
    src/http/request.cc \
    src/http/response.cc \
    src/http/exceptions/malformed_components.cc \
    src/http/protocol.cc \
    src/config/config_read_error.cc \
    src/http/request_handler.cc \
    src/fs/file_access_denied.cc \
    src/http/chunk.cc \
    src/fs/cache.cc \
    src/listener.cc

HEADERS += \
    src/http_server.h \
    src/util/thread_pool.h \
    src/util/mapper.h \
    src/util/misc.h \
    src/file_reader/file.h \
    src/file_reader/file_reader.h \
    src/config/server_config.h \
    src/config/config_reader.h \
    src/file_reader/file_error.h \
    src/fs/file.h \
    src/fs/file_error.h \
    src/fs/file_reader.h \
    src/http/header.h \
    src/http/methods.h \
    src/http/request.h \
    src/http/response.h \
    src/http/exceptions/malformed_components.h \
    src/http/protocol.h \
    src/config/config_read_error.h \
    src/http/request_handler.h \
    src/fs/file_access_denied.h \
    src/http/chunk.h \
    src/fs/cache.h \
    src/listener.h


unix {
    LIBS += -lboost_system \
            -lboost_filesystem \
            -lboost_thread \
            -lboost_program_options
}

OTHER_FILES += \
    http.conf
