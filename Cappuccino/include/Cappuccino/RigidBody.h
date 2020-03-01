#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Cappuccino/HitBox.h"
#include "Cappuccino/ShaderProgram.h"
#include <math.h>


namespace Cappuccino {
	
	class Physics {
	public:

		//by default 9.8 m/s^2
		static float gravity;
		//by default 6.67x10^-11
		static float UniversalG;


	};
	
	class RigidBody	{
	public:
		RigidBody(const glm::vec3& transformPosition, const float mass = 1,bool gravity = false);
		/*
		Purp: The update function is called each update to update all parts of the rigidBody
		Pre: A float of the time between frames, a mat4 for the hitBox shader's model
		Post: None
		*/
		void update(float dt);
		/*
		setter
		*/
		void setAccel(const glm::vec3& force);
		/*
		setter
		*/
		void addAccel(const glm::vec3& force);
		/*
		setter
		*/
		void setVelocity(const glm::vec3& force);
		/*
		setter
		*/
		void addVelocity(const glm::vec3& force);
		/*
		setter
		*/
		void addPosition   (const glm::vec3& force);
		/*
		getter
		*/
		bool getGrav() { return _grav; }
		/*
		setter
		*/
		void setGrav(bool yn) { _grav = yn; }
		/*
		Purp: Check if a ray is intersecting with cube hitboxes
		Req: A ray
		returns: whether or not it is intersecting
		*/
		bool intersecting(const Ray& ray);

		/*
		Purp: to see what hitbox is the closest to where the ray is
		Pre: the ray 
		Post: a vec3 of the position of the closest hitbox origin
		*/
		glm::vec3 getFirstInteresect(const Ray& ray);

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
		Getter
		*/
		glm::mat4 getRotation() { return _rotateMat;}
		/*
		Purp: This function will rotate the rigidbody and all of it's data on 90 degree increments
		Pre: a float for the angle rotated
		Post: None
		*/
		void rotateRigid(float angle);

		/*
		movement variables
		*/
		glm::vec3 _position;
		glm::vec3 _accel{ 0,0,0 };
		glm::vec3 _vel{ 0,0,0 };
		float _accelCap = 200;
		float _velCap = 200;
		bool _grav = true;


		std::vector<HitBox> _hitBoxes;
		

		/*
		Collision variables
		*/
		bool _collision = false;
		std::string myType = "";//objects that should not collide with eachother should be called the same type (bullet/loot)
		bool _moveable = false;//if this object can move around the scene(enemy/bullet/player)
		bool _canTouch= true;//if the object can touch other objects(almost everything)
		bool _bounce = false;//if the object should bounce instead of stopping (grenade?)
		bool _creature = false;//if the object is a creature (enemy/player)
		/*
		projectile collision variables
		*/
		bool _projectile = false;//if the object is a bullet of some sort
		bool _hitWall = false;//for bullets only
	private:
		glm::mat4 _tempModel{ 1.0f };
		glm::vec3 _scale{ 0 };
		glm::mat4 _rotateMat;

		float _mass = 1;
	};
}
