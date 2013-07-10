#include "player.h"

Player::Player(QObject* parent)
    : QObject(parent)
    , m_pId(0)
    , m_name("Unnamed")
    , m_race("R")
    , m_email("noreply@pppdbrd.nope")
{}

Player::Player(int pId, QString name, QString race, QString email, QObject* parent)
    : QObject(parent)
    , m_pId (pId)
    , m_name (name)
    , m_race (race)
    , m_email (email)
{

}
