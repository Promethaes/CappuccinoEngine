#include "Cappuccino/AnimationSystem.h"
#include "Cappuccino/CappMath.h"
#include "glm/common.hpp"
#include "glm/gtx/compatibility.hpp"

namespace Cappuccino {
	Animation::Animation(const std::vector<Mesh*>& keyFrames, AnimationType type)
		: _originalMesh({}, {}, {}, {}), _type(type)
	{
		_keyFrames = keyFrames;

		_currentVerts = _keyFrames[0]->verts;
		_currentNorms = _keyFrames[0]->norms;
		_currentTangs = _keyFrames[0]->tangs;

		_originalMesh.verts = _currentVerts;
		_originalMesh.norms = _currentNorms;
		_originalMesh.tangs = _currentTangs;
		_originalMesh.texts = _keyFrames[0]->texts;
	}
	void Animation::play(float dt)
	{
		if (!_shouldPlay)
			return;

		static bool stop = false;
		if (stop)
			return;
		t += dt;

		if (t >= 1.0f) {
			t = 0.0f;

			_currentVerts = _keyFrames[index]->verts;
			_currentNorms = _keyFrames[index]->norms;
			_currentTangs = _keyFrames[index]->tangs;

			index++;
			if (index > _keyFrames.size() - 1) {
				if (_loop) {
					_currentVerts = _originalMesh.verts;
					_currentNorms = _originalMesh.norms;
					_currentTangs = _originalMesh.tangs;
					index = 1;
				}
				else
					stop = true;
			}
		}

		else {
			std::vector<float> tempVerts;
			for (unsigned i = 0; i < _currentVerts.size(); i += 3) {
				auto e = (glm::lerp(glm::vec3(_currentVerts[i], _currentVerts[i + 1], _currentVerts[i + 2]), glm::vec3(_keyFrames[index]->verts[i], _keyFrames[index]->verts[i + 1], _keyFrames[index]->verts[i + 2]), t));
				tempVerts.push_back(e.x);
				tempVerts.push_back(e.y);
				tempVerts.push_back(e.z);

			}
			std::vector<float> tempNorms;
			for (unsigned i = 0; i < _currentNorms.size(); i += 3) {
				auto e = (glm::lerp(glm::vec3(_currentNorms[i], _currentNorms[i + 1], _currentNorms[i + 2]), glm::vec3(_keyFrames[index]->norms[i], _keyFrames[index]->norms[i + 1], _keyFrames[index]->norms[i + 2]), t));
				tempNorms.push_back(e.x);
				tempNorms.push_back(e.y);
				tempNorms.push_back(e.z);

			}
			std::vector<float> tempTangs;
			for (unsigned i = 0; i < _currentTangs.size(); i += 3) {
				auto e = (glm::lerp(glm::vec3(_currentTangs[i], _currentTangs[i + 1], _currentTangs[i + 2]), glm::vec3(_keyFrames[index]->tangs[i], _keyFrames[index]->tangs[i + 1], _keyFrames[index]->tangs[i + 2]), t));
				tempTangs.push_back(e.x);
				tempTangs.push_back(e.y);
				tempTangs.push_back(e.z);

			}

			_keyFrames[0]->reload(tempVerts, tempNorms, tempTangs);
		}

	}
	Mesh& Animation::getOriginalMesh()
	{
		return _originalMesh;
	}
	Animator::Animator()
	{
		for (unsigned i = 0; i < 7; i++)
			_animations.push_back(nullptr);
	}
	void Animator::addAnimation(Animation* animation)
	{
		_animations[(int)animation->getAnimationType()] = animation;
	}
	void Animator::playAnimation(AnimationType type, float dt)
	{
		_animations[(int)type]->play(dt);
		_playingAnimation = true;
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
}