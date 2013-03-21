
#include "sound.h"
#include <windows.h>
#include <alc.h>
#include <alut.h>

int SOUND_LEVEL = 10;

#define NUM_OF_SOURCES 20

/*
 * These are OpenAL "names" (or "objects"). They store and id of a buffer
 * or a source object. Generally you would expect to see the implementation
 * use values that scale up from '1', but don't count on it. The spec does
 * not make this mandatory (as it is OpenGL). The id's can easily be memory
 * pointers as well. It will depend on the implementation.
 */

// Buffers to hold sound data.
ALuint Buffer[256];

// Sources are points of emitting sound.
ALuint Source[NUM_OF_SOURCES];
ALboolean bSource[NUM_OF_SOURCES];
// 0 - muzyka gry,
// 1 - klikniecia myszy
// 2 - ruch bohatera
// reszta dynamicznie


/*
 * These are 3D cartesian vector coordinates. A structure or class would be
 * a more flexible of handling these, but for the sake of simplicity we will
 * just leave it as is.
 */

// Position of the source sound.
//ALfloat SourcePos[256][3];

// Velocity of the source sound.
//ALfloat SourceVel[256][3];
// uzywajac tych - mozemy zmieniac polozenie zrodla, ciekawe jak to dziala ;-)
ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
ALfloat SourceVel[] = { 1.0, 1.0, 1.0 };

// Position of the listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

// Velocity of the listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

void FixSources()
{
	ALint val;
	for( int i = 4; i < NUM_OF_SOURCES; i++ )
	{
		 alGetSourcei( Source[i], AL_SOURCE_STATE, &val);
		 if( val != AL_PLAYING )
		 {
			 bSource[i] = 0;
		 }
	}
}

ALboolean LoadALFileFull( char* Filename, ALboolean loop, ALuint* BufferFile )
{
	alGetError();
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	
	alGenBuffers(1, BufferFile);

	if(alGetError() != AL_NO_ERROR)
	{	
		MessageBox( NULL, "Blad przy generowaniu bufora", "Argh!", MB_OK );
		return AL_FALSE;
	}

	alutLoadWAVFile(Filename, &format, &data, &size, &freq, &loop);
	alBufferData(*BufferFile, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);


			ALenum err = alGetError();
			if(1)
			{	
				switch(err)
				{
					case AL_INVALID_NAME:
						MessageBox( NULL, "AL_INVALID_NAME", "Argh!", MB_OK );
					break;

					case AL_INVALID_ENUM:
						MessageBox( NULL, "AL_INVALID_ENUM", "Argh!", MB_OK );
					break;

					case AL_INVALID_VALUE:
						MessageBox( NULL, "AL_INVALID_VALUE", "Argh!", MB_OK );
					break;

					case AL_INVALID_OPERATION:
						MessageBox( NULL, "AL_INVALID_OPERATION", "Argh!", MB_OK );
					break;

					case AL_OUT_OF_MEMORY:
					     MessageBox( NULL, "AL_OUT_OF_MEMORY", "Argh!", MB_OK );
					break;
				};
			}

	if(alGetError() == AL_NO_ERROR)
		return AL_TRUE;

} 

/*
 * ALboolean LoadALData()
 *
 *	This function will load our sample data from the disk using the Alut
 *	utility and send the data into OpenAL as a buffer. A source is then
 *	also created to play that buffer.
 */
 
ALboolean LoadALFile( char* Filename, int index, ALboolean loop )
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	
	alGenBuffers(1, &Buffer[index]);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alutLoadWAVFile(Filename, &format, &data, &size, &freq, &loop);
	alBufferData(Buffer[index], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// Bind the buffer with the source.

	if(alGetError() != AL_NO_ERROR)
	{
		MessageBox( NULL, "Nie udalo sie wczytac bufora dzwieku", "Argh!", MB_OK );
		return AL_FALSE;
	}


	alSourcei (Source[index], AL_BUFFER,   Buffer[index]   );
	alSourcef (Source[index], AL_PITCH,    1.0      );
	alSourcef (Source[index], AL_GAIN,     1.0      );
	alSourcefv(Source[index], AL_POSITION, SourcePos);
	alSourcefv(Source[index], AL_VELOCITY, SourceVel);
    alSourcei (Source[index], AL_LOOPING,  loop     );

	if(alGetError() == AL_NO_ERROR)
		return AL_TRUE;
	MessageBox( NULL, "Nie udalo sie zaladowac dzwieku", "Argh!", MB_OK );
    return 1;      
} 
ALboolean LoadALData()
{
	// Variables to load into.
	
	// Load wav data into a buffer.
	if
		( !LoadALFile( "data/sound/footsteps1.wav", 0, 0 ) ||
		 !LoadALFile( "data/sound/door_open.wav", 1, 0 ) ||
		 !LoadALFile( "data/sound/elevator2.wav", 2, 0 ) ||
		 !LoadALFile( "data/sound/theme.wav", 3, 0 ))
	{
         MessageBox( NULL, "Nie udalo sie zaladowac dzwieku", "Argh!", MB_OK );
         //return 0;
	}
	// Do another error check and return.

	if(alGetError() == AL_NO_ERROR)
		return AL_TRUE;

	return AL_FALSE;
}



/*
 * void SetListenerValues()
 *
 *	We already defined certain values for the Listener, but we need
 *	to tell OpenAL to use that data. This function does just that.
 */
