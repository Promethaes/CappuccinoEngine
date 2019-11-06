#include "Cappuccino/AnimationSystem.h"
#include "Cappuccino/CappMath.h"

namespace Cappuccino {
	Animation::Animation(std::vector<Mesh*>& morphTargets)
	{
		_morphTargets = morphTargets;

		_originalVerts = _morphTargets[0]->verts;
		_originalTexts = _morphTargets[0]->texts;
		_originalNorms = _morphTargets[0]->norms;
	}
	void Animation::animate(float dt)
	{
		t += dt;

		if (t >= 1.0f) {
			t = 0.0f;
			_originalVerts = _morphTargets[index]->verts;
			_originalTexts = _morphTargets[index]->texts;
			_originalNorms = _morphTargets[index]->norms;
			index++;
			if (index > _morphTargets.size() - 1)
				index = 0;
		}
		else {
			std::vector<float> tempVerts;
			for (unsigned i = 0; i < _originalVerts.size(); i++) {
				tempVerts.push_back(Math::lerp(_originalVerts[i], _morphTargets[index]->verts[i], t));

			}
			std::vector<float> tempTexts;
			for (unsigned i = 0; i < _originalTexts.size(); i++) {
				tempTexts.push_back(Math::lerp(_originalTexts[i], _morphTargets[index]->texts[i], t));

			}
			std::vector<float> tempNorms;
			for (unsigned i = 0; i < _originalNorms.size(); i++) {
				tempTexts.push_back(Math::lerp(_originalNorms[i], _morphTargets[index]->norms[i], t));

			}
			_morphTargets[index - 1]->reload(tempVerts, tempTexts, tempNorms);
		}

	}
}