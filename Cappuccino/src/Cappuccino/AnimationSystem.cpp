#include "Cappuccino/AnimationSystem.h"
#include "Cappuccino/CappMath.h"

namespace Cappuccino {
	Animation::Animation(const std::vector<Mesh*>& keyFrames,AnimationType type)
		: _originalMesh({}, {}, {}, {}),_type(type)
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
			if (index > _keyFrames.size() - 1)
				stop = true;
		}
		else {
			std::vector<float> tempVerts;
			for (unsigned i = 0; i < _currentVerts.size(); i++) {
				tempVerts.push_back(Math::lerp(_currentVerts[i], _keyFrames[index]->verts[i], t));

			}
			std::vector<float> tempNorms;
			for (unsigned i = 0; i < _currentNorms.size(); i++) {
				tempNorms.push_back(Math::lerp(_currentNorms[i], _keyFrames[index]->norms[i], t));

			}
			std::vector<float> tempTangs;
			for (unsigned i = 0; i < _currentTangs.size(); i++) {
				tempTangs.push_back(Math::lerp(_currentTangs[i], _keyFrames[index]->tangs[i], t));

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
	void Animator::addAnimation(Animation& animation)
	{
		_animations[(int)animation.getAnimationType()] = &animation;
	}
	void Animator::playAnimation(AnimationType type,float dt)
	{
		_animations[(int)type]->play(dt);
	}
	void Animator::clearAnimation(AnimationType type)
	{
		delete _animations[(int)type];
		_animations[(int)type] = nullptr;
	}
}