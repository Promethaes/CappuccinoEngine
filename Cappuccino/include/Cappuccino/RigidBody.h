#pragma once
#include <glm/glm.hpp>
namespace Cappuccino {
	class RigidBody	{
	public:
		RigidBody();
		void update(float dt);
		void addForce(glm::vec3 force);
	private:
		unsigned _mass = 1;
		bool _moveable = false;
		bool _gravity = false;
		bool _collision = false;
		glm::vec3 _force = glm::vec3(0,0,0);
		glm::vec3 _acceleration = glm::vec3(0, 0, 0);
		glm::vec3 _accelerationCap = glm::vec3(0, 0, 0);

	};
}
