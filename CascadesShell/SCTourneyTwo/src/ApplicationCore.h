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

class ApplicationCore: public QObject {
	Q_OBJECT
	Q_PROPERTY(bool loadedOne READ lone NOTIFY loadedOneChanged FINAL)
	Q_PROPERTY(bool loadedTwo READ ltwo NOTIFY loadedTwoChanged FINAL)
	Q_PROPERTY(bb::cascades::GroupDataModel* modelOne READ mone NOTIFY modelOneChanged FINAL)
	Q_PROPERTY(bb::cascades::GroupDataModel* modelTwo READ mtwo NOTIFY modelTwoChanged FINAL)

public:
	ApplicationCore(QObject* parent=0); //triggers async fetching of model #1
	virtual ~ApplicationCore();

	bool lone() const { return m_l1; }
	bool ltwo() const { return m_l2; }
	bb::cascades::GroupDataModel* mone() { return m_m1; }
	bb::cascades::GroupDataModel* mtwo() { return m_m2; }
public Q_SLOTS:
	void loadEvent(const QString &name); //triggers fetching of model #2
Q_SIGNALS:
	void loadedOneChanged();
	void loadedTwoChanged();
	void modelOneChanged();
	void modelTwoChanged();
private:
	bool m_l1;
	bool m_l2;
	bb::cascades::GroupDataModel* m_m1;
	bb::cascades::GroupDataModel* m_m2;
};

#endif /* APPLICATIONCORE_H_ */
