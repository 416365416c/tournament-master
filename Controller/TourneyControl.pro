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
    qml/TourneyDisplay.qml \
    notes.txt \
    qml/Line.qml \
    qml/LineControl.js \
    qml/MatchEditor.qml \
    qml/Button.qml \
    qml/LineEdit.qml \
    qml/CheckBox.qml \
    qml/MyText.qml \
    qml/DisplayDelegate.qml

RESOURCES +=
