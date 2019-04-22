TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -g -O2 -Wall -Werror -std=c99

SOURCES += main.c
