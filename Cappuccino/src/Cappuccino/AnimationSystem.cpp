#include "Cappuccino/AnimationSystem.h" 
#include "Cappuccino/CappMath.h" 
#include "glm/common.hpp" 
#include "glm/gtx/compatibility.hpp" 
#include "Cappuccino/ResourceManager.h" 

using namespace Cappuccino;

Animation::Animation(const std::vector<MeshProperties>& keyFrames, AnimationType type) :
	_type(type), _originalMesh({}, {}, {}, {}) {
	_keyFrames = keyFrames;
	auto firstFrame = MeshLibrary::getMesh(keyFrames[0].name);

	_currentVerts = firstFrame->verts;
	_currentNorms = firstFrame->norms;
	_currentTangs = firstFrame->tangs;

	_originalMesh.verts = _currentVerts;
	_originalMesh.norms = _currentNorms;
	_originalMesh.tangs = _currentTangs;
	_originalMesh.texts = firstFrame->texts;
}

void Animation::play(float dt) {
	const auto keyFrame = MeshLibrary::getMesh(_keyFrames[index].name);

	if(!_shouldPlay)
		return;

	static bool stop = false;
	if(stop)
		return;
	t += dt * _speed;

	if(t >= 1.0f) {
		t = 0.0f;


		_currentVerts = keyFrame->verts;
		_currentNorms = keyFrame->norms;
		_currentTangs = keyFrame->tangs;

		index++;
		if(index > (int)_keyFrames.size() - 1) {
			if(_loop) {
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
		for(unsigned i = 0; i < _currentVerts.size(); i += 3) {
			auto e = (glm::slerp(glm::quat(1.0f, _currentVerts[i], _currentVerts[i + 1], _currentVerts[i + 2]), glm::quat(1.0f, keyFrame->verts[i], keyFrame->verts[i + 1], keyFrame->verts[i + 2]), t));
			tempVerts.push_back(e.x);
			tempVerts.push_back(e.y);
			tempVerts.push_back(e.z);

		}
		std::vector<float> tempNorms;
		for(unsigned i = 0; i < _currentNorms.size(); i += 3) {
			auto e = (glm::slerp(glm::quat(1.0f, _currentNorms[i], _currentNorms[i + 1], _currentNorms[i + 2]), glm::quat(1.0f, keyFrame->norms[i], keyFrame->norms[i + 1], keyFrame->norms[i + 2]), t));
			tempNorms.push_back(e.x);
			tempNorms.push_back(e.y);
			tempNorms.push_back(e.z);

		}
		std::vector<float> tempTangs;
		for(unsigned i = 0; i < _currentTangs.size(); i += 3) {
			auto e = (glm::slerp(glm::quat(1.0f, _currentTangs[i], _currentTangs[i + 1], _currentTangs[i + 2]), glm::quat(1.0f, keyFrame->tangs[i], keyFrame->tangs[i + 1], keyFrame->tangs[i + 2]), t));
			tempTangs.push_back(e.x);
			tempTangs.push_back(e.y);
			tempTangs.push_back(e.z);

		}

		auto newFrame = MeshLibrary::getMesh(_keyFrames[0].name);
		newFrame->reload(tempVerts, tempNorms, tempTangs);
	}

}
Mesh& Animation::getOriginalMesh() {
	return _originalMesh;
}
Animator::Animator() {
	for(unsigned i = 0; i < (int)AnimationType::NumTypes; i++)
		_animations.push_back(nullptr);
}
void Animator::addAnimation(Animation* animation) {
	_animations[(int)animation->getAnimationType()] = animation;
}
void Animator::playAnimation(AnimationType type, float dt) {
	_animations[(int)type]->play(dt);
	_playingAnimation = true;
}
void Animator::clearAnimation(AnimationType type) {
	delete _animations[(int)type];
	_animations[(int)type] = nullptr;
}
void Animator::setLoop(AnimationType type, bool yn) {
	_animations[(int)type]->setLoop(yn);
}
void Animator::setSpeed(AnimationType type, float speed) {
	_animations[(int)type]->setSpeed(speed);
}
