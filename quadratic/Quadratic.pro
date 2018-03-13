TARGET=Quadratic
#CONFIG-=qt core
SOURCES+=main.cpp
HEADERS+= ../include/simd.h
LIBS+=-L/public/devel/lib  -lgtest -lbenchmark -lpthread
INCLUDEPATH+=/public/devel/include
INCLUDEPATH+=/usr/local/include
INCLUDEPATH+=../include/
macx:CONFIG-=app_bundle
CONFIG+=c++11
QMAKE_CXXFLAGS += -mfma -mavx2 -m64 -O3 -ffast-math

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
  message("including $HOME/NGL")
  include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
  message("Using custom NGL location")
  include($(NGLDIR)/UseNGL.pri)
}



