#ifndef MAP_H
#define MAP_H

#include "sector.h"

class Map
{
public:
    Map();
    ~Map();

    static Map* load(const char *filename);

    int n_sectors;
    char title[128];
    Sector sector[16]; // kazda mapa sklada sie z sektorow
};

#endif // MAP_H
