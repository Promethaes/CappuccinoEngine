#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/CappMacros.h"
namespace Cappuccino {

	bool SoundSystem::_initialized = false;
	std::vector<FMOD::Sound*> SoundSystem::sounds = {};
	std::vector<FMOD::ChannelGroup*> SoundSystem::channelGroups = {};
	std::vector<FMOD::Channel*> SoundSystem::channels = {};
	FMOD::System* SoundSystem::_system = NULL;
	FMOD_RESULT SoundSystem::_result = FMOD_OK;
	std::string SoundSystem::_soundPath = "";


	void Cappuccino::SoundSystem::init(const std::string& defaultFilePath)
	{
		//i followed some documentation https://www.fmod.com/resources/documentation-api?version=2.0&page=white-papers-getting-started.html
		if (!_initialized) {
			_soundPath = defaultFilePath;

			_result = FMOD::System_Create(&_system);
			checkFmodErrors(_result, "system creation");

			_result = _system->init(512, FMOD_INIT_NORMAL, 0);
			checkFmodErrors(_result, "system initialization");
			_initialized = true;
		}
	}

	unsigned Cappuccino::SoundSystem::load2DSound(const std::string& relativeFilePath)
	{
		FMOD::Sound* sound = NULL;
		_result = _system->createSound(std::string(_soundPath + relativeFilePath).c_str(), FMOD_2D, nullptr, &sound);
		checkFmodErrors(_result, "sound creation");

		sounds.push_back(sound);

		return sounds.size() - 1;
	}

	unsigned SoundSystem::createChannelGroup(const std::string& groupName)
	{
		FMOD::ChannelGroup* group = NULL;
		_result = _system->createChannelGroup(groupName.c_str(), &group);
		checkFmodErrors(_result, "group creation");

		channelGroups.push_back(group);

		return channelGroups.size() - 1;

	}

	void SoundSystem::playSound2D(unsigned soundsIndex,unsigned groupsIndex)
	{
		FMOD::Channel* channel = NULL;
		_result = _system->playSound(sounds[soundsIndex], channelGroups[groupsIndex], false, &channel);
		checkFmodErrors(_result, "play sound 2D");

		channels.push_back(channel);

	}

	void SoundSystem::checkFmodErrors(FMOD_RESULT& result, const std::string& whereError)
	{
		if (result != FMOD_OK)
		{
			CAPP_PRINT_ERROR("FMOD error! (%d) %s at %s\n", result, FMOD_ErrorString(result),whereError.c_str());
			SYS_EXIT(-1);
		}

	}

}