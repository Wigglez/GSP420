// Audio Core
// Core Lead: Derek Finch

#include "CoreAudio.h"

////////////////////////////////////////////
// Error Checking for FMOD
////////////////////////////////////////////
Void ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
} // ErrorCheck

////////////////////////////////////////////
// Clears the SoundInstance
////////////////////////////////////////////
Void SoundInstance::Clear() {
	fileName.clear();
	soundType = SOUND_TYPE_INVALID;
	fsound = NULL;
}

////////////////////////////////////////////
// Shutdown can be called to handle the shutdown
////////////////////////////////////////////
Void SoundManager::Shutdown() {
	// If we still have our system active upon shutdown, we should close and release it
	// This should also remove any sounds still lingering in channels
	if(fsystem) {
		result = fsystem->close();
		ErrorCheck(result);

		result = fsystem->release();
		ErrorCheck(result);
	}

	// Clear any indexes that may still be active
	for(Int vectorIndex = 0; vectorIndex < (Int)soundInstanceVector->capacity(); vectorIndex++) {
		soundInstanceVector->at(vectorIndex)->fileName.clear();
		delete soundInstanceVector->at(vectorIndex);
	}

	// No longer need the soundInstanceVector
	delete soundInstanceVector;
} // SoundManager::Shutdown

////////////////////////////////////////////
// Initialization
// The system gets created here for the audio device
// We also check compatibilities with FMOD
////////////////////////////////////////////
Void SoundManager::Initialize() {
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	uInt version;
	Int numdrivers;
	Char name[256];

	// Create the system object
	result = FMOD::System_Create(&fsystem);
	ErrorCheck(result);

	result = fsystem->getVersion(&version);
	ErrorCheck(result);

	if(version < 00044006) {
		printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, 00044006);
		return;
	}

	result = fsystem->getNumDrivers(&numdrivers);
	ErrorCheck(result);

	if(numdrivers == 0) {
		result = fsystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ErrorCheck(result);
	} else {
		result = fsystem->getDriverCaps(0, &caps, 0, &speakermode);
		ErrorCheck(result);

		// Set the user selected speaker mode
		result = fsystem->setSpeakerMode(speakermode);
		ErrorCheck(result);

		if(caps & FMOD_CAPS_HARDWARE_EMULATED) {
			// The user has the 'Acceleration' slider set to off. This is terrible for latency. User should be warned.
			result = fsystem->setDSPBufferSize(1024, 10);
			ErrorCheck(result);
		}

		result = fsystem->getDriverInfo(0, name, 256, 0);
		ErrorCheck(result);

		if(strstr(name, "SigmaTel")) {
			// Sigmatel sound devices crackle if the format is PCM 16bit. 
			// PCM floating point output seems to solve it.
			result = fsystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			ErrorCheck(result);
		}
	}

	// Initialize to have max channels on normal settings
	result = fsystem->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);
	if(result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
		// The speaker mode selected isn't support by the sound card.
		// Switching it to stereo
		result = fsystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		ErrorCheck(result);

		// re-init
		result = fsystem->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);
	}
	ErrorCheck(result);

	// Enable the callbacks for file management
	result = fsystem->setFileSystem(&fmodFileOpenCallback, &fmodFileCloseCallback, &fmodFileReadCallback, &fmodFileSeekCallback, NULL, NULL, 2048);

	////////////////////////////////////////////
	// Since the constructor isn't being used
	// the init will have to take care of some initialization for variables
	////////////////////////////////////////////
	fsystem = NULL;
	soundInstanceVector = new SoundInstanceVector;
	nextSoundInstanceIndex = 0;

	// INITIAL_VECTOR_SIZE can be started off from soundInstanceVectors and can scale from here
	soundInstanceVector->resize(INITIAL_VECTOR_SIZE);
	for(Int vectorIndex = 0; vectorIndex < INITIAL_VECTOR_SIZE; vectorIndex++) {
		soundInstanceVector->at(vectorIndex) = new SoundInstance;
		soundInstanceVector->at(vectorIndex)->Clear();
	}

	printf("SoundManager initialized\n");
} // SoundManager::Initialize

