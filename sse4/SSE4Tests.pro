TARGET=SSE4Test
CONFIG-=qt
SOURCES+=main.cpp
HEADERS+=
LIBS+=-L/public/devel/lib -L/usr/local/lib -lgtest -lpthread
INCLUDEPATH+=/public/devel/include
INCLUDEPATH+=/usr/local/include
INCLUDEPATH+=../include/
macx:CONFIG-=app_bundle
CONFIG+=c++11
QMAKE_CXXFLAGS += -mfma -msse4 -m64 -O3 -ffast-math

