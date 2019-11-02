#include "Cappuccino/RigidBody.h"
#include "Cappuccino/CappMacros.h"
namespace Cappuccino {
	float Physics::gravity = -9.8f;
	float Physics::UniversalG = 6.67f * (float)(pow(10, -11));
	Cappuccino::RigidBody::RigidBody(const glm::vec3& transformPosition, const glm::vec3& origin, const float mass, bool gravity)
		: _mass(mass), _position(transformPosition), _origin(origin), _grav(gravity) {}


	void Cappuccino::RigidBody::update(float dt, glm::mat4 model)
	{
		if (_grav && _position.y != 0)
			//	if (_accel.y <= _accelCap.y)
			addAccel(glm::vec3(0, Physics::gravity * dt, 0));
		//	else
		//		setAccel(glm::vec3(_accel.x, _accelCap.y, _accel.x));
		/*else
		{
			setAccel(glm::vec3(_accel.x, 0, _accel.z));
			setVelocity(glm::vec3(_accel.x, 0, _accel.z), dt);
		}
		*/

		addVelocity(_accel * dt);
		//if (_vel.y > _velCap.y)
		//{
		//	setVelocity(glm::vec3(_vel.x, _velCap.y, _vel.z), dt);
		//}
		addPosition(_vel * dt);


		glm::mat4 newModel(1.0f);
		newModel[3].x = model[3].x;
		newModel[3].y = model[3].y;
		newModel[3].z = model[3].z;

		_shader.use();
		_shader.loadModelMatrix(newModel);
		_shader.setUniform("view",_view);
		_shader.setUniform("projection", _projection);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (drawHitBox)
			for (unsigned i = 0; i < _hitBoxes.size(); i++)
			{
				_hitBoxes[i].draw();
			}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Cappuccino::RigidBody::addAccel(const glm::vec3& force)
	{
		_accel += force;
	}

	void RigidBody::setAccel(const glm::vec3& force)
	{
		_accel = force;
	}

	void RigidBody::setVelocity(const glm::vec3& force)
	{
		_vel = force;
	}

	void RigidBody::addVelocity(const glm::vec3& force)
	{
		_vel += force;
	}

	void RigidBody::addPosition(const glm::vec3& force)
	{
		_position += force;
	}

	bool RigidBody::intersecting(const Ray& ray)
	{
		for (unsigned i=0;i<_hitBoxes.size();i++)
		{
			if (_hitBoxes[i].intersecting(ray, _position))
				return true;
		}
		return false;
	}

	bool RigidBody::checkCollision(RigidBody& other)
	{
		for (unsigned i=0;i<_hitBoxes.size();i++)
		{
			for (unsigned n = 0; n < other._hitBoxes.size(); n++)
			{
				if (_hitBoxes[i].checkCollision(other._hitBoxes[n],other._position,_position))
					return true;
			}
		}
		return false;
	}

}
glm::mat4 Cappuccino::RigidBody::_projection =glm::mat4();
glm::mat4 Cappuccino::RigidBody::_view =  glm::mat4();