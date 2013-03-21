
#include "structure.h"

// kod odpowiedzialny za poruszanie modelami - aktywacja na dwojaki sposob: przez polozenie uzytkownika lub przycisk klawiatury
MOVEMENT MoveGL( INSTANCE* instance );

void LightUp();

// do naprawienia/uzupelnienia
bool CheckCollision( float x, float y, float z, MAP* map );

void RotateGL( double x, double y, double z );

// jezeli uzytkownik sie poruszyl
void MovingCamera();