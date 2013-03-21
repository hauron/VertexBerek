#ifndef SOUND_H
#define SOUND_H

#include <al.h>

/*
 * ALboolean LoadALData()
 *
 *	This function will load our sample data from the disk using the Alut
 *	utility and send the data into OpenAL as a buffer. A source is then
 *	also created to play that buffer.
 */
ALboolean LoadALFile( char* Filename, int index, ALboolean loop );
ALboolean LoadALFileFull( char* Filename, ALboolean loop, ALuint* Bfr );
ALboolean LoadALData();


int PlaySoundAL( ALuint buffer_index, float xtrans, float ytrans, float ztrans );

/*
 * void SetListenerValues()
 *
 *	We already defined certain values for the Listener, but we need
 *	to tell OpenAL to use that data. This function does just that.
 */
void SetListenerValues( float* ListenerPos, float* ListenerVel, float* ListenerOri );


/*
 * void KillALData()
 *
 *	We have allocated memory for our buffers and sources which needs
 *	to be returned to the system. This function frees that memory.
 */
void KillALData();

bool SoundInit();
void HaltAllSounds();
void ChangeVolume( int x );

void StopSound( ALuint sound_source );

// play sound in a specific source - allows for easy control over first few sources (for special use)
void PlaySoundALReserved(ALuint source_index, ALuint buffer_index, float xtrans, float ytrans, float ztrans );

//funkcja testujaca = puszczamy wszystkie dzwieki
void PlayAllSounds();

void AlterSourcePosition( int source_index, float* Position );


#endif // SOUND_H
