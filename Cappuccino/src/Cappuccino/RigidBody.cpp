#include "Cappuccino/RigidBody.h"

void Cappuccino::RigidBody::update(float dt)
{
	for (int i=0;i<hitBox.size();i++)
	{
		hitBox[i];
	}

}

void Cappuccino::RigidBody::updatePosition(glm::vec3& newPos)
{
	_position = newPos;
}

void Cappuccino::RigidBody::addForce(glm::vec3 force)
{
	_acceleration.x += force.x / _mass;
	_acceleration.y += force.y / _mass;
	_acceleration.z += force.z / _mass;
}
