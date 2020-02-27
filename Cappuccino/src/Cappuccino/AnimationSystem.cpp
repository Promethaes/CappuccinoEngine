#include "Cappuccino/AnimationSystem.h"
#include "Cappuccino/CappMath.h"
#include "glm/common.hpp"
#include "glm/gtx/compatibility.hpp"

namespace Cappuccino {
	Shader* Animator::_animationShader = nullptr;
	Animation::Animation(const std::vector<Mesh*>& keyFrames, AnimationType type)
		: _originalMesh({}, {}, {}, {}), _type(type)
	{
		_keyFrames = keyFrames;


		_originalMesh.verts = _currentVerts;
		_originalMesh.norms = _currentNorms;
		_originalMesh.tangs = _currentTangs;
		_originalMesh.texts = _keyFrames[0]->texts;
	}
	void Animation::play(float dt)
	{
		if (Animator::_animationShader == nullptr) {
			printf("Animation shader not set! animations cannot be played!\n");
			return;
		}
		Animator::_animationShader->use();
		Animator::_animationShader->setUniform("shouldPlay",_shouldPlay);

		if (!_shouldPlay)
			return;

		if (t == 0.0f) {

			_keyFrames[0]->animationFunction(*_keyFrames[index]);
		}

		t += dt * _speed;
		Animator::_animationShader->setUniform("dt",t);

		if (t >= 1.0f) {
			t = 0.0f;
			
			_keyFrames[0]->_VBO = _keyFrames[index]->_VBO;
			index++;
			if (index > _keyFrames.size() - 1) {
				if (!_loop)
					_shouldPlay = false;
				index = 1;
			}
		}
	}
	
	Mesh& Animation::getOriginalMesh()
	{
		return _originalMesh;
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
			if (x != nullptr && x->_shouldPlay)
				x->play(dt);
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
}