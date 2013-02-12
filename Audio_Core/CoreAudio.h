// Audio Core
// Core Lead: Derek Finch

#ifndef __COREAUDIO_H__
#define __COREAUDIO_H__

////////////////////////////////////////////
// Includes
////////////////////////////////////////////
#include "Typedefs.h"
#include "CoreIO.h"

#include <fmod.hpp>
#include <fmod_errors.h>
#include <assert.h>

////////////////////////////////////////////
// Definitions
////////////////////////////////////////////
#define MAX_SOUND_CHANNELS 100
#define INVALID_SOUND_INDEX 0
#define INVALID_SOUND_CHANNEL -1
#define INITIAL_VECTOR_SIZE 100
#define INCREASE_VECTOR_SIZE 20

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
	// Variables
	////////////////////////////////////////////
	String fileName; // The current sound instance's fileName
	SOUND_TYPE soundType; // The type of sound the file is (looped or not)
	FMOD::Sound *fsound;

	////////////////////////////////////////////
	// Name: Clear
	// Type: Void
	// Parameters: None
	// Description: Clears the current sound instances in use
	////////////////////////////////////////////
	Void Clear();
};

////////////////////////////////////////////
// Name: ISoundManager
// Type: class
// Description: Interface for the SoundManager
// Singleton: false
////////////////////////////////////////////
class ISoundManager {
protected:
	ISoundManager() {} // Default constructor
	ISoundManager(const ISoundManager &o); // Copy constructor
	const ISoundManager &operator=(const ISoundManager &o); // Assignment
public:
	virtual ~ISoundManager() {}
	
	virtual Int CreateSound(String &fileName) = 0;
	virtual Int CreateLoopedSound(String &fileName) = 0;
	virtual Void PlaySound(Int soundIndex, Int *channelIndex) = 0;
	virtual Void StopSound(Int *channelIndex) = 0;
	virtual Void StopAllSounds() = 0;
	virtual Float GetSoundLength(Int soundIndex) = 0;
	

};

////////////////////////////////////////////
// Name: SoundManager
// Type: class
// Description: Manages sounds
// Singleton: true
////////////////////////////////////////////
class SoundManager : public ISoundManager {
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

	FMOD_RESULT result;

	////////////////////////////////////////////
	// Name: SoundManager
	// Type: None
	// Parameters: None
	// Description: Default constructor
	////////////////////////////////////////////
	SoundManager() {}

	////////////////////////////////////////////
	// Name: ~SoundManager
	// Type: virtual
	// Parameters: None
	// Description: Destructor
	////////////////////////////////////////////
	virtual ~SoundManager() {}

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

private:
	
};

#endif