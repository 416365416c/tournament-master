#-------------------------------------------------
#
# Project created by QtCreator 2010-04-28T20:00:55
#
#-------------------------------------------------

QT       += core gui widgets qml quick
DEFINES += NEW_DECLARATIVE
TARGET = TourneyControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../Common/tournament.cpp \
    ../Common/player.cpp \
    ../Common/match.cpp

HEADERS  += mainwindow.h \
    ../Common/tournament.h \
    ../Common/player.h \
    ../Common/match.h

FORMS    += mainwindow.ui

QML_FILES += \
    ../Common/qml/TourneyDisplay.qml \
    ../Common/qml/Line.qml \
    ../Common/qml/LineControl.js \
    ../Common/qml/MatchEditor.qml \
    ../Common/qml/Button.qml \
    ../Common/qml/LineEdit.qml \
    ../Common/qml/CheckBox.qml \
    ../Common/qml/MyText.qml \
    ../Common/qml/DisplayDelegate.qml

RESOURCES +=
