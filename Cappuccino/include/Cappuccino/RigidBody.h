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
		void addForce   (const glm::vec3& force, float dt);
		bool getGrav() { return _grav; }
		void setGrav(bool yn) { _grav = yn; }
		/*
		Purp: check if a ray is intersecting with cube hitboxes
		Req: A ray
		returns: whether or not it is intersecting
		*/
		bool intersecting(const Ray& ray);
		bool checkCollision(RigidBody& other);

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
	private:
		bool _collision = false;
		glm::vec3 _scale{ 0 };
		glm::mat4 _rotateMat{ 1.0f };
		float _mass = 1;
		glm::vec3 _origin;
		bool _grav = false;

		
	};
}
