#pragma once
#include <string>
#include <vector>

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

namespace Cappuccino {
	class SoundSystem {
	public:

		enum class ChannelType {
			SoundEffect = 0,
			Music
		};

		/*
		Purp: initialize the sound system
		Req: default path for sound files eg. Assets/Sounds/
		*/
		static void init(const std::string& defaultFilePath);

		/*
		Purp: load a 2D sound into memory
		Req: relative file path to the sound
		Returns: index at which the sound is created
		*/
		static unsigned load2DSound(const std::string& relativeFilePath);

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
		static void playSound2D(unsigned soundsIndex, unsigned groupsIndex,ChannelType type);

		static void setDefaultPath(const std::string& defaultFilePath) { _soundPath = defaultFilePath; }

		static void update();
	private:

		/*
		Purp: check Fmod Errors
		Req: FMOD_RESULT, and a string describing what function crashed the program
		*/
		static void checkFmodErrors(FMOD_RESULT& result, const std::string& whereError);

		static std::string _soundPath;
		static bool _initialized;
		static std::vector<FMOD::Sound*> _sounds;
		static std::vector<FMOD::ChannelGroup*>_channelGroups;
		static std::vector<FMOD::Channel*>_channels;
		static FMOD::System* _system;
		static FMOD_RESULT _result;
	};
}