#pragma once
#include "Cappuccino/Mesh.h"
#include <vector>
enum class AnimationType {
	Idle = 0,
	Attack,
	Run,
	Crouch,
	Jump,
	Death,
	Walk,
	Dance
};
namespace Cappuccino {

	class Animation {
	public:
		//a list of key frames and the type of animation is required
		Animation(const std::vector<Mesh*>& keyFrames, AnimationType type);

		void setLoop(bool yn) { _loop = yn; }
		void play(float dt);
		Mesh& getOriginalMesh();

		AnimationType getAnimationType() { return _type; }

	private:
		bool _loop = false;
		AnimationType _type;
		int index = 1;
		float t = 0.0f;
		std::vector<Mesh*> _keyFrames;
		std::vector<float> _currentVerts;
		std::vector<float> _currentTangs;
		std::vector<float> _currentNorms;

		Mesh _originalMesh;
	};
	class Animator {
	public:
		Animator();

		/*
		Purp: add or change an animation to the list AT THE INDEX ACCORDING TO THE ANIMATION TYPE

		eg. addAnimation(Animation(...,AnimationType::Idle) will place the element at 0

		whereas AnimationType::Attack will place the element at 1 and so on

		Req: an animation
		*/
		void addAnimation(Animation* animation);

		void playAnimation(AnimationType type, float dt);

		/*
		Purp: deletes an animation at the type given
		Req: type as an index
		*/
		void clearAnimation(AnimationType type);

		//set the animation at the index to loop
		void setLoop(AnimationType type,bool yn);

	private:
		std::vector<Animation*> _animations;
	};
}