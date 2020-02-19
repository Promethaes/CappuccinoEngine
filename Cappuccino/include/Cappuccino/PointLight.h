#pragma once
#include <vector>
#include "glm/common.hpp"

namespace Cappuccino {
	/*
	Purp: a point light. be sure to update the view pos every frame
	and also, be sure to load the view matrix every frame (both externally)
	*/
	class PointLight {
	public:
		PointLight(const glm::vec3& position, const glm::vec3& colour);

		glm::vec3 _pos;
		glm::vec3 _col;
		bool _isActive = true;
	};
}