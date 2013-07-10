#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <QObject>
#include <QString>
#include <QStringList>
#ifndef NO_DECLARATIVE
#ifdef NEW_DECLARATIVE
#include <QQmlListProperty>
#define QMLLIST QQmlListProperty
#else
#define QMLLIST QDeclarativeListProperty
#include <QDeclarativeListProperty>
#endif
#endif
class Match;
class Player;
class Tournament : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(QString description READ desc WRITE setDesc NOTIFY descChanged);
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle NOTIFY subTitleChanged);
    Q_PROPERTY(QString time READ time WRITE setTime NOTIFY timeChanged);
    Q_PROPERTY(QString eventType READ type WRITE setType NOTIFY typeChanged);
#ifndef NO_DECLARATIVE
    Q_PROPERTY(QMLLIST<Player> players READ players NOTIFY playersChanged);
    Q_PROPERTY(QMLLIST<Match> matches READ matches NOTIFY matchesChanged);
#endif
public:
    explicit Tournament(QObject *parent = 0);

    void loadFromXml(QByteArray xml);
    QByteArray writeToXml() const;

    QString name() const { return _name; }
    void setName(const QString &name){if(name==_name) return; _name=name; emit nameChanged();}
#ifndef NO_DECLARATIVE
    QMLLIST<Match> matches() {return QMLLIST<Match>(this, _matches);}
    QMLLIST<Player> players() {return QMLLIST<Player>(this, _players);}
#endif
    Q_INVOKABLE Match* matchAt(int idx){if(idx >= 0 && idx < _matches.count()) return _matches[idx]; return 0;}
    Q_INVOKABLE Player* playerAt(int idx){if(idx >= 0 && idx < _players.count()) return _players[idx]; return 0;}
    Q_INVOKABLE int matchCount() { return _matches.count(); }
    Q_INVOKABLE int playerCount() { return _players.count(); }

    void addEmptyMatch();
    void addEmptyPlayer();

    void arrangeMatches();
    QString desc() const
    {
        return m_description;
    }

    QString subTitle() const
    {
        return m_subTitle;
    }

    QString time() const
    {
        return m_time;
    }

    QString type() const
    {
        return m_eventType;
    }

signals:
    void nameChanged();
    void playersChanged();
    void matchesChanged();

    void descChanged(QString arg);

    void subTitleChanged(QString arg);

    void timeChanged(QString arg);

    void typeChanged(QString arg);

public slots:
    void shufflePlayerOrder();
    void matchFinished(Match* match, bool playerOneWon);
    void createSingleElim();
    void createDoubleElim();
    void createRoundRobin();
    void setDesc(QString arg)
    {
        if (m_description != arg) {
            m_description = arg;
            emit descChanged(arg);
        }
    }

    void setSubTitle(QString arg)
    {
        if (m_subTitle != arg) {
            m_subTitle = arg;
            emit subTitleChanged(arg);
        }
    }

    void setTime(QString arg)
    {
        if (m_time != arg) {
            m_time = arg;
            emit timeChanged(arg);
        }
    }

    void setType(QString arg)
    {
        if (m_eventType != arg) {
            m_eventType = arg;
            emit typeChanged(arg);
        }
    }

private:
    QString _name;
    QList<Player*> _players;
    QList<Match*> _matches;
    QString m_description;
    QString m_subTitle;
    QString m_time;
    QString m_eventType;
};


#endif // TOURNAMENT_H
