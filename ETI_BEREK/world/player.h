#ifndef PLAYER_H
#define PLAYER_H

#include "physics/physicsutils.h"

class Player
{
public:
    Player();

    Position& position();
    Physics& physics();

protected:
    Position m_position;
    Physics m_physics;
};

#endif // PLAYER_H
