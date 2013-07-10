#ifndef MATCH_H
#define MATCH_H

#include <QObject>
#include <QString>

class Player;
class Match : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ getIndex CONSTANT);
    Q_PROPERTY(int winnerTo READ winnerTo WRITE setWinnerTo NOTIFY winnerToChanged);
    Q_PROPERTY(int loserTo READ loserTo WRITE setLoserTo NOTIFY loserToChanged);
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged);
    Q_PROPERTY(Player* player1 READ player1 WRITE setP1 NOTIFY playersChanged);
    Q_PROPERTY(Player* player2 READ player2 WRITE setP2 NOTIFY playersChanged);
    Q_PROPERTY(Player* winner READ winner NOTIFY matchDone);
    Q_PROPERTY(bool done READ isDone NOTIFY matchDone);
    Q_PROPERTY(int xPos READ xPos WRITE setXPos NOTIFY xPosChanged);//??
    Q_PROPERTY(int yPos READ yPos WRITE setYPos NOTIFY yPosChanged);//??

    Q_PROPERTY(bool p1approves READ p1a WRITE setp1a NOTIFY schedChanged);
    Q_PROPERTY(bool p2approves READ p1a WRITE setp1a NOTIFY schedChanged);
    Q_PROPERTY(bool schConf READ getSchConf WRITE setSchConf NOTIFY schedChanged);
    Q_PROPERTY(QString schedule READ getSched WRITE setSched NOTIFY schedChanged);

public:
    explicit Match(QObject *parent = 0);
    QString getTitle() { return title; }
    Player* player1() { return p1; }
    Player* player2() { return p2; }
    Player* winner() { if(!done) return 0; if(oneWon) return p1; return p2; }
    bool isDone() {return done;}
    void setP1(Player* t){if(t==p1) return; p1=t; emit playersChanged();}
    void setP2(Player* t){if(t==p2) return; p2=t; emit playersChanged();}
    void setTitle(const QString& t){if(t==title) return; title=t; emit titleChanged();}
    QString getSched() { return schedule; }
    void setSched(const QString& t){if(t==schedule) return; schedule=t; emit schedChanged();}

    bool p1a() {return p1approves;}
    void setp1a(bool n){if(n==p1approves) return; p1approves=n; emit schedChanged();}
    bool p2a() {return p2approves;}
    void setp2a(bool n){if(n==p2approves) return; p2approves=n; emit schedChanged();}
    bool getSchConf() {return schConf;}
    void setSchConf(bool n){if(n==schConf) return; schConf=n; emit schedChanged();}

    int getIndex() const {return index;}
    void setIndex(int i) {index = i;}//Not strictly enforcing constancy from C++

    int winnerTo() const {return wTo;}
    void setWinnerTo(int i){if(i==wTo) return; wTo = i; emit winnerToChanged();}
    int loserTo() const {return lTo;}
    void setLoserTo(int i){if(i==lTo) return; lTo = i; emit loserToChanged();}

    int xPos() const {return xP;}
    void setXPos(int i){ if(i==xP) return; xP = i; emit xPosChanged();}
    int yPos() const {return yP;}
    void setYPos(int i){ if(i==yP) return; yP = i; emit yPosChanged();}

signals:
    void matchDone();
    void playersChanged();
    void titleChanged();
    void schedChanged();
    void winnerToChanged();
    void loserToChanged();
    void xPosChanged();
    void yPosChanged();

public slots:
private:
    friend class Tournament;
    QString title, schedule;
    Player* p1;
    Player* p2;
    bool p1approves, p2approves, schConf; //??
    bool oneWon;
    bool done;
    int index;
    int wTo, lTo;
    int xP, yP;
};

#endif // MATCH_H
