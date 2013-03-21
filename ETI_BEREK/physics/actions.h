#ifndef ACTIONS_H
#define ACTIONS_H

#include "world/map/map.h"
#include "utils/structures.h"

// kod odpowiedzialny za poruszanie modelami - aktywacja na dwojaki sposob: przez polozenie uzytkownika lub przycisk klawiatury
MOVEMENT MoveGL( INSTANCE* instance );

void LightUp();

// do naprawienia/uzupelnienia
bool CheckCollision( float x, float y, float z, Map* map );

void RotateGL( double x, double y, double z );

// jezeli uzytkownik sie poruszyl
void MovingCamera();

#endif // ACTIONS_H
