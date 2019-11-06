#include "Cappuccino/AnimationSystem.h"
#include "Cappuccino/CappMath.h"

namespace Cappuccino {
	Animation::Animation(std::vector<Mesh*>& morphTargets)
	{
		_morphTargets = morphTargets;
	}
	void Animation::animate(float dt)
	{
		t += dt;

		if (t >= 1.0f) {
			t = 0.0f;
			index++;
		}
		//code

		std::vector<float> tempVerts;
		for (unsigned i = 0; i < _morphTargets[0]->verts.size(); i++) {
			tempVerts[i] = Math::lerp(_morphTargets[0]->verts[i], _morphTargets[1]->verts[i], t);

		}
		std::vector<float> tempTexts;
		for (unsigned i = 0; i < _morphTargets[0]->texts.size(); i++) {
			tempTexts[i] = Math::lerp(_morphTargets[0]->texts[i], _morphTargets[1]->texts[i], t);

		}
		std::vector<float> tempNorms;
		for (unsigned i = 0; i < _morphTargets[0]->norms.size(); i++) {
			tempTexts[i] = Math::lerp(_morphTargets[0]->norms[i], _morphTargets[1]->norms[i], t);

		}
		_morphTargets[0]->reload(tempVerts, tempTexts, tempNorms);

	}
}