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

	void SoundSystem::init(const std::string& defaultFilePath)
	{
		//i followed some documentation https://www.fmod.com/resources/documentation-api?version=2.0&page=white-papers-getting-started.html
		if (!_initialized) {
			_soundPath = defaultFilePath;

			_result = FMOD::System_Create(&_system);
			checkFmodErrors(_result, "System creation");

			_result = _system->init(512, FMOD_INIT_NORMAL, 0);
			checkFmodErrors(_result, "System initialization");
			_initialized = true;

			//channel 0 is sfx
			FMOD::Channel* channel = NULL;
			FMOD::Channel* channel2 = NULL;
			FMOD::Channel* channel3 = NULL;

			_channels.push_back(channel);
			_channels.push_back(channel2);
			_channels.push_back(channel3);
		}
	}

	unsigned SoundSystem::load2DSound(const std::string& relativeFilePath)
	{
		FMOD::Sound* sound = NULL;
		_result = _system->createSound(std::string(_soundPath + relativeFilePath).c_str(), FMOD_2D, nullptr, &sound);
		checkFmodErrors(_result, "Sound creation");

		_sounds.push_back(sound);

		return _sounds.size() - 1;
	}

	unsigned SoundSystem::createChannelGroup(const std::string& groupName)
	{
		FMOD::ChannelGroup* group = NULL;
		_result = _system->createChannelGroup(groupName.c_str(), &group);
		checkFmodErrors(_result, "Group creation");

		_channelGroups.push_back(group);

		return _channelGroups.size() - 1;
	}

	void SoundSystem::playSound2D(unsigned soundsIndex, unsigned groupsIndex, ChannelType type)
	{
		_result = _system->playSound(_sounds[soundsIndex], _channelGroups[groupsIndex], false, &_channels[(int)type]);
		checkFmodErrors(_result, "Play sound 2D");
	}

	void SoundSystem::update()
	{
		_system->update();
	}

	std::vector<FMOD::Channel*>& SoundSystem::getChannels()
	{
		return _channels;
	}

	std::vector<FMOD::Sound*>& SoundSystem::getSounds()
	{
		return _sounds;
	}

	void SoundSystem::checkFmodErrors(FMOD_RESULT& result, const std::string& whereError)
	{
		if (result != FMOD_OK)
		{
			CAPP_PRINT_ERROR("FMOD error! (%d) %s at %s\n", result, FMOD_ErrorString(result), whereError.c_str());
			SYS_EXIT(-1);
		}
	}
	Sound::Sound(const std::string& PATH, const std::optional<std::string>& createGroup, SoundSystem::ChannelType type)
	{
		_type = type;
		_sound = SoundSystem::load2DSound(PATH);
		if (createGroup.has_value())
			_group = SoundSystem::createChannelGroup(createGroup.value());
	}
	Sound::Sound(unsigned soundHandle, unsigned groupHandle)
		:_sound(soundHandle),_group(groupHandle)
	{
	}
	void Sound::play()
	{
		SoundSystem::playSound2D(_sound, _group, _type);
	}
}