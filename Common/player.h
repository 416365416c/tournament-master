#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include <QStringList>

class Player : public QObject
{
    Q_OBJECT
    //Don't actually know how much we need on the players?
    Q_PROPERTY(int pId READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString race READ getRace WRITE setRace NOTIFY raceChanged)
    Q_PROPERTY(QString email READ getEmail WRITE setEmail NOTIFY emailChanged)
public:
    Player(QObject* parent = 0);
    Player(int pId, QString name, QString race, QString email, QObject* parent = 0);

    int getId() const
    {
        return m_pId;
    }

    QString getName() const
    {
        return m_name;
    }

    QString getRace() const
    {
        return m_race;
    }

    QString getEmail() const
    {
        return m_email;
    }

public slots:
    void setId(int arg)
    {
        if (m_pId != arg) {
            m_pId = arg;
            emit idChanged(arg);
        }
    }

    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }

    void setRace(QString arg)
    {
        if (m_race != arg) {
            m_race = arg;
            emit raceChanged(arg);
        }
    }

    void setEmail(QString arg)
    {
        if (m_email != arg) {
            m_email = arg;
            emit emailChanged(arg);
        }
    }

signals:
    void idChanged(int arg);

    void nameChanged(QString arg);

    void raceChanged(QString arg);

    void emailChanged(QString arg);

private:
    friend class Tournament;
    int m_pId;
    QString m_name;
    QString m_race;
    QString m_email;
};
#endif
