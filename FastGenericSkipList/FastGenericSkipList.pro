TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    skiplist.h

# remove lower optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add full optimization
QMAKE_CXXFLAGS_RELEASE *= -Ox
