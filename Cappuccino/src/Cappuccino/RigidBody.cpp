#include "Cappuccino/RigidBody.h"

void Cappuccino::RigidBody::update(float dt)
{
	_acceleration.x += _force.x / _mass;
	_acceleration.y += _force.y / _mass;
	_acceleration.z += _force.z / _mass;

}
