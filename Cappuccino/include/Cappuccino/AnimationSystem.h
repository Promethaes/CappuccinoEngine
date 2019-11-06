#pragma once
#include "Cappuccino/Mesh.h"
#include <vector>
namespace Cappuccino {
	
	class Animation {
	public:
		//should never have meshes that are different in this list
		Animation(std::vector<Mesh*>& morphTargets);

		void animate(float dt);

	private:
		unsigned index = 0;
		float t = 0.0f;
		std::vector<Mesh*> _morphTargets;
	};
}