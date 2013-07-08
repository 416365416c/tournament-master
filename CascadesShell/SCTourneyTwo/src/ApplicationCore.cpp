/*
 * ApplicationCore.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: Alan
 */

#include "ApplicationCore.h"

ApplicationCore::ApplicationCore(QObject* parent) : QObject(parent)
	, m_l1(false)
	, m_l2(false)
	, m_m1(0)
	, m_m2(0)
{

}

ApplicationCore::~ApplicationCore() {
}

void ApplicationCore::loadEvent(const QString &name) {

}

void ApplicationCore::confirmSchedule(int mId, bool doit) {
}

void ApplicationCore::setWinner(int mId, int which) {
}

void ApplicationCore::setSchedule(QDateTime t) {
}

void ApplicationCore::registerPlayer(QString title, QString name,
		QString email, QString race) {
}
