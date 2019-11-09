#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
namespace Cappuccino {
	class HitBox {
	public:
		HitBox() = default;
		/*
		Purp:Constructor for Sphere Collider
		Pre: A Vec3 of where, relative to the rigidbody, the hitbox is and a float of the radius
		*/
		HitBox(glm::vec3& newPos, float newRadius);
		/*
		Purp:Constructor for Cube Collider
		Pre: A Vec3 of where, relative to the rigidbody, the hitbox is and a vec3 of the size of the Cube
		*/
		HitBox(glm::vec3& newPos, glm::vec3& newSize);
		/*
		Purp: This function checks if the hitbox is colliding with another hitbox
		Pre: Another hitbox object, a vec3 of its connected Rigid Body location, and our Rigid Body location
		Post: A boolean (True for collision)
		*/
		bool checkCollision(HitBox& other, glm::vec3& rigidLoc, glm::vec3& ourRigidLoc);
		/*
		Purp: For drawing the hitBox
		Pre: None
		Post: None
		*/
		void draw();
		/*
		Purp: check if a ray is intersecting the hitbox
		Req: A ray and a glm of the rigidbodies position
		returns: whether or not it is intersecting
		*/
		bool intersecting(const Ray& ray,glm::vec3&);

		void rotateBox(float angle);

		void rebindVBO();
		glm::vec3 _position = glm::vec3(0, 0, 0);
		float _radius = 0;
		float _scale = 1;
		glm::vec3 _size = glm::vec3(0, 0, 0);
		glm::vec3 _rotationAround = glm::vec3(0, 0, 0);

	protected:
		unsigned _VAO = 0;
		unsigned _VBO = 0;
		unsigned int _numVerts = 0;
	private:
		/*
		Purp: This function gathers the required distance data from a cube and sphere
		Pre: A vec3 of the spher's location, a vec3 of the box's position, a vec3 of the box's size
		Post: A float of the distance data
		*/
		float checkCircleBox(glm::vec3& circ, glm::vec3& boxPos, glm::vec3& boxSize);
		/*
		Purp: This function is used to get each axis distance data between two cubes
		Pre: A float of the first box's location, a float for it's size, a float for the second box's location, and a float for it's size
		Post: A boolean (true for collision)
		*/
		bool checkSize(float box1, float size1, float box2, float size2);
		/*
		Purp: This function checks the distance for each axis of a cube and sphere
		Pre: A float of the sphere's position, a float of the box's position, and a float of it's size
		Post: A boolean (true for collision)
		*/
		float checkDist(float circ, float boxPos, float boxSize);
	};

	enum class angle
	{
		x, y, z
	};
	class Capsule
	{
	public:
		/*
		Purp: This is the contructor for a capsule
		Pre: A vec3 of it's position relative to the rigidbody, a vec2 for the length(x) and thickness(y), and an enum of x,y,z for the orientation of the capsule
		Post: None
		*/
		Capsule(glm::vec3& pos, glm::vec2& size, angle orientation);
		/*
		Purp: This function checks if the hitbox is colliding with another hitbox
		Pre: Another hitbox object, a vec3 of its connected Rigid Body location, and our Rigid Body location
		Post: A boolean (True for collision)
		*/
		bool checkCollision(HitBox& other, glm::vec3& rigidLoc, glm::vec3& ourRigidLoc);
	protected:
		HitBox hitBox[3];
		angle _orientation = angle::x;
	};
}