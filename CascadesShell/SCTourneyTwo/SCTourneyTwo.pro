APP_NAME = SCTourneyTwo
QT += network
CONFIG += qt warn_on cascades10

SOURCES += ../../Common/tournament.cpp \
	../../Common/player.cpp \
	../../Common/match.cpp

HEADERS += ../../Common/tournament.h \
	../../Common/player.h \
	../../Common/match.h
	
include(config.pri)
