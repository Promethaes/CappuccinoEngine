#include "Cappuccino/RigidBody.h"

void Cappuccino::RigidBody::update(float dt)
{

}

void Cappuccino::RigidBody::addForce(glm::vec3 force)
{
	_acceleration.x += force.x / _mass;
	_acceleration.y += force.y / _mass;
	_acceleration.z += force.z / _mass;
}
