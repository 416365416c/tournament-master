/*
 * ApplicationCore.h
 *
 *  Created on: Jul 5, 2013
 *      Author: Alan
 */

#ifndef APPLICATIONCORE_H_
#define APPLICATIONCORE_H_

#include <qobject.h>
#include <bb/cascades/GroupDataModel>
#include "../../../Common/tournament.h"
#include "../../../Common/player.h"
#include "../../../Common/match.h"

class QNetworkAccessManager;
class QNetworkAccessManager;
class ApplicationCore: public QObject {
	Q_OBJECT
	Q_PROPERTY(bool loaded READ lone NOTIFY loadedOneChanged FINAL)
	Q_PROPERTY(bool loadedTwo READ ltwo NOTIFY loadedTwoChanged FINAL)
	Q_PROPERTY(bb::cascades::GroupDataModel* tournamentsModel READ mone NOTIFY modelOneChanged FINAL)
	Q_PROPERTY(bb::cascades::GroupDataModel* matchesModel READ mtwo NOTIFY modelTwoChanged FINAL)

public:
	ApplicationCore(QObject* parent=0); //triggers async fetching of model #1
	virtual ~ApplicationCore();

	bool lone() const { return m_l1; }
	bool ltwo() const { return m_l2; }
	bb::cascades::GroupDataModel* mone() { return m_m1; }
	bb::cascades::GroupDataModel* mtwo() { return m_m2; }
public Q_SLOTS:
	void loadEvent(const QString &name); //triggers generating of model #2
	void confirmSchedule(int mId, bool doit = true);
	void setWinner(int mId, int which);
	void setSchedule(QDateTime t);
	void registerPlayer(QString title, QString name, QString email, QString race);
	void changeServer(const QUrl &url);
private Q_SLOTS:
	void handleNetworkReply(QNetworkReply*);
	void generateModelOne();
	void serverUpdate(const QString &cmds);
Q_SIGNALS:
	void loadedOneChanged();
	void loadedTwoChanged();
	void modelOneChanged();
	void modelTwoChanged();
private:
	void fetchData();
	QNetworkAccessManager* m_manager;
	QUrl m_server;
	bool m_l1;
	bool m_l2;
	QList<Tournament *> m_ts;
	bb::cascades::GroupDataModel* m_m1;
	bb::cascades::GroupDataModel* m_m2;
	QNetworkReply *m_listReply;
	QNetworkReply *m_editReply;
	int m_xmlCount;
	int m_currentTIdx;
};

#endif /* APPLICATIONCORE_H_ */
