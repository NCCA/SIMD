TARGET=SSE4Test
QMAKE_CXX=/usr/bin/g++
CONFIG-=qt
SOURCES+=main.cpp
HEADERS+= ../include/simd.h
LIBS+=-L/public/devel/lib -L/usr/local/lib -lgtest -lpthread
INCLUDEPATH+=/public/devel/include
INCLUDEPATH+=/usr/local/include
INCLUDEPATH+=../include/
macx:CONFIG-=app_bundle
CONFIG+=c++11
QMAKE_CXXFLAGS += -mfma -msse4 -m64 -O3 -ffast-math

