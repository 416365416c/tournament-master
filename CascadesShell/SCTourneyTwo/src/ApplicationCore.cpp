/*
 * ApplicationCore.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: Alan
 */

#include "ApplicationCore.h"
#include <QtNetwork>

ApplicationCore::ApplicationCore(QObject* parent) : QObject(parent)
	, m_manager(0)
	, m_server("http://ec2-184-169-240-202.us-west-1.compute.amazonaws.com/cgi-bin/tournamentCgi")
	, m_l1(false)
	, m_l2(false)
	, m_t(0)
	, m_m1(0)
	, m_m2(0)
	, m_xmlCount(-1)
{
	m_manager = new QNetworkAccessManager(this);
	connect(m_manager, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(handleNetworkReply(QNetworkReply*)));
	fetchData();
}

ApplicationCore::~ApplicationCore() {
}

void ApplicationCore::fetchData() {
	QByteArray cmd(QString("password=%1&request=list").arg(SHARED_PASSWORD).toLatin1());
	m_listReply = m_manager->post(m_server, cmd);
}

void ApplicationCore::handleNetworkReply(QNetworkReply* reply)
{
	if(reply == m_listReply) {
		m_xmlCount = 0;
		qDebug() << "Network Alpha" << reply.errorString();
		while (reply.canReadLine()) {
			QUrl xmlUrl = m_server;
			QByteArray path = reply.readLine();
			if (path.length() > 8) {
				xmlUrl.setPath(path.mid(8)); //chop off "/var/www"
				m_manager->get(xmlUrl);
				m_xmlCount++;
			}
		}
	} else if (reply == m_editReply){
		qDebug() << "Network Beta" << reply.errorString() << reply.readAll();
		//TODO: Error handling
	} else {
		qDebug() << "Network Gamma" << reply.errorString();
		QByteArray xml = reply.readAll();
		Tournament* t = new Tournament(this);
		t.loadFromXml(xml);
		m_ts << t;
		if(--m_xmlCount)
			generateModelOne();
	}
}

void ApplicationCore::generateModelOne()
{
	QStringList tps;
	tps << "name" << "description" << "subTitle" << "time" << "eventType";
	m_m1 = new GroupDataModel(tps);
	foreach(Tournament* t, m_ts)
		m_m1->insert(t);
	emit modelOneChanged();
	m_l1=true;
	emit loadedOneChanged();

	QStringList mps;
	mps << "index" << "winnerTo" << "loserTo" << "title" << "player1" << "player2" << "winner" << "done" << "schConf" << "schedule";
	m_m2 = new GroupDataModel(mps);
	loadEvent(m_ts[0]->name());
	m_l2 = true;
	emit loadedTwoChanged();
}

void ApplicationCore::loadEvent(const QString &name) {
	int i,j;
	for(i=0; i<m_ts.count(); i++)//deliberate fallback to last event if nothing is found.
		if (m_ts[i]->name() == name)
			break;

	m_currentTIdx = i;
	j=0;
	m_m2.clear();
	while(j<m_ts[i]->matchCount())
		m_m2->insert(m_ts[i]->matchAt[j++]);
	emit modelTwoChanged();
}

void ApplicationCore::confirmSchedule(int mId, bool doit) {
	serverUpdate(QString("matchIndex=%1&schConf=%2").arg(mId).arg(doIt));
}

void ApplicationCore::setWinner(int mId, int which) {
}

void ApplicationCore::setSchedule(QDateTime t) {
}

void ApplicationCore::registerPlayer(QString title, QString name,
		QString email, QString race) {
}

void ApplicationCore::changeServer(const QUrl& url) {
	m_server = url;
	fetchData();
}

void ApplicationCore::serverUpdate(const QString &s)
{
	QByteArray name = m_ts[m_currentTIdx]->name
	QByteArray cmd(QString("password=%1&request=edit").arg(SHARED_PASSWORD).toLatin1());

}
