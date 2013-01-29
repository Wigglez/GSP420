// Audio Core
// Core Lead: Derek Finch

#ifndef __COREAUDIO_H__
#define __COREAUDIO_H__

////////////////////////////////////////////
// Includes
////////////////////////////////////////////
#include "Typedefs.h"
#include <fmod.hpp>

////////////////////////////////////////////
// Definitions
////////////////////////////////////////////
#define MAX_SOUND_CHANNELS 100
#define INVALID_SOUND_INDEX 0

////////////////////////////////////////////
// Name: SOUND_TYPE
// Type: enumeration
// Description: Flags for different sound types
////////////////////////////////////////////
typedef enum {
	SOUND_TYPE_INVALID = 0, // The sound is not valid
	SOUND_TYPE_2D_SOUND, // The sound should be played once
	SOUND_TYPE_2D_SOUND_LOOPED, // The sound should be looped
} SOUND_TYPE;

////////////////////////////////////////////
// Name: SoundInstance
// Type: class
// Description: Handles single instances of loaded audio files
// Singleton: false
////////////////////////////////////////////
class SoundInstance {
public:
	////////////////////////////////////////////
	// Name: Clear
	// Type: Void
	// Parameters: None
	// Description: Clears the current sound instances in use
	////////////////////////////////////////////
	Void Clear();

	////////////////////////////////////////////
	// Variables
	////////////////////////////////////////////
	String fileName; // The current sound instance's fileName
	SOUND_TYPE soundType; // The type of sound the file is (looped or not)
};


////////////////////////////////////////////
// Name: SoundManager
// Type: class
// Description: Manages sounds
// Singleton: true
////////////////////////////////////////////
class SoundManager {
public:
	////////////////////////////////////////////
	// Name: &GetInstance
	// Type: static SoundManager
	// Parameters: None
	// Description: This is the entry point to SoundManager and should be called to get a single instance
	// Usage Example: SoundManager *soundMgr = &soundMgr->GetInstance()
	////////////////////////////////////////////
	static SoundManager &GetInstance() {
		static SoundManager soundMgr;
		return soundMgr;
	}

	////////////////////////////////////////////
	// Name: Initialize
	// Type: Void
	// Parameters: None
	// Description: Creates the system object (audio device)
	////////////////////////////////////////////
	Void Initialize();
	
	////////////////////////////////////////////
	// Name: CreateSound
	// Type: Int
	// Parameters: String
	// Description: Returns a sound that is not looped
	////////////////////////////////////////////
	Int CreateSound(String &fileName); 

	////////////////////////////////////////////
	// Name: CreateLoopedSound
	// Type: Int
	// Parameters: String
	// Description: Returns a sound that is looped
	////////////////////////////////////////////
	Int CreateLoopedSound(String &fileName);

	////////////////////////////////////////////
	// Name: CreateSound
	// Type: Int
	// Parameters: String, SOUND_TYPE
	// Description: Sets up a sound for use
	////////////////////////////////////////////
	Int CreateSound(String &fileName, SOUND_TYPE soundType); 

	////////////////////////////////////////////
	// Name: PlaySound
	// Type: Void
	// Parameters: None
	// Description: Plays a given sound
	////////////////////////////////////////////
	Void PlaySound();

	////////////////////////////////////////////
	// Name: StopSound
	// Type: Void
	// Parameters: None
	// Description: Stops a given sound
	////////////////////////////////////////////
	Void StopSound();

	////////////////////////////////////////////
	// Name: GetSoundLength
	// Type: Float
	// Parameters: None
	// Description: Returns sound length in seconds
	////////////////////////////////////////////
	Float GetSoundLength();

protected:
	////////////////////////////////////////////
	// Variables
	////////////////////////////////////////////
	Int nextSoundInstanceIndex; // Counter for SoundInstance
	FMOD::Channel *fchannel;
	FMOD::System *fsystem;
	FMOD::Sound *fsound;

private:
	////////////////////////////////////////////
	// Name: SoundManager
	// Type: None
	// Parameters: None
	// Description: Default constructor
	////////////////////////////////////////////
	SoundManager() {}

	////////////////////////////////////////////
	// Name: SoundManager
	// Type: None
	// Parameters: const
	// Description: Helper function for singleton
	////////////////////////////////////////////
	SoundManager(const SoundManager&); 

	////////////////////////////////////////////
	// Name: SoundManager
	// Type: None
	// Parameters: const
	// Description: Allows &GetInstance to be tagged to the SoundManager
	////////////////////////////////////////////
	SoundManager& operator=(const SoundManager&);

	////////////////////////////////////////////
	// Name: SoundManager
	// Type: None
	// Parameters: None
	// Description: Destructor that releases any objects that may still exist
	////////////////////////////////////////////
	~SoundManager();

	////////////////////////////////////////////
	// FMOD Callbacks
	// Description: Currently unused
	////////////////////////////////////////////
	//static FMOD_RESULT F_CALLBACK fmodFileOpenCallback(const Char *fileName, Int unicode, uInt *filesize, Void **handle, Void **userdata);
	//static FMOD_RESULT F_CALLBACK fmodFileCloseCallback(Void *handle, Void *userdata);
	//static FMOD_RESULT F_CALLBACK fmodFileReadCallback(Void *handle, Void *buffer, uInt sizebytes, uInt *bytesread, Void *userdata);
	//static FMOD_RESULT F_CALLBACK fmodFileSeekCallback(Void *handle, uInt pos, Void *userdata);
};

#endif