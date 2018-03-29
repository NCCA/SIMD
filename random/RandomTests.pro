TARGET=RandomTests
QMAKE_CXX=/usr/bin/g++
CONFIG-=qt
SOURCES+=$$PWD/random.cpp \
         $$PWD/randomTests.cpp
HEADERS+= ../include/simd.h \
          $$PWD/random.h
LIBS+=-L/public/devel/lib -L/usr/local/lib -lgtest -lpthread
INCLUDEPATH+=/public/devel/include
INCLUDEPATH+=/usr/local/include
INCLUDEPATH+=../include/
macx:CONFIG-=app_bundle
CONFIG+=c++11
QMAKE_CXXFLAGS += -mfma -mavx2 -m64 -mf16c -O3

DEFINES+=UNITTESTS
