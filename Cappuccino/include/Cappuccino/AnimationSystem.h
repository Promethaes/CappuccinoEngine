#pragma once
#include "Cappuccino/Mesh.h"
#include <vector>
namespace Cappuccino {
	
	class Animation {
	public:
		//should never have meshes that are different in this list
		Animation(const std::vector<Mesh*>& morphTargets);

		void animate(float dt);

	private:
		int index = 1;
		float t = 0.0f;
		std::vector<Mesh*> _morphTargets;
		std::vector<float> _originalVerts;
		std::vector<float> _originalTexts;
		std::vector<float> _originalNorms;
	};
}