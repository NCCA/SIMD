TARGET=Quadratic
CONFIG-=qt
SOURCES+=main.cpp
HEADERS+= ../include/simd.h
LIBS+=-L/public/devel/lib -L/usr/local/lib -lgtest -lbenchmark -lpthread
INCLUDEPATH+=/public/devel/include
INCLUDEPATH+=/usr/local/include
INCLUDEPATH+=../include/
macx:CONFIG-=app_bundle
CONFIG+=c++11
QMAKE_CXXFLAGS += -mfma -msse4 -m64 -O3 -ffast-math

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
  message("including $HOME/NGL")
  include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
  message("Using custom NGL location")
  include($(NGLDIR)/UseNGL.pri)
}