void SetListenerValues( float* ListenerPos, float* ListenerVel, float* ListenerOri )
{
	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}
int PlaySoundAL( ALuint buffer_index, float xtrans, float ytrans, float ztrans )
{
	ALfloat SourcePosx[3] = { xtrans, ytrans, ztrans };
	FixSources();
	for( int i = 4; i < NUM_OF_SOURCES; i++ )
	{
		if( bSource[i] == 0 )
		{	
			alGetError(); 
			// Bind the buffer with the source.
			alSourceStop( Source[i] ); // na wszelki wypadek
			alSourcei (Source[i], AL_BUFFER,   Buffer[buffer_index]   );
			alSourcef (Source[i], AL_PITCH,    1.0      );
			alSourcef (Source[i], AL_GAIN,     (double)SOUND_LEVEL/10      );
			alSourcefv(Source[i], AL_POSITION, SourcePosx);
			alSourcefv(Source[i], AL_VELOCITY, SourceVel);
			alSourcei (Source[i], AL_LOOPING,  0     );
			ALenum err = alGetError();
			if(1)
			{	
				switch(err)
				{
					case AL_INVALID_NAME:
						MessageBox( NULL, "AL_INVALID_NAME", "Argh!", MB_OK );
					break;

					case AL_INVALID_ENUM:
						MessageBox( NULL, "AL_INVALID_ENUM", "Argh!", MB_OK );
					break;

					case AL_INVALID_VALUE:
						MessageBox( NULL, "AL_INVALID_VALUE", "Argh!", MB_OK );
					break;

					case AL_INVALID_OPERATION:
						MessageBox( NULL, "AL_INVALID_OPERATION", "Argh!", MB_OK );
					break;

					case AL_OUT_OF_MEMORY:
					     MessageBox( NULL, "AL_OUT_OF_MEMORY", "Argh!", MB_OK );
					break;
				};
			}
			alSourcePlay( Source[i] );
			bSource[i] = 1;
			return i;
		}
	}
	return -1;

}

/*
 * void KillALData()
 *
 *	We have allocated memory for our buffers and sources which needs
 *	to be returned to the system. This function frees that memory.
 */
void KillALData()
{
    for( int i = 0; i < NUM_OF_SOURCES; i++ )
    {
    	alDeleteBuffers(1, &Buffer[i]);
	    alDeleteSources(1, &Source[i]);
    }
	alutExit();
}

void AlterSourcePosition( int source_index, float* Position )
{
		alSourcefv(Source[source_index], AL_POSITION, Position);
}

// play sound in a specific source - allows for easy control over first few sources (for special use)
void PlaySoundALReserved(ALuint source_index, ALuint buffer_index, float xtrans, float ytrans, float ztrans )
{
	// check, if the sound is currently being played - skip if yes
	ALint val;
	alGetSourcei(Source[source_index], AL_SOURCE_STATE, &val);
    if( val == AL_PLAYING )
         return;

	ALfloat SourcePosx[3] = { xtrans, ytrans, ztrans };
	FixSources();
	alGetError(); 
	// Bind the buffer with the source.
	alSourceStop( Source[source_index] ); // na wszelki wypadek
	alSourcei (Source[source_index], AL_BUFFER,   Buffer[buffer_index]   );
	alSourcef (Source[source_index], AL_PITCH,    1.0      );
	alSourcef (Source[source_index], AL_GAIN,     (double)SOUND_LEVEL/10      );
	alSourcefv(Source[source_index], AL_POSITION, SourcePosx);
	alSourcefv(Source[source_index], AL_VELOCITY, SourceVel);
	alSourcei (Source[source_index], AL_LOOPING,  0     );
	ALenum err = alGetError();
	if(1)
	{	
		switch(err)
		{
			case AL_INVALID_NAME:
				MessageBox( NULL, "AL_INVALID_NAME", "Argh!", MB_OK );
			break;

			case AL_INVALID_ENUM:
				MessageBox( NULL, "AL_INVALID_ENUM", "Argh!", MB_OK );
			break;

			case AL_INVALID_VALUE:
				MessageBox( NULL, "AL_INVALID_VALUE", "Argh!", MB_OK );
			break;

			case AL_INVALID_OPERATION:
				MessageBox( NULL, "AL_INVALID_OPERATION", "Argh!", MB_OK );
			break;

			case AL_OUT_OF_MEMORY:
					MessageBox( NULL, "AL_OUT_OF_MEMORY", "Argh!", MB_OK );
			break;
		};
	}
	alSourcePlay( Source[source_index] );
	bSource[source_index] = 1;
}


bool SoundInit()
{
	// Initialize OpenAL and clear the error bit.

	alutInit(NULL, 0);
	alGetError();
    alGenSources(NUM_OF_SOURCES, Source );
	// Load the wav data.

	if(LoadALData() == AL_FALSE)
		return 0;
    SetListenerValues( ListenerPos, ListenerVel, ListenerOri );

	// Setup an exit procedure.

	// Loop.
	return 1;
}

void HaltAllSounds()
{
	for( int i = 0; i < 32; i++ )
		alSourcePause( Source[i] );
}

void StopSound( ALuint sound_source )
{
	alSourceStop( Source[sound_source] );
}
void ChangeVolume( int x )
{	
	SOUND_LEVEL = x;
}

void PlayAllSounds()
{
	for( int i = 0; i < 256; i++ )
	{
		if( Buffer[i] != 0 )
			PlaySoundAL( i, 0, 0, 0);
	}
}