////////////////////////////////////////////
// Designate a sound type of 2D sound to a given sound
////////////////////////////////////////////
Int SoundManager::CreateSound(String &fileName) {
	printf("CreateSound success\n");
	return CreateSound(fileName, SOUND_TYPE_2D_SOUND);
} // SoundManager::CreateSound

////////////////////////////////////////////
// Designate a sound type of 2D looped sound to a given sound
////////////////////////////////////////////
Int SoundManager::CreateLoopedSound(String &fileName) {
	printf("CreateLoopedSound success\n");
	return CreateSound(fileName, SOUND_TYPE_2D_SOUND_LOOPED);
} // SoundManager::CreateLoopedSound

////////////////////////////////////////////
// The setup for using a sound
////////////////////////////////////////////
Int  SoundManager::CreateSound(String &fileName, SOUND_TYPE soundType) {
	SoundInstance *newSoundInstance;
	Int soundIndex;
	FMOD::Sound *sound;

	soundIndex = FindSound(fileName, soundType);
	if(soundIndex != INVALID_SOUND_INDEX)
		return soundIndex;

	IncrementNextSoundInstanceIndex();
	newSoundInstance = soundInstanceVector->at(nextSoundInstanceIndex);
	newSoundInstance->fileName = fileName;
	newSoundInstance->soundType = soundType;

	// Allows the soundType to be the deciding factor of whether the sound gets looped or doesn't
	switch(soundType) {
	case SOUND_TYPE_2D_SOUND:
		{
			result = fsystem->createStream((const Char *)newSoundInstance, FMOD_DEFAULT, 0, &newSoundInstance->fsound);
			printf("Sound created\n");
			break;

		}
	case SOUND_TYPE_2D_SOUND_LOOPED:
		{
			result = fsystem->createStream((const Char *)newSoundInstance, FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &newSoundInstance->fsound);
			printf("Looped sound created\n");
			break;
		}
	default:
		printf("Could not load sound '", fileName, "' - invalid sound type\n");
		return INVALID_SOUND_INDEX;
	} // end case

	// Check to make sure the fileName and soundType is alright
	ErrorCheck(result);

	if(result != FMOD_OK) {
		printf("Could not load sound '", fileName, "' \n");
		return INVALID_SOUND_INDEX;
	}

//	newSoundInstance->fsound = sound;

	printf("CreateSound success\n");
	return nextSoundInstanceIndex;
} // SoundManager::CreateSound

////////////////////////////////////////////
// Play a given sound
////////////////////////////////////////////
Void SoundManager::PlaySound(Int soundIndex, Int *channelIndex) {
	Bool isPlaying;
	Int channelIndexTemp;
	SoundInstance *soundInstance;

	if(soundIndex == INVALID_SOUND_INDEX)
		return;

	if(channelIndex)
		channelIndexTemp = *channelIndex;
	else
		channelIndexTemp = INVALID_SOUND_CHANNEL;

	assert((soundIndex > 0) && (soundIndex < (Int)soundInstanceVector->capacity()));

	if(channelIndexTemp != INVALID_SOUND_CHANNEL) {
		result = fsystem->getChannel(channelIndexTemp, &fchannel);

		if(result == FMOD_OK) {
			// Check the channel to see if the sound is already playing
			result = fchannel->isPlaying(&isPlaying);
			ErrorCheck(result);

			if(result == FMOD_OK && isPlaying == true)
				return; // This sound is clearly already playing
		}
	}

	soundInstance = soundInstanceVector->at(soundIndex);

	// Start the sound paused (the file is not started yet)
	result = fsystem->playSound(FMOD_CHANNEL_FREE, soundInstance->fsound, true, &fchannel);
	ErrorCheck(result);

	// If the sound can't play the sound, it becomes invalid
	if(result != FMOD_OK) {
		if(channelIndex)
			*channelIndex = INVALID_SOUND_CHANNEL;
		return;
	}

	fchannel->getIndex(&channelIndexTemp);

	// Make sure the volume is consistent unless changed otherwise
	result = fchannel->setVolume(1.0);
	ErrorCheck(result);

	// The sound can finally start
	result = fchannel->setPaused(false);
	ErrorCheck(result);

	if(channelIndex)
		*channelIndex = channelIndexTemp;

	printf("PlaySound success\n");
} // SoundManager::PlaySound

