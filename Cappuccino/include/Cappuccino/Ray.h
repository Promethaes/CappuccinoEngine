#pragma once
#include "glm/common.hpp"
#include "Cappuccino/RigidBody.h"

namespace Cappuccino {
	class Ray {
		friend RigidBody;
		friend HitBox;
	public:
		Ray(glm::vec3& camFrontVec, glm::vec3& camPosition);


		bool checkPointingAt(RigidBody& body);
		glm::vec3* getRayVec() const { return _rayDir; }
		void setRayVec(const glm::vec3& newRayVec) { *_rayDir = newRayVec; }
	private:
		std::vector<unsigned> _pointingAtIndex = {};
		bool _pointingAtHitbox = false;
		glm::vec3* _rayDir;
		glm::vec3* _rayOrigin;
	};
}