#include "Cappuccino/RigidBody.h"
#include "Cappuccino/CappMacros.h"
namespace Cappuccino {
	float Physics::gravity = -0.1f;
	float Physics::UniversalG = 6.67f * (pow(10, -11));
	Cappuccino::RigidBody::RigidBody(const glm::vec3& transformPosition, const glm::vec3& dimensions, const glm::vec3& origin, const float mass, bool gravity)
		:_dimensions(dimensions), _mass(mass), _position(transformPosition), _origin(origin), _grav(gravity) {}

	void Cappuccino::RigidBody::update(float dt)
	{
		if (_grav && _position.y > 0)
			_vel += (glm::vec3(0, Physics::gravity, 0)) * dt;
		else
			_vel.y = 0;
		_vel += _accel * dt;
		_position += _vel * dt;

	}

	void Cappuccino::RigidBody::addAccel(const glm::vec3& force, float dt)
	{
		_accel += (force / _mass) * dt;
	}

	void RigidBody::setAccel(const glm::vec3& force, float dt)
	{
		_accel = (force / _mass) * dt;
	}

	void RigidBody::setVelocity(const glm::vec3& force, float dt)
	{
		_vel = (force / _mass) * dt;
	}

}