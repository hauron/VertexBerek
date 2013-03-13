
#include "actions.h"
#include "sound.h"

extern float xtrans;
extern float ytrans;
extern float ztrans;
extern bool keys[256];

float x_light = 0.0f, y_light = 0.0f, z_light= 0.0f;


MOVEMENT MoveGL( INSTANCE* instance  )
{
	MOVEMENT wektor;
	wektor.xm = 1; wektor.ym = 1; wektor.zm = 1; wektor.x_angle = 0; wektor.y_angle = 0; wektor.z_angle = 0;


		// sprawdz pozycje, porownaj ze stalymi
	if( ( xtrans >= instance->action_moving.A.x &&
		xtrans <= instance->action_moving.B.x &&
		ytrans >= instance->action_moving.A.y &&
		ytrans <= instance->action_moving.B.y &&
		ztrans >= instance->action_moving.A.z && 
		ztrans <= instance->action_moving.B.z ) &&
		( keys[instance->action_moving.activation] || instance->action_moving.activation == '0' || instance->action_moving.increasing == 1 ) )
	{
		// uruchamiamy ruch
		if( !(instance->action_moving.increasing) )
		{
			if( instance->action_moving.sound_source == -1 )
			{
				instance->action_moving.sound_source = 
						PlaySoundAL( instance->action_moving.sound_buffer, 
									instance->action_moving.x, 
									instance->action_moving.y, 
									instance->action_moving.z ); 
			}
			else
			{
				StopSound( instance->action_moving.sound_source );
				instance->action_moving.sound_source = 
						PlaySoundAL( instance->action_moving.sound_buffer, 
									instance->action_moving.x, 
									instance->action_moving.y, 
									instance->action_moving.z ); 
			}
		}
		instance->action_moving.increasing = 1;
	}
	else
	{
		// roslo, nagle pzrestaje - zatrzymaj dzwiek, przygotuj do puszczenia od nowa
		if( instance->action_moving.increasing )
		{
			StopSound( instance->action_moving.sound_source );
			instance->action_moving.sound_source = -1 ;
		}
		instance->action_moving.increasing = 0;
	}

	if( instance->action_moving.increasing == 1  )
	{

		if( instance->action_moving.current != instance->action_moving.n )
			instance->action_moving.current++;
		else
		{
			StopSound( instance->action_moving.sound_source );
			instance->action_moving.sound_source = -1;
		}
	}
	else
	{
		if( instance->action_moving.current != 0 )
		{
			// jezeli dzwiek nie jest puszczony i zaczyna spadac...
			if( instance->action_moving.sound_source == -1 )
			{
					instance->action_moving.sound_source = 
					PlaySoundAL( instance->action_moving.sound_buffer, 
								instance->action_moving.x, 
								instance->action_moving.y, 
								instance->action_moving.z ); 
			}
			instance->action_moving.current--;
		}
		else
		{
			if( instance->action_moving.sound_source != -1 )
			{
				StopSound( instance->action_moving.sound_source );
				instance->action_moving.sound_source = -1 ;
			}
		}
	}

	wektor.x = instance->wektor.x + instance->action_moving.x*((float)instance->action_moving.current/(float)instance->action_moving.n); // wartosc bazowa + jakas czesc wartosci koncowej (current moze byc funkcja, np. sinus)
	wektor.y = instance->wektor.y + instance->action_moving.y*((float)instance->action_moving.current/(float)instance->action_moving.n); 
	wektor.z = instance->wektor.z + instance->action_moving.z*((float)instance->action_moving.current/(float)instance->action_moving.n);
	/*
	if( instance->action_moving.sound_source != -1 )
	{
		float Position[] = { wektor.x, wektor.y, wektor.z };
		AlterSourcePosition( instance->action_moving.sound_source, Position );
	} */
	return wektor;
}


void LightUp()
{
	float light_speed = 0.03f;
	if( keys['Y'] )
		z_light+=light_speed;
	if( keys['H'] )
		z_light-=light_speed;
	if( keys['G'] )
		x_light+=light_speed;
	if( keys['J'] )
		x_light-=light_speed;
	if( keys['T'] )
		y_light-=light_speed;
	if( keys['U'] )
		y_light+=light_speed;
}

// do naprawienia/uzupelnienia
bool CheckCollision( float x, float y, float z, MAP* map )
{
	return true;
}

void RotateGL( double x, double y, double z )
{
	glRotated( x, 1.0f, 0.0f, 0.0f );
	glRotated( y, 0.0f, 1.0f, 0.0f );
	glRotated( z, 0.0f, 0.0f, 1.0f );
}
// source[i] - odglos poruszania sie - CHODU!!
void MovingCamera()
{
	if( keys['S'] || keys['A'] || keys['W'] || keys['D'] )
	{
		float ListenerPos[3] = { xtrans, ytrans, ztrans };
		float ListenerVel[3] = { 0, 0, 0 };
		float ListenerOri[6] = { 0, 0, 1, 0, 0, -1 };
		SetListenerValues( ListenerPos, ListenerVel, ListenerOri );
		PlaySoundALReserved( 1, 0, xtrans, ytrans, ztrans );

	}
	else
		StopSound( 1 );
	//	StopSound( footsteps_source );
}