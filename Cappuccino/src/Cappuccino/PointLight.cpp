#include "Cappuccino/PointLight.h"
#include "Cappuccino/ResourceManager.h"


Cappuccino::PointLight::PointLight(const glm::vec3& position, const glm::vec3& colour)
	:_pos(position),_col(colour)
{
}
