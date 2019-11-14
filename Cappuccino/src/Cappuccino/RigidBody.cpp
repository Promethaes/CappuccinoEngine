#include "Cappuccino/RigidBody.h"
#include "Cappuccino/CappMacros.h"

namespace Cappuccino {

	glm::mat4 RigidBody::_projection = glm::mat4();
	glm::mat4 RigidBody::_view = glm::mat4();
	
	float Physics::gravity = -98.0f;
	float Physics::UniversalG = 6.67f * static_cast<float>(pow(10, -11));
	
	RigidBody::RigidBody(const glm::vec3& transformPosition, const glm::vec3& origin, const float mass, bool gravity)
		: _position(transformPosition), _mass(mass), _origin(origin), _grav(gravity) {}


	void RigidBody::update(const float dt, glm::mat4 model)
	{
		if (_grav)
			addAccel(glm::vec3(0.0f, Physics::gravity * dt, 0.0f));

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

		
		if(drawHitBox) {
			CAPP_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
			CAPP_GL_CALL(glDisable(GL_CULL_FACE));
			
			for(auto& hitBox : _hitBoxes) {
				hitBox.draw();
			}
			
			//CAPP_GL_CALL(glEnable(GL_CULL_FACE));
			CAPP_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}
		
	}

	void RigidBody::addAccel(const glm::vec3& force)
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
		if (_hitBoxes.empty() || other._hitBoxes.empty())
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

		return false;
	}

	bool RigidBody::checkCollision(HitBox other,glm::vec3 pos)
	{
		if (_hitBoxes.empty())
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

		return false;
	}

	void RigidBody::rotateRigid(float angle)
	{
		for(auto& hitBox : _hitBoxes) {
			hitBox.rotateBox(angle);
		}
	}

}
