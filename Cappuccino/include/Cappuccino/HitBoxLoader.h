#pragma once
#include <string>
#include <vector>
#include "HitBox.h"
#include <glm/glm.hpp>
#include <fstream>
namespace Cappuccino
{
	class HitBoxLoader
	{
	public:
		HitBoxLoader(const char* filename);
		float findRadius();
		glm::vec3 findBox();
		glm::vec3 findCenter();
		std::vector <HitBox> boxes;
	protected:


	private:

		std::vector <glm::vec3> tempVerts;
	};
}