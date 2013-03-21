#ifndef SECTOR_H
#define SECTOR_H

#include "utils/structures.h"

class Sector
{
public:
    Sector();

    char title[256];
    int n_models, n_instances;
    double rotation;
    float x, y, z; // wektor przesuniecia
    float xm, ym, zm; // wektor odbicia
    MODEL models[256]; // modele w sektorze
    INSTANCE instances[256]; // wystapienia powyzszych

};

#endif // SECTOR_H
