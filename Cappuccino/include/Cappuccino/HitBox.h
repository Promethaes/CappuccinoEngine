#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Ray.h"
#include <cmath>
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
		Purp: Constructor for an OBB collider
		Pre: A Vec3 of it's position, a vec3 for it's size, a mat 4x4 rotation matrix
		*/
		HitBox(glm::vec3& newPos,glm::vec3& newSize, glm::mat4 & newRotation);
		/*
		Purp: This function checks if the hitbox is colliding with another hitbox
		Pre: Another hitbox object, a vec3 of its connected Rigid Body location, and our Rigid Body location
		Post: A boolean (True for collision)
		*/
		bool checkCollision(HitBox& other, glm::vec3& rigidLoc, glm::vec3& ourRigidLoc);
		/*
		Purp: check if a ray is intersecting the hitbox
		Pre: A ray and a glm vec3 of the rigidbody position
		Post: whether or not it is intersecting
		*/
		bool intersecting(const Ray& ray,glm::vec3&);
		/*
		Purp: to find the closest point on the ray where it intersects with the hitbox
		Pre: A ray and glm vec3 of the rigidbody position
		Post: a glm vec3 of the closest intersection point
		*/
		glm::vec3 getIntersectPoint(const Ray& ray,glm::vec3& position);
		/*
		Purp: Rotate the hitbox
		Pre: an angle of rotation
		Post: None
		*/
		void rotateBox(float angle);
		glm::vec3 _position = glm::vec3(0, 0, 0);
		
		glm::vec3 _size = glm::vec3(0, 0, 0);
		glm::mat4 _rotationMatrix = glm::mat4(1.0f);

		bool _OBB = false;
		float _radius = 0.0f;
	protected:

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
		/*
		Purp: To see if we can stick a plane between the two OBB boxes
		Pre: 
		Post: 
		*/
		bool checkPlane(glm::vec3 dist,glm::vec3 plane, HitBox other);
	};
}