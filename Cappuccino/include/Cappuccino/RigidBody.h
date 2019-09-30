#pragma once
#include <glm/glm.hpp>
namespace Cappuccino {
	class RigidBody	{
	public:
		RigidBody();
		/*
		Purp: The update function is called each update to update all parts of the rigidBody
		Pre: A float of the time between frames,
		Post: None
		*/
		void update(float dt);
		/*
		Purp: This function updates all parts of the collision boxes positions
		Pre: A vector representing the position of the center of mass of the rigidbody
		Post:None
		*/
		void updatePosition(glm::vec3 &newPos);
		/*
		Purp: This function updates all parts of the collision boxes rotations
		Pre:A vector representing the rotation of the center of mass of the rigidbody
		Post:None
		*/
		void updateRotation(glm::vec3& newRot);
		/*
		Purp:
		Pre:
		Post:None
		*/
		void addForce(glm::vec3 force);

		glm::vec3 getPosition() { return _position;}
		glm::mat4 getRotation() { return _rotateMat;}
	private:
		unsigned _mass = 1;
		bool _moveable = false;
		bool _gravity = false;
		bool _collision = false;
		glm::vec3 _acceleration = glm::vec3(0, 0, 0);
		glm::vec3 _accelerationCap = glm::vec3(0, 0, 0);
		glm::vec3 _position{ 0,0,0 };
		glm::mat4 _rotateMat{ 1.0f };

	};
}
