#include "world.h"

World *World::instance()
{
    static World world;
    return &world;
}

bool World::initializeMap(const char *filename)
{
    if(m_pMap) delete m_pMap;

    m_pMap = Map::load(filename);

    if(m_pMap == 0)
        return false;
    return true;
}

Map *World::getMap()
{
    return m_pMap;
}

bool World::initializePlayer()
{
    m_pPlayer = new Player();
    return true;
}

Player *World::getPlayer()
{
    return m_pPlayer;
}

World::World()
{
    m_pMap = 0;
    m_pPlayer = 0;
}

World::~World()
{
    if(m_pMap) delete m_pMap;
    if(m_pPlayer) delete m_pPlayer;
}

