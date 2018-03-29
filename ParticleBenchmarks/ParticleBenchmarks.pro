# This specifies the exe name
TARGET=Particle
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core
# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
	cache()
	DEFINES +=QT5BUILD
}
DEFINES+=GOOGLEBENCH
# where to put moc auto generated files
MOC_DIR=moc
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)

QMAKE_CXXFLAGS += -mfma -mavx2 -mf16c -m64 -O3 -ffast-math
# same for the .h files
SOURCES += $$PWD/src/main.cpp \
$$PWD/src/ParticleSystemSSE.cpp \
$$PWD/src/ParticleSystemSSEFMA.cpp \
$$PWD/src/ParticleSystemSSERand.cpp \
$$PWD/src/ParticleSystemAVX2.cpp \
$$PWD/src/ParticleSystemNormal.cpp \
$$PWD/src/ParticleSystemAOS.cpp \
$$PWD/src/ParticleSSE.cpp \
$$PWD/src/random.cpp
INCLUDEPATH+=/public/devel/include/
LIBS+=-L/public/devel/lib/ -lbenchmark

HEADERS+=$$PWD/include/ParticleSSE.h \
          $$PWD/include/ParticleSystemSSE.h \
          $$PWD/include/ParticleSystemSSEFMA.h \
          $$PWD/include/ParticleSystemSSERand.h \
          $$PWD/include/ParticleSystemAVX2.h \
          $$PWD/include/ParticleSystemNormal.h \
          $$PWD/include/ParticleSystemAOS.h \
          $$PWD/include/AlignedArray.h \
          ../include/simd.h
# and add the include dir into the search path for Qt and make
INCLUDEPATH+=./include
INCLUDEPATH+= $$PWD/../include/
# where our exe is going to live (root of project)
DESTDIR=./
# add the glsl shader files
OTHER_FILES+= shaders/*.glsl  \
              README.md
# were are going to default to a console app
CONFIG += console
# note each command you add needs a ; as it will be run as a single line
# first check if we are shadow building or not easiest way is to check out against current
!equals(PWD, $${OUT_PWD}){
	copydata.commands = echo "creating destination dirs" ;
	# now make a dir
	copydata.commands += mkdir -p $$OUT_PWD/shaders ;
	copydata.commands += echo "copying files" ;
	# then copy the files
	copydata.commands += $(COPY_DIR) $$PWD/shaders/* $$OUT_PWD/shaders/ ;
	# now make sure the first target is built before copy
	first.depends = $(first) copydata
	export(first.depends)
	export(copydata.commands)
	# now add it as an extra target
	QMAKE_EXTRA_TARGETS += first copydata
}
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
	message("including $HOME/NGL")
	include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
	message("Using custom NGL location")
	include($(NGLDIR)/UseNGL.pri)
}