////////////////////////////////////////////
// Stop an active sound
////////////////////////////////////////////
Void SoundManager::StopSound(Int *channelIndex) {
	if(*channelIndex == INVALID_SOUND_CHANNEL)
		return;

	// Get the currently active channel
	fsystem->getChannel(*channelIndex, &fchannel);

	// Stops the active channel
	fchannel->stop();

	// Set the sound channel invalid
	*channelIndex = INVALID_SOUND_CHANNEL;

	printf("StopSound success\n");
} // SoundManager::StopSound

////////////////////////////////////////////
// Stop all active sounds
////////////////////////////////////////////
Void SoundManager::StopAllSounds() {
	Int channelIndex;
	FMOD::Channel *nextChannel;


	for(channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++) {
		// Get the currently active channel
		result = fsystem->getChannel(channelIndex, &nextChannel);

		if((result == FMOD_OK) && (nextChannel != NULL))
			// Stops the active channel
			nextChannel->stop();

		// Set the sound channel invalid
		channelIndex = INVALID_SOUND_CHANNEL;
	}

	printf("StopAllSounds success\n");
} // SoundManager::StopAllSounds

// Finds the sound index
Int SoundManager::FindSound(String &fileName, SOUND_TYPE soundType) {
	Int vectorIndex, vectorCapacity;
	SoundInstance *nextSoundInstance;

	vectorCapacity = (Int)soundInstanceVector->capacity();
	for(vectorIndex = 0; vectorIndex < vectorCapacity; vectorIndex++) {
		nextSoundInstance = soundInstanceVector->at(vectorIndex);
		if((soundType == nextSoundInstance->soundType) && (fileName == nextSoundInstance->fileName))
			return vectorIndex;
	}

	printf("FindSound success\n");
	return INVALID_SOUND_INDEX;
} // SoundManager::FindSound

////////////////////////////////////////////
// SoundManager can get the length of a sound
////////////////////////////////////////////
Float SoundManager::GetSoundLength(Int soundIndex) {
	uInt soundLength; // length in milliseconds
	SoundInstance *soundInstance;

	if(soundIndex == INVALID_SOUND_INDEX)
		return 0.0;

	assert((soundIndex > 0) && (soundIndex < (Int)soundInstanceVector->capacity()));

	soundInstance = soundInstanceVector->at(soundIndex);
	if(soundInstance) {
		result = soundInstance->fsound->getLength(&soundLength, FMOD_TIMEUNIT_MS);
		ErrorCheck(result);

		if(result != FMOD_OK)
			printf("GetSoundLength could not get length\n");

	} else {
		printf("GetSoundLength could not find soundInstance\n");
	}

	printf("GetSoundLength success\n");

	return (Float)soundLength / 1000.0f;
} // SoundManager::GetSoundLength

////////////////////////////////////////////
// Get the current sound instance 
////////////////////////////////////////////
SoundInstance *SoundManager::GetSoundInstance(Int soundIndex) {
	printf("GetSoundInstance success\n");
	return soundInstanceVector->at(soundIndex);
} // SoundManager::GetSoundInstance

