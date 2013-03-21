#ifndef PHYSICSUTILS_H
#define PHYSICSUTILS_H

#include <cmath>

struct Position
{
    Position();

    float x, y, z;
    float lx, ly, lz;
    float angleHorizontal, angleVertical;
};

enum PhysicsType
{
    PHYSICS_MOVE_FORWARD    = 0x0001,
    PHYSICS_MOVE_BACKWARD   = 0x0002,
    PHYSICS_MOVE_LEFT       = 0x0004,
    PHYSICS_MOVE_RIGHT      = 0x0008,
    PHYSICS_MOVE_UP         = 0x0010,
    PHYSICS_MOVE_DOWN       = 0x0020,
    PHYSICS_TURN_LEFT       = 0x0040,
    PHYSICS_TURN_RIGHT      = 0x0080
};

#include <QDebug>
#include <QString>
struct Physics
{
    Physics();

    unsigned movement;

    QString toString()
    {
        QString str;
        for(int i = 31; i >= 0; i--)
            str.append(movement & (1 << i) ? '1' : '0');
        return str;
    }

    void setBit(int mask, bool state)
    {
        if(state)   movement |= (mask);
        else        movement ^= (mask);

        qDebug() << "setting bit:" << mask << "- to:" << state << "- result:" << toString();
    }

    void setMovingForward(bool state)
    {
        setBit(PHYSICS_MOVE_FORWARD, state);
    }

    void setMovingBackward(bool state)
    {
        setBit(PHYSICS_MOVE_BACKWARD, state);
    }

    void setMovingLeft(bool state)
    {
        setBit(PHYSICS_MOVE_LEFT, state);
    }

    void setMovingRight(bool state)
    {
        setBit(PHYSICS_MOVE_RIGHT, state);
    }

    void setMovingUp(bool state)
    {
        setBit(PHYSICS_MOVE_UP, state);
    }

    void setMovingDown(bool state)
    {
        setBit(PHYSICS_MOVE_DOWN, state);
    }

    void setTurningLeft(bool state)
    {
        setBit(PHYSICS_TURN_LEFT, state);
    }

    void setTurningRight(bool state)
    {
        setBit(PHYSICS_TURN_RIGHT, state);
    }

};

namespace PhysicsUtils
{
    const float fraction = 0.1f;
    const float angleMouse = 0.01f;
    const float flightSpeed = 0.07f;

    void move(Position &position, Physics &movement, float timePassed);
}

#endif // PHYSICSUTILS_H
