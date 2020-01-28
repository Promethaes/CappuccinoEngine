#include "Cappuccino/AnimationSystem.h"
#include "Cappuccino/CappMath.h"

namespace Cappuccino {
	Animation::Animation(const std::vector<Mesh*>& morphTargets)
	{
		_morphTargets = morphTargets;

		_originalVerts = _morphTargets[0]->verts;
		_originalNorms = _morphTargets[0]->norms;
		_originalTangs = _morphTargets[0]->tangs;
	}
	void Animation::animate(float dt)
	{
		static bool stop = false;
		if (stop)
			return;
		t += dt;

		if (t >= 1.0f) {
			t = 0.0f;

			_originalVerts = _morphTargets[index]->verts;
			_originalNorms = _morphTargets[index]->norms;
			_originalTangs = _morphTargets[index]->tangs;

			index++;
			if (index > _morphTargets.size() - 1)
				stop = true;
		}
		else {
			std::vector<float> tempVerts;
			for (unsigned i = 0; i < _originalVerts.size(); i++) {
				tempVerts.push_back(Math::lerp(_originalVerts[i], _morphTargets[index]->verts[i], t));

			}
			std::vector<float> tempNorms;
			for (unsigned i = 0; i < _originalNorms.size(); i++) {
				tempNorms.push_back(Math::lerp(_originalNorms[i], _morphTargets[index]->norms[i], t));

			}
			std::vector<float> tempTangs;
			for (unsigned i = 0; i < _originalTangs.size(); i++) {
				tempTangs.push_back(Math::lerp(_originalTangs[i], _morphTargets[index]->tangs[i], t));

			}

			_morphTargets[0]->reload(tempVerts, tempNorms,tempTangs);
		}

	}
}