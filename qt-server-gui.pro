#-------------------------------------------------
#
# Project created by QtCreator 2018-06-30T08:59:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-server-gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        server.cpp

HEADERS += \
        server.h

FORMS += \
        server.ui

INCLUDEPATH += /usr/local/include

# DBOX Preprocessing module
unix:!macx: LIBS += -L$$PWD/../Preprocessing/ -lPreprocessing
INCLUDEPATH += $$PWD/../Preprocessing
DEPENDPATH += $$PWD/../Preprocessing

# DBOX Extraction module
unix:!macx: LIBS += -L$$PWD/../Extraction/ -lExtraction
INCLUDEPATH += $$PWD/../Extraction
DEPENDPATH += $$PWD/../Extraction

# DBOX Matcher module
unix:!macx: LIBS += -L$$PWD/../Matcher/ -lMatcher
INCLUDEPATH += $$PWD/../Matcher
DEPENDPATH += $$PWD/../Matcher

# CUDA
unix:!macx: LIBS += -L/opt/cuda/lib64/ -lcudart
unix:!macx: LIBS += -L/usr/local/cuda/lib64/ -lcudart
INCLUDEPATH += /opt/cuda/include
DEPENDPATH += /opt/cuda/include
INCLUDEPATH += /usr/local/cuda/include
DEPENDPATH += /usr/local/cuda/include

# Suprema BioMini Slim
unix:!macx: LIBS += -L/usr/local/lib/ -lUFMatcher
unix:!macx: LIBS += -L/usr/local/lib64/ -lUFMatcher
INCLUDEPATH += /usr/local/include/suprema

# OpenCV
unix:!macx: LIBS += -L/usr/local/lib64/ -lopencv_world
unix:!macx: LIBS += -L/usr/local/lib/ -lopencv_world

# ArrayFire
unix:!macx: LIBS += -L/usr/lib/ -lafcuda
unix:!macx: LIBS += -L/usr/local/lib/ -lafcuda
unix:!macx: LIBS += -L/usr/local/lib64/ -lafcuda

# Caffe 1.0
unix:!macx: LIBS += -L/usr/local/lib64/ -lcaffe
unix:!macx: LIBS += -L/usr/local/lib/ -lcaffe
unix:!macx: LIBS += -L/usr/lib/ -lboost_system
unix:!macx: LIBS += -L/usr/lib/ -lglog
unix:!macx: LIBS += -L/usr/lib/ -lprotobuf
