#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <QObject>
#include <QString>
#include <QStringList>
#ifndef NO_DECLARATIVE
#include <QDeclarativeListProperty>
#endif
class Match;
class Tournament : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(QStringList players READ players WRITE setPlayers NOTIFY playersChanged);
#ifndef NO_DECLARATIVE
    Q_PROPERTY(QDeclarativeListProperty<Match> matches READ matches NOTIFY matchesChanged);
#endif
public:
    explicit Tournament(QObject *parent = 0);

    void loadFromXml(QByteArray xml);
    QByteArray writeToXml() const;

    QString name() const { return _name; }
    void setName(const QString &name){if(name==_name) return; _name=name; emit nameChanged();}
    QStringList players() const { return _players; }
    void setPlayers(const QStringList &players);
#ifndef NO_DECLARATIVE
    QDeclarativeListProperty<Match> matches() {return QDeclarativeListProperty<Match>(this, _matches);}
#endif
    Q_INVOKABLE Match* matchAt(int idx){if(idx >= 0 && idx < _matches.count()) return _matches[idx]; return 0;}

    void addEmptyMatch();

    void arrangeMatches();
signals:
    void nameChanged();
    void playersChanged();
    void matchesChanged();

public slots:
    void shufflePlayerOrder();
    void matchFinished(Match* match, bool playerOneWon);
    void createStandard();
private:
    QString _name;
    QStringList _players;
    QList<Match*> _matches;
};


#endif // TOURNAMENT_H
