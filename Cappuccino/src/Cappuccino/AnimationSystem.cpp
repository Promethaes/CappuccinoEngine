#include "Cappuccino/AnimationSystem.h"
#include "Cappuccino/CappMath.h"
#include "glm/common.hpp"
#include "glm/gtx/compatibility.hpp"

namespace Cappuccino {

	std::vector<Animation*> Animation::_allAnimations = {};
	Animation::Animation(const std::vector<Mesh*>& keyFrames, AnimationType type)
		: _type(type)
	{
		_keyFrames = keyFrames;
		_allAnimations.push_back(this);
	}
	float Animation::play(float dt)
	{
		if (_animationShader == nullptr) {
			printf("Animation shader not set! animations cannot be played!\n");
			return -1.0f;
		}
		_animationShader->use();

		if (!_shouldPlay)
			return -1.0f;


		if (t == 0.0f)
			_keyFrames[0]->animationFunction(*_keyFrames[index], _shouldPlay);


		t += dt * _speed;
		//_animationShader->setUniform("dt",t);

		if (t >= 1.0f) {
			t = 0.0f;

			_keyFrames[0]->_VBO = _keyFrames[index]->_VBO;
			index++;
			if (index > _keyFrames.size() - 1) {
				if (!_loop)
					_shouldPlay = false;
				index = 1;
				_keyFrames[0]->resetVertAttribPointers();

			}
		}
		return t;
	}

	float Animator::_dt = 0.0f;
	Animator::Animator()
	{
		for (unsigned i = 0; i < (int)AnimationType::NumTypes; i++)
			_animations.push_back(nullptr);
	}
	void Animator::update(float dt)
	{
		_dt = dt;
		for (auto x : _animations) {
			if (x != nullptr && x->_shouldPlay) {
				_currentT = x->play(dt);
				break;
			}
		}
	}
	void Animator::addAnimation(Animation* animation)
	{
		_animations[(int)animation->getAnimationType()] = animation;
	}
	void Animator::playAnimation(AnimationType type)
	{
		_animations[(int)type]->_shouldPlay = true;
		_playingAnimation = true;
	}
	bool Animator::isPlaying(AnimationType type)
	{
		return _animations[(int)type]->_shouldPlay;
	}
	void Animator::setAnimationShader(AnimationType type, Shader* shader)
	{
		_animations[(int)type]->_animationShader = shader;
	}
	void Animator::clearAnimation(AnimationType type)
	{
		delete _animations[(int)type];
		_animations[(int)type] = nullptr;
	}
	void Animator::setLoop(AnimationType type, bool yn)
	{
		_animations[(int)type]->setLoop(yn);
	}
	void Animator::setSpeed(AnimationType type, float speed)
	{
		_animations[(int)type]->setSpeed(speed);
	}
	bool Animator::animationExists(AnimationType type)
	{
		if (_animations[(int)type] != nullptr)
			return true;
		return false;
	}
}