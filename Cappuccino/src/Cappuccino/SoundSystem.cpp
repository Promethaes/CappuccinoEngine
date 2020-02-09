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

	unsigned SoundSystem::load3DSound(const std::string& relativeFilePath)
	{
		FMOD::Sound* sound = NULL;
		_result = _system->createSound(std::string(_soundPath + relativeFilePath).c_str(), FMOD_3D, nullptr, &sound);
		checkFmodErrors(_result, "Sound creation");

		_sounds.push_back(sound);

		return _sounds.size() - 1;
	}

	void SoundSystem::setListenerAttributes(int listener, glm::vec3& pos, glm::vec3& vel, glm::vec3& forward, glm::vec3& up)
	{
		FMOD_VECTOR p, v, f, u;
		p.x = pos.x; p.y = pos.y; p.z = pos.z;
		v.x = vel.x; v.y = vel.y; v.z = vel.z;
		f.x = forward.x; f.y = forward.y; f.z = forward.z;
		u.x = up.x; u.y = up.y; u.z = up.z;
		_system->set3DListenerAttributes(listener, &p, &v, &f, &u);
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

	void SoundSystem::playSound3D(unsigned soundsIndex, unsigned groupsIndex, ChannelType type)
	{
		_result = _system->playSound(_sounds[soundsIndex], _channelGroups[groupsIndex],false, &_channels[(int)type]);
		checkFmodErrors(_result, "Play sound 3D");
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
		:_sound(soundHandle), _group(groupHandle)
	{
	}
	void Sound::play()
	{
		SoundSystem::playSound2D(_sound, _group, _type);
	}

	//studio sound
	bool StudioSound::_initialized = false;
	FMOD_RESULT StudioSound::_result = FMOD_OK;
	std::string StudioSound::_soundPath = "";
	FMOD::Studio::System* StudioSound::_system = nullptr;
	std::vector<FMOD::Studio::Bank*> StudioSound::_banks = {};
	std::vector<FMOD::Studio::EventInstance*> StudioSound::_events = {};
	void StudioSound::init(const std::string& defaultFilePath)
	{
		if (!_initialized) {
			_soundPath = defaultFilePath;

			_result = FMOD::Studio::System::create(&_system);
			checkFmodErrors(_result, "System creation");
			_result = _system->initialize(512, FMOD_STUDIO_INIT_NORMAL, 0, 0);

			checkFmodErrors(_result, "System initialization");
			_initialized = true;

		}
	}
	void StudioSound::checkFmodErrors(FMOD_RESULT& result, const std::string& whereError)
	{
		SoundSystem::checkFmodErrors(result, whereError);
	}
	void StudioSound::update()
	{
		_system->update();
	}
	unsigned StudioSound::loadBank(const std::string& relativeFilePath, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
	{
		FMOD::Studio::Bank* bank = NULL;
		_result = _system->loadBankFile((_soundPath + relativeFilePath).c_str(), flags, &bank);
		checkFmodErrors(_result, "loading bank file");
		_banks.push_back(bank);
		return _banks.size() - 1;
	}
	unsigned StudioSound::loadEvent(const std::string& eventID)
	{
		FMOD::Studio::EventDescription* desc = NULL;
		_result = _system->getEvent(eventID.c_str(), &desc);
		checkFmodErrors(_result, "checking event decription");

		FMOD::Studio::EventInstance* instance = NULL;
		_result = desc->createInstance(&instance);
		checkFmodErrors(_result, "creating event instance");

		_events.push_back(instance);
		return _events.size() - 1;
	}
	void StudioSound::playEvent(unsigned handle)
	{
		_events[handle]->start();
	}
	void StudioSound::stopEvent(unsigned handle, bool stopInstant)
	{
		FMOD_STUDIO_STOP_MODE mode;
		mode = stopInstant ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
		_result = _events[handle]->stop(mode);
		checkFmodErrors(_result, "stopping a sound");
	}
	void StudioSound::setEventParameter(unsigned handle, const std::string& paramName, float value)
	{
		_result = _events[handle]->setParameterByName(paramName.c_str(), value);
		checkFmodErrors(_result, "setting parameter " + paramName);
	}
	SoundBank::SoundBank(const std::string& path, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
	{
		_bank = StudioSound::loadBank(path, flags);
	}
	void SoundBank::addEvent(const std::string& eventID)
	{
		_events.push_back(StudioSound::loadEvent(eventID));
	}
	void SoundBank::playEvent(unsigned index)
	{
		StudioSound::playEvent(_events[index]);
	}
	void SoundBank::stopEvent(unsigned index, bool instaStop)
	{
		StudioSound::stopEvent(_events[index], instaStop);
	}
	Sound3D::Sound3D(const std::string& PATH, const std::optional<std::string>& createGroup, SoundSystem::ChannelType type)
		:_type(type)
	{
		_type = type;
		_sound = SoundSystem::load3DSound(PATH);
		if (createGroup.has_value())
			_group = SoundSystem::createChannelGroup(createGroup.value());
	}
	void Sound3D::play()
	{
		SoundSystem::playSound3D(_sound, _group, SoundSystem::ChannelType::SoundEffect);
	}
	void Sound3D::SetChannelPosition(glm::vec3& pos)
	{
		FMOD_VECTOR p = glmToFmod(pos);
		SoundSystem::_channels[(int)_type]->set3DAttributes(&p, NULL);
	}
	FMOD_VECTOR& glmToFmod(glm::vec3& v)
	{
		FMOD_VECTOR nV{ v.x,v.y,v.z };
		return nV;
	}
}