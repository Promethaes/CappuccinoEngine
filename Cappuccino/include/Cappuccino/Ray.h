#pragma once
#include "glm/common.hpp"
#include <vector>

namespace Cappuccino {

	class Ray {
	public:
		Ray(glm::vec3& camFront, glm::vec3& camPos);
		~Ray();

		glm::vec3* _rayDir = nullptr;
		glm::vec3* _rayPos = nullptr;
	};
	class RayBox {
	public:
		RayBox(const glm::vec3& min, const glm::vec3& max);

		/*
		Purp: check if a ray is intersecting the box
		Req: A ray
		returns: whether or not it is intersecting
		*/
		bool intersecting(const Ray& ray);

	private:
		std::vector<glm::vec3> _bounds;

	};
}