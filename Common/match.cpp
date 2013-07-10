#include "match.h"

Match::Match(QObject *parent) :
    QObject(parent), title("Untitled"), p1(0), p2(0), p1approves(false), p2approves(false), schConf(false), oneWon(false), done(false), index(-1), wTo(-1), lTo(-1), xP(0), yP(0)
{
}
