#include "player.h"

Player::Player()
{
}

Position &Player::position()
{
    return m_position;
}

Physics &Player::physics()
{
    return m_physics;
}
