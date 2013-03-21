#ifndef WORLD_H
#define WORLD_H

#include "map/map.h"
#include "player.h"

class World
{
public:
    static World *instance();

    bool initializeMap(const char *filename);
    Map *getMap();

    bool initializePlayer();
    Player *getPlayer();

protected:
    World();
    ~World();

    Map *m_pMap;
    Player *m_pPlayer;
};

#endif // WORLD_H
