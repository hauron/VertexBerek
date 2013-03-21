#include "physicsutils.h"

Position::Position()
{
    x = y = z = 0.0f;
    lx = ly = lz = 0.0f;
    angleHorizontal = angleVertical = 0.0f;
}

Physics::Physics()
{
    movement = 0;
}

void PhysicsUtils::move(Position &position, Physics &movement, float timePassed)
{
    unsigned _movement = movement.movement;

    // take into account the actual time passed - consider 25ms as a temporary solution ONLY, TODO

    if(_movement & PHYSICS_MOVE_FORWARD)
    {
        position.x += position.lx * fraction;
        position.z += position.lz * fraction;
    }
    if(_movement & PHYSICS_MOVE_BACKWARD)
    {
        position.x -= position.lx * fraction;
        position.z -= position.lz * fraction;
    }
    if(_movement & PHYSICS_MOVE_LEFT)
    {
        position.x += position.lz * fraction;
        position.z -= position.lx * fraction;
    }
    if(_movement & PHYSICS_MOVE_RIGHT)
    {
        position.x -= position.lz * fraction;
        position.z += position.lx * fraction;
    }
    if(_movement & PHYSICS_MOVE_UP)
    {
        position.y += flightSpeed;
    }
    if(_movement & PHYSICS_MOVE_DOWN)
    {
        position.y -= flightSpeed;
    }
    if(_movement & PHYSICS_TURN_LEFT)
    {
        position.angleHorizontal -= 0.02f;
        position.lx = sin(position.angleHorizontal);
        position.lz = -cos(position.angleHorizontal);
    }
    if(_movement & PHYSICS_TURN_RIGHT)
    {
        position.angleHorizontal += 0.02f;
        position.lx = sin(position.angleHorizontal);
        position.lz = -cos(position.angleHorizontal);
    }
}
