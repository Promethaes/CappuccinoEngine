#include "Cappuccino/RigidBody.h"
#include "Cappuccino/CappMacros.h"
namespace Cappuccino {
	float Physics::gravity = -98.0f;
	float Physics::UniversalG = 6.67f * (float)(pow(10, -11));
	Cappuccino::RigidBody::RigidBody(const glm::vec3& transformPosition, const glm::vec3& origin, const float mass, bool gravity)
		: _mass(mass), _position(transformPosition), _origin(origin), _grav(gravity) {}


	void Cappuccino::RigidBody::update(float dt, glm::mat4 model)
	{
		if (_grav)
			addAccel(glm::vec3(0.0f, Physics::gravity*dt, 0.0f));

		addVelocity(_accel*dt);
		addPosition(_vel*dt);

		glm::mat4 newModel(1.0f);
		newModel[3].x = model[3].x;
		newModel[3].y = model[3].y;
		newModel[3].z = model[3].z;

		_shader.use();
		_shader.loadModelMatrix(newModel);
		_shader.setUniform("view",_view);
		_shader.setUniform("projection", _projection);

		CAPP_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		if (drawHitBox)
			for (unsigned i = 0; i < _hitBoxes.size(); i++)
			{
				//_hitBoxes[i].draw();
			}
		CAPP_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
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
		if (!_hitBoxes.size()||!other._hitBoxes.size())
			return false;
		if (_hitBoxes.size() > 1)
		{
			if (_hitBoxes[0].checkCollision(other._hitBoxes[0], other._position, _position))
				for (unsigned i = 1; i < _hitBoxes.size(); i++)
				{
					for (unsigned n = 1; n < other._hitBoxes.size(); n++)
					{
						if (_hitBoxes[i].checkCollision(other._hitBoxes[n], other._position, _position))
							return true;
					}
				}
		}
		else if (_hitBoxes[0].checkCollision(other._hitBoxes[0], other._position, _position))
			return true;
		else
			return false;
	}

	bool RigidBody::checkCollision(HitBox other,glm::vec3 pos)
	{
		if (_hitBoxes.size() == 0)
			return false;
		if (_hitBoxes.size() > 1)
		{
			if (_hitBoxes[0].checkCollision(other, pos, _position))
				for (unsigned i = 1; i < _hitBoxes.size(); i++)
					if (_hitBoxes[i].checkCollision(other, pos, _position))
						return true;
		}
		else if (_hitBoxes[0].checkCollision(other,pos,_position))
			return true;
		else
			return false;
	}

}
glm::mat4 Cappuccino::RigidBody::_projection =glm::mat4();
glm::mat4 Cappuccino::RigidBody::_view =  glm::mat4();
