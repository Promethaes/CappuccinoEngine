#include "Cappuccino/RigidBody.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

namespace Cappuccino {

	glm::mat4 RigidBody::_projection = glm::mat4();
	glm::mat4 RigidBody::_view = glm::mat4();
	
	float Physics::gravity = -98.0f;
	float Physics::UniversalG = 6.67f * static_cast<float>(pow(10, -11));
	
	

	Shader RigidBody::_shader;
	bool RigidBody::drawHitBox = false;
	RigidBody::RigidBody(const glm::vec3& transformPosition, const float mass, const bool gravity)
		: _position(transformPosition), _grav(gravity), _mass(mass) {

		char* vert = R"(#version 420 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

		char* frag = R"(#version 420 core

out vec4 outColour;
uniform vec4 ourColour;

void main()
{
	outColour = ourColour;
})";

		_shader = Shader(true,vert,frag);
	}


	void RigidBody::update(const float dt)
	{
		//physics calculations for movement
		addPosition(_vel * dt);
		addVelocity(_accel*dt);
		
		if (_grav)
			addAccel(glm::vec3(0.0f, Physics::gravity * dt, 0.0f));	

		if(_velCap<glm::length(_vel))
			_vel = glm::normalize(_vel)*_velCap;

		if (_accelCap < glm::length(_accel))
			_accel = glm::normalize(_accel) * _accelCap;

	}

	void RigidBody::draw()
	{
		_shader.use();
		_shader.setUniform("view", _view);//shader uniforms
		_shader.setUniform("projection", _projection);
		_shader.setUniform("ourColour",_shaderColour);

		if (drawHitBox) {
			CAPP_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));//wireframe mode
			CAPP_GL_CALL(glDisable(GL_CULL_FACE));
			for (auto& hitBox : _hitBoxes) {
				glm::mat4 newModel = hitBox._rotationMatrix;
				newModel[3].x = _tempModel[3].x+_position.x;
				newModel[3].y = _tempModel[3].y+_position.y;
				newModel[3].z = _tempModel[3].z+_position.z;
				_shader.loadModelMatrix(newModel);
				hitBox.draw();//drawing hitboxes
				
			}
			CAPP_GL_CALL(glEnable(GL_CULL_FACE));
			CAPP_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));//set back to normal draw mode
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
		if (_hitBoxes.size() == 0)
			return false;
		else if (_hitBoxes.size() == 1) {
			if (_hitBoxes[0].intersecting(ray, _position))
				return true;
		}
		else
			if(_hitBoxes[0].intersecting(ray, _position))
				for (unsigned i=1;i<_hitBoxes.size();i++)
				{
					if (_hitBoxes[i].intersecting(ray, _position))
						return true;
				}
		return false;
	}

	bool RigidBody::checkCollision(RigidBody& other)
	{
		if (_hitBoxes.empty() || other._hitBoxes.empty())//if no hitboxes
			return false;
		
		if (_hitBoxes.size() > 1){
			if (_hitBoxes[0].checkCollision(other._hitBoxes[0], other._position, _position))//if the two bounding boxes collide they might be colliding
				for (unsigned i = 1; i < _hitBoxes.size(); i++){
					for (unsigned n = 1; n < other._hitBoxes.size(); n++){
						if (_hitBoxes[i].checkCollision(other._hitBoxes[n], other._position, _position))//if any hitboxes are colliding we know we are colliding
							return true;
					}
				}
		}
		else if (_hitBoxes[0].checkCollision(other._hitBoxes[0], other._position, _position))//if they both only have one hitbox
			return true;

		return false;
	}

	bool RigidBody::willCollide(RigidBody& other, glm::vec3 direction, float dt)
	{
		if (_hitBoxes.empty() || other._hitBoxes.empty())//if no hitboxes
			return false;
		glm::vec3 tempVel = _vel;
		glm::vec3 tempPos = _position;
		for (unsigned i = 0; i < 3; i++) {//for all three dimensions
			tempVel[i] *= direction[i];
		}
		tempPos += (tempVel * dt);//our future position
		if (_hitBoxes.size() > 1){
			if (_hitBoxes[0].checkCollision(other._hitBoxes[0], other._position, tempPos)){//now check collision
				for (unsigned i = 1; i < _hitBoxes.size(); i++){
					for (unsigned n = 1; n < other._hitBoxes.size(); n++){
						if (_hitBoxes[i].checkCollision(other._hitBoxes[n], other._position, tempPos))//if they will collide
							return true;
					}
				}
			}
				
		}
		else if (_hitBoxes[0].checkCollision(other._hitBoxes[0], other._position, tempPos))//if only one hitbox each
			return true;
		return false;
	}

	bool RigidBody::checkCollision(HitBox other,glm::vec3 pos)
	{
		if (_hitBoxes.empty())//if we have no hitboxes
			return false;
		if (_hitBoxes.size() > 1)
		{
			if (_hitBoxes[0].checkCollision(other, pos, _position))//if our bounding box is colliding
				for (unsigned i = 1; i < _hitBoxes.size(); i++)//check rest of boxes
					if (_hitBoxes[i].checkCollision(other, pos, _position))//if any are colliding we are colliding
						return true;
		}
		else if (_hitBoxes[0].checkCollision(other,pos,_position))//if we only have one hitbox as well
			return true;

		return false;
	}

	bool RigidBody::willCollide(HitBox other, glm::vec3 pos, glm::vec3 direction, float dt)
	{
		if (_hitBoxes.empty())//if we have not hitboxes
			return false;
		glm::vec3 temp = _vel;
		glm::vec3 tempPos = _position;
		for (unsigned i = 0; i < 3; i++){//all dimensions
			temp[i] *= direction[i];
			tempPos[i] *= direction[i];
		}
		tempPos += temp*dt;
			
		
		if (_hitBoxes.size() > 1){
			if (_hitBoxes[0].checkCollision(other, pos, tempPos))//if bounding box is colliding
				for (unsigned i = 1; i < _hitBoxes.size(); i++)
					if (_hitBoxes[i].checkCollision(other, pos, tempPos))//if any boxes touch
						return true;
		}
		else if (_hitBoxes[0].checkCollision(other, pos, tempPos))//if we only have one hitbox
			return true;

		return false;
	}

	void RigidBody::rotateRigid(float angle)
	{
		for(auto& hitBox : _hitBoxes){
			hitBox.rotateBox(angle);//all hitboxes will change shape and position
		}
	}

}
