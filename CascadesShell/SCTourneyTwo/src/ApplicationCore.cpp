/*
 * ApplicationCore.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: Alan
 */

#include "ApplicationCore.h"
#include "../../../Common/password.h"
#include <QtCore>
#include <QtNetwork>
#include <bb/cascades/GroupDataModel>
using namespace bb::cascades;
ApplicationCore::ApplicationCore(QObject* parent) : QObject(parent)
	, m_manager(0)//TODO: https!
	, m_server("http://ec2-184-169-240-202.us-west-1.compute.amazonaws.com/cgi-bin/tournamentCgi")
	, m_l1(false)
	, m_l2(false)
	, m_m1(0)
	, m_m2(0)
	, m_xmlCount(-1)
{
	qDebug() << "Hello world";
	m_manager = new QNetworkAccessManager(this);
	connect(m_manager, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(handleNetworkReply(QNetworkReply*)));
	fetchData();
}

ApplicationCore::~ApplicationCore() {
}

void ApplicationCore::fetchData() {
	QByteArray cmd(QString("password=%1&request=list").arg(SHARED_PASSWORD).toLatin1());
	m_listReply = m_manager->post(QNetworkRequest(m_server), cmd);
}

void ApplicationCore::handleNetworkReply(QNetworkReply* reply)
{
	if(reply == m_listReply) {
		m_xmlCount = 0;
		qDebug() << "Network Alpha" << reply->errorString();
		while (reply->canReadLine()) {
			QUrl xmlUrl = m_server;
			QByteArray path = QByteArray::fromPercentEncoding(reply->readLine()).trimmed();

			if (path.contains("FAIL")) {
				qDebug() << "Network Epsilon" << path;
				return;
			}
			if (path.contains("xml") && path.length() > 8) {
				xmlUrl.setPath(path.mid(8)); //chop off "/var/www"
				m_manager->get(QNetworkRequest(xmlUrl));
				m_xmlCount++;
			}
		}
	} else if (reply == m_editReply){
		qDebug() << "Network Beta" << reply->errorString() << reply->readAll();
		//TODO: Error handling
	} else {
		qDebug() << "Network Gamma" << reply->errorString();
		QByteArray xml = reply->readAll();
		Tournament* t = new Tournament(this);
		t->loadFromXml(xml);
		m_ts << t;
		if(--m_xmlCount)
			generateModelOne();
	}
}

void ApplicationCore::generateModelOne()
{
	QStringList tps;
    //tps << "name" << "description" << "subTitle" << "time" << "eventType" << "status";
    tps << "status" << "eventType" << "name";
	m_m1 = new GroupDataModel(tps);
    m_m1->setGrouping(ItemGrouping::ByFullValue);
	foreach(Tournament* t, m_ts)
        if (t->type() != "hidden")
            m_m1->insert(t);
	emit modelOneChanged();
	m_l1=true;
	emit loadedOneChanged();

    //Note that I don't think refs will work, so we're converting to variant map here
	QStringList mps;
	//mps << "index" << "p1race" << "p2race" << "title" << "player1" << "player2" << "winner" << "schedule" << "active";
	mps << "index";
	m_m2 = new GroupDataModel(mps);
    m_m2->setGrouping(ItemGrouping::None);
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
	m_m2->clear();
	while(j<m_ts[i]->matchCount()) {
        QVariantMap map;
		Match* m = m_ts[i]->matchAt(j++);
        map["mId"] = m->getIndex();
        map["title"] = m->getTitle();
        map["winner"] = m->winner() ? m->winner()->getName() : QString();
        map["schedule"] = m->getSched();

        map["player1"] = m->player1() ? m->player1()->getName() : QLatin1String("???");
        map["player2"] = m->player2() ? m->player2()->getName() : QLatin1String("???");
        map["p1race"] = m->player1() ? m->player1()->getRace() : QLatin1String("R");
        map["p2race"] = m->player2() ? m->player2()->getRace() : QLatin1String("R");

        map["active"] = m_ts[i]->status() == QString("active");

        m_m2->insert(map);
    }
	emit modelTwoChanged();
}

void ApplicationCore::confirmSchedule(int mId, bool doit) {
	serverUpdate(QString("matchIndex=%1&schConf=%2").arg(mId).arg(doit));
}

void ApplicationCore::setWinner(int mId, int which) {
    Match *m = m_ts[m_currentTIdx]->matchAt(mId);
    QString winnerName;
    if (which == 1)
        winnerName = m->player1()->getName();
    else
        winnerName = m->player2()->getName();
	serverUpdate(QString("matchIndex=%1&winner=%2").arg(mId).arg(winnerName));
}

void ApplicationCore::setSchedule(int mId, QDateTime t) {
	serverUpdate(QString("matchIndex=%1&schedule=%2").arg(mId).arg(t.toString(Qt::ISODate)));
}

void ApplicationCore::registerPlayer(QString title, QString name,
		QString email, QString race) {
    Q_UNUSED(title); //we loaded this event already...
	serverUpdate(QString("playerIndex=-1&name=%1&email=%2&race=%3")
        .arg(name).arg(email).arg(race));
}

void ApplicationCore::changeServer(const QUrl& url) {
	m_server = url;
	fetchData();
}

void ApplicationCore::serverUpdate(const QString &s)
{
    //TODO: sort out tournamentTitle, name and path, which currently have a messed up relationship
	QByteArray name = "testTournament";
	QByteArray cmd(QString("password=%1&request=edit&tournamentTitle=%2&%3")
        .arg(SHARED_PASSWORD)
        .arg(QString::fromLatin1(name))
        .arg(s).toLatin1());
	m_editReply = m_manager->post(QNetworkRequest(m_server), cmd);
}
