#pragma once
#include "glm/common.hpp"
#include <vector>

namespace Cappuccino {

	class Ray {
	public:
		Ray(glm::vec3& camFront, glm::vec3& camPos);
		~Ray();

		glm::vec3 _rayDir;
		glm::vec3 _rayPos;
	};

}