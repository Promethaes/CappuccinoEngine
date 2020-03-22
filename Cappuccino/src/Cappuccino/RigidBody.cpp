#include "Cappuccino/RigidBody.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

namespace Cappuccino {
	
	float Physics::gravity = -98.0f;
	float Physics::UniversalG = 6.67f * static_cast<float>(pow(10, -11));
	
	

	RigidBody::RigidBody(const glm::vec3& transformPosition, const float mass, const bool gravity)
		: _position(transformPosition), _grav(gravity), _mass(mass) 
	{}


	void RigidBody::update(const float dt)
	{
		//physics calculations for movement
		addPosition(_vel * dt);
		addVelocity(_accel*dt);
		
		if (_grav)
			addAccel(glm::vec3(0.0f, Physics::gravity * dt, 0.0f));	

		if (_vel.x >= 0)
			_vel.x = fminf(_velCap.x, _vel.x);
		else
			_vel.x = fmaxf(-_velCap.x, _vel.x);
		if (_vel.y >= 0)
			_vel.y = fminf(_velCap.y, _vel.y);
		else
			_vel.y = fmaxf(-_velCap.y, _vel.y);
		if (_vel.z >= 0)
			_vel.z = fminf(_velCap.z, _vel.z);
		else
			_vel.z = fmaxf(-_velCap.z, _vel.z);

		if (_accelCap < glm::length(_accel))
			_accel = glm::normalize(_accel) * _accelCap;

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

	glm::vec3 RigidBody::getFirstInteresect(const Ray& ray)
	{
		glm::vec3 nearestIntersectPoint = glm::vec3(0.0f);//possible edge case scenario
		float smallestLength = 0.0f;//base length

		if (_hitBoxes.size() == 1) {//if there is only one hitbox
			if (_hitBoxes[0].intersecting(ray, _position)) {//if that hitbox is touched
				glm::vec3 tempRayIntersect = _hitBoxes[0].getIntersectPoint(ray, _position);//find where
				float tempRayIntersectLength = glm::length(tempRayIntersect);//the length of the vector
				if ((tempRayIntersectLength < smallestLength || smallestLength == 0.0f) //if the length is smaller than the current length or we have the default length
					&& tempRayIntersect != glm::vec3(0.0f)) {//and it's not a false hit
					smallestLength = tempRayIntersectLength;//we have a new smallest length
					nearestIntersectPoint = tempRayIntersect;//we have the closest point to the ray origin
				}
				else {
					std::cout << "RigidBody::GetFirstIntersect::This should never happen. One hitbox and no collision "<<std::endl;
				}
			}
		}
		else if (_hitBoxes.size() > 1) {//if more than one hitbox
			for (unsigned i = 1; i < _hitBoxes.size(); i++) {
				if (_hitBoxes[i].intersecting(ray, _position)) {
					glm::vec3 tempRayIntersect = _hitBoxes[i].getIntersectPoint(ray, _position);
					float tempRayIntersectLength = glm::length(tempRayIntersect);
					if ((tempRayIntersectLength < smallestLength || smallestLength == 0.0f) && tempRayIntersect != glm::vec3(0.0f)) {
						smallestLength = tempRayIntersectLength;
						nearestIntersectPoint = tempRayIntersect;
					}
				}
			}
		}
		else
		{
			//std::cout <<"RigidBody::GetFirstIntersect::Evyn, this shouldn't happen, There are no hitboxes on a collided object"<<std::endl;
		}
		
		//if (nearestIntersectPoint == glm::vec3(0.0f))
		//	std::cout << "RigidBody::GetFirstIntersect::THIS SHOULD NOT HAPPEN! Tell EVYN ray Intersect hit an edge case!\n";
		return nearestIntersectPoint;
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
