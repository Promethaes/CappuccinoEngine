#include "Cappuccino/Ray.h"
#include "Cappuccino/CappMacros.h"

using namespace Cappuccino;

RayBox::RayBox(const glm::vec3& min, const glm::vec3& max)
{
	_bounds.push_back(min);
	_bounds.push_back(max);
}

bool RayBox::intersecting(const Ray& ray)
{
	//	https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	float txMin, txMax, tyMin, tyMax, tzMin, tzMax;
	auto inverseDir = 1.0f / *ray._rayDir;

	std::vector<int> sign;

	sign.push_back((inverseDir.x < 0));
	sign.push_back((inverseDir.y < 0));
	sign.push_back((inverseDir.z < 0));

	txMin = (_bounds[sign[0]].x     - ray._rayPos->x) * inverseDir.x;
	txMax = (_bounds[1 - sign[0]].x - ray._rayPos->x) * inverseDir.x;
	tyMin = (_bounds[sign[1]].y     - ray._rayPos->y) * inverseDir.y;
	tyMax = (_bounds[1 - sign[1]].y - ray._rayPos->y) * inverseDir.y;
	tzMin = (_bounds[sign[2]].z     - ray._rayPos->z) * inverseDir.z;
	tzMax = (_bounds[1 - sign[2]].z - ray._rayPos->z) * inverseDir.z;

	if ((txMin > tyMax) || (tyMin > txMax))
		return false;
	if (tyMin > txMin)
		txMin = tyMin;
	if (tyMax < txMax)
		txMax = tyMax;
	if ((txMin > tzMax) || (tzMin > txMax))
		return false;

	return true;
}

Ray::Ray(glm::vec3& camFront, glm::vec3& camPos)
{
	_rayDir = &camFront;
	_rayPos = &camPos;
}

Cappuccino::Ray::~Ray()
{

}
