#include "Cappuccino/Game Elements/Ray.h"
#include "Cappuccino/Core/CappMacros.h"

using namespace Cappuccino;

Ray::Ray(glm::vec3& camFront, glm::vec3& camPos)
{
	_rayDir = camFront;
	_rayPos = camPos;
}

Cappuccino::Ray::~Ray()
{

}
