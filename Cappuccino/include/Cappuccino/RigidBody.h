#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Cappuccino/HitBox.h"
#include "Cappuccino/ShaderProgram.h"
#include <math.h>


namespace Cappuccino {
	
	struct collisionData {
		HitBox ourBox;
		HitBox otherBox[8];
		unsigned collisions = 0;
	};

	class Physics {
	public:

		//by default 9.8 m/s^2
		static float gravity;
		//by default 6.67x10^-11
		static float UniversalG;


	};
	
	class RigidBody	{
	public:
		RigidBody(const glm::vec3& transformPosition, const glm::vec3& origin = glm::vec3(0.0f, 0.0f, 0.0f), const float mass = 1,bool gravity = false);
		/*
		Purp: The update function is called each update to update all parts of the rigidBody
		Pre: A float of the time between frames, a mat4 for the hitBox shader's model
		Post: None
		*/
		void update(float dt, glm::mat4 model);

		void setViewProjMat(glm::mat4 &view, glm::mat4 &projection) { _view = view; _projection = projection; };
		void setAccel(const glm::vec3& force);
		void addAccel(const glm::vec3& force);
		void setVelocity(const glm::vec3& force);
		void addVelocity(const glm::vec3& force);
		void addPosition   (const glm::vec3& force);
		bool getGrav() { return _grav; }
		void setGrav(bool yn) { _grav = yn; }
		/*
		Purp: Check if a ray is intersecting with cube hitboxes
		Req: A ray
		returns: whether or not it is intersecting
		*/
		bool intersecting(const Ray& ray);
		/*
		Purp: Check if two rigidbodies are colliding
		Pre: Another RigidBody
		Post: A boolean (True for Collision)
		*/
		bool checkCollision(RigidBody& other);
		/*
		Purp: Check if two rigidbodies will collide
		Pre: Another RigidBody
		Post: A boolean (True if they Collide)
		*/
		bool willCollide(RigidBody& other, glm::vec3 direction, float dt);
		/*
		Purp: To check collision between this rigidbody and single hitbox
		Pre: a hitbox
		Post: a boolean true if colliding
		*/
		bool checkCollision(HitBox other, glm::vec3 pos);
		/*
		Purp: To check if the rigidbody will collide with a hitbox
		Pre: a hitbox
		Post: a boolean true if they will collide
		*/
		bool willCollide(HitBox other, glm::vec3 pos, glm::vec3 direction, float dt);
		/*
		Purp: to get the data from a collision
		Pre: another RigidBody
		Post: a struct of the collisions data
		*/
		collisionData getData(RigidBody& other);
		glm::mat4 getRotation() { return _rotateMat;}
		std::vector<HitBox> _hitBoxes;
		glm::vec3 _position;
		glm::vec3 _accel{ 0,0,0 };
		glm::vec3 _vel{ 0,0,0 };
		glm::vec3 _accelCap{ 20, 20, 20 };
		glm::vec3 _velCap{ 20, 20, 20 };
		bool drawHitBox = true;
		static glm::mat4 _view;
		static glm::mat4 _projection;
		Shader _shader{ "hitBox.vert","hitBox.frag" };
		void rotateRigid(float angle);
		bool _moveable = false;
		bool _canTouch= true;
	private:
		bool _collision = false;
		glm::vec3 _scale{ 0 };
		glm::mat4 _rotateMat{ 1.0f };
		float _mass = 1;
		glm::vec3 _origin;
		bool _grav = false;
	};
}
