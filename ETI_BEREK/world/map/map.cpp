#include "map.h"

#include "graphics/modelling.h"

Map::Map()
{
}

Map::~Map()
{

}

Map *Map::load(const char *filename)
{
    Map *map = new Map();
    LoadGLMap(map, filename);
    return map;
}
