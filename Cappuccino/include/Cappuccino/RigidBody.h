#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Cappuccino/HitBox.h"
namespace Cappuccino {
	class RigidBody	{
	public:
		/*
		Purp:This update function is called each update and will update the position,scale, and rotation of the rigid body
		Pre:a float of the time between frames, a vec3 of the position of the rigidbody, a mat4 quanternion of its rotation, and a vec3 of its scale
		Post: None
		*/
		void update(float dt,glm::vec3 &pos,glm::mat4 &rotation,glm::vec3 &scale);
		/*
		Purp: The update function is called each update to update all parts of the rigidBody
		Pre: A float of the time between frames,
		Post: None
		*/
		void update(float dt);
		/*
		Purp: This function updates all parts of the collision boxes positions
		Pre: A vec3 representing the position of the center of mass of the rigidbody
		Post: None
		*/
		void setPosition(glm::vec3 &newPos);
		/*
		Purp: This function updates all parts of the collision boxes rotations
		Pre:A vec3 representing the rotation of the center of mass of the rigidbody
		Post: None
		*/
		void setRotation(glm::vec3& newRot);
		/*
		Purp: This function will update the scale of the rigid body
		Pre: A vec3 representing the scale in the 3 dimensions
		Post: None
		*/
		void updateScale(glm::vec3 &newScale);
		/*
		Purp:
		Pre:
		Post: None
		*/
		void addForce(glm::vec3 force);


        
		glm::vec3 getPosition() { return _position;}
		glm::mat4 getRotation() { return _rotateMat;}
		std::vector<HitBox> hitBox;
	private:
		unsigned _mass = 1;
		bool _moveable = false;
		bool _gravity = false;
		bool _collision = false;
		glm::vec3 _acceleration{ 0 };
		glm::vec3 _accelerationCap{ 0 };
		glm::vec3 _position{ 0 };
		glm::vec3 _scale{ 0 };
		glm::mat4 _rotateMat{ 1.0f };
		


	};
}

		
		