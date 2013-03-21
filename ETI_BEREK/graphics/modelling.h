#ifndef MODELLING_H
#define MODELLING_H


#include <cmath>
#include <cstdio>
#include "utils/structures.h"

#include "graphics/graphicssystem.h"
#include "physics/actions.h"

#include "world/map/map.h"

void readLine(FILE *f,char *string);

// ladowanie modeli, podajemy indeks w tablicy modeli (da sie to zrobic lepiej?) oraz nazwe pliku, program sam laduje model
void LoadModelMovementGL( INSTANCE* instance, char* file_dir );

void LoadGLModel( MODEL* model, char* file_dir );
void LoadGLSector( Sector* sector, char* file_dir );
void LoadGLMap(Map* map, const char *file_dir);

void DrawTriangleGL( TRIANGLE* triangle, POINT_XYZ* p, POINT_XYZ* normal );

int DrawGLModel(MODEL* GLModel, MOVEMENT wektor);
int DrawGLSector(Sector* sector );
void DrawGLMap( Map* map );

void SetLights();

POINT_XYZ* NormalizeNormalGL( POINT_XYZ* v);
POINT_XYZ* ComputeNormalGL(POINT_XYZ* v);

#endif // MODELLING_H
