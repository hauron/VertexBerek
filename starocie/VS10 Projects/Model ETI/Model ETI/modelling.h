

#include "math.h"
#include "stdio.h"
#include "structure.h"
#include "actions.h"

void readstr(FILE *f,char *string);

// ladowanie modeli, podajemy indeks w tablicy modeli (da sie to zrobic lepiej?) oraz nazwe pliku, program sam laduje model
void LoadModelMovementGL( INSTANCE* instance, char* file_dir );

void LoadGLModel( MODEL* model, char* file_dir );

int LoadGLSector( SECTOR* sector, char* file_dir );

void LoadGLMap( MAP* map, char* file_dir );

void DrawTriangleGL( TRIANGLE* triangle, POINT_XYZ* p, POINT_XYZ* normal );

int DrawGLModel(MODEL* GLModel, MOVEMENT wektor);

int DrawGLSector( SECTOR* sector );

void DrawGLMap( MAP* map );

void SetLights();

POINT_XYZ* NormalizeNormalGL( POINT_XYZ* v);

POINT_XYZ* ComputeNormalGL(POINT_XYZ* v);