////////////////////////////////////////////
// Deals with vectors to keep the the sound instance index incrementing
////////////////////////////////////////////
Void SoundManager::IncrementNextSoundInstanceIndex() {
	Int oldVectorCapacity;

	oldVectorCapacity = (Int)soundInstanceVector->capacity();
	nextSoundInstanceIndex += 1;
	if(nextSoundInstanceIndex < oldVectorCapacity)
		return;

	Int vectorIndex;
	SoundInstanceVector *newSoundInstanceVector;

	// Create a new and larger SoundInstanceVector
	newSoundInstanceVector = new SoundInstanceVector;
	newSoundInstanceVector->resize(oldVectorCapacity + INCREASE_VECTOR_SIZE);

	// Copy values from old vector to new
	for(vectorIndex = 0; vectorIndex < oldVectorCapacity; vectorIndex++) {
		newSoundInstanceVector->at(vectorIndex) = soundInstanceVector->at(vectorIndex);
	}

	Int newVectorCapacity;

	newVectorCapacity = (Int)newSoundInstanceVector->capacity();
	// Clear the rest of the new vector
	while(vectorIndex < newVectorCapacity) {
		newSoundInstanceVector->at(vectorIndex) = new SoundInstance;
		newSoundInstanceVector->at(vectorIndex)->Clear();
		vectorIndex++;
	}

	// Clear the old vector and point to the new
	soundInstanceVector->clear();
	delete(soundInstanceVector);
	soundInstanceVector = newSoundInstanceVector;

	printf("IncrementNextSoundInstanceIndex success\n");
} // SoundManager::IncrementNextSoundInstanceIndex

////////////////////////////////////////////
// Callback for opening
////////////////////////////////////////////
FMOD_RESULT SoundManager::fmodFileOpenCallback(const Char *fileName, Int unicode, uInt *filesize, Void **handle, Void **userdata) {
	SoundInstance *soundInstance;
	FileHandler fileHand;
	FileDescriptor fileDesc;
	FilePointer filePtr;

	// Evaluate the fileName, if it evals to 0, there is an assertion failure
	assert(fileName);

	// The fileName is a pointer to a SoundInstance, passed from CreateSound()
	soundInstance = (SoundInstance*)fileName;
	
	*handle = (Void*)soundInstance;
	*userdata = NULL;

	// FilePtr is storing the openfile info
	filePtr = fileHand.OpenFile_r(soundInstance->fileName);
	
	// The file descriptor stores the fileDesc that uses the filePtr
	fileDesc = fileDesc.GetFileDescriptor(filePtr);

	// The file descriptor is used to get the file length
	*filesize = fileHand.FileLength(fileDesc);

	printf("fmodFileOpenCallback success\n");

	return FMOD_OK;
} // SoundManager::fmodFileOpenCallback

////////////////////////////////////////////
// Callback for closing
////////////////////////////////////////////
FMOD_RESULT SoundManager::fmodFileCloseCallback(Void *handle, Void *userdata) {
	printf("fmodFileCloseCallback success\n");
	return FMOD_OK;
} // SoundManager::fmodFileCloseCallback

////////////////////////////////////////////
// Callback for reading
////////////////////////////////////////////
FMOD_RESULT SoundManager::fmodFileReadCallback(Void *handle, Void *buffer, uInt sizeBytes, uInt *bytesRead, Void *userData) {
	SoundInstance *soundInstance;
	FileHandler fileHand;
	FilePointer filePtrOpen;

	soundInstance = (SoundInstance*)handle;

	// Make sure the file is open
	filePtrOpen = fileHand.OpenFile_r(soundInstance->fileName);
	
	// Check the bytes read in the file
	*bytesRead = fileHand.ReadFile(buffer, sizeBytes, *bytesRead, filePtrOpen);

	// If we have zero bytes read, the file is ended
	if(*bytesRead == 0)
		return FMOD_ERR_FILE_EOF;

	printf("fmodFileReadCallback success\n");

	return FMOD_OK;
} // SoundManager::fmodFileReadCallback

////////////////////////////////////////////
// Callback for seeking
////////////////////////////////////////////
FMOD_RESULT SoundManager::fmodFileSeekCallback(Void *handle, uInt pos, Void *userdata) {
	SoundInstance *soundInstance;
	FileHandler fileHand;
	FilePointer filePtrOpen;
	Int fileSeek;

	soundInstance = (SoundInstance*)handle;

	// Make sure the file is open
	filePtrOpen = fileHand.OpenFile_r(soundInstance->fileName);
	// Must seek from the beginning
	fileSeek = fileHand.Seek(filePtrOpen, pos, 0);
	
	printf("fmodFileSeekCallback success\n");

	return FMOD_OK;
} // SoundManager::fmodFileSeekCallback

