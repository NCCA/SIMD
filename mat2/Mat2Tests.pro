# This specifies the exe name
TARGET=Mat2Tests
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core
 cache()
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/Mat2Tests.cpp \
          $$PWD/Mat2Func.cpp

HEADERS+= $$PWD/Mat2Func.h

# same for the .h files

DEPENDPATH+=$$PWD/include
# and add the include dir into the search path for Qt and make
INCLUDEPATH +=./include
# where our exe is going to live (root of project)
DESTDIR=./
# were are going to default to a console app
CONFIG += console
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lgtest -lgtest_main

QMAKE_CXXFLAGS+=-O3 -ffast-math  -mavx2 -mfma -mf16c
