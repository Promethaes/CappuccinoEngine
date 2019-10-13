#include "Cappuccino/Ray.h"
#include "Cappuccino/CappMacros.h"
#include <algorithm>

namespace Cappuccino {
	Ray::Ray(glm::vec3& camFrontVec,glm::vec3& camPosition)
	{
		_rayDir = &camFrontVec;
		_rayOrigin = &camPosition;
	}

	bool Ray::checkPointingAt(RigidBody& body)
	{
		glm::vec3 tempVec = *_rayDir;
		if (std::abs(tempVec.x) > std::abs(tempVec.y) && std::abs(tempVec.x) > std::abs(tempVec.z))
			tempVec.x *= 100;
		else if (std::abs(tempVec.y) > std::abs(tempVec.x) && std::abs(tempVec.y) > std::abs(tempVec.z))
			tempVec.y *= 100;
		else if (std::abs(tempVec.z) > std::abs(tempVec.x) && std::abs(tempVec.z) > std::abs(tempVec.y))
			tempVec.z *= 100;

		for (unsigned i = 0; i < body.hitBox.size(); i++) {
			if ((tempVec.x <= (body.hitBox[i]._position.x + (body.hitBox[i]._size.x / 2.0f)) && tempVec.x >= (body.hitBox[i]._position.x - (body.hitBox[i]._size.x / 2.0f))) &&
				(tempVec.y <= (body.hitBox[i]._position.y + (body.hitBox[i]._size.y / 2.0f)) && tempVec.y >= (body.hitBox[i]._position.y - (body.hitBox[i]._size.y / 2.0f))) &&
				(tempVec.z <= (body.hitBox[i]._position.z + (body.hitBox[i]._size.z / 2.0f)) && tempVec.z >= (body.hitBox[i]._position.z - (body.hitBox[i]._size.z / 2.0f)))) {
				_pointingAtHitbox = true;
				CAPP_PRINT_N("POINTING");
				_pointingAtIndex.push_back(i);
			}
		}

		return false;
	}
}