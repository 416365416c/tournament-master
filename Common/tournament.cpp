#include "tournament.h"
#include "match.h"
#include "player.h"
#include <math.h>
#include <stdlib.h>
#include <QTime>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

Tournament::Tournament(QObject *parent) :
    QObject(parent)
{
    m_eventType = QLatin1String("hidden");//hidden,sc2,ahgl,lol,arcade
    m_status = QLatin1String("signup");//signup,active,done
    addEmptyPlayer();
    _players[0]->setEmail("none@none.none");
    _players[0]->setName("Absent");
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

void Tournament::addEmptyPlayer()
{
    Player* p = new Player(this);
    p->setId(_players.count());//other defaults are sensible
    _players << p;
    emit playersChanged();
}

void Tournament::createSingleElim(){
    QList<Match*> oldMs = _matches;
    _matches.clear();
    //Setup the titles, the first tier matches, and the 'byes' (players offset by 1 to treat p[0] special)
    int tiers = ceil((double)log((double)_players.count()-1.0)/log(2.0));//Couldn't find log2
    tiers = qMax(tiers, 1);
    int c=1;//this tree formula is rooted at 1
    for(int tier=0; tier<tiers; tier++){//Set titles
        int acc=0;
        while(acc < 1<<tier){
            Match* match = new Match(this);
            match->setIndex(c-1);
            match->setWinnerTo((c-2)/2);

            switch(tier){
            case 0:
                match->title = QString("Finals"); break;
            case 1:
                match->title = QString("Semi-Final %1").arg(acc + 1);  break;
            //case 2:
            //    match->title = QString("Quarter Final %1").arg(acc + 1);  break;
            default:
                match->title = QString("Round of %1 Match %2").arg(2 << tier).arg(acc + 1);
            }
            _matches << match;
            ++acc;
            c++;
        }
    }
    int acc = 1;
    for(int idx = 1 << (tiers - 1);  idx < 1 << tiers; idx++){
        Q_ASSERT(idx <= _matches.count());
        Match* match = _matches[idx-1];
        if ( acc >= _players.count() ) { //truncate
            int acc2;
            for (acc2 = 0; (acc2 + idx - 1) < _matches.count(); acc2++)
                delete _matches[acc2 + idx - 1];

            while (acc2--)
                _matches.removeLast();

            break;
        }
        match->p1 = acc < _players.count() ? _players[acc++] : 0;
        match->p2 = acc < _players.count() ? _players[acc++] : 0;
        if(match->p2 == 0)
            matchFinished(match, true);
    }

    emit matchesChanged();
    qDeleteAll(oldMs); //otherwise the references update and cause a crash...
}

void Tournament::createDoubleElim(){
    QList<Match*> oldMs = _matches;
    _matches.clear();
    //Setup the titles, the first tier matches, and the 'byes'(players offset by 1 to treat p[0] special)
    int tiers = ceil((double)log((double)_players.count()-1.0)/log(2.0));//Couldn't find log2
    tiers = qMax(tiers, 1);
    int c=1;//this tree formula is rooted at 1
    //Upper Bracket First
    for(int tier=0; tier<tiers; tier++){//Set titles
        int acc=0;
        while(acc < 1<<tier){
            Match* match = new Match(this);
            match->setIndex(c-1);
            match->setWinnerTo((c-2)/2);
            switch(tier){
            case 0:
                match->title = QString("Upper Bracket Finals"); break;
            //case 1:
            //    match->title = QString("Semi-Final %1").arg(acc + 1);  break;
            //case 2:
            //    match->title = QString("Quarter Final %1").arg(acc + 1);  break;
            default:
                match->title = QString("Upper Bracket Round of %1 Match %2").arg(2 << tier).arg(acc + 1);
            }
            _matches << match;
            ++acc;
            c++;
        }
    }
    int acc = 1;
    for(int idx = 1 << (tiers - 1);  idx < 1 << tiers; idx++){
        Match* match = _matches[idx-1];
        if ( acc >= _players.count() ) { //truncate
            int acc2;
            for (acc2 = 0; (acc2 + idx - 1) < _matches.count(); acc2++)
                delete _matches[acc2 + idx - 1];

            while (acc2--)
                _matches.removeLast();
            break;
        }
        match->p1 = acc < _players.count() ? _players[acc++] : _players[0];
        match->p2 = acc < _players.count() ? _players[acc++] : _players[0];
        if(match->p2 == 0)
            matchFinished(match, true);
    }

    c = _matches.count(); //We can drop the fake offset now, and maybe any lost matches
    Match* gmatch = new Match(this);
    gmatch->setIndex(c);
    gmatch->setTitle("Grand Finals");
    _matches << gmatch;
    _matches[0]->setWinnerTo(c);
    c++;

    //Lower Bracket
    int c0 = 0;//idx for new 'tree' (actually 0 based!)
    int c2 = c;//idx new tree is offset from c
    int c3 = 0;//tracking progress on the old 'tree'
    //For each tier we have a losers match, the winner of which plays the new loser
    int c1=0;
    for(int tier=0; tier<(tiers-1); tier++){//Set titles
        int acc=0;
        c1 = c0;//idx for start of this tier
        while(acc < 1<<tier){
            //new losers match
            Match* match = new Match(this);
            match->setIndex(c);
            if (!c0)
                match->setWinnerTo(c-1);
            else {
                int target = c0/2;
                if (!(target % 2))
                    target--;
                match->setWinnerTo(c2 + target);
            }
            _matches[c3++]->setLoserTo(c);
            switch(tier){
            case 0:
                match->title = QString("Lower Bracket Finals"); break;
            //case 1:
            //    match->title = QString("Semi-Final %1").arg(acc + 1);  break;
            //case 2:
            //    match->title = QString("Quarter Final %1").arg(acc + 1);  break;
            default:
                match->title = QString("Lower Bracket Match %2").arg(c0);
            }
            _matches << match;
            c++;
            c0++;
            //old losers match
            match = new Match(this);
            match->setIndex(c);
            match->setWinnerTo(c-1);
            match->title = QString("Lower Bracket Match %2").arg(c0);
            _matches << match;
            ++acc;
            c++;
            c0++;
        }
    }
    //Redirect last tier upper bracket losers
    c1 += c2 + 1;
    while (_matches[c3]->p2 != _players[0]) {
        _matches[c3++]->setLoserTo(c1);
        if (_matches[c3]->p2 != _players[0]) {
            _matches[c3++]->setLoserTo(c1);
        } else {
            _matches[c1]->p2 = _players[0];
        }
        c1 += 2;
    }

    while (c1 <= c) {
        _matches[c1]->done = true;
        c1 += 2;
    }

    qDebug() << writeToXml();
    emit matchesChanged();
    qDeleteAll(oldMs); //otherwise the references update and cause a crash...
}

void Tournament::createRoundRobin() {
    //NOTE: No scoring yet... do that externally
    QList<Match*> oldMs = _matches;
    _matches.clear();
    //Setup the titles and the matches
    int c = 0;
    for(int p1=1; p1<_players.count(); p1++) {
        for (int p2 = p1 + 1; p2 < _players.count(); p2++) {
            Match* match = new Match(this);
            match->setIndex(c);
            match->title = QString("Match %1").arg(c);
            match->p1 = _players[p1];
            match->p2 = _players[p2];
            _matches << match;
            c++;
        }
    }
    emit matchesChanged();
    qDeleteAll(oldMs); //otherwise the references update and cause a crash...
}

void Tournament::arrangeMatches()//just geom, and just an ELIM helper
{
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

void Tournament::shufflePlayerOrder()
{
    //### Resets ids
    QList<Player*> shuffled;
    qsrand(QTime::currentTime().msecsTo(QTime()));
    while(_players.count()){
        int r = qrand() % _players.count();
        shuffled << _players[r];
        _players.removeAt(r);
    }
    _players = shuffled;
    for(int i=0; i<_players.count(); i++)
        _players[i]->setId(i);
    emit playersChanged();
}

void Tournament::matchFinished(Match* match, bool playerOneWon)
{
    if(!match)
        return;
    match->done = true;
    match->oneWon = playerOneWon;
    match->matchDone();
    Player* winner = match->winner();
    Player* loser = playerOneWon?match->p2:match->p1;

    int idx = match->wTo;
    if(idx >= 0){
        if(_matches[idx]->p1 != winner && _matches[idx]->p2 != winner){
            if(!_matches[idx]->p1)
                _matches[idx]->setP1(winner);
            else
                _matches[idx]->setP2(winner);
        }
    }
    idx = match->lTo;
    if(idx >= 0){
        if(_matches[idx]->p1 != loser && _matches[idx]->p2 != loser){
            if(!_matches[idx]->p1)
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
            m_description = reader.attributes().value("desc").toString();
            m_time = reader.attributes().value("time").toString();
            m_subTitle = reader.attributes().value("subT").toString();
            m_eventType = reader.attributes().value("type").toString();
            m_status = reader.attributes().value("stat").toString();
        }else if(reader.name() == "Player"){
            Player* p = new Player(this);
            p->m_name = reader.attributes().value("name").toString();
            p->m_pId = reader.attributes().value("pId").toString().toInt();
            p->m_race = reader.attributes().value("race").toString();
            p->m_email = reader.attributes().value("email").toString();
            if (p->getId() == 0) //special sentinel user can't be written
                delete p;
            else {
                if (p->m_pId != _players.count())
                    qCritical() << "Error: Player index mismatch at" << p->m_pId;
                _players << p;
            }
        }else if(reader.name() == "Match"){
            Match* m = new Match(this);
            m->title = reader.attributes().value("title").toString();
            m->setProperty("tempP1", reader.attributes().value("player1").toString());
            m->setProperty("tempP2", reader.attributes().value("player2").toString());
            m->setProperty("tempWin", reader.attributes().value("winner").toString());
            m->wTo = reader.attributes().value("winnerTo").toString().toInt();
            m->lTo = reader.attributes().value("loserTo").toString().toInt();
            m->xP = reader.attributes().value("xPos").toString().toInt();
            m->yP = reader.attributes().value("yPos").toString().toInt();
            m->p1approves = (reader.attributes().value("p1approves").toString() == QLatin1String("true"));
            m->p2approves = (reader.attributes().value("p2approves").toString() == QLatin1String("true"));
            m->schConf = (reader.attributes().value("confirmed").toString() == QLatin1String("true"));
            m->schedule = reader.attributes().value("schedule").toString();
            m->index = reader.attributes().value("index").toString().toInt();
            if (m->index != _matches.count())
                qCritical() << "Error: Match index mismatch at" << m->index;
            _matches << m;
        }
    }

    //Fixup data in p1/p2, all players should be loaded
    foreach (Match* m, _matches) {
        int p1 = m->property("tempP1").toInt();
        int p2 = m->property("tempP2").toInt();
        int winner = m->property("tempWin").toInt();
        //### we're kindof screwed if ids don't match loaded order
        if (p1 != -1 && p1 < _players.count())
            m->p1 = _players[p1];
        else
            m->p1 = 0;
        if (p2 != -1 && p2 < _players.count())
            m->p2 = _players[p2];
        else
            m->p2 = 0;

        if(winner == -1){
            m->done = false;
        }else if(winner == p1){
            m->done = true;
            m->oneWon = true;
        }else{
            m->done = true;
            m->oneWon = false;
        }
    }
}

QByteArray Tournament::writeToXml() const
{
    QByteArray ret;
    QXmlStreamWriter writer(&ret);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("Tournament");
    writer.writeAttribute("name", _name);
    writer.writeAttribute("desc", m_description);
    writer.writeAttribute("time", m_time);
    writer.writeAttribute("subT", m_subTitle);
    writer.writeAttribute("type", m_eventType);
    writer.writeAttribute("stat", m_status);
    foreach(Player* p, _players){
        if (p->getId() == 0)
            continue;
        writer.writeStartElement("Player");
        writer.writeAttribute("pId", QString::number(p->getId()));
        writer.writeAttribute("name", p->getName());
        writer.writeAttribute("email", p->getEmail());
        writer.writeAttribute("race", p->getRace());
        writer.writeEndElement();
    }
    foreach(Match* m, _matches){
        writer.writeStartElement("Match");
        writer.writeAttribute("title", m->title);
        writer.writeAttribute("index", QString::number(m->index));
        writer.writeAttribute("player1", !m->p1 ? "-1" : QString::number(m->p1->getId()));
        writer.writeAttribute("player2", !m->p2 ? "-1" : QString::number(m->p2->getId()));
        writer.writeAttribute("winner", !m->winner() ? QString("-1") : QString::number(m->winner()->getId()));
        writer.writeAttribute("winnerTo", QString::number(m->wTo));
        writer.writeAttribute("loserTo", QString::number(m->lTo));
        writer.writeAttribute("xPos", QString::number(m->xP));
        writer.writeAttribute("yPos", QString::number(m->yP));
        writer.writeAttribute("p1approves", m->p1approves ? QLatin1String("true") : QLatin1String("false"));
        writer.writeAttribute("p2approves", m->p2approves ? QLatin1String("true") : QLatin1String("false"));
        writer.writeAttribute("confirmed", m->schConf ? QLatin1String("true") : QLatin1String("false"));
        writer.writeAttribute("schedule", m->schedule);
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    return ret;
}
