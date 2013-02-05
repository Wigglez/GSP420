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
#define INVALID_SOUND_CHANNEL -1

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
	// Name: Shutdown
	// Type: Void
	// Parameters: None
	// Description: Releases any objects that may still exist
	////////////////////////////////////////////
	Void Shutdown();

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
	// Parameters: Int, Int
	// Description: Plays a given sound
	////////////////////////////////////////////
	Void PlaySound(Int soundIndex, Int *channelIndex);

	////////////////////////////////////////////
	// Name: StopSound
	// Type: Void
	// Parameters: Int
	// Description: Stops a given sound
	////////////////////////////////////////////
	Void StopSound(Int *channelIndex);

	////////////////////////////////////////////
	// Name: FindSound
	// Type: Int
	// Parameters: String, SOUND_TYPE
	// Description: Verifies and returns the sound index
	////////////////////////////////////////////
	Int FindSound(String &fileName, SOUND_TYPE soundType);

	////////////////////////////////////////////
	// Name: StopAllSounds
	// Type: Void
	// Parameters: 
	// Description: Stops all sounds
	////////////////////////////////////////////
	Void StopAllSounds();

	////////////////////////////////////////////
	// Name: GetSoundLength
	// Type: Float
	// Parameters: Int
	// Description: Returns sound length in seconds
	////////////////////////////////////////////
	Float GetSoundLength(Int soundIndex);

	////////////////////////////////////////////
	// Name: GetSoundInstance
	// Type: SoundInstance
	// Parameters: Int
	// Description: Gets the current sound instance
	////////////////////////////////////////////
	SoundInstance *GetSoundInstance(Int soundIndex);

protected:
	////////////////////////////////////////////
	// Variables
	////////////////////////////////////////////
	Int nextSoundInstanceIndex; // Counter for SoundInstance
	typedef std::vector<SoundInstance*> SoundInstanceVector;
	typedef SoundInstanceVector::iterator SoundInstanceVectorItr;

	SoundInstanceVector *soundInstanceVector;

	FMOD::Channel *fchannel;
	FMOD::System *fsystem;
	FMOD::Sound *fsound;

	FMOD_RESULT result;


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
	// Description: Destructor
	////////////////////////////////////////////
	~SoundManager() {};

	////////////////////////////////////////////
	// Name: IncrementNextSoundInstanceIndex
	// Type: Void
	// Parameters: None
	// Description: Deals with sound instance indexes
	////////////////////////////////////////////
	Void IncrementNextSoundInstanceIndex();

	////////////////////////////////////////////
	// FMOD Callbacks
	// Description: Currently unused
	////////////////////////////////////////////
	static FMOD_RESULT F_CALLBACK fmodFileOpenCallback(const Char *fileName, Int unicode, uInt *filesize, Void **handle, Void **userdata);
	static FMOD_RESULT F_CALLBACK fmodFileCloseCallback(Void *handle, Void *userdata);
	static FMOD_RESULT F_CALLBACK fmodFileReadCallback(Void *handle, Void *buffer, uInt sizebytes, uInt *bytesread, Void *userdata);
	static FMOD_RESULT F_CALLBACK fmodFileSeekCallback(Void *handle, uInt pos, Void *userdata);
};

#endif