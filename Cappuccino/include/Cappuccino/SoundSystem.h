#pragma once
#include <string>
#include <vector>

#include "glm/common.hpp"

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

//studio
#include "FmodStudio/fmod_studio.hpp"
#include "FmodStudio/fmod_studio_common.h"

#include <optional>

namespace Cappuccino {

	FMOD_VECTOR& glmToFmod(glm::vec3& v);

	class SoundSystem {
	public:

		/*
		Purp: initialize the sound system
		Req: default path for sound files eg. Assets/Sounds/
		*/
		static void init(const std::string& defaultFilePath, unsigned numChannels = 512);

		/*
		Purp: load a 2D sound into memory
		Req: relative file path to the sound
		Returns: index at which the sound is created
		*/
		static unsigned load2DSound(const std::string& relativeFilePath);
		/*
		Purp: load a 3D sound into memory
		Req: relative filepath to the sound
		Returns: index at with sound is stored
		*/
		static unsigned load3DSound(const std::string& relativeFilePath);
		static void setListenerAttributes(int listener, glm::vec3& pos, glm::vec3& vel, glm::vec3& forward = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));


		/*
		Purp: create a new channel group
		Req: name of the group you want to create
		Returns: index at which the group was created
		*/
		static unsigned createChannelGroup(const std::string& groupName);

		/*
		Purp: play a 2D sound at the provided indicies
		Req: index to the proper sounds and group
		*/
		static unsigned playSound2D(unsigned soundsIndex, unsigned groupsIndex);

		//play a 3D sound at index
		static void playSound3D(unsigned soundsIndex, unsigned groupsIndex);
		
		static void setDefaultPath(const std::string& defaultFilePath) { _soundPath = defaultFilePath; }

		static void update();

		static std::vector<FMOD::Channel*>& getChannels();
		static std::vector<FMOD::Sound*>& getSounds();

		/*
		Purp: check Fmod Errors
		Req: FMOD_RESULT, and a string describing what function crashed the program
		*/
		static void checkFmodErrors(FMOD_RESULT& result, const std::string& whereError);
		
		static std::vector<FMOD::Sound*> _sounds;
		static std::vector<FMOD::ChannelGroup*>_channelGroups;
		static std::vector<FMOD::Channel*>_channels;
		static FMOD::System* _system;
	private:

		static std::string _soundPath;
		static bool _initialized;
		static FMOD_RESULT _result;
	};

	/*
	Purp: make the handling of sounds cleaner, more organized
	Req: sound and group handles
	*/
	class Sound {
	public:
		Sound() = default;
		Sound(const std::string& PATH, const std::optional<std::string>& createGroup = std::nullopt);
		Sound(unsigned soundHandle, unsigned groupHandle);

		//default play function, uses default sound manager play
		void play();

		void setSoundHandle(unsigned soundHandle) { _sound = soundHandle; }
		void setGroupHandle(unsigned groupHandle) { _group = groupHandle; }

		unsigned getSoundHandle() const { return _sound; }
		unsigned getGroupHandle() const { return _group; }
	private:
		unsigned _sound = 0, _group = 0,_channel = 0;
		
	};

	class Sound3D {
	public:
		Sound3D() = default;
		Sound3D(const std::string& PATH, const std::optional<std::string>& createGroup = std::nullopt);

		//default play function, uses default sound manager play
		void play();

		void SetChannelPosition(glm::vec3& pos);

		void setSoundHandle(unsigned soundHandle) { _sound = soundHandle; }
		void setGroupHandle(unsigned groupHandle) { _group = groupHandle; }

		unsigned getSoundHandle() const { return _sound; }
		unsigned getGroupHandle() const { return _group; }
	private:
		glm::vec3 _position;
		unsigned _sound = 0, _group = 0;
	
	};

	//High level FMOD,inspiration from gamesound goop
	class StudioSound {
	public:
		static void init(const std::string& defaultFilePath);
		static void checkFmodErrors(FMOD_RESULT& result, const std::string& whereError);
		static void update();

		static unsigned loadBank(const std::string& relativeFilePath, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		static unsigned loadEvent(const std::string& eventID);

		static void playEvent(unsigned handle);
		static void stopEvent(unsigned handle, bool stopInstant = false);
		static void setEventParameter(unsigned handle, const std::string& paramName, float value);

		static std::vector<FMOD::Studio::Bank*> _banks;
		static std::vector<FMOD::Studio::EventInstance*> _events;
	private:
		static bool _initialized;
		static FMOD_RESULT _result;
		static std::string _soundPath;
		static FMOD::Studio::System* _system;
	};

	//uses Fmod studio API, uses a bank
	class SoundBank {
	public:
		SoundBank() = default;
		SoundBank(const std::string& path, FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMOD_STUDIO_LOAD_BANK_NORMAL);

		//event ID is the horrible garbage string that FMOD studio spits out 
		void addEvent(const std::string& eventID);
		//play the event at index
		void playEvent(unsigned index);

		//stops event at index
		void stopEvent(unsigned index, bool instaStop = false);

	private:
		unsigned _bank = 0;
		std::vector<unsigned> _events;
	};
}