#include "tournament.h"
#include "match.h"
#include <cmath>
#include <QTime>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

Tournament::Tournament(QObject *parent) :
    QObject(parent)
{
}

void Tournament::addEmptyMatch(){
    Match* m = new Match(this);
    m->setIndex(_matches.count());
    m->setWinnerTo(0);
    m->setTitle("Unnamed Match");
    m->setXPos(32);
    m->setYPos(32);
    _matches << m;
    emit matchesChanged();
}

void Tournament::createStandard(){
    foreach(Match* m, _matches)
        delete m;
    _matches.clear();
    //Setup the titles, the first tier matches, and the 'byes'
    int tiers = ceil((double)log((double)_players.count())/log(2.0));//Couldn't find log2
    tiers = qMax(tiers, 1);
    int c=0;
    for(int tier=0; tier<tiers; tier++){//Set titles
        int acc=0;
        while(acc < 1<<tier){
            Match* match = new Match(this);
            match->setIndex(c);
            match->setWinnerTo((c-1)/2);
            c++;
            switch(tier){
            case 0:
                match->title = QString("Finals"); break;
            case 1:
                match->title = QString("Semi-Final %1").arg(acc + 1);  break;
            case 2:
                match->title = QString("Quarter Final %1").arg(acc + 1);  break;
            default:
                match->title = QString("Round %1 Match %2").arg(tiers-tier).arg(acc + 1);
            }
            _matches << match;
            ++acc;
        }
    }
    int acc = 0;
    for(int idx = 1 << (tiers - 1);  idx < 1 << tiers; idx++){
        Match* match = _matches[idx-1];
        match->p1 = acc < _players.count() ? _players[acc++] : QString();
        match->p2 = acc < _players.count() ? _players[acc++] : QString();
        if(match->p2 == QString())
            matchFinished(match, true);
    }

    emit matchesChanged();
}

void Tournament::arrangeMatches()
{
    //TODO: Topsort and lay out matches tree-like in a vaguely intelligent fashion
    foreach(Match* m, _matches){
        if(m->wTo == -1){
            m->setXPos(512);
            m->setYPos(32);
        }else{
            m->setXPos(_matches[m->wTo]->xPos());
            m->setYPos(_matches[m->wTo]->yPos() + 128);
        }
    }
}

void Tournament::setPlayers(const QStringList &players)
{
    _players = players;
    emit playersChanged();
}

void Tournament::shufflePlayerOrder()
{
    QStringList shuffled;
    qsrand(QTime::currentTime().msecsTo(QTime()));
    while(_players.count()){
        int r = qrand() % _players.count();
        shuffled << _players[r];
        _players.removeAt(r);
    }
    _players = shuffled;
    emit playersChanged();
}

void Tournament::matchFinished(Match* match, bool playerOneWon)
{
    if(!match)
        return;
    match->done = true;
    match->oneWon = playerOneWon;
    match->matchDone();
    QString winner = match->winner();
    QString loser = playerOneWon?match->p2:match->p1;

    int idx = match->wTo;
    if(idx >= 0){
        if(_matches[idx]->p1 != winner && _matches[idx]->p2 != winner){
            if(_matches[idx]->p1.isEmpty())
                _matches[idx]->setP1(winner);
            else
                _matches[idx]->setP2(winner);
        }
    }
    idx = match->lTo;
    if(idx >= 0){
        if(_matches[idx]->p1 != loser && _matches[idx]->p2 != loser){
            if(_matches[idx]->p1.isEmpty())
                _matches[idx]->setP1(loser);
            else
                _matches[idx]->setP2(loser);
        }
    }
}

void Tournament::loadFromXml(QByteArray xml)
{
    QXmlStreamReader reader(xml);
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType type = reader.readNext();
        if(type != QXmlStreamReader::StartElement)
            continue;
        if(reader.name() == "Tournament"){
            _name = reader.attributes().value("name").toString();
        }else if(reader.name() == "Player"){
            _players << reader.attributes().value("name").toString();
        }else if(reader.name() == "Match"){
            Match* m = new Match;
            m->title = reader.attributes().value("title").toString();
            m->p1 = reader.attributes().value("player1").toString();
            m->p2 = reader.attributes().value("player2").toString();
            QString winner  = reader.attributes().value("winner").toString();
            m->wTo = reader.attributes().value("winnerTo").toString().toInt();
            m->lTo = reader.attributes().value("loserTo").toString().toInt();
            m->xP = reader.attributes().value("xPos").toString().toInt();
            m->yP = reader.attributes().value("yPos").toString().toInt();
            m->p1approves = (reader.attributes().value("p1approves").toString() == QLatin1String("true"));
            m->p2approves = (reader.attributes().value("p2approves").toString() == QLatin1String("true"));
            m->schedule = reader.attributes().value("schedule").toString();
            if(winner == QString()){
                m->done = false;
            }else if(winner == m->p1){
                m->done = true;
                m->oneWon = true;
            }else{
                m->done = true;
                m->oneWon = false;
            }
            _matches << m;
        }
    }
}

QByteArray Tournament::writeToXml() const
{
    QByteArray ret;
    QXmlStreamWriter writer(&ret);
    writer.writeStartDocument();
    writer.writeStartElement("Tournament");
    writer.writeAttribute("name", _name);
    foreach(const QString &str, _players){
        writer.writeStartElement("Player");
        writer.writeAttribute("name", str);
        writer.writeEndElement();
    }
    foreach(Match* m, _matches){
        writer.writeStartElement("Match");
        writer.writeAttribute("title", m->title);
        writer.writeAttribute("index", QString::number(m->index));
        writer.writeAttribute("player1", m->p1);
        writer.writeAttribute("player2", m->p2);
        writer.writeAttribute("winner", m->winner());
        writer.writeAttribute("winnerTo", QString::number(m->wTo));
        writer.writeAttribute("loserTo", QString::number(m->lTo));
        writer.writeAttribute("xPos", QString::number(m->xP));
        writer.writeAttribute("yPos", QString::number(m->yP));
        writer.writeAttribute("p1approves", m->p1approves ? QLatin1String("true") : QLatin1String("false"));
        writer.writeAttribute("p2approves", m->p2approves ? QLatin1String("true") : QLatin1String("false"));
        writer.writeAttribute("schedule", m->schedule);
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    return ret;
}
