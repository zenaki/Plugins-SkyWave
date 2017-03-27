QT += core network
QT -= gui

CONFIG += c++11

TARGET = SkyWave
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    controller/worker.cpp

HEADERS += \
    controller/worker.h
