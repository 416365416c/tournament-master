#include "match.h"

Match::Match(QObject *parent) :
    QObject(parent), title("Untitled"), p1approves(false), p2approves(false), done(false), index(-1), wTo(-1), lTo(-1), xP(0), yP(0)
{
}
