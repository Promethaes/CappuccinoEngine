#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/CappMacros.h"
namespace Cappuccino {
	bool SoundSystem::_initialized = false;
	std::vector<FMOD::Sound*> SoundSystem::_sounds = {};
	std::vector<FMOD::ChannelGroup*> SoundSystem::_channelGroups = {};
	std::vector<FMOD::Channel*> SoundSystem::_channels = {};
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

			//channel 0 is sfx
			FMOD::Channel* channel = NULL;

			_channels.push_back(channel);
		}
	}

	unsigned Cappuccino::SoundSystem::load2DSound(const std::string& relativeFilePath)
	{
		FMOD::Sound* sound = NULL;
		_result = _system->createSound(std::string(_soundPath + relativeFilePath).c_str(), FMOD_2D, nullptr, &sound);
		checkFmodErrors(_result, "sound creation");

		_sounds.push_back(sound);

		return _sounds.size() - 1;
	}

	unsigned SoundSystem::createChannelGroup(const std::string& groupName)
	{
		FMOD::ChannelGroup* group = NULL;
		_result = _system->createChannelGroup(groupName.c_str(), &group);
		checkFmodErrors(_result, "group creation");

		_channelGroups.push_back(group);

		return _channelGroups.size() - 1;
	}

	void SoundSystem::playSound2D(unsigned soundsIndex, unsigned groupsIndex, ChannelType type)
	{
		_result = _system->playSound(_sounds[soundsIndex], _channelGroups[groupsIndex], false, &_channels[(int)type]);
		checkFmodErrors(_result, "play sound 2D");
	}

	void SoundSystem::update()
	{
		_system->update();
	}

	void SoundSystem::checkFmodErrors(FMOD_RESULT& result, const std::string& whereError)
	{
		if (result != FMOD_OK)
		{
			CAPP_PRINT_ERROR("FMOD error! (%d) %s at %s\n", result, FMOD_ErrorString(result), whereError.c_str());
			SYS_EXIT(-1);
		}
	}
}